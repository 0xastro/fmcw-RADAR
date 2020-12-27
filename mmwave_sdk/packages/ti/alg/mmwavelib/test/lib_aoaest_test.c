/*
 *   @file  lib_aoaest_test.c
 *
 *   @brief
 *      Unit Test code for floating-point AOAestimate mmwavelib functions
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

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

FILE *fid;
int16_t testPASS=1;  /* for cfar av, acc, so, go testing */
uint32_t t1,t2;
uint32_t  cycles;
#define MAXNUMANT 24  /* maximum number of antennna */
#define MAXNUMSTEERINGVEC 301  /* [-15:0.1:15] */
#define PIOVER180 (3.141592653589793/180.0)
#define MAXNUMPEAKS 4  /* test vector maximum peaks is 4 */
#define PI 3.141592653589793
#define MAXSPECTRUMERR 4.f
#define MAXVARIANCEERR 1.f

/* steering vector buffer */
#pragma DATA_SECTION(steeringVectors,".l2data");
cplxf_t steeringVectors[MAXNUMSTEERINGVEC * (MAXNUMANT-1) ];

/* input data */
#pragma DATA_SECTION(inputData,".l2data");
cplxf_t inputData[MAXNUMANT];

/* angular spectrum*/
#pragma DATA_SECTION(scratchBuffer,".l2data");
float scratchBuffer[MAXNUMSTEERINGVEC+3*MAXNUMPEAKS];

#pragma DATA_SECTION(spectrumRef,".l2data");
float spectrumRef[MAXNUMSTEERINGVEC];

#define MAXIMUMANGSETS 2
uint32_t numAngSets;
float angEstRange[MAXIMUMANGSETS];
float angEstResolution[MAXIMUMANGSETS];
uint32_t numAntSets[MAXIMUMANGSETS];

