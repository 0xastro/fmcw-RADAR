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
  *   @file     mmwavelib_cfarcaall_float.c
  *
  *   @brief    This file contains routines related to CFAR CA(cell average),
  *             CACC (cell accumulation) SO, GO peak detection processing, using
  *             floating-point signal power as input signal.
*/
#include <c6x.h>
#include <ti/alg/mmwavelib/include/mmwavelib_c674_emulate_c66_intrinsics.h>
#include <stdint.h>
#include "mmwavelib_detection_float.h"
#include "mmwavelib_detection.h"

/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_cfarfloat_caall
 *
 * \par
 * <b>Description</b>  :    Performs a regular Cell Averaging (CFAR-CA), Cell Accumulation (CFAR-CACC), Cell SO (CFAR-SO), 
 *                          and Cell GO CFAR (CFAR-GO) based on floating point signal power. The first CUT index is leftSkipLen.
 *                          and its left noise samples are the samples from leftSkipLen+guardLen to leftSkipLen+guardLen+guardLen+searchLen- 1.  
 *                          The last CUT index is fftSize-leftSkipLen, and its right noise samples are the samples from 
 *                          fftSize-rightSkipLen-guardLen-searchLen-rightSkipLen to fftSize-rightSkipLen-rightSkipLen -1.
 *                          
 * @param        [in]       InputPower   : input signal power array (floating point)
 * @param        [in]       fftSize      : number of elements in the InputPower
 * @param        [in]       cfarType     : 0-CFAR-CA, 1-CFAR-CAGO, 2-CFAR-SO, 3-CFAR-CACC
 * @param        [in]       guardLen     : one sided guard length
 * @param        [in]       searchLen    : one sided search length
 * @param        [in]       leftSkipLen  : number of samiples to be skipped on the left side in the search array
 * @param        [in]       rightSkipLen : number of samiples to be skipped on the right side in the search array
 * @param        [in]       relThr       : the relative threshold between noise power and signal power. if signal 
 *                                         power is greater then relThr*noisepower, declare a peak
 * @param        [out]      index        : output array with indices of the detected peaks
 * @param        [out]      noise        : output array with the noise power for every detected peaks, it is the noise power used
 *                                         in the CFAR detection. If it is the CFAR-CA, the noise power is the average power
 *                                         for all the samples in the two-sides search window. if it is CFAR-CACC, the noise power 
 *                                         is the sum of the powers for the samples in the two sides search window, 
 *                                         if it is CFAR-SO, it is the minimum average power between samples in the two sides search 
 *                                         window, if it is CFAR-GO, it is the maximum average power between samples in 
 *                                         the two sides search window.
 *                                          
 * 
 *
 * @return                  Number of detected peaks (i.e length of out)
 *
 * @pre                    
 *
 * @ingroup                 MMWAVELIB_DETECT_FLOAT
 * 
 * cycles                   8.40*fftsize + 677 (cgt 8.1.3)
 *
 *******************************************************************************************************************
 */

