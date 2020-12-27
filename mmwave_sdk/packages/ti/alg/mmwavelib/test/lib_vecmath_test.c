/*
 *   @file  lib_vecmath_test.c
 *
 *   @brief
 *      Unit Test code for vector math utility mmwavelib library functions
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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ti/alg/mmwavelib/mmwavelib.h>
#include <ti/utils/testlogger/logger.h>

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

int32_t totalFailed;
int32_t prevFailed;

uint32_t t1, t2;
uint32_t cycles;

#define MAX_LEN 512

/* buffer for input data */
#pragma DATA_SECTION(databuf,".l2data");
#pragma DATA_ALIGN(databuf, 8);
int16_t databuf[MAX_LEN];

/* buffer for reference output data */
#pragma DATA_SECTION(refbuf,".l2data");
#pragma DATA_ALIGN(refbuf, 8);
int16_t refbuf[MAX_LEN];

/* buffer for 32bit data */
#pragma DATA_SECTION(data32buf,".l2data");
#pragma DATA_ALIGN(data32buf, 8);
int32_t data32buf[MAX_LEN];


/* --- test vectors with two peaks for testing the second peak search --- */

float32_t tv1databuf[64] = {
 911724.80, 1290968.27, 1620774.12, 1872886.78, 2027158.08, 2073624.99, 2013365.34, 1857963.26,
1627557.50, 1347580.23, 1044456.07,  740885.35,  452681.42,  200796.74,  193411.45,  449226.21,
 748667.19, 1065927.67, 1382517.60, 1673890.94, 1912504.98, 2072381.26, 2133260.84, 2083755.40,
1923216.14, 1662199.52, 1321545.60,  930221.66,  522210.66,  132857.89,  205308.81,  465191.49,
 629122.48,  689881.97,  651482.01,  528422.51,  343777.20,  126411.29,   93713.42,  285370.66,
 425158.30,  496081.68,  490877.86,  412516.71,  273600.75,   94625.61,   98968.34,  279421.28,
 421183.15,  503554.82,  513640.27,  448107.50,  313828.67,  127584.44,   90414.88,  304022.28,
 487398.30,  611288.20,  652766.68,  597424.08,  441313.11,  192584.04,  142134.38,  516470.62
};
/* test vector's length, first peak index, second peak index */
int32_t tv1ref[3] = {64, 22, 5};

float32_t tv2databuf[128] = {
 911724.80, 1105520.62, 1290968.27, 1463975.39, 1620774.12, 1758021.07, 1872886.78, 1963130.57,
2027158.08, 2064059.22, 2073624.99, 2056342.14, 2013365.34, 1946466.66, 1857963.26, 1750624.12,
1627557.50, 1492081.41, 1347580.23, 1197352.26, 1044456.07,  891569.25,  740885.35,  594105.79,
 452681.42,  318883.83,  200796.74,  137687.92,  193411.45,  311929.90,  449226.21,  595758.79,
 748667.19,  906077.02, 1065927.67, 1225718.73, 1382517.60, 1533060.74, 1673890.94, 1801505.84,
1912504.98, 2003728.28, 2072381.26, 2116143.92, 2133260.84, 2122610.92, 2083755.40, 2016963.59,
1923216.14, 1804186.22, 1662199.52, 1500174.61, 1321545.60, 1130169.58,  930221.66,  726080.87,
 522210.66,  323038.84,  132857.89,   44761.77,  205308.81,  346358.53,  465191.49,  559865.69,
 629122.48,  672408.49,  689881.97,  682399.36,  651482.01,  599263.96,  528422.51,  442093.87,
 343777.20,  237233.56,  126411.29,   16969.61,   93713.42,  194601.60,  285370.66,  363027.43,
 425158.30,  469920.45,  496081.68,  503048.81,  490877.86,  460264.76,  412516.71,  349504.98,
 273600.75,  187596.88,   94625.61,    3938.06,   98968.34,  192498.97,  279421.28,  356587.84,
 421183.15,  470807.34,  503554.82,  518079.03,  513640.27,  490135.23,  448107.50,  388739.50,
 313828.67,  225761.67,  127584.44,   26593.92,   90414.88,  198902.96,  304022.28,  401450.11,
 487398.30,  558382.70,  611288.20,  643456.27,  652766.68,  637706.55,  597424.08,  531767.16,
 441313.11,  327423.71,  192584.04,   48734.77,  142134.38,  324026.81,  516470.62,  713893.85
};
int32_t tv2ref[3] = {128, 44, 10};

float32_t tv3databuf[32] = {
 911724.80, 1620774.12, 2027158.08, 2013365.34, 1627557.50, 1044456.07,  452681.42,  193411.45,
 748667.19, 1382517.60, 1912504.98, 2133260.84, 1923216.14, 1321545.60,  522210.66,  205308.81,
 629122.48,  651482.01,  343777.20,   93713.42,  425158.30,  490877.86,  273600.75,   98968.34,
 421183.15,  513640.27,  313828.67,   90414.88,  487398.30,  652766.68,  441313.11,  142134.38
 };
int32_t tv3ref[3] = {32, 11, 2};


