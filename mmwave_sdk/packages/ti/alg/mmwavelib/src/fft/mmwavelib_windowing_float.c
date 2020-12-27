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
 *   @file     mmwavelib_windowing_float.c
 *
 *   @brief    Contains functions related to windowing of data (typically
 *             prior to an FFT)
 *
 */

#include <stdint.h>
#include <c6x.h>
#include <ti/alg/mmwavelib/include/mmwavelib_c674_emulate_c66_intrinsics.h>
#include "mmwavelib_fft_float.h"

/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_windowing1DFltp
 *
 * \par
 * <b>Description</b>  :    Windowing function for 1D FFT per antenna per chirp, prepare for the input to 
 *                          floating point 1D FFT.
 *
 * @param[in]               nSamplesValidPerChirp     : Number of valid samples per chirp
 * @param[in]               fftSize1D    : 1D FFT size, must be power of 2
 * @param[in]               fftWin1D     : Input pointer to Windowing real array (16 bit) of length 
 *                                         nSamplesValidPerChirp/2+1. It is assumed the window is symmetric.
 * @param[in]               inputPtr     : Input 32 bits complex signal array, 16bits I/Q.
 * @param[out]              outputPtr    : Output complex signal array, floating point I/Q, same order as the 
 *                                         input.
 *
 * @return                  void
 * @pre                     (1)Input Windowing array (fftWin1D) is 4 byte aligned.
 *                          (2)Input data (inputPtr) is 8 byte aligned.
 *
 * @ingroup                 MMWAVELIB_FFT_FLOAT
 *
 * Cycles                  2.87*length+56(cgt 8.1.3)
 *******************************************************************************************************************
 */
void    mmwavelib_windowing1DFltp(
                              uint32_t nSamplesValidPerChirp,
                              uint32_t fftSize1D,
                              int16_t  fftWin1D[restrict],
                              int16_t * inputPtr,
                              float32_t * outputPtr)

