/*
 *   @file  lib_matutil_test.c
 *
 *   @brief
 *      Unit Test code for vector math utility mmwavelib library functions
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

#define MAX_ROWS 128
#define MAX_COLUMNS 32

/* buffer for 32bit data */
#pragma DATA_SECTION(matrix,".l2data");
#pragma DATA_ALIGN(matrix, 8);
int32_t matrix[MAX_ROWS*MAX_COLUMNS*2];


void test_matrix_transpose()
{
	int32_t * in;
	int32_t * out;
	int32_t numrows = 128;
	int32_t numcols = 32;
	int32_t i, j;

	printf("\n===== Test mmwavelib_mattrans =====\n");
	memset(matrix, 0, sizeof(matrix));

	in = &matrix[0];
	out = &matrix[numrows*numcols];

	for ( i = 0; i < numrows*numcols; i++ )
	{
		in[i] = rand();
	}

	mmwavelib_mattrans((float32_t *)in, numrows, numcols, (float32_t *)out);

	prevFailed = totalFailed;
	for ( i = 0; i < numrows ; i++)
	{
		for ( j = 0; j < numcols; j++)
		{
			if ( in[i*numcols+j] != out[j*numrows+i] )
			{
				totalFailed ++;
				printf("i=%d, j=%d, in=%d, out=%d; \n", i, j, in[i*numcols+j], out[j*numrows+i]);
				break;
			}
		}
	}

	if ( totalFailed == prevFailed)
	{
		MCPI_setFeatureTestResult("Matrix Transpose mmwavelib_mattrans", MCPI_TestResult_PASS);
	}
	else
	{
		MCPI_setFeatureTestResult("Matrix Transpose mmwavelib_mattrans", MCPI_TestResult_FAIL);
	}

	printf("\nBenchmark for mmwavelib_mattrans\nrows\tcols\tcycles:\n");
	t1 = TSCL;
	mmwavelib_mattrans((float32_t *)in, numrows, numcols, (float32_t *)out);
	t2 = TSCL;
	printf("%d\t%d\t%d\n", numrows, numcols,  (t2-t1));
	t1 = TSCL;
	mmwavelib_mattrans((float32_t *)in, numrows, numcols, (float32_t *)out);
	t2 = TSCL;
	printf("%d\t%d\t%d\n", numrows, numcols,  (t2-t1));

	printf("rows\tcols\tcycles\t0.75*rows*cols+3*cols+35:\n");
	for ( numrows = 4; numrows <= 512; numrows *=2)
	{
		for ( numcols = 2; numcols <= 6 ; numcols +=2)
		{
			out = &matrix[numrows*numcols];
			mmwavelib_mattrans((float32_t *)in, numrows, numcols, (float32_t *)out);
			t1 = TSCL;
			mmwavelib_mattrans((float32_t *)in, numrows, numcols, (float32_t *)out);
			t2 = TSCL;
			printf("%d\t%d\t%d\t%d\n", numrows, numcols, (t2-t1), (int32_t)(0.75*numcols*numrows+3*numcols+35));
		}
		printf("\n");
	}

	printf("rows\tcols\tcycles\t1*rows*cols+3*cols+35:\n");
	for ( numrows = 4; numrows <= 128; numrows *=2)
	{
		for ( numcols = 8; numcols <= 32 ; numcols +=8)
		{
			out = &matrix[numrows*numcols];
			mmwavelib_mattrans((float32_t *)in, numrows, numcols, (float32_t *)out);
			t1 = TSCL;
			mmwavelib_mattrans((float32_t *)in, numrows, numcols, (float32_t *)out);
			t2 = TSCL;
			printf("%d\t%d\t%d\t%d\n", numrows, numcols, (t2-t1), (int32_t)(1*numcols*numrows+3*numcols+35));
		}
		printf("\n");
	}
} /* test_matrix_transpose */
