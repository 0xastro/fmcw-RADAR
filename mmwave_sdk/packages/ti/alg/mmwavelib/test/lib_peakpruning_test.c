/*
 *   @file  lib_peakpruning_test.c
 *
 *   @brief
 *      Unit Test code for peak group/pruning mmwavelib library functions
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


#ifdef DEBUGPEAKPRUNING
#define DEBUGPREVIOUSAPI 1
#else
#define DEBUGPREVIOUSAPI 0
#endif
/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#if DEBUGPREVIOUSAPI
#include "lib_peakpruning_debug.h"
#else
#include <ti/alg/mmwavelib/mmwavelib.h>
#endif
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
int16_t testPASS=1;  
uint32_t t1,t2;
uint32_t  cycles;

#define MAXRANGEBIN 512
#define MAXDOPPLERBIN 64
#define MAXDETPEAKS  200
#define MAXDETOBJS 100


/* input signal power  */
#pragma DATA_SECTION(powerMatrix,".l2data");
uint16_t powerMatrix[MAXRANGEBIN * MAXDOPPLERBIN ];

#if DEBUGPREVIOUSAPI
/* cfar peak output */
#pragma DATA_SECTION(cfarDetPeaks,".l2data");
mmwavelib_detObj cfarDetPeaks[MAXDETPEAKS ];
/* peak pruning output */
#pragma DATA_SECTION(detObjs,".l2data");
mmwavelib_detObj detObjs[MAXDETOBJS ];
#endif

#pragma DATA_SECTION(grpPeakIndex,".l2data");
uint16_t grpPeakIndex[MAXDETOBJS ];


/* cfar peak range index */
#pragma DATA_SECTION(peakRangeIdx,".l2data");
uint16_t peakRangeIdx[MAXDETPEAKS ];
#pragma DATA_SECTION(peakDopplerIdx,".l2data");
uint16_t peakDopplerIdx[MAXDETPEAKS ];


#pragma DATA_SECTION(peakValue,".l2data");
uint16_t peakValue[MAXDETPEAKS ];


/* range/doppler reference index */
#pragma DATA_SECTION(detObjIdxRef,".l2data");
int16_t detObjIdxRef[MAXDETOBJS*2 ];

uint32_t numDetObj;
uint32_t numDetPeaks;
uint32_t numDopplerBin;
uint16_t grpPeakRangeIdx, grpPeakDopplerIdx;


