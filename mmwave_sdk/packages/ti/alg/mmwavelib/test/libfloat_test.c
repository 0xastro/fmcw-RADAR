/*
 *   @file  libfloat_test.c
 *
 *   @brief
 *      Test code for floating-point mmwavelib library functions
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
#include "gen_twiddle_fft16x16.h"
#include "DSP_fft16x16.h"
#include "DSPF_sp_fftSPxSP.h"
#include <ti/utils/testlogger/logger.h>
#include <ti/alg/mmwavelib/include/mmwavelib_cplx_types.h>


/* Standard Include Files. */
#include <c6x.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define NUM_RANGE_BINS 		256
#define NUM_DOPPLER_BINS 	32
#define NUM_ANT				8
#define NUM_ANGLE_BINS		16
#define MAX_NUM_RANGE_BINS 	256
#define MAX_NUM_DOPPLER_BINS 32

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

/* Buffer to store twiddle factors for 1D FFT */
#pragma DATA_SECTION(twiddle1dFFT,".l2data");
#pragma DATA_ALIGN(twiddle1dFFT, 8);
int16_t    twiddle1dFFT[MAX_NUM_RANGE_BINS*2];

/* input data buffer */
#pragma DATA_SECTION(inputbuf,".l2data");
#pragma DATA_ALIGN(inputbuf, 8);
int16_t    inputbuf[MAX_NUM_RANGE_BINS*2]; /* complex values */

/* buffer to store one half of real-symmetric window for 1D FFT */
#pragma DATA_SECTION(win1Dbuf,".l2data");
#pragma DATA_ALIGN(win1Dbuf, 8);
int16_t    win1Dbuf[(MAX_NUM_RANGE_BINS>>1)];

#pragma DATA_SECTION(fft1Doutbuf,".l2data");
#pragma DATA_ALIGN(fft1Doutbuf, 8);
int16_t    fft1Doutbuf[MAX_NUM_RANGE_BINS*2];

int16_t    fftref[MAX_NUM_RANGE_BINS*2];

/* External RadarCubed Memory. This is where the 1D FFT data is stored */
#pragma DATA_SECTION(radarcubebuf,".l3data");
#pragma DATA_ALIGN(radarcubebuf, 8);
int16_t    radarcubebuf[NUM_ANT*MAX_NUM_RANGE_BINS*NUM_DOPPLER_BINS*2];

#pragma DATA_SECTION(fft2Dinput,".l2data");
#pragma DATA_ALIGN(fft2Dinput, 8);
int16_t    inputbuf2D[MAX_NUM_DOPPLER_BINS*2];

/* buffer to store one half of real-symmetric window for 2D FFT */
#pragma DATA_SECTION(win2Dbuf,".l2data");
#pragma DATA_ALIGN(win2Dbuf, 8);
int16_t    win2Dbuf[MAX_NUM_DOPPLER_BINS>>1];

/* Temporary buffer for storing output of 2D windowing, i.e., input of 2D FFT */
#pragma DATA_SECTION(fft2Dinput,".l2data");
#pragma DATA_ALIGN(fft2Dinput, 8);
float   fft2Dinput[MAX_NUM_DOPPLER_BINS*2];

static unsigned char brev[64] = {
    0x0, 0x20, 0x10, 0x30, 0x8, 0x28, 0x18, 0x38,
    0x4, 0x24, 0x14, 0x34, 0xc, 0x2c, 0x1c, 0x3c,
    0x2, 0x22, 0x12, 0x32, 0xa, 0x2a, 0x1a, 0x3a,
    0x6, 0x26, 0x16, 0x36, 0xe, 0x2e, 0x1e, 0x3e,
    0x1, 0x21, 0x11, 0x31, 0x9, 0x29, 0x19, 0x39,
    0x5, 0x25, 0x15, 0x35, 0xd, 0x2d, 0x1d, 0x3d,
    0x3, 0x23, 0x13, 0x33, 0xb, 0x2b, 0x1b, 0x3b,
    0x7, 0x27, 0x17, 0x37, 0xf, 0x2f, 0x1f, 0x3f
};

/* buffer to store twiddle factors for 2D FFT */
#pragma DATA_ALIGN(twiddle2dFFT, 8);
#pragma DATA_SECTION(twiddle2dFFT, ".l2data");
float   twiddle2dFFT[MAX_NUM_DOPPLER_BINS*2];

/* Temporary buffer for storing output of 2D FFT */
#pragma DATA_SECTION(fft2Doutbuf, ".l2data");
#pragma DATA_ALIGN(fft2Doutbuf, 8);
float   fft2Doutbuf[MAX_NUM_DOPPLER_BINS*2];

