/*
 *   @file  lib_windowflt_test.c
 *
 *   @brief
 *      Unit Test code for floating point windowing mmwavelib functions
 *
 */
/*
 * (C) Copyright 2017 Texas Instruments, Inc.
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

#include <ti/alg/mmwavelib/mmwavelib.h>
#include <ti/utils/testlogger/logger.h>

/* Standard Include Files. */
#include <c6x.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

FILE *fid;
int16_t testPASS=1;
uint32_t t1,t2;
uint32_t  cycles;

#define MAXNUMFFTSIZE 10  /* maximum number of tested fft size */
#define MAX_NUM_RANGE_BINS 4096   /* maximum fft size */
#define MAXIMUM_ERROR 1.f

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);


/* input data buffer */
#pragma DATA_SECTION(inputbuf,".l2data");
#pragma DATA_ALIGN(inputbuf, 8);
int16_t inputbuf[MAX_NUM_RANGE_BINS*2];   /* complex values */

/* 1D windowing buffer */
#pragma DATA_SECTION(winBuf,".l2data");
#pragma DATA_ALIGN(winBuf, 8);
/* length is numsamples/2+1 for 1D, length is numsamples/2 for 2D */
int16_t winBuf[(MAX_NUM_RANGE_BINS>>1)+1];

/* output data buffer */
#pragma DATA_SECTION(outputbuf,".l2data");
float outputbuf[MAX_NUM_RANGE_BINS*2];   /* complex value */

/* output referef buffer */
#pragma DATA_SECTION(refbuf,".l2data");
float refbuf[MAX_NUM_RANGE_BINS*2];   /* complex value */



/* generate reference for 1D fixed-point in, float-point out reference */
void ref_windowingFltp(uint32_t chirpSize,
					   uint32_t fftSize1D,
                       uint8_t IQswapFlag,
					   int16_t  *winBuf,
					   int16_t * input,
					   float * output)
{
	uint32_t ii,halfWinSize;
	int16_t realIn, imagIn;
	int16_t win;

	halfWinSize = chirpSize/2;
	if (chirpSize&0x1)
	{
		halfWinSize++;
	}
	for (ii = 0; ii < chirpSize; ii++)
	{
        if (IQswapFlag == 0 )
        {
            realIn = input[2*ii];
            imagIn = input[2*ii+1];
        }
        else
        {
            imagIn = input[2*ii];
            realIn = input[2*ii+1];
        }
		if (ii<halfWinSize)
		{
			win = winBuf[ii];
		}
		else
		{
			win = winBuf[chirpSize-1-ii];
		}

		output[2*ii] = realIn * win/32768.f;
		output[2*ii+1] = imagIn * win/32768.f;
	}
	for (ii=chirpSize;ii<fftSize1D;ii++ )
	{
		output[2*ii]=0;
		output[2*ii+1] = 0;
	}
}


void test_window1DFltp(uint32_t numtest, uint32_t *fftSizeArray)
{
	uint32_t numTests;
	uint32_t ii,jj,kk;
	uint32_t fftSize;
	uint32_t numValidChirpSamples[5];

	for (ii=0;ii<numtest;ii++)
	{

		fftSize=fftSizeArray[ii];

		/* read in number of tested chirp size */
		fread(&numTests,sizeof(uint32_t),1,fid);
		/* read in valid chirp sizes */
		fread(&numValidChirpSamples[0],sizeof(int32_t),numTests,fid);

		/* read in the input data buffer */
		fread(&inputbuf[0],sizeof(int16_t),fftSize*2,fid);

		for (jj=0;jj<numTests;jj++)
		{
			/* read in the window buffer */
			if (numValidChirpSamples[jj]&0x1)  /* odd */
			{
				fread(&winBuf[0],sizeof(int16_t),(numValidChirpSamples[jj]>>1)+1,fid);
			}
			else
			{
				fread(&winBuf[0],sizeof(int16_t),(numValidChirpSamples[jj]>>1),fid);
				winBuf[(numValidChirpSamples[jj]>>1)] = winBuf[(numValidChirpSamples[jj]>>1)-1];    //window is symmetric
			}
			mmwavelib_windowing1DFltp(numValidChirpSamples[jj],
										  fftSize,
										  winBuf,
										  inputbuf,
										  outputbuf);

			ref_windowingFltp(numValidChirpSamples[jj],
							 fftSize,
                             0,
							 winBuf,
							 inputbuf,
							 refbuf);

			/* check the result */
			for (kk=0;kk<fftSize;kk++)
			{
				if ((outputbuf[2*kk] -refbuf[2*kk] >MAXIMUM_ERROR ) ||(outputbuf[2*kk] -refbuf[2*kk] <-MAXIMUM_ERROR))
				{
					testPASS=0;
					printf("fftsize=%d, jj=%d, chirpsize=%d, kk=%d, real\n",fftSize,jj,numValidChirpSamples[jj],kk);
				}

				if ((outputbuf[2*kk+1] -refbuf[2*kk+1] >MAXIMUM_ERROR ) ||(outputbuf[2*kk+1] -refbuf[2*kk+1] <-MAXIMUM_ERROR))
				{
					testPASS=0;
					printf("fftsize=%d, jj=%d, chirpsize=%d, kk=%d,imag\n",fftSize,jj,numValidChirpSamples[jj],kk);

				}
			}

		}

	}
}

