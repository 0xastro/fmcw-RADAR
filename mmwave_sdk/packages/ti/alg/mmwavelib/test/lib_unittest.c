/*
 *   @file  lib_unittest.c
 *
 *   @brief
 *      Unit Test code for mmwavelib library functions
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016 Texas Instruments, Inc.
 *
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

#include <ti/alg/mmwavelib/mmwavelib.h>
#include "gen_twiddle_fft32x32.h"
#include "gen_twiddle_fft16x16.h"
#include "DSP_fft16x16.h"
#include "DSP_fft32x32.h"
#include <ti/utils/testlogger/logger.h>

/* Standard Include Files. */
#include <c6x.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define NUM_RANGE_BINS 		256
#define NUM_DOPPLER_BINS 	32  /* to go with 256 and 1024 Range bins */
#define NUM_ANT				8
#define NUM_ANGLE_BINS		16
#define MAX_NUM_RANGE_BINS 	1024 /* to go with 32 Doppler bins */
#define MAX_NUM_DOPPLER_BINS 	128  /* 128 to go with 256 Range bins */

extern void test_log2abs32();
extern void test_log2abs16();
extern void test_windowing_types(int32_t, int32_t);
extern void	benchmark_windowing();
extern void test_histogram_shift(int32_t, int32_t);
extern void test_hist_ones_allbins();
extern void test_shift32right();

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

/* Buffer to store twiddle factors for 1D FFT */
#pragma DATA_SECTION(xh_16x16Twiddle1dFFT,".l2data");
#pragma DATA_ALIGN(xh_16x16Twiddle1dFFT, 8);
volatile int16_t xh_16x16Twiddle1dFFT[MAX_NUM_RANGE_BINS*2];

/* buffer to store twiddle factors for 2D FFT */
#pragma DATA_ALIGN( xw_32x32Twiddle2dFFT,8);
#pragma DATA_SECTION( xw_32x32Twiddle2dFFT,".l2data");
int32_t xw_32x32Twiddle2dFFT[MAX_NUM_DOPPLER_BINS*2];

#pragma DATA_SECTION(inputbuf,".l2data");
#pragma DATA_ALIGN(inputbuf, 8);
int16_t inputbuf[MAX_NUM_RANGE_BINS*2];

/* buffer to store one half of real-symmetric window for 1D FFT */
#pragma DATA_SECTION(win1Dbuf,".l2data");
#pragma DATA_ALIGN(win1Dbuf, 8);
int16_t win1Dbuf[MAX_NUM_RANGE_BINS>>1];

/* buffer to store one half of real-symmetric window for 2D FFT */
#pragma DATA_SECTION(win2Dbuf,".l2data");
#pragma DATA_ALIGN(win2Dbuf, 8);
int32_t win2Dbuf[MAX_NUM_DOPPLER_BINS>>1];

#pragma DATA_SECTION(fft1Doutbuf,".l2data");
#pragma DATA_ALIGN(fft1Doutbuf, 8);
int16_t fft1Doutbuf[MAX_NUM_RANGE_BINS*2];

int16_t refbuf[MAX_NUM_RANGE_BINS*2];

/* External RadarCubed Memory. This is where the 1D FFT data is stored */
#pragma DATA_SECTION(radarcubebuf,".l3data");
#pragma DATA_ALIGN(radarcubebuf, 8);
int16_t radarcubebuf[1024*NUM_DOPPLER_BINS*2]; /* *NUM_ANT*2  //1024*32 = 256*128 */

#pragma DATA_SECTION(fft2Dinput,".l2data");
#pragma DATA_ALIGN(fft2Dinput, 8);
int16_t inputbuf2D[MAX_NUM_DOPPLER_BINS*2];

/* Temporary buffer for storing output of 2D windowing, i.e., input of 2D FFT */
#pragma DATA_SECTION(fft2Dinput,".l2data");
#pragma DATA_ALIGN(fft2Dinput, 8);
int32_t fft2Dinput[MAX_NUM_DOPPLER_BINS*2];

/* Temporary buffer for storing output of 2D FFT */
#pragma DATA_SECTION( fft2Doutbuf,".l2data");
#pragma DATA_ALIGN( fft2Doutbuf,8);
/* int32_t fft2Doutbuf[NUM_DOPPLER_BINS*2*NUM_RANGE_BINS]; */
int32_t fft2Doutbuf[MAX_NUM_DOPPLER_BINS*2];

#pragma DATA_SECTION( tmp32buf,".l2data");
#pragma DATA_ALIGN( tmp32buf,8);
int32_t tmp32buf[MAX_NUM_RANGE_BINS*2];

/* Temporary buffer for storing log2abs of 2D FFT output*/
#pragma DATA_SECTION( h_log2Abs,".l2data");
#pragma DATA_ALIGN( h_log2Abs,8);
uint16_t h_log2Abs[1024];

#pragma DATA_SECTION( h_sumAbs,".l2data");
#pragma DATA_ALIGN( h_sumAbs,8);
uint16_t h_sumAbs[1024*NUM_DOPPLER_BINS]; /* 1024*32 = 256*128 */

uint16_t   tmp16buf[MAX_NUM_DOPPLER_BINS];

/* buffer to store twiddle factors for 3D FFT */
#pragma DATA_SECTION( xw_Twiddle3D,".l2data");
#pragma DATA_ALIGN( xw_Twiddle3D,8);
int32_t xw_Twiddle3D[NUM_ANGLE_BINS*2]={
/* #include "twiddle3D.txt" */
2147483647,	0,1984016188,-821806413,1518500249,-1518500249,821806413,-1984016188,
0,-2147483647,-821806413,-1984016188,-1518500249,-1518500249,-1984016188,-821806413,
-2147483647,0,-1984016188,821806413,-1518500249,1518500249,-821806413,1984016188,
0,2147483647,821806413,1984016188,1518500249,1518500249,1984016188,821806413
};

#define NUM_OBJECTS 4
#pragma DATA_SECTION(anglefftInput,".l2data");
#pragma DATA_ALIGN(anglefftInput, 8);
int32_t anglefftInput[NUM_ANT*2*NUM_OBJECTS];

#pragma DATA_SECTION(anglefftOut,".l2data");
#pragma DATA_ALIGN(anglefftOut, 8);
int32_t anglefftOut[NUM_ANGLE_BINS*2*NUM_OBJECTS];


FILE       *fid;
int32_t    cnt;

int32_t    testFailed = 0;
int32_t    totalFailed = 0;
int32_t    prevFailed = 0;

volatile uint32_t w_startTime1;
uint32_t  t1, t2, cycles;
int32_t   tsc_overhead;

int32_t   moretv = 0; /* default 0 for tv size constraint */

#define COMPAREWIN 0 /* turn on if need debug */
#define FFT1DTEST  1
#define COMPARE_2DFFT 0

#if COMPARE_2DFFT
#define USEFFT1DREF   1
#define TEST_CASE2			 1
#define TEST_CASE2_MULTI	 1
#else
#define TEST_MULTIOBJ 1
#if TEST_MULTIOBJ
#define TEST_MULTIOBJ_SNR0   1
#define TEST_MULTIOBJ_M5     1
#define TEST_MULTIOBJ_M15    1
#endif
#define TEST_SINGLEOBJ 1
#if TEST_SINGLEOBJ
#define TEST_SINGLEOBJ_SNR0  1
#define TEST_SINGLEOBJ_M10   1
#define TEST_SINGLEOBJ_M15   1
#define TEST_CAPTURED_DATA   1
#endif
#define TEST_CASE1			 1
#define TEST_CASE1_MULTI	 1
#define TEST_CASE1_512  	 1
#define TEST_CASE2			 1
#define TEST_CASE2_MULTI	 1
#endif