#pragma DATA_SECTION( sumPower,".l2data");
#pragma DATA_ALIGN( sumPower,8);
float   sumPower[MAX_NUM_RANGE_BINS*NUM_DOPPLER_BINS]; /* max 1024*32 = 256*128 */

#pragma DATA_SECTION( powbuf,".l2data");
#pragma DATA_ALIGN( powbuf,8);
float   powbuf[MAX_NUM_RANGE_BINS];



#define MAX_DET_OBJ 20

uint32_t detIdxArray[MAX_DET_OBJ];
float    detNoisePowArray[MAX_DET_OBJ];


int32_t    tmp32buf[MAX_NUM_DOPPLER_BINS>>1];
uint16_t   tmp16buf[MAX_NUM_DOPPLER_BINS];


#define MAX_NUM_OBJECTS 16
uint16_t refobject[1+2*MAX_NUM_OBJECTS];
float    refaoa[MAX_NUM_OBJECTS];

#define MAX_NUM_POINTS 165   /* for cluster points */
cplx16_t point2DLoc[MAX_NUM_POINTS];  /* 2D location */
int16_t velocity[MAX_NUM_POINTS];    /* velocity */
int16_t clusterRef[MAX_NUM_POINTS];  /* cluster ID reference */
#define MAX_NUM_CLUSTERS 2

#pragma DATA_SECTION (clusterScratchBuf,".l2data");
#pragma DATA_ALIGN( clusterScratchBuf,8);
int8_t   clusterScratchBuf[4 * ((MAX_NUM_POINTS/3+1)*3)];


FILE       *fid;
FILE       *refFid;
int32_t    cnt;

int32_t    testFailed = 0;
int32_t    totalFailed = 0;
int32_t    prevFailed = 0;

volatile   uint32_t w_startTime1;
uint32_t   t1, t2, cycles;
int32_t    tsc_overhead;


uint32_t cfarGuardLen, cfarSearchLen;
uint8_t dbscanClusterTestFlag;
uint32_t clusterPntIdx;

void test_window1d_rangefft(int32_t numRangeBins, int32_t numDopplerBins, int32_t antIdx)
{
	int32_t   chirpIdx, j;
	int16_t * fft1Doutptr;
	uint32_t  wincyc, fftcyc;
	int16_t * radarcubeptr;

    TSCL = 0;
    printf("\n--- 1D Processing:\n");

    radarcubeptr = &radarcubebuf[antIdx*numRangeBins*numDopplerBins*2];
	fft1Doutptr = &fft1Doutbuf[0];

    /* For every chirp, test 1D windowing and 1D Range FFT */
	for ( chirpIdx = 0; chirpIdx < numDopplerBins; chirpIdx ++)
	{
  		/* printf("chirp index %d:\n", chirpIdx); */
		cnt = fread(inputbuf, sizeof(int16_t), numRangeBins*2, fid);

		w_startTime1=TSCL;
		mmwavelib_windowing16x16( inputbuf, win1Dbuf, numRangeBins);
		wincyc = TSCL-w_startTime1;
		/* printf("window16x16 cycles =%d ", wincyc); */

		w_startTime1=TSCL;
		DSP_fft16x16((int16_t*)twiddle1dFFT, numRangeBins, inputbuf, fft1Doutptr);
		fftcyc = TSCL-w_startTime1;

/* if not run 1dfft, read 1dfft output from tv if tv stores them: */
		//cnt = fread(fft1Doutbuf, sizeof(int16_t), numRangeBins*2, fid);


		for ( j=0; j < numRangeBins*2; j+=2 )
		{
			/* transfer to columns:chirpIdx*2 and chirpIdx*2+1, of radar cube memory */
			radarcubeptr[j*numDopplerBins + chirpIdx*2] = fft1Doutbuf[j];
			radarcubeptr[j*numDopplerBins + chirpIdx*2+1] = fft1Doutbuf[j+1];
#if 0
			/* transfer to chirpIdx-th row of radar cube memory */
			radarcubeptr[chirpIdx*numRangeBins*2 + j] = fft1Doutbuf[j];
			radarcubeptr[chirpIdx*numRangeBins*2 + j +1] = fft1Doutbuf[j+1];
#endif
		}


	} /* chirp loop for range FFT */
	printf("%d-point Window16x16 cycles =%d, FFT16x16 cycles =%d\n", numRangeBins, wincyc, fftcyc);

} /* test_window1d_rangefft */


