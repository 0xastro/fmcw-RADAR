/*
 *   @file  lib_abslog2_test.c
 *
 *   @brief
 *      Unit Test code for log2abs mmwavelib library functions
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
#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <ti/alg/mmwavelib/mmwavelib.h>
#include <ti/utils/testlogger/logger.h>

#define DEBUG 0
#define THRESHOLDABS32 0.097
#define THRESHOLDABS16 0.024

/*****************************************************************************
* test_log2abs32 tests the mmwavelib_log2Abs32, which calculate the 256*log2(abs)
*
*  The test is done by set the angle of the complex value to 10, 20, and 30 degree to
*  cover the maximum error. and the absolute value of complex value changes from 2^5
*  2^31.
*
*  the lib output is compared with float point log function
 ******************************************************************************/

void test_log2abs32()
{
	int32_t   inptr[32*2];
	uint16_t  outptr[32];
	int32_t    i,j,k,m;
	float      ftemp,ftempSin, ftempCos;
	double angle;
	double pi = 3.1415926535897f;
	double refout[32];
	double refdiff;
	uint8_t testPASS=1;
#if DEBUG
	FILE *fptr;

	fptr=fopen("data.dat","w");

#endif


	/* add a corner case, abs(0) */
	inptr[0] = 0;
	inptr[1] = 0;
	mmwavelib_log2Abs32(&inptr[0], &outptr[0], 1);
	if (outptr[0]!=0)
	{
		testPASS = 0;
		printf("mmwavelib_log2Abs32: log2abs(0) = %d\n", outptr[0]);
	}

	angle = 10/180.f * pi;

	for (i =5; i < 32; i++)
	{
		k = (1<<i) >> 5; /* number of loops, each loop run 32 points  */
		if (k>512)
		{
			k=512;
		}
		for (j=0; j < k; j++ ) /* maximum loop is 512 */
		{
			for (m = 0; m < 32; m++)
			{
				if (i<15)
				{
					ftemp = ( (1<<i)+ (j*32+m)) ; /* step is 1 */
				}
				else
				{
					ftemp = pow(2,i+((j*32+m)/512.f/32.f)); /* step is 2^i, 2^(i+m/total) */
				}
				ftempCos = ftemp*cos(angle);

				if (ftempCos>0)
				{
					inptr[2*m] = (int32_t) (ftempCos+0.5);
				}
				else
				{
					inptr[2*m] = (int32_t) (ftempCos-0.5);
				}

				ftempSin = ftemp *sin(angle);
				if (ftempSin>0)
				{
					inptr[2*m+1] = (int32_t) (ftempSin+0.5);
				}
				else
				{
					inptr[2*m+1] = (int32_t) (ftempSin-0.5);
				}


				refout[m] = log10((float)inptr[2*m]*inptr[2*m]+(float)inptr[2*m+1]*inptr[2*m+1]) * 3.3219 /2.f;
			}

			mmwavelib_log2Abs32(&inptr[0], &outptr[0], 32);

			/*check the results */
			for (m = 0; m < 32; m++)
			{
				refdiff = refout[m]-outptr[m]/256.f;

				if ((refdiff>THRESHOLDABS32) || (refdiff<-1.f*THRESHOLDABS32))
				{
					testPASS = 0;
				}
			}
		}
	}


	angle = 20/180.f * pi;

	for (i =5; i < 32; i++)
	{
		k = (1<<i) >> 5; /* number of loops, each loop run 32 points */
		if (k>512)
		{
			k=512;
		}
		for (j=0; j < k; j++ ) /*maximum loop is 512 */
		{
			for (m = 0; m < 32; m++)
			{
				if (i<15)
				{
					ftemp = ( (1<<i)+ (j*32+m)) ; /* step is 1 */
				}
				else
				{
					ftemp = pow(2,i+((j*32+m)/512.f/32.f)); /* step 2^i, 2^(i+m/total) */
				}
				ftempCos = ftemp*cos(angle);
				if (ftempCos>0)
				{
					inptr[2*m] = (int32_t) (ftempCos+0.5);
				}
				else
				{
					inptr[2*m] = (int32_t) (ftempCos-0.5);
				}
				ftempSin = ftemp *sin(angle);
				if (ftempSin>0)
				{
					inptr[2*m+1] = (int32_t) (ftempSin+0.5);
				}
				else
				{
					inptr[2*m+1] = (int32_t) (ftempSin-0.5);
				}
#if DEBUG
				fprintf(fptr,"%d,%d\n", inptr[2*m], inptr[2*m+1]);
#endif

				refout[m] = log10((float)inptr[2*m]*inptr[2*m]+(float)inptr[2*m+1]*inptr[2*m+1]) * 3.3219 /2.f;
			}

			mmwavelib_log2Abs32(&inptr[0], &outptr[0], 32);

			/* check the results */
			for (m = 0; m < 32; m++)
			{
				refdiff = refout[m]-outptr[m]/256.f;
				if ((refdiff>THRESHOLDABS32) || (refdiff<-1.f*THRESHOLDABS32))
				{
					testPASS = 0;
				}
			}
		}
	}

#if DEBUG
	fclose(fptr);
#endif

	angle = 30/180.f * pi;
	for (i =5; i < 32; i++)
	{
		k = (1<<i) >> 5; /* number of loops, each loop run 32 points */
		if (k>512)
		{
			k=512;
		}
		for (j=0; j < k; j++ ) /* maximum loop is 512 */
		{
			for (m = 0; m < 32; m++)
			{
				if (i<15)
				{
					ftemp = ( (1<<i)+ (j*32+m)) ; /* step is 1 */
				}
				else
				{
					ftemp = pow(2,i+((j*32+m)/512.f/32.f)); /* 2^i, 2^(i+m/total) */
				}
				ftempCos = ftemp*cos(angle);
				if (ftempCos>0)
				{
					inptr[2*m] = (int32_t) (ftempCos+0.5);
				}
				else
				{
					inptr[2*m] = (int32_t) (ftempCos-0.5);
				}

				ftempSin = ftemp *sin(angle);
				if (ftempSin>0)
				{
					inptr[2*m+1] = (int32_t) (ftempSin+0.5);
				}
				else
				{
					inptr[2*m+1] = (int32_t) (ftempSin-0.5);
				}

				refout[m] = log10((float)inptr[2*m]*inptr[2*m]+(float)inptr[2*m+1]*inptr[2*m+1]) * 3.3219 /2.f;
			}

			mmwavelib_log2Abs32(&inptr[0], &outptr[0], 32);

			/* check the results */
			for (m = 0; m < 32; m++)
			{
				refdiff = refout[m]-outptr[m]/256.f;
				if ((refdiff>THRESHOLDABS32) || (refdiff<-1.f*THRESHOLDABS32))
				{
					testPASS = 0;
				}
			}
		}
	}


	if (testPASS)
	{
		MCPI_setFeatureTestResult("The log2abs32 functionality test", MCPI_TestResult_PASS);
	}
	else
	{
		MCPI_setFeatureTestResult("The log2abs32 functionality test", MCPI_TestResult_FAIL);
	}

}