void test_window1d_rangefft(int32_t numRangeBins, int32_t numDopplerBins)
{
	int32_t   i, j, chirpIdx;
	int16_t * fft1Doutptr;
	uint32_t  wincyc, fftcyc;

    TSCL = 0;
    printf("\n--- 1D Processing:\n");

	fft1Doutptr = &fft1Doutbuf[0];
	testFailed=0;

    /* For every chirp, test 1D windowing and 1D Range FFT */
	for ( chirpIdx = 0; chirpIdx < numDopplerBins; chirpIdx ++)
	{
  		/* printf("chirp index %d:\n", chirpIdx); */
		cnt = fread(inputbuf, sizeof(int16_t), numRangeBins*2, fid);

#if COMPAREWIN
		for ( i=0; i < numRangeBins/2; i++)
		{
			tmpbuf[2*i] = ((int32_t)inputbuf[2*i]*win1Dbuf[i] + 0x4000) >>15;
			tmpbuf[2*i+1] = ((int32_t)inputbuf[2*i+1]*win1Dbuf[i] + 0x4000) >>15;
			tmpbuf[2*(numRangeBins-1-i)] = ((int32_t)inputbuf[2*(numRangeBins-1-i)] * win1Dbuf[i] + 0x4000) >>15;
			tmpbuf[2*(numRangeBins-1-i)+1] = ((int32_t)inputbuf[2*(numRangeBins-1-i)+1] * win1Dbuf[i] + 0x4000) >>15;
		}
#endif

#ifdef FFT1DTEST
		w_startTime1=TSCL;
		mmwavelib_windowing16x16( inputbuf, win1Dbuf, numRangeBins);
		wincyc = TSCL-w_startTime1;
		/* printf("window16x16 cycles =%d ", wincyc); */
#endif

#if COMPAREWIN
		for ( i=0; i < numRangeBins*2; i++)
		{
			if ( (inputbuf[i] - tmpbuf[i]) !=0  )
			{
				testFailed++;
				printf("windowing test failed!\n");
				break;
			}
		}
#endif

		/* fft1Doutptr = &fft1Doutbuf[numRangeBins*2*chirpIdx]; */

#ifdef FFT1DTEST
		cnt = fread(refbuf, sizeof(int16_t), numRangeBins*2, fid);

		w_startTime1=TSCL;
		DSP_fft16x16((int16_t*)xh_16x16Twiddle1dFFT, numRangeBins, inputbuf, fft1Doutptr);
		fftcyc = TSCL-w_startTime1;
		/* printf("FFT16x16 cycles =%d\n", fftcyc); */

		for ( i=2; i < numRangeBins*2; i++) /* ignore first complex sample */
		{
			if ( ((fft1Doutptr[i] - refbuf[i])>12) || ((fft1Doutptr[i] - refbuf[i])<-12) ) /* 10, case1:12 */
			{
				testFailed++;
				printf("FFT16x16 test failed!i=%d, data %d ref %d, diff=%d\n",
					i, fft1Doutptr[i], refbuf[i],(fft1Doutptr[i] - refbuf[i]));
				/* break; */
			}
		}

		if ( testFailed >0 )
		{
		    printf("chirp index %d\n", chirpIdx);
		    totalFailed += testFailed;
		}

#ifdef USEFFT1DREF
		for ( i=0; i < numRangeBins*2; i++)
		{
			fft1Doutptr[i] = refbuf[i];
		}
#endif

#else /* if not run 1dfft, read 1dfft output from tv: */
		cnt = fread(fft1Doutptr, sizeof(int16_t), numRangeBins*2, fid);
#endif


		for ( j=0; j < numRangeBins*2; j+=2 )
		{
#if 0
			/* transfer to chirpIdx-th row of radar cube memory */
			radarcubebuf[chirpIdx*numRangeBins*2 + j] = fft1Doutbuf[j];
			radarcubebuf[chirpIdx*numRangeBins*2 + j +1] = fft1Doutbuf[j+1];
#else
			/* transfer to columns:chirpIdx*2 and chirpIdx*2+1, of radar cube memory */
			radarcubebuf[j*numDopplerBins + chirpIdx*2] = fft1Doutbuf[j];
			radarcubebuf[j*numDopplerBins + chirpIdx*2+1] = fft1Doutbuf[j+1];
#endif
		}


	} /* chirp loop for range FFT */
	printf("%d-point Window16x16 cycles =%d, FFT16x16 cycles =%d\n", numRangeBins, wincyc, fftcyc);

	if ( testFailed ==0 )
	{
		printf("Test 1D windowing16x16 and FFT16x16 passed.\n\n");
	}

}


void benchmark_64pt_2dprocess(uint16_t refRangeBin)
{
	int32_t    i, numDopplerBins =64;
	uint32_t   wincyc, fftcyc, logcyc, accumcyc, winSwapcyc;

	printf("\nBenchmark %d-point 2D processing:\n", numDopplerBins);

	for (i = refRangeBin-2; i<=refRangeBin; i++)
	{
		memcpy((void*)&radarcubebuf[i*64*2], inputbuf2D, numDopplerBins*2);
		w_startTime1 = TSCL;
		mmwavelib_windowing16x32(inputbuf2D, win2Dbuf, fft2Dinput, numDopplerBins);
		wincyc = TSCL-w_startTime1;
        
        w_startTime1 = TSCL;
		mmwavelib_windowing16x32_IQswap(inputbuf2D, win2Dbuf, fft2Dinput, numDopplerBins);
		winSwapcyc = TSCL-w_startTime1;
        

		w_startTime1 = TSCL;
		DSP_fft32x32((int32_t*)xw_32x32Twiddle2dFFT, numDopplerBins, fft2Dinput, fft2Doutbuf); //fft2Doutptr);
		fftcyc = TSCL-w_startTime1;

		w_startTime1 = TSCL;
		mmwavelib_log2Abs32(fft2Doutbuf, h_log2Abs, numDopplerBins); //fft2Doutptr
		logcyc = TSCL-w_startTime1;

		w_startTime1 = TSCL;
		mmwavelib_accum16(h_log2Abs,tmp16buf, numDopplerBins);
		accumcyc = TSCL-w_startTime1;

		printf("%d-point Win16x32 cycles =%d; Win16x32_IQswap cycles =%d; FFT32x32 cycles =%d; logabs cycles =%d; accum16 cycles =%d\n", numDopplerBins, wincyc, winSwapcyc, fftcyc, logcyc, accumcyc);
	}
}