void benchmark_window1DFltp(uint32_t numtest, uint32_t *fftSizeArray)
{
	uint32_t ii;
	for (ii=0;ii<numtest;ii++)
	{
		t1 = TSCL;

		mmwavelib_windowing1DFltp(fftSizeArray[ii],
									  fftSizeArray[ii],
									   winBuf,
									  inputbuf,
									  outputbuf);
		t2 = TSCL;
		cycles=t2-t1;
		printf("%d\t%d\n",fftSizeArray[ii],cycles );
	}
}

/* generate reference for 2D floating-point windowing */
void ref_windowing2DFltp(uint32_t numSamples, float win, float * input, float * output)
{
	uint32_t ii;
	for (ii=0;ii<numSamples;ii++)
	{
		output[2*ii] = input[2*ii] * win;
		output[2*ii+1] = input[2*ii+1]*win;
	}
}

void test_window2DFltp(uint32_t numTests, uint32_t * fftSizeArray, float * win2Dfactor)
{
	uint32_t ii,jj;


	for (ii=0;ii<numTests;ii++)
	{
		/* read in the float input data */
		fread(&outputbuf[0],sizeof(float),fftSizeArray[ii]*2,fid );
		/* call reference first */
		ref_windowing2DFltp(fftSizeArray[ii],win2Dfactor[ii], outputbuf,refbuf);
		/* call lib */
		mmwavelib_windowing2DFltp(fftSizeArray[ii],
								  win2Dfactor[ii],
								  outputbuf);
		/* check the results */

		for (jj=0;jj<fftSizeArray[ii]*2;jj++)
		{
			if ((refbuf[jj] - outputbuf[jj] > MAXIMUM_ERROR) || (refbuf[jj] - outputbuf[jj] < -1*MAXIMUM_ERROR))
			{
				testPASS=0;
			}
		}
	}
}

void benchmark_window2DFltp(uint32_t numTests, uint32_t * fftSizeArray, float * win2Dfactor)
{
	uint32_t ii;
	for (ii=0;ii<numTests;ii++)
	{
		t1 = TSCL;
		mmwavelib_windowing2DFltp(fftSizeArray[ii],
								      win2Dfactor[ii],
									  outputbuf);
		t2 = TSCL;
		cycles=t2-t1;
		printf("%d\t%d\n",fftSizeArray[ii],cycles );
	}
}

void test_window2DFxdpinFltOut(uint32_t numTests, uint32_t * fftSizeArray, int16_t * win2Dfactor)
{
	uint32_t ii,jj;
	uint32_t *inputPtr;
	float * output;

	inputPtr=(uint32_t*) inputbuf;
	for (ii=0;ii<numTests;ii++)
	{
		/* read in the fixed point input data */
		fread(&inputbuf[0],sizeof(int16_t),fftSizeArray[ii]*2,fid );

		/* call reference */
		for (jj = 0; jj < fftSizeArray[ii]/2;jj++)
		{
			winBuf[jj]=win2Dfactor[ii];
		}
		winBuf[fftSizeArray[ii]/2] = winBuf[fftSizeArray[ii]/2-1];

		ref_windowingFltp(fftSizeArray[ii],
						  fftSizeArray[ii],
                          0,
						  winBuf ,
						  inputbuf,
						  refbuf);
		/* call lib */
		mmwavelib_chirpProcWin2DFxdpinFltOut(fftSizeArray[ii],
										  win2Dfactor[ii],
										  inputPtr);
		output = (float*) &inputPtr[0]; /* change it into float point pointer */
		
		/* check the results */
		for (jj=0;jj<fftSizeArray[ii]*2;jj++)
		{
			if ((refbuf[jj] - output[jj] > MAXIMUM_ERROR) || (refbuf[jj] - output[jj] < -1*MAXIMUM_ERROR))
			{
				testPASS=0;
			}
		}

	}
}