uint32_t mmwavelib_cfarfloat_caall(float32_t * InputPower,
                                   uint32_t  fftSize,
                                   uint8_t   cfarType,
                                   uint32_t  guardLen,
                                   uint32_t  searchLen,
                                   uint32_t  leftSkipLen,
                                   uint32_t  rightSkipLen,
                                   float32_t relThr,
                                   uint32_t  * index,
                                   float32_t * noise)
{
        int32_t     k;
        float32_t * restrict powerPtr, totalPower;
        float32_t  leftWinPower, rightWinPower;
        float32_t   leftscale, rightscale, relativeThr, threshold;
        __float2_t  f2temp1, scale, power2f;
        uint32_t    tempDetected;
        int32_t     totalWinSize, searchWinSize, guardSize, leftSkipSize,rightSkipSize;


        totalWinSize    =    (int32_t)guardLen + (int32_t)searchLen;
        searchWinSize   =    (int32_t)searchLen;
        guardSize       =    (int32_t)guardLen;
        leftSkipSize    =    (int32_t) leftSkipLen;
        rightSkipSize   =    (int32_t) rightSkipLen;
        relativeThr     =    relThr;

        /* default is CA average if (cfarType == CFAR_CA ) */
        leftscale    =    0.5f/(float32_t) (searchLen);
        rightscale   =    0.5f/(float32_t) (searchLen);
            
        if (cfarType == CFAR_CACC )/* CA ACCUMATION */
        {
            leftscale    =    1.f;
            rightscale   =    1.f;
        }
        if (cfarType == CFAR_CASO )/* CA SO */
        {
            leftscale    =    1.f/(float32_t) (searchLen);
            rightscale   =    0.f;
        }
        if (cfarType == CFAR_CAGO ) /* CA GO */
        {
            leftscale    =    0.f;
            rightscale   =    1.f/(float32_t) (searchLen);
        }
        
        
        scale    =    _ftof2(leftscale, rightscale);

        tempDetected    =    0;
        powerPtr        =    InputPower;
        leftWinPower    =    0.0;
        rightWinPower   =    0.0;

        /* copy starting from guardSize+skip, searchwin+guard samples to the left, to replace
         * the skip and wrap around samples.
         the sequence is
        guardSize+skip, guardSize+skip+1,......, guardSize+skip+searchwin+guard-1, skip, skip+1,
         ......
        skip is the first detected samples
         */

        /* first detectionCFARInst->guardSizeRange + detectionCFARInst->searchWinSizeRange samples starting from detectionCFARInst->leftSkipSize*/
        for (k = leftSkipSize + guardSize; k < leftSkipSize + totalWinSize; k += 2 )
        {
            f2temp1         =    _mem8_f2(&powerPtr[k]);
            leftWinPower    +=   _hif2(f2temp1);
            leftWinPower    +=   _lof2(f2temp1);
        }
        if (searchLen & 1U)
        {
            leftWinPower    -=   powerPtr[leftSkipSize + totalWinSize];
        }

        rightWinPower    =    leftWinPower -  powerPtr[leftSkipSize + guardSize];
        rightWinPower   +=    powerPtr[leftSkipSize + totalWinSize];

        power2f       =    (__float2_t) _ftof2(rightWinPower, leftWinPower);
        if (rightWinPower > leftWinPower)
        {
            power2f     =   (__float2_t)  _ftof2(leftWinPower, rightWinPower);
        }
        power2f         =    _dmpysp(power2f, scale);
        totalPower      =    _hif2(power2f) + _lof2(power2f);
        threshold       =    totalPower * relativeThr;
        if (powerPtr[leftSkipLen] > threshold)
        {
            noise[tempDetected]    =    totalPower;
            index[tempDetected]    =    leftSkipLen;
            tempDetected++;
        }
        for (k = 1 + leftSkipSize; k < leftSkipSize + guardSize + 1; k++ )
        {
            leftWinPower        -=    powerPtr[k + guardSize - 1];
            leftWinPower        +=    powerPtr[k + totalWinSize - 1];

            rightWinPower       +=    powerPtr[k + totalWinSize];
            rightWinPower       -=    powerPtr[k + guardSize];
            power2f              =   (__float2_t) _ftof2(rightWinPower, leftWinPower);
            if (rightWinPower > leftWinPower)
            {
                power2f          =   (__float2_t) _ftof2(leftWinPower, rightWinPower);
            }
            power2f             =    _dmpysp(power2f, scale);
            totalPower          =    _hif2(power2f) + _lof2(power2f);
            threshold           =    totalPower * relativeThr;
            if (powerPtr[k] > threshold)
            {
                noise[tempDetected]    =   totalPower;
                index[tempDetected]    =   (uint32_t) k;
                tempDetected++;
            }
        }
        for (k = leftSkipSize + guardSize + 1; k < leftSkipSize + totalWinSize + 1; k++ )
        {
            leftWinPower        -=     powerPtr[k + guardSize - 1];
            leftWinPower        +=     powerPtr[k - guardSize - 1];

            rightWinPower       +=     powerPtr[k + totalWinSize];
            rightWinPower       -=     powerPtr[k + guardSize];
            power2f              =    (__float2_t) _ftof2( rightWinPower,  leftWinPower);
            if (rightWinPower > leftWinPower)
            {
                power2f          =   (__float2_t) _ftof2( leftWinPower,  rightWinPower);
            }
            power2f           =    _dmpysp(power2f, scale);
            totalPower        =    _hif2(power2f) + _lof2(power2f);
            threshold         =    totalPower * relativeThr;
            if (powerPtr[k] > threshold)
            {
                noise[tempDetected]      =      totalPower;
                index[tempDetected]    =   (uint32_t) k;
                tempDetected++;
            }
        }

        /* middle portion of samples */
        for (k = leftSkipSize + totalWinSize + 1; k < (int32_t) fftSize - rightSkipSize - totalWinSize; k++ )
        {
            leftWinPower        -=     powerPtr[k - totalWinSize - 1];
            leftWinPower        +=     powerPtr[k - 1 -  guardSize];

            rightWinPower       +=     powerPtr[k + totalWinSize];
            rightWinPower       -=     powerPtr[k + guardSize];
            power2f             =    (__float2_t) _ftof2( rightWinPower,  leftWinPower);
            if (rightWinPower > leftWinPower)
            {
                power2f         =    (__float2_t) _ftof2( leftWinPower,  rightWinPower);
            }
            power2f             =    _dmpysp(power2f, scale);
            totalPower          =    _hif2(power2f) + _lof2(power2f);
            threshold           =    totalPower * relativeThr;
            if (powerPtr[k] > threshold)
            {
                noise[tempDetected]    =    totalPower;
                index[tempDetected]    =   (uint32_t) k;
                tempDetected++;
            }
        }

        /*
         *  copy from fft-righskip-win-rightkip,  searchwin+guard samples to replace the the samples in the skip
         *  fftsize-rskip is the last samples to be tested
         */

        /* last detectionCFARInst->guardSizeRange + detectionCFARInst->searchWinSizeRange samples before detectionCFARInst->fft1DSize - detectionCFARInst->rightSkipSize*/
        for (k = (int32_t) fftSize - rightSkipSize - totalWinSize; k < (int32_t) fftSize - rightSkipSize - guardSize; k++ )
        {
            leftWinPower        -=     powerPtr[k - totalWinSize - 1];
            leftWinPower        +=     powerPtr[k - guardSize - 1];

            rightWinPower       +=     powerPtr[k - rightSkipSize];
            rightWinPower       -=     powerPtr[k + guardSize];

            power2f              =    (__float2_t) _ftof2( rightWinPower,  leftWinPower);
            if (rightWinPower > leftWinPower)
            {
                power2f          =    (__float2_t) _ftof2( leftWinPower,  rightWinPower);
            }
            power2f              =    _dmpysp(power2f, scale);
            totalPower           =    _hif2(power2f) + _lof2(power2f);
            threshold            =    totalPower * relativeThr;
            if (powerPtr[k] > threshold)
            {
                noise[tempDetected]    =    totalPower;
                index[tempDetected]    =    (uint32_t) k;
                tempDetected++;
            }
        }
        for (k = (int32_t) fftSize - rightSkipSize - guardSize; k < (int32_t) fftSize - rightSkipSize; k++ )
        {
            leftWinPower        -=     powerPtr[k - totalWinSize - 1];
            leftWinPower        +=     powerPtr[k - guardSize - 1];

            rightWinPower       +=     powerPtr[k - rightSkipSize];
            rightWinPower       -=     powerPtr[k - rightSkipSize - searchWinSize];

            power2f              =    (__float2_t) _ftof2( rightWinPower,  leftWinPower);
            if (rightWinPower > leftWinPower)
            {
                power2f          =    (__float2_t) _ftof2( leftWinPower,  rightWinPower);
            }
            power2f              =    _dmpysp(power2f, scale);
            totalPower           =    _hif2(power2f) + _lof2(power2f);
            threshold            =    totalPower * relativeThr;
            if (powerPtr[k] > threshold)
            {
                noise[tempDetected]    =    totalPower;
                index[tempDetected]    =    (uint32_t) k;
                tempDetected++;
            }
        }

        return(tempDetected);
}


/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_cfarfloat_caall_opt
 *
 * \par
 * <b>Description</b>  :    Performs a regular Cell Averaging, Cell Accumulation, SO, and GO CFAR based on floating
 *                          point signal power same as function  mmwavelib_cfarfloat_caall, but with less cycles, and
 *                          the detected peaks are not in ascending order.
 *
 * @param        [in]       InputPower   : input signal power array (floating point)
 * @param        [in]       fftSize      : number of elements in the InputPower
 * @param        [in]       cfarType     : 0-CFAR-CA, 1-CFAR-GO, 2-CFAR-SO, 3-CFAR-CACC
 * @param        [in]       guardLen     : one sided guard length
 * @param        [in]       searchLen    : one sided search length
 * @param        [in]       leftSkipLen  : number of samiples to be skipped on the left side in the search array
 * @param        [in]       rightSkipLen : number of samiples to be skipped on the right side in the search array
 * @param        [in]       relThr       : the relative threshold between noise power and signal power. if signal
 *                                         power is greater then relThr*noisepower, declare a peak
 * @param        [out]      index        : output array with indices of the detected peaks
 * @param        [out]      noise        : output array with the noise power for every detected peaks, it is the noise power used
 *                                         in the CFAR detection. If it is the CFAR-CA, the noise power is the average power
 *                                         for all the samples in the two-sides search window. if it is CFAR-CACC, the noise power 
 *                                         is the sum of the powers for the samples in the two sides search window, 
 *                                         if it is CFAR-SO, it is the minimum average power between samples in the two sides search 
 *                                         window, if it is CFAR-GO, it is the maximum average power between samples in 
 *                                         the two sides search window.
 *     *
 *
 *
 * @return                  Number of detected peaks (i.e length of out)
 *
 * @pre
 *
 * @ingroup                 MMWAVELIB_DETECT_FLOAT
 *
 * cycles                   4.49*fftsize+981 (cgt 8.1.3)
 *******************************************************************************************************************
 */
