/*
 *   @file  lib_cfaros_test.c
 *
 *   @brief
 *      Unit Test code for CFAR OS mmwavelib library function
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
#include <ti/alg/mmwavelib/mmwavelib.h>
#include <ti/utils/testlogger/logger.h>

#define MAX_NUM_RANGE_BINS 	1024

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

int32_t totalFailed;
int32_t prevFailed;

uint32_t t1, t2;
uint32_t cycles;

uint16_t detobj[MAX_NUM_RANGE_BINS];
uint16_t kos[MAX_NUM_RANGE_BINS];
uint16_t refdet[MAX_NUM_RANGE_BINS];
uint16_t refkos[MAX_NUM_RANGE_BINS];

/* buffer for input data */
#pragma DATA_SECTION(databuf,".l2data");
#pragma DATA_ALIGN(databuf, 8);
uint16_t databuf[MAX_NUM_RANGE_BINS];

#pragma DATA_SECTION( log2Abs_tv,".l2data");
#pragma DATA_ALIGN( log2Abs_tv,8);
uint16_t log2Abs_tv[256]={ 
/* accumulated log magnitude values from test vector of captured data: */
17648, 18180, 16372, 15455, 14951, 14552, 14194, 13828,
13411, 14005, 13624, 15200, 12070, 19956, 16191, 15056,
16150, 15318, 15276, 17363, 15264, 13439, 15336, 14608,
14613, 13840, 14403, 15932, 13907, 13188, 12165, 13273,
15932, 15003, 15267, 14473, 13816, 11356, 9231, 13098,
13740, 11831, 13324, 12030, 12989, 13324, 13753, 13417,
13237, 12947, 12503, 14228, 13816, 12715, 12798, 14050,
15761, 14436, 14542, 11905, 12758, 13513, 13359, 12824,

13067, 11910, 11443, 11450, 13903, 13023, 7849, 12598,
11369, 11776, 13497, 11393, 12468, 11292, 11978, 10581,
12371, 12249, 11085, 13373, 13231, 12832, 12393, 10148,
10794, 10644, 9736, 12267, 10128, 11243, 11628, 11490,
10624, 11929, 11643, 11221, 11619, 10937, 9859, 10838,
9584, 9666, 10820, 11619, 11640, 11445, 10953, 10427,
10036, 10668, 10459, 10673, 11158, 10060, 7104, 10058,
10868, 11029, 10645, 10091, 10215, 9834, 10205, 10303,

10555, 10077, 10293, 10180, 10268, 10263, 10376, 10332,
10347, 10503, 10449, 10334, 10334, 10480, 10361, 10493,
10227, 10500, 10350, 10519, 10472, 10362, 10393, 10534,
10314, 10250, 10412, 10411, 10265, 10329, 10378, 10654,
10528, 10500, 10226, 10453, 10582, 10211, 10585, 10678,
10267, 10529, 10711, 10350, 10906, 10639, 10662, 10531,
10296, 10678, 10766, 10766, 10791, 10703, 10958, 10759,
10688, 10726, 10783, 11208, 10184, 10685, 10936, 10936,

10933, 10954, 10558, 11215, 10991, 10686, 11107, 10918,
11016, 11208, 10840, 11094, 11125, 11450, 11103, 11501,
11410, 11413, 10868, 11144, 11586, 11338, 11369, 11447,
11550, 11579, 11450, 11678, 11425, 11950, 12251, 11132,
11540, 12010, 11725, 11759, 11914, 12450, 12222, 12047,
12468, 12328, 12579, 12409, 11959, 13627, 12151, 12655,
12820, 12630, 13401, 15672, 12471, 13379, 13258, 13282,
13481, 13641, 13852, 14102, 14392, 14760, 15243, 16261
};