{

    uint32_t    j, leftover;
    uint32_t     winCoef0;
    uint32_t    itemp0;
    int64_t     lltemp1, lltemp3;
    int32_t     * restrict winFunc;
    int32_t     winFac;
    int32_t     * restrict tempInput1Ant;
    int64_t     * restrict input1Ant1, * restrict input2Ant1;
    __float2_t  * restrict output1Ant1, * restrict output2Ant1, f2temp1;
    

    winFunc = (int32_t *)fftWin1D;
    input1Ant1 = (int64_t  *) inputPtr;
    input2Ant1 = (int64_t  *) &inputPtr[2U * nSamplesValidPerChirp - 4U];
    output1Ant1 = (__float2_t  *) outputPtr;
    output2Ant1 = (__float2_t  *) &outputPtr[2U * nSamplesValidPerChirp - 2U];

    /* windowing - assuming symmetric*/
    if( (nSamplesValidPerChirp & 0x3U) == 0)
    {
        for (j = 0; j < nSamplesValidPerChirp/4U; j++)
        {
            winFac      =   _amem4(winFunc);
            winFunc++;
            lltemp1     =   _itoll(_packh2((uint32_t)winFac, 0), _pack2((uint32_t)winFac, 0));
            lltemp3     =   _dcmpyr1(lltemp1, _amem8(input1Ant1));
            input1Ant1++;
            itemp0      =   _loll(lltemp3);
            _amem8_f2(output1Ant1) =    _dinthsp((int32_t)_packhl2(itemp0,itemp0));
            output1Ant1++;
            itemp0      =   _hill(lltemp3);
            _amem8_f2(output1Ant1) =    _dinthsp((int32_t)_packhl2(itemp0, itemp0));
            output1Ant1++;
            lltemp1     =   _itoll(_pack2((uint32_t)winFac, 0), _packh2((uint32_t)winFac, 0));
            lltemp3     =   _dcmpyr1(lltemp1, _amem8(input2Ant1));
            input2Ant1--;
            itemp0      =   _hill(lltemp3);
            _amem8_f2(output2Ant1)      =   _dinthsp((int32_t)_packhl2(itemp0, itemp0));
            output2Ant1--;
            itemp0      =   _loll(lltemp3);
            _amem8_f2(output2Ant1)      =   _dinthsp((int32_t)_packhl2(itemp0, itemp0));
            output2Ant1--;
        }
    }
    else if ( (nSamplesValidPerChirp & 0x3U) == 2U)
    {
        for (j = 0; j < nSamplesValidPerChirp/4U; j++)
        {
            winFac      =   _amem4(winFunc);
            winFunc++;
            lltemp1     =   _itoll(_packh2((uint32_t)winFac, 0), _pack2((uint32_t)winFac, 0));
            lltemp3     =   _dcmpyr1(lltemp1, _amem8(input1Ant1));
            input1Ant1++;
            itemp0      =   _loll(lltemp3);
            _amem8_f2(output1Ant1) =    _dinthsp((int32_t)_packhl2(itemp0, itemp0));
            output1Ant1++;
            itemp0      =   _hill(lltemp3);
            _amem8_f2(output1Ant1) =    _dinthsp((int32_t)_packhl2(itemp0, itemp0));
            output1Ant1++;
            lltemp1     =   _itoll(_pack2((uint32_t)winFac, 0), _packh2((uint32_t)winFac, 0));
            lltemp3     =   _dcmpyr1(lltemp1, _amem8(input2Ant1));
            input2Ant1--;
            itemp0      =   _hill(lltemp3);
            _amem8_f2(output2Ant1)      =   _dinthsp((int32_t)_packhl2(itemp0, itemp0));
            output2Ant1--;
            itemp0      =   _loll(lltemp3);
            _amem8_f2(output2Ant1)      =   _dinthsp((int32_t)_packhl2(itemp0, itemp0));
            output2Ant1--;
        }
        /* last pair */
        winFac      =   _amem4(winFunc);
        lltemp1     =   _itoll(_packh2((uint32_t)winFac, 0), _pack2((uint32_t)winFac, 0));
        lltemp3     =   _dcmpyr1(lltemp1, _amem8(input1Ant1));
        input1Ant1++;
        itemp0      =   _loll(lltemp3);
        _amem8_f2(output1Ant1) =    _dinthsp((int32_t)_packhl2(itemp0, itemp0));
        output1Ant1++;
        itemp0      =   _hill(lltemp3);
        _amem8_f2(output1Ant1) =    _dinthsp((int32_t)_packhl2(itemp0, itemp0));
        output1Ant1++;
    }
    else
    {
        for (j = 0; j < nSamplesValidPerChirp/4U; j++)
        {
            winFac      =   _amem4(winFunc);
            winFunc++;
            lltemp1     =   _itoll(_packh2((uint32_t)winFac, 0), _pack2((uint32_t)winFac, 0));
            lltemp3     =   _dcmpyr1(lltemp1, _amem8(input1Ant1));
            input1Ant1++;
            itemp0      =   _loll(lltemp3);
            _amem8_f2(output1Ant1) =    _dinthsp((int32_t)_packhl2(itemp0, itemp0));
            output1Ant1++;
            itemp0      =   _hill(lltemp3);
            _amem8_f2(output1Ant1) =    _dinthsp((int32_t)_packhl2(itemp0, itemp0));
            output1Ant1++;
            lltemp1     =   _itoll(_pack2((uint32_t)winFac, 0), _packh2((uint32_t)winFac, 0));
            lltemp3     =   _dcmpyr1(lltemp1, _mem8(input2Ant1));
            input2Ant1--;
            itemp0      =   _hill(lltemp3);
            _amem8_f2(output2Ant1)      =   _dinthsp((int32_t)_packhl2(itemp0, itemp0));
            output2Ant1--;
            itemp0      =   _loll(lltemp3);
            _amem8_f2(output2Ant1)      =   _dinthsp((int32_t)_packhl2(itemp0, itemp0));
            output2Ant1--;
        }
        if((nSamplesValidPerChirp & 0x3U) ==3U)  /* nSamplesValidPerChirp%4==3 */
        {
            winFac      =   _amem4(winFunc);
            lltemp1     =   _itoll(_packh2((uint32_t)winFac, 0), _pack2((uint32_t)winFac, 0));
            lltemp3     =   _dcmpyr1(lltemp1, _amem8(input1Ant1));
            input1Ant1++;
            itemp0      =   _loll(lltemp3);
            _amem8_f2(output1Ant1) =    _dinthsp((int32_t)_packhl2(itemp0, itemp0));
            output1Ant1++;
            itemp0      =   _hill(lltemp3);
            _amem8_f2(output1Ant1) =    _dinthsp((int32_t)_packhl2(itemp0, itemp0));
            output1Ant1++;
    
            /* symmetric for last one*/
            winFac      =   fftWin1D[2U*(nSamplesValidPerChirp/4U)];
        }
        else /* nSamplesValidPerChirp%4==1 */
        {
            winFac      =   fftWin1D[nSamplesValidPerChirp/2U];
        }
        /* last one */
        tempInput1Ant   =   (int32_t *) input1Ant1;
        winCoef0    =  _pack2((uint32_t)winFac, 0);
        itemp0      =   _cmpyr1(_amem4(tempInput1Ant), winCoef0);
        _amem8_f2(output1Ant1) = 
                _ftof2((float32_t)(_ext((int32_t)itemp0, 0, 16U)), (float32_t)(_ext((int32_t)itemp0, 16U, 16U)));
        output1Ant1++;
    }
    leftover    =   (4U - (nSamplesValidPerChirp & 0x3U)) & 0x3U;
    for (j = 0; j < leftover * 2U; j++)
    {
        outputPtr[2U * nSamplesValidPerChirp + j] = 0.f;
    }

    /*zero padding to FFT1DSize*/
    output1Ant1 = (__float2_t  *) &outputPtr[2U * nSamplesValidPerChirp + 2U * leftover];
    f2temp1 = _ftof2(0.f, 0.f);
    for (j = 0; j < (fftSize1D - nSamplesValidPerChirp)/2U; j++)
    {
        _amem8_f2(output1Ant1) = f2temp1;
        output1Ant1++;
        _amem8_f2(output1Ant1) = f2temp1;
        output1Ant1++;
    }
}