void test_window2d_dopplerfft_log2abs(int32_t numRangeBins, int32_t numDopplerBins, uint32_t cfardBc1 )
{
	int32_t    i, rangebinIdx;
	int16_t    diff;
	uint32_t   wincyc, fftcyc, logcyc, accumcyc;
	float      ftemp, fdiff;
	int32_t    sumIQ[2];
	int32_t    sumref[2]={0};
	int16_t    avgIQ[2];
	uint32_t   avgIQpack;
	int16_t *  tmp16ptr=(int16_t *)&tmp32buf[0];
	uint32_t   sumcyc, sumrefcyc, subcyc, subrefcyc;
    int16_t tempValue;

    printf("--- 2D Processing: ---\n");
	testFailed=0;


	for ( rangebinIdx = 0; rangebinIdx <numRangeBins; rangebinIdx ++)
	{
#if 0 /* 1D out in cube was stored row by row */
		for ( i=0; i < numDopplerBins; i++)
		{
			inputbuf2D[2*i] = radarcubebuf[i*numRangeBins*2 + rangebinIdx*2];/* fft1Doutbuf */
			inputbuf2D[2*i+1] = radarcubebuf[i*numRangeBins*2 + rangebinIdx*2 +1];
		}
#else /* 1D out in cube was stored col by col. So 2D input is in one row. */
		for ( i=0; i < 2*numDopplerBins; i++)
		{
			inputbuf2D[i] = radarcubebuf[rangebinIdx*numDopplerBins*2 + i];
		}
#endif
		/* --- test vecsum ---*/
		w_startTime1 = TSCL;
		mmwavelib_vecsum(inputbuf2D, sumIQ,numDopplerBins);
		sumcyc = TSCL-w_startTime1;
		//printf("vecsum cycles =%d ", sumcyc);
		sumref[0] = 0;
		sumref[1] = 0;
		w_startTime1 = TSCL;
		for ( i=0; i < numDopplerBins; i++)
		{
			sumref[0] += inputbuf2D[2*i];
			sumref[1] += inputbuf2D[2*i+1];
		}
		sumrefcyc = TSCL-w_startTime1;
		if (( sumIQ[0]!=sumref[0] ) || ( sumIQ[1]!=sumref[1] ))
		{
			printf("sumIQ %d, %d ref %d, %d\n", sumIQ[0], sumIQ[1], sumref[0], sumref[1]);
			testFailed++;
			printf("mmwavelib_vecsum test failed summing %d elements. rangebin=%d.\n", numDopplerBins, rangebinIdx);
		}

		/* --- test vecsubc --- */
		avgIQ[0]  = (int16_t)(sumIQ[0]/numDopplerBins);
		avgIQ[1]  = (int16_t)(sumIQ[1]/numDopplerBins);
		avgIQpack = *((uint32_t*)avgIQ);
		w_startTime1 = TSCL;
		for ( i=0; i < numDopplerBins; i++)
		{
			tmp16ptr[2*i] = inputbuf2D[2*i] - avgIQ[0];
			tmp16ptr[2*i+1] = inputbuf2D[2*i+1] - avgIQ[1];
		}
		subrefcyc = TSCL-w_startTime1;
		w_startTime1 = TSCL;
		mmwavelib_vecsubc(inputbuf2D, &tmp16ptr[numDopplerBins*2], avgIQpack,numDopplerBins);
		subcyc = TSCL-w_startTime1;
		for ( i=0; i < numDopplerBins*2; i++)
		{
			if ( tmp16ptr[i+numDopplerBins*2] != tmp16ptr[i])
			{
				testFailed++;
				printf("mmwavelib_vecsubc test failed dopplerbin=%d. rangebin=%d.\n", i/2, rangebinIdx);
			}
		}


		/* --- test windowing16x32 --- */
		w_startTime1 = TSCL;
		mmwavelib_windowing16x32(inputbuf2D, win2Dbuf, fft2Dinput, numDopplerBins);
		wincyc = TSCL-w_startTime1;
		/* printf("win16x32 cycles =%d ", wincyc); */

		for ( i=0; i < numDopplerBins/2; i++)
		{
			tmp32buf[2*i] = ( (int64_t)win2Dbuf[i]*inputbuf2D[2*i] + 0x4000 ) >>15;
			tmp32buf[2*i+1] = ( (int64_t)win2Dbuf[i]*inputbuf2D[2*i+1] + 0x4000 ) >>15;
			tmp32buf[2*(numDopplerBins-i)-2] = ( (int64_t)win2Dbuf[i]* inputbuf2D[2*(numDopplerBins-i)-2] + 0x4000 ) >>15;
			tmp32buf[2*(numDopplerBins-i)-1] = ( (int64_t)win2Dbuf[i]* inputbuf2D[2*(numDopplerBins-i)-1] + 0x4000 ) >>15;
		}

		for ( i=0; i <numDopplerBins*2; i++)
		{
			if ( (fft2Dinput[i] - tmp32buf[i]) !=0  )
			{
				testFailed++;
				printf("windowing16x32 test failed!i=%d,data %d ref %d, diff=%d\n", i,fft2Dinput[i],tmp32buf[i],fft2Dinput[i]-tmp32buf[i]);
				break;
			}
		}
        
        /* --- test windowing16x32_IQswap --- */
        for (i = 0; i < numDopplerBins; i++)  /* swap the input I/Q order */
        {
            tempValue = inputbuf2D[2 * i];
            inputbuf2D[2 * i] = inputbuf2D[2 * i + 1];
            inputbuf2D[2 * i + 1] = tempValue;
        }
		mmwavelib_windowing16x32_IQswap(inputbuf2D, win2Dbuf, fft2Dinput, numDopplerBins);
		
		for ( i=0; i <numDopplerBins*2; i++)
		{
			if ( (fft2Dinput[i] - tmp32buf[i]) !=0  )
			{
				testFailed++;
				printf("windowing16x32_IQswap test failed!i=%d,data %d ref %d, diff=%d\n", i,fft2Dinput[i],tmp32buf[i],fft2Dinput[i]-tmp32buf[i]);
				break;
			}
		}
        

		/* only when big buffer available to store all fft2D output: fft2Doutptr = &fft2Doutbuf[NUM_DOPPLER_BINS*2*rangebinIdx]; */
		w_startTime1 = TSCL;
		DSP_fft32x32((int32_t*)xw_32x32Twiddle2dFFT, numDopplerBins, fft2Dinput, fft2Doutbuf); /* fft2Doutptr); */
		fftcyc = TSCL-w_startTime1;
		/* printf("FFT32x32 cycles =%d ", fftcyc); */

		/* --- test log2Abs32 --- */
		w_startTime1 = TSCL;
		mmwavelib_log2Abs32(fft2Doutbuf, h_log2Abs, numDopplerBins); /* fft2Doutptr */
		logcyc = TSCL-w_startTime1;


#if COMPARE_2DFFT
		cnt = fread(tmp32buf, sizeof(int32_t), numDopplerBins*2, fid);

		for ( i=0; i < 8; i++)
		{	if ( ((fft2Doutbuf[i] - tmp32buf[i])>45) || ((fft2Doutbuf[i] - tmp32buf[i])<-45) )
				printf("range bin %d: FFT32x32 test: i=%d, data %d ref %d, diff=%d\n", rangebinIdx, i, fft2Doutbuf[i], tmp32buf[i],(fft2Doutbuf[i] - tmp32buf[i]));
		}
		for ( i=8; i < numDopplerBins*2; i++)
		{
			if ( ((fft2Doutbuf[i] - tmp32buf[i])>25) || ((fft2Doutbuf[i] - tmp32buf[i])<-25) )
			{
			    printf("range bin index %d:", rangebinIdx);
				printf("FFT32x32 test: i=%d, data %d ref %d, diff=%d\n", i, fft2Doutbuf[i], tmp32buf[i],(fft2Doutbuf[i] - tmp32buf[i]));
				/*
				testFailed++;
				break;
				*/
			}
		}
#endif
#if 1 /* Compare log2abs output */

		cnt = fread(tmp16buf, sizeof(uint16_t), numDopplerBins, fid);
		for ( i=1; i < numDopplerBins; i++ ) /*  ignore dc */
		{
			diff = h_log2Abs[i] - tmp16buf[i];
			if ( (diff > 77 || diff  < -77) && tmp16buf[i]>6*256 ) /* 77=0.3*256 */
			{
				ftemp = log10(fft2Doutbuf[2*i]*fft2Doutbuf[2*i]+fft2Doutbuf[2*i+1]*fft2Doutbuf[2*i+1]) * 3.322 /2;
				fdiff = (float)h_log2Abs[i]/256 - ftemp;
				/*
			    printf("\nrange bin index %d: ", rangebinIdx);
				printf("log2abs: i=%d, data %d ref %d, diff=%d; ", i, h_log2Abs[i], tmp16buf[i],diff);
				printf("float actual=%5.2f, (data/256-actual)=%5.2f\n", ftemp, fdiff);
				*/
				if ( fdiff > 0.16 || fdiff <-0.16)
				{
			    	testFailed++;
				    printf("\nrange bin index %d: ", rangebinIdx);
					printf("log2abs: i=%d, data %d ref %d, diff=%d; ", i, h_log2Abs[i], tmp16buf[i],diff);
					printf("float actual=%5.2f, (data/256-actual)=%5.2f\n", ftemp, fdiff);
				}
			}
		}
#endif

		w_startTime1 = TSCL;
		mmwavelib_accum16(h_log2Abs,&h_sumAbs[rangebinIdx*numDopplerBins], numDopplerBins);
		accumcyc = TSCL-w_startTime1;

#if CFAR_PER_ANT
		int32_t    numDetObjTemp=0;
		uint16_t   h_detObjIdxTemp[NUM_DOPPLER_BINS];

		numDetObjTemp = mmwavelib_cfarCadB(h_log2Abs, h_detObjIdxTemp,numDopplerBins, cfardBc1,4,4,8);
		if ( numDetObjTemp >0 )
		{
	        printf("\nrange bin index %d,num detected object: %d, detected doppler bin index: %d", rangebinIdx,numDetObjTemp,h_detObjIdxTemp[0]);
	        printf(" (%d)", h_log2Abs[h_detObjIdxTemp[0]]);
		    for ( i=1; i<numDetObjTemp;i++)
		        printf(",%d (%d)", h_detObjIdxTemp[i], h_log2Abs[h_detObjIdxTemp[i]]);
		    printf("\n");
		}
#endif

	}/* end of loop rangebin */

	printf("%d-point Win16x32 cycles =%d; FFT32x32 cycles =%d; logabs cycles =%d; accum16 cycles =%d ", numDopplerBins, wincyc, fftcyc, logcyc, accumcyc);
	printf("\n%d-point vecsum cycles =%d, sumref cycles =%d; vecsubc cycles =%d, subref cycles =%d\n", numDopplerBins, sumcyc, sumrefcyc, subcyc, subrefcyc);

	if ( testFailed ==0 )
	{
		printf("\nTest 2D windowing16x32, 2D windowing16x32_IQswap, and FFT32x32, log2Abs32 passed.\n");
	}
	else
		totalFailed += testFailed;

} /*test_window2d_dopplerfft_log2abs_cfar*/