void tw_gen_float (float *w, int n)
{
    int i, j, k;
    double x_t, y_t, theta1, theta2, theta3;
    const double PI = 3.141592654;

    for (j = 1, k = 0; j <= n >> 2; j = j << 2)
    {
        for (i = 0; i < n >> 2; i += j)
        {
            theta1 = 2 * PI * i / n;
            x_t = cos (theta1);
            y_t = sin (theta1);
            w[k] = (float) x_t;
            w[k + 1] = (float) y_t;

            theta2 = 4 * PI * i / n;
            x_t = cos (theta2);
            y_t = sin (theta2);
            w[k + 2] = (float) x_t;
            w[k + 3] = (float) y_t;

            theta3 = 6 * PI * i / n;
            x_t = cos (theta3);
            y_t = sin (theta3);
            w[k + 4] = (float) x_t;
            w[k + 5] = (float) y_t;
            k += 6;
        }
    }
}


#define CFAR_PER_ANT 0

void test_window2d_dopplerfft_accumpow_floatp(int32_t numRangeBins, int32_t numDopplerBins, float thresh, int32_t antIdx )
{
	int32_t    i, rangebinIdx;
	int32_t    rad2D;
	uint32_t   wincyc, fftcyc, accumcyc;
	int16_t *  radarcubeptr;

    radarcubeptr = &radarcubebuf[antIdx*numRangeBins*numDopplerBins*2];

    printf("--- 2D Processing: ---\n");

	rad2D = 2;
	i  = 30 - _norm(numDopplerBins);
	if ((i & 1) == 0)
	{	rad2D = 4; }


	for ( rangebinIdx = 0; rangebinIdx <numRangeBins; rangebinIdx ++)
	{
		 /* 1D out in cube was stored col by col. So 2D input is in one row. */
		for ( i=0; i < 2*numDopplerBins; i++)
		{
			inputbuf2D[i] = radarcubeptr[rangebinIdx*numDopplerBins*2 + i];
		}

		//Here simply use the same rectangle window win1Dbuf for now
		w_startTime1 = TSCL;
		mmwavelib_dopplerProcWin2DFxdpinFltOut(
									  numDopplerBins,
									  inputbuf2D,
									  win1Dbuf,
									  fft2Dinput );
		wincyc = TSCL-w_startTime1;

		w_startTime1 = TSCL;
		DSPF_sp_fftSPxSP (
				numDopplerBins,
				fft2Dinput,
				twiddle2dFFT,
				fft2Doutbuf,
				brev,
				rad2D,
				0,
				numDopplerBins);
		fftcyc = TSCL-w_startTime1;

		w_startTime1 = TSCL;
		mmwavelib_accumPowerFltp(numDopplerBins,
								antIdx,
								fft2Doutbuf,
								&sumPower[rangebinIdx*numDopplerBins]);

		accumcyc = TSCL-w_startTime1;

#if CFAR_PER_ANT
		int32_t    numDetObj=0;

		numDetObj = mmwavelib_cfarfloat_wrap_opt(
									&sumPower[rangebinIdx*numDopplerBins],
									numDopplerBins,
									MMWSDK_DETECTIONCFAR_CFAR_CAVG, //     uint8_t   cfarType,
									4, //                         uint32_t  guardLen,
									8, //      uint32_t  searchLen,
									thresh,//       float     relThr,
									detIdxArray,
									detNoisePowArray);

		if ( numDetObj >0 )
		{
	        printf("\nAt range bin index %d, num detected object = %d, ", rangebinIdx, numDetObj);
	        printf("detected doppler bin index (and power) : %d (%e)", detIdxArray[0], sumPower[rangebinIdx*numDopplerBins+detIdxArray[0]]);
		    for ( i=1; i<numDetObj;i++)
		        printf(",%d (%e)", detIdxArray[i], sumPower[rangebinIdx*numDopplerBins+detIdxArray[i]] ); //detPowArray[i]
		    printf("\n");
		}
#endif

	}/* end of loop rangebin */

	printf("%d-point FixedpinFltpOut Windowing cycles =%d; DSPF_sp_fftSPxSP cycles =%d; accumPowerFltp cycles =%d\n ", numDopplerBins, wincyc, fftcyc, accumcyc);


#if 0
	// for old test vectors, sumAbs values need to read out before reach next ant:
	for ( rangebinIdx = 0; rangebinIdx < numRangeBins; rangebinIdx++ )
	{
		fread (tmp16buf, sizeof(uint16_t), numDopplerBins, fid);
	}
#endif

} /*test_window2d_dopplerfft_accumpow_floatp*/


