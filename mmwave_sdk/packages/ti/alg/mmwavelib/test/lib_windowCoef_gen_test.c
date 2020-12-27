/*
 *   @file  lib_windowCoef_gen_test.c
 *
 *   @brief
 *      Unit Test code for windowing coefficient generation mmwavelib library functions
 *
 */
/*
 * (C) Copyright 2018 Texas Instruments, Inc.
 -------------------------------------------------------------------------
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


#include <c6x.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ti/alg/mmwavelib/mmwavelib.h>
#include <ti/utils/testlogger/logger.h>


/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

#pragma diag_push
#pragma diag_suppress 48
#include <ti/mathlib/mathlib.h>
#pragma diag_pop





#define MAXIMUMWINSIZE  8192
#define NUMFFTSIZES 10  /* test 2^4 to 2^13 */

int16_t testPASS = 1;  
uint32_t t1,t2;
uint32_t  cycles;


#pragma DATA_SECTION(winCoeffLib,".l2data");
#pragma DATA_ALIGN(winCoeffLib,8);
int32_t winCoeffLib[MAXIMUMWINSIZE];   


#pragma DATA_SECTION(winCoeffRef,".l2data");
int32_t winCoeffRef[8192]; 

#define ABSERROR32 0.00000763

#define ABSERROR16 0

#define PI_ 3.1415926535897

