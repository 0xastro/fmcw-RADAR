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
  *   @file     mmwavelib_aoaest_float.c
  *
  *   @brief    This file contains routines related to AOA estimation using
  *             beamforming method.
*/
#include <c6x.h>
#include <ti/alg/mmwavelib/include/mmwavelib_cplx_types.h>
#include <ti/alg/mmwavelib/include/mmwavelib_c674_emulate_c66_intrinsics.h>
#include <stdint.h>
#include "mmwavelib_aoaest_float.h"

/*A constant used in angle variance estimation */
#define MMWAVELIB_AOAESTBF_VAREST_CONST 1 /* 1: no adjustment*/

/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_aoaEstBFSinglePeak
 *
 * \par
 * <b>Description</b>  :    Estimate the AOA, and estimted AOA variance of single detected object using beamforming method
 *
 *
 * @param        [in]       numAnt          : number of antenna
 * @param        [in]       noise           : input noise variance of all receiver antenna
 * @param        [in]       estResolution   : angle resolution
 * @param        [in]       sigIn           : input antenna samples
 * @param        [in]       steeringVecSize : the number of steering vectors
 * @param        [in]       steeringVec     : Pointer to the steering vectors with size of steeringVecSize*(numAnt-1)
 *                                            the steering vector for first antenna is always 1, no need to save it
 * @param        [in]       scratchPad      : scratch memory buffer to save the angular spectrum with size of steeringVecSize
 * @param        [out]      estVar          : Output variance of the detected angle.
 * @param        [out]      angleEst        : Output angle estimate.
 *                                          
 * 
 *
 * @return                  Number of detected peaks, for this function, it is always 1.
 *
 * @pre                     sigIn and steeringVec are 8 byte aligned.
 *
 * @ingroup                 MMWAVELIB_AOAESTIMATION
 * 
 * cycles                   4 antenna: 7.31*steeringVecSize+489 (cgt 8.1.3)
 *                          8 antenna: 18.74*steeringVecSize+122
 *                          16 antenna: 126*steeringVecSize+195, if steeringVecSize < 200
 *                          24 antenna: 180*steeringVecSize+128, if steeringVecSize < 200
 *
 *
 *******************************************************************************************************************
 */