void test_accumLog2abs_cfar ( int32_t numRangeBins,
		                      int32_t numDopplerBins,
							  uint16_t refNumObject,
							  uint16_t *refRangeBin,
							  uint16_t *refDopBin,
							  uint32_t cfardBc1 )
{
	int32_t    i, rangebinIdx, j;
	uint16_t * h_sumAbsPtr;
	uint32_t    numDetObjTemp=0;
	uint16_t   h_detObjIdxTemp[NUM_DOPPLER_BINS];
	uint32_t   c2oneside, noiselen, guardlen;
	uint32_t   temp32, sumRight, sumLeft;
	uint16_t   numDetect=0;
	uint16_t   gap;

	c2oneside = 3;
	noiselen = (1 << c2oneside); /* one side noise length */
	guardlen = 4;

	testFailed = 0;

	printf("\n --- CFAR on accumulated (over all antennae) log2abs values:\n");

	for ( rangebinIdx = 0; rangebinIdx < numRangeBins; rangebinIdx++ )
	{
		fread (tmp16buf, sizeof(uint16_t), numDopplerBins, fid);
		h_sumAbsPtr = &h_sumAbs[rangebinIdx * numDopplerBins];

		for ( i=1; i < numDopplerBins; i++ )
		{
			if ( (h_sumAbsPtr[i] - tmp16buf[i]) >205 || (h_sumAbsPtr[i] - tmp16buf[i]) <-205 ) /* 0.8db error */
			{
				if ( h_sumAbsPtr[i] > 2000*4) /* 4ant captured data */
				{
					printf("range bin %d, doppler bin %d: sumAbs=%d, ref=%d, diff=%d\n",
						rangebinIdx,i,h_sumAbsPtr[i],tmp16buf[i],h_sumAbsPtr[i]-tmp16buf[i]);
				}
			}
		}

#if CFAR_PER_RANGE
		numDetObjTemp = mmwavelib_cfarCadB( h_sumAbsPtr, h_detObjIdxTemp,
											numDopplerBins,
											cfardBc1, c2oneside+1,
											guardlen, noiselen);
		if ( numDetObjTemp >0 )
		{
	        printf("\nrange bin index %d,num detected object: %d, detected doppler bin index: %d",
							rangebinIdx, numDetObjTemp, h_detObjIdxTemp[0]);
	        printf(" (%d)", h_sumAbsPtr[h_detObjIdxTemp[0]]);
		    for ( i=1; i<numDetObjTemp;i++)
		        printf(",%d (%d)", h_detObjIdxTemp[i], h_sumAbsPtr[h_detObjIdxTemp[i]]);
		    printf("\n");

		    for ( i=0; i < refNumObject; i++ )
		    {
				if ( rangebinIdx == refRangeBin[i] )
				{
					if ( h_detObjIdxTemp[0] == refDopBin[i] )
					{
				    	numDetect ++;
			    		printf("Detected the object%d succesfully. \n", i+1);
					}
					else if ( numDetObjTemp > 1 && h_detObjIdxTemp[1] == refDopBin[i] )
					{
				    	numDetect ++;
			    		printf("Detected the object%d succesfully. \n", i+1);
					}
				}
		    }
		} /* if numDetObjTemp >0 */
#endif

	} /* rangebinIdx loop */

	printf("\nRun %d-point cfarCadB (type CFAR_CA) on every doppler bin:\n",numRangeBins);
	for ( j = 0; j < numDopplerBins; j++ )
	{

		for ( i = 0; i < numRangeBins; i ++)
		{
			h_log2Abs[i] = h_sumAbs[ i*numDopplerBins + j ];
		}
		w_startTime1 = TSCL;
		numDetObjTemp = mmwavelib_cfarCadB_SOGO( h_log2Abs, h_detObjIdxTemp,
												numRangeBins,
												CFAR_CA,
												cfardBc1, c2oneside+1,
												guardlen, noiselen);
		cycles = TSCL-w_startTime1;

		if ( numDetObjTemp >0 )
		{
			printf("Doppler Bin %d: Num detected object: %d, detected range bin index: %d",
								j, numDetObjTemp, h_detObjIdxTemp[0]);
			printf(" (%d)", h_log2Abs[h_detObjIdxTemp[0]]);
			for ( i=1; i<numDetObjTemp;i++)
				printf(",%d (%d)", h_detObjIdxTemp[i], h_log2Abs[h_detObjIdxTemp[i]]);
			printf("\n");
		}

	    for ( i=0; i < refNumObject; i++ )
	    {
			if ( j == refDopBin[i] )
			{
				if ( h_detObjIdxTemp[0] == refRangeBin[i] )
				{
			    	numDetect ++;
		    		printf("Detected the object%d succesfully. \n", i+1);
				}
				else if ( numDetObjTemp > 1 && h_detObjIdxTemp[1] == refRangeBin[i] )
				{
			    	numDetect ++;
		    		printf("Detected the object%d succesfully. \n", i+1);
				}
			}
	    }
	}/* end of for ( j = 0; j < numDopplerBins; j++ ) */

	if ( numDetect < refNumObject )
	{
		testFailed++;
		printf("Test Failed: Didn't detect all %d objects.\n", refNumObject);
	}

	printf("\ncfarCadB_SOGO (type CFAR_CA) cycles for length %d: %d\n", numRangeBins, cycles);

	printf("\n ----- Miss/False detection test on cfarCadB; cfarCadB_SOGO for type CA_SO and CA-GO:\n");
	printf("Run %d-point cfar:\n",numRangeBins);
	for ( j = 0; j < refNumObject; j++ )
	{
		printf("\nOn doppler bin %d :\n", refDopBin[j]);
		for ( i = 0; i < numRangeBins; i ++)
		{
			h_log2Abs[i] = h_sumAbs[ i*numDopplerBins + refDopBin[j] ];
		}
		w_startTime1 = TSCL;
		numDetObjTemp = mmwavelib_cfarCadB( h_log2Abs, h_detObjIdxTemp, 
											numRangeBins,
											cfardBc1, c2oneside+1,
											guardlen, noiselen);
		cycles = TSCL-w_startTime1;

		if ( numDetObjTemp >0 )
		{
			printf("CA: when threshold=%d: Num detected object: %d, detected range bin index: %d",
									cfardBc1, numDetObjTemp, h_detObjIdxTemp[0]);
			printf(" (%d)",h_log2Abs[h_detObjIdxTemp[0]]);
			for ( i=1; i<numDetObjTemp;i++)
				printf(",%d (%d)", h_detObjIdxTemp[i], h_log2Abs[h_detObjIdxTemp[i]]);
			printf("\n");
		}
		for ( i=0; i<numDetObjTemp;i++)
		{
			if ( h_detObjIdxTemp[i] > refRangeBin[j]+1 || h_detObjIdxTemp[i] < refRangeBin[j]-1 )
			{	testFailed++;
				printf("Detected range bin non-adjacent to ref object. \n");
			}
		}
		if ( numDetObjTemp >0 && testFailed ==0 )
			printf("Detected the object%d succesfully. \n", j+1);

		/* ----- test CFAR calculations ----- */
		temp32 = 0;
		sumRight = 0;
		sumLeft  = 0;
		if ( refRangeBin[j] >=12 && refRangeBin[j] < (numRangeBins-12) )
		{
			for ( i = 1; i <= noiselen; i++)
			{
				sumRight += h_log2Abs[ refRangeBin[j] + guardlen +i ];
				sumLeft  += h_log2Abs[ refRangeBin[j] - guardlen -i ];
			}
			temp32 = sumRight + sumLeft; /* sum noise */
			temp32 = temp32 >> (c2oneside+1); /* avg noise */
		}
		else if (refRangeBin[j] < 12)
		{
			for ( i = 1; i <= noiselen; i++)
			{
				temp32 += h_log2Abs[ refRangeBin[j] + guardlen +i ];
			}
			temp32 = temp32 >> c2oneside; /* avg noise */
		}
#if 1 /* one side noise calculation */
		else /* peak is near ending edge of all range bins */
		{
			for ( i = 1; i <= noiselen; i++)
			{
				temp32 += h_log2Abs[ refRangeBin[j] - guardlen -i ];
			}
			temp32 = temp32 >> c2oneside; /* avg noise */
		}
#else /* two sides, only if wrap version cfar is used next. */
		else
		{
			for ( i = 1; i <= noiselen; i++)
			{
				temp32 += h_log2Abs[ refRangeBin[j] - guardlen -i ];
			}
			for ( i = 1; i <= noiselen; i++)
				temp32 += h_log2Abs[ (refRangeBin[j] +4+i)&(numRangeBins-1) ];
			temp32 = temp32 >> (c2oneside+1); /* avg noise */
		}
#endif
		gap = h_log2Abs[refRangeBin[j]]-temp32;
		printf("CA: peak= %d, avg noise= %d, gap= %d.\n",
			h_log2Abs[refRangeBin[j]], temp32, gap );

		/* Test CfarCadB on missed detection: */
    	numDetObjTemp = mmwavelib_cfarCadB( h_log2Abs, h_detObjIdxTemp,
											numRangeBins,
											gap-1, c2oneside+1,
											guardlen, noiselen);
    	if ( numDetObjTemp >0 && h_detObjIdxTemp[0] == refRangeBin[j] )
    		printf("PASS: no missed detection when threshold=%d.\n", gap-1);
    	else
    	{
    		testFailed++;
    		printf("CfarCadB function FAILED with missed detection when threshold=%d.\n", gap-1);
    	}

		/* Test CfarCadB on false detection: */
    	numDetObjTemp = mmwavelib_cfarCadB( h_log2Abs, h_detObjIdxTemp, 
											numRangeBins,
											gap, c2oneside+1,
											guardlen, noiselen);
    	if ( numDetObjTemp ==0 )
    		printf("PASS: no false detection when threshold=%d.\n", gap);
    	else
    	{
    		testFailed++;
    		printf("CfarCadB function FAILED with false detection when threshold=%d.\n", gap);
    	}

		/* ----- test CFAR CA-SO and CA-GO calculations ----- */
		if ( refRangeBin[j] >=12 && refRangeBin[j] < (numRangeBins-12) )
		{	/* CA-SO: */
			numDetObjTemp = mmwavelib_cfarCadB_SOGO( h_log2Abs, h_detObjIdxTemp,
												numRangeBins,
												CFAR_CASO,
												cfardBc1, c2oneside+1,
												guardlen, noiselen);
			if ( numDetObjTemp >0 )
			{
				printf("\nCA-SO: when threshold=%d: Num detected object: %d, detected range bin index: %d",
						cfardBc1, numDetObjTemp, h_detObjIdxTemp[0]);
				printf(" (%d)",h_log2Abs[h_detObjIdxTemp[0]]);
				for ( i=1; i<numDetObjTemp;i++)
					printf(",%d (%d)", h_detObjIdxTemp[i], h_log2Abs[h_detObjIdxTemp[i]]);
				printf("\n");
			}

			temp32 = (sumLeft < sumRight)?sumLeft:sumRight; /* SO */
			temp32 >>= c2oneside;
			gap = h_log2Abs[refRangeBin[j]] - temp32;
			printf("CA-SO: peak= %d, CASO avg noise= %d, gap= %d.\n",
					h_log2Abs[refRangeBin[j]], temp32, gap );
		
			/* Test CfarCadB CA-SO on missed detection: */
    		numDetObjTemp = mmwavelib_cfarCadB_SOGO( h_log2Abs, h_detObjIdxTemp,
											numRangeBins,
											CFAR_CASO,
											gap-1, c2oneside+1,
											guardlen, noiselen);

    		if ( numDetObjTemp >0 && ( h_detObjIdxTemp[0] == refRangeBin[j] ||
        		 ( numDetObjTemp >1 && h_detObjIdxTemp[1] == refRangeBin[j] )))
    			printf("PASS: cfarCadB_SOGO type CA-SO has no missed detection when threshold=%d.\n", gap-1);
    		else
    		{
    			testFailed++;
    			printf("cfarCadB_SOGO, type CASO: FAILED with missed detection when threshold=%d.\n", gap-1);
    		}

			/* Test CfarCadB CA-SO on false detection: */
    		numDetObjTemp = mmwavelib_cfarCadB_SOGO( h_log2Abs, h_detObjIdxTemp, 
											numRangeBins,
											CFAR_CASO,
											gap, c2oneside+1,
											guardlen, noiselen);
    		if ( numDetObjTemp ==0 )
    			printf("PASS: cfarCadB_SOGO type CASO has no false detection when threshold=%d.\n", gap);
    		else
    		{
    			testFailed++;
    			printf("CfarCadB_SOGO, type CASO: FAILED with false detection when threshold=%d.\n", gap);
    		}


			/* CA-GO: */
			numDetObjTemp = mmwavelib_cfarCadB_SOGO( h_log2Abs, h_detObjIdxTemp,
												numRangeBins,
												CFAR_CAGO,
												cfardBc1, c2oneside+1,
												guardlen, noiselen);
			if ( numDetObjTemp >0 )
			{
				printf("\nCA-GO /w threshold %d: Num detected object: %d, detected range bin index: %d",
						cfardBc1, numDetObjTemp, h_detObjIdxTemp[0]);
				printf(" (%d)",h_log2Abs[h_detObjIdxTemp[0]]);
				for ( i=1; i<numDetObjTemp;i++)
					printf(",%d (%d)", h_detObjIdxTemp[i], h_log2Abs[h_detObjIdxTemp[i]]);
				printf("\n");
			}

			temp32 = (sumLeft > sumRight) ? sumLeft:sumRight; /* GO */
			temp32 >>= c2oneside;
			gap = h_log2Abs[refRangeBin[j]] - temp32;
			printf("CA-GO: peak= %d, CAGO avg noise= %d, gap= %d.\n",
					h_log2Abs[refRangeBin[j]], temp32, gap );
			/* Test CfarCadB CA-GO on missed detection: */
    		numDetObjTemp = mmwavelib_cfarCadB_SOGO( h_log2Abs, h_detObjIdxTemp,
											numRangeBins,
											CFAR_CAGO,
											gap-1, c2oneside+1,
											guardlen, noiselen);

    		if ( numDetObjTemp >0 && h_detObjIdxTemp[0] == refRangeBin[j] )
    			printf("PASS: cfarCadB_SOGO type CAGO has no missed detection when threshold=%d.\n", gap-1);
    		else
    		{
    			testFailed++;
    			printf("cfarCadB_SOGO, type CAGO: FAILED with missed detection when threshold=%d.\n", gap-1);
    		}

			/* Test CfarCadB CA-GO on false detection: */
    		numDetObjTemp = mmwavelib_cfarCadB_SOGO( h_log2Abs, h_detObjIdxTemp, 
											numRangeBins,
											CFAR_CAGO,
											gap, c2oneside+1,
											guardlen, noiselen);
    		if ( numDetObjTemp ==0 )
    			printf("PASS: cfarCadB_SOGO type CAGO has no false detection when threshold=%d.\n", gap);
    		else
    		{
    			testFailed++;
    			printf("CfarCadB_SOGO, type CAGO: FAILED with false detection when threshold=%d.\n", gap);
    		}


		} /* test CA-SO and CA-GO, when two sides of noise available */

	} /* loop refNumObject. test on cfarCadB. and test on CA-SO, CA-GO */

	printf("\ncfarCadB cycles for length %d: %d\n", numRangeBins, cycles);

	printf("\ntestFailed = %d.\n", testFailed);


	printf("\n Miss/False detection test on cfarCadBwrap:\n");
	printf("Run %d-point cfar:\n",numDopplerBins);
	for ( j = 0; j < refNumObject; j++ )
	{
		/*
		if (refDopBin[j] < (noiselen+guardlen) || refDopBin[j] >= (numDopplerBins-(noiselen+guardlen)))
		*/
		{
			printf("\nOn range bin %d :", refRangeBin[j]);
			h_sumAbsPtr = &h_sumAbs[refRangeBin[j] * numDopplerBins];
			w_startTime1 = TSCL;
	    	numDetObjTemp = mmwavelib_cfarCadBwrap(h_sumAbsPtr, h_detObjIdxTemp, numDopplerBins, cfardBc1,4,guardlen,noiselen);
			cycles = TSCL-w_startTime1;

			if ( numDetObjTemp >0 )
			{
				printf("Num detected object: %d, detected doppler bin index: %d", numDetObjTemp,h_detObjIdxTemp[0]);
				printf(" (%d)",h_sumAbsPtr[h_detObjIdxTemp[0]]);
				for ( i=1; i<numDetObjTemp;i++)
					printf(",%d (%d)", h_detObjIdxTemp[i], h_sumAbsPtr[h_detObjIdxTemp[i]]);
				printf("\n");
			}

			/* test cfarCadBwrap calculations */
			temp32 = 0;
			if (refDopBin[j] < (noiselen+guardlen))  /* peak is near the beginning edge */
			{
				for ( i = 1; i <= noiselen; i++)
					temp32 += h_sumAbsPtr[ refDopBin[j] +4+i ];

				for ( i = numDopplerBins-((noiselen+guardlen)-refDopBin[j]); i < numDopplerBins-(guardlen-refDopBin[j]); i++)
					temp32 += h_sumAbsPtr[ i&(numDopplerBins-1) ];
			}
			else if (refDopBin[j] >= (numDopplerBins-(noiselen+guardlen))) /* peak is near the ending edge */
			{
				for ( i = 1; i <= noiselen; i++)
					temp32 += h_sumAbsPtr[ refDopBin[j] -4-i ];
				for ( i = 1; i <= noiselen; i++)
					temp32 += h_sumAbsPtr[ (refDopBin[j] +4+i)&(numDopplerBins-1) ];
			}
			else
			{
				for ( i = 1; i <= noiselen; i++)
				{	temp32 += h_sumAbsPtr[ refDopBin[j] -4-i ];
					temp32 += h_sumAbsPtr[ refDopBin[j] +4+i ];
				}
			}
			temp32 = temp32 >>4; /* avg noise */
			gap = h_sumAbsPtr[refDopBin[j]]-temp32;
			printf("peak= %d, avg noise= %d, gap= %d, threshold %d.\n",
					h_sumAbsPtr[refDopBin[j]], temp32, gap, cfardBc1 );
			/* Test CfarCadBwrap on missed detection: */
	    	numDetObjTemp = mmwavelib_cfarCadBwrap(h_sumAbsPtr, h_detObjIdxTemp, numDopplerBins, gap-1,4,guardlen,noiselen);
	    	if ( numDetObjTemp >0 && h_detObjIdxTemp[0] == refDopBin[j] )
	    		printf("PASS: no missed detection when threshold=%d.\n", gap-1);
	    	else
	    	{
	    		testFailed++;
	    		printf("FAILED with missed detection when threshold=%d.\n", gap-1);
	    	}

			/* Test CfarCadBwrap on false detection: */
	    	numDetObjTemp = mmwavelib_cfarCadBwrap(h_sumAbsPtr, h_detObjIdxTemp, numDopplerBins ,gap,4,guardlen,noiselen);
	    	if ( numDetObjTemp ==0 )
	    		printf("PASS: no false detection when threshold=%d.\n", gap);
	    	else
	    	{
	    		testFailed++;
	    		printf("FAILED with false detection when threshold=%d.\n", gap);
	    	}

		} /* if doppler peak is near the edge */
	} /* loop refNumObject. test on cfarCadBwrap */
	printf("\ncfarCadBwrap cycles for length %d: %d\n", numDopplerBins, cycles);

	printf("\ntestFailed = %d.\n", testFailed);
	totalFailed += testFailed;

} /* test_accumLog2abs_cfar */