void genWindow_ref(void *win,
                   uint32_t windowDatumType,
                   uint32_t winLen,
                   uint32_t winGenLen,
                   int32_t oneQformat,
                   uint32_t winType)
{
    uint32_t winIndx;
    float eR,eR2;
    float phi ;
    int32_t winVal;
    int16_t * win16 = (int16_t *) win;
    int32_t * win32 = (int32_t *) win;
    
    
    if (winType == MMWAVELIB_WIN_HANNING)
    {
        if (oneQformat == (1<<31))
            oneQformat = oneQformat -1;
        //Hanning window
        for(winIndx = 0; winIndx < winGenLen; winIndx++)
        {
            phi = 2.f * PI_ * winIndx / ((float) winLen - 1.f);
            eR = cossp(phi);
            winVal = (int32_t) ((oneQformat * 0.5* (1 - eR)) + 0.5);  /* 0.5 * (1-cos(2*pi*n/N-1)) */
            if(winVal >= oneQformat)
            {
                winVal = oneQformat - 1;
            }
            if (windowDatumType == MMWAVELIB_WINDOW_INT32)
            {
                win32[winIndx] = (int32_t) winVal;
            }
            if (windowDatumType == MMWAVELIB_WINDOW_INT16)
            {
                win16[winIndx] = (int16_t) winVal;
            }
        }
    }
    else if (winType == MMWAVELIB_WIN_HAMMING)
    {
         if (oneQformat == (1<<31))
            oneQformat = oneQformat -1;
        for(winIndx = 0; winIndx < winGenLen; winIndx++)
        {
            phi = 2.f * PI_ * winIndx / ((float) winLen - 1.f);
            eR = cossp(phi);
            winVal = (int32_t) ((oneQformat *  (0.54 -0.46* eR)) + 0.5); 
            if(winVal >= oneQformat)
            {
                winVal = oneQformat - 1;
            }
            if (windowDatumType == MMWAVELIB_WINDOW_INT32)
            {
                win32[winIndx] = (int32_t) winVal;
            }
            if (windowDatumType == MMWAVELIB_WINDOW_INT16)
            {
                win16[winIndx] = (int16_t) winVal;
            }
        }
    }
    else if (winType == MMWAVELIB_WIN_RECT)
    {
        for(winIndx = 0; winIndx < winGenLen; winIndx++)
        {
             if (windowDatumType == MMWAVELIB_WINDOW_INT32)
            {
                win32[winIndx] = oneQformat -1;
             }
             if (windowDatumType == MMWAVELIB_WINDOW_INT16)
            {
                win16[winIndx] = (int16_t )(oneQformat -1);
             }
        }
    }
    else if (winType == MMWAVELIB_WIN_BLACKMAN)
    {
        if (oneQformat == (1<<31))
            oneQformat = oneQformat -1;
        for(winIndx = 0; winIndx < winGenLen; winIndx++)
        {
            phi = 2.f * PI_ * winIndx / ((float) winLen - 1.f);
            eR = cossp(phi);
            phi = 4.f * PI_ * winIndx / ((float) winLen - 1.f);
            eR2 = cossp(phi);
            winVal = (int32_t) ((oneQformat *  (0.42-0.5*eR+0.08*eR2)) + 0.5); 
             if(winVal >= oneQformat)
            {
                winVal = oneQformat - 1;
            }
            if (windowDatumType == MMWAVELIB_WINDOW_INT32)
            {
                win32[winIndx] = (int32_t) winVal;
            }
            if (windowDatumType == MMWAVELIB_WINDOW_INT16)
            {
                win16[winIndx] = (int16_t) winVal;
            }
        }
    }

}
void test_winCoefGen_32bits()
{
    uint32_t ii,jj,kk;
    uint32_t winLength;
    float initCos, initSin;
    float phi; 
    int32_t diffError; 
    int32_t maxError;
    uint32_t qFormat;
    
    //qFormat = 19;
    for (ii = 0; ii < 7; ii++ )  /* for 32 bits, test up to winLength = 2^10 */
    {
        for (jj = 0; jj < 4; jj++)
        {
            winLength = (1<<(ii+4)) + jj;   /* 2^4 + (0,1,2,3) */

            printf("window (Hanning, Hamming, Rect, Blackman) with normal length is %d\n", winLength);
            printf("Qformat is: ");

            for (qFormat = 15; qFormat < 32; qFormat ++)
            {
                maxError = (int32_t) (ABSERROR32 *  (float) ((1<<qFormat) -1) + 0.5);
                if (maxError == 0)
                    maxError = 1;
                printf("%d  ",qFormat);
               
                //printf("window normal length is %d, generated window coefficient length is %d\n", winLength, (winLength + 1)/2);
                
                //printf("Hanning window,");
                 phi = 2 * PI_ / ((float) winLength - 1);
                /* call the lib */
                initCos = cossp(phi);
                initSin = sinsp(phi);
                mmwavelib_windowCoef_gen((void *)winCoeffLib,
                                         MMWAVELIB_WINDOW_INT32,
                                         initCos,
                                         initSin,
                                         (winLength + 1)/2,
                                         qFormat,   /* Q format */
                                         MMWAVELIB_WIN_HANNING);

                genWindow_ref( (void *) winCoeffRef,
                                MMWAVELIB_WINDOW_INT32,
                               winLength,
                               (winLength + 1)/2, 
                                1<<qFormat,   /* one in Q format */
                               MMWAVELIB_WIN_HANNING);


                /* check the results*/
                for (kk = 0; kk <(winLength + 1)/2; kk++ )
                {
                    diffError = winCoeffRef[kk] - winCoeffLib[kk];

                    if ((diffError > maxError) || (diffError < -maxError))
                    {
                        testPASS = 0;
                        printf("Hanning: winL=%d, coefL=%d, index=%d, ref= %d, lib=%d\n", winLength, (winLength+1)/2, kk,winCoeffRef[kk],winCoeffLib[kk] );
                    }
                }


                //printf("Hamming window,");

                mmwavelib_windowCoef_gen((void *)winCoeffLib,
                                         MMWAVELIB_WINDOW_INT32,
                                         initCos,
                                         initSin,
                                         (winLength + 1)/2,
                                         qFormat,   /* Q format */
                                         MMWAVELIB_WIN_HAMMING);

                /* call the reference */
                genWindow_ref( (void *) winCoeffRef,
                                   MMWAVELIB_WINDOW_INT32,
                                   winLength,
                                   (winLength + 1)/2, 
                                   (1 << qFormat),
                                    MMWAVELIB_WIN_HAMMING);

                /* check the results*/
                for (kk = 0; kk <(winLength + 1)/2; kk++ )
                {
                    diffError = winCoeffRef[kk] - winCoeffLib[kk];

                    if ((diffError > maxError) || (diffError < -maxError))
                    {
                        testPASS = 0;
                        printf("Hamming: winL=%d, coefL=%d, index=%d, ref= %d, lib=%d\n", winLength, (winLength+1)/2, kk,winCoeffRef[kk],winCoeffLib[kk] );
                    }
                }



                //printf("Rectangle window,");

                mmwavelib_windowCoef_gen((void *)winCoeffLib,
                                         MMWAVELIB_WINDOW_INT32,
                                         initCos,
                                         initSin,
                                         (winLength + 1)/2,
                                         qFormat,   /* Q format */
                                         MMWAVELIB_WIN_RECT);

 
               genWindow_ref( (void *) winCoeffRef,
                                   MMWAVELIB_WINDOW_INT32,
                                   winLength,
                                   (winLength + 1)/2, 
                                   (1 << qFormat),
                                    MMWAVELIB_WIN_RECT);
 
               /* check the results*/
                for (kk = 0; kk <(winLength + 1)/2; kk++ )
                {
                    diffError = winCoeffRef[kk] - winCoeffLib[kk];

                    if ((diffError > maxError) || (diffError < -maxError))
                    {
                        testPASS = 0;
                        printf("Rectangle: winL=%d, coefL=%d, index=%d, ref= %d, lib=%d\n", winLength, (winLength+1)/2, kk,winCoeffRef[kk],winCoeffLib[kk] );
                    }
                }



                //printf("Blackman window \n");

                mmwavelib_windowCoef_gen((void *)winCoeffLib,
                                         MMWAVELIB_WINDOW_INT32,
                                         initCos,
                                         initSin,
                                         (winLength + 1)/2,
                                         qFormat,   /* Q format */
                                         MMWAVELIB_WIN_BLACKMAN);

                genWindow_ref( (void *) winCoeffRef,
                                   MMWAVELIB_WINDOW_INT32,
                                   winLength,
                                   (winLength + 1)/2, 
                                   (1 << qFormat),
                                    MMWAVELIB_WIN_BLACKMAN);
    
                /* check the results*/
                for (kk = 0; kk <(winLength + 1)/2; kk++ )
                {
                    diffError = winCoeffRef[kk] - winCoeffLib[kk];

                    if ((diffError > maxError) || (diffError < -maxError))
                    {
                        testPASS = 0;
                        printf("Blackman: winL=%d, coefL=%d, index=%d, ref= %d, lib=%d\n", winLength, (winLength+1)/2, kk,winCoeffRef[kk],winCoeffLib[kk] );
                    }
                }
            }
            printf("\n");
        }
        
    }
    
    if (testPASS)
	{
		MCPI_setFeatureTestResult("The mmwavelib_windowCoef_gen 32bits functionality test", MCPI_TestResult_PASS);
	}
	else
	{
		MCPI_setFeatureTestResult("The mmwavelib_windowCoef_gen 32 bits functionality test", MCPI_TestResult_FAIL);
	}

    
}