int32_t  mmwavelib_aoaEstBFSinglePeak(
                              uint8_t numAnt,
                              float32_t   noise,
                              float32_t   estResolution,
                              cplxf_t sigIn[],
                              int32_t steeringVecSize,
                              cplxf_t * steeringVec,
                              float32_t scratchPad[],
                              float32_t * estVar,
                              int32_t * angleEst)
{
    int32_t     i, maxInd, leftInd, rightInd, temp3dBSpan;
    float32_t       * restrict doaSpectrum;
    float32_t       tempPow, totPower, signalPower, maxPow, thre3dB, tempVar, inputPower, tempVarSqrInv, tempInterpol;
    __float2_t  input0,input1, f2temp1;
    __float2_t  input;
    __float2_t  * restrict tempSteerVecPtr;

    /*
         Solve: spectrum = A'*Rn*A = |A'x|.^2;
         where:
                 A = [steeringVec(theta)]  is a nRxAnt by numAngles matrix
                 Rn is the covariance matrix of the antenna signal
                 x is the input signal vector
    */

    doaSpectrum     =   (float32_t *) &scratchPad[0];

    /* Compute the spectrum and record the max */
    totPower    =   0.f;
    maxPow      =   0.f;
    maxInd      =   -1;
    inputPower  =   0.f;
    if ( numAnt == 4U )
    {
        __float2_t  input2, input3;

        input0          =   _amem8_f2(&sigIn[0]);
        input1          =   _amem8_f2(&sigIn[1U]);
        input2          =   _amem8_f2(&sigIn[2U]);
        input3          =   _amem8_f2(&sigIn[3U]);
        input           =   _dmpysp(input0,input0);
        inputPower      +=  _hif2(input) + _lof2(input);
        input           =   _dmpysp(input1,input1);
        inputPower      +=  _hif2(input) + _lof2(input);
        input           =   _dmpysp(input2,input2);
        inputPower      +=  _hif2(input) + _lof2(input);
        input           =   _dmpysp(input3,input3);
        inputPower      +=  _hif2(input) + _lof2(input);

        tempSteerVecPtr =   (__float2_t *) steeringVec;

        for (i = 0; i < steeringVecSize; i++)
        {
            f2temp1     =   _daddsp(input0, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input1));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input2));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input3));
            tempSteerVecPtr++;
            f2temp1     =   _dmpysp(f2temp1, f2temp1);
            tempPow     =   _hif2(f2temp1) + _lof2(f2temp1);
            doaSpectrum[i]  =   tempPow;
            totPower    +=  tempPow;
            if (tempPow > maxPow)
            {
                maxPow  =   tempPow;
                maxInd  =   i;
            }
        }
    }
    else if ( numAnt == 8U )
    {
        __float2_t input2, input3, input4, input5, input6, input7;

        input0          =   _amem8_f2(&sigIn[0]);
        input1          =   _amem8_f2(&sigIn[1U]);
        input2          =   _amem8_f2(&sigIn[2U]);
        input3          =   _amem8_f2(&sigIn[3U]);
        input4          =   _amem8_f2(&sigIn[4U]);
        input5          =   _amem8_f2(&sigIn[5U]);
        input6          =   _amem8_f2(&sigIn[6U]);
        input7          =   _amem8_f2(&sigIn[7U]);

        tempSteerVecPtr =   (__float2_t *) steeringVec;

        input           =   _dmpysp(input0,input0);
        inputPower      +=  _hif2(input) + _lof2(input);
        input           =   _dmpysp(input1,input1);
        inputPower      +=  _hif2(input) + _lof2(input);
        input           =   _dmpysp(input2,input2);
        inputPower      +=  _hif2(input) + _lof2(input);
        input           =   _dmpysp(input3,input3);
        inputPower      +=  _hif2(input) + _lof2(input);
        input           =   _dmpysp(input4,input4);
        inputPower      +=  _hif2(input) + _lof2(input);
        input           =   _dmpysp(input5,input5);
        inputPower      +=  _hif2(input) + _lof2(input);
        input           =   _dmpysp(input6,input6);
        inputPower      +=  _hif2(input) + _lof2(input);
        input           =   _dmpysp(input7,input7);
        inputPower      +=  _hif2(input) + _lof2(input);

        for (i = 0; i < steeringVecSize; i++)
        {
            f2temp1     =   _daddsp(input0, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input1));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input2));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input3));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input4));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input5));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input6));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input7));
            tempSteerVecPtr++;
            f2temp1     =   _dmpysp(f2temp1, f2temp1);
            tempPow     =   _hif2(f2temp1) + _lof2(f2temp1);
            doaSpectrum[i]  =   tempPow;
            totPower    +=  tempPow;
            if (tempPow > maxPow)
            {
                maxPow  =   tempPow;
                maxInd  =   i;
            }
        }
    }
    else
    {
        uint32_t j;
        __float2_t inputTemp;

        input0          =   _amem8_f2(&sigIn[0]);
        input           =   _dmpysp(input0, input0);
        inputPower      +=  _hif2(input) + _lof2(input);

        input1          =   _amem8_f2(&sigIn[1]);
        input           =   _dmpysp(input1, input1);
        inputPower      +=  _hif2(input) + _lof2(input);

        tempSteerVecPtr =   (__float2_t *) steeringVec;

        for (j = 2U; j < numAnt; j++)
        {
            inputTemp       =   _amem8_f2(&sigIn[j]);
            input       =   _dmpysp(inputTemp, inputTemp);
            inputPower      +=  _hif2(input) + _lof2(input);

        }



        for (i = 0; i < steeringVecSize; i++)
        {
            /* sigIn[0]+steeringVec[0]*conj(sigIn[1])*/

            f2temp1     =   _daddsp(input0, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input1));
            tempSteerVecPtr++;
            for (j = 2U; j < numAnt; j++ )
            {
                inputTemp   =   _amem8_f2(&sigIn[j]);
                f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), inputTemp));
                tempSteerVecPtr++;
            }
            f2temp1     =   _dmpysp(f2temp1, f2temp1);
            tempPow     =   _hif2(f2temp1) + _lof2(f2temp1);
            doaSpectrum[i]  =   tempPow;
            totPower    +=  tempPow;
            if (tempPow > maxPow)
            {
                maxPow  =   tempPow;
                maxInd  =   i;
            }
        }

    }
    thre3dB     =   maxPow * 0.5f;
    signalPower =   0.f;
    leftInd     =   maxInd;
    while ((doaSpectrum[leftInd] >= thre3dB) && (leftInd >= 0))
    {
        signalPower     +=  doaSpectrum[leftInd];
        leftInd--;
        if (leftInd < 0)
        {
            leftInd     =   steeringVecSize - 1;
        }
    }

    /* leftInd is the point index, whose power is < thre3dB*/
    rightInd     =  maxInd + 1;
    while ((doaSpectrum[rightInd] >= thre3dB) && (rightInd < steeringVecSize))
    {
        signalPower     +=  doaSpectrum[rightInd];
        rightInd++;
        if (rightInd == steeringVecSize)
        {
            rightInd    =   0;
        }
    }

    /* rightInd is the point index, whose power is < thre3dB*/

    temp3dBSpan         =   rightInd - (leftInd + 1);
    if (temp3dBSpan < 0)
    {
        temp3dBSpan     +=  steeringVecSize;
    }
    /* 2 * signalInputPower * sig3dBpower*numAnt*/
    tempVarSqrInv       =   2.f * (float32_t) MMWAVELIB_AOAESTBF_VAREST_CONST * (float32_t) MMWAVELIB_AOAESTBF_VAREST_CONST * inputPower * (float32_t)  numAnt * signalPower;
    /*1 /(noise * sumPowerAllSteering) */
    tempInterpol        =   _rcpsp(noise * totPower);
    tempInterpol        =   tempInterpol * (2.f - noise * totPower * tempInterpol);
    tempInterpol        =   tempInterpol * (2.f - noise * totPower * tempInterpol);

    /* 2 * signalInputPower * sig3dbpower * numAnt/(noise*sumPowerAllSteering*/
    tempVarSqrInv       *=  tempInterpol;

    /* sqrt(1/tempVarSqrInv)*/
    tempInterpol        =   _rsqrsp(tempVarSqrInv);
    tempInterpol        =   tempInterpol * (1.5f - (0.5f * tempVarSqrInv) * tempInterpol * tempInterpol);
    tempInterpol        =   tempInterpol * (1.5f - (0.5f * tempVarSqrInv) * tempInterpol * tempInterpol);

    /* 3db bandwidth */
    tempVar             =   estResolution * (float32_t)  temp3dBSpan;
    tempVar             *=  tempInterpol;

    *estVar             =   tempVar;
    *angleEst           =   maxInd;

    return(1);
}