uint32_t mmwavelib_cfarfloat_caall_opt(float32_t   * InputPower,
                                   uint32_t  fftSize,
                                   uint8_t   cfarType,
                                   uint32_t  guardLen,
                                   uint32_t  searchLen,
                                   uint32_t  leftSkipLen,
                                   uint32_t  rightSkipLen,
                                   float32_t  relThr,
                                   uint32_t * index,
                                   float32_t* noise)
{
        int32_t     k;
        float32_t * restrict powerPtr, totalPower;
        float32_t  leftWinPower, rightWinPower;
        float32_t   leftscale, rightscale, relativeThr, threshold;
        __float2_t  f2temp1, scale, power2f;
        uint32_t    tempDetected;
        int32_t    totalWinSize,searchWinSize,guardSize,leftSkipSize,rightSkipSize;
        
        /* optimized code */
        float32_t  leftWinPowerEnd, rightWinPowerEnd;
        int32_t     k1,leftStart,rightStart, totalPoints;
        __float2_t  power2fEnd;
        float32_t   thresholdEnd, totalPowerEnd;
        

        totalWinSize    =    (int32_t) guardLen + (int32_t) searchLen;
        searchWinSize   =    (int32_t) searchLen;
        guardSize       =    (int32_t) guardLen;
        leftSkipSize    =    (int32_t) leftSkipLen;
        rightSkipSize   =    (int32_t) rightSkipLen;
        relativeThr     =    relThr;
        

        /* default is CA average if (cfarType == CFAR_CA )*/
        leftscale    =    0.5f/(float32_t) (searchLen);
        rightscale   =    0.5f/(float32_t) (searchLen);
        
        if (cfarType == CFAR_CACC )/* CA ACCUMULATION */
        {
            leftscale    =    1.f;
            rightscale   =    1.f;
        }
        if (cfarType == CFAR_CASO ) /* CA SO */
        {
            leftscale    =    1.f/(float32_t) (searchLen);
            rightscale   =    0.f;
        }
        if (cfarType == CFAR_CAGO ) /* CA GO */
        {
            leftscale    =    0.f;
            rightscale   =    1.f/(float32_t) (searchLen);
        }
        
        
        scale        =    _ftof2(leftscale, rightscale);

        /* peak search */
        tempDetected    =    0;
        powerPtr        =    InputPower;
        leftWinPower    =    0.0;
        rightWinPower   =    0.0;
            
        /* first detectionCFARInst->guardSizeRange + detectionCFARInst->searchWinSizeRange samples starting from detectionCFARInst->leftSkipSize*/
        for (k = leftSkipSize + guardSize; k < leftSkipSize + totalWinSize; k += 2 )
        {
            f2temp1          =    _mem8_f2(&powerPtr[k]);
            leftWinPower    +=  _hif2(f2temp1);
            leftWinPower    +=  _lof2(f2temp1);
        }
        if (searchLen & 1U)
        {
            leftWinPower    -=     powerPtr[leftSkipSize + totalWinSize ];
        }

        rightWinPower        =    leftWinPower -  powerPtr[leftSkipSize + guardSize];
        rightWinPower       +=     powerPtr[leftSkipSize + totalWinSize];
        power2f              =   (__float2_t) _ftof2( rightWinPower,  leftWinPower);
        if (rightWinPower > leftWinPower)
        {
            power2f          =    (__float2_t) _ftof2( leftWinPower,  rightWinPower);
        }
        power2f              =    _dmpysp(power2f, scale);
        totalPower           =    _hif2(power2f) + _lof2(power2f);
        threshold            =    totalPower * relativeThr;
        if (powerPtr[leftSkipLen] > threshold)
        {
            noise[tempDetected]    =    totalPower;
            index[tempDetected]    =    leftSkipLen;
            tempDetected++;
        }
        for (k = 1 + leftSkipSize; k < leftSkipSize + guardSize + 1; k++ )
        {
            leftWinPower        -=     powerPtr[k + guardSize - 1];
            leftWinPower        +=     powerPtr[k + totalWinSize - 1];

            rightWinPower       +=     powerPtr[k + totalWinSize];
            rightWinPower       -=     powerPtr[k + guardSize];
            power2f              =    (__float2_t) _ftof2( rightWinPower,  leftWinPower);
            if (rightWinPower > leftWinPower)
            {
                power2f          =    (__float2_t) _ftof2( leftWinPower,  rightWinPower);
            }
            power2f              =    _dmpysp(power2f, scale);
            totalPower           =    _hif2(power2f) + _lof2(power2f);
            threshold            =    totalPower * relativeThr;
            if (powerPtr[k] > threshold)
            {
                noise[tempDetected]    =    totalPower;
                index[tempDetected]    =   (uint32_t)  k;
                tempDetected++;
            }
        }
        for (k = leftSkipSize + guardSize + 1; k < leftSkipSize + totalWinSize  + 1; k++ )
        {
            leftWinPower        -=     powerPtr[k + guardSize - 1];
            leftWinPower        +=     powerPtr[k - guardSize - 1];

            rightWinPower       +=     powerPtr[k + totalWinSize];
            rightWinPower       -=     powerPtr[k + guardSize];
            power2f              =    (__float2_t) _ftof2( rightWinPower,  leftWinPower);
            if (rightWinPower > leftWinPower)
            {
                power2f          =    (__float2_t) _ftof2( leftWinPower,  rightWinPower);
            }
            power2f              =    _dmpysp(power2f, scale);
            totalPower           =    _hif2(power2f) + _lof2(power2f);
            threshold            =    totalPower * relativeThr;
            if (powerPtr[k] > threshold)
            {
                noise[tempDetected]    =    totalPower;
                index[tempDetected]    =    (uint32_t) k;
                tempDetected++;
            }
        }
            
        /* right edge */
        k =  (int32_t) fftSize - rightSkipSize - 1;
        rightWinPowerEnd = 0;
            
        for (k1 = (int32_t)fftSize - rightSkipSize - rightSkipSize - searchWinSize; k1 < (int32_t) fftSize - rightSkipSize - rightSkipSize; k1 += 2 )
        {
            f2temp1              =    _mem8_f2(&powerPtr[k1]);
            rightWinPowerEnd    +=  _hif2(f2temp1);
            rightWinPowerEnd    +=  _lof2(f2temp1);
        }
        if (searchLen & 1U)
        {
            rightWinPowerEnd    -=     powerPtr[(int32_t)fftSize - rightSkipSize - rightSkipSize];
        }

        leftWinPowerEnd = 0;
        for (k1 = k - totalWinSize; k1 < k - guardSize; k1 += 2 )
        {
            f2temp1            =    _mem8_f2(&powerPtr[k1]);
            leftWinPowerEnd    +=  _hif2(f2temp1);
            leftWinPowerEnd    +=  _lof2(f2temp1);
        }
        if (searchLen & 1U)
        {
            leftWinPowerEnd    -=     powerPtr[k - guardSize];
        }

        power2f    =    (__float2_t) _ftof2( rightWinPowerEnd,  leftWinPowerEnd);
        if (rightWinPowerEnd > leftWinPowerEnd)
        {
            power2f            =    (__float2_t) _ftof2( leftWinPowerEnd,  rightWinPowerEnd);
        }
        power2f                =    _dmpysp(power2f, scale);
        totalPower             =    _hif2(power2f) + _lof2(power2f);
        threshold              =    totalPower * relativeThr;
        if (powerPtr[k] > threshold)
        {
            noise[tempDetected]    =      totalPower;
            index[tempDetected]    =   (uint32_t) k;
            tempDetected++;
        }
        /* start to slide to the left window */
        for (k = (int32_t)fftSize - rightSkipSize - 2; k > (int32_t) fftSize- rightSkipSize - guardSize - 2; k-- )
        {
            rightWinPowerEnd     -=      powerPtr[k + 1 - rightSkipSize];
            rightWinPowerEnd     +=      powerPtr[k + 1 - rightSkipSize - searchWinSize ];

            leftWinPowerEnd     -=      powerPtr[k - guardSize];
            leftWinPowerEnd     +=      powerPtr[k - totalWinSize];

            power2f    =    (__float2_t) _ftof2( rightWinPowerEnd,  leftWinPowerEnd);
            if (rightWinPowerEnd > leftWinPowerEnd)
            {
                power2f        =    (__float2_t) _ftof2( leftWinPowerEnd,  rightWinPowerEnd);
            }
            power2f            =    _dmpysp(power2f, scale);
            totalPower         =    _hif2(power2f) + _lof2(power2f);
            threshold          =    totalPower * relativeThr;
            if (powerPtr[k] > threshold)
            {
                noise[tempDetected]    =      totalPower;
                index[tempDetected]    =   (uint32_t) k;
                tempDetected++;
            }
        }
            
        for ( k = (int32_t)fftSize - rightSkipSize - guardSize - 2; k > (int32_t) fftSize - rightSkipSize - totalWinSize - 2; k--)
        {
            rightWinPowerEnd    -=      powerPtr[k + 1 - rightSkipSize];
            rightWinPowerEnd    +=      powerPtr[k + 1 + guardSize ];

            leftWinPowerEnd     -=     powerPtr[k - guardSize];
            leftWinPowerEnd     +=     powerPtr[k - totalWinSize];

            power2f    =    (__float2_t) _ftof2( rightWinPowerEnd,  leftWinPowerEnd);
            if (rightWinPowerEnd > leftWinPowerEnd)
            {
                power2f          =    (__float2_t) _ftof2( leftWinPowerEnd,  rightWinPowerEnd);
            }
            power2f              =    _dmpysp(power2f, scale);
            totalPower           =    _hif2(power2f) + _lof2(power2f);
            threshold            =    totalPower * relativeThr;
            if (powerPtr[k] > threshold)
            {
                noise[tempDetected]    =      totalPower;
                index[tempDetected]    =   (uint32_t)  k;
                tempDetected++;
            }
        }
            
        /* middle part */
        leftStart = leftSkipSize + totalWinSize + 1 ;
        rightStart = (int32_t) fftSize - rightSkipSize - totalWinSize - 2;
        totalPoints = rightStart - leftStart + 1;
            
        for (k1 = 0; k1 < totalPoints/2; k1++ )
        {
            /* k = leftStart+k1 ; */
            leftWinPower    -=     powerPtr[leftStart + k1 - totalWinSize - 1];
            leftWinPower    +=     powerPtr[leftStart + k1 - guardSize - 1];

            rightWinPower    -=     powerPtr[leftStart + k1 + guardSize];
            rightWinPower    +=     powerPtr[leftStart + k1 + totalWinSize];

            power2f            =    (__float2_t) _ftof2( rightWinPower,  leftWinPower);
            if (rightWinPower > leftWinPower)
            {
                power2f        =    (__float2_t) _ftof2( leftWinPower,  rightWinPower);
            }
            power2f            =    _dmpysp(power2f, scale);
            totalPower        =    _hif2(power2f) + _lof2(power2f);
            threshold        =    totalPower * relativeThr;
                
                
            /* k = rightStart-k1; */
            leftWinPowerEnd     -=     powerPtr[rightStart - k1 - guardSize];
            leftWinPowerEnd     +=     powerPtr[rightStart - k1 - totalWinSize];

            rightWinPowerEnd    +=     powerPtr[rightStart - k1 + guardSize + 1];
            rightWinPowerEnd    -=     powerPtr[rightStart - k1 +  totalWinSize + 1];

            power2fEnd        =    (__float2_t) _ftof2( rightWinPowerEnd,  leftWinPowerEnd);
            if (rightWinPowerEnd > leftWinPowerEnd)
            {
                power2fEnd      =    (__float2_t) _ftof2( leftWinPowerEnd,  rightWinPowerEnd);
            }
            power2fEnd          =    _dmpysp(power2fEnd, scale);
            totalPowerEnd       =    _hif2(power2fEnd) + _lof2(power2fEnd);
            thresholdEnd        =    totalPowerEnd * relativeThr;
                
            if (powerPtr[leftStart+k1] > threshold)
            {
                noise[tempDetected]    =      totalPower;
                index[tempDetected]    =   (uint32_t) leftStart+k1;
                tempDetected++;
            }
                
            if (powerPtr[rightStart-k1] > thresholdEnd)
            {
                noise[tempDetected]      =      totalPowerEnd;
                index[tempDetected]    =    rightStart-k1;
                tempDetected++;
            }
                                
                
        }
        /* middle port */
        if ((uint32_t)totalPoints & 0x1U) 
        {
            k = leftStart + totalPoints/2;
            leftWinPower    -=     powerPtr[k - totalWinSize - 1];
            leftWinPower    +=     powerPtr[k - guardSize - 1];

            rightWinPower    -=     powerPtr[k + guardSize];
            rightWinPower    +=     powerPtr[k + totalWinSize];
            power2f            =   (__float2_t) _ftof2( rightWinPower,  leftWinPower);
            if (rightWinPower > leftWinPower)
            {
                power2f        =    (__float2_t)_ftof2( leftWinPower,  rightWinPower);
            }
            power2f            =    _dmpysp(power2f, scale);
            totalPower         =    _hif2(power2f) + _lof2(power2f);
            threshold          =    totalPower * relativeThr;
            if (powerPtr[k] > threshold)
            {
                noise[tempDetected]    =      totalPower;
                index[tempDetected]    =   (uint32_t) k;
                tempDetected++;
            }

        }

        return(tempDetected);
}