uint32_t gen_steeringVec(float angEstRange, float angEstResolution,uint32_t numAnt,cplxf_t * vec)
{
	uint32_t numVec;
	uint32_t kk,jj;
	double temp1,temp2,tempr, tempi;
	numVec = (uint32_t)((2.f * angEstRange)/angEstResolution + 1);
	for (kk = 0; kk < numVec;kk++)
	{
		/* antenna 0 is always 1, no need to save it */
		for (jj = 1; jj < numAnt; jj ++)
		{
			/* exp(-j2pi*antDis*antPos*sin(angle)
			 * antDis = 0.5
			 * antPos = 0,1,2,
			 * */
			temp1 = sin((-angEstRange + (double)kk * angEstResolution) * PIOVER180);
			temp2 = -1.f*PI*(double)jj*temp1;
			tempr = cos(temp2);
			tempi = sin(temp2);

			vec[(numAnt-1)*kk+(jj-1)].real = (float) tempr;
			vec[(numAnt-1)*kk+(jj-1)].imag = (float) tempi;
		}
	}
	return numVec;
}
void test_aoaest_bmSingle()
{
	uint32_t numAnt,numTests;
	uint32_t ii,jj,kk,mm;

	float noise,estVar;
	int32_t angleEst;
	uint32_t numOfSteeringVec;
	uint32_t angleRef;
	float varRef;
	uint32_t numAntArray[4]={4,8,16,24};/* for bench mark */
	uint32_t steeringVecArray[6]={51,101,151,201,251,301};


	for (ii = 0; ii < numAngSets;ii++)
	{
		for (jj = 0; jj < numAntSets[ii]; jj++)
		{
			/* read in number of ant */
			fread(&numAnt,sizeof(uint32_t),1,fid);
			/* read in number of tests */
			fread(&numTests,sizeof(uint32_t),1,fid);

			/* generate the steering vector */
			numOfSteeringVec = gen_steeringVec(angEstRange[ii],angEstResolution[ii],numAnt,steeringVectors);
			for (kk = 0; kk < numTests; kk++)
			{
				printf("angleset=%d,numAnt=%d,testId=%d\n",ii,numAnt,kk);
				/* read in input data */
				fread(inputData,sizeof(float),2*numAnt,fid);
				/* read in noise variance */
				fread(&noise,sizeof(float),1,fid);
				/* read in spctrum reference */
				fread(spectrumRef,sizeof(float),numOfSteeringVec,fid);
				/* read in estimated angle index*/
				fread(&angleRef,sizeof(uint32_t),1,fid);
				/* read in reference angle estimation variance */
				fread(&varRef,sizeof(float),1,fid);

				/* call lib function */
				mmwavelib_aoaEstBFSinglePeakDet(
												numAnt,
												inputData,
												numOfSteeringVec,
												steeringVectors,
												&angleEst);
				/* check the estimated angle */
				if (angleEst!=angleRef)
				{
					 testPASS = 0;
					 printf("AOA only: angleset=%d,numAnt=%d,test=%d, angleEst=%d,angleRef=%d\n",ii,numAnt,kk,angleEst,angleRef);
				}

				mmwavelib_aoaEstBFSinglePeak(numAnt,
				                              noise,
											  angEstResolution[ii],
											  inputData,
											  numOfSteeringVec,
											  steeringVectors,
				                              scratchBuffer,
				                              &estVar,
				                              &angleEst);

				 /* check the spectrum*/
				 for (mm=0;mm<numOfSteeringVec;mm++)
				 {
					 if (((scratchBuffer[mm]-spectrumRef[mm])>MAXSPECTRUMERR) || ((scratchBuffer[mm]-spectrumRef[mm])<-1.f*MAXSPECTRUMERR))
					 {
						 testPASS = 0;
						 printf("spectrum: ref=%f,est=%f,diff=%f\n",spectrumRef[mm],scratchBuffer[mm],spectrumRef[mm]-scratchBuffer[mm]);
					 }
				 }

				 /* check the estimated angle */
				 if (angleEst!=angleRef)
				 {
					 testPASS = 0;
					 printf("angleset=%d,numAnt=%d,test=%d, angleEst=%d,angleRef=%d\n",ii,numAnt,kk,angleEst,angleRef);
				 }

				 /* check the variance */
				 if ((estVar - varRef > MAXVARIANCEERR ) || (estVar - varRef < -1* MAXVARIANCEERR ) )
				{
					 testPASS = 0;
					printf("estVar=%f,varRef=%f, diff=%f\n",estVar,varRef,estVar-varRef);
				}

			}


		}
	}

	if (testPASS)
	{
		MCPI_setFeatureTestResult("Floating point AOA Beamforming Single Peak tests",MCPI_TestResult_PASS);
	}
	else
	{
		MCPI_setFeatureTestResult("Floating point AOA Beamforming Single Peak tests",MCPI_TestResult_FAIL);
	}


	/* benchmark*/
	printf("\nBenchmark AOA beamforming method single peak, AOA detection only \n");
	printf("number of Ant, number of Steering vector, cycles\n");
	for (jj = 0; jj < 4; jj++)
	{
		for (mm = 0; mm < 6; mm++)
		{
			t1 = TSCL;
			 mmwavelib_aoaEstBFSinglePeakDet(numAntArray[jj],
					                      inputData,
										  steeringVecArray[mm],
										  steeringVectors,
					                       &angleEst);
			 t2 = TSCL;

			 cycles = t2-t1;
			 printf("%d\t%d\t%d\n",numAntArray[jj],steeringVecArray[mm],cycles);

		}
	}

	printf("\nBenchmark aoaEstBeamformingSinglePeak only \n");
	printf("number of Ant, number of Steering vector, cycles\n");
	for (jj = 0; jj < 4; jj++)
	{
		for (mm = 0; mm < 6; mm++)
		{
			t1 = TSCL;
			 mmwavelib_aoaEstBFSinglePeak(numAntArray[jj],
					                      noise,
										  angEstResolution[0],
										  inputData,
										  steeringVecArray[mm],
										  steeringVectors,
					                      scratchBuffer,
					                       &estVar,
					                       &angleEst);
			 t2 = TSCL;
			 cycles = t2-t1;
			 printf("%d\t%d\t%d\n",numAntArray[jj],steeringVecArray[mm],cycles);
		}
	}


}

