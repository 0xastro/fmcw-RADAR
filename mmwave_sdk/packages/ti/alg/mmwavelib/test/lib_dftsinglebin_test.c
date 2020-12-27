/*
 *   @file  lib_dftsinglebin_test.c
 *
 *   @brief
 *   Unit Test codes for single bin DFT related mmwavelib library functions
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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
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

#define ONE_Q15 (1<<15)
#define PI 3.1415926
#define MAXIMUM_ERROR 108.f
#define MAXIMUM_SINCOS_ERROR 1

uint32_t w_startTime1;
uint32_t  cycles;

#define MAX_DFT_SIZE  1024
int16_t dftSinCosRef[MAX_DFT_SIZE * 2]; 
uint32_t dftSinCosLib[MAX_DFT_SIZE];


#pragma DATA_SECTION(inputBuf,".l2data");
#pragma DATA_ALIGN( inputBuf,8);
int32_t  inputBuf[128];  /* fftinput, 16 bit real and 16 bit imag */
#pragma DATA_SECTION(outputBuf,".l2data");
#pragma DATA_ALIGN( outputBuf,8);
int32_t outputBuf[128*2]; /* fft output */

#define ROUND(x) ((x) < 0 ? ((x) - 0.5) : ((x) + 0.5) )
#define PI_ 3.14159265

/* int16 in Q15 format, imaginary in even, real in odd location */
void gen_dftSinCosTable(uint32_t dftLength, int16_t *dftSinCosTable)
{
    uint32_t i;
    int32_t itemp;
    float temp;

    for (i = 0; i < dftLength; i++)
    {
        temp = ONE_Q15 * -sin(2*PI*i/dftLength);
        if (temp > 0)
        {
            temp = temp + 0.5;
        }
        else
        {
            temp = temp -0.5;
        }
        itemp = (int32_t) temp;

        if(itemp >= ONE_Q15)
        {
            itemp = ONE_Q15 - 1;
        }
        dftSinCosTable[2*i] =  (int16_t)itemp; /* imaginary in even */

        temp = ONE_Q15 * cos(2*PI*i/dftLength);
        if (temp > 0)
        {
            temp = temp + 0.5;
        }
        else
        {
            temp = temp -0.5;
        }
        itemp = (int32_t) temp;

        if(itemp >= ONE_Q15)
        {
            itemp = ONE_Q15 - 1;
        }
        dftSinCosTable[2*i+1] = (int16_t) itemp; /* real in odd */
    }
}


void calculate_dft(int32_t *input, uint8_t dftsize, int32_t *output,float scale)
{
	int32_t  nn;
	int32_t  kk;
	float inSampRe,inSampIm;
	float outSampRe,outSampIm;
	double angle;
	/*
	double anglecos,anglesin;
	*/
	double pi = 3.1415926535897f;

	/* scale = 1.f; // no scale */
	for(nn=0; nn<dftsize; nn++)
	{
		outSampRe = 0;
	    outSampIm = 0;
	    for(kk=0; kk<dftsize; kk++)
	    {
	    	inSampRe = (float) ((int16_t)(input[kk]&0xFFFF)); /* low 16 bits is real */
	    	inSampIm = (float) ((int16_t)((input[kk]>>16)&0xFFFF)); /* high 16 bits is imag */
	        angle = -2*pi*nn*kk/dftsize;
			/*
			anglecos = (float)cos(angle);
			anglesin = (float)sin(angle);
		   */
	        outSampRe = outSampRe + inSampRe*(float)cos(angle) - inSampIm*(float)sin(angle);
	        outSampIm = outSampIm + inSampIm*(float)cos(angle) + inSampRe*(float)sin(angle);

	    }
	    outSampRe  = scale*outSampRe;
	    if (outSampRe>0)
		{
	    	outSampRe = outSampRe + 0.5;
		}
	    else
		{
	    	outSampRe = outSampRe - 0.5;
		}
	    output[2*nn] = (int32_t )outSampRe;

	    outSampIm = scale*outSampIm;

	    if (outSampIm>0)
		{
	    	outSampIm = outSampIm + 0.5;
		}
	    else
		{
	    	outSampIm = outSampIm - 0.5;
		}


	    output[2*nn+1] = (int32_t)outSampIm;

	}
}