#pragma DATA_SECTION( log2Abs_tv512,".l2data");
#pragma DATA_ALIGN( log2Abs_tv512,8);
uint16_t log2Abs_tv512[512]={
/* accumulated log magnitude values from test vector case1_512: */
27562,27453,27807,25907,27258,28299,27365,27425,26670,26602,26742,27039,27404,26553,27696,27434,
26542,28197,26517,27301,25928,26024,28260,25928,27741,27435,27521,25196,26529,27387,26035,27408,
28512,27448,27687,27433,26992,26958,27281,27739,26249,26386,27804,27600,26655,27245,28328,28326,
27999,27506,27650,26933,28386,26325,27076,26267,26686,26788,26182,25361,27355,27284,26571,27840,
27388,25933,28144,27116,27186,27192,25665,26720,26973,27201,26285,27261,26954,27497,27461,27100,
26830,27338,27715,27324,26175,26789,27425,26398,28688,27206,26867,27453,26326,27180,27371,27115,
28082,27898,26693,27049,26544,27388,27604,26826,27589,27296,25786,27388,26838,27177,28554,27874,
27663,27180,27984,26650,27120,27363,26737,28381,25737,27390,27583,27256,26979,26241,27185,27369,
26773,27422,27400,27264,25564,27164,27467,25859,27830,26516,26980,27788,27240,27725,28167,27791,
28055,27306,26526,27147,27220,27111,27154,26467,27481,27220,26056,27183,24831,27307,27685,27418,
26851,27336,27974,27779,27281,28046,28174,27698,27565,26302,27764,27848,26698,27321,27968,27464,
27203,27151,26740,27918,26481,27063,25715,27430,26454,26923,26076,26390,27436,26385,27704,26820,
28152,26176,27666,27016,26636,27294,26960,27788,26323,27149,27827,26760,27207,27165,24663,27309,
27937,27162,27959,28393,26421,26186,26049,26534,27313,26647,26714,26633,25694,28138,27205,26676,
26926,26976,27654,28796,28555,27654,25322,27708,27908,27744,27910,28347,26084,27815,25990,26523,
26992,26940,27507,26055,27211,27334,26905,27418,28044,27155,26814,27502,26760,27038,28010,26564,
27508,27627,27278,26817,28035,27776,27299,27109,27496,27770,25897,26940,27322,26418,27099,27583,
26955,26024,26110,27184,27714,25640,25766,27277,26097,27282,26183,26366,27579,28383,27678,27914,
26789,25753,27889,25956,27681,27003,26844,27048,26592,27135,26814,27325,27906,27235,28138,27213,
26952,28130,27151,27445,27436,26855,26334,26245,27751,26492,27106,26943,27267,27089,26257,27141,
27081,26560,26780,27092,26091,27457,26682,27015,26555,27859,26977,28380,27437,27689,26610,26029,
28365,26618,27216,26904,26822,26796,26583,27428,26980,27195,26869,26344,27425,26794,26420,27426,
27737,26808,27296,26572,26609,27275,27240,26533,25669,26971,26960,26176,26911,27697,27513,27833,
25834,25968,25636,26855,26545,28205,26887,26366,27330,26811,26325,25915,27529,26869,27273,26861,
27840,28664,27555,26623,25966,28403,26586,26468,27829,27041,27372,29660,30264,32136,34957,35936,//peak
31752,29059,29682,26008,27159,26472,27827,27371,26477,25483,26396,28091,27368,26389,28203,27753,
27549,26615,27149,27938,26184,27288,27002,26935,27523,27126,27910,26384,28502,27694,27055,27132,
28839,27636,26233,25918,26087,26810,27848,25762,27287,27162,27702,27261,26226,27351,26761,26246,
27129,26643,26838,26862,27679,27875,26740,28821,27019,26281,26710,25893,27710,26911,27515,27201,
26456,26721,28135,27532,26852,27752,27439,26677,26195,27544,27491,27971,27305,26806,26748,27429,
26194,26818,27459,26398,26342,27881,26578,27943,27150,27971,27438,26500,27213,27651,26064,26856,
27338,26240,27502,27174,26373,25424,27243,27754,27158,27924,27883,26886,26986,27631,26465,25425
};

/* May use this C Natural function to replace mmwavelib_cfarOS in tests, if needed,
 * for stepping through, understanding the code, etc. Test results also match/pass. */
extern uint16_t mmwavelib_cfarOS_cn(const uint16_t * restrict data,
						const uint32_t len,
						const uint32_t lenCUTend,
						const int16_t k,
						const int16_t alpha,
						uint16_t * restrict detected_list,
						uint16_t * restrict k_os_list);


/* Sorting function used by reference CFAR-OS function */
void sort(uint16_t inp[restrict], uint16_t output[restrict], int32_t len)
{
	int32_t i,j;
	uint16_t tmp;

	for (i = 0; i< len; i++)
	{
		output[i] = inp[i];
	}
	
	for (i = 0; i< len-1; i++)
	{
		/*i=0, first round the largest goes to output[len-1]
		i=1, second round the 2nd-largest goes to output[len-2], ...
		i=30, 31th round the 31th-largest i.e. 2nd smallest goes to out[1] if len=32*/
		for (j = 0; j< len-1-i; j++)
		{
			if (output[j] > output[j+1])
			{
				tmp         = output[j];
				output[j]   = output[j+1];
				output[j+1] = tmp;
			}
		}
	}
} /* sort */