/*****************************************************************************
* test_log2abs16 tests the mmwavelib_log2Abs16, which calculate the 256*log2(abs^2)
*
* inside mmwavelib_log2Abs16 function, the real^2+imag^2 of a complex value
* is approximated into 2^p(1+fractional part). A 64 point table is
* used to calculate 256*log2(1+fractional part).
*
* The test is done by set the absolute value of a complex value from 1 to 2^15. the phase
* is universally distributed over 0 to 360 degree.
*
*  the lib output is compared with float point log function
 ******************************************************************************/


void test_log2abs16()
{
	int16_t   inptr[32*2];
	uint16_t  outptr[32];
	int32_t    i,j,k,m,mm;
	float      ftemp,ftempSin, ftempCos;
	double angle;
	double pi = 3.1415926535897f;
	double refout[32];
	double refdiff;
	uint8_t testPASS=1;
#if DEBUG
	FILE *fptr;

	fptr=fopen("data_16.dat","w");

#endif

	/* add a corner case, abs(0) */
	for (i=0; i < 32*2; i++)
	{
		inptr[i] = 0;
	}
	mmwavelib_log2Abs16(&inptr[0], &outptr[0], 32);
	for (i=0; i < 32; i++)
	{
		if (outptr[i]!=0)
		{
			testPASS = 0;
			printf("mmwavelib_log2Abs16: sample index i=%d, log2abs(0) = %d\n", i, outptr[i]);
			break;
		}
	}

	for (i=0; i < 16; i++)
	{
		if (i<5)
		{
			k=1;
		}
		else
		{
			k = (1<<i) >> 5; //number of loops, each loop run 32 points
		}
		
		for (j=0; j < k; j++ ) //maximum loop is 512
		{
			for (m = 0; m < 32; m++)
			{
				ftemp = ( (1<<i)+ (j*32+m)) ; //step is 1

				angle = ( (j * (k/11.25)) + m*11.25) *pi/180;

				ftempCos = ftemp*cos(angle);

				if (ftempCos>0)
				{
					inptr[2*m] = (int32_t) (ftempCos+0.5);
				}
				else
				{
					inptr[2*m] = (int32_t) (ftempCos-0.5);
				}

				ftempSin = ftemp *sin(angle);
				if (ftempSin>0)
				{
					inptr[2*m+1] = (int32_t) (ftempSin+0.5);
				}
				else
				{
					inptr[2*m+1] = (int32_t) (ftempSin-0.5);
				}
#if DEBUG
				fprintf(fptr,"%d,%d\n", inptr[2*m], inptr[2*m+1]);
#endif

				refout[m] = log10((float)inptr[2*m]*inptr[2*m]+(float)inptr[2*m+1]*inptr[2*m+1]) * 3.3219;
			}

			mmwavelib_log2Abs16(&inptr[0], &outptr[0], 32);

			/* check the results  */
			for (mm = 0; mm < 32; mm++)
			{
				refdiff = refout[mm]-outptr[mm]/256.f;

				if ((refdiff>THRESHOLDABS16) || (refdiff<-1.f*THRESHOLDABS16))
				{
					printf("m=%d, refout=%f, out=%f\n", mm, refout[mm],outptr[mm]/256.f);
					testPASS = 0;
				}
			}
		}
	}
	
	if (testPASS)
	{
		MCPI_setFeatureTestResult("The log2abs16 functionality test", MCPI_TestResult_PASS);
	}
	else
	{
		MCPI_setFeatureTestResult("The log2abs16 functionality test", MCPI_TestResult_FAIL);
	}


#if DEBUG
	fclose(fptr);
#endif
}