/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_aoaEstBFSinglePeakDet
 *
 * \par
 * <b>Description</b>  :    Estimate the AOA only of single detected object using beamforming method
 *
 *
 * @param        [in]       numAnt          : number of antenna
 * @param        [in]       sigIn           : input antenna samples
 * @param        [in]       steeringVecSize : the number of steering vectors
 * @param        [in]       steeringVec     : Pointer to the steering vectors with size of steeringVecSize*(numAnt-1)
 *                                            the steering vector for first antenna is always 1, no need to save it
 * @param        [out]      angleEst        : Output angle estimate.
 *
 *
 *
 * @return                  Number of detected peaks, for this function, it is always 1.
 *
 * @pre                     sigIn and steeringVec are 8 byte aligned.
 *
 * @ingroup                 MMWAVELIB_AOAESTIMATION
 *
 * cycles                   4 antenna: 6.99*steeringVecSize+337 (cgt 8.1.3)
 *                          8 antenna: 16.78*steeringVecSize+64, if steeringVecSize < 200
 *                          16 antenna: 117.69*steeringVecSize, if steeringVecSize < 200
 *                          24 antenna: 170.83*steeringVecSize+757, if steeringVecSize < 200
 *
 *
 *******************************************************************************************************************
 */

int32_t  mmwavelib_aoaEstBFSinglePeakDet(
                              uint8_t numAnt,
                              cplxf_t sigIn[],
                              int32_t steeringVecSize,
                              cplxf_t * steeringVec,
                              int32_t * angleEst)
{
    int32_t     i, maxInd;
    float32_t       tempPow,maxPow;
    __float2_t  input0,input1, f2temp1;
    __float2_t  * restrict tempSteerVecPtr;

    /*
         Solve: spectrum = A'*Rn*A = |A'x|.^2;
         where:
                 A = [steeringVec(theta)]  is a nRxAnt by numAngles matrix
                 Rn is the covariance matrix of the antenna signal
                 x is the input signal vector
         Compute the spectrum and record the max
    */


    maxInd      =   -1;
    maxPow      =   0.f;
    if ( numAnt == 4U )
    {
        __float2_t  input2, input3;

        input0          =   _amem8_f2(&sigIn[0]);
        input1          =   _amem8_f2(&sigIn[1]);
        input2          =   _amem8_f2(&sigIn[2]);
        input3          =   _amem8_f2(&sigIn[3]);
        tempSteerVecPtr =   (__float2_t *) steeringVec;

        for (i = 0; i < steeringVecSize; i++ )
        {
            f2temp1     =   _daddsp(input0, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input1));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input2));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input3));
            tempSteerVecPtr++;
            f2temp1     =   _dmpysp(f2temp1, f2temp1);
            tempPow     =   _hif2(f2temp1) + _lof2(f2temp1);
            if (tempPow > maxPow)
            {
                maxPow  =   tempPow;
                maxInd  =   i;
            }
        }
    }
    else if ( numAnt == 8U )
    {
        __float2_t input2, input3, input4, input5, input6, input7;

        input0          =   _amem8_f2(&sigIn[0]);
        input1          =   _amem8_f2(&sigIn[1U]);
        input2          =   _amem8_f2(&sigIn[2U]);
        input3          =   _amem8_f2(&sigIn[3U]);
        input4          =   _amem8_f2(&sigIn[4U]);
        input5          =   _amem8_f2(&sigIn[5U]);
        input6          =   _amem8_f2(&sigIn[6U]);
        input7          =   _amem8_f2(&sigIn[7U]);

        tempSteerVecPtr =   (__float2_t *) steeringVec;

        for (i = 0; i < steeringVecSize; i++ )
        {
            f2temp1     =   _daddsp(input0, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input1));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input2));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input3));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input4));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input5));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input6));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input7));
            tempSteerVecPtr++;
            f2temp1     =   _dmpysp(f2temp1, f2temp1);
            tempPow     =   _hif2(f2temp1) + _lof2(f2temp1);
            if (tempPow > maxPow)
            {
                maxPow  =   tempPow;
                maxInd  =   i;
            }
        }
    }
    else
    {
        uint32_t j;
        __float2_t inputTemp;

        input0          =   _amem8_f2(&sigIn[0]);

        input1          =   _amem8_f2(&sigIn[1]);

        tempSteerVecPtr =   (__float2_t *) steeringVec;

        for (i = 0; i < steeringVecSize; i++ )
        {
            /* sigIn[0]+steeringVec[0]*conj(sigIn[1])*/

            f2temp1     =   _daddsp(input0, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input1));
            tempSteerVecPtr++;
            for (j = 2U; j < numAnt; j++ )
            {
                inputTemp   =   _amem8_f2(&sigIn[j]);
                f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), inputTemp));
                tempSteerVecPtr++;
            }
            f2temp1     =   _dmpysp(f2temp1, f2temp1);
            tempPow     =   _hif2(f2temp1) + _lof2(f2temp1);
            if (tempPow > maxPow)
            {
                maxPow  =   tempPow;
                maxInd  =   i;
            }
        }

    }
    *angleEst           =   maxInd;

    return(1);
}