void benchmark_window2DFxdpinFltOut(uint32_t numTests, uint32_t * fftSizeArray, int16_t * win2Dfactor)
{
	uint32_t ii;
	for (ii=0;ii<numTests;ii++)
	{
		t1 = TSCL;
		mmwavelib_chirpProcWin2DFxdpinFltOut(fftSizeArray[ii],
										  win2Dfactor[ii],
										  (void *)&inputbuf[0]);
		t2 = TSCL;
		cycles=t2-t1;
		printf("%d\t%d\n",fftSizeArray[ii],cycles );
	}
}


void test_window2DFxdpinFltOut_IQswap(uint32_t numTests, uint32_t * fftSizeArray, int16_t * win2Dfactor)
{
	uint32_t ii,jj;
	uint32_t *inputPtr;
	float * output;

	inputPtr=(uint32_t*) inputbuf;
	for (ii=0;ii<numTests;ii++)
	{
		/* read in the fixed point input data */
		fread(&inputbuf[0],sizeof(int16_t),fftSizeArray[ii]*2,fid );

		/* call reference */
		for (jj = 0; jj < fftSizeArray[ii]/2;jj++)
		{
			winBuf[jj]=win2Dfactor[ii];
		}
		winBuf[fftSizeArray[ii]/2] = winBuf[fftSizeArray[ii]/2-1];

		ref_windowingFltp(fftSizeArray[ii],
						  fftSizeArray[ii],
                          1,
						  winBuf ,
						  inputbuf,
						  refbuf);
		/* call lib */
		mmwavelib_chirpProcWin2DFxdpinFltOut_IQswap(fftSizeArray[ii],
										  win2Dfactor[ii],
										  inputPtr);
		output = (float*) &inputPtr[0]; /* change it into float point pointer */
		
		/* check the results */
		for (jj=0;jj<fftSizeArray[ii]*2;jj++)
		{
			if ((refbuf[jj] - output[jj] > MAXIMUM_ERROR) || (refbuf[jj] - output[jj] < -1*MAXIMUM_ERROR))
			{
				testPASS=0;
			}
		}

	}
}


void benchmark_window2DFxdpinFltOut_IQswap(uint32_t numTests, uint32_t * fftSizeArray, int16_t * win2Dfactor)
{
	uint32_t ii;
	for (ii=0;ii<numTests;ii++)
	{
		t1 = TSCL;
		mmwavelib_chirpProcWin2DFxdpinFltOut_IQswap(fftSizeArray[ii],
										  win2Dfactor[ii],
										  (void *)&inputbuf[0]);
		t2 = TSCL;
		cycles=t2-t1;
		printf("%d\t%d\n",fftSizeArray[ii],cycles );
	}
}



void test_dopplerProcWinFxdpinFltOut(uint32_t numTests, uint32_t * fftsizeArray)
{
	uint32_t ii,kk;
	for (ii=0;ii<numTests;ii++)
	{
		/* read in input */
		fread(&inputbuf[0],sizeof(int16_t),fftsizeArray[ii]*2,fid);
		/* read in window */
		fread(&winBuf[0],sizeof(int16_t),fftsizeArray[ii]/2,fid);

		/* call ref */
		ref_windowingFltp(fftsizeArray[ii],
							fftsizeArray[ii],
                          0,   //no IQ order swap  
						  winBuf ,
						  inputbuf,
						   refbuf);

		/* call lib */
		mmwavelib_dopplerProcWin2DFxdpinFltOut(
										fftsizeArray[ii],
										inputbuf,
										winBuf,
										outputbuf);
		/* check the results */
		for (kk=0;kk<2*fftsizeArray[ii];kk++)
		{
			if ((refbuf[kk]-outputbuf[kk]>MAXIMUM_ERROR)||(refbuf[kk]-outputbuf[kk]<-1*MAXIMUM_ERROR))
			{
				testPASS=0;
			}
		}
	}
}