/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_cfarfloat_wrap
 *
 * \par
 * <b>Description</b>  :    Performs a regular Cell Averaging, Cell Accumulation, Cell SO, and Cell GO CFAR based on 
 *                          floating point signal power. The noise samples for the cell at the edges are the wrap around samples.
  *
 * @param        [in]       InputPower   : input signal power array (floating point)
 * @param        [in]       fftSize      : number of elements in the InputPower
 * @param        [in]       cfarType     : 0-CFAR-CA, 1-CFAR-GO, 2-CFAR-SO, 3-CFAR-CACC
 * @param        [in]       guardLen     : one sided guard length
 * @param        [in]       searchLen    : one sided search length
 * @param        [in]       relThr       : the relative threshold between noise power and signal power. if signal
 *                                         power is greater then relThr*noisepower, declare a peak
 * @param        [out]      index        : output array with indices of the detected peaks
 * @param        [out]      noise        : output array with the noise power for every detected peaks, it is the noise power used
 *                                         in the CFAR detection. If it is the CFAR-CA, the noise power is the average power
 *                                         for all the samples in the two-sides search window. if it is CFAR-CACC, the noise power 
 *                                         is the sum of the powers for the samples in the two sides search window, 
 *                                         if it is CFAR-SO, it is the minimum average power between samples in the two sides search 
 *                                         window, if it is CFAR-GO, it is the maximum average power between samples in 
 *                                         the two sides search window.
 *     
 *
 *
 * @return                  Number of detected peaks (i.e length of out)
 *
 * @pre
 *
 * @ingroup                 MMWAVELIB_DETECT_FLOAT
 *
 * cycles                  8.02*fftSize + 604 (cgt 8.1.3)
 *
 *******************************************************************************************************************
 */