void test_anglefft16pt(uint32_t refNumObj, int32_t numAnt)
{
	int32_t i, diff;
	int32_t *tmp32ptr;

	printf("\n --- Test %d-Object Angle FFT:\n", refNumObj);

	memset(anglefftInput, 0, sizeof(anglefftInput));

	cnt=fread (anglefftInput, sizeof(int32_t), numAnt*2*refNumObj, fid);/* + NUM_ANGLE_BINS*2*3 */
	printf("file read count for anglefft input =%d\n", cnt);

	tmp32ptr = &tmp32buf[NUM_ANGLE_BINS*2*refNumObj]; /* set scratch buffer to be after the ref output */
	t1 = TSCL;
	mmwavelib_fft16ptZeroPadded32x32( anglefftInput, xw_Twiddle3D,tmp32ptr, anglefftOut, refNumObj, refNumObj);
	cycles = TSCL-t1;
	printf("\nAnglefft16pt cycles =%d\n", cycles);

	cnt=fread (tmp32buf, sizeof(int32_t), NUM_ANGLE_BINS*2*refNumObj, fid);
	printf("file read count for anglefft out =%d\n", cnt);
	testFailed = 0;

	for ( i=0; i<NUM_ANGLE_BINS*2*refNumObj; i++)
	{
		diff = anglefftOut[i] - tmp32buf[i];
		if ( diff >6 || diff <-6 )
		{
			printf("Failed: at i=%d, angle FFT out =%d, ref=%d, diff=%d\n",i,anglefftOut[i],tmp32buf[i],diff);
			testFailed++;
		}
	}

	if ( testFailed == 0 )
		printf("16-point Angle FFT passed.\n");
	else
		totalFailed += testFailed;

} /* test_anglefft16pt */


