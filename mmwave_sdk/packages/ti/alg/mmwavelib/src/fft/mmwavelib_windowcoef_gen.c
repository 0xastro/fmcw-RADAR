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
 *   @file     mmwavelib_windowcoef_gen.c
 *
 *   @brief    Contains functions related to generate windowing coefficients for FFT processing.
 *
 */

#include <stdint.h>
#include <ti/alg/mmwavelib/src/fft/mmwavelib_fft.h>



/*!************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_windowCoef_gen
 *
 * \par
 * <b>Description</b>  :    The function generates window coefficients in specified format. It 
 *                          calls single precision sine an cosine functions from mathlib library
 *                          then recursively generates the cosine values for the other samples 
 * 
 * @param[in]               windowDatumType  : generated windowing coefficients data format.
 *                                           @ref MMWAVELIB_WINDOW_INT16, int16_t                                     
 *                                           @ref MMWAVELIB_WINDOW_INT32, int32_t
 * @param[in]               initR      : cos(2*pi/(winLen-1)) value in floating point format
 *                                       can be obtained by using mathlib function cossp()
 * @param[in]               initI      : sin(2*pi/(winLen-1)) value in floating point format
 *                                       can be obtained by using mathlib function sinsp()
 * @param[in]               winCoefLen : The number of the generated window samples. The 
 *                                       generated window is real symmetric window. Only 
 *								         half of the window samples are generated. If the 
 *										 window length is even, winCoefLen is the half of 
 *										 the window length, if the window length is odd, 
 *										 winCoefLen is half of the window length plus one. 
 * @param[in]               Qformat    : Q format used for the generated widown coefficients.
 * @param[in]               winType    : Type of window, one of 
 *                                       @ref MMWAVELIB_WIN_HANNING, w(n)=0.5*(1-cos(2*pi*n/(N-1))                                     
 *                                       @ref MMWAVELIB_WIN_BLACKMAN, w(n)=0.42-0.5*cos(2*pi*n/(N-1))
 *                                                                  +0.08*cos(4*pi*n/(N-1))
 *                                      
 *                                       @ref MMWAVELIB_WIN_RECT, w(n) = 1 
 *                                       @ref MMWAVELIB_WIN_HAMMING, w(n) = 0.54-0.46*cos(2*pi*n/(N-1))
 * @param[out]              winCoef    : Pointer to calculated window sample array, with length of
 *                                       winCoefLen.
 * @return                  void
 *
 * @pre                     (1) Only support Blackman, Rectangle, Hamming, and Hanning windows
 * @pre                     (2) winCoef must be 8 byte aligned.
 * 
 * @ingroup                 MMWAVELIB_FFT
 *
 * Cycles (cgt 8.1.3)       Hanning/Hamming: 5 * winCoefLen + 103
 *                          Blackman:  6 * winCoefLen + 120
 *                          rectangle: 0.25 * winCoefLen + 74
 **************************************************************************************************
 */