void test_vecsumabs( )
{
	int32_t i, length = 512;
	int32_t outsum, refsum;

	/* Fill input with data in range [-RAND_MAX/2, RAND_MAX/2] */
	for ( i=0; i < length; i++ )
	{
		databuf[i] = rand() - (RAND_MAX/2);
	}

	outsum = mmwavelib_vecsumabs(databuf, length);

	refsum = 0;
	for ( i=0; i < length; i++)
	{
		refsum += abs(databuf[i]);
	}

	if ( outsum == refsum )
	{
        MCPI_setFeatureTestResult("Vector SumAbs mmwavelib_vecsumabs", MCPI_TestResult_PASS);
	}
    else
    {
		totalFailed ++;
		printf("\nmmwavelib_vecsumabs output=%d, ref=%d\n", outsum, refsum);
        MCPI_setFeatureTestResult("Vector SumAbs", MCPI_TestResult_FAIL);
    }

	printf("\nBenchmark for mmwavelib_vecsumabs\nlength and cycles:\n");
	for (length = 64; length <=512; length=length*2)
	{
		t1 = TSCL;
		outsum = mmwavelib_vecsumabs(databuf, length);
		t2 = TSCL;
		printf("%d\t%d\n", length, (t2-t1));
	}

}/* test_vecsumabs */

void test_vecmul16x32()
{
	int32_t i, numCplx = 128;
	int16_t *sptr = &databuf[0];
	int32_t *wptr;
	int32_t *outptr;
	int32_t *refptr=(int32_t*)&refbuf[0];
	int16_t xI, xQ;
	int32_t wI, wQ, refI, refQ;
	int32_t temp;

	wptr   = &data32buf[0];
	outptr = &data32buf[numCplx*2];

	for ( i=0; i < numCplx*2; i++ )
	{
		wptr[i] = (int32_t)(rand() - RAND_MAX/2)*16;/*left shift 4 bits to Q19 format integer*/
	}

	mmwavelib_vecmul16x32(sptr, wptr, outptr, numCplx);

	/* reference code */
	for ( i=0; i < numCplx; i++ )
	{
		xI = sptr[2*i+1];
		xQ = sptr[2*i];
		wI = wptr[2*i+1];
		wQ = wptr[2*i];

		temp = ((int64_t)xI*wI +0x4000) >>15;
		refI = temp - (int32_t)(((int64_t)xQ*wQ +0x4000)>>15);

		temp = ((int64_t)xI*wQ +0x4000) >>15;
		refQ = temp + (int32_t)(((int64_t)xQ*wI +0x4000)>>15);

		refptr[2*i] = refI;
		refptr[2*i+1] = refQ;
	}

	/* compare output with reference */
	prevFailed = totalFailed;
	for ( i=0; i<numCplx*2; i++)
	{
		if ( outptr[i] != refptr[i] )
		{
			totalFailed ++;
			printf("i=%d, out=%d, ref=%d; \n", i, outptr[i], refptr[i]);
			break;
		}
	}
	/*printf("sI=%d, sQ=%d; wI=%d, wQ=%d;\n", sptr[3], sptr[2], wptr[3], wptr[2]);
	printf("refI=%d, refQ=%d; \n", refptr[2], refptr[3]);
	printf("outI=%d, outQ=%d; \n", outptr[2], outptr[3]);*/

	if ( totalFailed == prevFailed)
	{
		MCPI_setFeatureTestResult("Multiply Complex Vectors mmwavelib_vecmul16x32", MCPI_TestResult_PASS);
	}
    else
    {
    	MCPI_setFeatureTestResult("Multiply Complex Vectors mmwavelib_vecmul16x32", MCPI_TestResult_FAIL);
    }

	printf("\nBenchmark for mmwavelib_vecmul16x32\nNcplx and cycles:\n");
	for (numCplx = 16; numCplx <= 128; numCplx *= 2)
	{
		t1 = TSCL;
		mmwavelib_vecmul16x32(sptr, wptr, outptr, numCplx);
		t2 = TSCL;
		printf("%d\t%d\n", numCplx, (t2-t1));
	}

	printf("\n===== Test mmwavelib_vecmul16x32_anylen =====\n");
	totalFailed = prevFailed;
	numCplx = 1;
	mmwavelib_vecmul16x32_anylen((uint32_t*)sptr, (int64_t*)wptr, (int64_t*)outptr, numCplx);
	/* reference code */
	xI = sptr[1];
	xQ = sptr[0];
	wI = wptr[0];
	wQ = wptr[1];
	refI = (((int64_t)xI*wI +0x4000) >>15) - (int32_t)(((int64_t)xQ*wQ +0x4000)>>15);
	refQ = (((int64_t)xI*wQ +0x4000) >>15) + (int32_t)(((int64_t)xQ*wI +0x4000)>>15);

	if ( outptr[0]!=refI || outptr[1]!=refQ )
	{
		totalFailed ++;
		printf("numCplx=1: outI=%d, refI=%d; outQ=%d, refQ=%d;\n", outptr[1], refI, outptr[0], refQ);
	}

	numCplx = 128;
	mmwavelib_vecmul16x32_anylen((uint32_t*)sptr, (int64_t*)wptr, (int64_t*)outptr, numCplx);
	/* reference code */
	for ( i=0; i < numCplx; i++ )
	{
		xI = sptr[2*i+1];
		xQ = sptr[2*i];
		wI = wptr[2*i];
		wQ = wptr[2*i+1];
		refI = (((int64_t)xI*wI +0x4000) >>15) - (int32_t)(((int64_t)xQ*wQ +0x4000)>>15);
		refQ = (((int64_t)xI*wQ +0x4000) >>15) + (int32_t)(((int64_t)xQ*wI +0x4000)>>15);
		refptr[2*i] = refI;
		refptr[2*i+1] = refQ;
	}

	for ( i=0; i<numCplx*2; i++)
	{
		if ( outptr[i] != refptr[i] )
		{
			totalFailed ++;
			printf("i=%d, out=%d, ref=%d; \n", i, outptr[i], refptr[i]);
			break;
		}
	}

	if ( totalFailed == prevFailed)
	{
		MCPI_setFeatureTestResult("Multiply Complex Vectors mmwavelib_vecmul16x32_anylen", MCPI_TestResult_PASS);
	}
    else
    {
    	MCPI_setFeatureTestResult("Multiply Complex Vectors mmwavelib_vecmul16x32_anylen", MCPI_TestResult_FAIL);
    }

	printf("\nBenchmark for mmwavelib_vecmul16x32_anylen\nlen and cycles:\n");
	for (numCplx = 16; numCplx <= 128; numCplx *= 2)
	{
		t1 = TSCL;
		mmwavelib_vecmul16x32_anylen((uint32_t*)sptr, (int64_t*)wptr, (int64_t*)outptr, numCplx);
		t2 = TSCL;
		printf("%d\t%d\n", numCplx, (t2-t1));
	}


	printf("\n===== Test mmwavelib_vecmul32x16c =====\n");
	totalFailed = prevFailed;
	numCplx = 128;
	mmwavelib_vecmul32x16c(*((uint32_t*)sptr), (int64_t*)wptr, (int64_t*)outptr, numCplx);
	/* reference code */
	xI = sptr[1];
	xQ = sptr[0];
	for ( i=0; i < numCplx; i++ )
	{
		wI = wptr[2*i];
		wQ = wptr[2*i+1];
		refI = (((int64_t)xI*wI +0x4000) >>15) - (int32_t)(((int64_t)xQ*wQ +0x4000)>>15);
		refQ = (((int64_t)xI*wQ +0x4000) >>15) + (int32_t)(((int64_t)xQ*wI +0x4000)>>15);
		refptr[2*i] = refI;
		refptr[2*i+1] = refQ;
	}

	for ( i=0; i<numCplx*2; i++)
	{
		if ( outptr[i] != refptr[i] )
		{
			totalFailed ++;
			printf("i=%d, out=%d, ref=%d; \n", i, outptr[i], refptr[i]);
			break;
		}
	}

	if ( totalFailed == prevFailed)
	{
		MCPI_setFeatureTestResult("Multiply Complex Vectors mmwavelib_vecmul32x16c", MCPI_TestResult_PASS);
	}
    else
    {
    	MCPI_setFeatureTestResult("Multiply Complex Vectors mmwavelib_vecmul32x16c", MCPI_TestResult_FAIL);
    }

	printf("\nBenchmark for mmwavelib_vecmul132x16c\nlen and cycles:\n");
	for (numCplx = 16; numCplx <= 128; numCplx *= 2)
	{
		t1 = TSCL;
		mmwavelib_vecmul32x16c(*((uint32_t*)sptr), (int64_t*)wptr, (int64_t*)outptr, numCplx);
		t2 = TSCL;
		printf("%d\t%d\n", numCplx, (t2-t1));
	}

} /* test_vecmul16x32 */


