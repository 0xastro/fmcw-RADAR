/*
 *   @file  lib_window_test.c
 *
 *   @brief
 *      unit Test code for windowing mmwavelib library functions
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


#include <c6x.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ti/alg/mmwavelib/mmwavelib.h>
#include <ti/utils/testlogger/logger.h>

#define MAX_NUM_RANGE_BINS 	1024
extern volatile uint32_t w_startTime1;
extern uint32_t  cycles;

extern int16_t inputbuf[];
extern int16_t win1Dbuf[];
extern int16_t refbuf[];
extern int16_t inputbuf2D[];
extern int32_t win2Dbuf[];
extern int32_t fft2Dinput[];
extern int32_t tmp32buf[];
extern uint16_t h_sumAbs[];

extern int32_t totalFailed;
extern int32_t prevFailed ;

extern volatile uint32_t w_startTime1;
extern uint32_t cycles;

#define RECTANGLE 0
#define HAMMING 1
#define HANNING 2
#define BLACKMAN 3
#define COSINE  4
#define EXPONENTIAL 5
#define KAISER 6

#define Q15FORMAT 0
#define Q19FORMAT 1

#define PI 3.1415926535897
#define ONE_Q15 (1 << 15)
#define ONE_Q19 (1 << 19)

void gen_window_coeffs(uint16_t winsize, uint8_t winType, uint8_t format,  void *coeff);


void benchmark_windowing( void )
{
	int32_t i,j,len;

	printf("\nBenchmark:\nlength and windowing16x16 cycles:\n");

	for (i=128; i<=MAX_NUM_RANGE_BINS; i=i*2)
	{
		mmwavelib_windowing16x16(inputbuf, win1Dbuf, i);
		w_startTime1 = TSCL;
		mmwavelib_windowing16x16(inputbuf, win1Dbuf, i);
		cycles = TSCL-w_startTime1;
		printf("%d\t%d\n", i, cycles);
	}

	printf("\nlength and windowing16x16_evenlen cycles:\n");
	//mmwavelib_windowing16x16_evenlen(inputbuf, win1Dbuf, MAX_NUM_RANGE_BINS);
	for (i=128; i<=MAX_NUM_RANGE_BINS; i=i*2)
	{
		w_startTime1 = TSCL;
		mmwavelib_windowing16x16_evenlen(inputbuf, win1Dbuf, i);
		cycles = TSCL-w_startTime1;
		printf("%d\t%d\n", i, cycles);
	}
	
	for (j=0; j<=14; j+=2)
	{
		printf("len mod16 = %d:\n",j );
		for (i=128; i<=MAX_NUM_RANGE_BINS; i=i*2)
		{
			len = i-16+j;
			w_startTime1 = TSCL;
			mmwavelib_windowing16x16_evenlen(inputbuf, win1Dbuf, len);
			cycles = TSCL-w_startTime1;
			printf("%d\t%d\n", len, cycles);
		}
	}


	printf("\nlength and windowing16x32 cycles:\n");
    mmwavelib_windowing16x32(inputbuf2D, win2Dbuf, fft2Dinput, 256);
	for (i=16; i<=256; i=i*2)
	{
		w_startTime1 = TSCL;
		mmwavelib_windowing16x32(inputbuf2D, win2Dbuf, fft2Dinput, i);
		cycles = TSCL-w_startTime1;
		printf("%d\t%d\n", i, cycles);
	}

    printf("\nlength and windowing16x32_IQswap cycles:\n");
    for (i=16; i<=256; i=i*2)
	{
		w_startTime1 = TSCL;
		mmwavelib_windowing16x32_IQswap(inputbuf2D, win2Dbuf, fft2Dinput, i);
		cycles = TSCL-w_startTime1;
		printf("%d\t%d\n", i, cycles);
	}
    
}

void test_windowing_types( int32_t numRangeBins, int32_t numDopBins )
{
	int32_t i, wintype, winIdx;
	int16_t *refptr = &refbuf[0];
	int16_t *copyinp = (int16_t*)&h_sumAbs;
	uint32_t evenlen, len;

	prevFailed = totalFailed;

	printf("\n --- Test windowing types :\n");

	/* save a copy of input data since inputbuf will be overwritten */
	memcpy( (void*)copyinp, (void*)(&inputbuf[0]), numRangeBins*2*2);

	for ( wintype= 0; wintype <= 6; wintype++ )
	{
		gen_window_coeffs(numRangeBins, wintype, Q15FORMAT, win1Dbuf);

		//w_startTime1 = TSCL;
		for ( i=0; i < numRangeBins/2; i++)
		{
			refptr[2*i] = ((int32_t)inputbuf[2*i]*win1Dbuf[i] + 0x4000) >>15;
			refptr[2*i+1] = ((int32_t)inputbuf[2*i+1]*win1Dbuf[i] + 0x4000) >>15;
			refptr[2*(numRangeBins-1-i)] = ((int32_t)inputbuf[2*(numRangeBins-1-i)] * win1Dbuf[i] + 0x4000) >>15;
			refptr[2*(numRangeBins-1-i)+1] = ((int32_t)inputbuf[2*(numRangeBins-1-i)+1] * win1Dbuf[i] + 0x4000) >>15;
		}
		//cycles = TSCL-w_startTime1;
		//printf("%d %d\n", numRangeBins, cycles);
		//w_startTime1 = TSCL;
		mmwavelib_windowing16x16(inputbuf, win1Dbuf,numRangeBins);
		//cycles = TSCL-w_startTime1;
		//printf("%d %d\n", numRangeBins, cycles);

		for ( i=0; i < numRangeBins*2; i++)
		{
			if ( (inputbuf[i] - refptr[i]) !=0  )
			{
				totalFailed++;
				printf("\nwindowing16x16 test failed on type%d window!\n", wintype);
				break;
			}
		}
		memcpy( (void*)(&inputbuf[0]), (void*)copyinp, numRangeBins*2*2);

		w_startTime1 = TSCL;
		mmwavelib_windowing16x16_evenlen(inputbuf, win1Dbuf,numRangeBins);
		cycles = TSCL-w_startTime1;

		for ( i=0; i < numRangeBins*2; i++)
		{
			if ( (inputbuf[i] - refptr[i]) !=0  )
			{
				totalFailed++;
				printf("\nwindowing16x16_evenlen test failed on type%d window!\n", wintype);
				break;
			}
		}
		memcpy( (void*)(&inputbuf[0]), (void*)copyinp, numRangeBins*2*2);
	}
	printf("windowing16x16_evenlen length and cycles:\n%d %d\n", numRangeBins, cycles);

	gen_window_coeffs(numRangeBins, HAMMING, Q15FORMAT, win1Dbuf);

	for (evenlen = numRangeBins-16; evenlen<=numRangeBins; evenlen+=2)
	{
		memcpy( (void*)(&inputbuf[0]), (void*)copyinp, numRangeBins*2*2);
		for ( i=0; i<evenlen/2; i++)
		{
			refptr[2*i] = ((int32_t)inputbuf[2*i]*win1Dbuf[i] + 0x4000) >>15;
			refptr[2*i+1] = ((int32_t)inputbuf[2*i+1]*win1Dbuf[i] + 0x4000) >>15;
			refptr[2*(evenlen-1-i)] = ((int32_t)inputbuf[2*(evenlen-1-i)] * win1Dbuf[i] + 0x4000) >>15;
			refptr[2*(evenlen-1-i)+1] = ((int32_t)inputbuf[2*(evenlen-1-i)+1] * win1Dbuf[i] + 0x4000) >>15;
		}
		w_startTime1 = TSCL;
		mmwavelib_windowing16x16_evenlen(inputbuf, win1Dbuf,evenlen);
		cycles = TSCL-w_startTime1;
		printf("%d %d\n", evenlen, cycles);

		for ( i=0; i < evenlen; i++)
		{
			if ( !((inputbuf[2*i]==refptr[2*i]) && (inputbuf[2*i+1]==refptr[2*i+1])) )
			{
				totalFailed++;
				printf("\nevenlen=%d test failed.idx%d: I=%d refI=%d, Q=%d refQ=%d\n",
						evenlen, i, inputbuf[2*i], refptr[2*i], inputbuf[2*i+1], refptr[2*i+1] );
				winIdx=i;
				if (i>=evenlen/2)
					winIdx = evenlen-1-i;
				printf("win=%d, inpI=%d, inpQ=%d\n", win1Dbuf[winIdx], copyinp[2*i], copyinp[2*i+1] );
				break;
			}
		}
	}


	/* 2D windowing types test: */
	len = numDopBins;
	for ( wintype= 0; wintype <= 6; wintype++ )
	{
		gen_window_coeffs(len, wintype, Q19FORMAT, win2Dbuf);
		w_startTime1 = TSCL;
		mmwavelib_windowing16x32(inputbuf2D, win2Dbuf, fft2Dinput, len);
		cycles = TSCL-w_startTime1;
	    for ( i=0; i < len/2; i++)
	    {
		tmp32buf[2*i] = ( (int64_t)win2Dbuf[i]*inputbuf2D[2*i] + 0x4000 ) >>15;
		tmp32buf[2*i+1] = ( (int64_t)win2Dbuf[i]*inputbuf2D[2*i+1] + 0x4000 ) >>15;
		tmp32buf[2*(len-i)-2] = ( (int64_t)win2Dbuf[i]* inputbuf2D[2*(len-i)-2] + 0x4000 ) >>15;
		tmp32buf[2*(len-i)-1] = ( (int64_t)win2Dbuf[i]* inputbuf2D[2*(len-i)-1] + 0x4000 ) >>15;
	    }

	    for ( i=0; i <len*2; i++)
	    {
		if ( (fft2Dinput[i] - tmp32buf[i]) !=0  )
		{
			totalFailed++;
			printf("\n%d-pt windowing16x32 test failed on type%d window!\n", len, wintype);
			break;
		}
		}
	}
	printf("windowing16x32 length and cycles:\n%d %d\n", len, cycles);

	len = numDopBins -4; /* multiple of 4, not multiple of 8 */
	gen_window_coeffs(len, HAMMING, Q19FORMAT, win2Dbuf);
	w_startTime1 = TSCL;
	mmwavelib_windowing16x32(inputbuf2D, win2Dbuf, fft2Dinput, len);
	printf("%d %d\n", len, cycles);
	for ( i=0; i < len/2; i++)
	{
		tmp32buf[2*i] = ( (int64_t)win2Dbuf[i]*inputbuf2D[2*i] + 0x4000 ) >>15;
		tmp32buf[2*i+1] = ( (int64_t)win2Dbuf[i]*inputbuf2D[2*i+1] + 0x4000 ) >>15;
		tmp32buf[2*(len-i)-2] = ( (int64_t)win2Dbuf[i]* inputbuf2D[2*(len-i)-2] + 0x4000 ) >>15;
		tmp32buf[2*(len-i)-1] = ( (int64_t)win2Dbuf[i]* inputbuf2D[2*(len-i)-1] + 0x4000 ) >>15;
	}
	for ( i=0; i <len*2; i++)
	{
		if ( (fft2Dinput[i] - tmp32buf[i]) !=0  )
		{
			totalFailed++;
			printf("\n%d-pt windowing16x32 test failed. %d-th sample: %d, ref=%d!\n",
				     len, i/2, fft2Dinput[i],tmp32buf[i]);
			break;
		}
	}

    if ( totalFailed > prevFailed)
        MCPI_setFeatureTestResult("windowing various types test", MCPI_TestResult_FAIL);
    else
    {
        MCPI_setFeatureTestResult("windowing various types test", MCPI_TestResult_PASS);
    }

 
} /*test_windowing_types*/