void benchmark_winCoefGen_32bits()
{
    uint32_t ii;
    uint32_t winLength;
    float initCos, initSin;
    float phi;
    
    printf("Hanning window 32 bits benchmark\n");
    printf("Generated winCoef length \t cycles\n");
    for (ii = 0; ii < 7; ii++ )  /* for 32 bits, test up to winLength = 2^10 */
    {
        winLength = (1<<(ii+4));
        
        phi = 2 * PI_ / ((float) winLength - 1);
        initCos = cossp(phi);
        initSin = sinsp(phi);
        t1 = TSCL;
		mmwavelib_windowCoef_gen((void *)winCoeffLib,
                                 MMWAVELIB_WINDOW_INT32,
                                 initCos,
                                 initSin,
                                 (winLength + 1)/2,
                                  19,   /* Q format set to 19 */
                                  MMWAVELIB_WIN_HANNING);

        t2 = TSCL;
     	cycles = t2 - t1;
		printf("%d\t%d\n",(winLength + 1)/2, cycles);
    }
    
    printf("Rectangle window 32 bits benchmark\n");
    printf("Generated winCoef \t cycles\n");
    for (ii = 0; ii < 7; ii++ )  /* for 32 bits, test up to winLength = 2^10 */
    {
        winLength = (1<<(ii+4));
        t1 = TSCL;
        mmwavelib_windowCoef_gen((void *)winCoeffLib,
                                 MMWAVELIB_WINDOW_INT32,
                                 initCos,
                                 initSin,
                                 (winLength + 1)/2,
                                  19,   /* Q19 format */
                                  MMWAVELIB_WIN_RECT);

        t2 = TSCL;
     	cycles = t2 - t1;
		printf("%d\t%d\n",(winLength + 1)/2, cycles);
    }
    
    printf("Blackman window 32 bits benchmark\n");
    printf("Generated winCoef \t cycles\n");
    for (ii = 0; ii < 7; ii++ )  /* for 32 bits, test up to winLength = 2^10 */
    {
        winLength = (1<<(ii+4));
        
        phi = 2 * PI_ / ((float) winLength - 1);
        initCos = cossp(phi);
        initSin = sinsp(phi);

    	t1 = TSCL;
		mmwavelib_windowCoef_gen((void *)winCoeffLib,
                                 MMWAVELIB_WINDOW_INT32,
                                 initCos,
                                 initSin,
                                 (winLength + 1)/2,
                                  19,   /*Q19 format */
                                  MMWAVELIB_WIN_BLACKMAN);

        t2 = TSCL;
     	cycles = t2 - t1;
		printf("%d\t%d\n",(winLength + 1)/2, cycles);
    }
    
}