/* Reference CFAR-OS function in simple straightforward C implementation */
uint32_t ref_cfaros(uint16_t input[restrict],
                uint32_t inplen,
                uint32_t cut_end_len,
                uint16_t k,
                uint32_t thresh,
                uint16_t detlist[restrict],
                uint16_t koslist[restrict])
{
	uint32_t i, j;
	uint32_t numDet = 0;
	uint16_t kth_os;
	uint16_t search_window[WINDOW_SIZE+1];
	uint16_t sorted[WINDOW_SIZE+1];

	for (i = 0; i < (WINDOW_SIZE/2); i++)
	{
		search_window[i] = input[0];
		search_window[WINDOW_SIZE/2+i] = input[i]; //search_window[size/2]=input[0] the first CUT
	}

	for (i = 0; i < cut_end_len; i++)
	{
		search_window[WINDOW_SIZE] = input[i+ (WINDOW_SIZE/2)];
		if ( i+ (WINDOW_SIZE/2) >= inplen )
		{
			search_window[WINDOW_SIZE] = input[i+ (WINDOW_SIZE/2) - inplen];
		}
		sort(search_window, sorted, WINDOW_SIZE+1);
		kth_os = sorted[k-1]; //sorted[0] is the 1st smallest, sorted[k-1] is the k-th smallest

#if 0 /* no need this part since CUT in such case is not a peak for sure*/
		if (input[i] <= kth_os) 
		{//if CUT is among k smallest samples, sorted[k] is the k-th os to be accurate
			kth_os = sorted[k];
		}
#endif
		if (((uint32_t)input[i]*64) > (thresh*kth_os))
		{
			detlist[numDet] = i;
			koslist[numDet] = kth_os;
			numDet++;
		}

		/* update search window: slide right by one sample */
		for (j=0; j<=31; j++)
		{
			search_window[j] = search_window[j+1];
		}
	}

	return(numDet);

}/* ref_cfaros */


void test_cfaros(uint16_t * inptr, uint32_t inplen, uint16_t thresh)
{
	uint32_t i;
	uint16_t * outptr;
	uint16_t *refptr;
    uint16_t numDet=0, numDetRef=0;
	uint16_t orderk = 24;

	prevFailed = totalFailed;

	outptr = &detobj[0];
	refptr = &refdet[0];
	printf("\nTest all CUTs' all ordered statistics(k=1 to WINDOW_SIZE) by set threshold to 0:\n");
	for ( orderk = 1; orderk <= WINDOW_SIZE; orderk++ )
	{
		printf("Test %d-th ordered stat: ", orderk);

		numDet = mmwavelib_cfarOS(  inptr,
									inplen,
									inplen,/* test with lenCUTend=len for the full coverage */
									orderk,
									0,/* threshold 0 for detect all CUTs and output kos */
									outptr,
									&kos[0]);

		numDetRef = ref_cfaros(inptr, inplen, inplen, orderk, 0, &refdet[0], &refkos[0]);

		printf("Number of detected object =%d, ref=%d\n", numDet, numDetRef);
		if ( (numDet - numDetRef) !=0 )
		{
			printf("Mismatch on number of detect object.\n");
			totalFailed ++;
			break;
		}

		for ( i=0; i<numDetRef; i++ )
		{
			if ( (outptr[i] - refptr[i])!=0 )
			{
				printf("i=%d: Mismatch on detected object index: out=%d, ref=%d\n",
						i, outptr[i], refptr[i]);
				totalFailed++;
				break;
			}
			//printf("i=%d: %d-th order stat: out=%d, ref=%d\n", i, orderk, kos[i], refkos[i]);
			if ( (kos[i] - refkos[i])!=0 )
			{
				printf("i=%d: Mismatch on %d-th order stat value: out=%d, ref=%d\n",
						i, orderk, kos[i], refkos[i]);
				totalFailed++;
				break;
			}
		}
	}/*for orderk from 1 to WINDOW size, thresh 0*/

    if ( totalFailed == prevFailed)
	{
		printf("CFAR-OS ordered statistics test passed.\n");
	}

	printf("\nTest peak /w thresh= %d/64 = %f:\n", thresh, (float32_t)thresh/64);
	prevFailed = totalFailed;

	for ( orderk = (WINDOW_SIZE/2); orderk <= (WINDOW_SIZE*3/4); orderk++ )
	{
		printf("use %d-th ordered stat: ", orderk);
		numDet = mmwavelib_cfarOS(  inptr,
									inplen,
									inplen,
									orderk,
									thresh,
									outptr,
									&kos[0]);

		numDetRef = ref_cfaros( inptr,
								inplen,
								inplen,
								orderk,
								thresh,
								&refdet[0],
								&refkos[0]);

		printf("Number of detected object =%d, ref=%d\n", numDet, numDetRef);
		if ( numDet != numDetRef )
		{
			printf("Mismatch on number of detect object.\n");
			totalFailed ++;
		}

		for ( i=0; i<numDetRef; i++ )
		{
			if ( i < 3 )
			{
				printf("i=%d: detected object index:%d; peak=%d; kth os=%d\n",
						i, outptr[i], inptr[outptr[i]], kos[i]);
			}
			if ( outptr[i] != refptr[i] )
			{
				printf("i=%d: Mismatch on detected object index: out=%d, ref=%d\n",
						i, outptr[i], refptr[i]);
				totalFailed++;
			}
			if ( kos[i] != refkos[i] )
			{
				printf("i=%d: Mismatch on %d-th order stat value: out=%d, ref=%d\n",
						i, orderk, kos[i], refkos[i]);
				totalFailed++;
			}
		}
	}//for orderk

    if ( totalFailed == prevFailed)
	{
		printf("CFAR-OS peak test passed.\n");
	}

} /* test_cfaros */


