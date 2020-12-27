/*
 *   @file  lib_accumpowerflt_test.c
 *
 *   @brief
 *      Unit Test code for floating point power accumulation mmwavelib function
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
/* #include <ti/sysbios/family/c64p/Cache.h> */

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

FILE *fid;
int16_t testPASS=1;
uint32_t t1,t2;
uint32_t  cycles;

#define MAX_FFT_SIZE 1024   /* maximum fft size */
#define MAX_NUM_RX_ANTENNA 8
#define NUM_OF_TEST 7
#define MAXIMUM_ERROR 0.f

/* input data buffer */
#pragma DATA_SECTION(inputbuf,".l2data");
#pragma DATA_ALIGN(inputbuf, 8);
float inputbuf[MAX_FFT_SIZE*MAX_NUM_RX_ANTENNA*2];   /* complex values */


/* output data buffer */
#pragma DATA_SECTION(outputbuf,".l2data");
float outputbuf[MAX_FFT_SIZE];   /* accumulated power */


/* output referef buffer */
#pragma DATA_SECTION(refbuf,".l2data");
float refbuf[MAX_FFT_SIZE];   /* accumulated power */

void ref_poweraccum(uint32_t length, uint32_t numAnt, float * input, float * output)
{
	uint32_t ii,jj;
	double sum;
	for (ii = 0; ii < length; ii++)
	{
		sum=0;
		for (jj = 0; jj<numAnt; jj++)
		{
			sum+= input[jj*length*2+2*ii] * input[jj*length*2+2*ii];
			sum+= input[jj*length*2+2*ii+1] * input[jj*length*2+2*ii+1];
		}

		output[ii] = (float) sum;
	}
}
void main(void)
{
	uint32_t numTests;
	uint32_t fftSize[NUM_OF_TEST];
	uint32_t numRxAntArray[4]={1,2,4,8}; /* test 2, 4 and 8 RX antenna */

	uint32_t ii,jj,kk,mm;
	float diff;
	/* Cache_Size     cacheSize; */


	MCPI_Initialize();
	TSCL = 0;

	/* default is 32K */
	/*
	cacheSize.l1pSize = Cache_L1Size_32K;
	cacheSize.l1dSize = Cache_L1Size_32K;
	cacheSize.l2Size = Cache_L2Size_0K;
	Cache_setSize(&cacheSize);
	*/

	fid = fopen("..\\testdata\\accumPowerFlt_test.bin","rb");
    if (fid==NULL)
    {
    	printf("File Open Error: windowFlt_test.bin can not open\n");
    	exit(3);
    }
    /* read in number of 2D fft size */
    fread(&numTests,sizeof(uint32_t),1,fid);
    /* read in 2D fft size array */
    fread(fftSize,sizeof(uint32_t),numTests,fid);

    printf("\n------ accumPowerFltpAllAnt and accumPowerFltpPerAnt feature test\n ");
    for (ii=0;ii<numTests;ii++)
    {
    	/* read in the data */
    	fread(inputbuf,sizeof(float),fftSize[ii]*2*MAX_NUM_RX_ANTENNA,fid);
    	/* test for rx antenna */
    	for (jj=0;jj<4;jj++)
    	{
    		/* read in reference */
    		 fread(refbuf,sizeof(float),fftSize[ii],fid );
    		 /* ref_poweraccum(fftSize[ii], numRxAntArray[jj], inputbuf, refbuf); */

    		 /* call the lib */
    		for (mm=0; mm<numRxAntArray[jj];mm++)
			{
    			mmwavelib_accumPowerFltp(fftSize[ii],
    									mm,
										&inputbuf[mm*2*fftSize[ii]],
										outputbuf);

			}
			
    		/* check the result */
    		for (kk=0;kk<fftSize[ii];kk++)
    		{
    			diff = refbuf[kk] - outputbuf[kk];
    			if ( (diff>  MAXIMUM_ERROR) ||  (diff<-1* MAXIMUM_ERROR))
    			{
    				testPASS=0;
    				printf("perant: fftsize=%d, numAnt=%d,ref=%f,lib=%f,kk=%d, diff=%f\n",fftSize[ii],numRxAntArray[jj],refbuf[kk],outputbuf[kk],kk,diff);
    			}
    		}

    		mmwavelib_accumPowerFltpAllAnt(fftSize[ii],
    						   numRxAntArray[jj],
							   inputbuf,
							   outputbuf);



    		/* check the results */
    		for (kk=0;kk<fftSize[ii];kk++)
    		{
    			diff = refbuf[kk] - outputbuf[kk];
    			if ( (diff>  MAXIMUM_ERROR) ||  (diff<-1* MAXIMUM_ERROR))
    			{
    				testPASS=0;
    				printf("fftsize=%d, numAnt=%d,ref=%f,lib=%f,kk=%d, diff=%f\n",fftSize[ii],numRxAntArray[jj],refbuf[kk],outputbuf[kk],kk,diff);
    			}
    		}

    	}
    }


    if (testPASS)
	{
       	MCPI_setFeatureTestResult("The accumPowerFltpAllAnt and accumPowerFltpPerAnt functionality test", MCPI_TestResult_PASS);
    }
	else
	{
		MCPI_setFeatureTestResult("The accumPowerFltpAllAnt and accumPowerFltpPerAnt functionality test", MCPI_TestResult_FAIL);
	}

    printf("\n------ accumPowerFltpAllAnt benchmark\n ");
    printf("fftsize, numRxAnt, cycles\n");
    for (ii=0;ii<numTests;ii++)
    {
      	for (jj=0;jj<4;jj++)
       	{
       		mmwavelib_accumPowerFltpAllAnt(fftSize[ii],
        						   numRxAntArray[jj],
    							   inputbuf,
    							   outputbuf);
      		{
      			t1=TSCL;
       			mmwavelib_accumPowerFltpAllAnt(fftSize[ii],
       			    						   numRxAntArray[jj],
       										   inputbuf,
       										   outputbuf);
       			t2=TSCL;
       			cycles = t2-t1;
       		}

       		printf("%d\t%d\t%d\n",fftSize[ii],numRxAntArray[jj],cycles );

       	}
    }

    printf("\n------ accumPowerFltpPerAnt benchmark\n ");
    printf("fftsize, antIdx, cycles\n");
    for (ii=0;ii<numTests;ii++)
    {
       	for (jj=0;jj<2;jj++)
        {
        	mmwavelib_accumPowerFltp(fftSize[ii],
            						 jj,
        							 inputbuf,
        							 outputbuf);
          	{
          		t1=TSCL;
          		mmwavelib_accumPowerFltp(fftSize[ii],
          								 jj,
										 inputbuf,
           								 outputbuf);
           		t2=TSCL;
           		cycles = t2-t1;
           	}

           	printf("%d\t%d\t%d\n",fftSize[ii],jj,cycles );

           }
    }

    fclose(fid);
	
	if (testPASS)
	{
		printf("========== Floating point Power accumulation tests passed! ========== \n");
	}
	else
	{
		printf("==========  Floating point Power accumulation tests Failed! ========== \n");
	}
	printf("========== Testing completed! ==========\n");
	
	MCPI_setTestResult ();
}