void test_cfar( int32_t numRangeBins,
          int32_t numDopplerBins,
		  uint16_t refNumObject,
		  uint16_t *refRangeBin,
		  uint16_t *refDopBin,
		  float thresh )
{
	int32_t    i, j, k;
	int32_t    numDetObj=0;
	uint16_t   numDetect=0;
	float tempNoise;
	uint8_t tempPeakFlag;
	uint16_t tempNumPeak;
	int32_t numObjPerDoppler;
	int32_t numDetObjPerDopper;
	

	printf("\nRun %d-point cfar(type CFAR_CAVG) on every doppler bin:\n",numRangeBins);

	for ( j = 0; j < numDopplerBins; j++ )
	{

		for ( i = 0; i < numRangeBins; i ++)
		{
			powbuf[i] = sumPower[ i*numDopplerBins + j ];
		}
		w_startTime1 = TSCL;

		numDetObj = mmwavelib_cfarfloat_wrap_opt(
				                    powbuf,
									numRangeBins,
									CFAR_CA,
									cfarGuardLen, 
									cfarSearchLen, 
									thresh, 
									detIdxArray,
									detNoisePowArray);

		cycles = TSCL-w_startTime1;

		if (dbscanClusterTestFlag)  /* check the peak over doppler bin */
		{
			tempNumPeak = 0;
			for (k = 0; k < numDetObj; k++)
			{
				tempPeakFlag = mmwavelib_cfarfloat_point(&sumPower[detIdxArray[k]*numDopplerBins] ,
									   numDopplerBins,
									   CFAR_CA,
									   cfarGuardLen,
									   cfarSearchLen,
									   8.f,
									   j,
			                          &tempNoise);
				if (tempPeakFlag)
				{
					detIdxArray[tempNumPeak] =  detIdxArray[k];
					tempNumPeak++;
				}

			}
			numDetObj = tempNumPeak;
		}

		if ( numDetObj >0 )
		{
			printf("Doppler Bin %d: Num detected object: %d, detected range bin index: %d",
								j, numDetObj, detIdxArray[0]);
			printf(" (%e)", powbuf[detIdxArray[0]]);
			for ( i=1; i<numDetObj;i++)
				printf(",%d (%e)", detIdxArray[i], powbuf[detIdxArray[i]]);
			printf("\n");
		}

		if (!dbscanClusterTestFlag)
		{
			for ( i=0; i < refNumObject; i++ )
			{
				if ( j == refDopBin[i] )
				{
					if ( detIdxArray[0] == refRangeBin[i] )
					{
						numDetect ++;
						printf("Detected the object%d succesfully. \n", i+1);
					}
					else if ( numDetObj > 1 && detIdxArray[1] == refRangeBin[i] )
					{
						numDetect ++;
						printf("Detected the object%d succesfully. \n", i+1);
					}
					printf("cfarfloat_wrap_opt cycles =%d.\n", cycles);
				}
			}
		}
		else
		{
			numObjPerDoppler = 0;
			numDetObjPerDopper = 0;
			for (i = 0; i < refNumObject; i++)
			{				
				if (j == refDopBin[i]) 
					numObjPerDoppler++;
				
				for (k = 0; k < numDetObj; k++)
				{
					if ((j == refDopBin[i]) && (refRangeBin[i]==detIdxArray[k] ))
					{
						numDetect++;
						numDetObjPerDopper++;
					}
				}
			}
			
			if ((numObjPerDoppler==numDetObjPerDopper) && (numObjPerDoppler))
			{
				printf("Detected the object%d succesfully at Doppler bin %d\n", numObjPerDoppler, j);
			}
		}
	}/* end of for ( j = 0; j < numDopplerBins; j++ ) */

	if ( numDetect < refNumObject )
	{
		totalFailed++;
		printf("Test Failed: Didn't detect all %d objects.\n", refNumObject);
	}
	else
	{
		printf("\nCFAR detection PASSED.\n");
	}

} /*test cfar*/


#define PI 3.141592653589793
#define ANGLE_EST_RANGE      40 /* in degree */
#define ANGEL_EST_RESOLUTION  1
#define MAX_STEERVEC_SIZE    81 /* (ANGLE_EST_RANGE*2/ANGEL_EST_RESOLUTION +1)  for [-40:1:40] */

#define MAX_ANGLE_PEAK    5

int32_t  numOfSteeringVec = MAX_STEERVEC_SIZE;
cplxf_t  steeringVector[MAX_STEERVEC_SIZE*(NUM_ANT-1)];
cplxf_t  aoainput[NUM_ANT];
float    scratchbuf[MAX_STEERVEC_SIZE +3*MAX_ANGLE_PEAK];