void benchmark_vecsum()
{
	int16_t * inptr;
	int32_t   len, i;
	int32_t   sumIQ[2];
	uint32_t  avgIQ;

	inptr = (int16_t*)&h_sumAbs[0];
	mmwavelib_vecsum(inptr, sumIQ, 512);

	printf("\nlength and vecsum cycles:\n");
	for ( len=32, i=5; len<=512; len=len*2, i++)
	{
		w_startTime1 = TSCL;
		mmwavelib_vecsum(inptr, sumIQ, len);
		cycles = TSCL-w_startTime1;
		printf("%d\t%d\n", len, cycles);

		avgIQ = (sumIQ[0] >>i)&0xFFFF + (sumIQ[1]>>i)<<16;
		//w_startTime1 = TSCL;
		//mmwavelib_vecsubc(inptr, avgIQ, len);
		//printf("%d %d %d\n", len, cycles, TSCL-w_startTime1);
	}

	printf("\nlength and vecsubc cycles:\n");
	for ( len=32; len<=512; len=len*2)
	{
		w_startTime1 = TSCL;
		mmwavelib_vecsubc(inptr, inptr, avgIQ, len);
		cycles = TSCL-w_startTime1;
		printf("%d\t%d\n", len, cycles);
	}

}


void benchmark_log2abs32()
{
	uint16_t * outptr;
	int32_t    i;
	/*
	volatile int32_t numDetObjTemp; //use volatile to prevent compiler warning
	*/
	uint16_t   h_detObjIdxTemp[32];
	/*
	uint16_t * refptr;
	uint16_t   diff;
	int32_t    len;
	uint16_t   shift;
	float      ftemp, fdiff;
	*/

	outptr = &h_sumAbs[1024];

	w_startTime1 = TSCL;
	mmwavelib_log2Abs32(tmp32buf,outptr,1024);
	cycles = TSCL-w_startTime1;
	/* printf("\n1024 point log2abs cycles =%d\n", cycles); */

	printf("\nlength and log2abs cycles:\n");
	for ( i=32; i<=1024; i=i*2 )
	{
		w_startTime1 = TSCL;
		mmwavelib_log2Abs32(tmp32buf,outptr,i);
		cycles = TSCL-w_startTime1;
		w_startTime1 = TSCL;
		/*
		numDetObjTemp =
		*/
		mmwavelib_cfarCadB(outptr, h_detObjIdxTemp,i, 1200,4,4,8);
		printf("%d\t%d\n", i, cycles);/* "%d %d %d \n", i, cycles,TSCL-w_startTime1 */
	}

	printf("length and cfarCadB cycles:\n");
	for ( i=32; i<=1024; i=i*2 )
	{
		w_startTime1 = TSCL;
		/*
		numDetObjTemp =
		*/
		mmwavelib_cfarCadB(outptr, h_detObjIdxTemp,i, 1200,4,4,8);
		cycles = TSCL-w_startTime1;
		printf("%d\t%d\n", i, cycles);
	}

	printf("length and cfarCadBwrap cycles:\n");
	for ( i=32; i<=1024; i=i*2 )
	{
		w_startTime1 = TSCL;
		mmwavelib_cfarCadBwrap(outptr, h_detObjIdxTemp,i, 1200,4,4,8);
		cycles = TSCL-w_startTime1;
		printf("%d\t%d\n", i, cycles);
	}

	printf("length and cfarCadB_SOGO (type CFAR_CA) cycles:\n");
	for ( i=32; i<=1024; i=i*2 )
	{
		w_startTime1 = TSCL;
		mmwavelib_cfarCadB_SOGO(outptr, h_detObjIdxTemp,i, CFAR_CA, 1200,4,4,8);
		cycles = TSCL-w_startTime1;
		printf("%d\t%d\n", i, cycles);
	}

	printf("length and cfarCadB_SOGO (type CFAR_CAGO) cycles:\n");
	for ( i=32; i<=1024; i=i*2 )
	{
		w_startTime1 = TSCL;
		mmwavelib_cfarCadB_SOGO(outptr, h_detObjIdxTemp,i, CFAR_CAGO, 1200,4,4,8);
		cycles = TSCL-w_startTime1;
		printf("%d\t%d\n", i, cycles);
	}

	printf("length and cfarCadB_SOGO (type CFAR_CASO) cycles:\n");
	for ( i=32; i<=1024; i=i*2 )
	{
		w_startTime1 = TSCL;
		mmwavelib_cfarCadB_SOGO(outptr, h_detObjIdxTemp,i, CFAR_CASO, 1200,4,4,8);
		cycles = TSCL-w_startTime1;
		printf("%d\t%d\n", i, cycles);
	}

} /* benchmark log2abs32 */


