/*
 *   @file  lib_dcremovalflt_test.c
 *
 *   @brief
 *      Unit Test code for floating point DC removal
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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ti/alg/mmwavelib/mmwavelib.h>
#include <ti/utils/testlogger/logger.h>


extern int32_t totalFailed;
extern int32_t prevFailed;

extern uint32_t t1, t2;
extern uint32_t cycles;

extern int16_t databuf[];
extern int16_t refbuf[];
extern int32_t data32buf[];

void ref_dcremoval_float(int32_t ncplx, float32_t * inputbuf, float32_t * refoutbuf)
{
	int32_t i;
	float32_t    sumref[2]={0};
	float32_t    avgIQ[2];

	sumref[0] = 0;
	sumref[1] = 0;
	t1 = TSCL;
	for ( i=0; i < ncplx; i++)
	{
		sumref[0] += inputbuf[2*i];
		sumref[1] += inputbuf[2*i+1];
	}
	t2 = TSCL;
	/*printf("sum ref cycles: %d\t%d\n", ncplx, (t2-t1));*/

	avgIQ[0] = sumref[0]/ncplx;
	avgIQ[1] = sumref[1]/ncplx;

	t1 = TSCL;
	for ( i=0; i < ncplx; i++)
	{
		refoutbuf[2*i] = inputbuf[2*i] - avgIQ[0];
		refoutbuf[2*i+1] = inputbuf[2*i+1] - avgIQ[1];
	}
	t2 = TSCL;
	/*printf("subc ref cycles: %d\t%d\n", ncplx, (t2-t1));*/

}

void test_dcremoval_float()
{
	int32_t i, numCplx;
	float32_t * inptr   = (float32_t *)&databuf[0];
	float32_t * refoutptr = (float32_t*)&refbuf[0];
	float32_t * outptr = (float32_t*)&data32buf[0];
	float32_t diff;

	prevFailed = totalFailed;

	/* Fill input data */
	for ( i=0; i <= 128*2; i++ )
	{
		inptr[i] = 10.1*(rand() - RAND_MAX/2);
	}

	printf("\n===== Test mmwavelib_dcRemovalFloat =====\n");
	for (numCplx=1; numCplx<=128; numCplx++)
	{
		ref_dcremoval_float(numCplx, inptr, refoutptr);

		/* input and output can be the same array */
		/* use different arrays here for test and debug convenience */
		mmwavelib_dcRemovalFloat(numCplx, inptr, outptr);

		/* compare output with reference */
		for ( i=0; i < numCplx*2; i++ )
		{
			diff = refoutptr[i] -outptr[i];
			if ( diff > 0.02 || diff < -0.02 )
			{
				totalFailed ++;
				printf("numCplx=%d, i=%d, ref=%10.3f, out=%10.3f, diff=%10.3f\n",
						numCplx, i, refoutptr[i], outptr[i], diff);
				break;
			}
		}
	}

	if ( totalFailed == prevFailed)
	{
		MCPI_setFeatureTestResult("DC Removal mmwavelib_dcRemovalFloat", MCPI_TestResult_PASS);
	}
	else
	{
		MCPI_setFeatureTestResult("DC Removal mmwavelib_dcRemovalFloat", MCPI_TestResult_FAIL);
	}

	printf("\nBenchmark for mmwavelib_dcRemovalFloat\nNcplx\tcycles:\n");

	for (i = 0; i < 8; i++)
	{
		printf("ncplx mod 8 = %d:\n", i);
		for (numCplx = 16; numCplx <= 128; numCplx *=2)
		{
			t1 = TSCL;
			mmwavelib_dcRemovalFloat(numCplx+i, inptr, inptr);
			t2 = TSCL;
			printf("%d\t%d\n", numCplx+i, (t2-t1));
		}
	printf("\n");
	}

} /* test_dcremoval_float */
