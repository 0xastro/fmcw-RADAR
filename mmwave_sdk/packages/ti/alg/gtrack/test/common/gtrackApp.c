/*
 *   @file  gtrackApp.c
 *
 *   @brief
 *      Gtrack Application Unit Test code
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016 Texas Instruments, Inc.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

/* BIOS/XDC Include Files. */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/heaps/HeapBuf.h>
#include <ti/sysbios/heaps/HeapMem.h>

/* mmWave SK Include Files: */
#include <ti/common/sys_common.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/esm/esm.h>
#include <ti/utils/cycleprofiler/cycle_profiler.h>
#include <ti/utils/testlogger/logger.h>

#include <ti/alg/gtrack/gtrack.h>
#include "math.h"
#include "float.h"
#include "testlimits.h"

extern far void* _gtrackLibStart;
extern far void* _gtrackLibEnd;


/* This test application wants to modify default parameters */
GTRACK_sceneryParams appSceneryParams = {
	1,{{-4.0f,4.0f,0.5f,7.5f,-1.0f,3.0f}, {0.f,0.f,0.f,0.f,0.f,0.f}},    /* one boundary box {x1,x2,y1,y2,z1,z2} */
    1,{{-3.0f,3.0f,2.0f,6.0f,-0.5f,2.5f}, {0.f,0.f,0.f,0.f,0.f,0.f}} 	/* one static box {x1,x2,y1,y2,z1,z2} */
};
GTRACK_gatingParams appGatingParams = {
     2.f, {2.f, 2.f, 2.f, 0.f}    /* Gating Volume 2 "litters", Limits are set to 2m in depth, width, 2m (if applicable) in height and no limits in doppler */
};
GTRACK_stateParams appStateParams = {
     10U, 5U, 50U, 100U, 5U              /* det2act, det2free, act2free, stat2free, exit2free */
};
GTRACK_allocationParams appAllocationParams = {
     60.f, 200.f, 0.1f, 5U, 1.5f, 2.f           /* 60 in clear, 200 obscured SNRs, 0.1m/s minimal velocity, 5 points, 1.5m in distance, 2m/s in velocity */
};
/* Using standard deviation of uniformly distributed variable in the range [a b]: 1/sqrt(12)*(b-a) */
GTRACK_varParams appVariationParams = {
     /* Standard deviation of uniformly distributed number in range [a b]: sqrt(1/12)*(b-a) */
     0, 0, 0, 0     /* 1m in width, depth, and height, 2 m/s for doppler */
};

typedef struct {
    uint32_t    frameNum;
    uint32_t    numTLVs;

} binFileHeader;

typedef enum {
    POINT_CLOUD_TYPE = 6,
    TARGET_LIST_TYPE
} binFileElements;

typedef struct {
    uint32_t type;
    uint32_t length;
} binFileTLV;

typedef struct {
    uint32_t tid;
    float S[GTRACK_STATE_VECTOR_SIZE];
} checkTargetDescr;


GTRACK_measurementPoint pointCloud[GTRACK_NUM_POINTS_MAX];
GTRACK_targetDesc targetDescr[GTRACK_NUM_TRACKS_MAX];

extern far uint32_t memoryBytesUsed;

/**
 *  @b Description
 *  @n
 *      System Initialization Task which initializes the various
 *      components in the system.
 *
 *  @retval
 *      Not Applicable.
 */