void test_peakpruning()
{
    uint32_t numTests;
    uint32_t testIdx;
    uint32_t numRangeBin;
    uint16_t minRangeBin, maxRangeBin;
    uint16_t numDetObjRef;

    uint16_t ii,jj;

    fread(&numTests,sizeof(uint32_t),1,fid);    /*number of tests */

    for (testIdx=0;testIdx<numTests;testIdx++)
    {
        /* read in number of range and doppler bin */
        fread(&numRangeBin, sizeof(uint32_t),1,fid);
        fread(&numDopplerBin,sizeof(uint32_t),1,fid);

        printf("Test: numRangeBin: %d, numDopplerBin: %d\n",numRangeBin, numDopplerBin);
        /* read in signal power */
        fread(powerMatrix,sizeof(uint16_t), numRangeBin*numDopplerBin,fid);
        /* for each set of matrix size, test two different min/max range bin number */
        for (ii= 0;ii < 2; ii++)
        {
            printf("test Id %d\n", ii);
            /* read in min/max range bin */
            fread(&minRangeBin,sizeof(uint16_t),1,fid);
            fread(&maxRangeBin,sizeof(uint16_t),1,fid);

            /* read in number of peaks, and corresponding range/dopper index */
            fread(&numDetPeaks,sizeof(uint32_t),1,fid);
            fread(peakRangeIdx,sizeof(uint16_t),numDetPeaks, fid);
            fread(peakDopplerIdx,sizeof(uint16_t),numDetPeaks,fid);
#if DEBUGPREVIOUSAPI
            /* set up the input for lib function */
            for (jj=0;jj<numDetPeaks;jj++)
            {
                cfarDetPeaks[jj].rangeIdx = peakRangeIdx[jj];
                cfarDetPeaks[jj].dopplerIdx = peakDopplerIdx[jj];
                cfarDetPeaks[jj].peakVal = powerMatrix[peakRangeIdx[jj]*numDopplerBin+peakDopplerIdx[jj]];
            }
             /* debug */
            //numDetPeaks = 1;
            //cfarDetPeaks[0].rangeIdx = 96;
            //cfarDetPeaks[0].dopplerIdx = 0;
            //cfarDetPeaks[0].peakVal = powerMatrix[96*numDopplerBin+0];

#endif           
            /*call the lib, both range and doppler direction first */
#if DEBUGPREVIOUSAPI
            numDetObj = lib_cfarPeakPruning(
                                    detObjs,
                                    cfarDetPeaks,
                                    numDetPeaks,
                                    powerMatrix,
                                    numDopplerBin,
                                    maxRangeBin,
                                    minRangeBin,
                                    1,
                                    1,
                                    MAXDETOBJS);
#else
            numDetObj = mmwavelib_cfarPeakPruning(
                                        grpPeakIndex,
                                        peakRangeIdx,
                                        peakDopplerIdx,
                                        numDetPeaks,
                                        powerMatrix,
                                        numDopplerBin,
                                        maxRangeBin,
                                        minRangeBin,
                                        1,
                                        1,
                                        MAXDETOBJS);
#endif
            /* read in reference */
            fread(&numDetObjRef,sizeof(uint16_t),1,fid);
            fread(detObjIdxRef,sizeof(int16_t),2*numDetObjRef,fid);

            /* check results */
            if (numDetObj!=numDetObjRef)
            {
                testPASS = 0;
                printf("Peakpruning test(both direction) fails at number of detected objects\n");
            }

            for (jj=0;jj<numDetObj;jj++)
            {
#if DEBUGPREVIOUSAPI
                grpPeakRangeIdx = detObjs[jj].rangeIdx;
                grpPeakDopplerIdx = detObjs[jj].dopplerIdx;
#else
                 grpPeakRangeIdx = peakRangeIdx[grpPeakIndex[jj]];
                 grpPeakDopplerIdx = peakDopplerIdx[grpPeakIndex[jj]];
#endif
               	if (grpPeakRangeIdx!=detObjIdxRef[2*jj])
               	{
                    testPASS = 0;
                    printf("Peakpruning test(both direction) fails at range idx for objects %d, Ref=%d, Output=%d\n",jj,detObjIdxRef[2*jj], grpPeakRangeIdx);
                }
                if (grpPeakDopplerIdx!=detObjIdxRef[2*jj+1])
                {
                    testPASS = 0;
                    printf("Peakpruning test(both direction) fails at doppler idx for objects %d,, Ref=%d, Output=%d\n",jj,detObjIdxRef[2*jj+1],grpPeakDopplerIdx);
                }
            }
            /*call the lib,  range direction */
#if DEBUGPREVIOUSAPI
            numDetObj = lib_cfarPeakPruning(
                                            detObjs,
                                            cfarDetPeaks,
                                            numDetPeaks,
                                            powerMatrix,
                                            numDopplerBin,
                                            maxRangeBin,
                                            minRangeBin,
                                            0,
                                            1,
                                            MAXDETOBJS);
#else
            
            numDetObj = mmwavelib_cfarPeakPruning(
                                                grpPeakIndex,
                                                peakRangeIdx,
                                                peakDopplerIdx,
                                                numDetPeaks,
                                                powerMatrix,
                                                numDopplerBin,
                                                maxRangeBin,
                                                minRangeBin,
                                                0,
                                                1,
                                                MAXDETOBJS);
#endif
            /* read in reference */
            fread(&numDetObjRef,sizeof(uint16_t),1,fid);
            fread(detObjIdxRef,sizeof(int16_t),2*numDetObjRef,fid);

            /* check results */
            if (numDetObj!=numDetObjRef)
            {
                testPASS = 0;
                printf("Peakpruning test(range direction) fails at number of detected objects\n");
            }

            for (jj=0;jj<numDetObj;jj++)
            {
#if DEBUGPREVIOUSAPI
                grpPeakRangeIdx = detObjs[jj].rangeIdx;
                grpPeakDopplerIdx = detObjs[jj].dopplerIdx;
#else
                 grpPeakRangeIdx = peakRangeIdx[grpPeakIndex[jj]];
                 grpPeakDopplerIdx = peakDopplerIdx[grpPeakIndex[jj]];
#endif
               if (grpPeakRangeIdx!=detObjIdxRef[2*jj])
               {
                    testPASS = 0;
                    printf("Peakpruning test(range direction) fails at range idx for objects %d, Ref=%d, Output=%d\n",jj,detObjIdxRef[2*jj], grpPeakRangeIdx);
                }
                if (grpPeakDopplerIdx!=detObjIdxRef[2*jj+1])
                {
                    testPASS = 0;
                    printf("Peakpruning test(range direction) fails at doppler idx for objects %d,, Ref=%d, Output=%d\n",jj,detObjIdxRef[2*jj+1],grpPeakDopplerIdx);
                }
            }

            /*call the lib,  doppler direction */
 #if DEBUGPREVIOUSAPI
            numDetObj =lib_cfarPeakPruning(
                                        detObjs,
                                        cfarDetPeaks,
                                        numDetPeaks,
                                        powerMatrix,
                                        numDopplerBin,
                                        maxRangeBin,
                                        minRangeBin,
                                        1,
                                        0,
                                        MAXDETOBJS);
#else
            numDetObj = mmwavelib_cfarPeakPruning(
                                        grpPeakIndex,
                                        peakRangeIdx,
                                        peakDopplerIdx,
                                        numDetPeaks,
                                        powerMatrix,
                                        numDopplerBin,
                                        maxRangeBin,
                                        minRangeBin,
                                        1,
                                        0,
                                        MAXDETOBJS);
#endif
            /* read in reference */
            fread(&numDetObjRef,sizeof(uint16_t),1,fid);
            fread(detObjIdxRef,sizeof(int16_t),2*numDetObjRef,fid);

            /* check results */
            if (numDetObj!=numDetObjRef)
            {
               testPASS = 0;
               printf("Peakpruning test(doppler direction) fails at number of detected objects\n");
            }

            for (jj=0;jj<numDetObj;jj++)
            {
#if DEBUGPREVIOUSAPI
                grpPeakRangeIdx = detObjs[jj].rangeIdx;
                grpPeakDopplerIdx = detObjs[jj].dopplerIdx;
#else
                 grpPeakRangeIdx = peakRangeIdx[grpPeakIndex[jj]];
                 grpPeakDopplerIdx = peakDopplerIdx[grpPeakIndex[jj]];
#endif
                 if (grpPeakRangeIdx!=detObjIdxRef[2*jj])
                {
                    testPASS = 0;
                    printf("Peakpruning test(doppler direction) fails at range idx for objects %d, Ref=%d, Output=%d\n",jj,detObjIdxRef[2*jj], grpPeakRangeIdx);
                }
                if (grpPeakDopplerIdx!=detObjIdxRef[2*jj+1])
                {
                    testPASS = 0;
                    printf("Peakpruning test(doppler direction) fails at doppler idx for objects %d,, Ref=%d, Output=%d\n",jj,detObjIdxRef[2*jj+1],grpPeakDopplerIdx);
                }
            }

        }/* end of two tests*/
    }/* end of numRangebin,numDopplerbin loop */
    
    if (testPASS)
    {
        MCPI_setFeatureTestResult("cfarPeakPruning Test", MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult("cfarPeakPruning Test", MCPI_TestResult_FAIL);
    }

}/*end of functionality test */

void benchmark_peakpruning()
{
    uint32_t ii;
    uint32_t t1,t2;
    uint32_t  cycles;


    /* read in data */
    fread(powerMatrix,sizeof(uint16_t), 512*64,fid);
    fread(&numDetPeaks,sizeof(uint32_t),1,fid);
    fread(peakRangeIdx,sizeof(uint16_t),numDetPeaks, fid);
    fread(peakDopplerIdx,sizeof(uint16_t),numDetPeaks,fid);

    numDopplerBin  = 64;
#if DEBUGPREVIOUSAPI
    /* set up the input for lib function */
    for (ii=0;ii<numDetPeaks;ii++)
    {
        cfarDetPeaks[ii].rangeIdx = peakRangeIdx[ii];
        cfarDetPeaks[ii].dopplerIdx = peakDopplerIdx[ii];
        cfarDetPeaks[ii].peakVal = powerMatrix[peakRangeIdx[ii]*numDopplerBin+peakDopplerIdx[ii]];
    }
#endif
    
    printf("peak pruning benchmark, both directions\n");
    printf("number of peaks, cycles\n");
    /* get the warm cache benchmark*/
#if DEBUGPREVIOUSAPI
    numDetObj = lib_cfarPeakPruning(
                                    detObjs,
                                    cfarDetPeaks,
                                    100,
                                    powerMatrix,
                                    numDopplerBin,
                                    511,
                                    0,
                                    1,
                                    1,
                                    MAXDETOBJS);
#else
    numDetObj = mmwavelib_cfarPeakPruning(
                                        grpPeakIndex,
                                        peakRangeIdx,
                                        peakDopplerIdx,
                                        100,
                                        powerMatrix,
                                        numDopplerBin,
                                        511,
                                        0,
                                        1,
                                        1,
                                        MAXDETOBJS);
#endif
    for (ii=10;ii<110;ii+=10)
    {
        t1 = TSCL;
        /*call the lib, both range and doppler direction first */
#if DEBUGPREVIOUSAPI
        numDetObj = lib_cfarPeakPruning(
                                    detObjs,
                                    cfarDetPeaks,
                                    ii,
                                    powerMatrix,
                                    numDopplerBin,
                                    511,
                                    0,
                                    1,
                                    1,
                                    MAXDETOBJS);
#else
        numDetObj = mmwavelib_cfarPeakPruning(
                                        grpPeakIndex,
                                        peakRangeIdx,
                                        peakDopplerIdx,
                                        ii,
                                        powerMatrix,
                                        numDopplerBin,
                                        511,
                                        0,
                                        1,
                                        1,
                                        MAXDETOBJS);
#endif
        t2 = TSCL;
        cycles = t2-t1;
        printf("%d\t%d\n",ii,cycles);
    }
    
    
    printf("peak pruning benchmark, Doppler direction\n");
    printf("number of peaks, cycles\n");
    
    for (ii=10;ii<110;ii+=10)
    {
        t1 = TSCL;
        /*call the lib, both range and Doppler direction first */
#if DEBUGPREVIOUSAPI
        numDetObj = lib_cfarPeakPruning(
                                        detObjs,
                                        cfarDetPeaks,
                                        ii,
                                        powerMatrix,
                                        numDopplerBin,
                                        511,
                                        0,
                                        1, /* doppler direction */
                                        0,
                                        MAXDETOBJS);
#else
        numDetObj = mmwavelib_cfarPeakPruning(
                                        grpPeakIndex,
                                        peakRangeIdx,
                                        peakDopplerIdx,
                                        ii,
                                        powerMatrix,
                                        numDopplerBin,
                                        511,
                                        0,
                                        1, /* doppler direction */
                                        0,
                                        MAXDETOBJS);
#endif
        t2 = TSCL;
        cycles = t2-t1;
        printf("%d\t%d\n",ii,cycles);
    }
    
    
    printf("peak pruning benchmark, Range direction\n");
    printf("number of peaks, cycles\n");
    
    for (ii=10;ii<110;ii+=10)
    {
        t1 = TSCL;
        /*call the lib, both range and Doppler direction first */
#if DEBUGPREVIOUSAPI
        numDetObj = lib_cfarPeakPruning(
                                    detObjs,
                                    cfarDetPeaks,
                                    ii,
                                    powerMatrix,
                                    numDopplerBin,
                                    511,
                                    0,
                                    0, /* Doppler direction */
                                    1, /* Range direction */
                                    MAXDETOBJS);
#else       
        numDetObj = mmwavelib_cfarPeakPruning(
                                        grpPeakIndex,
                                        peakRangeIdx,
                                        peakDopplerIdx,
                                        ii,
                                        powerMatrix,
                                        numDopplerBin,
                                        511,
                                        0,
                                        0, /* Doppler direction */
                                        1, /* Range direction */
                                        MAXDETOBJS);
#endif
        t2 = TSCL;
        cycles = t2-t1;
        printf("%d\t%d\n",ii,cycles);
    }
}

void test_peakpruningQualifiedinorder()
{
    uint32_t numTests;
    uint32_t testIdx;
    uint32_t numRangeBin;
    uint16_t minRangeBin, maxRangeBin;
    uint16_t numDetObjRef;
   
    uint16_t ii,jj;
    int16_t tempTestPass=1;

    fread(&numTests,sizeof(uint32_t),1,fid);    /*number of tests */

    for (testIdx=0;testIdx<numTests;testIdx++)
    {
        /* read in number of range and doppler bin */
        fread(&numRangeBin, sizeof(uint32_t),1,fid);
        fread(&numDopplerBin,sizeof(uint32_t),1,fid);

        printf("Test: numRangeBin: %d, numDopplerBin: %d\n",numRangeBin, numDopplerBin);
        
        for (ii= 0;ii < 2; ii++)
        {
            printf("test Id %d\n", ii);
            /* read in min/max range bin */
            fread(&minRangeBin,sizeof(uint16_t),1,fid);
            fread(&maxRangeBin,sizeof(uint16_t),1,fid);

            /* read in number of peaks, and corresponding range/dopper index */
            fread(&numDetPeaks,sizeof(uint32_t),1,fid);
            fread(peakRangeIdx,sizeof(uint16_t),numDetPeaks, fid);
            fread(peakDopplerIdx,sizeof(uint16_t),numDetPeaks,fid);
            fread(peakValue,sizeof(uint16_t),numDetPeaks,fid);
            /* set up the input for lib function */
#if DEBUGPREVIOUSAPI
            for (jj=0;jj<numDetPeaks;jj++)
            {
                cfarDetPeaks[jj].rangeIdx = peakRangeIdx[jj];
                cfarDetPeaks[jj].dopplerIdx = peakDopplerIdx[jj];
                cfarDetPeaks[jj].peakVal = peakValue[jj];
            }
            
            numDetObj = lib_cfarPeakQualifiedInOrderPruning( 
                                                detObjs,
                                                cfarDetPeaks,
                                                numDetPeaks,
                                                numDopplerBin,
                                                maxRangeBin,
                                                minRangeBin,
                                                1,
                                                1,
                                                MAXDETOBJS);
#else
            numDetObj = mmwavelib_cfarPeakQualifiedInOrderPruning( 
                                                grpPeakIndex,
                                                peakRangeIdx,
                                                peakDopplerIdx,
                                                peakValue,
                                                numDetPeaks,
                                                numDopplerBin,
                                                maxRangeBin,
                                                minRangeBin,
                                                1,
                                                1,
                                                MAXDETOBJS);
#endif
            /* read in reference */
            fread(&numDetObjRef,sizeof(uint16_t),1,fid);
            fread(detObjIdxRef,sizeof(int16_t),2*numDetObjRef,fid);

            /* check results */
            if (numDetObj!=numDetObjRef)
            {
                testPASS = 0;
                tempTestPass = 0;
                printf("Peakpruning test(both direction) fails at number of detected objects ref=%d, det=%d\n",numDetObjRef,numDetObj);
            }

            for (jj=0;jj<numDetObj;jj++)
            {
#if DEBUGPREVIOUSAPI
                grpPeakRangeIdx = detObjs[jj].rangeIdx;
                grpPeakDopplerIdx = detObjs[jj].dopplerIdx;
#else
                grpPeakRangeIdx = peakRangeIdx[grpPeakIndex[jj]];
                grpPeakDopplerIdx = peakDopplerIdx[grpPeakIndex[jj]];
                            
#endif
                if (grpPeakRangeIdx!=detObjIdxRef[2*jj])
                {
                    testPASS = 0;
                    tempTestPass = 0;
                    printf("Peakpruning test(both direction) fails at range idx for objects %d, Ref=%d, Output=%d\n",jj,detObjIdxRef[2*jj], grpPeakRangeIdx);
                }
                if (grpPeakDopplerIdx!=detObjIdxRef[2*jj+1])
                {
                    testPASS = 0;
                    tempTestPass = 0;
                    printf("Peakpruning test(both direction) fails at doppler idx for objects %d,, Ref=%d, Output=%d\n",jj,detObjIdxRef[2*jj+1],grpPeakDopplerIdx);
                }
            }
            /*call the lib,  range direction */
#if DEBUGPREVIOUSAPI
             numDetObj = lib_cfarPeakQualifiedInOrderPruning(
                                                detObjs,
                                                cfarDetPeaks,
                                                numDetPeaks,
                                                numDopplerBin,
                                                maxRangeBin,
                                                minRangeBin,
                                                0,
                                                1,
                                                MAXDETOBJS);
#else             
            numDetObj = mmwavelib_cfarPeakQualifiedInOrderPruning(
                                                grpPeakIndex,
                                                peakRangeIdx,
                                                peakDopplerIdx,
                                                peakValue,
                                                numDetPeaks,
                                                numDopplerBin,
                                                maxRangeBin,
                                                minRangeBin,
                                                0,
                                                1,
                                                MAXDETOBJS);
#endif
            /* read in reference */
            fread(&numDetObjRef,sizeof(uint16_t),1,fid);
            fread(detObjIdxRef,sizeof(int16_t),2*numDetObjRef,fid);

            /* check results */
            if (numDetObj!=numDetObjRef)
            {
                testPASS = 0;
                tempTestPass = 0;
                printf("Peakpruning test(range direction) fails at number of detected objects ref=%d, det=%d\n",numDetObjRef,numDetObj);
            }

            for (jj=0;jj<numDetObj;jj++)
            {
#if DEBUGPREVIOUSAPI
                grpPeakRangeIdx = detObjs[jj].rangeIdx;
                grpPeakDopplerIdx = detObjs[jj].dopplerIdx;
#else
                grpPeakRangeIdx = peakRangeIdx[grpPeakIndex[jj]];
                grpPeakDopplerIdx = peakDopplerIdx[grpPeakIndex[jj]];
                            
#endif
                if (grpPeakRangeIdx!=detObjIdxRef[2*jj])
                {
                    testPASS = 0;
                    tempTestPass = 0;
                    printf("Peakpruning test(range direction) fails at range idx for objects %d, Ref=%d, Output=%d\n",jj,detObjIdxRef[2*jj], grpPeakRangeIdx);
                }
                if (grpPeakDopplerIdx!=detObjIdxRef[2*jj+1])
                {
                    testPASS = 0;
                    tempTestPass = 0;
                    printf("Peakpruning test(range direction) fails at doppler idx for objects %d,, Ref=%d, Output=%d\n",jj,detObjIdxRef[2*jj+1],grpPeakDopplerIdx);
                }
            }

            /*call the lib,  doppler direction */
#if DEBUGPREVIOUSAPI
           numDetObj = lib_cfarPeakQualifiedInOrderPruning(
                                               detObjs,
                                               cfarDetPeaks,
                                               numDetPeaks,
                                               numDopplerBin,
                                               maxRangeBin,
                                               minRangeBin,
                                               1,
                                               0,
                                               MAXDETOBJS);
#else
           
            numDetObj = mmwavelib_cfarPeakQualifiedInOrderPruning(
                                                grpPeakIndex,
                                                peakRangeIdx,
                                                peakDopplerIdx,
                                                peakValue,
                                                numDetPeaks,
                                                numDopplerBin,
                                                maxRangeBin,
                                                minRangeBin,
                                                1,
                                                0,
                                                MAXDETOBJS);
#endif
            /* read in reference */
            fread(&numDetObjRef,sizeof(uint16_t),1,fid);
            fread(detObjIdxRef,sizeof(int16_t),2*numDetObjRef,fid);

            /* check results */
            if (numDetObj!=numDetObjRef)
            {
                testPASS = 0;
                tempTestPass = 0;
                printf("Peakpruning test(doppler direction) fails at number of detected objectsref=%d, det=%d\n",numDetObjRef,numDetObj);
            }

            for (jj=0;jj<numDetObj;jj++)
            {
#if DEBUGPREVIOUSAPI
                grpPeakRangeIdx = detObjs[jj].rangeIdx;
                grpPeakDopplerIdx = detObjs[jj].dopplerIdx;
#else
                grpPeakRangeIdx = peakRangeIdx[grpPeakIndex[jj]];
                grpPeakDopplerIdx = peakDopplerIdx[grpPeakIndex[jj]];
                            
#endif
                if (grpPeakRangeIdx!=detObjIdxRef[2*jj])
                {
                    testPASS = 0;
                    tempTestPass = 0;
                    printf("Peakpruning test(doppler direction) fails at range idx for objects %d, Ref=%d, Output=%d\n",jj,detObjIdxRef[2*jj], grpPeakRangeIdx);
                }
                if (grpPeakDopplerIdx!=detObjIdxRef[2*jj+1])
                {
                    testPASS = 0;
                    tempTestPass = 0;
                    printf("Peakpruning test(doppler direction) fails at doppler idx for objects %d,, Ref=%d, Output=%d\n",jj,detObjIdxRef[2*jj+1],grpPeakDopplerIdx);
                }
            }
			
	}
    }
    
    if (tempTestPass)
    {
        MCPI_setFeatureTestResult("cfar Qualified Peaks (in-order) Pruning Test", MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult("cfar Qualified Peaks (in-order) Pruning Test", MCPI_TestResult_FAIL);
    }
}


void benchmark_peakpruningQualifiedinorder()
{
    uint32_t ii;
    uint32_t t1,t2;
    uint32_t  cycles;


    /* read in data */
    fread(&numDetPeaks,sizeof(uint32_t),1,fid);
    fread(peakRangeIdx,sizeof(uint16_t),numDetPeaks, fid);
    fread(peakDopplerIdx,sizeof(uint16_t),numDetPeaks,fid);
    fread(peakValue,sizeof(uint16_t),numDetPeaks,fid);
    
                 
    numDopplerBin  = 64;
    /* set up the input for lib function */
#if DEBUGPREVIOUSAPI
    for (ii=0;ii<numDetPeaks;ii++)
    {
        cfarDetPeaks[ii].rangeIdx = peakRangeIdx[ii];
	cfarDetPeaks[ii].dopplerIdx = peakDopplerIdx[ii];
	cfarDetPeaks[ii].peakVal = peakValue[ii];
    }
    numDetObj = lib_cfarPeakQualifiedInOrderPruning( 
                                        detObjs,
                                        cfarDetPeaks,
                                        100,
                                        numDopplerBin,
                                        511,
                                        0,
                                        1,
                                        1,
                                        MAXDETOBJS);
#else    
    /* get the warm cache benchmark */
    numDetObj = mmwavelib_cfarPeakQualifiedInOrderPruning( 
                                        grpPeakIndex,
                                        peakRangeIdx,
                                        peakDopplerIdx,
                                        peakValue,
                                        100,
                                        numDopplerBin,
                                        511,
                                        0,
                                        1,
                                        1,
                                        MAXDETOBJS);
#endif
    printf("bench mark of cfarPeakQualifiedInOrderPruning, in both directions\n ");
    printf("number of peaks, cycles\n");
    for (ii = 10;ii<110;ii+=10)
    {
        t1 = TSCL;
        
#if DEBUGPREVIOUSAPI
        numDetObj = lib_cfarPeakQualifiedInOrderPruning( 
                                        detObjs,
                                        cfarDetPeaks,
                                        ii,   /* number ofpeaks */
                                        numDopplerBin,
                                        511, /* maximum range bin index */
                                        0,   /* miminum range bin index */
                                        1,
                                        1,
                                        MAXDETOBJS);
#else
        numDetObj = mmwavelib_cfarPeakQualifiedInOrderPruning( 
                                        grpPeakIndex,
                                        peakRangeIdx,
                                        peakDopplerIdx,
                                        peakValue,
                                        ii,   /* number ofpeaks */
                                        numDopplerBin,
                                        511, /* maximum range bin index */
                                        0,   /* miminum range bin index */
                                        1,
                                        1,
                                        MAXDETOBJS);
#endif
        t2 = TSCL;
        cycles = t2-t1;
        printf("%d\t%d\n",ii,cycles);
    }
    
    
    printf("bench mark of cfarPeakQualifiedInOrderPruning, in Doppler directions\n ");
    printf("number of peaks, cycles\n");
    for (ii = 10;ii<110;ii+=10)
    {
        t1 = TSCL;
 #if DEBUGPREVIOUSAPI       
        numDetObj = lib_cfarPeakQualifiedInOrderPruning( 
                                        detObjs,
                                        cfarDetPeaks,
                                        ii,   /* number of peaks */
                                        numDopplerBin,
                                        511, /* maximum range bin index */
                                        0,   /* minimum range bin index */
                                        1,   /* Doppler direction */
                                        0,
                                        MAXDETOBJS);
#else        
        numDetObj = mmwavelib_cfarPeakQualifiedInOrderPruning( 
                                        grpPeakIndex,
                                        peakRangeIdx,
                                        peakDopplerIdx,
                                        peakValue,
                                        ii,   /* number of peaks */
                                        numDopplerBin,
                                        511, /* maximum range bin index */
                                        0,   /* minimum range bin index */
                                        1,   /* Doppler direction */
                                        0,
                                        MAXDETOBJS);
#endif
        t2 = TSCL;
        cycles = t2-t1;
        printf("%d\t%d\n",ii,cycles);
    }
    
    
    printf("bench mark of cfarPeakQualifiedInOrderPruning, in Range directions\n ");
    printf("number of peaks, cycles\n");
    for (ii = 10;ii<110;ii+=10)
    {
        t1 = TSCL;
 #if DEBUGPREVIOUSAPI
        numDetObj = lib_cfarPeakQualifiedInOrderPruning( 
                                        detObjs,
                                        cfarDetPeaks,
                                        ii,   /* number of peaks */
                                        numDopplerBin,
                                        511, /* maximum range bin index */
                                        0,   /* minimum range bin index */
                                        0,   /* Doppler direction */
                                        1,   /* Range direction */
                                        MAXDETOBJS);
#else               
        numDetObj = mmwavelib_cfarPeakQualifiedInOrderPruning( 
                                        grpPeakIndex,
                                        peakRangeIdx,
                                        peakDopplerIdx,
                                        peakValue,
                                        ii,   /* number of peaks */
                                        numDopplerBin,
                                        511, /* maximum range bin index */
                                        0,   /* minimum range bin index */
                                        0,   /* Doppler direction */
                                        1,   /* Range direction */
                                        MAXDETOBJS);
#endif
        t2 = TSCL;
        cycles = t2-t1;
        printf("%d\t%d\n",ii,cycles);
    }
}

#if (!DEBUGPREVIOUSAPI)
void test_peakpruningQualified()
{
    uint32_t numTests;
    uint32_t testIdx;
    uint32_t numRangeBin;
    uint16_t minRangeBin, maxRangeBin;
    uint16_t numDetObjRef;
   
    uint16_t ii,jj,kk;
    uint8_t foundFlag;
    int16_t tempTestPass = 1;

    fread(&numTests,sizeof(uint32_t),1,fid);    /*number of tests */

    for (testIdx=0;testIdx<numTests;testIdx++)
    {
        /* read in number of range and doppler bin */
        fread(&numRangeBin, sizeof(uint32_t),1,fid);
        fread(&numDopplerBin,sizeof(uint32_t),1,fid);

        printf("Test: numRangeBin: %d, numDopplerBin: %d\n",numRangeBin, numDopplerBin);
        
        for (ii= 0;ii < 2; ii++)
        {
            printf("test Id %d\n", ii);
            /* read in min/max range bin */
            fread(&minRangeBin,sizeof(uint16_t),1,fid);
            fread(&maxRangeBin,sizeof(uint16_t),1,fid);

            /* read in number of peaks, and corresponding range/dopper index */
            fread(&numDetPeaks,sizeof(uint32_t),1,fid);
            fread(peakRangeIdx,sizeof(uint16_t),numDetPeaks, fid);
            fread(peakDopplerIdx,sizeof(uint16_t),numDetPeaks,fid);
            fread(peakValue,sizeof(uint16_t),numDetPeaks,fid);

            numDetObj = mmwavelib_cfarPeakQualifiedPruning( 
                                                grpPeakIndex,
                                                peakRangeIdx,
                                                peakDopplerIdx,
                                                peakValue,
                                                numDetPeaks,
                                                numDopplerBin,
                                                maxRangeBin,
                                                minRangeBin,
                                                1,
                                                1,
                                                MAXDETOBJS);
            /* read in reference */
            fread(&numDetObjRef,sizeof(uint16_t),1,fid);
            fread(detObjIdxRef,sizeof(int16_t),2*numDetObjRef,fid);

            /* check results */
            if (numDetObj!=numDetObjRef)
            {
               	testPASS = 0;
                tempTestPass = 0;
               	printf("Peakpruning test(both direction) fails at number of detected objects ref=%d, det=%d\n",numDetObjRef,numDetObj);
            }

            for (jj=0;jj<numDetObj;jj++)
            {
                grpPeakRangeIdx = peakRangeIdx[grpPeakIndex[jj]];
                grpPeakDopplerIdx = peakDopplerIdx[grpPeakIndex[jj]];
               foundFlag = 0;
                for (kk=0;kk<numDetObj;kk++)
                {
                    if ((grpPeakRangeIdx==detObjIdxRef[2*kk]) && (grpPeakDopplerIdx==detObjIdxRef[2*kk+1]))
                    {
                        foundFlag = 1;
                        break;
                    }
                }
               if (!foundFlag) 
               {
                    testPASS = 0;
                    tempTestPass = 0;
                    printf("Peakpruning test(both direction) fails at objects %d\n",jj);
                }
               
            }
            
            /*call the lib,  range direction */
            numDetObj = mmwavelib_cfarPeakQualifiedPruning(
                                        grpPeakIndex,
                                        peakRangeIdx,
                                        peakDopplerIdx,
                                        peakValue,
                                        numDetPeaks,
                                        numDopplerBin,
                                        maxRangeBin,
                                        minRangeBin,
                                        0,
                                        1,
                                        MAXDETOBJS);
            /* read in reference */
            fread(&numDetObjRef,sizeof(uint16_t),1,fid);
            fread(detObjIdxRef,sizeof(int16_t),2*numDetObjRef,fid);

            /* check results */
            if (numDetObj!=numDetObjRef)
            {
               	testPASS = 0;
                tempTestPass = 0;
               	printf("Peakpruning test(range direction) fails at number of detected objects ref=%d, det=%d\n",numDetObjRef,numDetObj);
            }

            for (jj=0;jj<numDetObj;jj++)
            {

                grpPeakRangeIdx = peakRangeIdx[grpPeakIndex[jj]];
                grpPeakDopplerIdx = peakDopplerIdx[grpPeakIndex[jj]];
                         
                foundFlag = 0;
                for (kk=0;kk<numDetObj;kk++)
                {
                    if ((grpPeakRangeIdx==detObjIdxRef[2*kk]) && (grpPeakDopplerIdx==detObjIdxRef[2*kk+1]))
                    {
                        foundFlag = 1;
                        break;
                    }
                }
               if (!foundFlag) 
               {
                    testPASS = 0;
                    tempTestPass = 0;
                    printf("Peakpruning test(both direction) fails at objects %d\n",jj);
                }
            }

              

            /*call the lib,  doppler direction */
            numDetObj = mmwavelib_cfarPeakQualifiedPruning(
                                                grpPeakIndex,
                                                peakRangeIdx,
                                                peakDopplerIdx,
                                                peakValue,
                                                numDetPeaks,
                                               numDopplerBin,
                                               maxRangeBin,
                                               minRangeBin,
                                               1,
                                               0,
                                               MAXDETOBJS);
            /* read in reference */
            fread(&numDetObjRef,sizeof(uint16_t),1,fid);
            fread(detObjIdxRef,sizeof(int16_t),2*numDetObjRef,fid);

            /* check results */
            if (numDetObj!=numDetObjRef)
            {
                testPASS = 0;
                tempTestPass = 0;
                printf("Peakpruning test(doppler direction) fails at number of detected objectsref=%d, det=%d\n",numDetObjRef,numDetObj);
            }

            for (jj=0;jj<numDetObj;jj++)
            {
                grpPeakRangeIdx = peakRangeIdx[grpPeakIndex[jj]];
                grpPeakDopplerIdx = peakDopplerIdx[grpPeakIndex[jj]];

                   foundFlag = 0;
                for (kk=0;kk<numDetObj;kk++)
                {
                    if ((grpPeakRangeIdx==detObjIdxRef[2*kk]) && (grpPeakDopplerIdx==detObjIdxRef[2*kk+1]))
                    {
                        foundFlag = 1;
                        break;
                    }
                }
               if (!foundFlag) 
               {
                    testPASS = 0;
                    tempTestPass = 0;
                    printf("Peakpruning test(both direction) fails at objects %d\n",jj);
                }
            
            }
			
	}
    }
    
    if (tempTestPass)
    {
        MCPI_setFeatureTestResult("cfar Qualified Peaks Pruning Test", MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult("cfar Qualified Peaks Pruning Test", MCPI_TestResult_FAIL);
    }
}
void benchmark_peakpruningQualified()
{
    uint32_t ii;
    uint32_t t1,t2;
    uint32_t  cycles;


    /* read in data */
    fread(&numDetPeaks,sizeof(uint32_t),1,fid);
    fread(peakRangeIdx,sizeof(uint16_t),numDetPeaks, fid);
    fread(peakDopplerIdx,sizeof(uint16_t),numDetPeaks,fid);
    fread(peakValue,sizeof(uint16_t),numDetPeaks,fid);
    
                 
    numDopplerBin  = 64;
 
   
    /* get the warm cache benchmark */
    numDetObj = mmwavelib_cfarPeakQualifiedPruning( 
                                                grpPeakIndex,
                                                peakRangeIdx,
                                                peakDopplerIdx,
                                                peakValue,
                                                100,
                                                numDopplerBin,
                                                511,
                                                0,
                                                1,
                                                1,
                                                MAXDETOBJS);
    printf("bench mark of cfarPeakQualifiedPruning in both directions \n");
    printf("number of peaks, cycles\n");
    for (ii = 10;ii<110;ii+=10)
    {
        t1 = TSCL;
        numDetObj = mmwavelib_cfarPeakQualifiedPruning( 
                                                grpPeakIndex,
                                                peakRangeIdx,
                                                peakDopplerIdx,
                                                peakValue,
                                                ii,   /* number ofpeaks */
                                                numDopplerBin,
                                                511, /* maximum range bin index */
                                                0,   /* miminum range bin index */
                                                1,
                                                1,
                                                MAXDETOBJS);
        t2 = TSCL;
        cycles = t2-t1;
        printf("%d\t%d\n",ii,cycles);
    }
    
    printf("bench mark of cfarPeakQualifiedPruning in Doppler directions \n");
    printf("number of peaks, cycles\n");
    for (ii = 10;ii<110;ii+=10)
    {
        t1 = TSCL;
        numDetObj = mmwavelib_cfarPeakQualifiedPruning( 
                                                grpPeakIndex,
                                                peakRangeIdx,
                                                peakDopplerIdx,
                                                peakValue,
                                                ii,   /* number ofpeaks */
                                                numDopplerBin,
                                                511, /* maximum range bin index */
                                                0,   /* miminum range bin index */
                                                1,   /* Doppler direction */
                                                0,
                                                MAXDETOBJS);
        t2 = TSCL;
        cycles = t2-t1;
        printf("%d\t%d\n",ii,cycles);
    }
    
    printf("bench mark of cfarPeakQualifiedPruning in Range directions \n");
    printf("number of peaks, cycles\n");
    for (ii = 10;ii<110;ii+=10)
    {
        t1 = TSCL;
        numDetObj = mmwavelib_cfarPeakQualifiedPruning( 
                                                grpPeakIndex,
                                                peakRangeIdx,
                                                peakDopplerIdx,
                                                peakValue,
                                                ii,   /* number ofpeaks */
                                                numDopplerBin,
                                                511, /* maximum range bin index */
                                                0,   /* miminum range bin index */
                                                0,   /* Doppler direction */
                                                1,   /* range direction */
                                                MAXDETOBJS);
        t2 = TSCL;
        cycles = t2-t1;
        printf("%d\t%d\n",ii,cycles);
    }
}
#endif

void main(void)
{
    MCPI_Initialize();
    TSCL = 0;

    printf("..... Peak group/pruning test  .......\n");

    fid = fopen("..\\testdata\\peakpruning_test.bin","rb");
    if (fid==NULL)
    {
      	printf("File Open Error: peakpruning_test.bin can not open\n");
       	exit(3);
    }


    printf("peak group/pruning test ......\n");
    test_peakpruning();


    /* bench mark */
    printf("peak group/pruning benchmark ......\n");
    benchmark_peakpruning();

    printf("peak group/pruning with qualified in-order peaks ......\n");
    test_peakpruningQualifiedinorder();

    /* bench mark*/
    benchmark_peakpruningQualifiedinorder();

#if (!DEBUGPREVIOUSAPI)
    printf("peak group/pruning with qualified peaks ......\n");
    test_peakpruningQualified();

    /* bench mark*/
    benchmark_peakpruningQualified();
    
#endif
    
    fclose(fid);

    if (testPASS)
    {
        printf("========== Peak group/pruning tests all passed! ========== \n");
    }
    else
    {
        printf("========== Peak group/pruning tests Failed! ========== \n");
    }
    printf("========== Peak group/pruning Testing completed! ==========\n");

    MCPI_setTestResult ();
}