void test_winCoefGen_16bits()
{
    uint32_t ii,jj,kk;
    uint32_t winLength;
    float initCos, initSin;
    float phi; 
    
    int32_t diffError; 
    uint32_t qFormat;
    
    int16_t * winCoeffLib16;
    int16_t * winCoeffRef16;
    int16_t maxError;
    
    winCoeffLib16 = (int16_t *)winCoeffLib;
    winCoeffRef16 = (int16_t *)winCoeffRef;
    
    //qFormat = 15;
    for (ii = 0; ii < NUMFFTSIZES; ii++ )  /* for 16 bits, test up to winLength 8192 */
    {
        for (jj = 0; jj < 4; jj++)
        {
            winLength = (1<<(ii+4)) + jj;   /* 2^(ii+4) + (0,1,2,3) */
            
            printf("window (Hanning, Hamming, Rect, Blackman) with normal length is %d\n", winLength);
            printf("Qformat is ");
            
            for (qFormat = 10; qFormat < 16; qFormat ++)
            {
                maxError = (int32_t) (ABSERROR16 *  (float) ((1<<qFormat) -1) + 0.5);

                if (maxError == 0)
                    maxError = 1;
                
                //printf("Qformat is %d, max error is %d\n", qFormat,maxError);
                printf("%d   ", qFormat);
                phi = 2 * PI_ / ((float) winLength - 1);
                /* call the lib */
                initCos = cossp(phi);
                initSin = sinsp(phi);
                //printf("Hanning window,");

                mmwavelib_windowCoef_gen((void *)winCoeffLib16,
                                         MMWAVELIB_WINDOW_INT16,
                                         initCos,
                                         initSin,
                                         (winLength + 1)/2,
                                          qFormat,   /* Q format */
                                         MMWAVELIB_WIN_HANNING);

                /* call the reference */
               genWindow_ref( (void *) winCoeffRef16,
                                   MMWAVELIB_WINDOW_INT16,
                                   winLength,
                                   (winLength + 1)/2, 
                                   (1 << qFormat),
                                    MMWAVELIB_WIN_HANNING);
  
                /* check the results*/
                for (kk = 0; kk <(winLength + 1)/2; kk++ )
                {
                    diffError = winCoeffRef16[kk] - winCoeffLib16[kk];

                    if ((diffError > maxError) || (diffError < -maxError))
                    {
                        testPASS = 0;
                        printf("Hanning: winL=%d, coefL=%d, index=%d, ref= %d, lib=%d\n", winLength, (winLength+1)/2, kk,winCoeffRef16[kk],winCoeffLib16[kk] );
                    }
                }

                //printf("Hamming window,");
                mmwavelib_windowCoef_gen( (void *)winCoeffLib16,
                                         MMWAVELIB_WINDOW_INT16,
                                         initCos,
                                         initSin,
                                         (winLength + 1)/2,
                                         qFormat,   
                                         MMWAVELIB_WIN_HAMMING);

                /* call the reference */
                genWindow_ref( (void *) winCoeffRef16,
                                   MMWAVELIB_WINDOW_INT16,
                                   winLength,
                                   (winLength + 1)/2, 
                                   (1 << qFormat),
                                    MMWAVELIB_WIN_HAMMING);
    

                /* check the results*/
                for (kk = 0; kk <(winLength + 1)/2; kk++ )
                {
                    diffError = winCoeffRef16[kk] - winCoeffLib16[kk];

                    if ((diffError > maxError) || (diffError < -maxError))
                    {
                        testPASS = 0;
                        printf("Hamming: winL=%d, coefL=%d, index=%d, ref= %d, lib=%d\n", winLength, (winLength+1)/2, kk,winCoeffRef16[kk],winCoeffLib16[kk] );
                    }
                }

                //printf("Rectangle window,");
                mmwavelib_windowCoef_gen((void *)winCoeffLib16,
                                        MMWAVELIB_WINDOW_INT16,
                                         initCos,
                                         initSin,
                                         (winLength + 1)/2,
                                         qFormat,   
                                         MMWAVELIB_WIN_RECT);

                genWindow_ref( (void *) winCoeffRef16,
                                   MMWAVELIB_WINDOW_INT16,
                                   winLength,
                                   (winLength + 1)/2, 
                                   (1 << qFormat),
                                    MMWAVELIB_WIN_RECT);
    

                /* check the results*/
                for (kk = 0; kk <(winLength + 1)/2; kk++ )
                {
                    diffError = winCoeffRef16[kk] - winCoeffLib16[kk];

                    if ((diffError > maxError) || (diffError < -maxError))
                    {
                        testPASS = 0;
                        printf("Rectangle: winL=%d, coefL=%d, index=%d, ref= %d, lib=%d\n", winLength, (winLength+1)/2, kk,winCoeffRef16[kk],winCoeffLib16[kk] );
                    }
                }

                //printf("Blackman window \n");
                mmwavelib_windowCoef_gen((void *)winCoeffLib16,
                                         MMWAVELIB_WINDOW_INT16,
                                         initCos,
                                         initSin,
                                         (winLength + 1)/2,
                                          qFormat,   
                                         MMWAVELIB_WIN_BLACKMAN);

                genWindow_ref( (void *) winCoeffRef16,
                                   MMWAVELIB_WINDOW_INT16,
                                   winLength,
                                   (winLength + 1)/2, 
                                   (1 << qFormat),
                                    MMWAVELIB_WIN_BLACKMAN);
                /* check the results*/
                for (kk = 0; kk <(winLength + 1)/2; kk++ )
                {
                    diffError = winCoeffRef16[kk] - winCoeffLib16[kk];

                    if ((diffError > maxError) || (diffError < -maxError))
                    {
                        testPASS = 0;
                        printf("Blackman: winL=%d, coefL=%d, index=%d, ref= %d, lib=%d\n", winLength, (winLength+1)/2, kk,winCoeffRef16[kk],winCoeffLib16[kk] );
                    }
                }
            }
            printf("\n");
        }
    }
    
    if (testPASS)
	{
		MCPI_setFeatureTestResult("The mmwavelib_windowCoef_gen 16bits functionality test", MCPI_TestResult_PASS);
	}
	else
	{
		MCPI_setFeatureTestResult("The mmwavelib_windowCoef_gen 16 bits functionality test", MCPI_TestResult_FAIL);
	}

    
}