void Test_initTask(UArg arg0, UArg arg1)
{
	GTRACK_moduleConfig config;
    GTRACK_advancedParameters advParams;


    checkTargetDescr checkDescr[20];

    void *hTrackModule;
    bool testResult = true;
    bool performanceTestResultPass = true;
    bool memoryLeakTestResultPass = true;
    bool frameError = false;

    uint32_t gtrackStartTime;
    uint32_t benchmarkCycles;
    uint32_t benchmarks[GTRACK_BENCHMARK_SIZE+1];

    int32_t errCode;

    Memory_Stats            initMemoryStats;
    Memory_Stats            startMemoryStats;
    Memory_Stats            peakMemoryStats;
    Memory_Stats            endMemoryStats;

    binFileHeader frameHeader;
    uint32_t fileNumber;
    size_t result;
	
	uint32_t gtick;	
    uint16_t mNum;
    uint16_t tNum;
    uint16_t tCheckNum;

	uint32_t n, k;

	FILE *fCloud;
	char fileName[120];
    binFileTLV tlv;
    float distX,distY;
    bool tidFound;
    GTRACK_boundaryBox *box;

    uint32_t programBytesUsed;

    uint32_t dataBytesUsedInit;
    uint32_t dataBytesUsedStart;
    uint32_t dataBytesUsedPeak;
    uint32_t dataBytesUsedEnd;

    uint32_t benchmarkPerTrack;
    uint64_t benchmarkPerTrackTotal = 0;
    uint32_t benchmarkPerTrackCount = 0;
    uint32_t benchmarkPerTrackAve;
    uint32_t benchmarkPerTrackMax = 0;
    uint32_t benchmarkPerTrackMin = 0xFFFFFFFF;

    uint32_t cyclesPerSecond;
    uint32_t cyclesPerCCNT;
    float uSecondsPerCycle;

#if 1
    uint32_t start,end;
    start = (uint32_t)&_gtrackLibStart;
	end = (uint32_t)&_gtrackLibEnd;
    programBytesUsed = end - start;
#endif

	MCPI_Initialize();

    /* Get the heap statistics at the beginning of the tests */
    HeapMem_getStats (heap0, &initMemoryStats);
    System_printf ("Total Heap Size = %d bytes\n", initMemoryStats.totalSize);
    System_printf ("HeapMem: Used = %d bytes, Free = %d bytes\n", initMemoryStats.totalSize - initMemoryStats.totalFreeSize, initMemoryStats.totalFreeSize);
    dataBytesUsedInit = memoryBytesUsed;
    
	memset((void *)&config, 0, sizeof(GTRACK_moduleConfig));
#ifdef GTRACK_3D
    System_printf("Gtrack is configured for 3D\n");
	config.stateVectorType = GTRACK_STATE_VECTORS_3DA; // Track three dimensions with acceleration 
#else
    System_printf("Gtrack is configured for 2D\n");
	config.stateVectorType = GTRACK_STATE_VECTORS_2DA; // Track two dimensions with acceleration 
#endif
	config.verbose = GTRACK_VERBOSE_NONE;
	config.deltaT = 0.05f; // 50ms frames
	config.maxRadialVelocity = 5.29f; // Radial velocity from sensor is limited to +/- maxURV (in m/s)
	config.radialVelocityResolution = 0.083f; // Radial velocity resolution (in m/s)
	config.maxAcceleration[0] = 0.1f; // Target acceleration is not excedding 0.5 m/s2 in lateral direction
	config.maxAcceleration[1] = 0.1f; // Target acceleration is not excedding 0.5 m/s2 in longitudinal direction
	config.maxAcceleration[2] = 0.1f; // Target acceleration is not excedding 0.5 m/s2 in vertical direction
	config.maxNumPoints = 250;
	config.maxNumTracks = 20;
	config.initialRadialVelocity = 0; // Expected target radial velocity at the moment of detection, m/s
	
	
    /* Here, we want to set allocation, gating, and threshold parameters, leaving the rest to default */
	memset((void *)&advParams, 0, sizeof(GTRACK_advancedParameters));
	advParams.allocationParams = &appAllocationParams;
	advParams.gatingParams = &appGatingParams;
	advParams.stateParams = &appStateParams;
    advParams.sceneryParams = &appSceneryParams;
    advParams.variationParams = &appVariationParams;

	config.advParams = &advParams;

    System_printf("Tracker Configuration\n");
    System_printf("\tstateVectorType: %d\n", config.stateVectorType);
    System_printf("\tmaxNumPoints: %d\n", config.maxNumPoints);
    System_printf("\tmaxNumTracks: %d\n", config.maxNumTracks);
    System_printf("\tmaxRadialVelocity: %f\n", config.maxRadialVelocity);
    System_printf("\tradialVelocityResolution: %f\n", config.radialVelocityResolution);
    System_printf("\tdeltaT: %f\n", config.deltaT);
	
    System_printf("\tinitialRadialVelocity: %f\n", config.initialRadialVelocity);
    System_printf("\tmaxAcceleration: [%f, %f, %f]\n", config.maxAcceleration[0], config.maxAcceleration[1], config.maxAcceleration[2]);
	
    System_printf("\tscenery:\n");
    for (n = 0; n < config.advParams->sceneryParams->numBoundaryBoxes; n++) {
        box = &config.advParams->sceneryParams->boundaryBox[n];
        System_printf("\t\t BoundaryBox %d: [%f, %f, %f, %f, %f, %f]\n", n, box->x1, box->x2, box->y1, box->y2,box->z1, box->z2);
    }
    for (n = 0; n < config.advParams->sceneryParams->numBoundaryBoxes; n++) {
        box = &config.advParams->sceneryParams->staticBox[n];
        System_printf("\t\t StaticBox %d: [%f, %f, %f, %f, %f, %f]\n", n, box->x1, box->x2, box->y1, box->y2,box->z1, box->z2);
    }
    System_printf("\tallocation: [%f, %f, %f, %d, %f, %f]\n", config.advParams->allocationParams->snrThre, config.advParams->allocationParams->snrThreObscured, config.advParams->allocationParams->velocityThre, config.advParams->allocationParams->pointsThre, config.advParams->allocationParams->maxDistanceThre, config.advParams->allocationParams->maxVelThre);
    System_printf("\tgating: [%f, %f, %f, %f, %f]\n", config.advParams->gatingParams->volume, config.advParams->gatingParams->limitsArray[0], config.advParams->gatingParams->limitsArray[1], config.advParams->gatingParams->limitsArray[2], config.advParams->gatingParams->limitsArray[3]);
    System_printf("\tthresholds: [%d, %d, %d, %d, %d]\n", config.advParams->stateParams->det2actThre, config.advParams->stateParams->det2freeThre, config.advParams->stateParams->active2freeThre, config.advParams->stateParams->static2freeThre, config.advParams->stateParams->exit2freeThre);
	
	hTrackModule = gtrack_create(&config, &errCode);
	
    /* Get the heap statistics at the beginning of the tests */
    HeapMem_getStats (heap0, &startMemoryStats);
    dataBytesUsedStart = memoryBytesUsed - dataBytesUsedInit;
	
	Cycleprofiler_init();
	gtrackStartTime = gtrack_getCycleCount();
	for(n=0; n<1000; n++) {
        /* one milisecond */
        SOC_microDelay(1000);
	}
	cyclesPerSecond = gtrack_getCycleCount() - gtrackStartTime;
    System_printf("Calibration: 1 seconds is %u Cycless\n", cyclesPerSecond);
    uSecondsPerCycle = 1000000.f/cyclesPerSecond;

    gtrackStartTime = gtrack_getCycleCount();
    cyclesPerCCNT = gtrack_getCycleCount() - gtrackStartTime;
    System_printf("Calibration: CCNT read is %u Cycles\n", cyclesPerCCNT);


    fileNumber = 1;
	
    while (1) {
#ifdef GTRACK_3D
		sprintf(fileName, "../../../test/vectors/usecases/people_counting/fHistScene4_3d_%04u.bin", fileNumber);
#else
        sprintf(fileName, "../../../test/vectors/usecases/people_counting/fHistScene4_2d_%04u.bin", fileNumber);
#endif
		fCloud = fopen(fileName, "rb");
        if(fCloud == 0)
            break;
		
        while(1) {
            // Read frame Header
            result = fread(&frameHeader, sizeof(frameHeader), 1, fCloud);
            if(result != 1)
                break;

            frameError = false;
            mNum = 0;
            tCheckNum = 0;

            for(n = 0; n < frameHeader.numTLVs; n++) {
                result = fread(&tlv, 8, 1, fCloud);
                if(result != 1)
                    break;

                switch(tlv.type) {
                    case 6:
                        result = fread(pointCloud, tlv.length, 1, fCloud);
                        if(result != 1)
                            break;

                        mNum = (uint16_t)tlv.length/sizeof(GTRACK_measurementPoint);
                        break;

                    case 7:
                        result = fread(checkDescr, tlv.length, 1, fCloud);
                        if(result != 1)
                            break;
                        tCheckNum = (uint16_t)tlv.length/sizeof(checkTargetDescr);
                        break;

                    default:
                        break;
                }
            }

            gtick = frameHeader.frameNum;

            // Limit the points
            if(mNum > config.maxNumPoints)
                mNum = config.maxNumPoints;

            gtrackStartTime = gtrack_getCycleCount();
            gtrack_step(hTrackModule, pointCloud, 0, mNum, targetDescr, &tNum, 0, benchmarks);
            if(tCheckNum != tNum) {
                System_printf("Error, Frame #%u: missmatched number of targets\n", gtick);
                performanceTestResultPass = false;
                frameError = true;
            }
            for(n=0; n < tNum; n++) {
                tidFound = false;
                for(k=0; k < tNum; k++) {
                    if(targetDescr[n].uid == checkDescr[k].tid) {
                        tidFound = true;
                        break;
                    }
                }

                if(tidFound == false) {
                    System_printf("Error, Frame #%u: tid not found\n", gtick);
                    performanceTestResultPass = false;
                    frameError = true;
                    break;
                }

                distX = targetDescr[n].S[0]-checkDescr[k].S[0];
                distY = targetDescr[n].S[1]-checkDescr[k].S[1];
                if(sqrt(distX*distX+distY*distY) > 2) {
                    System_printf("Error, Frame #%u: missmatched distance\n", gtick);
                    performanceTestResultPass = false;
                    frameError = true;
                    break;
                }
            }

            benchmarkCycles = gtrack_getCycleCount() - gtrackStartTime;

            if(tNum) {
                benchmarkPerTrack = (uint32_t)benchmarkCycles/tNum;
                benchmarkPerTrackTotal += benchmarkPerTrack;
                benchmarkPerTrackCount += 1;

                if(benchmarkPerTrack < benchmarkPerTrackMin)
                    benchmarkPerTrackMin = benchmarkPerTrack;
                if(benchmarkPerTrack > benchmarkPerTrackMax)
                    benchmarkPerTrackMax = benchmarkPerTrack;
            }
            System_printf("Frame #%u, %u Targets, %u Points, %u Tracks, %u Cycles = (", gtick, tCheckNum, mNum, tNum, benchmarkCycles);
            for(n=0; n<GTRACK_BENCHMARK_SIZE; n++) {
                System_printf("%u, ", benchmarks[n] - gtrackStartTime);
                gtrackStartTime = benchmarks[n];

            }
            System_printf("%u)", benchmarks[n] - gtrackStartTime);

            if(frameError)
                System_printf("... ERROR\n");
            else
                System_printf("... OK\n");
        }
        fileNumber ++;
        fclose(fCloud);
    }

    HeapMem_getStats (heap0, &peakMemoryStats);
    dataBytesUsedPeak = memoryBytesUsed - dataBytesUsedInit;

    gtrack_delete(hTrackModule);

    HeapMem_getStats (heap0, &endMemoryStats);
    dataBytesUsedEnd = memoryBytesUsed - dataBytesUsedInit;


    System_printf("**************************************************************************************************************\n");
    if(config.stateVectorType == GTRACK_STATE_VECTORS_3DA)
        System_printf("Tracking in 3D space\n");
    else
        System_printf("Tracking in 2D space\n");

    System_printf("Configured for %d maxPoints and %d maxTracks\n", config.maxNumPoints, config.maxNumTracks);
    System_printf("Gtrack Library is using %d bytes of program memory\n", programBytesUsed);
    System_printf("Gtrack Library is using %d bytes of data memory\n", dataBytesUsedPeak);

    benchmarkPerTrackAve = (uint32_t)(benchmarkPerTrackTotal/benchmarkPerTrackCount);
    System_printf("Cycles, per target {mean, min, max} = %u, %u, %u\n", benchmarkPerTrackAve, benchmarkPerTrackMin, benchmarkPerTrackMax);
    System_printf("uSec, per target {mean, min, max} = %u, %u, %u\n", (uint32_t)(benchmarkPerTrackAve*uSecondsPerCycle), (uint32_t)(benchmarkPerTrackMin*uSecondsPerCycle), (uint32_t)(benchmarkPerTrackMax*uSecondsPerCycle));

	/* Test Results */
    /* Performance Test */
	if(performanceTestResultPass != true) {
        System_printf("ERROR: Tracking performance does not match with reference\n");
        testResult = false;
	}
	else
        System_printf("PERFORMANCE TEST PASSED\n");

	/* Memory Leak Test */
	if((dataBytesUsedEnd != 0) || (initMemoryStats.totalFreeSize != endMemoryStats.totalFreeSize)) {
        System_printf("ERROR: Memory leak after deletion\n");
        memoryLeakTestResultPass = false;
        testResult = false;
    }
    if((dataBytesUsedStart != dataBytesUsedPeak) || (startMemoryStats.totalFreeSize != peakMemoryStats.totalFreeSize)) {
	    System_printf("ERROR: Run time memory leak detected\n");
        memoryLeakTestResultPass = false;
	    testResult = false;
	}
    if(memoryLeakTestResultPass)
        System_printf("MEMORY LEAK TEST PASSED\n");

	/* Program memory check */
    if(programBytesUsed > LIMIT_PROG_MEMORY_SIZE)
        System_printf("Warning: Program memory size check exceeds configured limit\n");
    else
        System_printf("PROGRAM MEMORY SIZE TEST PASSED\n");

	/* Data memory check */
	if(dataBytesUsedPeak > (LIMIT_DATA_MEMORY_SIZE_MODULE + config.maxNumTracks*LIMIT_DATA_MEMORY_SIZE_UNIT))
        System_printf("Warning: Data memory size check exceeds configured limit\n");
    else
        System_printf("DATA MEMORY SIZE TEST PASSED\n");

	/* Run time benchmark check */
	if((benchmarkPerTrackAve > LIMIT_USEC_PER_TRACK_AVE/uSecondsPerCycle) || (benchmarkPerTrackMax > LIMIT_USEC_PER_TRACK_MAX/uSecondsPerCycle))
        System_printf("Warning: Run time benchmark check exceeds configured limits\n");
    else
        System_printf("BENCHMARK TEST PASSED\n");

	if(testResult == false) {
        System_printf("TEST FAILED\n");
    	MCPI_setFeatureTestResult("Group Tracker", MCPI_TestResult_FAIL);	
	}
	else {
        System_printf("ALL TEST PASSED\n");
    	MCPI_setFeatureTestResult("Group Tracker", MCPI_TestResult_PASS);	
	}
	System_printf("**************************************************************************************************************\n");
    
	MCPI_setTestResult();	
	BIOS_exit(0);
	return;
}