void calculate_dft_withwin(int32_t *input, uint8_t IQflag, int32_t * win, uint8_t dftsize, int32_t *output,float scale)
{
	int32_t  nn;
	int32_t  kk;
	float inSampRe,inSampIm;
	float outSampRe,outSampIm;
	double angle;
	/*
	double anglecos,anglesin;
	*/
	double pi = 3.1415926535897f;

	/* scale = 1.f; // no scale */
	for(nn=0; nn<dftsize; nn++)
	{
		outSampRe = 0;
	    outSampIm = 0;
	    for(kk=0; kk<dftsize; kk++)
	    {
            if (IQflag == 0)   /* ImRe*/
            {
                inSampRe = (float) ((int16_t)(input[kk]&0xFFFF)); /* low 16 bits is real */
                inSampIm = (float) ((int16_t)((input[kk]>>16)&0xFFFF)); /* high 16 bits is imag */
            }
            else  /* ReIm*/
            {
                inSampIm = (float) ((int16_t)(input[kk]&0xFFFF)); /* low 16 bits is imag*/
                inSampRe = (float) ((int16_t)((input[kk]>>16)&0xFFFF)); /* high 16 bits is real */
            }
	    	
	    	/* window */
	    	if (kk<dftsize/2)
	    	{
				inSampRe  = inSampRe * win[kk];
				inSampIm  = inSampIm * win[kk];
	    	}
	    	else
	    	{
	    		inSampRe  = inSampRe * win[dftsize - 1 - kk];
	    		inSampIm  = inSampIm * win[dftsize - 1 - kk];
	    	}
	    	inSampRe = inSampRe/32768.f;
	    	inSampIm = inSampIm/32768.f;
	    	
	        angle = -2*pi*nn*kk/dftsize;
			/*
			anglecos = (float)cos(angle);
			anglesin = (float)sin(angle);
		   */
	        outSampRe = outSampRe + inSampRe*(float)cos(angle) - inSampIm*(float)sin(angle);
	        outSampIm = outSampIm + inSampIm*(float)cos(angle) + inSampRe*(float)sin(angle);

	    }
	    outSampRe  = scale*outSampRe;
	    if (outSampRe>0)
		{
	    	outSampRe = outSampRe + 0.5;
		}
	    else
		{
	    	outSampRe = outSampRe - 0.5;
		}
	    output[2*nn] = (int32_t )outSampRe;

	    outSampIm = scale*outSampIm;

	    if (outSampIm>0)
		{
	    	outSampIm = outSampIm + 0.5;
		}
	    else
		{
	    	outSampIm = outSampIm - 0.5;
		}


	    output[2*nn+1] = (int32_t)outSampIm;

	}
}



/***********************************************************************************
 * use matlab randomly generate data as input,
 * test each bin for fft size 4,8,16,32, 64,128
 * reference is generated by calculate_dft, which is float point version of fft calculation
 * threshold is set to 101
 */
void test_dftsinglebin()
{
	uint8_t dftsize[6]={4,8,16,32, 64,128};
	uint8_t i,j,k;
	int16_t numTrails;
	int16_t sincos[128*2]; /* maximum test dft size is 128 */
	int32_t libOutput[2]; /* lib output each with 1 32 bits real, 1 32 bits original */
	uint8_t testPASS=1;
	FILE * fid;
	
	if ((fid = fopen ("..\\testdata\\dftsinglebin_test.bin","rb")) == NULL)
    {
        printf ("Can not open file dftsinglebin_test.bin\n");
        exit(3);
    }

	/* read in number of trails */
	fread(&numTrails,sizeof(int16_t), 1, fid);
	for (k = 0; k < numTrails; k++)  //run each trail
	{

		for (i=0;i<6;i++)  /* fft size */
		{
			/* read in input data for each dft size */
			fread(inputBuf, sizeof(int16_t), dftsize[i]*2, fid);
			/* call reference for all bins */
			calculate_dft(&inputBuf[0],dftsize[i],&outputBuf[0],0.5); //1 antenna

			/* generate the sincos tbl for lib function */
			gen_dftSinCosTable(dftsize[i], &sincos[0]);


			for (j = 0; j <dftsize[i];j++ )  /* for each bin */
			{
				/* call lib function */
				mmwavelib_dftSingleBin((uint32_t*)&inputBuf[0], (uint32_t*) &sincos[0], (uint64_t*) &libOutput[0], dftsize[i], j);
				/* check the results */
				if (abs(libOutput[0] - outputBuf[2*j]) > MAXIMUM_ERROR ) /* real */
				{
					testPASS = 0;
					printf("set=%d,fftsize=%d, bin=%d, lib=%d, ref=%d, error= %d\n", k,dftsize[i],j, libOutput[0],outputBuf[2*j],libOutput[0]- outputBuf[2*j]);
				}
				if (abs(libOutput[1] - outputBuf[2*j+1]) > MAXIMUM_ERROR ) /* imag */
				{
					testPASS = 0;
					printf("set=%d,fftsize=%d, bin=%d, lib=%d, ref=%d, error=%d\n", k,dftsize[i],j, libOutput[1],outputBuf[2*j+1],libOutput[1]-outputBuf[2*j+1] );
				}
			}

		} /* end of fft size loop */

	} /* end of trail loop */

	fclose(fid);

	if (testPASS)
	{
		MCPI_setFeatureTestResult("The mmwavelib_dftSingleBin functionality test", MCPI_TestResult_PASS);
	}
	else
	{
		MCPI_setFeatureTestResult("The mmwavelib_dftSingleBin functionality test", MCPI_TestResult_FAIL);
	}


	printf("\n mmwavelib_dftSingleBin benchmark \n");
	printf("length, mmwavelib_dftSingleBin cycles:\n");
	
	for (i=0;i<6;i++)  /* fft size */
	{
		w_startTime1 = TSCL;
		mmwavelib_dftSingleBin((uint32_t*)&inputBuf[0],(uint32_t*) &sincos[0], (uint64_t*) &libOutput[0], dftsize[i], dftsize[i]-1);
		cycles = TSCL-w_startTime1;
		printf("%d\t%d\n",dftsize[i], cycles);
	}


}



