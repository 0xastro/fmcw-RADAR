/*
 *   @file  lib_dbscanClustering_test.c
 *
 *   @brief
 *      Unit Test code for DBscan Clustering mmwavelib library functions
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

#define MAXNUMPOINTS 1000
#define MAXNUMCLUSTERS 10
#define MAXLOCATIONERROR 4
#define MAXSPEEDERROR 1.f

/* input location  */
#pragma DATA_SECTION(location2D,".l2data");
cplx16_t location2D[MAXNUMPOINTS ];

/* input speed */
#pragma DATA_SECTION(speedBuffer,".l2data");
int16_t speedBuffer[MAXNUMPOINTS];

/* scratch buffer */
#pragma DATA_SECTION(tempBuf,".l2data");
#pragma DATA_ALIGN(tempBuf, 8);
int8_t tempBuf[4*MAXNUMPOINTS];

/* cluster array */
#pragma DATA_SECTION(clusterIdx,".l2data");
uint16_t clusterIdx[MAXNUMPOINTS];

#pragma DATA_SECTION(clusterIdxRef,".l2data");
uint16_t clusterIdxRef[MAXNUMPOINTS];

#pragma DATA_SECTION(clusterParam,".l2data");
mmwavelib_clusterInfor clusterParam[MAXNUMCLUSTERS];