/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_aoaEstBFMultiPeakDet
 *
 * \par
 * <b>Description</b>  :    Estimate the AOA only of multiple detected objects using beamforming method
 *
 *
 * @param        [in]       numAnt          : number of antenna
 * @param        [in]       gamma           : threshold of detect a peak
 * @param        [in]       sidelobeLevel   : relative threshold to skip the lower peaks which potentially
 *                                            can be a sidelobe from the main peak
 * @param        [in]       sigIn           : input antenna samples
 * @param        [in]       steeringVecSize : the number of steering vectors
 * @param        [in]       steeringVec     : Pointer to the steering vectors with size of steeringVecSize*(numAnt-1)
 *                                            the steering vector for first antenna is always 1, no need to save it
 * @param        [in]       scratchPad      : scratch memory buffer to save the angular spectrum with size of steeringVecSize
 * @param        [out]      angleEst        : Output angle estimate.
 *
 *
 *
 * @return                  Number of detected peaks.
 *
 * @pre                     sigIn and steeringVec are 8 byte aligned.
 *
 * @ingroup                 MMWAVELIB_AOAESTIMATION
 *
 * cycles                   4 antenna: 28.53*steeringVecSize+500 (cgt 8.1.3)
 *                          8 antenna: 41.31*steeringVecSize+37
 *                          16 antenna: 142.33*steeringVecSize+16, if steeringVecSize < 200
 *                          24 antenna: 194.42*steeringVecSize+908, if steeringVecSize < 200
 *
 *******************************************************************************************************************
 */