uint32_t mmwavelib_cfarfloat_wrap(float32_t    * InputPower,
                                  uint32_t fftSize,
                                  uint8_t   cfarType,
                                  uint32_t  guardLen,
                                  uint32_t  searchLen,
                                  float32_t relThr,
                                  uint32_t * index,
                                  float32_t* noise)
{
    int32_t     k;
    float32_t * restrict powerPtr, totalPower;
    float32_t   leftWinPower, rightWinPower;
    float32_t   leftscale, rightscale, relativeThr, threshold;
    __float2_t  f2temp1, scale, power2f;
    uint32_t    tempDetected;
    int32_t     totalWinSize, guardSize;

    /* default is CA average if (cfarType == CFAR_CA )*/
    leftscale    =    0.5f/(float32_t) (searchLen);
    rightscale   =    0.5f/(float32_t) (searchLen);
        
    if (cfarType == CFAR_CACC ) /* CA ACCUMULATION */
    {
        leftscale    =    1.f;
        rightscale   =    1.f;
    }
    if (cfarType == CFAR_CASO ) /* CA SO */
    {
        leftscale    =    1.f/(float32_t) (searchLen);
        rightscale   =    0.f;
    }
    if (cfarType == CFAR_CAGO ) /* CA GO */
    {
        leftscale    =    0.f;
        rightscale   =    1.f/(float32_t) (searchLen);
    }
    
    scale        =    _ftof2(leftscale, rightscale);

    totalWinSize    =    (int32_t)guardLen + (int32_t)searchLen;
    guardSize       =    (int32_t)guardLen;
    relativeThr     =    relThr;

    /* peak search */
    tempDetected    =    0;
    powerPtr        =    InputPower;
    leftWinPower    =    0.0;
    rightWinPower   =    0.0;

    for (k = (int32_t) fftSize - totalWinSize; k < (int32_t) fftSize - guardSize; k += 2 )
    {
        f2temp1          =    _mem8_f2(&powerPtr[k]);
        leftWinPower    +=  _hif2(f2temp1);
        leftWinPower    +=  _lof2(f2temp1);
    }

    if (searchLen&0x1U)
    {
        leftWinPower    -=   powerPtr[fftSize - guardLen ];
    }

    for (k = guardSize + 1; k < totalWinSize + 1; k += 2)
    {
        f2temp1            =    _mem8_f2(&powerPtr[k]);
        rightWinPower    +=     _hif2(f2temp1);
        rightWinPower    +=     _lof2(f2temp1);
    }
    if (searchLen & 0x1U)
    {
        rightWinPower    -=   powerPtr[totalWinSize + 1];
    }


    power2f            =   (__float2_t) _ftof2( rightWinPower,  leftWinPower);
    if (rightWinPower > leftWinPower)
    {
        power2f        =    (__float2_t)_ftof2( leftWinPower,  rightWinPower);
    }
    power2f            =    _dmpysp(power2f, scale);
    totalPower         =    _hif2(power2f) + _lof2(power2f);
    threshold          =    totalPower * relativeThr;

    if (powerPtr[0] > threshold)
    {
        noise[tempDetected]    =    totalPower;
        index[tempDetected]    =    0;
        tempDetected++;
    }

    /* left edge */
    for (k = 1; k < guardSize + 1; k++ ) 
    {
        leftWinPower    -=     powerPtr[(int32_t) fftSize - totalWinSize + k - 1];
        leftWinPower    +=     powerPtr[(int32_t) fftSize - guardSize + k -1];

        rightWinPower    -=     powerPtr[k + guardSize];
        rightWinPower    +=     powerPtr[k + totalWinSize];

        power2f            =    (__float2_t)_ftof2( rightWinPower,  leftWinPower);
        if (rightWinPower > leftWinPower)
        {
            power2f        =   (__float2_t) _ftof2( leftWinPower,  rightWinPower);
        }
        power2f            =    _dmpysp(power2f, scale);
        totalPower         =    _hif2(power2f) + _lof2(power2f);
        threshold          =    totalPower * relativeThr;

        if (powerPtr[k] > threshold)
        {
            noise[tempDetected]      =      totalPower;
            index[tempDetected]    =  (uint32_t)  k;
            tempDetected++;
        }

    }

    for (k =  guardSize + 1; k < totalWinSize + 1; k++ ) 
    {
        leftWinPower    -=     powerPtr[(int32_t) fftSize - totalWinSize + k - 1];
        leftWinPower    +=     powerPtr[k - guardSize - 1];

        rightWinPower    -=     powerPtr[k + guardSize];
        rightWinPower    +=     powerPtr[k + totalWinSize];

        power2f            =    (__float2_t)_ftof2( rightWinPower,  leftWinPower);
        if (rightWinPower > leftWinPower)
        {
            power2f        =   (__float2_t) _ftof2( leftWinPower,  rightWinPower);
        }
        power2f            =    _dmpysp(power2f, scale);
        totalPower         =    _hif2(power2f) + _lof2(power2f);
        threshold          =    totalPower * relativeThr;
        if (powerPtr[k] > threshold)
        {
            noise[tempDetected]      =      totalPower;
            index[tempDetected]    =   (uint32_t) k;
            tempDetected++;
        }

    }

    /*middle part*/
    for (k = totalWinSize + 1; k<(int32_t) fftSize - totalWinSize; k++)
    {
        leftWinPower    -=     powerPtr[k - totalWinSize - 1];
        leftWinPower    +=     powerPtr[k - guardSize - 1];

        rightWinPower    -=     powerPtr[k + guardSize];
        rightWinPower    +=     powerPtr[k + totalWinSize];

        power2f            =    (__float2_t)_ftof2( rightWinPower,  leftWinPower);
        if (rightWinPower > leftWinPower)
        {
            power2f        =    _ftof2( leftWinPower,  rightWinPower);
        }
        power2f            =    _dmpysp(power2f, scale);
        totalPower         =    _hif2(power2f) + _lof2(power2f);
        threshold          =    totalPower * relativeThr;

        if (powerPtr[k] > threshold)
        {
            noise[tempDetected]        =      totalPower;
            index[tempDetected]    =   (uint32_t) k;
            tempDetected++;
        }
    }
    /* right edge */
    for (k=(int32_t) fftSize - totalWinSize; k <(int32_t) fftSize - guardSize; k++)
    {
        leftWinPower    -=     powerPtr[k - totalWinSize - 1];
        leftWinPower    +=     powerPtr[k - guardSize - 1];

        rightWinPower    -=     powerPtr[k + guardSize];
        rightWinPower    +=     powerPtr[k - (int32_t) fftSize + totalWinSize];

        power2f            =    (__float2_t) _ftof2( rightWinPower,  leftWinPower);
        if (rightWinPower > leftWinPower)
        {
            power2f        =    (__float2_t) _ftof2( leftWinPower,  rightWinPower);
        }
        power2f            =    _dmpysp(power2f, scale);
        totalPower         =    _hif2(power2f) + _lof2(power2f);
        threshold          =    totalPower * relativeThr;

        if (powerPtr[k] > threshold)
        {
            noise[tempDetected]      =      totalPower;
            index[tempDetected]    =   (uint32_t) k;
            tempDetected++;
        }
    }


    for (k = (int32_t) fftSize - guardSize; k < (int32_t) fftSize; k++)
    {
        leftWinPower    -=     powerPtr[k - totalWinSize - 1];
        leftWinPower    +=     powerPtr[k - guardSize - 1];

        rightWinPower    -=     powerPtr[k - (int32_t) fftSize + guardSize];
        rightWinPower    +=     powerPtr[k - (int32_t) fftSize + totalWinSize];

        power2f            =    (__float2_t) _ftof2( rightWinPower,  leftWinPower);
        if (rightWinPower > leftWinPower)
        {
            power2f        =   (__float2_t)  _ftof2( leftWinPower,  rightWinPower);
        }
        power2f            =    _dmpysp(power2f, scale);
        totalPower        =    _hif2(power2f) + _lof2(power2f);
        threshold        =    totalPower * relativeThr;
        if (powerPtr[k] > threshold)
        {
            noise[tempDetected]      =      totalPower;
            index[tempDetected]    =  (uint32_t)  k;
            tempDetected++;
        }
    }


    return tempDetected;
}