/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_windowing2DFltp
 *
 * \par
 * <b>Description</b>  :    Windowing function for 2D FFT per antenna per chirp, prepare for the input to 
 *                          floating point 2D FFT.
 *
 * @param[in]               fftSize1D     : 1D FFT size
 * @param[in]               inWin         : Input 2D windowing coefficient for this chirp, in floating point.
 * @param[in, out]          inOutPtr      : Floating-point complex-value input array, Output is written inplace
 *
 * @return                  void
 *
 * @ingroup                 MMWAVELIB_FFT_FLOAT
 *
 * Cycles                   1.98*fftsize (cgt 8.1.3)
 *******************************************************************************************************************
 */

void mmwavelib_windowing2DFltp(
                            uint32_t fftSize1D,
                            float32_t inWin,
                            float32_t inOutPtr[restrict])

{

    uint32_t    j;
    __float2_t  lltemp3;
    __float2_t  * restrict input1Ant1, * restrict output1Ant1;

    input1Ant1  = (__float2_t  *) inOutPtr;
    output1Ant1 = (__float2_t  *) inOutPtr;

    /* windowing - assuming symmetric*/
    lltemp3     =   _ftof2(inWin,inWin);
    for (j = 0; j < fftSize1D/2U; j++)
    {
        _amem8_f2(output1Ant1) =    _dmpysp(_amem8_f2(input1Ant1), lltemp3);
        output1Ant1++;
        input1Ant1++;
        _amem8_f2(output1Ant1) =    _dmpysp(_amem8_f2(input1Ant1), lltemp3);
        output1Ant1++;
        input1Ant1++;
    }
}