#pragma FUNCTION_OPTIONS (mmwavelib_windowCoef_gen, "-ms0")
void mmwavelib_windowCoef_gen(void *  winCoef,
                              uint32_t windowDatumType,
                              float initR,
                              float initI,
                              uint32_t winCoefLen,
                              int32_t  Qformat,
                              uint32_t winType)
{
    uint32_t ii;
    uint64_t * winOut64;   
    uint32_t * winOut32;
    float stepR,stepI,stepR2, stepI2;
    float evenCos, evenSin, oddCos, oddSin;
    float evenCos2, evenSin2, oddCos2, oddSin2;
    float a0, a1, a2;
    float valueEven, valueOdd;
    float evenTemp, oddTemp;
    int32_t winValueEven, winValueOdd;
    int32_t *winCoefLast32;
    int16_t *winCoefLast16;
    int32_t oneQformat;
    
       
    _nassert((uint32_t) winCoef % 8U == 0);
    
    winOut64 = (uint64_t * )winCoef;
    winOut32 = (uint32_t * )winCoef;
    
    winCoefLast32 = (int32_t *)winCoef;
    winCoefLast16 = (int16_t *)winCoef;

    evenSin = 0.f;  /* sin(2*pi*0/N-1) */
    evenCos = 1.0f; /* cos(2*pi*0/N-1) */
    oddSin = initI; /* sin(2*pi*1/(N-1)) */
    oddCos = initR; /* cos(2*pi*1/(N-1)) */
    
    /* step is 2*pi*2/N-1*/
    stepR = 1.f - (initI * initI) * 2.f;     /* cos(2a)=1-2*sin(a)^2 */
    stepI = 2.f * initR * initI;             /* sin(2a)=2*sin(a)*cos(a) */
    
    oneQformat = 1 << Qformat;
    /*If rectangle window, using Q15, the window coefficients is 0x7fff, instead of 0x8000
     If Qformat is 31, the "1" is set to (1<<Qformat -1) to avoid the overflow in the 
     window coefficient calculations
     */
    if ( (winType == MMWAVELIB_WIN_RECT) || (Qformat == 31))
    {
        oneQformat = oneQformat - 1;
    }
    
    
    if ((winType == MMWAVELIB_WIN_HANNING) || (winType == MMWAVELIB_WIN_HAMMING))
    {
        if (winType == MMWAVELIB_WIN_HAMMING)
        {
            a0 = 0.54;
            a1 = 0.46;
        }
        else
        {
            a0 = 0.5;
            a1 = 0.5;
        }
        for (ii = 0; ii < winCoefLen / 2; ii++)
        {
            /* a0-a1*cos(2*pi*n/N-1))*/
            valueEven = a0 - a1 * evenCos;
            valueOdd =  a0 - a1 * oddCos;
            winValueEven = _spint(valueEven * oneQformat + 0.5); 
            winValueOdd  = _spint(valueOdd * oneQformat + 0.5) ;
            if (winValueEven >= oneQformat )
            {
                winValueEven = oneQformat - 1;
            }
            if (winValueOdd >= oneQformat )
            {
                winValueOdd = oneQformat - 1;
            }
            
            if (windowDatumType == MMWAVELIB_WINDOW_INT32)
            {
                _amem8(&winOut64[ii]) = _itoll(winValueOdd, winValueEven); 
            }
            if (windowDatumType == MMWAVELIB_WINDOW_INT16)
            {
                _amem4(&winOut32[ii]) = _pack2(winValueOdd, winValueEven); 
            }
            /* update */
            evenTemp = evenCos;
            /* cos(a+b) = cos(a)cos(b)-sin(a)sin(b) */
            evenCos =  evenCos * stepR - evenSin * stepI; 
            /* sin(a+b) = sin(a)cos(b)+cos(a)sin(b)*/
            evenSin = evenTemp * stepI + evenSin * stepR;
            
            oddTemp = oddCos;
            oddCos = oddCos * stepR - oddSin * stepI;
            oddSin = oddTemp * stepI + oddSin * stepR;
        }
        
        if (winCoefLen & 0x1)
        {
            valueEven = a0 - a1 * evenCos;
            winValueEven = _spint(valueEven * oneQformat + 0.5);  
            if (winValueEven >= oneQformat )
            {
                winValueEven = oneQformat - 1;
            }
            if (windowDatumType)
            {
                winCoefLast32[winCoefLen - 1] = winValueEven;
            }
            if (windowDatumType == MMWAVELIB_WINDOW_INT16)
            {
                winCoefLast16[winCoefLen - 1] = (int16_t) winValueEven;
            }
        }
    }
    else if (winType == MMWAVELIB_WIN_RECT)
    {
        if (windowDatumType == MMWAVELIB_WINDOW_INT32)
        {
        
            for (ii = 0; ii < winCoefLen / 2; ii++)
            {
                _amem8(&winOut64[ii]) = _itoll(oneQformat,oneQformat);
            }
        }
        if (windowDatumType == MMWAVELIB_WINDOW_INT16)
        {
            for (ii = 0; ii < winCoefLen / 2; ii++)
            {
                 _amem4(&winOut32[ii]) = _pack2(oneQformat, oneQformat);
            }
                
        }
    
        if (winCoefLen & 0x1)
        {
            if (windowDatumType == MMWAVELIB_WINDOW_INT32)
            {
                winCoefLast32[winCoefLen - 1] = oneQformat;
            }
            if (windowDatumType == MMWAVELIB_WINDOW_INT16)
            {
                winCoefLast16[winCoefLen - 1] = (int16_t) oneQformat;
            }
        }
    }
    else if (winType == MMWAVELIB_WIN_BLACKMAN)
    {
        /* w(n)=0.42-0.5*cos(2*pi*n/(N-1))+0.08*cos(4*pi*n/(N-1)) */
        a0 = 0.42;
        a1 = 0.5;
        a2 = 0.08;
        
        /* cos (4*pi*2/(N-1)), sin(4*pi*2/(N-1) */
        stepR2 = 1.f - (initI * initI) * (initR * initR) * 8.f;     /* cos(4a)=1-8*cos(a)^2*sin(a)^2 */
        stepI2 = 4.f * initR * initI * (1 - 2.f * initI * initI);     /* sin(4a)=4*sin(a)*cos(a) *(1-2*sin(a)^2) */
        
        /* init values */
        evenSin2 = 0.f;         /* sin(4*pi*0/N-1) */
        evenCos2 = 1.f;   /* cos(4*pi*0/N-1)*/

        oddCos2 = stepR;     /* cos (4*pi*1/(N-1)) */
        oddSin2 = stepI;     /* sin (4*pi*1/(N-1)) */ 
        
        for (ii = 0; ii < winCoefLen / 2; ii++)
        {
            /* w(n)=0.42-0.5*cos(2*pi*n/(N-1))+0.08*cos(4*pi*n/(N-1)) */
            valueEven = a0 - a1 * evenCos + a2 * evenCos2;
            winValueEven = _spint(valueEven * oneQformat + 0.5);
            
            valueOdd = a0 - a1 * oddCos + a2 * oddCos2;
            winValueOdd = _spint(valueOdd * oneQformat + 0.5);
            
            if (winValueEven >= oneQformat )
            {
                winValueEven = oneQformat - 1;
            }
            if (winValueOdd >= oneQformat )
            {
                winValueOdd = oneQformat - 1;
            }
            
            if (windowDatumType == MMWAVELIB_WINDOW_INT32)
            {
                _amem8(&winOut64[ii]) = _itoll(winValueOdd, winValueEven);
            }
            if (windowDatumType == MMWAVELIB_WINDOW_INT16)
            {
                 _amem4(&winOut32[ii]) = _pack2(winValueOdd, winValueEven);
            }
            /* update even */
            evenTemp = evenCos;
            /* cos(a+b) = cos(a)cos(b)-sin(a)sin(b) */
            evenCos =  evenCos * stepR - evenSin * stepI; 
            /* sin(a+b) = sin(a)cos(b)+cos(a)sin(b)*/
            evenSin = evenTemp * stepI + evenSin * stepR;
            
            evenTemp = evenCos2;
            evenCos2 = evenCos2 * stepR2 - evenSin2 * stepI2;
            evenSin2 = evenTemp * stepI2 + evenSin2 * stepR2;
            
            oddTemp = oddCos;
            oddCos = oddCos * stepR - oddSin * stepI;
            oddSin = oddTemp * stepI + oddSin * stepR;
            
            oddTemp = oddCos2;
            oddCos2 = oddCos2 * stepR2 - oddSin2 * stepI2;
            oddSin2 = oddTemp * stepI2 + oddSin2 * stepR2;
                    
        }
        
        if (winCoefLen & 0x1)
        {
            valueEven = a0 - a1 * evenCos + 0.08 * evenCos2;
            winValueEven = _spint(valueEven * oneQformat + 0.5);
            if (winValueEven >= oneQformat )
            {
                winValueEven = oneQformat - 1;
            }
            if (windowDatumType == MMWAVELIB_WINDOW_INT32)
            {
                winCoefLast32[winCoefLen - 1] = winValueEven ;
            }
            if (windowDatumType == MMWAVELIB_WINDOW_INT16)
            {
                winCoefLast16[winCoefLen - 1] = (int16_t) winValueEven;
            }
        }
  
    }
    
   
}