/*------------------------------------------------------------*/
/* Bessel function of first kind and order  */
/*          0 at input x                                      */
/*------------------------------------------------------------*/

static double bessj0( double x )
{

	double fac[6]={1,4,36,576, 14400, 518400};
	uint8_t ii;
	double xx,y,ans,ans1;

	ans = 1;
	xx = x * x;
	y = xx/4.f;

	ans1 = 1;
	for (ii=0;ii<6;ii++)
	{
		//ans1 = pow(y,ii+1);
		ans1 *= y;
		ans += (ans1/fac[ii]);
	}

   return ans;
}


void gen_window_coeffs(uint16_t winsize, uint8_t winType, uint8_t format,  void *coeff)
{

	uint16_t i;
	float temp;
	int16_t itemp16;
	int32_t itemp32;
	int32_t oneformat;
	int16_t * coeffQ15 = (int16_t *) coeff;
	int32_t * coeffQ19 = (int32_t *) coeff;
	double tempd;
	double tempd1;

	if (format==Q15FORMAT)
		oneformat = ONE_Q15;
	else if (format==Q19FORMAT)
		oneformat = ONE_Q19;

	if (winType == KAISER)
		tempd = bessj0(3*PI);
	for (i = 0; i < winsize/2; i++)
	{

		if (winType == HAMMING)
		{
			/* wn = 0.54-0.46*cos(2*pi*n/(N-1))*/
			temp = oneformat * (0.54-0.46 * cos(2*PI*i/(winsize-1)));
		}
		else if (winType==HANNING)
		{
			/*wn = 0.5*[1-cos(2*pi*i/(N-1));*/
			temp = oneformat * (0.5*(1-cos(2*PI*i/(winsize-1))));
		}
		else if (winType==BLACKMAN)
		{
			/* wn = 0.42659-0.49656*cos(2*pi*n/(N-1))+0.076849*cos(4*pi*n/(N-1))*/
			temp = oneformat * (0.42659-0.49656*cos(2*PI*i/(winsize-1))+0.076849*cos(4*PI*i/(winsize-1)));
		}
		else if (winType==COSINE)
		{
			temp = oneformat*cos(PI*i/(winsize-1)-PI/2);
		}
		else if (winType==RECTANGLE)
		{
			temp = oneformat-1;
		}
		else if (winType == EXPONENTIAL)
		{
			temp = oneformat * exp(-(winsize/2-i)*2/winsize);
		}
		else if (winType == KAISER)
		{
			tempd1 = 1-(double)2*i/(winsize-1); //2n/(N-1)-1
			tempd1 = tempd1*tempd1; //^2
			temp = oneformat * (bessj0(3*PI*sqrt(1-tempd1))/tempd);
		}


		if (temp>0)
		{
			temp = temp + 0.5;
		}
		else
		{
			temp = temp - 0.5;
		}

		if (format==Q15FORMAT)
		{
			itemp16 = (int16_t) temp;

			if (itemp16>=ONE_Q15)
				itemp16 = ONE_Q15 - 1;

			coeffQ15[i] = itemp16;

		}
		else if (format ==Q19FORMAT )
		{
			itemp32 = (int32_t) temp;

			if (itemp32 >= ONE_Q19)
				itemp32 = ONE_Q19 - 1;

			coeffQ19[i] = itemp32;

		}
	}

}/*gen_window_coeffs*/