void test_dftsinglebinwithwin()
{
	uint8_t dftsize[6]={4,8,16,32, 64,128};
	uint8_t i,j,k;
	int16_t numTrails;
	int16_t sincos[128*2]; /* maximum test dft size is 128 */
	int32_t libOutput[2]; /* lib output each with 1 32 bits real, 1 32 bits original */
	int32_t window[64]; /* half of the fft length */
	uint8_t testPASS=1;
	FILE * fid;
	double error;
	
	if ((fid = fopen ("..\\testdata\\dftsinglebinwithwin_test.bin","rb")) == NULL)
	{
		printf ("Can not open file dftsinglebinwithwin_test.bin\n");
	    exit(3);
	}

	/* read in number of trails */
	fread(&numTrails,sizeof(int16_t), 1, fid);
	for (k = 0; k < numTrails; k++)  //run each trail
	{

		/* read in the window factor */
		fread(window,sizeof(int32_t), dftsize[5]/2,fid);
		
		for (i=0;i<6;i++)  /* fft size */
		{
			/* read in input data for each dft size */
			fread(inputBuf, sizeof(int16_t), dftsize[i]*2, fid);
			/* call reference for all bins */
			calculate_dft_withwin(&inputBuf[0],0, window, dftsize[i],&outputBuf[0],0.5f); 

			/* generate the sincos tbl for lib function */
			gen_dftSinCosTable(dftsize[i], &sincos[0]);


			for (j = 0; j <dftsize[i];j++ )  /* for each bin */
			{
				mmwavelib_dftSingleBinWithWindow((uint32_t*)&inputBuf[0], (uint32_t*) &sincos[0], window, (uint64_t*) &libOutput[0], dftsize[i], j);
				/* check the results */
				error = abs(libOutput[0] - outputBuf[2*j]);
				if (error > MAXIMUM_ERROR )
				{
					testPASS = 0;
					printf("set=%d,fftsize=%d, bin=%d, lib=%d, ref=%d, error= %2.6f\n", k,dftsize[i],j, libOutput[0],outputBuf[2*j],error); //libOutput[0]- outputBuf[2*j]);
				}
				error = abs(libOutput[1] - outputBuf[2*j+1]);
				if (error > MAXIMUM_ERROR)
				{
					testPASS = 0;
					printf("set=%d,fftsize=%d, bin=%d, lib=%d, ref=%d, error=%2.6f\n", k,dftsize[i],j, libOutput[1],outputBuf[2*j+1],error); //libOutput[1]-outputBuf[2*j+1] );
				}
			}

		} /* end of fft size loop */

	} /* end of trail loop */

	fclose(fid);

	if (testPASS)
	{
		MCPI_setFeatureTestResult("The mmwavelib_dftSingleBinWithWindow functionality test", MCPI_TestResult_PASS);
	}
	else
	{
		MCPI_setFeatureTestResult("The mmwavelib_dftSingleBinWithWindow functionality test", MCPI_TestResult_FAIL);
	}


		printf("\n mmwavelib_dftSingleBinWithWindow benchmark \n");
		printf("length, mmwavelib_dftSingleBinWithWindow cycles:\n");
		
		for (i=0;i<6;i++)  /* fft size */
		{
			w_startTime1 = TSCL;
			mmwavelib_dftSingleBinWithWindow((uint32_t*)&inputBuf[0],(uint32_t*) &sincos[0], window, (uint64_t*) &libOutput[0], dftsize[i], dftsize[i]-1);
			cycles = TSCL-w_startTime1;
			printf("%d\t%d\n",dftsize[i], cycles);
		}

}