void test_vecmul16x16()
{
	int32_t i, numCplx = 128;
	int16_t *sptr = &databuf[0];
	int16_t *wptr, *refptr=&refbuf[0];
	int16_t xI, xQ, wI, wQ, refI, refQ;
	int32_t temp;

	wptr = &databuf[numCplx*2];

	/* reference code */
	for ( i=0; i < numCplx; i++ )
	{
		xI = sptr[2*i+1];
		xQ = sptr[2*i];
		wI = wptr[2*i+1];
		wQ = wptr[2*i];
		temp = ((int32_t)xI*wI - (int32_t)xQ*wQ + 0x4000) >>15;
		if ( temp >32767 )
		{ temp = 32767; }
		else if ( temp <-32768 )
		{ temp = -32768; }
		refI = temp;

		temp = ((int32_t)xI*wQ + (int32_t)xQ*wI + 0x4000) >>15;
		if ( temp >32767 )
		{ temp = 32767; }
		else if ( temp <-32768 )
		{ temp = -32768; }
		refQ = temp;

		refptr[2*i] = refQ;
		refptr[2*i+1] = refI;
	}

	/*printf("4th: sI=%d, sQ=%d; wI=%d, wQ=%d;\n", sptr[7], sptr[6], wptr[7], wptr[6]);*/

	mmwavelib_vecmul16x16(sptr, wptr, numCplx);

	/* printf("refI=%d, refQ=%d; \n", refptr[7], refptr[6]);
	printf("outI=%d, outQ=%d; \n", sptr[7], sptr[6]); //for debug */

	/* compare output with reference */
	prevFailed = totalFailed;
	for ( i=0; i<numCplx*2; i++)
	{
		if ( sptr[i] != refptr[i] )
		{
			totalFailed ++;
			printf("i=%d, out=%d, ref=%d; \n", i, sptr[i], refptr[i]);
		}
	}

	if ( totalFailed == prevFailed)
	{
		MCPI_setFeatureTestResult("Multiply Complex Vectors mmwavelib_vecmul16x16", MCPI_TestResult_PASS);
	}
    else
    {
    	MCPI_setFeatureTestResult("Multiply Complex Vectors mmwavelib_vecmul16x16", MCPI_TestResult_FAIL);
    }

	printf("\nBenchmark for mmwavelib_vecmul16x16\nNcplx and cycles:\n");
	for (numCplx = 16; numCplx <= 128; numCplx *= 2)
	{
		t1 = TSCL;
		mmwavelib_vecmul16x16(sptr, wptr, numCplx);
		t2 = TSCL;
		printf("%d\t%d\n", numCplx, (t2-t1));
	}

} /* test_vecmul16x16 */