void benchmark_cfaros()
{
	uint16_t * inptr, *outptr;
	uint32_t len, maxlen = MAX_NUM_RANGE_BINS;

	uint16_t numDet;
	inptr = &databuf[0];
	outptr = &detobj[0];

	printf("\nBenchmark for mmwavelib_cfarOS /w WINDOW SIZE %d :\n", WINDOW_SIZE);

	/*call once with maxlen to get input data to cache*/
	numDet = mmwavelib_cfarOS(inptr, maxlen, maxlen, (WINDOW_SIZE-1), 160, outptr, &kos[0]);

	printf("length(lenCUTend=len) and cfar cycles:\n");
	for ( len=64 ; len<=maxlen; len=len*2)
	{
		t1 = TSCL;
		numDet = mmwavelib_cfarOS(inptr, len, len, (WINDOW_SIZE/2), 0, outptr,&kos[0]);
		t2 = TSCL;
		printf("%d\t%d\n", len, (t2-t1));
	}
	printf("Number of detected object: %d\n\n", numDet);

	printf("lenCUTend(=len-WINDOW_SIZE/2) and cfar cycles:\n");
	for ( len=64 ; len<=maxlen; len=len*2)
	{
		t1 = TSCL;
		numDet = mmwavelib_cfarOS(inptr,
								len, (len-(WINDOW_SIZE/2)),
								(WINDOW_SIZE/2),
								0,
								outptr,
								&kos[0]);
		t2 = TSCL;
		printf("%d\t%d\n", (len-(WINDOW_SIZE/2)), (t2-t1));
	}
	printf("Number of detected object: %d\n\n", numDet);

}/* benchmark_cfaros*/


void main(void)
{
	uint32_t i;
    uint16_t threshold;

	TSCL = 0;
    t1 = TSCL;
    t2 = TSCL;
    printf("tsc_overhead = %d cycles\n", (t2-t1));

	MCPI_Initialize();

	totalFailed = 0;

	printf("\n ===== Test CFAR-OS =====\n");
	printf("Search Window Size : %d\n", WINDOW_SIZE);

	/* test 256 input data length with test vector data */
	printf("\n--- Input Data Length : 256 ---\n");

	threshold = 79; /*79/64 = 1.234375 */

	test_cfaros(&log2Abs_tv[0], 256, threshold);

	/* test 512 input data length with test vector data */
	printf("\n--- Input Data Length : 512 ---\n");

	/* right shift 1 bit to have all tv data <=32767 */
	{
    uint64_t * inptr;
    uint64_t * outptr;
    uint64_t doubleWord;
    uint32_t upperWord, lowerWord;
    inptr = (uint64_t*)&log2Abs_tv512[0];
    outptr = (uint64_t*)&databuf[0];
    for(i = 0; i < 512/4U; i++)
    {
        doubleWord = inptr[i]; /*four 16-bit input*/
        lowerWord = _shru2(_loll(doubleWord), 1);
        upperWord = _shru2(_hill(doubleWord), 1);
        outptr[i] = _itoll(upperWord, lowerWord);
    }
	}
	threshold = 70;

    test_cfaros(databuf, 512, threshold);

	/* test 1024 input data length with random data */
	printf("\n--- Input Data Length : %d ---\n", MAX_NUM_RANGE_BINS);
	for ( i=0; i<MAX_NUM_RANGE_BINS; i++ )
	{
		databuf[i] = rand();
	}

	threshold = 128;
	test_cfaros(&databuf[0], MAX_NUM_RANGE_BINS, threshold);

	/* Benchmarking */
	benchmark_cfaros();

	if ( totalFailed == 0)
	{
        MCPI_setFeatureTestResult("CFAR-OS", MCPI_TestResult_PASS);
	}
    else
    {
        MCPI_setFeatureTestResult("CFAR-OS", MCPI_TestResult_FAIL);
    }

	if (totalFailed == 0)
	{
		printf("\n========== CFAR-OS tests passed! ========== \n");
	}
	else
	{
		printf("\n======== CFAR-OS tests Failed! ======= \n");
	}
	printf("========== Testing completed! ==========\n");
	
	MCPI_setTestResult ();

}