/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_chirpProcWin2DFxdpinFltOut
 *
 * \par
 * <b>Description</b>  :    Windowing function for 2D FFT per antenna per chirp, the input is fixed point 
 *                          complex, prepare for the input to floating point 2D FFT.
 *
 * @param[in]               fftSize1D     : 1D FFT size
 * @param[in]               inWin         : Input 2D windowing coefficient for this chirp, fixed point.
 * @param[in, out]          inoutPtr      : Input is 32 bits complex value, with int16 I/Q, Output is written in 
 *                                          place, and it is floating point, in the same I/Q order as the 
 *                                          input buffer
 *
 * @return                  void
 *
 * @pre                     (1) inoutPtr must be aligned to 8 byte boundary.
 * @pre                     (2) Input is at the first half of the buffer pointed by inoutPtr, in 32 bits 
 *                              complex value,
 *                             
 *
 * @ingroup                 MMWAVELIB_FFT_FLOAT
 *
 * Cycles                   2.18*fftSize (cgt 8.1.3)
 *******************************************************************************************************************
 */



void  mmwavelib_chirpProcWin2DFxdpinFltOut(
                              uint32_t  fftSize1D,
                              int16_t   inWin,
                              void      * inoutPtr)

{

    uint32_t    j;
    int64_t     lltemp1;
    int64_t     lltemp3;
    int64_t     * restrict input1Ant1;
    __float2_t   * restrict output1Ant1;
    int32_t     win2, * fxpTempPtr;
    int32_t     itemp;
    float32_t   * fltpTempPtr;

    fxpTempPtr  =   (int32_t  *) inoutPtr;
    input1Ant1  =   (int64_t  *) &fxpTempPtr[fftSize1D - 2U];
    fltpTempPtr =   (float32_t  *) inoutPtr;
    output1Ant1 =   (__float2_t  *) &fltpTempPtr[2U * fftSize1D - 2U];
    win2        =   _pack2(inWin, 0);

    /* windowing - assuming symmetric*/
    for (j = 0; j < fftSize1D/2U; j++)
    {
        lltemp1                 =   _amem8(input1Ant1);
        input1Ant1--;
        lltemp3                 =   _dcmpyr1(lltemp1, _itoll(win2, win2));
        itemp                   =   _hill(lltemp3);
        _amem8_f2(output1Ant1)  =    _dinthsp((int32_t)_packhl2(itemp, itemp));
        output1Ant1--;
        itemp                   =   _loll(lltemp3);
        _amem8_f2(output1Ant1)  =    _dinthsp((int32_t)_packhl2(itemp, itemp));
        output1Ant1--;
    }
}

/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_chirpProcWin2DFxdpinFltOut_IQswap
 *
 * \par
 * <b>Description</b>  :    Windowing function for 2D FFT per antenna per chirp, the input is fixed point complex,
 *                          output is floating point complex value with I/Q order swapped compared with I/Q order
 *                          for input.
 *
 * @param[in]               fftSize1D     : 1D FFT size
 * @param[in]               inWin         : Input 2D windowing coefficient for this chirp, fixed point.
 * @param[in, out]          inoutPtr      : Input is 32 bits complex value, with int16 I/Q, Output is written in 
 *                                          place, and it is floating point, in the swapped I/Q order.
 *
 * @return                  void
 *
 * @pre                     (1) inoutPtr must be aligned to 8 byte boundary.
 * @pre                     (2) Input is at the first half of the buffer pointed by inoutPtr,
 *                              with 32 bits complex value, real and imag are 16 bits.
 *                             
 *
 * @ingroup                 MMWAVELIB_FFT_FLOAT
 *
 * Cycles                   2.5 * fftSize (cgt 8.1.3)
 *******************************************************************************************************************
 */

void  mmwavelib_chirpProcWin2DFxdpinFltOut_IQswap(
                              uint32_t  fftSize1D,
                              int16_t   inWin,
                              void      * inoutPtr)