int32_t mmwavelib_aoaEstBFMultiPeakDet(
                                uint8_t numAnt,
                                float32_t gamma,
                                float32_t sidelobeLevel,
                                cplxf_t * sigIn,
                                cplxf_t * steeringVec,
                                int32_t steeringVecSize,
                                float32_t   * scratchPad,
                                int32_t * angleEst)
{
    float32_t minVal = (float32_t) (3.0e+30); /*INFINITY*/
    float32_t currentVal;
    float32_t peakThreshold;
    float32_t maxVal;
    float32_t gamma_inv = 1.f/gamma;
    int32_t locateMax = 0;
    int32_t runningInd;
    int32_t     i;
    int32_t extendLoc, ind_loc, maxLoc,  numMax, initStage;
    float32_t       tempPow, maxPow;
    __float2_t  input0;
    __float2_t  * restrict tempSteerVecPtr;
    float32_t * restrict doaSpectrum;


    /*
        Solve: spectrum = A'*Rn*A = |A'x|.^2;
        where:
              A = [steeringVec(theta)]  is a nRxAnt by numAngles matrix
              Rn is the covariance matrix of the antenna signal
              x is the input signal vector
        compute the angular spectrum, and find out the maximum power
    */

    doaSpectrum  =   (float32_t *) &scratchPad[0];
    maxPow      =   0.f;
    if ( numAnt == 4U )
    {
        __float2_t input1, input2, input3, f2temp1;

        input0          =   _amem8_f2(&sigIn[0]);
        input1          =   _amem8_f2(&sigIn[1U]);
        input2          =   _amem8_f2(&sigIn[2U]);
        input3          =   _amem8_f2(&sigIn[3U]);
        tempSteerVecPtr =   (__float2_t *) steeringVec;

        for (i = 0; i < steeringVecSize; i++ )
        {
            f2temp1     =   _daddsp(input0, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input1));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input2));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input3));
            tempSteerVecPtr++;
            f2temp1     =   _dmpysp(f2temp1, f2temp1);
            tempPow     =   _hif2(f2temp1) + _lof2(f2temp1);
            doaSpectrum[i]  =   tempPow;
            if (tempPow > maxPow)
            {
                maxPow  =   tempPow;
            }
        }
    }
    else if ( numAnt == 8U )
    {
        __float2_t input1, input2, input3, input4, input5, input6, input7, f2temp1;

        input0          =   _amem8_f2(&sigIn[0]);
        input1          =   _amem8_f2(&sigIn[1U]);
        input2          =   _amem8_f2(&sigIn[2U]);
        input3          =   _amem8_f2(&sigIn[3U]);
        input4          =   _amem8_f2(&sigIn[4U]);
        input5          =   _amem8_f2(&sigIn[5U]);
        input6          =   _amem8_f2(&sigIn[6U]);
        input7          =   _amem8_f2(&sigIn[7U]);

        tempSteerVecPtr =   (__float2_t *) steeringVec;

        for (i = 0; i < steeringVecSize; i++ )
        {
            f2temp1     =   _daddsp(input0, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input1));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input2));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input3));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input4));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input5));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input6));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input7));
            tempSteerVecPtr++;
            f2temp1     =   _dmpysp(f2temp1, f2temp1);
            tempPow     =   _hif2(f2temp1) + _lof2(f2temp1);
            doaSpectrum[i]  =   tempPow;
            if (tempPow > maxPow)
            {
                maxPow  =   tempPow;
            }
        }
    }
    else
    {
        uint32_t j;
        __float2_t input1, f2temp1, inputTemp;

        input0          =   _amem8_f2(&sigIn[0]);
        tempSteerVecPtr =   (__float2_t *) steeringVec;

        input1          =   _amem8_f2(&sigIn[1]);
        for (i = 0; i < steeringVecSize; i++ )
        {
            f2temp1     =   _daddsp(input0, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input1));
            tempSteerVecPtr++;
            for (j = 2U; j < numAnt; j++ )
            {
                inputTemp       =   _amem8_f2(&sigIn[j]);
                f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), inputTemp));
                tempSteerVecPtr++;
            }
            f2temp1     =   _dmpysp(f2temp1, f2temp1);
            tempPow     =   _hif2(f2temp1) + _lof2(f2temp1);
            doaSpectrum[i]  =   tempPow;
            if (tempPow > maxPow)
            {
                maxPow  =   tempPow;
            }
        }
    }

    /* to avoid the sidelobe */
    peakThreshold = maxPow * sidelobeLevel;


    /* Multiple Peaks search */
    runningInd = 0;
    numMax = 0;
    extendLoc = 0;
    initStage = 1;
    maxVal = 0.0;
    maxLoc = 0;
    while (runningInd < (steeringVecSize + extendLoc))
    {
        if (runningInd >= steeringVecSize)
        {
            ind_loc = runningInd - steeringVecSize;
        }
        else
        {
            ind_loc = runningInd;
        }

        currentVal = doaSpectrum[ind_loc];

         /* Record the local maximum value and its location */
         if (currentVal > maxVal)
         {
             maxVal = currentVal;
             maxLoc = ind_loc;
         }

         /* Record the local minimum value */
         if (currentVal < minVal)
         {
              minVal = currentVal;
         }

         if (locateMax == 1)
         {
             /* peak search */
             if (currentVal < maxVal*gamma_inv) /* find the bandwidth */
             {
                 /* ignore the lower peaks to avoid detect sidelobe of the main peaks */
                 if (maxVal >= peakThreshold)
                 {
                     /* the previous maximum is a peak,  Assign maximum value only if the value has
                       fallen below the max by gamma, and avoid the sidelobe, thereby declaring that the max value was a peak
                      */
                     angleEst[numMax]   = maxLoc;
                     numMax++;
                 }
                 minVal = currentVal;
                 locateMax = 0;
             }
         }
         else
         {
             /* find the bottom */
             if (currentVal > minVal * gamma)
             {
                 /* Assign minimum value if the value has risen above the min by
                  gamma, thereby declaring that the min value was a valley
                 */
                 locateMax = 1;
                 maxVal = currentVal;
                 if (initStage == 1)
                 {
                     extendLoc = runningInd;
                     initStage = 0;
                 }

             }
         }

         runningInd++;
    }

    return(numMax);
}