void test_aoaest(int32_t numAnt,
          int32_t numRangeBins,
          int32_t numDopplerBins,
		  uint16_t refNumObject,
		  uint16_t *refRangeBin,
		  uint16_t *refDopBin,
		  uint16_t *refAngle,
		  uint16_t numpeak)
{
	float      varEst;
	int32_t    numAoaPeakDet;
	int32_t    angleEst[MAX_ANGLE_PEAK];
	float      angleEstDegree;
	float      refdegree;
	int16_t *  radarcubeptr;
	int32_t    i, objIdx, antIdx;
	int32_t    rad2D;
	
	rad2D = 2;
	i  = 30 - _norm(numDopplerBins);
	if ((i & 1) == 0)
	{
		rad2D = 4;
	}

	testFailed = 0;
	printf("\nTest AOA estimation:\n");

	for ( objIdx =0; objIdx < refNumObject; objIdx++ )
	{
		for ( antIdx =0; antIdx < numAnt; antIdx++ )
		{
			radarcubeptr = &radarcubebuf[antIdx*numRangeBins*numDopplerBins*2];
			for ( i=0; i < 2*numDopplerBins; i++)
			{
				inputbuf2D[i] = radarcubeptr[refRangeBin[objIdx]*numDopplerBins*2 + i];
			}

			mmwavelib_dopplerProcWin2DFxdpinFltOut(
										  numDopplerBins,
										  inputbuf2D,
										  win1Dbuf,
										  fft2Dinput );


			DSPF_sp_fftSPxSP (
					numDopplerBins,
					fft2Dinput,
					twiddle2dFFT,
					fft2Doutbuf,
					brev,
					rad2D,
					0,
					numDopplerBins);

			aoainput[antIdx].real = fft2Doutbuf[2*refDopBin[objIdx]];  /* even is real */
			aoainput[antIdx].imag = fft2Doutbuf[2*refDopBin[objIdx]+1]; /* odd is imag */
		} /* for numAnt */

		
		w_startTime1 = TSCL;
		numAoaPeakDet = mmwavelib_aoaEstBFSinglePeak(numAnt,
										  1.0, //noise. no test for varEst, so just use 1.0 input here.
										  1.0, //angEstResolution,
										  aoainput,
										  numOfSteeringVec,
										  steeringVector,
										  scratchbuf,
										  &varEst,
										  &angleEst[0]);
		cycles = TSCL-w_startTime1;
		printf(" aoaEstBFSinglePeak cycles =%d.\n", cycles);
		printf(" num detected AOA peaks =%d, index=%d.\n\n", numAoaPeakDet, angleEst[0]);

		if (numpeak == 1)
		{
			angleEstDegree = 1.0f* (angleEst[0] - ANGLE_EST_RANGE);
			refdegree = refaoa[objIdx];
			if ( (angleEstDegree) < (refdegree -1.0f) || angleEstDegree > (refdegree +1.0f) )
			{
				testFailed++;
				printf("AOA estimate failed on object %d: ", objIdx);
			}
			else
			{
				printf("AOA estimate passed on object %d: ", objIdx);
			}
			printf("ref=%5.2f, estimate in degree=%5.2f (index=%d)\n\n",
						refdegree, angleEstDegree, angleEst[0]);
		}

		w_startTime1 = TSCL;
		numAoaPeakDet = mmwavelib_aoaEstBFMultiPeakDet(
											numAnt,
											1.1, //float gamma,
											0.25119, //float sidelobeLevel,
											aoainput,
											steeringVector,
											numOfSteeringVec,
											scratchbuf,
											&angleEst[0]);
		cycles = TSCL-w_startTime1;
		printf(" aoaEstBFMultiPeakDet cycles =%d.\n", cycles);
		printf(" num detected AOA peaks =%d, index=%d", numAoaPeakDet, angleEst[0]);
		for ( i=1; i < numAoaPeakDet; i++ )
		{
			printf(", %d", angleEst[i]);
		}
		printf("\n\n");

		if (numpeak > 1)
		{
			for ( i=0; i < numAoaPeakDet; i++ )
			{
				angleEstDegree = 1.0f* (angleEst[i] - ANGLE_EST_RANGE);
				refdegree = refaoa[objIdx*numpeak +i];
				if ( (angleEstDegree) < (refdegree -1.0f) || angleEstDegree > (refdegree +1.0f) )
				{
					testFailed++;
					printf("AOA estimate failed on object %d: ", objIdx);
				}
				else
				{
					printf("AOA estimate passed on object %d: ", objIdx);
				}
				printf("ref=%5.2f, estimate in degree=%5.2f (index=%d)\n",
							refdegree, angleEstDegree, angleEst[i]);
			}
		}
		
	} /* for refNumObject */

	if ( testFailed == 0 )
	{
		printf("\nAOA estimation PASSED.\n");
	}
	else
	{
		totalFailed += testFailed;
		printf("\nAOA estimation FAILED.\n");
	}
}