void test_dbscan_modSym()
{
	uint32_t numTests,ii,jj;
	uint16_t numPoints;
	int32_t epsilon;
	uint16_t numClusters,numClustersRef;
	int16_t meanXRef, meanYRef, sizeXRef,sizeYRef;
	float  varRange;
	uint16_t numPointsInClusterRef;

	/* for this tests, the speed is set to 0 */
	memset(speedBuffer,0,sizeof(int16_t)*MAXNUMPOINTS);

	/* read in number of tests */
	fread(&numTests,sizeof(uint32_t),1,fid);
	for (ii=0;ii<numTests;ii++)
	{
		printf("modulated data test %d\n",ii);
		/* read in the number of points */
		fread(&numPoints,sizeof(uint16_t),1,fid);
		/* read in the locations */
		fread(location2D,sizeof(int16_t),2*numPoints,fid);
		/* read in the threshold */
		fread(&epsilon,sizeof(int32_t),1,fid);
		/* read in reference for skip version first */
		fread(clusterIdxRef,sizeof(uint16_t), numPoints,fid);

		/* call skip found neighbor version */
		mmwavelib_dbscan_skipFoundNeiB(location2D,
									speedBuffer,
									epsilon,
									1.f,  /* speed is 0, any weight*/
									numPoints,
									20,  /* minimum point is hard code to 20 */
									tempBuf,
									&numClusters,
									clusterIdx,
									clusterParam);

		for (jj=0;jj<numPoints;jj++)
		{
			if (clusterIdxRef[jj]!=clusterIdx[jj])
			{
				 testPASS = 0;
				 printf("dbscan skip found neighbor: test=%d,pointIdx=%d, ref=%d,lib=%d\n",ii,jj, clusterIdxRef[jj],clusterIdx[jj]);

			}
		}

		/* read in clusterId reference */
		fread(clusterIdxRef,sizeof(uint16_t), numPoints,fid);
		/* read in numberof cluster reference */
		fread(&numClustersRef,sizeof(uint16_t),1,fid);
		/* cluster */
		mmwavelib_dbscan(location2D,
							speedBuffer,
							epsilon,
							1.f,  /* speed is 0, any weight*/
							numPoints,
							20,  /* minimum point is hard code to 20 */
							tempBuf,
							&numClusters,
							clusterIdx,
							clusterParam);

		/* check results */
		if (numClustersRef!=numClusters)
		{
			 testPASS = 0;
			 printf("number of clusters: test=%d,ref=%d,lib=%d\n",ii,numClustersRef,numClusters);

		}

		for (jj=0;jj<numPoints;jj++)
		{
			if (clusterIdxRef[jj]!=clusterIdx[jj])
			{
				 testPASS = 0;
				 printf("clusterId: test=%d,pointIdx=%d, ref=%d,lib=%d\n",ii,jj, clusterIdxRef[jj],clusterIdx[jj]);

			}
		}



		for (jj=0;jj<numClusters;jj++)
		{
			/*read in reference number of points in cluster*/
			fread(&numPointsInClusterRef,sizeof(uint16_t),1,fid);
			if (numPointsInClusterRef!=clusterParam[jj].numPointsInCluster)
			{
				 testPASS = 0;
				 printf("numPointsIncluster: test=%d,clusterIdx=%d, ref=%d,lib=%d\n",ii,jj, numPointsInClusterRef,clusterParam[jj].numPointsInCluster);

			}

			/* read in mean x */
			fread(&meanXRef,sizeof(int16_t),1,fid);
			if ((meanXRef-clusterParam[jj].xCenter > MAXLOCATIONERROR) || (meanXRef-clusterParam[jj].xCenter < -1.f* MAXLOCATIONERROR))
			{
				 testPASS = 0;
				 printf("cluster X mean: test=%d,clusterIdx=%d, ref=%d,lib=%d\n",ii,jj, meanXRef,clusterParam[jj].xCenter);

			}
			fread(&meanYRef,sizeof(int16_t),1,fid);
			if (((meanYRef-clusterParam[jj].yCenter) > MAXLOCATIONERROR) || ((meanYRef-clusterParam[jj].yCenter) < -1.f*MAXLOCATIONERROR))
			{
				 testPASS = 0;
				 printf("cluster Y mean: test=%d,clusterIdx=%d, ref=%d,lib=%d\n",ii,jj, meanYRef,clusterParam[jj].yCenter);
			}

			/* read in cluster size */
			fread(&sizeXRef,sizeof(int16_t),1,fid);
			if (((sizeXRef-clusterParam[jj].xSize) > MAXLOCATIONERROR) || ((sizeXRef-clusterParam[jj].xSize) < -1.f* MAXLOCATIONERROR))
			{
				 testPASS = 0;
				 printf("cluster X size: test=%d,clusterIdx=%d, ref=%d,lib=%d\n",ii,jj, sizeXRef,clusterParam[jj].xSize);
			}

			fread(&sizeYRef,sizeof(int16_t),1,fid);
			if (((sizeYRef-clusterParam[jj].ySize) > MAXLOCATIONERROR) || ((sizeYRef-clusterParam[jj].ySize) < -1.f*MAXLOCATIONERROR))
			{
				 testPASS = 0;
				 printf("cluster Y size: test=%d,clusterIdx=%d, ref=%d,lib=%d\n",ii,jj, sizeYRef,clusterParam[jj].ySize);
			}

			/* range variance */
			fread(&varRange,sizeof(float),1,fid);
			if (((varRange-clusterParam[jj].rangeVar) > MAXLOCATIONERROR) || ((varRange-clusterParam[jj].rangeVar) < -1.f* MAXLOCATIONERROR))
			{
				 testPASS = 0;
				 printf("cluster range variance: test=%d,clusterIdx=%d, ref=%f,lib=%f\n",ii,jj, varRange,clusterParam[jj].rangeVar);
			}

			/* speed */
			if (clusterParam[jj].aveVelocity)
			{
				 testPASS = 0;
				 printf("cluster ave speed: test=%d,clusterIdx=%d, lib=%d\n",ii,jj, clusterParam[jj].aveVelocity);

			}

			if (clusterParam[jj].velocityVar)
			{
				 testPASS = 0;
				 printf("cluster speed variance: test=%d,clusterIdx=%d, lib=%f\n",ii,jj, clusterParam[jj].velocityVar);
			}

		}


	}




}


