/*
 *   @file  lib_cfarcall_test.c
 *
 *   @brief
 *      Unit Test code for floating-point CFAR mmwavelib functions
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
int16_t testOPTPASS=1; /* for cfar less cycles testing */
int16_t testPOINTPASS = 1; /* for single point cfar testing */
int16_t testPEAKPASS = 1; /* for cfar peak detection testing */
uint32_t t1,t2;
uint32_t  cycles;

#define MAXNUMFFTSIZE 5

#define MAX_NUM_RANGE_BINS 2048   /* maximum fft size */
#define MAXIMUM_ERROR 1.f


/* input data buffer, accumulated power */
#pragma DATA_SECTION(inputPowerPtr,".l2data");
float inputPowerPtr[MAX_NUM_RANGE_BINS];   

/* output data buffer */
#pragma DATA_SECTION(noiseVarBuf,".l2data");
float noiseVarBuf[MAX_NUM_RANGE_BINS];


#pragma DATA_SECTION(refBuf,".l2data");
float refBuf[MAX_NUM_RANGE_BINS];

#pragma DATA_SECTION(clusterIndex,".l2data");
uint32_t clusterIndex[MAX_NUM_RANGE_BINS];



void test_cfar_caall()
{
	uint32_t numTestFFTSize;
	uint32_t fftSizeArray[MAXNUMFFTSIZE] ;
	uint32_t numTestPerFFT;
	uint32_t ii,jj,kk;
	uint32_t lskip, rskip, guard,winsize;
	uint32_t numDetected;


    fid = fopen("..\\testdata\\cfarca_test.bin","rb");
    if (fid==NULL)
    {
    	printf("File Open Error: cfarca_test.bin can not open\n");
    	exit(3);
    }
    /* read in tested number of fftsize */
    fread(&numTestFFTSize, sizeof(uint32_t), 1, fid);
    /* read in tested fft size */
    fread(&fftSizeArray[0],sizeof(uint32_t), numTestFFTSize,fid);

    for (ii=0;ii<numTestFFTSize;ii++)
    {

    	printf("test %d FFT Size = %d\n", ii, fftSizeArray[ii]);
        /* read in number of tests per fftsize */
        fread(&numTestPerFFT,sizeof(uint32_t),1,fid);

    	/* read in the accumulated pwer array */
    	fread(inputPowerPtr,sizeof(float),fftSizeArray[ii],fid);
    	for (jj=0;jj<numTestPerFFT;jj++)
    	{
			/* read in left skip */
    		fread(&lskip,sizeof(uint32_t),1,fid);
    		/* read in right skip */
    		fread(&rskip,sizeof(uint32_t),1,fid);
    		/* read in guard size one side */
    		fread(&guard,sizeof(uint32_t),1,fid);
    		/* read in search window size */
    		fread(&winsize,sizeof(uint32_t),1,fid);

			/*
    		printf("fftsize=%d, lskip=%d, rskip=%d, guard=%d, winsize=%d\n", fftSizeArray[ii],lskip,rskip,guard,winsize);
    		*/
			
    		/* read in ca average reference */
    		fread(refBuf,sizeof(float),fftSizeArray[ii]-lskip-rskip,fid);
    		/* call lib ca average */
    		numDetected = mmwavelib_cfarfloat_caall(inputPowerPtr,
    												fftSizeArray[ii],
    										        CFAR_CA , /* cell average */
													guard,
													winsize,
													lskip,
													rskip,
													0, /* threshold */
													clusterIndex,
													noiseVarBuf);

    		if (numDetected!=fftSizeArray[ii]-lskip-rskip)
    		{
    			testPASS=0;
    			printf("CA: fftsize=%d, lskip=%d, rskip=%d, numDetected=%d\n", fftSizeArray[ii],lskip,rskip,numDetected);
    			
    		}
    		/* check the result */
    		for (kk=0;kk<numDetected;kk++)
    		{
    			if ((noiseVarBuf[kk]-refBuf[clusterIndex[kk]-lskip]>MAXIMUM_ERROR) ||(noiseVarBuf[kk]-refBuf[clusterIndex[kk]-lskip]<-1.f*MAXIMUM_ERROR))
    			{
    				testPASS=0;
    				printf("CA:fftsize=%d, winsize=%d, kk=%d, ref=%f, lib=%f, diff=%f\n", fftSizeArray[ii],winsize,kk,refBuf[clusterIndex[kk]-lskip],noiseVarBuf[kk], noiseVarBuf[kk]-refBuf[clusterIndex[kk]-lskip] );

    			}
    		}

    		/* call lib ca average optimized */
    		numDetected = mmwavelib_cfarfloat_caall_opt(inputPowerPtr,
    		    										fftSizeArray[ii],
    		    										CFAR_CA , /* cell average */
    													guard,
    													winsize,
    													lskip,
    													rskip,
    													0, /* threshold */
    													clusterIndex,
    													noiseVarBuf);

    		 if (numDetected!=fftSizeArray[ii]-lskip-rskip)
    		 {
    			 testOPTPASS=0;
    		 	printf("CA-opt: fftsize=%d, lskip=%d, rskip=%d, numDetected=%d\n", fftSizeArray[ii],lskip,rskip,numDetected);
    		 }
    		 /* check the result */
    		 for (kk=0;kk<numDetected;kk++)
    		 {
    		 	if ((noiseVarBuf[kk]-refBuf[clusterIndex[kk]-lskip]>MAXIMUM_ERROR) ||(noiseVarBuf[kk]-refBuf[clusterIndex[kk]-lskip]<-1.f*MAXIMUM_ERROR))
    		 	{
    		 		testOPTPASS=0;
    		 		printf("CA-opt:fftsize=%d, winsize=%d, kk=%d, ref=%f, lib=%f, diff=%f\n", fftSizeArray[ii],winsize,kk,refBuf[clusterIndex[kk]-lskip],noiseVarBuf[kk], noiseVarBuf[kk]-refBuf[clusterIndex[kk]-lskip] );
    			}
    		}

    		 /* read in CA accumulation reference */
     		fread(refBuf,sizeof(float),numDetected,fid);

    		/* call lib CA accumulation */
    		numDetected = mmwavelib_cfarfloat_caall(inputPowerPtr,
    		    									fftSizeArray[ii],
    		    									CFAR_CACC , /* cell accumulation */
    												guard,
    												winsize,
    												lskip,
    												rskip,
    												0, /* threshold  */
    												clusterIndex,
    												noiseVarBuf);

    		if (numDetected!=fftSizeArray[ii]-lskip-rskip)
    		{
    			testPASS=0;
    			printf("CACC: fftsize=%d, lskip=%d, rskip=%d, numDetected=%d\n", fftSizeArray[ii],lskip,rskip,numDetected);
    		}
    		/* check the result */
    		for (kk=0;kk<numDetected;kk++)
    		{
    			if ((noiseVarBuf[kk]-refBuf[clusterIndex[kk]-lskip]>MAXIMUM_ERROR) ||(noiseVarBuf[kk]-refBuf[clusterIndex[kk]-lskip]<-1.f*MAXIMUM_ERROR))
    			{
    				testPASS=0;
    		 		printf("CACC: fftsize=%d, winsize=%d, kk=%d, ref=%f, lib=%f, diff=%f\n", fftSizeArray[ii],winsize,kk,refBuf[clusterIndex[kk]-lskip],noiseVarBuf[kk], noiseVarBuf[kk]-refBuf[clusterIndex[kk]-lskip] );
    			}
    		}
    		
    		/* call lib CA accumulation optimized */
    	    numDetected = mmwavelib_cfarfloat_caall_opt(inputPowerPtr,
    		    									fftSizeArray[ii],
    		    									CFAR_CACC , /* cell accumulation */
    												guard,
    												winsize,
    												lskip,
    												rskip,
    												0, /* threshold */
    												clusterIndex,
    												noiseVarBuf);

    		if (numDetected!=fftSizeArray[ii]-lskip-rskip)
    		{
    			testOPTPASS=0;
    			printf("CACC-opt: fftsize=%d, lskip=%d, rskip=%d, numDetected=%d\n", fftSizeArray[ii],lskip,rskip,numDetected);
    		}
    		 /* check the result */
    		 for (kk=0;kk<numDetected;kk++)
    		 {
    		 	if ((noiseVarBuf[kk]-refBuf[clusterIndex[kk]-lskip]>MAXIMUM_ERROR) ||(noiseVarBuf[kk]-refBuf[clusterIndex[kk]-lskip]<-1.f*MAXIMUM_ERROR))
    		 	{
    		 		testOPTPASS=0;
    		  		printf("CACC-opt: fftsize=%d, winsize=%d, kk=%d, ref=%f, lib=%f, diff=%f\n", fftSizeArray[ii],winsize,kk,refBuf[clusterIndex[kk]-lskip],noiseVarBuf[kk], noiseVarBuf[kk]-refBuf[clusterIndex[kk]-lskip] );
    		    }
    		 }
    		 
    		 /* read in CFAR SO reference */
    		 fread(refBuf,sizeof(float),numDetected,fid);

    		/* call lib SO */
    		numDetected = mmwavelib_cfarfloat_caall(inputPowerPtr,
    		    									fftSizeArray[ii],
    		    		    						CFAR_CASO , /* so  */
    		    									guard,
    		    									winsize,
    		    									lskip,
    		    									rskip,
    		    									0, /* threshold */
    		    									clusterIndex,
    		    									noiseVarBuf);

    		if (numDetected!=fftSizeArray[ii]-lskip-rskip)
    		{
    			testPASS=0;
    			printf("SO: fftsize=%d, lskip=%d, rskip=%d, numDetected=%d\n", fftSizeArray[ii],lskip,rskip,numDetected);
    		}
    		
    		/* check the result */
    		for (kk=0;kk<numDetected;kk++)
    		{
    			if ((noiseVarBuf[kk]-refBuf[clusterIndex[kk]-lskip]>MAXIMUM_ERROR) ||(noiseVarBuf[kk]-refBuf[clusterIndex[kk]-lskip]<-1.f*MAXIMUM_ERROR))
    			{
    				testPASS=0;
    		    	printf("CACC: fftsize=%d, winsize=%d, kk=%d, ref=%f, lib=%f, diff=%f\n", fftSizeArray[ii],winsize,kk,refBuf[clusterIndex[kk]-lskip],noiseVarBuf[kk], noiseVarBuf[kk]-refBuf[clusterIndex[kk]-lskip] );
    		    }
    		}

    		/* call lib SO opt */
    		numDetected = mmwavelib_cfarfloat_caall_opt(inputPowerPtr,
    		    		    							fftSizeArray[ii],
    		    		    		    				CFAR_CASO , /* so  */
    		    		    							guard,
    		    		    							winsize,
    		    		    							lskip,
    		    		    							rskip,
    		    		    							0, /* threshold */
    		    		    							clusterIndex,
    		    		    							noiseVarBuf);

    		if (numDetected!=fftSizeArray[ii]-lskip-rskip)
    		{
    			testOPTPASS=0;
    			printf("SO-opt: fftsize=%d, lskip=%d, rskip=%d, numDetected=%d\n", fftSizeArray[ii],lskip,rskip,numDetected);
    		}
    		    		
    		 /* check the result */
    		 for (kk=0;kk<numDetected;kk++)
    		 {
    		 	if ((noiseVarBuf[kk]-refBuf[clusterIndex[kk]-lskip]>MAXIMUM_ERROR) ||(noiseVarBuf[kk]-refBuf[clusterIndex[kk]-lskip]<-1.f*MAXIMUM_ERROR))
    		 	{
    		 		testOPTPASS=0;
    		     	printf("CACC: fftsize=%d, winsize=%d, kk=%d, ref=%f, lib=%f, diff=%f\n", fftSizeArray[ii],winsize,kk,refBuf[clusterIndex[kk]-lskip],noiseVarBuf[kk], noiseVarBuf[kk]-refBuf[clusterIndex[kk]-lskip] );
    		     }
    		 }

    		 /* read in GO reference  */
    		 fread(refBuf,sizeof(float),numDetected,fid);   		
    		/* call lib GO accumulation */
    		numDetected = mmwavelib_cfarfloat_caall(inputPowerPtr,
    												fftSizeArray[ii],
    		    		    						CFAR_CAGO , 
    		    									guard,
    		    									winsize,
    		    									lskip,
    		    									rskip,
    		    									0, /* threshold */
    		    									clusterIndex,
    		    									noiseVarBuf);

    		if (numDetected!=fftSizeArray[ii]-lskip-rskip)
    		{
    			testPASS=0;
    			printf("GO: fftsize=%d, lskip=%d, rskip=%d, numDetected=%d\n", fftSizeArray[ii],lskip,rskip,numDetected);
    		}
    		
    		/* check the result */
    		for (kk=0;kk<numDetected;kk++)
    		{
    			if ((noiseVarBuf[kk]-refBuf[clusterIndex[kk]-lskip]>MAXIMUM_ERROR) ||(noiseVarBuf[kk]-refBuf[clusterIndex[kk]-lskip]<-1.f*MAXIMUM_ERROR))
    			{
    				testPASS=0;
    		    	printf("GO: fftsize=%d, winsize=%d, kk=%d, ref=%f, lib=%f, diff=%f\n", fftSizeArray[ii],winsize,kk,refBuf[clusterIndex[kk]-lskip],noiseVarBuf[kk], noiseVarBuf[kk]-refBuf[clusterIndex[kk]-lskip] );
    		    }
    		}
    		
    		
    		numDetected = mmwavelib_cfarfloat_caall_opt(inputPowerPtr,
    		    										fftSizeArray[ii],
    		    		    		    				CFAR_CAGO , 
    		    		    							guard,
    		    		    							winsize,
    		    		    							lskip,
    		    		    							rskip,
    		    		    							0, /* threshold */
    		    		    							clusterIndex,
    		    		    							noiseVarBuf);

    		if (numDetected!=fftSizeArray[ii]-lskip-rskip)
    		{
    			testOPTPASS=0;
    			printf("GO-OPT: fftsize=%d, lskip=%d, rskip=%d, numDetected=%d\n", fftSizeArray[ii],lskip,rskip,numDetected);
    		}
    		    		
    		 /* check the result */
    		 for (kk=0;kk<numDetected;kk++)
    		 {
    		 	if ((noiseVarBuf[kk]-refBuf[clusterIndex[kk]-lskip]>MAXIMUM_ERROR) ||(noiseVarBuf[kk]-refBuf[clusterIndex[kk]-lskip]<-1.f*MAXIMUM_ERROR))
    		 	{
    		 		testOPTPASS=0;
    		     	printf("GO-opt: fftsize=%d, winsize=%d, kk=%d, ref=%f, lib=%f, diff=%f\n", fftSizeArray[ii],winsize,kk,refBuf[clusterIndex[kk]-lskip],noiseVarBuf[kk], noiseVarBuf[kk]-refBuf[clusterIndex[kk]-lskip] );
    		    }
    		 }
    	}


    }

    if (testPASS)
    	MCPI_setFeatureTestResult("The cfar-CA, CACC, SO, GO with noise copy, noise variance calculation test", MCPI_TestResult_PASS);
    else
    	MCPI_setFeatureTestResult("The cfar-CA, CACC, SO, GO with noise copy, noise variance calculation test", MCPI_TestResult_FAIL);

    if (testOPTPASS)
       	MCPI_setFeatureTestResult("The cfar-CA, CACC, SO, GO with noise copy, less cycles, noise variance calculation test", MCPI_TestResult_PASS);
    else
      	MCPI_setFeatureTestResult("The cfar-CA, CACC, SO, GO with noise copy, less cycles, noise variance calculation test", MCPI_TestResult_FAIL);

    
    fclose(fid);
    
    
    printf(".....benchmark for mmwavelib_cfarfloat_caall, CFAR-CA, CACC, SO, GO with noise copy .......\n");
    printf("fftsize, cycles\n");
    for (ii=0;ii<numTestFFTSize;ii++)
    {
    	t1=TSCL;
    	numDetected = mmwavelib_cfarfloat_caall(inputPowerPtr,
    	    		  							fftSizeArray[ii],
    	    		  							CFAR_CA , /* cell average */
    	    									4, /* guard */
    	    									16,/* search win */
												0, /* lskip */
												0, /* rskip */
												0, /* threshold */
    	    									clusterIndex,
    	    									noiseVarBuf);
    	t2=TSCL;
    	cycles = t2-t1;
    	printf("%d\t%d\n",fftSizeArray[ii],cycles);
    }
    
    
    printf(".....benchmark for mmwavelib_cfarfloat_caall_opt, CFAR-CA, CACC, SO, GO with noise copy, more optimized version .......\n");
    printf("fftsize, cycles\n");
    for (ii=0;ii<numTestFFTSize;ii++)
    {
       	t1=TSCL;
       	numDetected = mmwavelib_cfarfloat_caall_opt(inputPowerPtr,
       	    		  							fftSizeArray[ii],
       	    		  							CFAR_CA , /* cell average */
       	    									4, /* guard */
       	    									16,/* search win */
   												0, /* lskip */
   												0, /* rskip */
   												0, /* threshold */
       	    									clusterIndex,
       	    									noiseVarBuf);
       	t2=TSCL;
       	cycles = t2-t1;
       	printf("%d\t%d\n",fftSizeArray[ii],cycles);
    }
    return;
}