void test_maxpow( )
{
	int32_t i, numCplx = 64;
	int32_t *inptr = (int32_t *)&databuf[0];
	int32_t outMaxIdx, refMaxIdx=0;
	float32_t outMaxPow=0;
	float32_t refMaxPow=0, cplxSamplePow;
	float32_t * refPow = (float32_t*)&refbuf[0];
	float32_t * outPow = (float32_t*)&data32buf[0];

	/* Fill input with data in range [-RAND_MAX/2, RAND_MAX/2] */
	for ( i=0; i < numCplx*2*2; i++ )
	{
		databuf[i] = rand() - (RAND_MAX/2);
	}

	/* --- test the lib function for max power search --- */ 
	outMaxIdx = mmwavelib_maxpow(inptr, numCplx, &outMaxPow);

	/* reference code */
	for ( i=0; i < numCplx; i++ )
	{
		cplxSamplePow = (float32_t)inptr[2*i]*inptr[2*i] + (float32_t)inptr[2*i+1]*inptr[2*i+1];
		if ( cplxSamplePow > refMaxPow )
		{
			refMaxPow = cplxSamplePow;
			refMaxIdx = i;
		}
		refPow[i] = cplxSamplePow;
	}

	/* compare output with reference */
	if ((outMaxIdx == refMaxIdx) && (outMaxPow == refMaxPow))
	{
		MCPI_setFeatureTestResult("Max power search mmwavelib_maxpow", MCPI_TestResult_PASS);
	}
	else
	{
		MCPI_setFeatureTestResult("Max power search mmwavelib_maxpow", MCPI_TestResult_FAIL);
		totalFailed ++;

		printf("outMaxIdx=%d, refMaxIdx=%d\n", outMaxIdx, refMaxIdx);
		printf("outMaxPow=%f, refMaxPow=%f\n", outMaxPow, refMaxPow);
		printf("I=%d, Q= %d\n", inptr[2*refMaxIdx], inptr[2*refMaxIdx+1]);
	}

	/* --- test the lib function for power calc and max search --- */ 
	outMaxIdx = mmwavelib_powerAndMax(inptr, numCplx, outPow, &outMaxPow);

	/* compare output with reference */
	prevFailed = totalFailed;
	for ( i=0; i < numCplx; i++ )
	{
		if ( refPow[i] != outPow[i])
		{
			totalFailed ++;
			printf("i=%d, I=%d, Q=%d\n", i, inptr[2*i], inptr[2*i+1]);
			printf("i=%d, outPow=%f, refPow=%f\n", i, outPow[i], refPow[i]);
			break;
		}
	}
	if ((outMaxIdx != refMaxIdx) || (outMaxPow != refMaxPow))
	{
		totalFailed ++;

		printf("outMaxIdx=%d, refMaxIdx=%d\n", outMaxIdx, refMaxIdx);
		printf("outMaxPow=%f, refMaxPow=%f\n", outMaxPow, refMaxPow);
		printf("I=%d, Q= %d\n", inptr[2*refMaxIdx], inptr[2*refMaxIdx+1]);
	}

	if ( totalFailed == prevFailed )
	{
		MCPI_setFeatureTestResult("Power calc and Max Search mmwavelib_powerAndMax", MCPI_TestResult_PASS);
	}
	else
	{
		MCPI_setFeatureTestResult("Power calc and Max Search mmwavelib_powerAndMax", MCPI_TestResult_FAIL);
	}

	printf("\nBenchmark for mmwavelib_maxpow\nNcplx and cycles:\n");
	for (numCplx = 16; numCplx <= 128; numCplx *= 2)
	{
		t1 = TSCL;
		outMaxIdx = mmwavelib_maxpow(inptr, numCplx, &outMaxPow);
		t2 = TSCL;
		printf("%d\t%d\n", numCplx, (t2-t1));
	}

	printf("\nBenchmark for mmwavelib_powerAndMax\nNcplx and cycles:\n");
	for (numCplx = 16; numCplx <= 128; numCplx *= 2)
	{
		t1 = TSCL;
		outMaxIdx = mmwavelib_powerAndMax(inptr, numCplx, outPow, &outMaxPow);
		t2 = TSCL;
		printf("%d\t%d\n", numCplx, (t2-t1));
	}

} /* test_maxpow */