void test_dftsinglebinwithwin_ReIm()
{
	uint8_t dftsize[6]={4,8,16,32, 64,128};
	uint8_t i,j,k;
	int16_t numTrails;
	int16_t sincos[128*2]; /* maximum test dft size is 128 */
	int32_t libOutput[2]; /* lib output each with 1 32 bits real, 1 32 bits original */
	int32_t window[64]; /* half of the fft length */
	uint8_t testPASS=1;
	FILE * fid;
	double error;
	
	if ((fid = fopen ("..\\testdata\\dftsinglebinwithwinQI_test.bin","rb")) == NULL)
	{
		printf ("Can not open file dftsinglebinwithwinQI_test.bin\n");
	    exit(3);
	}

	/* read in number of trails */
	fread(&numTrails,sizeof(int16_t), 1, fid);
	for (k = 0; k < numTrails; k++)  //run each trail
	{

		/* read in the window factor */
		fread(window,sizeof(int32_t), dftsize[5]/2,fid);
		
		for (i=0;i<6;i++)  /* fft size */
		{
			/* read in input data for each dft size */
			fread(inputBuf, sizeof(int16_t), dftsize[i]*2, fid);
			/* call reference for all bins */
			calculate_dft_withwin(&inputBuf[0],1,window, dftsize[i],&outputBuf[0],0.5f); 

			/* generate the sincos tbl for lib function */
			gen_dftSinCosTable(dftsize[i], &sincos[0]);


			for (j = 0; j <dftsize[i];j++ )  /* for each bin */
			{
				mmwavelib_dftSingleBinWithWindow_ReIm((uint32_t*)&inputBuf[0], (uint32_t*) &sincos[0], window, (uint64_t*) &libOutput[0], dftsize[i], j);
				/* check the results */
				error = abs(libOutput[0] - outputBuf[2*j]);
				if (error > MAXIMUM_ERROR )
				{
					testPASS = 0;
					printf("set=%d,fftsize=%d, bin=%d, lib=%d, ref=%d, error= %2.6f\n", k,dftsize[i],j, libOutput[0],outputBuf[2*j],error); //libOutput[0]- outputBuf[2*j]);
				}
				error = abs(libOutput[1] - outputBuf[2*j+1]);
				if (error > MAXIMUM_ERROR)
				{
					testPASS = 0;
					printf("set=%d,fftsize=%d, bin=%d, lib=%d, ref=%d, error=%2.6f\n", k,dftsize[i],j, libOutput[1],outputBuf[2*j+1],error); //libOutput[1]-outputBuf[2*j+1] );
				}
			}

		} /* end of fft size loop */

	} /* end of trail loop */

	fclose(fid);

	if (testPASS)
	{
		MCPI_setFeatureTestResult("The mmwavelib_dftSingleBinWithWindow_ReIm functionality test", MCPI_TestResult_PASS);
	}
	else
	{
		MCPI_setFeatureTestResult("The mmwavelib_dftSingleBinWithWindow_ReIm functionality test", MCPI_TestResult_FAIL);
	}


		printf("\n mmwavelib_dftSingleBinWithWindow_ReIm benchmark \n");
		printf("length, mmwavelib_dftSingleBinWithWindow_ReIm cycles:\n");
		
		for (i=0;i<6;i++)  /* fft size */
		{
			w_startTime1 = TSCL;
			mmwavelib_dftSingleBinWithWindow_ReIm((uint32_t*)&inputBuf[0],(uint32_t*) &sincos[0], window, (uint64_t*) &libOutput[0], dftsize[i], dftsize[i]-1);
			cycles = TSCL-w_startTime1;
			printf("%d\t%d\n",dftsize[i], cycles);
		}

}