void cluster_aoaest(int32_t numAnt,
          int32_t numRangeBins,
          int32_t numDopplerBins,
		  uint16_t refNumObject,
		  uint16_t *refRangeBin,
		  uint16_t *refDopBin)
{
	int32_t    numAoaPeakDet;
	int32_t    angleEst[MAX_ANGLE_PEAK];
	float      angleEstDegree;
	int32_t    i, objIdx;
	double    xytemp;

	printf("\n AOA estimation:\n");

	for ( objIdx =0; objIdx < refNumObject; objIdx++ )
	{
		/* read in aoa input */
		fread(aoainput,sizeof(float), 2 * numAnt, fid);
		

		numAoaPeakDet = mmwavelib_aoaEstBFMultiPeakDet(
														numAnt,
														1.1, //float gamma,
														0.25119, //float sidelobeLevel,
														aoainput,
														steeringVector,
														numOfSteeringVec,
														scratchbuf,
														&angleEst[0]);
		printf(" aoaEstBFMultiPeakDet cycles =%d.\n", cycles);
		printf(" num detected AOA peaks =%d (objectindex=%d), estimate in degree=", numAoaPeakDet,objIdx);

		for ( i=0; i < numAoaPeakDet; i++ )
		{
			angleEstDegree = 1.0f* (angleEst[i] - ANGLE_EST_RANGE);
			printf("%5.2f ,", angleEstDegree);
			/* conver the detected into cluster input */
			xytemp = refRangeBin[objIdx] * 0.038775275735294 * cos(PI *angleEstDegree/180.f) * 256.f;
			if (xytemp)
				point2DLoc[clusterPntIdx].real = (int16_t) (xytemp+0.5);
			else
				point2DLoc[clusterPntIdx].real = (int16_t) (xytemp-0.5);
			xytemp = refRangeBin[objIdx] * 0.038775275735294 * sin(PI *angleEstDegree/180.f) * 256.f;
			if (xytemp)
				point2DLoc[clusterPntIdx].imag = (int16_t) (xytemp+0.5);
			else
				point2DLoc[clusterPntIdx].imag = (int16_t) (xytemp-0.5);

			velocity[clusterPntIdx] = (int16_t ) (refDopBin[objIdx] * 0.936563436563437 * 256.f);
			clusterPntIdx++;

		}
			printf("\n");

	} /* for refNumObject */

	printf("\nAOA estimation DONE.\n");
	
}

int32_t gen_steeringVec(float angEstRange, float angEstResolution,uint32_t numAnt,cplxf_t * vec)
{
	int32_t numVec;
	int32_t kk,jj;
	double temp1,temp2,tempr, tempi;

	numVec = (int32_t)((2.f * angEstRange)/angEstResolution + 1);

	for (kk = 0; kk < numVec;kk++)
	{
		/* antenna 0 is always 1, no need to save it */
		for (jj = 1; jj < numAnt; jj ++)
		{
			/* exp(-j2pi*antDis*antPos*sin(angle)
			 * antDis = 0.5
			 * antPos = 0,1,2,
			 * */
			temp1 = sin((-angEstRange + (double)kk * angEstResolution) * PI/180.0);
			temp2 = -1.f*PI*(double)jj*temp1;
			tempr = cos(temp2);
			tempi = sin(temp2);

			vec[(numAnt-1)*kk+(jj-1)].real = (float) tempr;
			vec[(numAnt-1)*kk+(jj-1)].imag = (float) tempi;
		}
	}
	return numVec;
}


void read_windowfactors(int32_t numRangeBins, int32_t numDopBins )
{
	cnt = fread(win1Dbuf, sizeof(int16_t), numRangeBins/2, fid);
	printf("file read count for 1D window =%d\n", cnt);

	//existing test vec 2d window factors are 32-bit, just pass it, not used it.
	cnt = fread(tmp32buf, sizeof(int32_t), numDopBins/2, fid);
	//printf("file read count for 2D window =%d\n", cnt);
}