/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_aoaEstBFMultiPeak
 *
 * \par
 * <b>Description</b>  :    Estimate the AOA, and AOA variance of multiple detected objects using beamforming method
 *
 * @param        [in]       numAnt          : the number of receiver antenna
 * @param        [in]       gamma           : threshold of detect a peak
 * @param        [in]       sidelobeLevel   : relative threshold to skip the lower peaks which potentially
 *                                            can be a sidelobe from the main peak
 * @param        [in]       widthAdjust_3dB : the peak search will find the gamma bandwidth of the peak, we adjust the gamma bandwidth
 *                                            to get the 3dB bandwidth of a detected peak
 * @param        [in]       inputSNR        : the signal to noise ratio in linear format for the input signal samples
 * @param        [in]       estResolution   : the steerring vector angle resolution
 * @param        [in]       sigIn           : input antenna samples
 * @param        [in]       steeringVecSize : the number of steering vectors
 * @param        [in]       steeringVec     : Pointer to the steering vectors with size of steeringVecSize*(numAnt-1)
 *                                            the steering vector for first antenna is always 1, no need to save it
 * @param        [in]       scratchPad      : scratch memory buffer to save the angular spectrum with size of steeringVecSize+3*maximumpeaks
 * @param        [out]      estVar          : Output angle estimate variances.
 * @param        [out]      angleEst        : Output angle estimates.
 *
 *
 *
 * @return                  Number of detected peaks.
 *
 * @pre                     sigIn and steeringVec are 8 byte aligned.
 *
 * @ingroup                 MMWAVELIB_AOAESTIMATION
 *
 * cycles                   4 antenna: 91.31*steeringVecSize+497 (cgt 8.1.3)
 *                          8 antenna: 91.52*steeringVecSize
 *                          16 antenna: 191.0*steeringVecSize + 41, if steeringVecSize < 100
 *                          24 antenna: 190.06*steeringVecSize+539, if steeringVecSize < 100
 *
 *******************************************************************************************************************
 */