void test_power( )
{
	int32_t i, numCplx = 64;
	int32_t *inptr = (int32_t *)&databuf[0];
	float32_t * refPow = (float32_t*)&refbuf[0];
	float32_t * outPow = (float32_t*)&data32buf[0];

	prevFailed = totalFailed;

	/* Fill input with data in range [-RAND_MAX/2, RAND_MAX/2] */
	for ( i=0; i < numCplx*2; i++ )
	{
		inptr[i] = rand() - (RAND_MAX/2);
	}
	for ( i=0; i < numCplx; i++ )
	{
		outPow[i] = 0.0;
	}

	mmwavelib_power(inptr, numCplx, outPow);

	/* reference code */
	for ( i=0; i < numCplx; i++ )
	{
		refPow[i] = (float32_t)inptr[2*i]*inptr[2*i] + (float32_t)inptr[2*i+1]*inptr[2*i+1];
	}

	/* compare output with reference */
	for ( i=0; i < numCplx; i++ )
	{
		if ( refPow[i] != outPow[i])
		{
			totalFailed ++;
			printf("i=%d, I=%d, Q=%d\n", i, inptr[2*i], inptr[2*i+1]);
			printf("i=%d, outPow=%f, refPow=%f\n", i, outPow[i], refPow[i]);
			break;
		}
	}

	if ( totalFailed == prevFailed )
	{
		MCPI_setFeatureTestResult("Power calculation mmwavelib_power", MCPI_TestResult_PASS);
	}
    else
    {
    	MCPI_setFeatureTestResult("Power calculation mmwavelib_power", MCPI_TestResult_FAIL);
    }

	printf("\nBenchmark for mmwavelib_power\nNcplx and cycles:\n");
	for (numCplx = 16; numCplx <= 64; numCplx *= 2)
	{
		t1 = TSCL;
		mmwavelib_power(inptr, numCplx, outPow);
		t2 = TSCL;
		printf("%d\t%d\n", numCplx, (t2-t1));
	}

} /* test_power */

void test_power_float( )
{
	int32_t i, numCplx = 64;
	float32_t *inptr   = (float32_t *)&databuf[0];
	float32_t * refPow = (float32_t*)&refbuf[0];
	float32_t * outPow = (float32_t*)&data32buf[0];

	prevFailed = totalFailed;

	/* Fill input data */
	for ( i=0; i < numCplx*2; i++ )
	{
		inptr[i] = 10.1*(rand() - RAND_MAX/2);
	}
	for ( i=0; i < numCplx; i++ )
	{
		outPow[i] = 0.0;
	}

	mmwavelib_power_float(inptr, numCplx, outPow);

	/* reference code */
	for ( i=0; i < numCplx; i++ )
	{
		refPow[i] = inptr[2*i]*inptr[2*i] + inptr[2*i+1]*inptr[2*i+1];
	}

	/* compare output with reference */
	for ( i=0; i < numCplx; i++ )
	{
		if ( refPow[i] != outPow[i])
		{
			totalFailed ++;
			printf("i=%d, I=%f, Q=%f\n", i, inptr[2*i], inptr[2*i+1]);
			printf("i=%d, outPow=%f, refPow=%f\n", i, outPow[i], refPow[i]);
			break;
		}
	}

	if ( totalFailed == prevFailed )
	{
		MCPI_setFeatureTestResult("Power calculation mmwavelib_power_float", MCPI_TestResult_PASS);
	}
    else
    {
    	MCPI_setFeatureTestResult("Power calculation mmwavelib_power_float", MCPI_TestResult_FAIL);
    }

	printf("\nBenchmark for mmwavelib_power_float\nNcplx and cycles:\n");
	for (numCplx = 16; numCplx <= 64; numCplx *= 2)
	{
		t1 = TSCL;
		mmwavelib_power_float(inptr, numCplx, outPow);
		t2 = TSCL;
		printf("%d\t%d\n", numCplx, (t2-t1));
	}

} /* test_power_float */


uint16_t ref_multiplePeakSearch(float32_t * x,
                                uint16_t * peakIdxBuf,
                                int32_t    len,
                                float32_t  thresholdScale)
{

	uint16_t numDetPeaks = 0;
    uint32_t i, iLeft, iRight;
    uint32_t mask = len - 1;
    float32_t maxPeakVal;
    float32_t threshold;

    maxPeakVal = 0;
    for (i = 0; i < len; i++)
    {
        if (x[i] > maxPeakVal)
        {
            maxPeakVal = x[i];
        }
    }

    threshold = maxPeakVal * thresholdScale;

    for (i = 0; i < len; i++)
    {
        iLeft = (i - 1) & mask;
        iRight = (i + 1) & mask;

        if ((x[i] >= x[iLeft]) && (x[i] >= x[iRight]) && (x[i] > threshold))
        {
            peakIdxBuf[numDetPeaks] = i;
            numDetPeaks++;
        }
    }

    return numDetPeaks;

} /* ref_multiplePeakSearch */

