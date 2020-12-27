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
 *   @file     mmwavelib_poweracc_float.c
 *
 *   @brief    Routines for floating point signal power accumulation
 */

#include <stdint.h>
#include <c6x.h>
#include <ti/alg/mmwavelib/include/mmwavelib_c674_emulate_c66_intrinsics.h>
#include "mmwavelib_poweracc_float.h"

/*!**************************************************************************************
 * @brief       Function mmwavelib_accumPowerFltpAllAnt accumulates number of fftSize signal
 *              power over all antenna. It can be called per range bin or dopper bin.
 *
 *
 * @param[in]   fftSize   : Input FFT sizes.
 * @param[in]   numAnt    : number of accumulated RX antennas.
 * @param[in]   inputPtr  : floating-point I/Q complex signal input, size of fftSize*2*numAnt
 *                           the data is stored one antenna after another.
 *                           Must be aligned to 8-byte boundary.
 * @param[out]   outputPtr : accumulated power output (non-coherent combining).
 *                           Must be aligned to 8-byte boundary.
 *
 * @return      void
 *
 * @pre         (1)inputPtr and outputPtr arrays both are 8 byte aligned
 *
 * \ingroup     MMWAVELIB_VEC_UTILS
 *
 * Cycles       L1P and L1D cache is set to 32K. Input and output are all
 *              floating point version. (8.1.3)
 *             1 Antenna: 1*fftsize+61,
 *             2 Antenna: 2.5*fftsize+90, except 1024 with 4224
 *             4 Antenna: 4.08*fftsize+53 except 1024 with 27076
 *             8 Antenna: 9.16*fftsize+61, except 512 with 39526, and 1024 with 83316
 ****************************************************************************************
 */


void  mmwavelib_accumPowerFltpAllAnt(
                              uint32_t fftSize,
                              int32_t numAnt,
                              float32_t * inputPtr,
                              float32_t * outputPtr)
{
    int32_t     j;
    __float2_t  lltemp1;
    __float2_t  results1, results2;

    if (numAnt == 4)
    {
        __float2_t  * restrict input1;
        __float2_t  * restrict input2;
        __float2_t  * restrict input3;
        __float2_t  * restrict input4;
        __float2_t  *restrict output;

        input1 = (__float2_t  *) inputPtr;
        input2 = (__float2_t  *) &inputPtr[2U * fftSize];
        input3 = (__float2_t  *) &inputPtr[4U * fftSize];
        input4 = (__float2_t  *) &inputPtr[6U * fftSize];
        output = (__float2_t *)     outputPtr;
        for (j = 0; j < (int32_t) fftSize; j += 2)
        {
            lltemp1     =   _amem8_f2(input1);
            input1++;
            results1    =   _dmpysp(lltemp1,lltemp1);
            lltemp1     =   _amem8_f2(input2);
            input2++;
            results1    =   _daddsp(results1, _dmpysp(lltemp1,lltemp1));
            lltemp1     =   _amem8_f2(input3);
            input3++;
            results1    =   _daddsp(results1, _dmpysp(lltemp1,lltemp1));
            lltemp1     =   _amem8_f2(input4);
            results1    =   _daddsp(results1, _dmpysp(lltemp1,lltemp1));
            input4++;
            lltemp1     =   _amem8_f2(input1);
            input1++;
            results2    =   _dmpysp(lltemp1,lltemp1);
            lltemp1     =   _amem8_f2(input2);
            input2++;
            results2    =   _daddsp(results2, _dmpysp(lltemp1,lltemp1));
            lltemp1     =   _amem8_f2(input3);
            input3++;
            results2    =   _daddsp(results2, _dmpysp(lltemp1,lltemp1));
            lltemp1     =   _amem8_f2(input4);
            input4++;
            results2    =   _daddsp(results2, _dmpysp(lltemp1,lltemp1));

            _amem8_f2(output)   =   _ftof2(_hif2(results2) + _lof2(results2), _hif2(results1) + _lof2(results1));
            output++;
        }
    }
    else if (numAnt == 8)
    {
        __float2_t  * restrict input1;
        __float2_t  * restrict input2;
        __float2_t  * restrict input3;
        __float2_t  * restrict input4;
        __float2_t  * restrict input5;
        __float2_t  * restrict input6;
        __float2_t  * restrict input7;
        __float2_t  * restrict input8;
        float32_t   * restrict output;

        input1 = (__float2_t  *) inputPtr;
        input2 = (__float2_t  *) &inputPtr[2U * fftSize];
        input3 = (__float2_t  *) &inputPtr[4U * fftSize];
        input4 = (__float2_t  *) &inputPtr[6U * fftSize];
        input5 = (__float2_t  *) &inputPtr[8U * fftSize];
        input6 = (__float2_t  *) &inputPtr[10U * fftSize];
        input7 = (__float2_t  *) &inputPtr[12U * fftSize];
        input8 = (__float2_t  *) &inputPtr[14U * fftSize];
        output = (float32_t *)  outputPtr;
        for (j = 0; j < (int32_t) fftSize; j++)
        {
            lltemp1     =   _amem8_f2(input1);
            input1++;
            results1    =   _dmpysp(lltemp1,lltemp1);
            lltemp1     =   _amem8_f2(input2);
            input2++;
            results1    =   _daddsp(results1, _dmpysp(lltemp1,lltemp1));
            lltemp1     =   _amem8_f2(input3);
            input3++;
            results1    =   _daddsp(results1, _dmpysp(lltemp1,lltemp1));
            lltemp1     =   _amem8_f2(input4);
            input4++;
            results1    =   _daddsp(results1, _dmpysp(lltemp1,lltemp1));
            lltemp1     =   _amem8_f2(input5);
            input5++;
            results1    =   _daddsp(results1, _dmpysp(lltemp1,lltemp1));
            lltemp1     =   _amem8_f2(input6);
            input6++;
            results1    =   _daddsp(results1, _dmpysp(lltemp1,lltemp1));
            lltemp1     =   _amem8_f2(input7);
            input7++;
            results1    =   _daddsp(results1, _dmpysp(lltemp1,lltemp1));
            lltemp1     =   _amem8_f2(input8);
            input8++;
            results1    =   _daddsp(results1, _dmpysp(lltemp1,lltemp1));
            *output =   _hif2(results1) + _lof2(results1);
            output++;
        }
    }
    else
    {
        __float2_t  * restrict input1;
        float32_t   * restrict output;
        int32_t i;
        /* first antenna */
        input1 = (__float2_t  *) inputPtr;
        output = (float32_t *)  outputPtr;
        for (j = 0; j < (int32_t) fftSize; j += 2)
        {
            lltemp1     =   _amem8_f2(input1);
            input1++;
            results1     =  _dmpysp(lltemp1,lltemp1);
            *output     =   _hif2(results1) + _lof2(results1);
            output++;
            lltemp1     =   _amem8_f2(input1);
            input1++;
            results1     =  _dmpysp(lltemp1,lltemp1);
            *output     =   _hif2(results1) + _lof2(results1);
            output++;
        }

        for (i = 1; i < numAnt; i++ )
        {
            input1 = (__float2_t  *) &inputPtr[2U * fftSize * i];
            output = (float32_t *)  outputPtr;
            for (j = 0; j < (int32_t) fftSize; j+=2)
            {
                lltemp1     =   _amem8_f2(input1);
                input1++;
                results1     =  _dmpysp(lltemp1,lltemp1);
                *output     +=  _hif2(results1) + _lof2(results1);
                output++;
                lltemp1     =   _amem8_f2(input1);
                input1++;
                results1     =  _dmpysp(lltemp1,lltemp1);
                *output     +=  _hif2(results1) + _lof2(results1);
                output++;
            }
        }
    }
}