void test_dbscan_randomData()
{
	uint32_t numTests,ii,jj;
	uint16_t numPoints;
	int32_t epsilon;
	float weight;
	uint16_t minPoints;
	uint16_t numClusters,numClustersRef;
	int16_t meanXRef, meanYRef, sizeXRef,sizeYRef, meanSpeed;
	float varSpeed,varRange;
	uint16_t numPointsInClusterRef;

	/* number of tests */
	fread(&numTests,sizeof(uint32_t),1,fid);
	/* read in epsilo*/
	fread(&epsilon,sizeof(float),1,fid);
	/* read in weight */
	fread(&weight,sizeof(float),1,fid);
	/* read in minimum points */
	fread(&minPoints, sizeof(uint16_t),1,fid);


	for (ii = 0; ii < numTests; ii++)
	{
		printf("random data test %d\n",ii);
		/* read in number of data */
		fread(&numPoints,sizeof(uint16_t),1,fid);
		/* read in data */
		fread(location2D,sizeof(int16_t),2*numPoints,fid);
		/* read in speed */
		fread(speedBuffer,sizeof(int16_t),numPoints,fid);
		/* read in clusteridx reference for skip version*/
		fread(clusterIdxRef,sizeof(uint16_t),numPoints,fid);
		/* cluster */
		mmwavelib_dbscan_skipFoundNeiB(location2D,
									      speedBuffer,
									      epsilon,
									      weight,
									      numPoints,
									      minPoints,  /* minimum point is hard code to 20 */
									      tempBuf,
									      &numClusters,
									      clusterIdx,
									      clusterParam);
		/* check results */
		for (jj=0;jj<numPoints;jj++)
		{
			if (clusterIdxRef[jj]!=clusterIdx[jj])
			{
				 testPASS = 0;
				 printf("dbscan skip found neighbors: test=%d,pointIdx=%d, ref=%d,lib=%d\n",ii,jj, clusterIdxRef[jj],clusterIdx[jj]);

			}
		}
		/* read in clusteridx reference */
		fread(clusterIdxRef,sizeof(uint16_t),numPoints,fid);
		/* read in numberof cluster reference */
		fread(&numClustersRef,sizeof(uint16_t),1,fid);


		/* cluster */
		mmwavelib_dbscan(location2D,
							speedBuffer,
							epsilon,
							weight,
							numPoints,
							minPoints,  /* minimum point is hard code to 20 */
							tempBuf,
							&numClusters,
							clusterIdx,
							clusterParam);
		/* check results */
		if (numClustersRef!=numClusters)
		{
			 testPASS = 0;
			 printf("number of clusters: test=%d,ref=%d,lib=%d\n",ii,numClustersRef,numClusters);
		}

		for (jj=0;jj<numPoints;jj++)
		{
			if (clusterIdxRef[jj]!=clusterIdx[jj])
			{
				 testPASS = 0;
				 printf("clusterId: test=%d,pointIdx=%d, ref=%d,lib=%d\n",ii,jj, clusterIdxRef[jj],clusterIdx[jj]);

			}
		}

		for (jj=0;jj<numClusters;jj++)
		{
			/*read in reference number of points in cluster*/
			fread(&numPointsInClusterRef,sizeof(uint16_t),1,fid);
			if (numPointsInClusterRef!=clusterParam[jj].numPointsInCluster)
			{
				 testPASS = 0;
				 printf("numPointsIncluster: test=%d,clusterIdx=%d, ref=%d,lib=%d\n",ii,jj, numPointsInClusterRef,clusterParam[jj].numPointsInCluster);

			}

			/* read in mean x */
			fread(&meanXRef,sizeof(int16_t),1,fid);
			if ((meanXRef-clusterParam[jj].xCenter > MAXLOCATIONERROR) || (meanXRef-clusterParam[jj].xCenter < -1.f* MAXLOCATIONERROR))
			{
				 testPASS = 0;
				 printf("cluster X mean: test=%d,clusterIdx=%d, ref=%d,lib=%d\n",ii,jj, meanXRef,clusterParam[jj].xCenter);
			}
			fread(&meanYRef,sizeof(int16_t),1,fid);
			if (((meanYRef-clusterParam[jj].yCenter) > MAXLOCATIONERROR) || ((meanYRef-clusterParam[jj].yCenter) < -1.f*MAXLOCATIONERROR))
			{
				 testPASS = 0;
				 printf("cluster Y mean: test=%d,clusterIdx=%d, ref=%d,lib=%d\n",ii,jj, meanYRef,clusterParam[jj].yCenter);
			}

			/* read in cluster size */
			fread(&sizeXRef,sizeof(int16_t),1,fid);
			if (((sizeXRef-clusterParam[jj].xSize) > MAXLOCATIONERROR) || ((sizeXRef-clusterParam[jj].xSize) < -1.f* MAXLOCATIONERROR))
			{
				 testPASS = 0;
				 printf("cluster X size: test=%d,clusterIdx=%d, ref=%d,lib=%d\n",ii,jj, sizeXRef,clusterParam[jj].xSize);
			}
			fread(&sizeYRef,sizeof(int16_t),1,fid);
			if (((sizeYRef-clusterParam[jj].ySize) > MAXLOCATIONERROR) || ((sizeYRef-clusterParam[jj].ySize) < -1.f*MAXLOCATIONERROR))
			{
				 testPASS = 0;
				 printf("cluster Y size: test=%d,clusterIdx=%d, ref=%d,lib=%d\n",ii,jj, sizeYRef,clusterParam[jj].ySize);
			}

			/* range variance */
			fread(&varRange,sizeof(float),1,fid);
			if (((varRange-clusterParam[jj].rangeVar) > MAXLOCATIONERROR) || ((varRange-clusterParam[jj].rangeVar) < -1.f* MAXLOCATIONERROR))
			{
				 testPASS = 0;
				 printf("cluster range variance: test=%d,clusterIdx=%d, ref=%f,lib=%f\n",ii,jj, varRange,clusterParam[jj].rangeVar);
			}

			/* read in speed mean */
			fread(&meanSpeed,sizeof(int16_t),1,fid);
			if((clusterParam[jj].aveVelocity - meanSpeed > MAXSPEEDERROR) || (clusterParam[jj].aveVelocity - meanSpeed < -1.f * MAXSPEEDERROR))
			{
				 testPASS = 0;
				 printf("cluster ave speed: test=%d,clusterIdx=%d, ref=%d, lib=%d\n",ii,jj, meanSpeed,clusterParam[jj].aveVelocity);
			}
			/* read in speed variance */
			fread(&varSpeed,sizeof(float),1,fid);

			if ((clusterParam[jj].velocityVar - varSpeed >MAXSPEEDERROR ) || (clusterParam[jj].velocityVar - varSpeed < -1.f*MAXSPEEDERROR ))
			{
				 testPASS = 0;
				 printf("cluster veloctiy variance: test=%d,clusterIdx=%d, ref=%f, lib=%f\n",ii,jj, varSpeed,clusterParam[jj].velocityVar);
			}

		}
	}

}