void read_refobj(uint16_t numAoaPeak)
{
	int32_t i;
	uint16_t numberObj;

	cnt = fread(refobject, sizeof(uint16_t), 1, refFid);
	printf("ref number of cfar objects: %d\n", refobject[0]);

	numberObj = refobject[0];
	cnt = fread(&refobject[1], sizeof(uint16_t), 2*numberObj, refFid);
	printf("ref doppler&range indices" );
	for ( i=1; i<=numberObj*2; i++ )
	{
		printf(", %d", refobject[i] );
	}
	printf(".\n");


	if (!dbscanClusterTestFlag)
	{
		cnt = fread(refaoa, sizeof(float), numberObj*numAoaPeak, refFid);
		printf("ref angle of arrival in degree" );
		for ( i=0; i<numberObj*numAoaPeak; i++ )
		{
			printf(", %5.2f", refaoa[i] );
		}
		printf(".\n");
	}

}

void read_refCluster()
{
	fread(clusterRef,sizeof (uint16_t), clusterPntIdx, refFid);
}

void test_libs( int32_t numRangeBins,
			int32_t numDopplerBins,
			int32_t numAnt,
			uint16_t numObjRef,
			float relthres,
			uint16_t numAoaPeak,
			const char* tvname)
{
	int32_t antIdx;

	prevFailed = totalFailed;

	read_windowfactors(numRangeBins, numDopplerBins);

	for ( antIdx = 0; antIdx < numAnt; antIdx++ )
	{
		printf ("\n----- Antenna index %d:", antIdx);
		test_window1d_rangefft(numRangeBins, numDopplerBins, antIdx);
		test_window2d_dopplerfft_accumpow_floatp(numRangeBins, numDopplerBins, relthres, antIdx);
	}

	printf("\n----- All %d antennae's 1D&2D processing done.\n", numAnt);

	test_cfar(numRangeBins, numDopplerBins, numObjRef, &refobject[1+numObjRef], &refobject[1], relthres);

	if  (!dbscanClusterTestFlag)  
		test_aoaest(numAnt, numRangeBins, numDopplerBins, numObjRef, &refobject[1+numObjRef], &refobject[1], &refobject[1+ 2*numObjRef], numAoaPeak);
	else
		cluster_aoaest(numAnt, numRangeBins, numDopplerBins, numObjRef, &refobject[1+numObjRef], &refobject[1]);
	

	printf("\nThis test case's fail count = %d. Total failed = %d\n", totalFailed-prevFailed, totalFailed);

    if ( totalFailed > prevFailed)
    	MCPI_setFeatureTestResult(tvname, MCPI_TestResult_FAIL);
    else
        MCPI_setFeatureTestResult(tvname, MCPI_TestResult_PASS);

} /* test_libs */

void test_dbscan()
{
	mmwavelib_clusterInfor clusterParam[MAX_NUM_CLUSTERS];
	uint16_t clusterIdxArray[MAX_NUM_POINTS];
	uint16_t numClusters;
	int32_t  i;
	testFailed = 0;
	printf("\nTest DBSCAN Clustering:\n");

	w_startTime1 = TSCL;
	mmwavelib_dbscan(point2DLoc,
			         velocity,
					 2*256*256,
					 1.f,
					 clusterPntIdx, /* total points*/
					 20,
					 clusterScratchBuf,
					 &numClusters,
					 clusterIdxArray,
					 clusterParam
					);

	cycles = TSCL-w_startTime1;
	printf(" DBSCAN cycles =%d.\n", cycles);

	for (i = 0; i < clusterPntIdx; i++)
	{
		if (clusterIdxArray[i]!=clusterRef[i])
		{
			testFailed++;
			printf("DBSCAN cluster failed on object %d: ", i);
		}
	}

	if ( testFailed == 0 )
	{
		printf("\nDBSCAN Clustering PASSED.\n");
	}
	else
	{
		totalFailed += testFailed;
		printf("\nDBSCAN Clustering FAILED.\n");
	}


}/* test_dbscan*/
#define TEST_MULTIOBJ 1
#if TEST_MULTIOBJ
#define TEST_MULTIOBJ_SNR0   1
#define TEST_AOA_MULTIPEAK   1
#define TEST_DBSCAN_CLUSTERING 1
#endif