/*!**************************************************************************************
 * @brief       Function mmwavelib_accumPowerFltp accumulates signal power of the input signal .
 *             
 *              if (accumFlag==0)
 *              {  for (j = 0; j < length; j++)
 *                    outputPtr[j]=inputPtr[2*j]^2+inputPtr[2*j+1]^2;
 *              }
 *              else //accuFlag !=0
 *              {   for (j = 0; j < length; j++)
 *                     outputPtr[j]+=(inputPtr[2*j]^2+inputPtr[2*j+1]^2);
 *              }
 *
 * @param[in]   length       : Input complex signal length.
 * @param[in]   accumFlag    : if 0, initialize the output as the input signal power 
 *                             if not 0, accumulate the input signal power to output power
 * @param[in]   inputPtr     : floating-point I/Q complex signal input, size of length*2,
 *                             Must be aligned to 8-byte boundary.
 * @param[out]   outputPtr   : accumulated power output (non-coherent combining).
 *                             Must be aligned to 8-byte boundary.
 *
 * @return      void
 *
 * @pre         (1)inputPtr and outputPtr arrays both are 8 byte aligned
 *
 * \ingroup     MMWAVELIB_VEC_UTILS
 *
 * Cycles       (cgt 8.1.3)
 *              1) accumFlag=0, 1*length+36
 *              2) accumFlag!=0, 1.5*length+49
 *
 * *************************************************************************
 */

void mmwavelib_accumPowerFltp(uint32_t length,
                        int32_t accumFlag,
                        float32_t * inputPtr,
                        float32_t * outputPtr)
{
    int32_t     j;
    __float2_t  * restrict input1;
    float32_t   * restrict output;
    __float2_t  lltemp1;
    __float2_t  results;

    if (accumFlag == 0)
    {
        input1 = (__float2_t  *) inputPtr;
        output = (float32_t *)  outputPtr;
        for (j = 0; j < (int32_t) length; j += 2)
        {
            lltemp1     =   _amem8_f2(input1);
            input1++;
            results     =   _dmpysp(lltemp1,lltemp1);
            *output     =   _hif2(results) + _lof2(results);
            output++;
            lltemp1     =   _amem8_f2(input1);
            input1++;
            results     =   _dmpysp(lltemp1,lltemp1);
            *output     =   _hif2(results) + _lof2(results);
            output++;
        }
    }
    else
    {

        input1 = (__float2_t  *) inputPtr;
        output = (float32_t *)  outputPtr;
        for (j = 0; j < (int32_t) length; j += 2)
        {
            lltemp1     =   _amem8_f2(input1);
            input1++;
            results     =   _dmpysp(lltemp1,lltemp1);
            *output     +=  _hif2(results) + _lof2(results);
            output++;
            lltemp1     =   _amem8_f2(input1);
            input1++;
            results     =   _dmpysp(lltemp1,lltemp1);
            *output     +=  _hif2(results) + _lof2(results);
            output++;
        }
    }
}