int32_t mmwavelib_aoaEstBFMultiPeak(
                                uint8_t numAnt,
                                float32_t gamma,
                                float32_t sidelobeLevel,
                                float32_t widthAdjust_3dB,
                                float32_t inputSNR,
                                float32_t estResolution,
                                cplxf_t * sigIn,
                                cplxf_t * steeringVec,
                                int32_t steeringVecSize,
                                float32_t   * scratchPad,
                                float32_t   * estVar,
                                int32_t * angleEst)
{
    int32_t  ind_obj;
    float32_t * restrict doaSpectrum;
    mmwavelib_aoAEstBF_peakRecord_t * restrict maxData;
    int32_t extendLoc, ind_loc, maxLoc, maxLoc_r, numMax, bwidth, initStage;
    float32_t minVal = (float32_t) (3.0e+30); /*INFINITY*/
    float32_t currentVal;
    float32_t peakThreshold;
    float32_t maxVal;
    float32_t peakWidth;
    float32_t gamma_inv = 1.f/gamma;
    float32_t snr;
    int32_t locateMax = 0;
    int32_t runningInd;

     int32_t    i;
     float32_t      tempPow, totPower, maxPow,tempInterpol;
     __float2_t     input0;
     __float2_t     * restrict tempSteerVecPtr;


     /*
        Solve: spectrum = A'*Rn*A = |A'x|.^2;
        where:
              A = [steeringVec(theta)]  is a nRxAnt by numAngles matrix
              Rn is the covariance matrix of the antenna signal
              x is the input signal vector
     */

     doaSpectrum  =   (float32_t *) &scratchPad[0];
     maxData      =   (mmwavelib_aoAEstBF_peakRecord_t *) &scratchPad[steeringVecSize];

     /* Compute the spectrum and record the max */

    totPower    =   0.f;
    maxPow      =   0.f;
    if ( numAnt == 4U )
    {
        __float2_t input1, input2, input3, f2temp1;

        input0          =   _amem8_f2(&sigIn[0]);
        input1          =   _amem8_f2(&sigIn[1U]);
        input2          =   _amem8_f2(&sigIn[2U]);
        input3          =   _amem8_f2(&sigIn[3U]);
        tempSteerVecPtr =   (__float2_t *) steeringVec;

        for (i = 0; i < steeringVecSize; i++ )
        {
            f2temp1     =   _daddsp(input0, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input1));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input2));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input3));
            tempSteerVecPtr++;
            f2temp1     =   _dmpysp(f2temp1, f2temp1);
            tempPow     =   _hif2(f2temp1) + _lof2(f2temp1);
            doaSpectrum[i]  =   tempPow;
            totPower    +=  tempPow;
            if (tempPow > maxPow)
            {
                maxPow  =   tempPow;
            }
        }
    }
    else if ( numAnt == 8U )
    {
        __float2_t input1, input2, input3, input4, input5, input6, input7, f2temp1;

        input0          =   _amem8_f2(&sigIn[0]);
        input1          =   _amem8_f2(&sigIn[1U]);
        input2          =   _amem8_f2(&sigIn[2U]);
        input3          =   _amem8_f2(&sigIn[3U]);
        input4          =   _amem8_f2(&sigIn[4U]);
        input5          =   _amem8_f2(&sigIn[5U]);
        input6          =   _amem8_f2(&sigIn[6U]);
        input7          =   _amem8_f2(&sigIn[7U]);

        tempSteerVecPtr =   (__float2_t *) steeringVec;

        for (i = 0; i < steeringVecSize; i++ )
        {
            f2temp1     =   _daddsp(input0, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input1));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input2));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input3));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input4));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input5));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input6));
            tempSteerVecPtr++;
            f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input7));
            tempSteerVecPtr++;
            f2temp1     =   _dmpysp(f2temp1, f2temp1);
            tempPow     =   _hif2(f2temp1) + _lof2(f2temp1);
            doaSpectrum[i]  =   tempPow;
            totPower    +=  tempPow;
            if (tempPow > maxPow)
            {
                maxPow  =   tempPow;
            }
        }
    }
    else
    {
        uint32_t j;
        __float2_t input1, f2temp1, inputTemp;

        input0          =   _amem8_f2(&sigIn[0]);
        tempSteerVecPtr =   (__float2_t *) steeringVec;

        input1          =   _amem8_f2(&sigIn[1U]);
        for (i = 0; i < steeringVecSize; i++ )
        {
            f2temp1     =   _daddsp(input0, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), input1));
            tempSteerVecPtr++;
            for (j = 2U; j < numAnt; j++ )
            {
                inputTemp       =   _amem8_f2(&sigIn[j]);
                f2temp1     =   _daddsp(f2temp1, _complex_conjugate_mpysp(_amem8_f2(tempSteerVecPtr), inputTemp));
                tempSteerVecPtr++;
            }
            f2temp1     =   _dmpysp(f2temp1, f2temp1);
            tempPow     =   _hif2(f2temp1) + _lof2(f2temp1);
            doaSpectrum[i]  =   tempPow;
            if (tempPow > maxPow)
            {
                maxPow  =   tempPow;
            }
        }
    }

    peakThreshold = maxPow * sidelobeLevel;


    /* Multiple Peak search */
    runningInd = 0;
    numMax = 0;
    extendLoc = 0;
    initStage = 1;
    maxVal = 0.0;
    totPower = 0.f;
    maxLoc = 0;
    maxLoc_r = 0;
    while (runningInd < (steeringVecSize + extendLoc))
    {
        if (runningInd >= steeringVecSize)
        {
            ind_loc = runningInd - steeringVecSize;
        }
        else
        {
            ind_loc = runningInd;
        }
        currentVal = doaSpectrum[ind_loc];

         /* Record the local maximum value and its location */
         if (currentVal > maxVal)
         {
             maxVal = currentVal;
             maxLoc = ind_loc;
             maxLoc_r = runningInd;
         }

         /* Record the local minimum value */
         if (currentVal < minVal)
         {
              minVal = currentVal;
         }

         if (locateMax == 1)
         {
             /* peak search */
             if (currentVal < maxVal*gamma_inv)
             {
                 /* ignore the lower peaks to avoid detect sidelobe of the main peaks.*/
                 if (maxVal >= peakThreshold)
                 {
                     /* Assign maximum value only if the value has fallen below the max by
                      gamma, thereby declaring that the max value was a peak
                     */
                     bwidth = runningInd - maxLoc_r;
                     maxData[numMax].peakLoc   = maxLoc;
                     maxData[numMax].peakValue = maxVal;
                     maxData[numMax].peakWidth = bwidth;
                     totPower += maxVal;
                     numMax++;
                 }
                 minVal = currentVal;
                 locateMax = 0;
             }
         }
         else
         {
             /* bottom search */
             if (currentVal > minVal * gamma)
             {
                 /* Assign minimum value if the value has risen above the min by
                  gamma, thereby declaring that the min value was a valley
                 */
                 locateMax = 1;
                 maxVal = currentVal;
                 if (initStage == 1)
                 {
                     extendLoc = runningInd ;
                     initStage = 0;
                 }

             }
         }
         runningInd++;
    }

    for (ind_obj = 0; ind_obj < numMax; ind_obj ++)
    {
        /* variance estimation
            estVar(ind) = 2*angleStep*peakWidth_gamma(ind)*widthAdjust_3dB(peakLoc(ind))/...
                (km * sqrt(2*10^(estSNR/10)*maxData(ind, 2)/totPower));
        */
        peakWidth = 2.f*estResolution*maxData[ind_obj].peakWidth*widthAdjust_3dB;
        snr = 2.f*inputSNR*maxData[ind_obj].peakValue / totPower;
        /*sqrt(1/snr)*/

        tempInterpol        =   _rsqrsp(snr);
        tempInterpol        =   tempInterpol * (1.5f - (0.5f * snr) * tempInterpol * tempInterpol);
        tempInterpol        =   tempInterpol * (1.5f - (0.5f * snr) * tempInterpol * tempInterpol);


        estVar[ind_obj] =  (float32_t) (peakWidth / ((float32_t) (MMWAVELIB_AOAESTBF_VAREST_CONST)) * tempInterpol );
        angleEst[ind_obj] = maxData[ind_obj].peakLoc;
    }

    return(numMax);
}