void test_multipeak()
{
	int32_t i, len=64;
	float32_t *inptr = (float32_t *)&databuf[0];
	uint16_t numpeaks;
	uint16_t outpeakIdx[32];
	uint16_t outpeakRef[32];
	uint16_t numpeaksRef;
	float32_t threshscale;

	prevFailed = totalFailed;

	for (i = 0; i < len; i++)
	{
		inptr[i] = 1000.0*rand()/32767;
	}

	for (threshscale = 0; threshscale < 1; threshscale += 0.9)
	{
		numpeaksRef = ref_multiplePeakSearch(inptr, outpeakRef, len, threshscale);
		numpeaks = mmwavelib_multiPeakSearch(inptr, outpeakIdx, len, threshscale);
		printf("len=%d, threshscale = %4.2f: ", len, threshscale);

		if (numpeaksRef == numpeaks)
		{
			printf("Number peaks match.\n");
			for (i=0; i <numpeaks; i++)
			{
				if (outpeakIdx[i] != outpeakRef[i])
				{
					totalFailed++;
					printf("Failed: peakIdx = %d: peakIdxRef = %d\n", outpeakIdx[i], outpeakRef[i]);
				}
			}
		}
		else
		{
			totalFailed++;
			printf("Failed: Number peaks don't match.\n");
		}
	}

	for (i = 0; i < numpeaks; i++)
	{
		printf("idx = %d: val = %f\n", outpeakIdx[i], inptr[outpeakIdx[i]]);
	}

	/* test vector data of len 128 with 2 peaks */
	inptr = &tv2databuf[0];
	len = tv2ref[0];
	threshscale = 0.9;
	numpeaksRef = ref_multiplePeakSearch(inptr, outpeakRef, len, threshscale);
	numpeaks = mmwavelib_multiPeakSearch(inptr, outpeakIdx, len, threshscale);
	printf("len=%d, threshscale = %4.2f: ", len, threshscale);

	if (numpeaksRef == numpeaks)
	{
		printf("Number peaks match.\n");
		for (i=0; i <numpeaks; i++)
		{
			if (outpeakIdx[i] != outpeakRef[i])
			{
				totalFailed++;
				printf("Failed: peakIdx = %d: peakIdxRef = %d\n", outpeakIdx[i], outpeakRef[i]);
			}
		}
	}
	else
	{
		totalFailed++;
		printf("Failed: Number peaks don't match.\n");
	}
	for (i = 0; i < numpeaks; i++)
	{
		printf("idx = %d: val = %f\n", outpeakIdx[i], inptr[outpeakIdx[i]]);
	}

	if ( totalFailed == prevFailed)
	{
		MCPI_setFeatureTestResult("Multiple Peak Search mmwavelib_multiPeakSearch", MCPI_TestResult_PASS);
	}
	else
	{
		MCPI_setFeatureTestResult("Multiple Peak Search mmwavelib_multiPeakSearch", MCPI_TestResult_FAIL);
	}

	printf("\nBenchmark for mmwavelib_multipeak\nLen and cycles:\n");
	for (len = 16; len <= 128; len *= 2)
	{
		t1 = TSCL;
		numpeaks = mmwavelib_multiPeakSearch(inptr, outpeakIdx, len, 0.9);
		t2 = TSCL;
		printf("%d\t%d\n", len, (t2-t1));
	}

} /* test_multipeak */


void test_secondpeak()
{
	int32_t i, len;
	float32_t *inptr;
    float32_t maxval;
	int32_t   maxidx;
	int32_t   secondpeakIdx;
	int32_t   secondpeakRef;
	float32_t data[64];
	int32_t   testid;

	prevFailed = totalFailed;

	/* start with test vector of length 64 */
	inptr = &tv1databuf[0];
	len = tv1ref[0];
	secondpeakRef = tv1ref[2];
	for (testid = 1; testid <= 5; testid ++)
	{
		if (testid == 2)
		{
			/* test vector of length 128 */
			inptr  = &tv2databuf[0];
			len    = tv2ref[0];
			secondpeakRef = tv2ref[2];
		}
		else if (testid == 3)
		{
			/* test vector of length 128 */
			inptr  = &tv3databuf[0];
			len    = tv3ref[0];
			secondpeakRef = tv3ref[2];
		}
		else if (testid == 4)
		{
			/* shift 1st peak position in tv to edge 0 */
			len = tv1ref[0];
			maxidx = tv1ref[1];
			for (i = 0; i < len; i++)
			{
				data[i] = tv1databuf[(maxidx+i)&(len-1)];
			}
			inptr  = &data[0];
			secondpeakRef = (tv1ref[2] - maxidx)&(len-1);
		}
		else if (testid == 5)
		{
			/* shift 1st peak position in tv to the other edge 63 */
			len = tv1ref[0];
			maxidx = tv1ref[1];
			for (i = 0; i < len; i++)
			{
				data[i] = tv1databuf[(maxidx+1+i)&(len-1)];
			}
			inptr  = &data[0];
			secondpeakRef = (tv1ref[2] +63 - maxidx)&(len-1);
		}

		maxval = 0.0;
		for (i = 0; i < len; i++)
		{
			if (inptr[i] > maxval)
			{
				maxval = inptr[i];
				maxidx = i;
			}
		}
		printf("len = %d, maxidx = %d, maxval = %10.2f\n", len, maxidx, maxval);

		secondpeakIdx = maxidx;
		t1 = TSCL;
		secondpeakIdx = mmwavelib_secondPeakSearch(inptr, len, maxidx);
		t2 = TSCL;
		printf("Second peak's idx = %d, val = %10.2f\n", secondpeakIdx, inptr[secondpeakIdx]);
		if ( secondpeakIdx == secondpeakRef )
		{
			printf("Second peak index is correct.\n");
		}
		else
		{
			totalFailed ++;
			printf("Failed: Second peak index =%d, Ref =%d\n", secondpeakIdx, secondpeakRef);
		}
		printf("len and cycles:\n%d\t%d\n\n", len, (t2-t1));
	} /* for testid */

	if ( totalFailed == prevFailed)
	{
		MCPI_setFeatureTestResult("Second Peak Search mmwavelib_secondPeakSearch", MCPI_TestResult_PASS);
	}
	else
	{
		printf("This feature's failed count = %d; totalFailed = %d\n", totalFailed-prevFailed, totalFailed);
		MCPI_setFeatureTestResult("Second Peak Search mmwavelib_secondPeakSearch", MCPI_TestResult_FAIL);
	}

} /* test_secondpeak */