/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_cfarfloat_wrap_opt
 *
 * \par
 * <b>Description</b>  :    Performs a regular Cell Averaging, Cell Accumulation, Cell SO, and Cell GO CFAR  based on 
 *                          floating  point signal power, same as mmwavelib_cfarfloat_wrap, but with less cycles, and the 
 *                          detected peaks are not in ascending order.
  *
 * @param        [in]       InputPower   : input signal power array (floating point)
 * @param        [in]       fftSize      : number of elements in the InputPower
 * @param        [in]       cfarType     : 0-CFAR-CA, 1-CFAR-GO, 2-CFAR-SO, 3-CFAR-CACC
 * @param        [in]       guardLen     : one sided guard length
 * @param        [in]       searchLen    : one sided search length
 * @param        [in]       relThr       : the relative threshold between noise power and signal power. if signal
 *                                         power is greater then relThr*noisepower, declare a peak
 * @param        [out]      index        : output array with indices of the detected peaks
 * @param        [out]      noise        : output array with the noise power for every detected peaks, it is the noise power used
 *                                         in the CFAR detection. If it is the CFAR-CA, the noise power is the average power
 *                                         for all the samples in the two-sides search window. if it is CFAR-CACC, the noise power 
 *                                         is the sum of the powers for the samples in the two sides search window, 
 *                                         if it is CFAR-SO, it is the minimum average power between samples in the two sides search 
 *                                         window, if it is CFAR-GO, it is the maximum average power between samples in 
 *                                         the two sides search window.
 *     
 *
 *
 * @return                  Number of detected peaks (i.e length of out)
 *
 * @pre
 *
 * @ingroup                 MMWAVELIB_DETECT_FLOAT
 *
 * cycles                  4.5*fftSize + 894 (cgt 8.1.3)
 *
 *******************************************************************************************************************
 */