{

    uint32_t    j;
    int64_t     lltemp1;
    int64_t     lltemp3;
    int64_t     * restrict input1Ant1;
    __float2_t   * restrict output1Ant1;
    int32_t     win2, * fxpTempPtr;
    int32_t     itemp;
    float32_t   * fltpTempPtr;
    uint32_t    symbol0, symbol1;

    fxpTempPtr  =   (int32_t  *) inoutPtr;
    input1Ant1  =   (int64_t  *) &fxpTempPtr[fftSize1D - 2U];
    fltpTempPtr =   (float32_t  *) inoutPtr;
    output1Ant1 =   (__float2_t  *) &fltpTempPtr[2U * fftSize1D - 2U];
    win2        =   _pack2(inWin, 0);

    /* windowing - assuming symmetric*/
    for (j = 0; j < fftSize1D/2U; j++)
    {
        lltemp1                 =   _amem8(input1Ant1);
        input1Ant1--;
        symbol1                 =   _swap2(_hill(lltemp1));
        symbol0                 =   _swap2(_loll(lltemp1));
        lltemp3                 =   _dcmpyr1(_itoll(symbol1,symbol0), _itoll(win2, win2));
        itemp                   =   _hill(lltemp3);
        _amem8_f2(output1Ant1)  =   _dinthsp((int32_t)_packhl2(itemp, itemp));
        output1Ant1--;
        itemp                   =   _loll(lltemp3);
        _amem8_f2(output1Ant1)  =   _dinthsp((int32_t)_packhl2(itemp, itemp));
        output1Ant1--;
    }
}


/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_dopplerProcWin2DFxdpinFltOut
 *
 * \par
 * <b>Description</b>  :    Windowing function for fixed point complex input and floating point complex output.
 *
 * @param[in]               length        : the number of complex values in the input array.
 * @param[in]               inputPtr      : Pointer to 16-bit I/Q input array.
 * @param[in]               windowPtr     : Pointer to 16-bit real windowing coefficients buffer.
 * @param[out]              outputPtr     : Pointer to floating-point output buffer after windowing,
 *                                          with same I/Q order as the input I/Q buffer.
 *
 * @return                  void
 *
 * @pre                     (1) inputPtr and outputPtr must be aligned to 8 byte boundary.
 * @pre                     (2) windowPtr must be aligned to 4 byte boundary.
 * @pre                     (3) Window is assumed to be symmetric so only length/2 coefficients are needed.
 * @pre                     (4) length must be multiple of 4.
 *
 * @ingroup                 MMWAVELIB_FFT_FLOAT
 *
 * Cycles                   1.55*length + 45 (cgt 8.1.3)
 *******************************************************************************************************************
 */


void mmwavelib_dopplerProcWin2DFxdpinFltOut(
                              uint32_t length,
                              int16_t * restrict inputPtr,
                              int16_t * restrict windowPtr,
                              float32_t * restrict outputPtr)
{

    uint32_t  j;
    int64_t     lltemp1;
    int64_t     lltemp3;
    int64_t     * restrict input1, * restrict input2;
    __float2_t   * restrict output1, * restrict output2;
    int32_t     win2;
    uint32_t     itemp;

    input1  =   (int64_t  *) inputPtr;
    output1 =   (__float2_t  *) outputPtr;

    input2  =   (int64_t  *) &inputPtr[2U*length - 4U];
    output2 =   (__float2_t  *) &outputPtr[2U * length - 2U];
    /* windowing - assuming symmetric*/
    for (j = 0; j < length/4U; j++)
    {
        lltemp1                 =   _amem8(input1);
        input1++;
        win2                    =   _amem4(&windowPtr[2U*j]);
        lltemp3                 =   _dcmpyr1(lltemp1, _dpack2((uint32_t)win2, 0));
        itemp                   =   _loll(lltemp3);
        _amem8_f2(output1)      =   _dinthsp((int32_t)_packhl2(itemp,itemp));
        output1++;
        itemp                   =   _hill(lltemp3);
        _amem8_f2(output1)      =   _dinthsp((int32_t)_packhl2(itemp, itemp));
        output1++;
        win2                    =   (int32_t)_rotl((uint32_t)win2, 16U);
        lltemp1                 =   _amem8(input2);
        input2--;
        lltemp3                 =   _dcmpyr1(lltemp1, _dpack2((uint32_t)win2, 0));
        itemp                   =   _hill(lltemp3);
        _amem8_f2(output2)      =   _dinthsp((int32_t)_packhl2(itemp, itemp));
        output2--;
        itemp                   =   _loll(lltemp3);
        _amem8_f2(output2)      =   _dinthsp((int32_t)_packhl2(itemp, itemp));
        output2--;
    }
}