void read_windowfactors(int32_t numRangeBins, int32_t numDopBins )
{
	cnt = fread(win1Dbuf, sizeof(int16_t), numRangeBins/2, fid);
	printf("file read count for 1D window =%d\n", cnt);

	cnt = fread(win2Dbuf, sizeof(int32_t), numDopBins/2, fid);
	printf("file read count for 2D window =%d\n", cnt);
}


uint16_t refobject[11];

void read_refobj(void)
{
	int32_t i;

	cnt = fread(refobject, sizeof(uint16_t), 1, fid);
	printf("ref number of objects: %d\n", refobject[0]);
	cnt = fread(&refobject[1], sizeof(uint16_t), 2*refobject[0], fid);
	printf("ref doppler&range indices" );
	for ( i=1; i<=refobject[0]*2;i++ )
	{
		printf(", %d", refobject[i] );
	}
	printf(".\n");

	fclose(fid);
}

void test_libs( int32_t numRangeBins, int32_t numDopplerBins, int32_t numAnt, uint16_t numObjRef, uint32_t cfardBc1, const char* tvname)
{
	int32_t antIdx;

	prevFailed =totalFailed;
	memset(h_sumAbs, 0, sizeof(h_sumAbs));

	read_windowfactors(numRangeBins, numDopplerBins);

	for ( antIdx = 0; antIdx < numAnt; antIdx++ )
	{
		printf ("\n----- Antenna index %d:", antIdx);
		test_window1d_rangefft(numRangeBins, numDopplerBins);
		test_window2d_dopplerfft_log2abs(numRangeBins, numDopplerBins, cfardBc1);
	}

	test_accumLog2abs_cfar(numRangeBins, numDopplerBins, numObjRef, &refobject[1+numObjRef], &refobject[1], cfardBc1*numAnt);
	test_anglefft16pt(numObjRef, numAnt);

	test_histogram_shift(numRangeBins, numDopplerBins);

	fclose(fid);
	printf("\nThis test case's fail count = %d. Total failed = %d\n", totalFailed-prevFailed, totalFailed);

    if ( totalFailed > prevFailed)
	{
		MCPI_setFeatureTestResult(tvname, MCPI_TestResult_FAIL);
	}
    else
	{
        MCPI_setFeatureTestResult(tvname, MCPI_TestResult_PASS);
	}

} /* test_libs */