uint32_t mmwavelib_cfarfloat_wrap_opt(float32_t * InputPower,
                                      uint32_t  fftSize,
                                      uint8_t   cfarType,
                                      uint32_t  guardLen,
                                      uint32_t  searchLen,
                                      float32_t relThr,
                                      uint32_t * index,
                                      float32_t* noise)
{
    int32_t     k;
    float32_t * restrict powerPtr, totalPower;
    float32_t  leftWinPower, rightWinPower;
    float32_t  leftWinPowerEnd, rightWinPowerEnd;
    float32_t   leftscale, rightscale, relativeThr, threshold;
    __float2_t  f2temp1, scale, power2f;
    uint32_t    tempDetected;
    int32_t     totalWinSize, guardSize;
    int32_t     leftStart, rightStart, totalPoints, k1;

    /* default is CA average if (cfarType == CFAR_CA )*/
    leftscale    =    0.5f/(float32_t) (searchLen);
    rightscale   =    0.5f/(float32_t) (searchLen);
        
    if (cfarType == CFAR_CACC ) /* CA ACCUMULATION */
    {
        leftscale    =    1.f;
        rightscale   =    1.f;
    }
    if (cfarType == CFAR_CASO )/* CA SO */
    {
        leftscale    =    1.f/(float32_t) (searchLen);
        rightscale   =    0.f;
    } 
    if (cfarType == CFAR_CAGO ) /* CA GO */
    {
        leftscale    =    0.f;
        rightscale   =    1.f/(float32_t) (searchLen);
    }
    
    scale        =    _ftof2(leftscale, rightscale);

    totalWinSize    =    (int32_t)guardLen + (int32_t)searchLen;
    guardSize       =    (int32_t)guardLen;
    relativeThr     =    relThr;

    /* peak search */
    tempDetected    =    0;
    powerPtr        =    InputPower;
    leftWinPower    =    0.0;
    rightWinPower   =    0.0;

    for (k = (int32_t) fftSize - totalWinSize; k < (int32_t) fftSize - guardSize; k += 2 )
    {
        f2temp1          =    _mem8_f2(&powerPtr[k]);
        leftWinPower    +=  _hif2(f2temp1);
        leftWinPower    +=  _lof2(f2temp1);
    }

    if (searchLen & 0x1U)
    {
        leftWinPower    -=   powerPtr[(int32_t)fftSize - guardSize ];
    }

    for (k = guardSize + 1; k < totalWinSize + 1; k += 2)
    {
        f2temp1            =    _mem8_f2(&powerPtr[k]);
        rightWinPower     +=    _hif2(f2temp1);
        rightWinPower     +=    _lof2(f2temp1);
    }
    if (searchLen & 0x1U)
    {
        rightWinPower    -=   powerPtr[totalWinSize + 1];
    }


    power2f            =    (__float2_t) _ftof2( rightWinPower,  leftWinPower);
    if (rightWinPower > leftWinPower)
    {
        power2f        =    (__float2_t) _ftof2( leftWinPower,  rightWinPower);
    }
    power2f            =    _dmpysp(power2f, scale);
    totalPower         =    _hif2(power2f) + _lof2(power2f);
    threshold          =    totalPower * relativeThr;

    if (powerPtr[0] > threshold)
    {
        noise[tempDetected]    =      totalPower;
        index[tempDetected]    =    0;
        tempDetected++;
    }

     /* left edge */
    for (k = 1; k < guardSize + 1; k++ )
    {
        leftWinPower    -=     powerPtr[(int32_t) fftSize - totalWinSize + k - 1];
        leftWinPower    +=     powerPtr[(int32_t) fftSize - guardSize + k -1];

        rightWinPower    -=     powerPtr[k + guardSize];
        rightWinPower    +=     powerPtr[k + totalWinSize];

        power2f            =    (__float2_t) _ftof2( rightWinPower,  leftWinPower);
        if (rightWinPower > leftWinPower)
        {
            power2f        =    (__float2_t) _ftof2( leftWinPower,  rightWinPower);
        }
        power2f            =    _dmpysp(power2f, scale);
        totalPower         =    _hif2(power2f) + _lof2(power2f);
        threshold          =    totalPower * relativeThr;

        if (powerPtr[k] > threshold)
        {
            noise[tempDetected]    =      totalPower;
            index[tempDetected]    =  (uint32_t)  k;
            tempDetected++;
        }

    }

    for (k =  guardSize + 1; k < totalWinSize + 1; k++ ) 
    {
        leftWinPower    -=     powerPtr[(int32_t)fftSize - totalWinSize + k - 1];
        leftWinPower    +=     powerPtr[k - guardSize - 1];

        rightWinPower    -=     powerPtr[k + guardSize];
        rightWinPower    +=     powerPtr[k + totalWinSize];

        power2f            =    (__float2_t) _ftof2( rightWinPower,  leftWinPower);
        if (rightWinPower > leftWinPower)
        {
            power2f        =    (__float2_t) _ftof2( leftWinPower,  rightWinPower);
        }
        power2f            =    _dmpysp(power2f, scale);
        totalPower         =    _hif2(power2f) + _lof2(power2f);
        threshold          =    totalPower * relativeThr;
        if (powerPtr[k] > threshold)
        {
            noise[tempDetected]    =      totalPower;
            index[tempDetected]    =   (uint32_t)  k;
            tempDetected++;
        }

    }

    /* right edge */
    leftWinPowerEnd = 0;
    rightWinPowerEnd = 0;
    /* k=fftSize-1 */
    for (k = guardSize; k < totalWinSize; k += 2)
    {
        f2temp1              =    _mem8_f2(&powerPtr[k]);
        rightWinPowerEnd    +=    _hif2(f2temp1);
        rightWinPowerEnd    +=    _lof2(f2temp1);
    }

    if (searchLen & 0x1U)
    {
        rightWinPowerEnd    -=    powerPtr[totalWinSize];
    }

    for (k = (int32_t) fftSize - totalWinSize - 1; k < (int32_t) fftSize - guardSize - 1; k += 2)
    {
        f2temp1            =    _mem8_f2(&powerPtr[k]);
        leftWinPowerEnd   +=    _hif2(f2temp1);
        leftWinPowerEnd   +=    _lof2(f2temp1);
    }

    if (searchLen & 0x1U)
    {
        leftWinPowerEnd    -=   powerPtr[(int32_t) fftSize-guardSize-1];
    }
    power2f            =    (__float2_t) _ftof2( rightWinPowerEnd,  leftWinPowerEnd);
    if (rightWinPowerEnd > leftWinPowerEnd)
    {
        power2f        =    (__float2_t) _ftof2( leftWinPowerEnd,  rightWinPowerEnd);
    }
    power2f            =    _dmpysp(power2f, scale);
    totalPower        =    _hif2(power2f) + _lof2(power2f);
    threshold        =    totalPower * relativeThr;
    if (powerPtr[fftSize-1U] > threshold)
    {
        noise[tempDetected]    =    totalPower;
        index[tempDetected]    =    fftSize-1U;
        tempDetected++;
    }

    /* right edge */
    for (k = (int32_t) fftSize - 2; k > (int32_t)fftSize - guardSize - 2 ; k--)
    {
        leftWinPowerEnd        +=     powerPtr[k - totalWinSize];
        leftWinPowerEnd        -=     powerPtr[k - guardSize];

        rightWinPowerEnd    -=     powerPtr[k - (int32_t) fftSize + totalWinSize + 1];
        rightWinPowerEnd    +=     powerPtr[k - (int32_t) fftSize + guardSize + 1];

        power2f            =   (__float2_t) _ftof2( rightWinPowerEnd,  leftWinPowerEnd);
        if (rightWinPowerEnd > leftWinPowerEnd)
        {
            power2f        =    (__float2_t) _ftof2( leftWinPowerEnd,  rightWinPowerEnd);
        }
        power2f            =    _dmpysp(power2f, scale);
        totalPower        =    _hif2(power2f) + _lof2(power2f);
        threshold        =    totalPower * relativeThr;
        if (powerPtr[k] > threshold)
        {
            noise[tempDetected]     =      totalPower;
            index[tempDetected]    =  (uint32_t)  k;
            tempDetected++;
        }
    }

    for (k = (int32_t) fftSize - guardSize - 2; k > (int32_t) fftSize - totalWinSize - 2; k--)
    {
        leftWinPowerEnd        +=     powerPtr[k - totalWinSize];
        leftWinPowerEnd        -=     powerPtr[k - guardSize];

        rightWinPowerEnd    -=     powerPtr[k - (int32_t) fftSize + totalWinSize + 1];
        rightWinPowerEnd    +=     powerPtr[k + guardSize + 1];

        power2f            =    (__float2_t) _ftof2( rightWinPowerEnd,  leftWinPowerEnd);
        if (rightWinPowerEnd > leftWinPowerEnd)
        {
            power2f        =   (__float2_t)  _ftof2( leftWinPowerEnd,  rightWinPowerEnd);
        }
        power2f            =    _dmpysp(power2f, scale);
        totalPower        =    _hif2(power2f) + _lof2(power2f);
        threshold        =    totalPower * relativeThr;
        if (powerPtr[k] > threshold)
        {
            noise[tempDetected]    =      totalPower;
            index[tempDetected]    =  (uint32_t)  k;
            tempDetected++;
        }
    }

    leftStart =  totalWinSize + 1 ;
    rightStart = (int32_t) fftSize - totalWinSize - 2;
    totalPoints = rightStart - leftStart + 1;

    /* middle part */
    for (k1 = 0; k1 < totalPoints/2; k1++)
    {
        k = leftStart + k1;
        leftWinPower     -=     powerPtr[k - totalWinSize - 1];
        leftWinPower     +=     powerPtr[k - guardSize - 1];

        rightWinPower    -=     powerPtr[k + guardSize];
        rightWinPower    +=     powerPtr[k + totalWinSize];

#ifdef _TMS320C6600 /* C66 */
        power2f           =    (__float2_t) _ftof2( rightWinPower,  leftWinPower);
        if (rightWinPower > leftWinPower)
        {
            power2f       =    (__float2_t) _ftof2( leftWinPower,  rightWinPower);
        }
        power2f           =    _dmpysp(power2f, scale);
        totalPower        =    _hif2(power2f) + _lof2(power2f);
#else /* C674 */
        totalPower        =    rightWinPower * leftscale + leftWinPower * rightscale;
        if (rightWinPower > leftWinPower)
        {
            totalPower    =    leftWinPower * leftscale + rightWinPower * rightscale;
        }
#endif
        threshold         =    totalPower * relativeThr;

        if (powerPtr[k] > threshold)
        {
            noise[tempDetected]    =      totalPower;
            index[tempDetected]    =   (uint32_t) k;
            tempDetected++;
        }

        /* k= rightStart - k1 */

        leftWinPowerEnd     -=     powerPtr[rightStart - k1 - guardSize];
        leftWinPowerEnd     +=     powerPtr[rightStart - k1 - totalWinSize];
        rightWinPowerEnd    -=     powerPtr[rightStart - k1 + totalWinSize + 1];
        rightWinPowerEnd    +=     powerPtr[rightStart - k1 + guardSize + 1];

#ifdef _TMS320C6600 /* C66 */
        power2f            =    (__float2_t) _ftof2( rightWinPowerEnd,  leftWinPowerEnd);
        if (rightWinPowerEnd > leftWinPowerEnd)
        {
            power2f        =   (__float2_t)  _ftof2( leftWinPowerEnd,  rightWinPowerEnd);
        }
        power2f            =    _dmpysp(power2f, scale);
        totalPower         =    _hif2(power2f) + _lof2(power2f);
#else /* C674 */
        totalPower         =    leftWinPowerEnd * leftscale + rightWinPowerEnd * rightscale;
        if (rightWinPowerEnd < leftWinPowerEnd)
        {
            totalPower     =    rightWinPowerEnd * leftscale + leftWinPowerEnd * rightscale;
        }
#endif
        threshold          =    totalPower * relativeThr;

        if (powerPtr[rightStart - k1] > threshold)
        {
            noise[tempDetected]    =      totalPower;
            index[tempDetected]    =    rightStart - k1;
            tempDetected++;
        }


    }

    if ((uint32_t) totalPoints & 0x1U)
    {
        k = leftStart + totalPoints/2;
        leftWinPower    -=     powerPtr[k - totalWinSize - 1];
        leftWinPower    +=     powerPtr[k - guardSize - 1];

        rightWinPower    -=     powerPtr[k + guardSize];
        rightWinPower    +=     powerPtr[k + totalWinSize];

        power2f            =   (__float2_t) _ftof2( rightWinPower,  leftWinPower);
        if (rightWinPower > leftWinPower)
        {
            power2f        =    (__float2_t) _ftof2( leftWinPower,  rightWinPower);
        }
        power2f            =    _dmpysp(power2f, scale);
        totalPower        =    _hif2(power2f) + _lof2(power2f);
        threshold        =    totalPower * relativeThr;

        if (powerPtr[leftStart + k1] > threshold)
        {
            noise[tempDetected]     =      totalPower;
            index[tempDetected]    =  leftStart + k1;
            tempDetected++;
        }

    }

    return tempDetected;
}