void benchmark_winCoefGen_16bits()
{
    uint32_t ii;
    uint32_t winLength;
    float initCos, initSin;
    float phi;
    
    int16_t * winCoeffLib16;
    
    winCoeffLib16 = (int16_t *)winCoeffLib;
    
    
    
    printf("Hanning window 16 bits benchmark\n");
    printf("Generated winCoef Length \t cycles\n");
    for (ii = 0; ii < NUMFFTSIZES; ii++ )  /* for 32 bits, test up to winLength = 2^10 */
    {
        winLength = (1<<(ii+4));
    	
        phi = 2 * PI_ / ((float) winLength - 1);
        initCos = cossp(phi);
        initSin = sinsp(phi);
        t1 = TSCL;
        
		mmwavelib_windowCoef_gen((void *)winCoeffLib16,
                                 MMWAVELIB_WINDOW_INT16,
                                 initCos,
                                 initSin,
                                 (winLength + 1)/2,
                                  15,   
                                  MMWAVELIB_WIN_HANNING);

                
        t2 = TSCL;
     	cycles = t2 - t1;
		printf("%d\t%d\n",(winLength + 1)/2, cycles);
    }
    
    printf("Rectangle window 16 bits benchmark\n");
    printf("Generated winCoef length \t cycles\n");
    for (ii = 0; ii < NUMFFTSIZES; ii++ )  /* for 32 bits, test up to winLength = 2^10 */
    {
        winLength = (1<<(ii+4));
        t1 = TSCL;
       mmwavelib_windowCoef_gen((void *)winCoeffLib16,
                                 MMWAVELIB_WINDOW_INT16,
                                 initCos,
                                 initSin,
                                 (winLength + 1)/2,
                                  15,   /* one in Q19 format */
                                  MMWAVELIB_WIN_RECT);

        t2 = TSCL;
     	cycles = t2 - t1;
		printf("%d\t%d\n",(winLength + 1)/2, cycles);
    }
    
    printf("Blackman window 16 bits benchmark\n");
    printf("Generated winCoef length \t cycles\n");
    for (ii = 0; ii < NUMFFTSIZES; ii++ )  
    {
        winLength = (1<<(ii+4));
        
        phi = 2 * PI_ / ((float) winLength - 1);
        initCos = cossp(phi);
        initSin = sinsp(phi);

    	t1 = TSCL;
		mmwavelib_windowCoef_gen((void *)winCoeffLib16,
                                 MMWAVELIB_WINDOW_INT16,
                                 initCos,
                                 initSin,
                                 (winLength + 1)/2,
                                  15,   /* one in Q19 format */
                                  MMWAVELIB_WIN_BLACKMAN);
    
        

        t2 = TSCL;
     	cycles = t2 - t1;
		printf("%d\t%d\n",(winLength + 1)/2, cycles);
    }
    
}

void main(void)
{
    MCPI_Initialize();
    TSCL = 0;
      
    printf("...... Window Coefficient Generations test ......\n\n");
      
    printf("32 bits window coefficients generation test ........\n");
    test_winCoefGen_32bits();
      
    printf("\n32 bits window coefficients generation benchmark ........\n");
    benchmark_winCoefGen_32bits();
    
    printf("\n16 bits window coefficients generation test ........\n");
    test_winCoefGen_16bits();
    
    printf("\n16 bits window coefficients generation benchmark ......\n");
    benchmark_winCoefGen_16bits();
    
      
    if (testPASS)
    {
        printf("========== Window Coefficient Generations tests all passed! ========== \n\n");
    }
    else
    {
        printf("==========  Window Coefficient Generations tests  Failed! ========== \n\n");
    }
    printf("========== Window Coefficient Generations Testing completed! ==========\n");

    MCPI_setTestResult();
    
}