void benchmark_dopplerProcWinFxdpinFltOut(uint32_t numTests, uint32_t * fftsizeArray)
{
	uint32_t ii;
	for (ii=0;ii<numTests;ii++)
	{
		t1 = TSCL;
		mmwavelib_dopplerProcWin2DFxdpinFltOut(
										fftsizeArray[ii],
										inputbuf,
										winBuf,
										outputbuf);
		t2 = TSCL;
		cycles=t2-t1;
		printf("%d\t%d\n",fftsizeArray[ii],cycles );
	}
}


void test_dopplerProcWinFxdpinFltOut_IQswap(uint32_t numTests, uint32_t * fftsizeArray)
{
	uint32_t ii,kk;
	for (ii=0;ii<numTests;ii++)
	{
		/* read in input */
		fread(&inputbuf[0],sizeof(int16_t),fftsizeArray[ii]*2,fid);
		/* read in window */
		fread(&winBuf[0],sizeof(int16_t),fftsizeArray[ii]/2,fid);

		/* call ref */
		ref_windowingFltp(fftsizeArray[ii],
						  fftsizeArray[ii],
                          1,   //input, output IQ order swap  
						  winBuf ,
						  inputbuf,
						   refbuf);

		/* call lib */
		mmwavelib_dopplerProcWin2DFxdpinFltOut_IQswap(
										fftsizeArray[ii],
										inputbuf,
										winBuf,
										outputbuf);
		/* check the results */
		for (kk=0;kk<2*fftsizeArray[ii];kk++)
		{
			if ((refbuf[kk]-outputbuf[kk]>MAXIMUM_ERROR)||(refbuf[kk]-outputbuf[kk]<-1*MAXIMUM_ERROR))
			{
				testPASS=0;
			}
		}
	}
}


void benchmark_dopplerProcWinFxdpinFltOut_IQswap(uint32_t numTests, uint32_t * fftsizeArray)
{
	uint32_t ii;
	for (ii=0;ii<numTests;ii++)
	{
		t1 = TSCL;
		mmwavelib_dopplerProcWin2DFxdpinFltOut_IQswap(
										fftsizeArray[ii],
										inputbuf,
										winBuf,
										outputbuf);
		t2 = TSCL;
		cycles=t2-t1;
		printf("%d\t%d\n",fftsizeArray[ii],cycles );
	}
}