/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_cfarfloat_point
 *
 * \par
 * <b>Description</b>  :    Performs single CUT CFAR detection using Cell Averaging, Cell Accumulation, Cell SO, 
 *                          or Cell GO CFAR based on floating point signal power.
  *
 * @param        [in]       InputPower   : input signal power array (floating point)
 * @param        [in]       fftSize      : number of elements in the InputPower
 * @param        [in]       cfarType     : 0-CFAR-CA, 1-CFAR-GO, 2-CFAR-SO, 3-CFAR-CACC
 * @param        [in]       guardLen     : one sided guard length
 * @param        [in]       searchLen    : one sided search length
 * @param        [in]       relThr       : the relative threshold between noise power and signal power. if signal
 *                                         power is greater then relThr*noisepower, declare a peak
 * @param        [in]       index        : the detected element index in the InputPower array
 * @param        [out]      noise        : output noise power for the detected index
 *
 *
 *
 * @return                  1 if it is a peak, 0 if not a peak
 *
 * @pre
 *
 * @ingroup               MMWAVELIB_DETECT_FLOAT
 *
 * cycles                 540 (cgt 8.1.3)
 *
 *******************************************************************************************************************
 */
uint8_t mmwavelib_cfarfloat_point(float32_t * InputPower,
                                  uint32_t fftSize,
                                  uint8_t cfarType,
                                  uint32_t guardLen,
                                  uint32_t searchLen,
                                  float32_t relThr,
                                  uint32_t index,
                                  float32_t * noise)
{
    int32_t     leftrepeat, rightrepeat, totalWinSize, searchSize, guardSize;
    float32_t  leftWinPower, rightWinPower;
    float32_t   leftscale, rightscale, threshold;
    __float2_t  scale, power2f;

    float32_t * restrict powerPtr, totalPower;
    int32_t k;

    uint8_t peakFlag;
    powerPtr = InputPower;

    /* default is CA average if (cfarType == CFAR_CA )*/
    leftscale    =    0.5f/(float32_t) (searchLen);
    rightscale   =    0.5f/(float32_t) (searchLen);
        
    if (cfarType == CFAR_CACC )
    {
        leftscale    =    1.f;
        rightscale    =    1.f;
    }
    if (cfarType == CFAR_CASO )
    {
        leftscale    =    1.f/(float32_t) (searchLen);
        rightscale    =    0.f;
    }
    if (cfarType == CFAR_CAGO )
    {
        leftscale    =    0.f;
        rightscale    =    1.f/(float32_t) (searchLen);
    }
    
    scale        =    _ftof2(leftscale, rightscale);
    searchSize   =   (int32_t) searchLen;
    guardSize    =   (int32_t) guardLen;
    totalWinSize =  searchSize + guardSize;

    leftrepeat        =    totalWinSize - (int32_t) index;
    if (leftrepeat < 0)
    {
        leftrepeat = 0;
    }

    rightrepeat        =    totalWinSize - ((int32_t)fftSize - 1 - (int32_t)index);
    if (rightrepeat < 0)
    {
        rightrepeat = 0;
    }

    leftWinPower    =   0.f;
    rightWinPower   =   0.f;

    /* left edge */
    if ((int32_t) index < guardSize + 1) 
    {
        for (k = (int32_t) fftSize - leftrepeat; k < (int32_t) fftSize - guardSize + (int32_t)index; k ++)
        {
            leftWinPower += powerPtr[k];
        }

        for (k = (int32_t) index + guardSize + 1; k < (int32_t) index + totalWinSize + 1; k++)
        {
            rightWinPower += powerPtr[k];
        }

    }
    else if ((int32_t)index <  totalWinSize) 
    {

        for (k = (int32_t)fftSize - leftrepeat; k < (int32_t)fftSize; k++ )
        {
            leftWinPower += powerPtr[k];
        }
        for (k = 0; k <  searchSize - leftrepeat ; k++ )
        {
            leftWinPower += powerPtr[k];
        }
        for (k = (int32_t)index + guardSize + 1; k <= (int32_t) index + totalWinSize; k++ )
        {
            rightWinPower += powerPtr[k];
        }
    }
    else if ((int32_t)index >= (int32_t) fftSize - 1 - guardSize)
    {
        for (k = guardSize - ((int32_t)fftSize - 1 - (int32_t)index); k < rightrepeat; k++)
        {
            rightWinPower += powerPtr[k];
        }

        for (k = (int32_t)index - totalWinSize; k < (int32_t) index - guardSize; k++ )
        {
            leftWinPower += powerPtr[k];
        }

    }
    /* right edge */
    else if((int32_t) index >= (int32_t)fftSize - totalWinSize) 
    {
        for (k = 0; k < rightrepeat ; k++)
        {
            rightWinPower += powerPtr[k];
        }
        for (k = (int32_t)index + guardSize + 1; k < (int32_t) fftSize; k++)
        {
            rightWinPower += powerPtr[k];
        }
        for (k = (int32_t) index - totalWinSize; k < (int32_t) index - guardSize; k++ )
        {
            leftWinPower += powerPtr[k];
        }
    }

    else
    {
     for (k = (int32_t)index - totalWinSize; k < (int32_t) index - guardSize; k++ )
     {
         leftWinPower += powerPtr[k];
     }
     for (k = (int32_t)index + guardSize + 1; k <= (int32_t) index + totalWinSize; k++ )
     {
         rightWinPower += powerPtr[k];
     }
    }

    power2f         =    (__float2_t)_ftof2( rightWinPower,  leftWinPower);
    if (rightWinPower > leftWinPower)
    {
     power2f     =    (__float2_t) _ftof2( leftWinPower,  rightWinPower);
    }
    power2f         =    _dmpysp(power2f, scale);
    totalPower     =    _hif2(power2f) + _lof2(power2f);
    threshold     =    totalPower * relThr;

    *noise          =    totalPower;

    peakFlag = 0;
    if (powerPtr[index] > threshold)
    {
        peakFlag = 1U;
    }
   
     return peakFlag;
    
}


    