void test_aoaest_bmMulti()
{

	uint32_t numAnt,numTests;
	uint32_t ii,jj,kk,mm;
	uint32_t numPeaksRef,numPeaks;
	uint32_t angleIdxRef[MAXNUMPEAKS];
	int32_t  angleIdx[MAXNUMPEAKS];
	int32_t tempangleIdx[105];
	float angleVar[MAXNUMPEAKS],varRef[MAXNUMPEAKS];
	float tempanglevar[105];
	uint32_t numOfSteeringVec;
	uint32_t numAntArray[4]={4,8,16,24};/* for bench mark */
	uint32_t steeringVecArray[6]={51,101,151,201,251,301};


	for (ii = 0; ii < numAngSets;ii++)
	{
		for (jj = 0; jj < numAntSets[ii]; jj++)
		{
			/* read in number of ant */
			fread(&numAnt,sizeof(uint32_t),1,fid);
			/* read in number of tests */
			fread(&numTests,sizeof(uint32_t),1,fid);
			/* generate the steering vector */
			numOfSteeringVec = gen_steeringVec(angEstRange[ii],angEstResolution[ii],numAnt,steeringVectors);

			for (kk=0;kk<numTests;kk++)
			{
				printf("multiple peak: angleset=%d,numAnt=%d,testId=%d\n",ii,numAnt,kk);
				/* read in input data */
				fread(inputData,sizeof(float),2*numAnt,fid);
				/* read in number of detect peaks*/
				fread(&numPeaksRef,sizeof(uint32_t),1,fid);
				/* read in detect peak index*/
				fread(angleIdxRef,sizeof(uint32_t),numPeaksRef,fid);
				/* read in estimation variance */
				fread(varRef,sizeof(float),numPeaksRef,fid);

				/* call lib */
				numPeaks = mmwavelib_aoaEstBFMultiPeakDet(
														numAnt,
				                                        1.2f,
														0.251188643150958,
														inputData,
														steeringVectors,
													    numOfSteeringVec,
														scratchBuffer,
													    angleIdx);

				/* check results */
				if (numPeaks!=numPeaksRef)
				{
					 testPASS = 0;
					 printf("multiPeakDet: angleset=%d,numAnt=%d,test=%d, numPeaksEst=%d,numPeaksRef=%d\n",ii,numAnt,kk,numPeaks,numPeaksRef);

				}

				for(mm=0;mm<numPeaksRef;mm++)
				{
					if (angleIdx[mm]!=angleIdxRef[mm])
					{
						 testPASS = 0;
						 printf("multiPeakDet: angleset=%d,numAnt=%d,test=%d, PeaksEst=%d,PeaksRef=%d\n",ii,numAnt,kk,angleIdx[mm],angleIdxRef[mm]);

					}
				}

				/* call lib */
				numPeaks = mmwavelib_aoaEstBFMultiPeak(
													   numAnt,
													   1.2f,
													   0.251188643150958, /* -6 db */
													   2.5f,
													   10000.f, /* 40 dB linear format*/
													   angEstResolution[ii],
													   inputData,
													   steeringVectors,
													   numOfSteeringVec,
													   scratchBuffer,
													   angleVar,
													   angleIdx);

				/* check results */
				if (numPeaks!=numPeaksRef)
				{
					 testPASS = 0;
					 printf("multiPeak: angleset=%d,numAnt=%d,test=%d, numPeaksEst=%d,numPeaksRef=%d\n",ii,numAnt,kk,numPeaks,numPeaksRef);
				}

				for(mm=0;mm<numPeaksRef;mm++)
				{
					if (angleIdx[mm]!=angleIdxRef[mm])
					{
						 testPASS = 0;
						 printf("multiPeak: angleset=%d,numAnt=%d,test=%d, PeaksEst=%d,PeaksRef=%d\n",ii,numAnt,kk,angleIdx[mm],angleIdxRef[mm]);
					}
				}

				for(mm=0;mm<numPeaksRef;mm++)
				{
					if((angleVar[mm]-varRef[mm]>MAXVARIANCEERR) || (angleVar[mm]-varRef[mm]<-1.f*MAXVARIANCEERR))
					{
						 testPASS = 0;
						 printf("multiPeak: angleset=%d,numAnt=%d,test=%d, varEst=%f,varRef=%f\n",ii,numAnt,kk,angleVar[mm],varRef[mm]);
					}
				}


			}
		}
	}

	if (testPASS)
	{
		MCPI_setFeatureTestResult("Floating point AOA Beamforming Multiple Peaks tests",MCPI_TestResult_PASS);
	}
	else
	{
		MCPI_setFeatureTestResult("Floating point AOA Beamforming Multiple Peaks tests",MCPI_TestResult_FAIL);
	}

	printf("\nBenchmark AOA beamforming method multiple peaks, AOA detection only \n");
	printf("number of Ant, number of Steering vector, cycles\n");
	for (jj = 0; jj < 4; jj++)
	{
		for (mm = 0; mm < 6; mm++)
		{
			t1 = TSCL;
			numPeaks = mmwavelib_aoaEstBFMultiPeakDet(
					                                 numAntArray[jj],
							                         1.2f,
													 0.251188643150958,
													 inputData,
													 steeringVectors,
													 steeringVecArray[mm],
													 scratchBuffer,
													 tempangleIdx);
			t2 = TSCL;
			cycles = t2-t1;
			printf("%d\t%d\t%d\n",numAntArray[jj],steeringVecArray[mm],cycles);
		}
	}

	printf("\nBenchmark AOA beamforming method multiple peaks\n");
	printf("number of Ant, number of Steering vector, cycles\n");
	for (jj = 0; jj < 4; jj++)
	{
		for (mm = 0; mm < 6; mm++)
		{
			t1 = TSCL;
			numPeaks = mmwavelib_aoaEstBFMultiPeak(
					                              numAntArray[jj],
												  1.2f,
												  0.251188643150958, /* -6 db */
												  2.5f,
												  10000.f, /* 40 dB linear format*/
												  1.f,
												  inputData,
												  steeringVectors,
												  steeringVecArray[mm]/2,
												  scratchBuffer,
												  tempanglevar,
												  tempangleIdx);
			t2 = TSCL;
			cycles = t2-t1;
			printf("%d\t%d\t%d\n",numAntArray[jj],steeringVecArray[mm]/2,cycles);
		}
	}




}
void main(void)
{

	MCPI_Initialize();
	TSCL = 0;
	
	printf("..... AOA Estimation using beamforming method test  .......\n");

	fid = fopen("..\\testdata\\aoaest_test.bin","rb");
	if (fid==NULL)
	{
	  	printf("File Open Error: aoaest_test.bin can not open\n");
	   	exit(3);
	}

	/* read in number of angle sets */
	fread(&numAngSets,sizeof(uint32_t),1,fid);
	/* read in angle range */
	fread(&angEstRange,sizeof(float),numAngSets,fid);
	/* read in angle resolution */
	fread(&angEstResolution, sizeof(float),numAngSets,fid);
	/* read in number of antenna sets for each set */
	fread(&numAntSets,sizeof(uint32_t),numAngSets,fid);

	printf("AOA estimation using beamforming single peak testing\n");
	test_aoaest_bmSingle();
	printf("AOA estimation using beamforming multiple peaks testing\n");
	test_aoaest_bmMulti();

	fclose(fid);
	if (testPASS)
	{
		printf("========== Floating point AOA ESTIMATION tests all passed! ========== \n");
	}
	else
	{
		printf("==========  Floating point AOA ESTIMATION tests Failed! ========== \n");
	}
	printf("========== AOA Testing completed! ==========\n");

	MCPI_setTestResult ();
}