void test_dftsincosTblGen()
{
	uint32_t dftsize[9]={4,8,16,32,64,128,256,512,1024};
	uint32_t k, j;
	int16_t libCos, libSin;
	uint8_t testPASS=1;
	int16_t diff;
	
	for (k = 0; k < 9; k++)  
	{

        printf("FFT size %d\n", dftsize[k]);
		/* generate the sincos tbl for reference */
		gen_dftSinCosTable(dftsize[k], &dftSinCosRef[0]);
		
		/* call the lib */
		mmwavelib_dftSinCosTabelGen(dftsize[k], &dftSinCosLib[0]);
        
        /* check the results*/
        for (j = 0; j < dftsize[k]; j++)
        {
            libCos = (int16_t )(dftSinCosLib[j]&0xffff); 
            diff = dftSinCosRef[2 * j] - libCos;
            if ((diff > MAXIMUM_SINCOS_ERROR) || (diff < -1 * MAXIMUM_SINCOS_ERROR))
            {
                testPASS = 0;
                printf("dft sin/cos tbl gen fail at %d cos value ref=%d, lib=%d\n", j,dftSinCosRef[2 * j],libCos );
            }
            
            libSin = (int16_t )((dftSinCosLib[j] >> 16)&0xffff);
            diff = dftSinCosRef[2 * j + 1] - libSin;
            if ((diff > MAXIMUM_SINCOS_ERROR) || (diff < -1 * MAXIMUM_SINCOS_ERROR))
            {
                testPASS = 0;
                printf("dft sin/cos tbl gen fail at %d sin value ref=%d, lib=%d\n", j,dftSinCosRef[2 * j + 1],libSin);
            }
        }
	}
    
    
    if (testPASS)
	{
		MCPI_setFeatureTestResult("The mmwavelib_dftSinCosTabelGen functionality test", MCPI_TestResult_PASS);
	}
	else
	{
		MCPI_setFeatureTestResult("The mmwavelib_dftSinCosTabelGen functionality test", MCPI_TestResult_FAIL);
	}

    printf("\n mmwavelib_dftSinCosTabelGen benchmark \n");
    printf("length,  cycles:\n");

    for (k = 0;k < 9; k++)  /* fft size */
    {
        w_startTime1 = TSCL;
        mmwavelib_dftSinCosTabelGen(dftsize[k], &dftSinCosLib[0]);
        cycles = TSCL-w_startTime1;
        printf("%d\t%d\n",dftsize[k], cycles);
	}

}


void test_dftHalfBinTbl()
{
    uint32_t dftsize[9]={4,8,16,32,64,128,256,512,1024};
    uint32_t k;
    float temp;
    int32_t itemp;
    uint8_t testPASS=1;
    
    for (k = 0; k < 9; k++)
    {
        temp = ONE_Q15 * - sinsp(PI_/dftsize[k]);
        itemp = (int32_t) ROUND(temp);

        if(itemp >= ONE_Q15)
        {
            itemp = ONE_Q15 - 1;
        }
        if ( (int16_t) itemp != mmwavelib_dftHalfBinExpValue[2 * k])
        {
            testPASS = 0;
            printf("dfthalfbin table fail at fft size %d sin, ref = %d, table=%d\n",dftsize[k], itemp, mmwavelib_dftHalfBinExpValue[2 * k]);
        }
        
        temp = ONE_Q15 * cossp(PI_/dftsize[k]);
        itemp = (int32_t) ROUND(temp);

        if(itemp >= ONE_Q15)
        {
            itemp = ONE_Q15 - 1;
        }
        
        if ( (int16_t) itemp != mmwavelib_dftHalfBinExpValue[2 * k + 1])
        {
            testPASS = 0;
            printf("dfthalfbin table fail at fft size %d cos, ref = %d, table=%d\n",dftsize[k], itemp, mmwavelib_dftHalfBinExpValue[2 * k + 1]);
        }
    }
    
    if (testPASS)
	{
		MCPI_setFeatureTestResult("The mmwavelib_dftHalfBinExpValue table test", MCPI_TestResult_PASS);
	}
	else
	{
		MCPI_setFeatureTestResult("The mmwavelib_dftHalfBinExpValue table test", MCPI_TestResult_FAIL);
	}
}


void main(void)
{
    MCPI_Initialize();
    TSCL = 0;
    
    printf("\n Start DFT single bin related functions test ......\n");
    printf("\nTest dftSingleBin :\n");
	test_dftsinglebin();
	printf("\nTest dftSingleBinWithWindow : \n");
	test_dftsinglebinwithwin();
	printf("\nTest dft sin/cos table generation: \n");
	test_dftsincosTblGen();
    printf("\n Test pre-generated dft half bin sin/cos table \n");
    test_dftHalfBinTbl();
    printf("\n Test dftSingleBinWithWindow with input high 16 bits is real, and low 16 bits is imag\n");
    test_dftsinglebinwithwin_ReIm();
    printf("\n DFT single bin related functions test completed ......\n");
    
    MCPI_setTestResult ();
}