void test_dbscan_benchmark()
{
	uint16_t ii,jj;
	mmwavelib_clusterInfor  clusterInf;
	uint16_t * neighBorList;
	uint16_t numPoints;
	uint16_t numClusters;
	uint16_t numTests;
	uint8_t numOjbs[MAXNUMCLUSTERS];


	printf("DBSCAN find neighbor benchmark\n");
	printf("numOfPoints, cycles\n");
	memset(tempBuf,0,sizeof(uint8_t)*MAXNUMPOINTS);
	for (ii = 100; ii<MAXNUMPOINTS; ii+=50)
	{
		t1=TSCL;

		mmwavelib_dbscan_findNeighbors2(location2D,
				                             speedBuffer,
											 0,
											 4000,
											 1.f,
											 ii,
											 tempBuf,
											(uint16_t*)& tempBuf[MAXNUMPOINTS]
											 );
		t2=TSCL;
		cycles = t2-t1;
		printf("%d\t%d\n", ii, cycles);

	}

	printf("DBSCAN calculate cluster infor benchmark\n");
	printf("numOfPointsInCluster, cycles\n");
	neighBorList = (uint16_t *)tempBuf;
	for (ii = 0; ii < MAXNUMPOINTS;ii+=2)
	{
		neighBorList[ii] = ii;
		neighBorList[ii+1] = MAXNUMPOINTS-ii;
	}
	for (ii = 100; ii<MAXNUMPOINTS; ii+=50)
	{
		t1=TSCL;

		mmwavelib_dbscan_calCluster(location2D,
				                         speedBuffer,
										 ii,
										 neighBorList,  /* neighbor list */
										 &clusterInf
									    );
		t2=TSCL;
		cycles = t2-t1;
		printf("%d\t%d\n", ii, cycles);

	}

	fread(&numTests,sizeof(uint16_t),1,fid);
	fread(numOjbs,sizeof(uint8_t),numTests,fid);
	memset(speedBuffer,0,sizeof(int16_t)*MAXNUMPOINTS);

	for (ii = 0; ii < numTests; ii++)
	{
		printf("DBSCAN benchmark\n");
		printf("numOfClustersEst, numOfPoints, cycles\n");
		numPoints = 200;
		if (numOjbs[ii]==5)
		{
			numPoints = 300;
		}

		if (numOjbs[ii]==8)
		{
			numPoints = 500;
		}


		fread(location2D,sizeof(int16_t),MAXNUMPOINTS*2,fid);
		for (jj=numPoints;jj<MAXNUMPOINTS+100;jj+=100)
		{
			mmwavelib_dbscan(location2D,
								  speedBuffer,
								  1000,
								  1.f,
								  jj,
								  40,
								  tempBuf,
								  &numClusters,
								  clusterIdx,
								  clusterParam);

			t1=TSCL;
			mmwavelib_dbscan(location2D,
					              speedBuffer,
					              1000,
					              1.f,
					              jj,
					              40,
					              tempBuf,
					              &numClusters,
					              clusterIdx,
					              clusterParam);

			t2=TSCL;
			cycles = t2-t1;
			printf("%d\t%d\t%d\n", numClusters, jj, cycles);
		}

		printf("DBSCAN with skipped neighbors benchmark\n");
		printf("numOfCluster, numOfPoints, cycles\n");

		for (jj=numPoints;jj<MAXNUMPOINTS+100;jj+=100)
		{
			mmwavelib_dbscan_skipFoundNeiB(location2D,
										              speedBuffer,
										              1000,
										              1.f,
										              jj,
										              40,
										              tempBuf,
										              &numClusters,
										              clusterIdx,
										              clusterParam);

			t1=TSCL;
			mmwavelib_dbscan_skipFoundNeiB(location2D,
							              speedBuffer,
							              1000,
							              1.f,
							              jj,
							              40,
							              tempBuf,
							              &numClusters,
							              clusterIdx,
							              clusterParam);
			t2=TSCL;
			cycles = t2-t1;
			printf("%d\t%d\t%d\n", numClusters, jj, cycles);
		}


	}
}
void main(void)
{
	/*
	int32_t  weightFixed;
	int64_t  tempResLo;
	int32_t  temp1,temp3, temp4;
	int64_t temp2;

	weightFixed = 0x10000000;
	tempResLo = 0x0000000100000024;


	temp1 = _loll(tempResLo);
	temp2 = _mpyhill(weightFixed,temp1);
	temp3 = _mpyhir(weightFixed,temp1);
	temp4 = temp3<<3;

	*/


	MCPI_Initialize();
	TSCL = 0;
	printf("..... Clustering dbscan test  .......\n");

	fid = fopen("..\\testdata\\clustering_test.bin","rb");
	if (fid==NULL)
	{
	  	printf("File Open Error: clustering_test.bin can not open\n");
	   	exit(3);
	}


	printf("Clustering DBSCAN test using modulated symbols\n");
	test_dbscan_modSym();

	printf("Clustering DBSCN test using random data\n");
	test_dbscan_randomData();

	/* bench mark */
	test_dbscan_benchmark();

	fclose(fid);

	if (testPASS)
	{
		printf("========== Fixed point dbscan clustering tests all passed! ========== \n");
	}
	else
	{
		printf("==========  Fixed point dbscan clustering tests Failed! ========== \n");
	}
	printf("========== dbscan Fixed-point clustering Testing completed! ==========\n");

	MCPI_setTestResult ();
}
