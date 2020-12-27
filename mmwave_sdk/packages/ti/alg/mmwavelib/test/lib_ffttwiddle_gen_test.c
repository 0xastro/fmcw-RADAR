/*
 *   @file  lib_gen_ffttwiddle_test.c
 *
 *   @brief
 *      Unit Test code for generating FFT twiddle factor mmwavelib library functions
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

extern int gen_twiddle_fft16x16(short *w, int n);
extern int gen_twiddle_fft16x16_imre_sa(short *w, int n);
extern int gen_twiddle_fft32x32(int *w, int n, double scale);

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

#define NUMFFTSIZES  10

int16_t testPASS=1;  
uint32_t t1,t2;
uint32_t  cycles;

uint16_t FFTSIZE[NUMFFTSIZES] = {16,32,64,128,256,512, 1024, 2048,4096,8192};

#pragma DATA_SECTION(twiddle16Ref,".l3data");
int16_t twiddle16Ref[2*8192];   /* save the reference */

#pragma DATA_SECTION(twiddle32Ref,".l3data");
int32_t twiddle32Ref[2*8192]; 

#pragma DATA_SECTION(twiddle16,".l2data");
int16_t twiddle16[2*8192];   /* lib output */

#pragma DATA_SECTION(twiddle32,".l2data");
int32_t twiddle32[2*8192]; 

#define MAXERROR 1

