/*
 *   @file  lib_shift_histogram_test.c
 *
 *   @brief
 *      Unit Test code for shiftHistogram mmwavelib library functions
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
#include <stdlib.h>
#include <string.h>
#include <ti/alg/mmwavelib/mmwavelib.h>
#include <ti/utils/testlogger/logger.h>


extern volatile uint32_t w_startTime1;
extern uint32_t  cycles;

extern int16_t refbuf[];
extern uint16_t tmp16buf[]; //tmp16buf[MAX_NUM_DOPPLER_BINS]
extern int32_t tmp32buf[]; //tmp32buf[MAX_NUM_RANGE_BINS*2];
extern uint16_t h_sumAbs[];
extern uint16_t h_log2Abs[];

extern int32_t totalFailed;
int32_t prevFailed1 ;


void ref_shiftright(int16_t input[restrict],
	                int16_t output[restrict],
                    uint8_t numshift,
                    uint32_t inpLen)
{
	   uint32_t i;

	    for(i = 0; i < inpLen; i++)
		{
			output[i] = input[i] >> numshift;
		}
}

void ref_histogram(uint16_t input[restrict],
                   uint16_t hist[restrict],
                   uint32_t inpLen,
                   uint8_t numshift)
{
	   uint32_t i;

	    for(i = 0; i < inpLen; i++)
		{
			hist[ input[i] >> numshift ] ++;
		}
}


void test_histogram_shift(int32_t numRangeBins,int32_t numDopBins)
{
	uint32_t i;
	uint16_t * inptr, *outptr, *scratch;
	uint16_t *refptr = (uint16_t *)&refbuf[0];
	uint32_t histlen;
	uint8_t  numShift;
	int16_t  *inptr16, *outptr16, *refptr16;
	uint32_t length;

	prevFailed1 = totalFailed;

	printf("\n --- Test histogram calculation :\n");

	inptr = &h_sumAbs[0];
	outptr = &tmp16buf[0];
	scratch = (uint16_t*)&tmp32buf[0];

	numShift = 10;
	histlen = 1<<(16-numShift); //64
	length  = numDopBins*numRangeBins; //calculate histogram on all sumAbs values
	//memset(scratch, 0, histlen*8*2); //clear the buffer either here if function doesn't do it
	mmwavelib_histogram(inptr, outptr, scratch, length, histlen, numShift);

	memset(refptr, 0, histlen*2);
	ref_histogram(inptr, refptr, length, numShift);

	for ( i=0; i<histlen; i++ )
	{
	    //printf("hist bin# i=%d, out=%d, ref=%d\n", i, outptr[i], refptr[i]);
		if ( outptr[i] != refptr[i] )
		{
			printf("Mismatch at hist bin# i=%d: out=%d, ref=%d\n", i, outptr[i], refptr[i]);
			totalFailed++;
		}
	}

    if ( totalFailed > prevFailed1)
	{
        MCPI_setFeatureTestResult("Histogram test", MCPI_TestResult_FAIL);
	}
    else
    {
        MCPI_setFeatureTestResult("Histogram test", MCPI_TestResult_PASS);
    }


	/* --- test shiftright16 --- */
	printf("\n --- Test shiftright16 calculation :\n");
	prevFailed1 = totalFailed;

	inptr16 = (int16_t *)inptr;
	outptr16 = (int16_t *)outptr;
	refptr16 = &refbuf[128];
	numShift = 1;
	inptr16[0] = 0;
	inptr16[1] = 32767; //max 16-bit value
	inptr16[2] = -1;
	inptr16[1] = -32768; //min 16-bit value

	for (length = 8; length <=128; length *=2, numShift++)
	{
		mmwavelib_shiftright16(inptr16, outptr16, numShift, length);
		ref_shiftright(inptr16, refptr16, numShift, length);
		for ( i=0; i<length; i++ )
		{
			if ( outptr16[i] != refptr16[i] )
			{
				printf("i=%d, out=%d, ref=%d\n", i, outptr16[i], refptr16[i]);
				totalFailed++;
			}
		}
	}

    if ( totalFailed > prevFailed1)
    {
        MCPI_setFeatureTestResult("16-bit shiftright test", MCPI_TestResult_FAIL);
    }
    else
    {
        MCPI_setFeatureTestResult("16-bit shiftright test", MCPI_TestResult_PASS);
    }

} /* test_histogram_shift */