void test_cfar_wrap()
{
	uint32_t numTestFFTSize;
	uint32_t fftSizeArray[MAXNUMFFTSIZE] ;
	uint32_t numTestPerFFT;
	uint32_t ii,jj,kk;
	uint32_t guard,winsize;
	uint32_t numDetected;
	uint32_t peakDet;


	fid = fopen("..\\testdata\\cfarca_wrap_test.bin","rb");
	if (fid==NULL)
	{
	   	printf("File Open Error: cfarca_test_wrap.bin can not open\n");
	   	exit(3);
	}
	/* read in tested number of fftsize */
	fread(&numTestFFTSize, sizeof(uint32_t), 1, fid);
	/* read in tested fft size */
	fread(&fftSizeArray[0],sizeof(uint32_t), numTestFFTSize,fid);

	for (ii=0;ii<numTestFFTSize;ii++)
	{
		printf("test %d FFT Size = %d\n", ii, fftSizeArray[ii]);

		/* read in number of tests per fftsize */
	    fread(&numTestPerFFT,sizeof(uint32_t),1,fid);
	    /* read in the accumulated pwer array */
	    fread(inputPowerPtr,sizeof(float),fftSizeArray[ii],fid);
	    for (jj=0;jj<numTestPerFFT;jj++)
	    {
			/* read in guard size one side */
	    	fread(&guard,sizeof(uint32_t),1,fid);
	    	/* read in search window size */
	    	fread(&winsize,sizeof(uint32_t),1,fid);

			/*
	    	printf("fftsize=%d, guard=%d, winsize=%d\n", fftSizeArray[ii],guard,winsize);
			*/
			
	    	/* read in ca average reference */
	    	fread(refBuf,sizeof(float),fftSizeArray[ii],fid);
	    	/* call lib ca average */
	    	numDetected = mmwavelib_cfarfloat_wrap(inputPowerPtr,
	    											fftSizeArray[ii],
	    										    CFAR_CA , /* cell average */
													guard,
													winsize,
													0, /* threshold  */
													clusterIndex,
													noiseVarBuf);

	    	if (numDetected!=fftSizeArray[ii])
	    	{
	    		testPASS=0;
	    		printf("CA-wrap: fftsize=%d, numDetected=%d\n", fftSizeArray[ii],numDetected);
	    	}
	    	/* check the result */
	    	for (kk=0;kk<numDetected;kk++)
	    	{
	    		if ((noiseVarBuf[kk]-refBuf[kk]>MAXIMUM_ERROR) ||((noiseVarBuf[kk]-refBuf[kk])<-1.f*MAXIMUM_ERROR))
	    		{
	    			testPASS=0;
	    			printf("CA-wrap:fftsize=%d, winsize=%d, kk=%d, ref=%f, lib=%f, diff=%f\n", fftSizeArray[ii],winsize,kk,refBuf[kk],noiseVarBuf[kk], noiseVarBuf[kk]-refBuf[kk] );
    			}
	    	}

	    	//cycle less version
	    	numDetected = mmwavelib_cfarfloat_wrap_opt(inputPowerPtr,
	    		    								   fftSizeArray[ii],
	    		    								    CFAR_CA , /* cell average */
	    												guard,
	    												winsize,
	    												0, /* threshold */
	    												clusterIndex,
	    												noiseVarBuf);

	    	if (numDetected!=fftSizeArray[ii])
	    	{
	    		testOPTPASS=0;
	    		printf("CA-wrap-opt: fftsize=%d, numDetected=%d\n", fftSizeArray[ii],numDetected);
	    	}
	    	/* check the result */
	    	for (kk=0;kk<numDetected;kk++)
	    	{
	    		if ((noiseVarBuf[kk]-refBuf[clusterIndex[kk]]>MAXIMUM_ERROR) ||((noiseVarBuf[kk]-refBuf[clusterIndex[kk]])<-1.f*MAXIMUM_ERROR))
	    		{
	    			testOPTPASS=0;
	    		    printf("CA-wrap-opt:fftsize=%d, winsize=%d, kk=%d, ref=%f, lib=%f, diff=%f\n", fftSizeArray[ii],winsize,clusterIndex[kk],refBuf[clusterIndex[kk]],noiseVarBuf[kk], noiseVarBuf[kk]-refBuf[clusterIndex[kk]] );
	    	    }
	    	}

	    	/* single point version */
	    	for (kk = 0; kk < fftSizeArray[ii]; kk++)
	    	{
	    		peakDet = mmwavelib_cfarfloat_point(inputPowerPtr,
	    				 	 	 	 	   fftSizeArray[ii],
										   CFAR_CA , /* average */
										   guard,
										   winsize,
										   0, /* threshold */
										   kk, /* index  */
										   &noiseVarBuf[kk]);
	    		/* check result */
	    		if (!peakDet)
	    		{
	    			testPOINTPASS = 0;
	    			printf("CA-wrap-point: fftsize=%d, deteced = %d ", fftSizeArray[ii], peakDet);
	    		}

	    		if ((noiseVarBuf[kk]-refBuf[kk]>MAXIMUM_ERROR) ||((noiseVarBuf[kk]-refBuf[kk])<-1.f*MAXIMUM_ERROR))
	    		{
	    			testPOINTPASS = 0;
	    			printf("CA-wrap-point:fftsize=%d, winsize=%d, kk=%d, ref=%f, lib=%f, diff=%f\n", fftSizeArray[ii],winsize,kk,refBuf[kk],noiseVarBuf[kk], noiseVarBuf[kk]-refBuf[kk] );

				}
	    	}


	    	/* call ca wrap accumulation */
	    	fread(refBuf,sizeof(float),fftSizeArray[ii],fid);
	    	numDetected = mmwavelib_cfarfloat_wrap(inputPowerPtr,
	    										   fftSizeArray[ii],
	    										   CFAR_CACC , /* cell accumulation */
	    										   guard,
	    										   winsize,
	    										   0, /* threshold */
	    										   clusterIndex,
	    										   noiseVarBuf);

	    	if (numDetected!=fftSizeArray[ii])
	    	{
	    		 testPASS=0;
	    		 printf("CA-wrap: fftsize=%d, numDetected=%d\n", fftSizeArray[ii],numDetected);
	    	}
	    	/* check the result */
	    	for (kk=0;kk<numDetected;kk++)
	    	{
	    		if ((noiseVarBuf[kk]-refBuf[kk]>MAXIMUM_ERROR) ||((noiseVarBuf[kk]-refBuf[kk])<-1.f*MAXIMUM_ERROR))
	    		{
	    			testPASS=0;
	    			printf("CACC-wrap:fftsize=%d, winsize=%d, kk=%d, ref=%f, lib=%f, diff=%f\n", fftSizeArray[ii],winsize,kk,refBuf[kk],noiseVarBuf[kk], noiseVarBuf[kk]-refBuf[kk] );
	    		}
	    	}
	    	/* call cycle less version */
	    	numDetected = mmwavelib_cfarfloat_wrap_opt(inputPowerPtr,
	    		    								   fftSizeArray[ii],
	    		    								   CFAR_CACC , /* cell accumulation */
	    		    								   guard,
	    		    								   winsize,
	    		    									0, /* threshold */
	    		    								    clusterIndex,
	    		    									noiseVarBuf);

	    	if (numDetected!=fftSizeArray[ii])
	    	{
	    		testOPTPASS=0;
	    		 printf("CA-wrap-opt: fftsize=%d, numDetected=%d\n", fftSizeArray[ii],numDetected);
	    	}
	    	/* check the result */
	    	for (kk=0;kk<numDetected;kk++)
	    	{
	    		if ((noiseVarBuf[kk]-refBuf[clusterIndex[kk]]>MAXIMUM_ERROR) ||((noiseVarBuf[kk]-refBuf[clusterIndex[kk]])<-1.f*MAXIMUM_ERROR))
	    		{
	    			testOPTPASS=0;
	    		 	printf("CACC-wrap:fftsize=%d, winsize=%d, kk=%d, ref=%f, lib=%f, diff=%f\n", fftSizeArray[ii],winsize,clusterIndex[kk],refBuf[clusterIndex[kk]],noiseVarBuf[kk], noiseVarBuf[kk]-refBuf[clusterIndex[kk]] );
	    		 }
	    	}

	    	/* single point version */
	    	for (kk = 0; kk < fftSizeArray[ii]; kk++)
	    	{
	    		peakDet = mmwavelib_cfarfloat_point(inputPowerPtr,
	    		    				 	 	 	 	fftSizeArray[ii],
	    											CFAR_CACC , /* accumulation */
	    											guard,
	    											winsize,
	    											0, /* threshold */
	    											kk, /* index */
	    											&noiseVarBuf[kk]);
	    		  /* check result */
	    		  if (!peakDet)
	    		  {
	    			  testPOINTPASS = 0;
	    		   	  printf("CA-wrap-point: fftsize=%d, deteced = %d ", fftSizeArray[ii], peakDet);
	    		  }

	    		  if ((noiseVarBuf[kk]-refBuf[kk]>MAXIMUM_ERROR) ||((noiseVarBuf[kk]-refBuf[kk])<-1.f*MAXIMUM_ERROR))
	    		  {
	    			  testPOINTPASS = 0;
	    		      printf("CA-wrap-point:fftsize=%d, winsize=%d, kk=%d, ref=%f, lib=%f, diff=%f\n", fftSizeArray[ii],winsize,kk,refBuf[kk],noiseVarBuf[kk], noiseVarBuf[kk]-refBuf[kk] );

	    		  }
	    	}



	    	/* SO wrap */
	    	fread(refBuf,sizeof(float),fftSizeArray[ii],fid);
	    	numDetected = mmwavelib_cfarfloat_wrap(inputPowerPtr,
	    		    							   fftSizeArray[ii],
	    		    							   CFAR_CASO , /* SO */
												   guard,
	    										   winsize,
	    										   0, /* threshold */
	    										   clusterIndex,
	    										   noiseVarBuf);

	    	if (numDetected!=fftSizeArray[ii])
	    	{
	    		testPASS=0;
	    		printf("SO-wrap: fftsize=%d, numDetected=%d\n", fftSizeArray[ii],numDetected);
	    	}
	    	/* check the result */
	    	for (kk=0;kk<numDetected;kk++)
	    	{
	    		if ((noiseVarBuf[kk]-refBuf[kk]>MAXIMUM_ERROR) ||((noiseVarBuf[kk]-refBuf[kk])<-1.f*MAXIMUM_ERROR))
	    		{
	    			testPASS=0;
	    			printf("SO-wrap:fftsize=%d, winsize=%d, kk=%d, ref=%f, lib=%f, diff=%f\n", fftSizeArray[ii],winsize,kk,refBuf[kk],noiseVarBuf[kk], noiseVarBuf[kk]-refBuf[kk] );
	    	    }
	    	}
	    	/* SO cycle less version */
	    	numDetected = mmwavelib_cfarfloat_wrap_opt(inputPowerPtr,
	    		    		    					   fftSizeArray[ii],
	    		    		    					   CFAR_CASO, /* SO */
	    											   guard,
	    		    								   winsize,
	    		    								   0, /* threshold */
	    		    								   clusterIndex,
	    		    								   noiseVarBuf);

	       if (numDetected!=fftSizeArray[ii])
	       {
	    	   testOPTPASS=0;
	    	   printf("SO-wrap-opt: fftsize=%d, numDetected=%d\n", fftSizeArray[ii],numDetected);
	       }
	    	/* check the result */
	       	for (kk=0;kk<numDetected;kk++)
	    	{
	    		if ((noiseVarBuf[kk]-refBuf[clusterIndex[kk]]>MAXIMUM_ERROR) ||((noiseVarBuf[kk]-refBuf[clusterIndex[kk]])<-1.f*MAXIMUM_ERROR))
	    		{
	    			testOPTPASS=0;
	    			printf("SO-wrap:fftsize=%d, winsize=%d, kk=%d, ref=%f, lib=%f, diff=%f\n", fftSizeArray[ii],winsize,clusterIndex[kk],refBuf[clusterIndex[kk]],noiseVarBuf[kk], noiseVarBuf[kk]-refBuf[clusterIndex[kk]] );
	    		}
	    	}

	       	/* single point version */
	       	for (kk = 0; kk < fftSizeArray[ii]; kk++)
	       	{
	       		peakDet = mmwavelib_cfarfloat_point(inputPowerPtr,
	       	    				 	 	 	 	   fftSizeArray[ii],
	       										   CFAR_CASO , /* so */
	       										   guard,
	       										   winsize,
	       										   0, /* threshold */
	       										   kk, /* index */
	       										   &noiseVarBuf[kk]);
	       		 /* check result */
	       		 if (!peakDet)
	       		 {
	       		 	 testPOINTPASS = 0;
	       		     printf("SO-wrap-point: fftsize=%d, deteced = %d ", fftSizeArray[ii], peakDet);
	       		 }

	       		 if ((noiseVarBuf[kk]-refBuf[kk]>MAXIMUM_ERROR) ||((noiseVarBuf[kk]-refBuf[kk])<-1.f*MAXIMUM_ERROR))
	       		 {
	       		 	 testPOINTPASS = 0;
	       		     printf("SO-wrap-point:fftsize=%d, winsize=%d, kk=%d, ref=%f, lib=%f, diff=%f\n", fftSizeArray[ii],winsize,kk,refBuf[kk],noiseVarBuf[kk], noiseVarBuf[kk]-refBuf[kk] );

	       		}
	       	}

	    	/* GO wrap */
	    	fread(refBuf,sizeof(float),fftSizeArray[ii],fid);
	    	numDetected = mmwavelib_cfarfloat_wrap(inputPowerPtr,
	    		    							   fftSizeArray[ii],
	    		    							   CFAR_CAGO , /* GO */
	    										   guard,
	    										   winsize,
	    										   0, /* threshold */
	    										   clusterIndex,
	    										   noiseVarBuf);

	    	if (numDetected!=fftSizeArray[ii])
	    	{
	    		testPASS=0;
	    		printf("GO-wrap: fftsize=%d, numDetected=%d\n", fftSizeArray[ii],numDetected);
	    	}
	    	/* check the result */
	    	for (kk=0;kk<numDetected;kk++)
	    	{
	    		if ((noiseVarBuf[kk]-refBuf[kk]>MAXIMUM_ERROR) ||((noiseVarBuf[kk]-refBuf[kk])<-1.f*MAXIMUM_ERROR))
	    		{
	    			testPASS=0;
	    			printf("GO-wrap:fftsize=%d, winsize=%d, kk=%d, ref=%f, lib=%f, diff=%f\n", fftSizeArray[ii],winsize,kk,refBuf[kk],noiseVarBuf[kk], noiseVarBuf[kk]-refBuf[kk] );
	    	    }
	    	}

	    	/*  call cycle less version */
	    	numDetected = mmwavelib_cfarfloat_wrap_opt(inputPowerPtr,
	    		    		    					   fftSizeArray[ii],
	    		    		    					   CFAR_CAGO , /* GO */
	    		    								   guard,
	    		    								   winsize,
	    		    								   0, /* threshold */
	    		    								   clusterIndex,
	    		    								   noiseVarBuf);

	    	if (numDetected!=fftSizeArray[ii])
	    	{
	    		testOPTPASS=0;
	    		printf("GO-wrap-opt: fftsize=%d, numDetected=%d\n", fftSizeArray[ii],numDetected);
	    	}
	    	/* check the result */
	    	for (kk=0;kk<numDetected;kk++)
	    	{
	    		if ((noiseVarBuf[kk]-refBuf[clusterIndex[kk]]>MAXIMUM_ERROR) ||((noiseVarBuf[kk]-refBuf[clusterIndex[kk]])<-1.f*MAXIMUM_ERROR))
	    		{
	    			testOPTPASS=0;
	    			printf("GO-wrap-opt:fftsize=%d, winsize=%d, kk=%d, ref=%f, lib=%f, diff=%f\n", fftSizeArray[ii],winsize,clusterIndex[kk],refBuf[clusterIndex[kk]],noiseVarBuf[kk], noiseVarBuf[kk]-refBuf[clusterIndex[kk]] );
	    		}
	    	}

	    	/* single point version */
	    	for (kk = 0; kk < fftSizeArray[ii]; kk++)
	    	{
	    		peakDet = mmwavelib_cfarfloat_point(inputPowerPtr,
	    		    				 	 	 	 	fftSizeArray[ii],
	    											CFAR_CAGO , /* GO, */
	    											guard,
	    											winsize,
	    											0, /* threshold */
	    											kk, /* index */
	    											&noiseVarBuf[kk]);
	    		/* check result */
	    		if (!peakDet)
	    		{
	    			testPOINTPASS = 0;
	    			printf("GO-wrap-point: fftsize=%d, deteced = %d ", fftSizeArray[ii], peakDet);
	    		}

	    		if ((noiseVarBuf[kk]-refBuf[kk]>MAXIMUM_ERROR) ||((noiseVarBuf[kk]-refBuf[kk])<-1.f*MAXIMUM_ERROR))
	    		{
	    		    testPOINTPASS = 0;
	    		    printf("GO-wrap-point:fftsize=%d, winsize=%d, kk=%d, ref=%f, lib=%f, diff=%f\n", fftSizeArray[ii],winsize,kk,refBuf[kk],noiseVarBuf[kk], noiseVarBuf[kk]-refBuf[kk] );

	    		}
	    	}
	    }
	}

	 if (testPASS)
	 {
	   	MCPI_setFeatureTestResult("The cfar-CA, CACC, SO, GO with noise wrap, noise variance calculation test", MCPI_TestResult_PASS);
	 }
	else
	{
	  	MCPI_setFeatureTestResult("The cfar-CA, CACC, SO, GO with noise wrap, noise variance calculation test", MCPI_TestResult_FAIL);
	}

	 if (testOPTPASS)
	 {
		 MCPI_setFeatureTestResult("The cfar-CA, CACC, SO, GO with noise wrap, less cycles, noise variance calculation test", MCPI_TestResult_PASS);
	 }
	 else
	 {
		 MCPI_setFeatureTestResult("The cfar-CA, CACC, SO, GO with noise wrap, less cycles, noise variance calculation test", MCPI_TestResult_FAIL);
	 }

	 if (testPOINTPASS)
	 {
	 	MCPI_setFeatureTestResult("The cfar-CA, CACC, SO, GO with noise wrap, single point, noise variance calculation test", MCPI_TestResult_PASS);
 	 }
	 else
 	 {
		 MCPI_setFeatureTestResult("The cfar-CA, CACC, SO, GO with noise wrap, single point, noise variance calculation test", MCPI_TestResult_FAIL);
	 }

	  printf(".....benchmark for mmwavelib_cfarfloat_wrap, CFAR-CA, CACC, SO, GO with noise wrap .......\n");
	  printf("fftsize, cycles\n");
	  for (ii=0;ii<numTestFFTSize;ii++)
	  {
		  t1=TSCL;
	      numDetected = mmwavelib_cfarfloat_wrap(inputPowerPtr,
	    	    		  						fftSizeArray[ii],
	    	    		  						CFAR_CA , /* cell average */
	    	    								4, /* guard */
	    	    								16,/* search win */
												0, /* threshold */
	    	    								clusterIndex,
	    	    								noiseVarBuf);
	    	t2=TSCL;
	    	cycles = t2-t1;
	    	printf("%d\t%d\n",fftSizeArray[ii],cycles);
	   }

	  printf(".....benchmark for mmwavelib_cfarfloat_wrap_opt, CFAR-CA, CACC, SO, GO with noise wrap, more optimized version .......\n");
	  printf("fftsize, cycles\n");

	  for (ii=0;ii<numTestFFTSize;ii++)
	  {
	  	  t1=TSCL;
	      numDetected = mmwavelib_cfarfloat_wrap_opt(inputPowerPtr,
	   	    		  							fftSizeArray[ii],
	   	    		  							CFAR_CA , /* cell average */
	   	    									4, /* guard */
	   	    									16,/* search win */
	  											0, /* threshold */
	   	    									clusterIndex,
	   	    									noiseVarBuf);
	      t2=TSCL;
	  	  cycles = t2-t1;
	  	  printf("%d\t%d\n",fftSizeArray[ii],cycles);
	 }

	  printf(".....benchmark for mmwavelib_cfarfloat_point, CFAR-CA, CACC, SO, GO with noise wrap, single point .......\n");
	  printf("1 point, cycles\n");
  	  t1=TSCL;
  	  peakDet = mmwavelib_cfarfloat_point(inputPowerPtr,
  		    		    				  1024,
  		    							  CFAR_CA , /* cell average, */
  		    							  4, /* guard, */
  		    							  16, /* winsize, */
  		    							  0, /* threshold */
  		    							  512, /* index */
  		    							  &noiseVarBuf[0]);
      t2=TSCL;
  	  cycles = t2-t1;
  	  printf("%d\n",cycles);


}