int32_t twiddleLength;
int32_t twiddleLengthRef;
void test_twiddlegen_16x16()
{
    uint32_t ii,jj;
    int16_t diff;
    int16_t constOne, constNegOne;
    
    constOne = 1 * MAXERROR;
    constNegOne = -1 * MAXERROR;
    for (ii = 0; ii < NUMFFTSIZES - 1; ii++)
    {
        printf("FFT size %d\n", FFTSIZE[ii]);
    	/* call the dsp lib for reference generation*/
        twiddleLengthRef = gen_twiddle_fft16x16(twiddle16Ref, FFTSIZE[ii]);
        
        /* call the lib*/
        twiddleLength = mmwavelib_gen_twiddle_fft16x16(twiddle16,FFTSIZE[ii]);
        
        //printf("twiddle factor length = %d, ref = %d\n", twiddleLength, twiddleLengthRef);

        if (twiddleLength!=twiddleLengthRef)
        {
             testPASS = 0;
             printf("test fail, FFT size %d, twiddle factor length ref=%d, lib=%d\n",FFTSIZE[ii],twiddleLength, twiddleLengthRef);
              
        }
        /* compare the results*/
        for (jj = 0; jj < twiddleLengthRef;jj+=2)
        {
            diff = twiddle16[jj] - twiddle16Ref[jj];
            if ((diff>constOne) || (diff<constNegOne))
            {
                testPASS = 0;
                printf("test fail, FFT size %d, point %d real: ref=%d, lib=%d\n",FFTSIZE[ii],jj>>1,twiddle16Ref[jj], twiddle16[jj]);
            }
            
            diff = twiddle16[jj+1] - twiddle16Ref[jj+1];
            if ((diff>constOne) || (diff<constNegOne))
            {
                testPASS = 0;
                printf("test fail, FFT size %d, point %d imag: ref=%d, lib=%d\n",FFTSIZE[ii],jj>>1,twiddle16Ref[jj+1], twiddle16[jj+1]);
                
            }
        }
    }
    
    if (testPASS)
    {
         MCPI_setFeatureTestResult("FFT twiddle factor(16x16) generation Test", MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult("FFT twiddle factor(16x16) generation Test", MCPI_TestResult_FAIL);
    }
}

void test_twiddlegen_16x16_imre_sa()
{
    uint32_t ii, jj;
    int16_t diff;
    int16_t constOne, constNegOne;

    constOne = 1 * MAXERROR;
    constNegOne = -1 * MAXERROR;
    for (ii = 0; ii < NUMFFTSIZES - 1; ii++)
    {
        printf("FFT size %d\n", FFTSIZE[ii]);
        /* call the dsp lib for reference generation*/
        twiddleLengthRef = gen_twiddle_fft16x16_imre_sa(twiddle16Ref, FFTSIZE[ii]);

        /* call the lib*/
        if (FFTSIZE[ii] <= 1024)
        {
            twiddleLength = mmwavelib_gen_twiddle_smallfft_16x16_imre_sa(twiddle16, FFTSIZE[ii]);
        }
        else
        {
            twiddleLength = mmwavelib_gen_twiddle_largefft_16x16_imre_sa(twiddle16, FFTSIZE[ii]);
        }
        

        //printf("twiddle factor length = %d, ref = %d\n", twiddleLength, twiddleLengthRef);

        if (twiddleLength != twiddleLengthRef)
        {
            testPASS = 0;
            printf("test fail, FFT size %d, twiddle factor length ref=%d, lib=%d\n", FFTSIZE[ii], twiddleLength, twiddleLengthRef);

        }
        /* compare the results*/
        for (jj = 0; jj < twiddleLengthRef; jj += 2)
        {
            diff = twiddle16[jj] - twiddle16Ref[jj];
            if ((diff>constOne) || (diff<constNegOne))
            {
                testPASS = 0;
                printf("test fail, FFT size %d, point %d real: ref=%d, lib=%d\n", FFTSIZE[ii], jj >> 1, twiddle16Ref[jj], twiddle16[jj]);
            }

            diff = twiddle16[jj + 1] - twiddle16Ref[jj + 1];
            if ((diff>constOne) || (diff<constNegOne))
            {
                testPASS = 0;
                printf("test fail, FFT size %d, point %d imag: ref=%d, lib=%d\n", FFTSIZE[ii], jj >> 1, twiddle16Ref[jj + 1], twiddle16[jj + 1]);

            }
        }
    }

    if (testPASS)
    {
        MCPI_setFeatureTestResult("FFT twiddle factor(16x16) imre generation Test", MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult("FFT twiddle factor(16x16) imre generation Test", MCPI_TestResult_FAIL);
    }
}

void benchmark_twiddlegen_16x16()
{
    
    int32_t ii;
    printf("fftsize, cycles\n");
    for (ii = 0; ii < NUMFFTSIZES - 1; ii++)
    {
        t1 = TSCL;
        mmwavelib_gen_twiddle_fft16x16(twiddle16,FFTSIZE[ii]);
        //gen_twiddle_fft16x16(twiddle16Ref, FFTSIZE[ii]);
        t2 = TSCL;
        cycles = t2-t1;
        printf("%d\t%d\n",FFTSIZE[ii],cycles);
    }
}

void benchmark_twiddlegen_16x16_imre_sa()
{
    int32_t ii;
    printf("fftsize, cycles\n");
    for (ii = 0; ii < NUMFFTSIZES - 1; ii++)    
    {
        if (FFTSIZE[ii] <= 1024)
        {
            t1 = TSCL;
            mmwavelib_gen_twiddle_smallfft_16x16_imre_sa(twiddle16, FFTSIZE[ii]);
            //gen_twiddle_fft16x16_imre(twiddle16Ref, FFTSIZE[ii]);
            t2 = TSCL;
        }
        else
        {
            t1 = TSCL;
            mmwavelib_gen_twiddle_largefft_16x16_imre_sa(twiddle16, FFTSIZE[ii]);
            //gen_twiddle_fft16x16_imre(twiddle16Ref, FFTSIZE[ii]);
            t2 = TSCL;
        }
        cycles = t2 - t1;
        printf("%d\t%d\n", FFTSIZE[ii], cycles);
    }
}
void test_twiddlegen_32x32()
{
    uint32_t ii,jj;
    int32_t diff;
    int32_t constOne, constNegOne;
    
    constOne = 1 * MAXERROR;
    constNegOne = -1 * MAXERROR;
    for (ii = 0; ii < NUMFFTSIZES; ii++)
    {
        
        printf("FFT size %d\n", FFTSIZE[ii]);
        
        if (FFTSIZE[ii]>2048)
        {
            constOne = 2 * constOne;
            constNegOne = 2 * constNegOne;
        }
    	/* call the dsp lib for reference generation*/
        twiddleLengthRef = gen_twiddle_fft32x32(twiddle32Ref, FFTSIZE[ii],2147483647.5);
        
        /* call the lib*/
        twiddleLength = mmwavelib_gen_twiddle_fft32x32(twiddle32,FFTSIZE[ii]);
        
        //printf("twiddle factor length = %d, ref = %d\n", twiddleLength, twiddleLengthRef);

        if (twiddleLength!=twiddleLengthRef)
        {
             testPASS = 0;
             printf("test fail, FFT size %d, twiddle factor length ref=%d, lib=%d\n",FFTSIZE[ii],twiddleLength, twiddleLengthRef);
              
        }
        /* compare the results*/
       for (jj = 0; jj < twiddleLengthRef;jj+=2)
        {
            diff = twiddle32[jj] - twiddle32Ref[jj];
            if ((diff>constOne) || (diff<constNegOne))
            {
                testPASS = 0;
                printf("test fail, FFT size %d, point %d real: ref=%d, lib=%d\n",FFTSIZE[ii],jj>>1,twiddle32Ref[jj], twiddle32[jj]);
            }
            
            diff = twiddle32[jj+1] - twiddle32Ref[jj+1];
            if ((diff>constOne) || (diff<constNegOne))
            {
                testPASS = 0;
                printf("test fail, FFT size %d, point %d imag: ref=%d, lib=%d\n",FFTSIZE[ii],jj>>1,twiddle32Ref[jj+1], twiddle32[jj+1]);
                
            }
        }
    }
    
    if (testPASS)
    {
         MCPI_setFeatureTestResult("FFT twiddle factor(32x32) generation Test", MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult("FFT twiddle factor(32x32) generation Test", MCPI_TestResult_FAIL);
    }
}

void benchmark_twiddlegen_32x32()
{
    
    int32_t ii;
    printf("fftsize, cycles\n");
    for (ii = 0; ii < NUMFFTSIZES; ii++)
    {
        t1 = TSCL;
        mmwavelib_gen_twiddle_fft32x32(twiddle32,FFTSIZE[ii]);
        //gen_twiddle_fft32x32(twiddle32Ref, FFTSIZE[ii],2147483647.5);
        t2 = TSCL;
        cycles = t2-t1;
        printf("%d\t%d\n",FFTSIZE[ii],cycles);
    }
}
void main(void)
{
    MCPI_Initialize();
    TSCL = 0;
      
    printf("...... Generate FFT Twiddle Factor test ......\n\n");
      
    printf("16x16 bits twiddle factor generation test ........\n");
    test_twiddlegen_16x16();
      
    printf("16x16 bits twiddle factor generation benchmark ........\n");
    benchmark_twiddlegen_16x16();
      

    printf("16x16 bits imre twiddle factor generation test ........\n");
    test_twiddlegen_16x16_imre_sa();

    printf("16x16 bits imre twiddle factor generation benchmark ........\n");
    benchmark_twiddlegen_16x16_imre_sa();


      
    printf("32x32 bits twiddle factor generation test ........\n");
    test_twiddlegen_32x32();
      
    printf("32x32 bits twiddle factor generation benchmark ........\n");
    benchmark_twiddlegen_32x32();
      
    if (testPASS)
    {
        printf("========== FFT twiddle factor generation tests all passed! ========== \n");
    }
    else
    {
        printf("==========  FFT twiddle factor generation tests  Failed! ========== \n");
    }
    printf("========== FFT twiddle factor generation Testing completed! ==========\n");

    MCPI_setTestResult ();
    
}