void test_accum16to32()
{
	int32_t i;
	int16_t *inptr = &databuf[0];
	int32_t *outptr = &data32buf[0];
	int32_t *refptr = (int32_t*)&refbuf[0];
	uint32_t length = 64;

	for ( i=0; i < length; i++ )
	{
		inptr[i] = rand() - (RAND_MAX/2);
	}
	for ( i=0; i < length; i++ )
	{
		outptr[i] = (int32_t)(rand() - RAND_MAX/2)*4;
	}

	prevFailed = totalFailed;

	for ( length =2; length <=64; length +=2)
	{
		/* reference code */
		for ( i=0; i < length; i++ )
		{
			refptr[i] = outptr[i] + inptr[i];
		}

		/* lib code */
		mmwavelib_accum16to32(inptr, outptr, length);

		/* compare output with reference */
		for ( i=0; i<length; i++)
		{
			if ( outptr[i] != refptr[i] )
			{
				totalFailed ++;
				printf("len=%d, i=%d, out=%d, ref=%d; \n", length, i, outptr[i], refptr[i]);
				break;
			}
		}
	}

	if ( totalFailed == prevFailed)
	{
		MCPI_setFeatureTestResult("Vector accumulation mmwavelib_accum16to32", MCPI_TestResult_PASS);
	}
	else
	{
		printf("This feature's failed count = %d; totalFailed = %d\n", totalFailed-prevFailed, totalFailed);
		MCPI_setFeatureTestResult("Vector accumulation mmwavelib_accum16to32", MCPI_TestResult_FAIL);
	}

	printf("\nBenchmark for mmwavelib_accum16to32\nlen\tcycles:\n");
	for (length = 8; length <= 64; length +=2)
	{
		t1 = TSCL;
		mmwavelib_accum16to32(inptr, outptr, length);
		t2 = TSCL;
		printf("%d\t%d\n", length, (t2-t1));
	}
	printf("\n");

} /* test_accum16to32 */


void test_shift32to16()
{
	int32_t i;
	int32_t *inptr  = &data32buf[0];
	int16_t *outptr = &databuf[0];
	int16_t *refptr = &refbuf[0];
	uint32_t length = 64;
	uint8_t  shiftnumbits = 5;

	for ( i=0; i < length; i++ )
	{
		refptr[i] = rand() - (RAND_MAX/2);
		inptr[i]  = (int32_t)(refptr[i]) << shiftnumbits;
	}

	prevFailed = totalFailed;

	for ( length =2; length <=64; length +=2)
	{
		/* lib code */
		mmwavelib_shiftright32to16(inptr, outptr, shiftnumbits, length);

		/* compare output with reference */
		for ( i =0; i < length; i++)
		{
			if ( outptr[i] != refptr[i] )
			{
				totalFailed ++;
				printf("len=%d, i=%d, in=%d, out=%d, ref=%d; \n", length, i, inptr[i], outptr[i], refptr[i]);
				break;
			}
		}
	}

	if ( totalFailed == prevFailed)
	{
		MCPI_setFeatureTestResult("Vector shift mmwavelib_shiftright32to16", MCPI_TestResult_PASS);
	}
	else
	{
		printf("This feature's failed count = %d; totalFailed = %d\n", totalFailed-prevFailed, totalFailed);
		MCPI_setFeatureTestResult("Vector shift mmwavelib_shiftright32to16", MCPI_TestResult_FAIL);
	}

	printf("\nBenchmark for mmwavelib_shiftright32to16\nlen\tcycles:\n");
	for (length = 2; length <= 64; length +=2)
	{
		t1 = TSCL;
		mmwavelib_shiftright32to16(inptr, outptr, shiftnumbits, length);
		t2 = TSCL;
		printf("%d\t%d\n", length, (t2-t1));
	}
	printf("\n");

} /* test_shift32to16 */