void main(void)
{
    int32_t  numAnt = 8;
    float    relthres = 16;
    uint16_t aoapeak = 1; /*default single peak for AOA*/

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

	
	/* Generate twiddle factors for 256-pt 1d FFT */
    t1 = TSCL;
	gen_twiddle_fft16x16((int16_t*)twiddle1dFFT, 256);
    t2 = TSCL;
    printf("256-point twiddle16x16 cycles=%d\n", t2-t1);

	/* Generate twiddle factors for 32-pt 2d FFT */
    t1 = TSCL;
	tw_gen_float(twiddle2dFFT, 32);
    t2 = TSCL;
    printf("32-point float twiddle cycles=%d\n", t2-t1);

    /* generate the steering vector */
    numOfSteeringVec = gen_steeringVec(40, 1.0, numAnt, steeringVector);


#if TEST_MULTIOBJ_SNR0
	printf("\n===  Floating-point test vector with single object: ===\n");

    if ((refFid = fopen ("..\\testdata\\radarDemo_ref_new.bin","rb")) == NULL)
    {
        printf ("file open error: can not open ref file\n");
        exit(3);
    }
    relthres = 16;
    cfarGuardLen = 4;
    cfarSearchLen = 8;
    dbscanClusterTestFlag = 0;
       
    read_refobj(aoapeak);
    fclose(refFid);

    if ((fid = fopen ("..\\testdata\\floatPoint_chain_test.bin","rb")) == NULL)
    {
        printf ("file open error: can not open input data file\n");
        exit(3);
    }
   
    test_libs(256, 32, numAnt, refobject[0], relthres, aoapeak, "demo case 256pt 1DFFT 32pt 2DFFT single-obj SNR0");
    fclose(fid);
	printf("\n=== Floating-point TV single object SNR0 done. ====\n");

#endif

#if TEST_AOA_MULTIPEAK
	printf("\n===  Floating-point test vector multi-peak-AOA: ===\n");

    if ((refFid = fopen ("..\\testdata\\floatpoint_test_aoaMultiObj_ref.bin","rb")) == NULL)
    {
        printf ("file open error: can not open ref file\n");
        exit(3);
    }

    
    aoapeak = 2; /* 2 peaks in this multi-peak-AOA tv. ref file doesn't have this infor. so hardcode here. */
    cfarGuardLen = 4;
    cfarSearchLen = 8;
    dbscanClusterTestFlag = 0;
    
    read_refobj( aoapeak);
    fclose(refFid);

    if ((fid = fopen ("..\\testdata\\floatpoint_test_aoaMultiObj.bin","rb")) == NULL)
    {
        printf ("file open error: can not open input data file\n");
        exit(3);
    }

    relthres = 16;
    test_libs(256, 32, numAnt, refobject[0], relthres, aoapeak, "demo case 256pt 1DFFT 32pt 2DFFT multi-peak-AOA");
    fclose(fid);
	printf("\n=== Floating-point TV multi-peak-AOA done. ====\n");

#endif

#if TEST_DBSCAN_CLUSTERING

	printf("\n===  Floating-point test vector DBSCAN Clustering: ===\n");


	if ((refFid = fopen ("..\\testdata\\floatpoint_test_dbscanCluster_ref.bin","rb")) == NULL)
	{
	    printf ("file open error: can not open ref file\n");
	    exit(3);
	}

	if ((fid = fopen ("..\\testdata\\floatpoint_test_dbscanCluster.bin","rb")) == NULL)
	{
	     printf ("file open error: can not open input data file\n");
	     exit(3);
	}


	relthres = 120.f;
	cfarGuardLen = 8;
	cfarSearchLen = 8;
	dbscanClusterTestFlag = 1;
	aoapeak = 1;

	clusterPntIdx = 0;

	/* first frame data */
	printf("\n---- first frame data processing for clustering test----\n");

	read_refobj( aoapeak);
	test_libs(256, 32, numAnt, refobject[0], relthres, aoapeak, "demo case 256pt 1DFFT 32pt 2DFFT first frame");
	

	/* second frame data */
	printf("\n---- second frame data processing for clustering test----\n");
	read_refobj( aoapeak);
	test_libs(256, 32, numAnt, refobject[0], relthres, aoapeak, "demo case 256pt 1DFFT 32pt 2DFFT second frame");
		
	/* third frame data */
	printf("\n---- third frame data processing for clustering test----\n");
	read_refobj( aoapeak);
	test_libs(256, 32, numAnt, refobject[0], relthres, aoapeak, "demo case 256pt 1DFFT 32pt 2DFFT third frame");
	
	/* forth frame data */
	printf("\n---- forth frame data processing for clustering test----\n");
	read_refobj( aoapeak);
	test_libs(256, 32, numAnt, refobject[0], relthres, aoapeak, "demo case 256pt 1DFFT 32pt 2DFFT forth frame");
	/* read the cluster ID reference */
	read_refCluster();
	/* run the dbscan */
	test_dbscan();
	fclose(refFid);
	fclose(fid);
	
	
	printf("\n=== Floating-point TV DBSCAN Clustering done. ====\n");
#endif

	if ( totalFailed == 0 )
		printf("\n==========  All tests passed! ==========\n");

	printf("\n==========  Testing completed! ==========\n");

        MCPI_setTestResult ();
}