void main(void)
{
	uint32_t numTestFFTSize;
	uint32_t fftSizeArray[MAXNUMFFTSIZE] ;
	float win2D[MAXNUMFFTSIZE];
	int16_t win2DArray[MAXNUMFFTSIZE];

	uint32_t numTestDopperSize;
	uint32_t dopperSizeArray[MAXNUMFFTSIZE];


	MCPI_Initialize();

	TSCL = 0;

	printf("\n------- 1D windowing float-point test\n");
    fid = fopen("..\\testdata\\windowFlt_test.bin","rb");
    if (fid==NULL)
    {
    	printf("File Open Error: windowFlt_test.bin can not open\n");
    	exit(3);
    }
    /* read in tested number of fftsize */
    fread(&numTestFFTSize, sizeof(uint32_t), 1, fid);
    /* read in tested fft size */
    fread(&fftSizeArray[0],sizeof(uint32_t), numTestFFTSize,fid);

    /* test 1D windowing with float point output */
	test_window1DFltp(numTestFFTSize,&fftSizeArray[0]);

    if (testPASS)
	{
    	MCPI_setFeatureTestResult("The windowing1DFltp functionality test", MCPI_TestResult_PASS);
	}
    else
	{
    	MCPI_setFeatureTestResult("The windowing1DFltp functionality test", MCPI_TestResult_FAIL);
	}

    printf("\n------- 1D windowing float-point benchmark\n");
    printf("fftsize, cycles\n");
    benchmark_window1DFltp(numTestFFTSize,&fftSizeArray[0]);


    printf("\n------- 2D windowing floating-point in and out test\n");
    /* read in 2D windoing factor */
    fread(&win2D[0],sizeof(float),numTestFFTSize,fid);
    test_window2DFltp(numTestFFTSize,fftSizeArray,win2D);
    if (testPASS)
	{
       	MCPI_setFeatureTestResult("The window2DFltp functionality test", MCPI_TestResult_PASS);
    }
	else
    {
		MCPI_setFeatureTestResult("The window2DFltp functionality test", MCPI_TestResult_FAIL);
	}
	
    printf("\n------- 2D windowing floating-point in and out benchmark\n");
    printf("chirpsize,cycles\n");
    benchmark_window2DFltp(numTestFFTSize,fftSizeArray,win2D);


    printf("\n------- 2D windowing fixed-point in floating-point out test\n");
    fread(&win2DArray[0],sizeof(int16_t),numTestFFTSize,fid);
    test_window2DFxdpinFltOut(numTestFFTSize,fftSizeArray,win2DArray);
    if (testPASS)
	{
        MCPI_setFeatureTestResult("The window2DFxdpinFltOut functionality test", MCPI_TestResult_PASS);
    }
	else
	{
		MCPI_setFeatureTestResult("The window2DFxdpinFltOut functionality test", MCPI_TestResult_FAIL);
	}
	
    printf("\n------- 2D windowing fixed-point in and float-point out benchmark\n");
    printf("chirpsize,cycles\n");

    benchmark_window2DFxdpinFltOut(numTestFFTSize,fftSizeArray,win2DArray);

    printf("\n------- 2D windowing fixed-point in floating-point out with IQ order swaptest\n");
    fread(&win2DArray[0],sizeof(int16_t),numTestFFTSize,fid);
    test_window2DFxdpinFltOut_IQswap(numTestFFTSize,fftSizeArray,win2DArray);
    if (testPASS)
	{
        MCPI_setFeatureTestResult("The window2DFxdpinFltOut_IQswap functionality test", MCPI_TestResult_PASS);
    }
	else
	{
		MCPI_setFeatureTestResult("The window2DFxdpinFltOut_IQswap functionality test", MCPI_TestResult_FAIL);
	}
    printf("\n------- 2D windowing fixed-point in and float-point out with I/Q order swap benchmark\n");
    printf("chirpsize,cycles\n");
    benchmark_window2DFxdpinFltOut_IQswap(numTestFFTSize,fftSizeArray,win2DArray);
    
    printf("\n----- 2D windowing across chirps with fixed-point in floating-point out test\n");
    /* number of dopper size test */
    fread(&numTestDopperSize,sizeof(uint32_t),1, fid);
    /* dopper fft size array */
    fread(&dopperSizeArray[0],sizeof(uint32_t),numTestDopperSize,fid );
    test_dopplerProcWinFxdpinFltOut(numTestDopperSize,dopperSizeArray);
    if (testPASS)
	{
    	MCPI_setFeatureTestResult("The dopplerProcWinFxdpinFltOut functionality test", MCPI_TestResult_PASS);
    }
	else
    {
		MCPI_setFeatureTestResult("The dopplerProcWinFxdpinFltOut functionality test", MCPI_TestResult_FAIL);
	}
    printf("\n------- 2D windowing across chirps fixed-point in and float-poing out benchmark\n");
    printf("chirpsize,cycles\n");
    benchmark_dopplerProcWinFxdpinFltOut(numTestDopperSize,dopperSizeArray);

    printf("\n----- 2D windowing across chirps with fixed-point in floating-point I/Q order swapped out  test\n");
     test_dopplerProcWinFxdpinFltOut_IQswap(numTestDopperSize,dopperSizeArray);
    if (testPASS)
	{
    	MCPI_setFeatureTestResult("The dopplerProcWinFxdpinFltOut_IQswap functionality test", MCPI_TestResult_PASS);
    }
	else
    {
		MCPI_setFeatureTestResult("The dopplerProcWinFxdpinFltOut_IQswap functionality test", MCPI_TestResult_FAIL);
	}
    printf("\n------- 2D windowing across chirps fixed-point in and float-point swapped I/Q order out benchmark\n");
    printf("chirpsize,cycles\n");
    benchmark_dopplerProcWinFxdpinFltOut_IQswap(numTestDopperSize,dopperSizeArray);
    fclose(fid);
	
	if (testPASS)
	{
		printf("========== All floating point windowing tests passed! ========== \n");
	}
	else
	{
		printf("==========  Floating point windowing tests Failed! ========== \n");
	}
	printf("========== Testing completed! ==========\n");

	MCPI_setTestResult ();
}