void check_detpeak(uint32_t numDetected,
				   uint32_t numPeakRef,
				   uint32_t order,
				   uint32_t *index,
				   uint32_t *peakIdxRef,
				   const char * feature)
{
	uint32_t jj,kk,found;
	
	if (numDetected != numPeakRef)
	{
		testPEAKPASS = 0;
		printf("cfar %s fail at peak detection tests\n", feature);
	}

	if (order)
	{
		for (jj = 0; jj < numDetected; jj++)
		{
			/* index[jj] */
			found = 0;
			for (kk = 0; kk < numPeakRef; kk++)
			{
				if (index[jj]==peakIdxRef[kk])
				{
					found = 1;
					break;
				}
			}
			if (!found)
			{
				testPEAKPASS = 0;
				printf("cfar %s at peak detection tests\n",feature);
			}
		}
	}
	else
	{
		for (jj = 0; jj < numDetected; jj++)
		{
			if (index[jj]!=peakIdxRef[jj])
			{
				testPEAKPASS = 0;
				printf("cfar %s fail at peak detection tests\n",feature);
			}
		}
	}
}

void test_cfar_peak()
{
	uint32_t numTestFFTSize;
	uint32_t fftSizeArray[MAXNUMFFTSIZE] ;
	uint32_t ii,jj;
	uint32_t numDetected;
	uint32_t numPeakRef, peakIdxRef[10]; /* maximum 10 peak in the test vectors */
	uint32_t lskip, rskip, guard, win;
	uint8_t peakFlag;
	

	fid = fopen("..\\testdata\\cfarca_peak_test.bin","rb");
	if (fid==NULL)
	{
	   	printf("File Open Error: cfarca_peak_test.bin can not open\n");
	   	exit(3);
	}
	/* read in tested number of fftsize */
	fread(&numTestFFTSize, sizeof(uint32_t), 1, fid);
	/* read in tested fft size */
	fread(&fftSizeArray[0],sizeof(uint32_t), numTestFFTSize,fid);

	
	for (ii = 0; ii < numTestFFTSize; ii++)
	{
		printf("test %d FFT Size = %d\n", ii, fftSizeArray[ii]);

		fread(&lskip,sizeof(uint32_t),1,fid);
		fread(&rskip,sizeof(uint32_t),1,fid);
		fread(&guard,sizeof(uint32_t),1,fid);
		fread(&win,sizeof(uint32_t),1,fid);
		fread(inputPowerPtr,sizeof(float),fftSizeArray[ii],fid);
		fread(&numPeakRef, sizeof(uint32_t),1,fid);
		fread(peakIdxRef,sizeof(uint32_t),numPeakRef,fid);

		/* call cfar noise copy, cell average */
		numDetected = mmwavelib_cfarfloat_caall(inputPowerPtr,
												fftSizeArray[ii],
												CFAR_CA , /* cell average */
												guard,
												win,
												lskip,
												rskip,
												7.f,
												clusterIndex,
												noiseVarBuf);

		check_detpeak(numDetected,
					 numPeakRef,
					 0, /* inorder */
					 clusterIndex,
					 peakIdxRef,
					 "CFAR-CA");
		
		numDetected = mmwavelib_cfarfloat_caall(inputPowerPtr,
												fftSizeArray[ii],
												CFAR_CACC , /* cell accumulation */
												guard,
												win,
												lskip,
												rskip,
												7.f/2.f/win,
												clusterIndex,
												noiseVarBuf);

		check_detpeak(numDetected,
					  numPeakRef,
					  0, 
					  clusterIndex,
					  peakIdxRef,
					 "CFAR-CACC");

		numDetected = mmwavelib_cfarfloat_caall(inputPowerPtr,
												fftSizeArray[ii],
												CFAR_CASO , /* so */
												guard,
												win,
												lskip,
												rskip,
												14.f,
												clusterIndex,
												noiseVarBuf);

		check_detpeak(numDetected,
				 numPeakRef,
				 0, 
				 clusterIndex,
				 peakIdxRef,
				 "CFAR-SO");
		
		numDetected = mmwavelib_cfarfloat_caall(inputPowerPtr,
												fftSizeArray[ii],
												CFAR_CAGO , /* go */
												guard,
												win,
												lskip,
												rskip,
												7.f,
												clusterIndex,
												noiseVarBuf);

		check_detpeak(numDetected,
					  numPeakRef,
					  0, 
					 clusterIndex,
					  peakIdxRef,
					 "CFAR-GO");
		
		numDetected = mmwavelib_cfarfloat_caall_opt(inputPowerPtr,
												fftSizeArray[ii],
												CFAR_CA , /* ave */
												guard,
												win,
												lskip,
												rskip,
												7.f,
												clusterIndex,
												noiseVarBuf);

		check_detpeak(numDetected,
					  numPeakRef,
					  1, /* out of order */
					  clusterIndex,
					  peakIdxRef,
					  "CFAR-CA-OPT");
				
		numDetected = mmwavelib_cfarfloat_caall_opt(inputPowerPtr,
													fftSizeArray[ii],
													CFAR_CACC , /* acc */
													guard,
													win,
													lskip,
													rskip,
													7.f/2.f/win,
													clusterIndex,
													noiseVarBuf);
		check_detpeak(numDetected,
					  numPeakRef,
					  1, 
					  clusterIndex,
					  peakIdxRef,
					  "CFAR-CACC-OPT");

		numDetected = mmwavelib_cfarfloat_caall_opt(inputPowerPtr,
													fftSizeArray[ii],
													CFAR_CASO , /* so */
													guard,
													win,
													lskip,
													rskip,
													14.f,
													clusterIndex,
													noiseVarBuf);
		check_detpeak(numDetected,
					  numPeakRef,
					  1,
					  clusterIndex,
					  peakIdxRef,
					  "CFAR-SO-OPT");
		numDetected = mmwavelib_cfarfloat_caall_opt(inputPowerPtr,
													fftSizeArray[ii],
													CFAR_CAGO , /* go */
													guard,
													win,
													lskip,
													rskip,
													7.f,
													clusterIndex,
													noiseVarBuf);

		check_detpeak(numDetected,
					  numPeakRef,
					  1, 
					  clusterIndex,
					  peakIdxRef,
					  "CFAR-GO-OPT");
		
		/* noise wrap version */
		numDetected = mmwavelib_cfarfloat_wrap(inputPowerPtr,
											   fftSizeArray[ii],
											   CFAR_CA , /* average */
											   guard,
											   win,
											   7.f,
											   clusterIndex,
											   noiseVarBuf);
		check_detpeak(numDetected,
					  numPeakRef,
					  0,
					  clusterIndex,
					  peakIdxRef,
					  "CFAR-CA-WRAP");
		
		numDetected = mmwavelib_cfarfloat_wrap(inputPowerPtr,
											   fftSizeArray[ii],
											   CFAR_CACC , /* accumulation */
											   guard,
												win,
												7.f/2.f/win,
											    clusterIndex,
												 noiseVarBuf);
		check_detpeak(numDetected,
					  numPeakRef,
					  0,
					  clusterIndex,
					  peakIdxRef,
					  "CFAR-CACC-WRAP");
		
		
		numDetected = mmwavelib_cfarfloat_wrap(inputPowerPtr,
											   fftSizeArray[ii],
											   CFAR_CASO , /* so */
											   guard,
												win,
												14.f,
												clusterIndex,
												noiseVarBuf);
		check_detpeak(numDetected,
					  numPeakRef,
					  0,
					  clusterIndex,
					  peakIdxRef,
					  "CFAR-SO-WRAP");
		
		
		numDetected = mmwavelib_cfarfloat_wrap(inputPowerPtr,
											   fftSizeArray[ii],
											   CFAR_CAGO , /* GO */
											   guard,
												win,
												7.f,
												clusterIndex,
												noiseVarBuf);
		check_detpeak(numDetected,
					  numPeakRef,
					  0,
					  clusterIndex,
					  peakIdxRef,
					  "CFAR-GO-WRAP");
		
		
		numDetected = mmwavelib_cfarfloat_wrap_opt(inputPowerPtr,
												   fftSizeArray[ii],
												   CFAR_CA ,/* average */
												   guard,
												   win,
												   7.f,
												   clusterIndex,
												   noiseVarBuf);
		
		check_detpeak(numDetected,
					  numPeakRef,
					  1,
					  clusterIndex,
					  peakIdxRef,
					  "CFAR-CAAV-WRAP-OPT");
		
		numDetected = mmwavelib_cfarfloat_wrap_opt(inputPowerPtr,
												   fftSizeArray[ii],
												   CFAR_CACC , /*  accumulation */
												   guard,
												   win,
												   7.f/2.f/win,
												   clusterIndex,
													noiseVarBuf);
				
		check_detpeak(numDetected,
					  numPeakRef,
					  1,
					  clusterIndex,
					  peakIdxRef,
					  "CFAR-ACC-WRAP-OPT");
		
		numDetected = mmwavelib_cfarfloat_wrap_opt(inputPowerPtr,
												   fftSizeArray[ii],
												   CFAR_CASO ,/* SO */
												   guard,
												   win,
												   14.f,
												   clusterIndex,
												   noiseVarBuf);
						
		check_detpeak(numDetected,
					  numPeakRef,
					  1,
					  clusterIndex,
					  peakIdxRef,
					  "CFAR-SO-WRAP-OPT");
		
		
		numDetected = mmwavelib_cfarfloat_wrap_opt(inputPowerPtr,
												   fftSizeArray[ii],
												   CFAR_CAGO , /* GO */
												   guard,
												   win,
												   7.f,
												   clusterIndex,
													noiseVarBuf);
								
		check_detpeak(numDetected,
					  numPeakRef,
					  1,
					  clusterIndex,
					  peakIdxRef,
					  "CFAR-GO-WRAP-OPT");
				
		/* check single point detection */
		for (jj = 0; jj < numDetected; jj++)
		{
			peakFlag = mmwavelib_cfarfloat_point(inputPowerPtr,
												fftSizeArray[ii],
												CFAR_CA ,  /* average */
												guard,
												win,
												7.f,
												clusterIndex[jj],
												&noiseVarBuf[jj]);
			
			if (!peakFlag)
			{
				testPEAKPASS = 0;
				printf("cfar ave single point detection fails at peak detection tests\n");
			}
			
			peakFlag = mmwavelib_cfarfloat_point(inputPowerPtr,
												 fftSizeArray[ii],
												 CFAR_CACC , /* accumulation */
												 guard,
											  	 win,
												 7.f/2.f/win,
												 clusterIndex[jj],
												&noiseVarBuf[jj]);
						
			if (!peakFlag)
			{
				testPEAKPASS = 0;
				printf("cfar acc single point detection fails at peak detection tests\n");
						
			}
			
			
			peakFlag = mmwavelib_cfarfloat_point(inputPowerPtr,
												 fftSizeArray[ii],
												 CFAR_CASO , /* SO */
												 guard,
												 win,
												 14.f,
												 clusterIndex[jj],
												&noiseVarBuf[jj]);
									
			if (!peakFlag)
			{
				testPEAKPASS = 0;
				printf("cfar SO single point detection fails at peak detection tests\n");
						
			}
				
			
			peakFlag = mmwavelib_cfarfloat_point(inputPowerPtr,
												 fftSizeArray[ii],
												 CFAR_CAGO ,  /* GO */
												 guard,
												 win,
												 7.f,
												 clusterIndex[jj],
												&noiseVarBuf[jj]);
												
			if (!peakFlag)
			{
				testPEAKPASS = 0;
				printf("cfar GO single point detection fails at peak detection tests\n");
									
			}
			
		}
		
		
				
				
				
	}

	if (testPEAKPASS)
	{
	 	MCPI_setFeatureTestResult("The CFAR all version peak detection test", MCPI_TestResult_PASS);
	}
	else
	{
		MCPI_setFeatureTestResult("The CFAR all version peak detection test", MCPI_TestResult_FAIL);
	}

}
void main(void)
{

	MCPI_Initialize();
	TSCL = 0;

	printf(".....Test noise variance calculation for CFAR-CA, CACC, SO, GO with noise copy at the edge .......\n");
	test_cfar_caall();
	printf(".....Test noise variance calculation for CFAR-CA, CACC, SO, GO with noise wrap at the edge .......\n");
	test_cfar_wrap();
	printf(".....Test CFAR noise copy and noise wrap, CA, CACC, SO, GO, peak detection .......\n");
	test_cfar_peak();
	
	if ((testPASS) && (testOPTPASS) && (testPOINTPASS) &&(testPEAKPASS))
	{
		printf("========== Floating point CFAR CA,CACC, SO, GO tests passed! ========== \n");
	}
	else
	{
		printf("==========  Floating point CFAR CA,CACC, SO, GO tests Failed! ========== \n");
	}
	printf("========== Testing completed! ==========\n");

	MCPI_setTestResult ();
}