void test_vecsub16()
{
	int32_t i;
	int16_t *inptr1, *inptr2;
	int16_t *outptr;
	int16_t *refptr = &refbuf[0];
	int32_t length = 64;

	inptr1 = &databuf[0];
	inptr2 = &databuf[length];
	outptr = &databuf[length*2];

	for ( i=0; i < length; i++ )
	{
		inptr1[i] = rand() - (RAND_MAX/2);
		inptr2[i] = rand() - (RAND_MAX/2);
	}

	prevFailed = totalFailed;

	for ( length =2; length <=64; length +=2)
	{
		/* reference code */
		for ( i=0; i < length; i++ )
		{
			refptr[i] = inptr1[i] - inptr2[i];
		}

		/* lib code */
		mmwavelib_vecsub16(inptr1, inptr2, outptr, length);

		/* compare output with reference */
		for ( i=0; i<length; i++)
		{
			if ( outptr[i] != refptr[i] )
			{
				totalFailed ++;
				printf("len=%d, i=%d, out=%d, ref=%d; \n", length, i, outptr[i], refptr[i]);
				break;
			}
		}
	}

	if ( totalFailed == prevFailed)
	{
		MCPI_setFeatureTestResult("Vector subtraction mmwavelib_vecsub16", MCPI_TestResult_PASS);
	}
	else
	{
		printf("This feature's failed count = %d; totalFailed = %d\n", totalFailed-prevFailed, totalFailed);
		MCPI_setFeatureTestResult("Vector subtraction mmwavelib_vecsub16", MCPI_TestResult_FAIL);
	}

	printf("\nBenchmark for mmwavelib_vecsub16\nlen\tcycles:\n");
	outptr = inptr1; /* output can point to the same array as inptr1 */
	printf("len mod 8 = 0:\n");
	for (length = 32; length <= 64; length +=8)
	{
		t1 = TSCL;
		mmwavelib_vecsub16(inptr1, inptr2, outptr, length);
		t2 = TSCL;
		printf("%d\t%d\n", length, (t2-t1));
	}
	printf("len mod 8 = 2:\n");
	for (length = 32+2; length < 64; length +=8)
	{
		t1 = TSCL;
		mmwavelib_vecsub16(inptr1, inptr2, outptr, length);
		t2 = TSCL;
		printf("%d\t%d\n", length, (t2-t1));
	}
	printf("len mod 8 = 4:\n");
	for (length = 32+4; length < 64; length +=8)
	{
		t1 = TSCL;
		mmwavelib_vecsub16(inptr1, inptr2, outptr, length);
		t2 = TSCL;
		printf("%d\t%d\n", length, (t2-t1));
	}
	printf("len mod 8 = 6:\n");
	for (length = 32+6; length < 64; length +=8)
	{
		t1 = TSCL;
		mmwavelib_vecsub16(inptr1, inptr2, outptr, length);
		t2 = TSCL;
		printf("%d\t%d\n", length, (t2-t1));
	}
	printf("\n");

} /* test_vecsub16 */


extern void test_matrix_transpose();
extern void test_dcremoval_float();


void main(void)
{
	TSCL = 0;
	t1 = TSCL;
	t2 = TSCL;
	printf("tsc_overhead = %d cycles\n", (t2-t1));

	MCPI_Initialize();

	printf("======== Testing started: ==========\n");
	totalFailed = 0;
	memset(databuf, 0, sizeof(databuf));
	memset(refbuf, 0, sizeof(refbuf));
	memset(data32buf, 0, sizeof(data32buf));

	printf("\n===== Test mmwavelib_vecsumabs =====\n");
	test_vecsumabs();

	printf("\n===== Test mmwavelib_vecmul16x32 =====\n");
	test_vecmul16x32();

	printf("\n===== Test mmwavelib_vecmul16x16 =====\n");
	test_vecmul16x16();

	printf("\n===== Test mmwavelib_maxpow and mmwavelib_powerAndMax =====\n");
	test_maxpow();

	printf("\n===== Test mmwavelib_power =====\n");
	test_power();

	printf("\n===== Test mmwavelib_power_float =====\n");
	test_power_float();

	printf("\n===== Test mmwavelib_multipeak =====\n");
	test_multipeak();

	printf("\n===== Test mmwavelib_secondpeak =====\n");
	test_secondpeak();

	test_matrix_transpose();

	test_dcremoval_float();

	printf("\n===== Test mmwavelib_accum16to32 =====\n");
	test_accum16to32();

	printf("\n===== Test mmwavelib_shiftright32to16 =====\n");
	test_shift32to16();

	printf("\n===== Test mmwavelib_vecsub16 =====\n");
	test_vecsub16();

	if (totalFailed == 0)
	{
		printf("\n======== Vector math tests and matrix test all passed! ========== \n");
	}
	else
	{
		printf("\n======== One or more tests failed! ======= \n");
	}
	printf("======== Testing completed! ==========\n");
	
	MCPI_setTestResult ();

}