void benchmark_hist()
{
	uint16_t * inptr, *outptr, *scratch;
	uint32_t   len, nhist=64;
	uint8_t shiftnum = 10;

	inptr = &h_log2Abs[0];
	outptr = &h_sumAbs[0];
	scratch = &h_sumAbs[128];/* enough space for nhist(64)*8 16-bit*/

	printf("\nBenchmark for mmwavelib_histogram :\n");
	printf("input length and 64-bin histogram cycles:\n");
	mmwavelib_histogram(inptr, outptr, scratch, 512, 64, shiftnum);/*to cache*/
	for ( len=32; len<=512; len=len*2)
	{
		w_startTime1 = TSCL;
	    mmwavelib_histogram(inptr, outptr, scratch, len, nhist, shiftnum);
		cycles = TSCL-w_startTime1;
		printf("%d\t%d\n", len, cycles);
	}

	nhist=32;
	printf("input length and %d-bin histogram cycles:\n", nhist);
	for ( len=32; len<=512; len=len*2)
	{
		w_startTime1 = TSCL;
	    mmwavelib_histogram(inptr, outptr, scratch, len, nhist, shiftnum);
		cycles = TSCL-w_startTime1;
		printf("%d\t%d\n", len, cycles);
	}


	/* --- Benchmark shiftright16 function --- */
	printf("\nBenchmark for mmwavelib_shiftright16:\n");
	printf("input length and cycles:\n");
	for ( len=32; len<=512; len=len*2)
	{
		w_startTime1 = TSCL;
	    mmwavelib_shiftright16((int16_t*)inptr, (int16_t*)outptr, shiftnum, len);
		cycles = TSCL-w_startTime1;
		printf("%d\t%d\n", len, cycles);
	}

}/* benchmark_hist*/


void test_hist_ones_allbins()
{
	uint16_t * inptr, *outptr, *scratch;
	uint32_t   i, len;
	uint8_t shiftnum = 10;

	inptr = &h_log2Abs[0];
	outptr = &h_sumAbs[0];
	scratch = &h_sumAbs[128];


	printf("\nTest histogram:\n");
	prevFailed1 = totalFailed;

	for ( i=0; i<64; i++)
	{
		inptr[i] = i << shiftnum;
	}
	mmwavelib_histogram(inptr, outptr, scratch, 64, 64, shiftnum);

	for ( i=0; i<64; i++)
	{
		if ( outptr[i] !=1)
		{
			printf("hist bin# i=%d, out=%d\n", i, outptr[i]);
			totalFailed++;
		}
	}

	shiftnum = 9;
	len = 128;
	memset(scratch, 0, 128*8*2);
	for ( i=0; i<len; i++)
	{
		inptr[i] = i << shiftnum;
	}
	mmwavelib_histogram(inptr, outptr, scratch, len, len, shiftnum);

	for ( i=0; i<len; i++)
	{
		if ( outptr[i] !=1)
		{
			printf("hist bin# i=%d, out=%d\n", i, outptr[i]);
			totalFailed++;
		}
	}
    if ( totalFailed > prevFailed1)
	{
        MCPI_setFeatureTestResult("Histogram test all bins", MCPI_TestResult_FAIL);
	}
    else
    {
        MCPI_setFeatureTestResult("Histogram test all bins", MCPI_TestResult_PASS);
    }

	benchmark_hist();

}


void test_shift32right()
{
	int32_t * inptr, *outptr;
	int32_t   refval;
	int32_t   i;
	uint32_t  len=64;
	uint8_t   shiftk=1;

	printf("\nTest shiftright32:\n");
	totalFailed = prevFailed1;

	inptr = &tmp32buf[0];
	outptr = &tmp32buf[512];

	for (i=0; i<=31; i++)
	{
		inptr[i] = (1<<i) -1; //0,1,2^2-1,... to 2^31-1
		inptr[i+32] = -(int32_t)(1<<i); //-1,-2,-4,... to -2^31
	}
	for (i=64; i<512; i+=2)
	{
		inptr[i] = rand();
		inptr[i+1] = (-1)*inptr[i];
	}

	for (len=4, shiftk=1; len<=512; len*=2, shiftk++)
	{
		mmwavelib_shiftright32(inptr, outptr, shiftk, len);

		for (i=0; i<len; i++)
		{
			refval = inptr[i] >> shiftk;
			if ( refval != outptr[i])
			{
				printf("shiftright32 failed i=%d, in%d, out%d, ref%d\n",
						i, inptr[i], outptr[i], refval);
				totalFailed ++;
			}
		}
	}

    if ( totalFailed > prevFailed1)
	{
        MCPI_setFeatureTestResult("32-bit shiftright test", MCPI_TestResult_FAIL);
	}
    else
    {
        MCPI_setFeatureTestResult("32-bit shiftright test", MCPI_TestResult_PASS);
    }

	/* --- Benchmarking ---- */
	printf("Benchmark for mmwavelib_shiftright32:\n");
	printf("input length and cycles:\n");
    mmwavelib_shiftright32(inptr, outptr, shiftk, 512);
	for ( len=32; len<=512; len=len*2)
	{
		w_startTime1 = TSCL;
	    mmwavelib_shiftright32(inptr, outptr, shiftk, len);
		cycles = TSCL-w_startTime1;
		printf("%d\t%d\n", len, cycles);
	}

}