void main(void)
{
    int32_t  numAnt;
    uint32_t cfardB_c1=768;

    TSCL = 0;
    t1 = TSCL; /* ranClock(); */
    t2 = TSCL; /* ranClock(); */
    tsc_overhead = t2 - t1;
    printf("tsc_overhead = %d cycles\n", tsc_overhead);
/*#if 1
	//setting up L1 data and L1 program as cache
	*((uint32_t*)0x01840020)=7;
	*((uint32_t*)0x01840040)=7;
#else
	*((uint32_t*)0x01840020)=0;
	*((uint32_t*)0x01840040)=0;
#endif
*/

	MCPI_Initialize();

	benchmark_windowing();

	benchmark_vecsum();

	/* log2abs32 benchmark */
	benchmark_log2abs32();

	/* test log2abs 32 and 16 functionality */
	printf("\nTest log2abs 16, 32 functionality :\n");
	test_log2abs32();
	test_log2abs16();
		
	test_hist_ones_allbins();
	test_shift32right();

	/* Generate twiddle factors for 256-pt 1d FFT */
    t1 = TSCL;
	gen_twiddle_fft16x16((int16_t*)xh_16x16Twiddle1dFFT, 256);
    t2 = TSCL;
    printf("\n256-point twiddle16x16 cycles=%d\n", t2-t1);

	/* Generate twiddle factors for 32-pt 2d FFT */
    t1 = TSCL;
	gen_twiddle_fft32x32((int32_t*)xw_32x32Twiddle2dFFT, 32,2147483647.5);
    t2 = TSCL;
    printf("32-point twiddle32x32 cycles=%d\n", t2-t1);

	

#if TEST_CAPTURED_DATA
	numAnt = 4;
	printf("\n===  test vector with captured data: ===\n");

    if ((fid = fopen ("..\\testdata\\radarDemo_captured_ref.bin","rb")) == NULL)
    {
        printf ("file open error: can not open ref file\n");
        exit(3);
    }

    read_refobj();

    if ((fid = fopen ("..\\testdata\\radarDemo_captured_test.bin","rb")) == NULL)
    {
        printf ("file open error: can not open input data file\n");
        exit(3);
    }

    test_libs(256, 32, numAnt, refobject[0], 1200, "demo case 256pt 1DFFT 32pt 2DFFT captured data");

	test_windowing_types(256, 32);

	printf("\n=== TV single object office captured data done. ====\n");

#endif

    numAnt = 8;

#if TEST_MULTIOBJ_SNR0
	printf("\n===  test vector with multiple objects: ===\n");

    if ((fid = fopen ("..\\testdata\\radarDemo_multiObj_ref.bin","rb")) == NULL)
    {
        printf ("file open error: can not open ref file\n");
        exit(3);
    }

    read_refobj();

    if ((fid = fopen ("..\\testdata\\radarDemo_multiObj_test.bin","rb")) == NULL)
    {
        printf ("file open error: can not open input data file\n");
        exit(3);
    }

    test_libs(256, 32, numAnt, refobject[0], cfardB_c1, "demo case 256pt 1DFFT 32pt 2DFFT multi-obj SNR0");

	printf("\n=== TV multiple objects SNR0 done. ====\n");

#endif

  if ( moretv )
  {
#if TEST_MULTIOBJ_M5
	printf("\n===  test vector with multiple objects SNR (-5) : ===\n");

    if ((fid = fopen ("..\\testdata\\radarDemo_multiObj_neg5_ref.bin","rb")) == NULL)
    {
        printf ("file open error: can not open ref file\n");
        exit(3);
    }

    read_refobj();

    if ((fid = fopen ("..\\testdata\\radarDemo_multiObj_neg5_test.bin","rb")) == NULL)
    {
        printf ("file open error: can not open input data file\n");
        exit(3);
    }

    test_libs(256, 32, numAnt, refobject[0], cfardB_c1, "demo case multi-obj SNR-5");

	printf("\n=== TV multiple objects SNR(-5) done. ====\n");

#endif

#if TEST_MULTIOBJ_M15
	printf("\n===  test vector with multiple objects SNR (-15) : ===\n");

    if ((fid = fopen ("..\\testdata\\radarDemo_multiObj_neg15_ref.bin","rb")) == NULL)
    {
        printf ("file open error: can not open ref file\n");
        exit(3);
    }

    read_refobj();

    if ((fid = fopen ("..\\testdata\\radarDemo_multiObj_neg15_test.bin","rb")) == NULL)
    {
        printf ("file open error: can not open input data file\n");
        exit(3);
    }

    test_libs(256, 32, numAnt, refobject[0], 500, "demo case multi-obj SNR-15");

	printf("\n=== TV multiple objects SNR(-15) done. ====\n");

#endif

#if TEST_SINGLEOBJ_SNR0
	printf("\n===  test vector with SNR 0: ===\n");

    if ((fid = fopen ("..\\testdata\\radarDemo_ref.bin","rb")) == NULL)
    {
        printf ("file open error: can not open ref file\n");
        exit(3);
    }

    read_refobj();

    if ((fid = fopen ("..\\testdata\\radarDemo_test.bin","rb")) == NULL) //_log2absout_, _flatWin2D_fft2Dout_test.bin
    {
        printf ("file open error: can not open input data file\n");
        exit(3);
    }

    test_libs(256, 32, numAnt, refobject[0], cfardB_c1, "demo case single obj SNR0");

	printf("\n=== TV single object SNR0 done. ====\n");

#endif

#if TEST_SINGLEOBJ_M10
	printf("\n===  test vector with different SNR (-10) : ====\n");
    if ((fid = fopen ("..\\testdata\\radarDemo_neg10_ref.bin","rb")) == NULL)
    {
        printf ("file open error: can not open ref file\n");
        exit(3);
    }

    read_refobj();

    if ((fid = fopen ("..\\testdata\\radarDemo_neg10_test.bin","rb")) == NULL)
    {
        printf ("file open error: can not open input data file\n");
        exit(3);
    }

    test_libs(256, 32, numAnt, refobject[0], cfardB_c1, "demo case single obj SNR-10");

	printf("\n=== TV single object SNR-10 done. ====\n");

#endif

#if TEST_SINGLEOBJ_M15
	printf("\n===  test vector with different SNR (-15) : ===\n");

    if ((fid = fopen ("..\\testdata\\radarDemo_neg15_ref.bin","rb")) == NULL)
    {
        printf ("file open error: can not open ref file\n");
        exit(3);
    }

    read_refobj();

    if ((fid = fopen ("..\\testdata\\radarDemo_neg15_test.bin","rb")) == NULL)
    {
        printf ("file open error: can not open test data file\n");
        exit(3);
    }

    test_libs(256, 32, numAnt, refobject[0], cfardB_c1, "demo case single obj SNR-15");

	printf("\n=== TV single object SNR-15 done. ====\n");

#endif


	/* Generate twiddle factors for 1d FFT */
    t1 = TSCL;
	gen_twiddle_fft16x16((int16_t*)xh_16x16Twiddle1dFFT, 1024);
    t2 = TSCL;
    printf("1024-point twiddle16x16 cycles=%d\n", t2-t1);


#if TEST_CASE1
	printf("\n===  test vector use case1 with 1DFFT size 1024: ===\n");

    if ((fid = fopen ("..\\testdata\\radarCase1_ref.bin","rb")) == NULL)
    {
        printf ("file open error: can not open ref file\n");
        exit(3);
    }

    read_refobj();

    if ((fid = fopen ("..\\testdata\\radarCase1_test.bin","rb")) == NULL)
    {
        printf ("file open error: can not open input data file\n");
        exit(3);
    }

    test_libs(1024, 32, numAnt, refobject[0], 1200, "use case1 1024pt 1DFFT");

	test_windowing_types(1024, 32);

	printf("\n=== TV use case1 single object 1024-point 1DFFT done. ====\n");
#endif

#if TEST_CASE1_MULTI
	printf("\n===  test vector use case1 multi-objects: ===\n");

    if ((fid = fopen ("..\\testdata\\radarCase1_mulObj_ref.bin","rb")) == NULL)
    {
        printf ("file open error: can not open ref file\n");
        exit(3);
    }

    read_refobj();

    if ((fid = fopen ("..\\testdata\\radarCase1_mulObj_test.bin","rb")) == NULL)
    {
        printf ("file open error: can not open input data file\n");
        exit(3);
    }

    test_libs(1024, 32, numAnt, refobject[0], 1200, "use case1 1024pt 1DFFT multi-obj");

	printf("\n=== TV use case1 multi objects done. ====\n");
#endif

#if TEST_CASE1_512
    t1 = TSCL;
	gen_twiddle_fft16x16((int16_t*)xh_16x16Twiddle1dFFT, 512);
    t2 = TSCL;
    printf("512-point twiddle16x16 cycles=%d\n", t2-t1);

	printf("\n===  test vector use case1 with 1DFFT size 512: ===\n");

    if ((fid = fopen ("..\\testdata\\radarCase1_512_ref.bin","rb")) == NULL)
    {
        printf ("file open error: can not open ref file\n");
        exit(3);
    }

    read_refobj();

    if ((fid = fopen ("..\\testdata\\radarCase1_512_test.bin","rb")) == NULL)
    {
        printf ("file open error: can not open input data file\n");
        exit(3);
    }

    test_libs(512, 32, numAnt, refobject[0], cfardB_c1, "use case1 512pt 1DFFT");

	printf("\n=== TV use case1 with 512-point 1DFFT done. ====\n");

#endif

    t1 = TSCL;
	gen_twiddle_fft16x16((int16_t*)xh_16x16Twiddle1dFFT, 256);
    t2 = TSCL;
    printf("256-point twiddle16x16 cycles=%d\n", t2-t1);

    t1 = TSCL;
	gen_twiddle_fft32x32((int32_t*)xw_32x32Twiddle2dFFT, 128,2147483647.5);
    t2 = TSCL;
    printf("128-point twiddle32x32 cycles=%d\n", t2-t1);

#if TEST_CASE2
	printf("\n===  test vector use case2: ===\n");

    if ((fid = fopen ("..\\testdata\\radarCase2_ref.bin","rb")) == NULL)
    {
        printf ("file open error: can not open ref file\n");
        exit(3);
    }

    read_refobj();
#if COMPARE_2DFFT
    if ((fid = fopen ("..\\testdata\\radarCase2_2dfftout_test.bin","rb")) == NULL)
#else
    if ((fid = fopen ("..\\testdata\\radarCase2_test.bin","rb")) == NULL)
#endif
    {
        printf ("file open error: can not open input data file\n");
        exit(3);
    }

    benchmark_64pt_2dprocess(refobject[2]);

    test_libs(256, 128, numAnt, refobject[0], 1000, "use case2 256pt 1DFFT 128pt 2DFFT");

    /* 
	benchmark_64pt_2dprocess(refobject[2]); 
	*/
	test_windowing_types(256, 128);

	printf("\n=== TV use case2 done. ====\n");
#endif

#if TEST_CASE2_MULTI
	printf("\n===  test vector use case2 multi objects: ===\n");

    if ((fid = fopen ("..\\testdata\\radarCase2_mulObj_ref.bin","rb")) == NULL)
    {
        printf ("file open error: can not open ref file\n");
        exit(3);
    }

    read_refobj();

#if COMPARE_2DFFT
    if ((fid = fopen ("..\\testdata\\radarCase2_mulObj_2dfftout_test.bin","rb")) == NULL)
#else
    if ((fid = fopen ("..\\testdata\\radarCase2_mulObj_test.bin","rb")) == NULL)
#endif
    {
        printf ("file open error: can not open input data file\n");
        exit(3);
    }

    test_libs(256, 128, numAnt, refobject[0],cfardB_c1, "use case2 multi-obj");

	printf("\n=== TV use case2 multiple objects done. ====\n");

#endif /* TEST_CASE2_MULTI */

  }/* if (moretv) */

	if ( totalFailed == 0 )
		printf("\n==========  All tests passed! ==========\n");

	printf("\n==========  Testing completed! ==========\n");

        MCPI_setTestResult ();
}