/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_dopplerProcWin2DFxdpinFltOut_IQswap
 *
 * \par
 * <b>Description</b>  :    Windowing function for fixed point complex input (16 bits I and Q), 
 *                          and the output is floating point complex output with swapped I/Q order.
 *
 * @param[in]               length        : the number of complex values in the input array.
 * @param[in]               inputPtr      : Pointer to 16-bit I/Q input array.
 * @param[in]               windowPtr     : Pointer to 16-bit real windowing coefficients buffer.
 * @param[out]              outputPtr     : Pointer to floating-point output buffer after windowing,
 *                                          with swapped I/Q order as the input I/Q buffer.
 *
 * @return                  void
 *
 * @pre                     (1) inputPtr and outputPtr must be aligned to 8 byte boundary.
 * @pre                     (2) windowPtr must be aligned to 4 byte boundary.
 * @pre                     (3) Window is assumed to be symmetric so only length/2 coefficients are needed.
 * @pre                     (4) length must be multiple of 4.
 *
 * @ingroup                 MMWAVELIB_FFT_FLOAT
 *
 * Cycles                   1.8 * length + 67 (cgt 8.1.3)
 *******************************************************************************************************************
 */

void mmwavelib_dopplerProcWin2DFxdpinFltOut_IQswap(
                              uint32_t length,
                              int16_t * restrict inputPtr,
                              int16_t * restrict windowPtr,
                              float32_t * restrict outputPtr)
{

    uint32_t  j;
    int64_t     lltemp1;
    int64_t     lltemp3;
    int64_t     * restrict input1, * restrict input2;
    __float2_t  * restrict output1, * restrict output2;
    int32_t     win2;
    uint32_t    itemp;
    uint32_t    symbol0, symbol1, symbol2, symbol3;
    
    input1  =   (int64_t  *) inputPtr;
    output1 =   (__float2_t  *) outputPtr;

    input2  =   (int64_t  *) &inputPtr[2U*length - 4U];
    output2 =   (__float2_t  *) &outputPtr[2U * length - 2U];
    /* windowing - assuming symmetric*/
    for (j = 0; j < length/4U; j++)
    {
        lltemp1                 =   _amem8(input1);
        symbol1                 =   _swap2(_hill(lltemp1));
        symbol0                 =   _swap2(_loll(lltemp1));
        input1++;
        win2                    =   _amem4(&windowPtr[2U*j]);
        lltemp3                 =   _dcmpyr1(_itoll(symbol1, symbol0), _dpack2((uint32_t)win2, 0));
        itemp                   =   _loll(lltemp3);
        _amem8_f2(output1)      =   _dinthsp((int32_t)_packhl2(itemp,itemp));
        output1++;
        itemp                   =   _hill(lltemp3);
        _amem8_f2(output1)      =   _dinthsp((int32_t)_packhl2(itemp, itemp));
        output1++;
        win2                    =   (int32_t)_rotl((uint32_t)win2, 16U);
        lltemp1                 =   _amem8(input2);
        symbol3                 =   _swap2(_hill(lltemp1));
        symbol2                 =   _swap2(_loll(lltemp1));
        input2--;
        lltemp3                 =   _dcmpyr1(_itoll(symbol3, symbol2), _dpack2((uint32_t)win2, 0));
        itemp                   =   _hill(lltemp3);
        _amem8_f2(output2)      =   _dinthsp((int32_t)_packhl2(itemp, itemp));
        output2--;
        itemp                   =   _loll(lltemp3);
        _amem8_f2(output2)      =   _dinthsp((int32_t)_packhl2(itemp, itemp));
        output2--;
    }
}
