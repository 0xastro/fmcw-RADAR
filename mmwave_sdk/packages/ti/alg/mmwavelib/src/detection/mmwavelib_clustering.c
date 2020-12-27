 /*
  * (C) Copyright 2016, Texas Instruments Incorporated -  http://www.ti.com/
  *-------------------------------------------------------------------------
  *
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
 /**
  *   @file     mmwavelib_clustering.c
  *
  *   @brief    This file contains routines related to clustering using
  *             DBSCAN algorithm.
*/
#include <c6x.h>
#include <ti/alg/mmwavelib/include/mmwavelib_cplx_types.h>
#include <ti/alg/mmwavelib/include/mmwavelib_c674_emulate_c66_intrinsics.h>
#include <stdint.h>
#include "mmwavelib_clustering.h"


/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_dbscan
 *
 * \par
 * <b>Description</b>  :    implements DBSCAN clustering method with fixed point input data
 *
 *
 * @param        [in]       locationXY          : detected object location information
 * @param        [in]       speedArray          : detected object speed array
 * @param        [in]       epsilon             : distance and speed threhold for clustering
 * @param        [in]       weight              : the weight for the speed
 * @param        [in]       numPoints           : the number of detected objects for clustering
 * @param        [in]       minPointsInCluster  : minimum number of neighbor points for the tested point to be in a cluster
 * @param        [in]       scratchBuf          : scratch memory buffer with length of 4*((numPoints/3+1)*3), must
 *                                                be 8 bytes aligned. it saves the visited buffer, and neighbors index for every cluster
 * @param        [out]      numClusters         : the detected number of clusters
 * @param        [out]      clusterIdxArray     : the cluster ID for each detected objects, clusterID starts at 1.
 * @param        [out]      clusterInfor        : the cluster information, inlcuding center, and min and max value, and variance for each cluster
 *                                          
 * 
 *
 * @return                  none.
 *
 * @pre                     (1) scratchBuf is 8 byte aligned.
 *                          (2) input speedArray and locationXY are 8 byte aligned.
 *
 * @ingroup                 MMWAVELIB_CLUSTERING
 * 
 * cycles                   cgt:8.1.3
 *                          1 cluster: 2.4320 * numPoints^2 + 57487
 *                          2 clusters: 2.4317 * numPoints^2 + 56776
 *                          4 clusters: 2.4315 * numPoints^2 + 55140
 *                          5 clusters: 2.4191 * numPoints^2 + 62838
 *                          8 clusters: 2.398 * numPoints^2 + 75288
 *
 *                          2.4258 * numPoints^2 + 59108
 ********************************************************************************************************************
 */
void mmwavelib_dbscan(cplx16_t locationXY[],
                         int16_t speedArray[],
                         int32_t epsilon,
                         float32_t weight,
                         uint16_t numPoints,
                         uint16_t minPointsInCluster,
                         int8_t scratchBuf[],
                         uint16_t *numClusters,
                         uint16_t clusterIdxArray[],
                         mmwavelib_clusterInfor clusterInfor[]
                        )
{
    uint16_t idx,idx1;
    uint16_t pointIdx;
    uint16_t newNeighborCount,neighborCount;
    uint16_t clusterId;
    int8_t  *visited;
    uint16_t *neighbor;
    int8_t *addIn;
    uint16_t numPointsMod8;


    _nassert(((int32_t)scratchBuf % 8U) == 0); /* 8 byte aligned */
    numPointsMod8 = (uint16_t)(numPoints >> 3U);
    if (numPoints & 0x7U)
    {
        numPointsMod8++;
    }
    /* global visited array */
    visited     = (int8_t *) &scratchBuf[0];
    /* local buffer to track if neighbor is already added */
    addIn       = (int8_t *) &scratchBuf[(uint16_t) (numPointsMod8 << 3U)];
    /* save the neighbor for every cluster */
    neighbor   = (uint16_t *)&scratchBuf[2U* ((uint16_t)(numPointsMod8 << 3U))];


    /* initialize the visited to 0- not visited */
    idx1= (uint16_t)((uint16_t)(numPoints >> 3U) << 3U);
    for (idx = 0; idx < idx1 ; idx += 8U)
    {
        _amem8(&visited[idx]) = 0;
        /* one elements can only be added in one cluster, since once it is added,
        it will be marked as visited, it will not be checked as another
        cluster's neighbor */

        _amem8(&addIn[idx]) = 0;
    }

    for (idx = idx1; idx < numPoints; idx++)
    {
        visited[idx] = 0;
        addIn[idx] = 0;
    }

    clusterId = 0;
    /* go through all points */
    for (pointIdx = 0; pointIdx < numPoints; pointIdx++)
    {
        if (visited[pointIdx] != POINT_VISITED) /* not visited before */
        {
            /* mark it as visited */
            visited[pointIdx] = POINT_VISITED;

            /* find the neighbour */
            newNeighborCount = mmwavelib_dbscan_findNeighbors2(
                                                               locationXY,
                                                               speedArray,
                                                               pointIdx,
                                                               epsilon,
                                                               weight,
                                                               numPoints,
                                                               visited,
                                                               neighbor);


            /* check if it is a noise or a cluster */
            if (newNeighborCount < minPointsInCluster)
            {
                /* mark it as noise */
                clusterIdxArray[pointIdx] = 0;
            }
            else
            {
                /* it is a new cluster */
                clusterId++;
                /* set the point to clusterId */
                clusterIdxArray[pointIdx] = clusterId;

                /* initialize the addIn buffer to track
                for (idx = 0; idx < idx1 ; idx += 8)
                {
                    _amem8(&addIn[idx]) = 0;
                }
                for (idx=idx1;idx<numPoints; idx++)
                {
                    addIn[idx] = 0;
                }

                */
                for (idx = 0; idx < newNeighborCount; idx++)
                {
                    addIn[neighbor[idx]] = 1;
                }

                /* expand the cluster */
                neighborCount = mmwavelib_dbscan_expandCluster(locationXY,
                                                  speedArray,
                                                  newNeighborCount,
                                                  epsilon,
                                                  weight,
                                                  minPointsInCluster,
                                                  numPoints,
                                                  visited,
                                                  addIn,
                                                  neighbor,
                                                  clusterId,
                                                  clusterIdxArray);

                /* add the original point into the cluster */
                neighbor[neighborCount] = pointIdx;
                neighborCount++;

                /* find out the cluster information */
                mmwavelib_dbscan_calCluster (locationXY,
                                             speedArray,
                                             neighborCount,
                                             neighbor,
                                             &clusterInfor[clusterId-1U]);


            }/* end of else new cluster */

        }/* end of if visited[point]!=POINT_VISITED */


    }/* end of for loop */


    *numClusters = clusterId; /* number of clusters */

}

/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_dbscan_findNeighbors2
 *
 * \par
 * <b>Description</b>  :    implements neighbour found algorithm for one specfic point
 *                          based on two dimension locations and speeds of the detected objects,
 *
 *
 * @param        [in]       locationXY      : two dimension locations of the detected objects
 * @param        [in]       speedArray      : speed array of the detected objects
 * @param        [in]       pointIdx        : the index of the object for finding the neighbours
 * @param        [in]       epsilon2        : the distance and speed threshold for clustering
 * @param        [in]       weight          : the weight for the speed
 * @param        [in]       numPoints       : the number of detected objects
 * @param        [in]       visited         : the global visited array with length of numPoints
 * @param        [out]      neighbor        : the found neighbour index.
 *
 *
 *
 * @return                  Number of found neighbours.
 *
 * @pre
 *
 * @ingroup                 MMWAVELIB_CLUSTERING
 *
 * cycles                   cgt(8.1.3)
 *                          1.9976*numPoints + 125
 *
 *******************************************************************************************************************
 */


uint16_t mmwavelib_dbscan_findNeighbors2(cplx16_t locationXY[],
                                         int16_t speedArray[],
                                         uint16_t pointIdx,
                                         int32_t epsilon2,
                                         float32_t weight,
                                         uint16_t numPoints,
                                         int8_t visited[],
                                         uint16_t * neighbor)
{
    int32_t i, idx1, itemp;
    int16_t speed;
    int32_t speedDif;
    int32_t  inputPoint, testInput;
    uint32_t inputSpeed;
    int32_t weightFixed, weightConst;
    int64_t  refPoints, testInput1, testInput2,lltemp1, lltemp2,lldist1, lldist2;
    int64_t  lltemp3,lltemp4;
    int64_t speedQuard,speedRef;
    int64_t epsilon2Pair;
    uint16_t count;
    int64_t  tempResHi, tempResLo;
    int64_t * restrict testPoints;
    int64_t * restrict speedPtr;
    int32_t * restrict visitedPointer;
    uint32_t sign;
    int32_t visitedCheck, visitedUnknown,visited2;
    int32_t disDif;

    /* tested point */
    inputPoint  =   _amem4(&locationXY[pointIdx]);
    refPoints   =   _itoll((uint32_t)inputPoint, (uint32_t)inputPoint); /* 2 elemenets */

    speed       =   speedArray[pointIdx];
    inputSpeed  =   _pack2((uint32_t)speed,(uint32_t)speed);
    speedRef    =  _itoll(inputSpeed, inputSpeed); /* 4 speed */

    count = 0;



    testPoints = (int64_t *) locationXY;
    speedPtr   = (int64_t *) speedArray;
    visitedPointer = (int32_t *) visited;

    weightConst  = (int32_t) ((uint32_t)1U<<((uint32_t)(31U-DBSCAN_FIXEDWEIGHTSHIFT)));
    weightFixed = _spint(weight * (float32_t) weightConst);
    epsilon2Pair = _itoll((uint32_t)epsilon2,(uint32_t)epsilon2);

    visitedUnknown = 0;
    idx1 = (int32_t) numPoints/4;
    /* go through all the points */
    for (i = 0; i < (int32_t)idx1; i++)
    {
        speedQuard  =   _mem8(speedPtr);  /* read in 4 speeds */
        speedPtr ++;
        lltemp3     =   _dssub2(speedQuard, speedRef);
        tempResHi   =   _mpy2ll((int32_t)_hill(lltemp3), (int32_t)_hill(lltemp3));
        tempResLo   =   _mpy2ll((int32_t)_loll(lltemp3), (int32_t)_loll(lltemp3));

        lltemp3     =   _dshl(_itoll((uint32_t)_mpyhir(weightFixed, (int32_t)_hill(tempResHi)), (uint32_t)_mpyhir(weightFixed, (int32_t)_loll(tempResHi))), DBSCAN_FIXEDWEIGHTSHIFT);
        lltemp4     =   _dshl(_itoll((uint32_t)_mpyhir(weightFixed, (int32_t)_hill(tempResLo)), (uint32_t)_mpyhir(weightFixed, (int32_t)_loll(tempResLo))), DBSCAN_FIXEDWEIGHTSHIFT);

        /*lltemp4   =   _dshl(_itoll(_mpyhir(weightFixed, _hill(tempResLo)), (_mpyhill(weightFixed, _loll(tempResLo))>>15)), DBSCAN_FIXEDWEIGHTSHIFT);*/

        testInput1  =   _mem8(testPoints); /* read in 2 elements */
        testPoints++;
        testInput2  =   _mem8(testPoints); /* read in 2 elements */
        testPoints++;
        lltemp1     =   _dssub2(refPoints,testInput1);
        lltemp2     =   _dssub2(refPoints,testInput2);
        lldist1     =   _itoll((uint32_t)_dotp2((int32_t)_hill(lltemp1), (int32_t)_hill(lltemp1)), (uint32_t)_dotp2((int32_t)_loll(lltemp1), (int32_t)_loll(lltemp1)));
        lldist2     =   _itoll((uint32_t)_dotp2((int32_t)_hill(lltemp2),(int32_t) _hill(lltemp2)), (uint32_t)_dotp2((int32_t)_loll(lltemp2), (int32_t)_loll(lltemp2)));

        lldist1     =   _dsadd(lldist1, lltemp4);
        lldist2     =   _dsadd(lldist2, lltemp3);

        lldist1     =   _dssub(lldist1, epsilon2Pair);
        lldist2     =   _dssub(lldist2, epsilon2Pair);
        sign        =    _deal(_dcmpgt2(0, lldist2));
        sign        =   (_rotl(sign, 2U) | _deal(_dcmpgt2(0, lldist1))) >> 16U;

        visited2        =   _mem4(visitedPointer);
        visitedPointer ++;
        visitedCheck    =   _cmpeq4(visited2, visitedUnknown);
        visitedCheck    =  (uint32_t) visitedCheck &sign;
        count           +=  _bitc4((uint32_t)visitedCheck);

        itemp = i * 4;
        if((uint32_t)visitedCheck & 1U)
        {
            *neighbor = (uint16_t)itemp;
            neighbor++;
        }
        if((uint32_t)visitedCheck & 2U)
        {
            *neighbor = (uint16_t)itemp+1U;
            neighbor++;
        }
        if((uint32_t)visitedCheck & 4U)
        {
            *neighbor = (uint16_t)itemp+2U;
            neighbor++;
        }
        if((uint32_t)visitedCheck & 8U)
        {
            *neighbor = (uint16_t)itemp+3U;
            neighbor++;
        }
    }

    idx1 = idx1 * 4;
    for(i = idx1; i < (int32_t) numPoints; i++)
    {
        speedDif    =   speedArray[i] - speed;
        speedDif    =   speedDif * speedDif;
        testInput   =   _amem4(&locationXY[i]);
        disDif      =   _ssub2(testInput,inputPoint);
        disDif      =   _dotp2(disDif, disDif);
        disDif      +=  _spint(weight*(float32_t)speedDif);

         if ((visited[i] != POINT_VISITED) && (disDif < epsilon2))
         {
               *neighbor = i;
               neighbor++;
               count++;
           }
      }

    return count;
}


/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_dbscan_expandCluster
 *
 * \par
 * <b>Description</b>  :    expands the cluster based on the alreay found neighbors.
 *
 *
 * @param        [in]       locationXY           : two dimension locations of the detected objects
 * @param        [in]       speedArray           : speed array of the detected objects
 * @param        [in]       currentNeighborCount : the number of already found neighbors
 * @param        [in]       epsilon              : the distancd and speed threshold for clustering
 * @param        [in]       weight               : the weight for the speed
 * @param        [in]       minPointsInCluster   : minimum number of neighbor points for the tested point to be in a cluster
 * @param        [in]       numPoints            : the total number of objects
 * @param        [inout]    visited              : the global visited buffer.
 * @param        [inout]    addIn                : the buffer to check if an object is already in another's objects neighbor.
 * @param        [inout]    neighbor             : the buffer to save the objects in the neighbor
 * @param        [in]       clusterId            : the current cluster ID
 * @param        [out]      clusterIdxArray      : the updated cluster ID array
 *
 * @return                  Number of total found neighbours.
 *
 * @pre
 *
 * @ingroup                 MMWAVELIB_CLUSTERING
 *
 * cycles
 *
 *******************************************************************************************************************
 */

uint16_t mmwavelib_dbscan_expandCluster(cplx16_t locationXY[],
                                       int16_t speedArray[],
                                       uint16_t currentNeighborCount,
                                       int32_t epsilon,
                                       float32_t weight,
                                       uint16_t minPointsInCluster,
                                       uint16_t numPoints,
                                       int8_t visited[],
                                       int8_t addIn[],
                                       uint16_t * neighbor,
                                       uint16_t clusterId,
                                       uint16_t clusterIdxArray[]
                                       )
{
    uint16_t idx,idx1;
    uint16_t neighborCount,newNeighborCount, newAddNeighborCount;

    idx=0;
    neighborCount = currentNeighborCount;
    /* expand the new cluster for every neighbour */
    while (idx < neighborCount)
    {
        /*if (!clusterIdxArray[neighbor[idx]])  *//* if it is not any cluster */
        clusterIdxArray[neighbor[idx]] = clusterId; /*assign clusterId to the neighbor */

        if (visited[neighbor[idx]] != POINT_VISITED)
        {
            visited[neighbor[idx]] = POINT_VISITED;
            /* find its new neighbour */
            mmwavelib_dbscan_expandFindNeighbors2(locationXY,
                                                  speedArray,
                                                  neighbor[idx],
                                                  epsilon,
                                                  weight,
                                                  numPoints,
                                                  visited,
                                                  addIn,
                                                  &newNeighborCount,
                                                  &newAddNeighborCount,
                                                  &neighbor[neighborCount]);


            if (newNeighborCount >= minPointsInCluster) /*new neighbor */
            {
                for (idx1 = 0; idx1 < newAddNeighborCount; idx1++)
                {
                    addIn[neighbor[neighborCount+idx1]] = 1;  /* update the new added neighbors */
                }
                neighborCount += newAddNeighborCount; /* only add in the new neighbor, skip the already add in neighbor */

            }
        }

        idx++;


    }/* end of while */

    return neighborCount;

}

/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_dbscan_expandFindNeighbors2
 *
 * \par
 * <b>Description</b>  :    implements neighbour expand algorithm for one specfic point based on two
 *                          dimension locations and speeds of the detected objects, add the found new
 *                          neighbors into the neighbor buffer
 *
 *
 * @param        [in]       locationXY      : two dimension locations of the detected objects
 * @param        [in]       speedArray      : speed array of the detected objects
 * @param        [in]       pointIdx        : the index of the object for finding the neighbours
 * @param        [in]       epsilon         : the distancd and speed threshold for clustering
 * @param        [in]       weight          : the weight for the speed
 * @param        [in]       numPoints       : the total number of detected objects
 * @param        [in]       visited         : the global visited array
 * @param        [in]       addIn           : the local addIn array to check if the object is alreay added into neighbor
 * @param        [out]      numNeighbor     : the total number of found neighbors.
 * @param        [out]      numNewNeighbor  : the number of new foud  neighbor index.
 * @param        [out]      neighbour       : the found neighbor index.
 *
 *
 *
 * @return                  void
 *
 * @pre
 *
 * @ingroup                 MMWAVELIB_CLUSTERING
 *
 * cycles
 *
 *******************************************************************************************************************
 */

void mmwavelib_dbscan_expandFindNeighbors2(cplx16_t locationXY[],
                                            int16_t speedArray[],
                                            uint16_t pointIdx,
                                            int32_t epsilon,
                                            float32_t weight,
                                            uint16_t numPoints,
                                            int8_t visited[],
                                            int8_t addIn[],
                                            uint16_t *numNeighbor,
                                            uint16_t *numNewNeighbor,
                                            uint16_t * neighbour)
{

    int32_t i, idx1,itemp;

    int32_t inputPoint;
    int16_t speed;
    int64_t * restrict testPoints;
    int64_t * restrict speedPtr;
    int32_t * restrict visitedPointer;
    int32_t * restrict addInPointer;
    int32_t speedRef;
    int64_t speedQuard, speedQuardRef;
    int32_t weightFixed, weightConst;
    int64_t  refPoints, testInput1, testInput2,lltemp1, lltemp2,lldist1, lldist2;
    int64_t  lltemp3,lltemp4;
    int64_t epsilonPair;
    uint16_t nCount, newNCount;
    uint32_t sign;
    int32_t visitedCheck, visitedUnknown;
    int32_t addInCheck;
    int64_t  tempResHi, tempResLo;
    int32_t speedDif,testInput,disDif;



    /* tested point */
    inputPoint  =   _amem4(&locationXY[pointIdx]);
    refPoints   =   _itoll((uint32_t)inputPoint, (uint32_t)inputPoint); /* 2 elemenets */

    speed       =   speedArray[pointIdx];
    speedRef    =   (int32_t) _pack2((uint16_t)speed,(uint16_t)speed);
    speedQuardRef  =  _itoll((uint32_t)speedRef, (uint32_t)speedRef);

    weightConst = (int32_t) ((uint32_t)1U<<(uint32_t)(31U-DBSCAN_FIXEDWEIGHTSHIFT));
    weightFixed = _spint(weight * (float32_t)weightConst);

    epsilonPair = _itoll((uint32_t)epsilon,(uint32_t)epsilon);


    testPoints = (int64_t *) locationXY;
    speedPtr   = (int64_t *) speedArray;
    visitedPointer = (int32_t *) visited;
    addInPointer = (int32_t *) addIn;


    nCount  = 0;
    newNCount = 0;
    idx1 = (int32_t)numPoints/4;

    visitedUnknown = 0;
    /* go through all the points */
    for (i=0;i<(int32_t) idx1; i++)
    {
        speedQuard  =   _mem8(speedPtr);  /* read in 4 speeds */
        speedPtr ++;
        lltemp3     =   _dssub2(speedQuard, speedQuardRef);
        tempResHi       =   _mpy2ll((int32_t)_hill(lltemp3), (int32_t)_hill(lltemp3));
        tempResLo       =   _mpy2ll((int32_t)_loll(lltemp3), (int32_t)_loll(lltemp3));
        lltemp3         =   _dshl(_itoll((uint32_t)_mpyhir(weightFixed, (int32_t)_hill(tempResHi)), (uint32_t)_mpyhir(weightFixed, (int32_t)_loll(tempResHi))), DBSCAN_FIXEDWEIGHTSHIFT);
        lltemp4         =   _dshl(_itoll((uint32_t)_mpyhir(weightFixed, (int32_t)_hill(tempResLo)), (uint32_t)_mpyhir(weightFixed, (int32_t)_loll(tempResLo))), DBSCAN_FIXEDWEIGHTSHIFT);


        testInput1  =   _mem8(testPoints); /* read in 2 elements */
        testPoints++;
        testInput2  =   _mem8(testPoints); /* read in 2 elements */
        testPoints++;
        lltemp1     =   _dssub2(refPoints,testInput1);
        lltemp2     =   _dssub2(refPoints,testInput2);
        lldist1     =   _itoll((uint32_t)_dotp2((int32_t)_hill(lltemp1), (int32_t)_hill(lltemp1)), (uint32_t)_dotp2((int32_t)_loll(lltemp1), (int32_t)_loll(lltemp1)));
        lldist2     =   _itoll((uint32_t)_dotp2((int32_t)_hill(lltemp2), (int32_t)_hill(lltemp2)), (uint32_t)_dotp2((int32_t)_loll(lltemp2), (int32_t)_loll(lltemp2)));
        lldist1         =   _dsadd(lldist1, lltemp4);
        lldist2         =   _dsadd(lldist2, lltemp3);

        lldist1         =   _dssub(lldist1, epsilonPair);
        lldist2         =   _dssub(lldist2, epsilonPair);
        sign            =   _deal(_dcmpgt2(0, lldist2));
        sign            =   (_rotl(sign, 2U) | _deal(_dcmpgt2(0, lldist1))) >> 16U;

        visitedCheck    =   _cmpeq4(_mem4(visitedPointer), visitedUnknown);
        visitedPointer ++;

        visitedCheck    = (uint32_t)visitedCheck & sign;
        nCount          +=  _bitc4((uint32_t)visitedCheck);

        addInCheck      =  _cmpeq4(_mem4(addInPointer), visitedUnknown);
        visitedCheck    = (uint32_t)visitedCheck & (uint32_t)addInCheck;
        addInPointer++;

        newNCount       +=  _bitc4((uint32_t)visitedCheck);

        itemp           =   i * 4U;
        if((uint32_t)visitedCheck & 1U)
        {
            *neighbour = itemp;
            neighbour++;
        }
        if((uint32_t)visitedCheck & 2U) 
        {
            *neighbour = itemp + 1U;
            neighbour++;
        }
        if((uint32_t)visitedCheck & 4U) 
        {
            *neighbour = itemp + 2U;
            neighbour++;
        }
        if((uint32_t)visitedCheck & 8U) 
        {
            *neighbour = itemp + 3U;
            neighbour++;
        }
    }

    idx1 = idx1 * 4;
    for (i = idx1; i < numPoints; i++)
    {
        speedDif    =   speedArray[i] - speed;
        speedDif    =   speedDif * speedDif;
        testInput   =   _amem4(&locationXY[i]);
        disDif      =   _ssub2(testInput,inputPoint);
        disDif      =   _dotp2(disDif, disDif);
        disDif      +=  _spint(weight*(float32_t)speedDif);

        if ((visited[i] != POINT_VISITED) && (disDif < epsilon))
        {

              nCount++;
              if (addIn[i] != POINT_VISITED)
              {
                  *neighbour = i;
                  neighbour++;
                  newNCount++;
              }
        }
    }
    *numNeighbor = nCount;
    *numNewNeighbor = newNCount;

}



/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_dbscan_calCluster
 *
 * \par
 * <b>Description</b>  :    calculates the cluster center, size and variance for location and speed using floating point .
 *
 *
 * @param        [in]       locationXY      : two dimension locations of the detected objects
 * @param        [in]       speedArray      : speed array of the detected objects
 * @param        [in]       neighborCount   : number of points in the cluster
 * @param        [in]       neighborList    : the point index for the detected objects in the culster
 * @param        [out]      clusterInf      : pointer to the cluster information structure.
 *
 *
 *
 * @return                  none
 *
 * @pre
 *
 * @ingroup                 MMWAVELIB_CLUSTERING
 *
 * cycles                   cgt 8.1.3
 *                          10.1174*neighborCount+218
 *
 *******************************************************************************************************************
 */
void mmwavelib_dbscan_calCluster (cplx16_t locationXY[],
                                  int16_t speedArray[],
                                  uint16_t neighborCount,
                                  uint16_t neighborList[],
                                  mmwavelib_clusterInfor * clusterInf)
{
    int32_t sumX,sumY,sumV,aveX, aveY,aveV;

    uint16_t ii,idx1;
    int32_t input1, input2;
    int32_t neighIdx;
    uint16_t index1,index2;
    int32_t packWordX, packWordY, packWordV, diffX, diffY, sizeX, sizeY;
    int32_t one = 0x10001U;
    float32_t scale, meanX, meanY,meanV,results3, diffV;
    __float2_t  meanXY,inputXY,diffXY, results1,results2,twoScale,prackDiffV,packMeanV;


    clusterInf->numPointsInCluster = neighborCount;

    sumX  =  0;
    sumY  =  0;
    sumV  =  0;
    idx1 = neighborCount/2U * 2U;
    for (ii = 0; ii < idx1; ii += 2U)
    {
    
        index1 = neighborList[ii];
        index2 = neighborList[ii + 1U];
        input1  =  _mem4(&locationXY[index1]);
        input2  =  _mem4(&locationXY[index2]);

        packWordY  =  (int32_t)_pack2((uint32_t)input1,(uint32_t)input2);
        packWordX  =  (int32_t) _packh2((uint32_t)input1,(uint32_t)input2);

        sumX       += _dotp2(packWordX, one);
        sumY       += _dotp2(packWordY, one);

        packWordV  =  (int32_t)_pack2((uint16_t)speedArray[index1],(uint16_t)speedArray[index2]);

        sumV      += _dotp2(packWordV, one);

    }

    if (neighborCount & 0x1U)
    {
        index1  =  neighborList[neighborCount - 1U];
        input1  =  _mem4(&locationXY[index1]);

        sumX    +=  _dotp2(input1,0x10000);
        sumY    +=  _dotp2(input1, 1);
        sumV    +=  speedArray[index1];
    }

    /* get average */
    scale       =   _rcpsp((float32_t)neighborCount);
    scale       =   scale * (2.f - (float32_t)neighborCount * scale);
    scale       =   scale * (2.f - (float32_t)neighborCount * scale);
    meanX       =   scale * (float32_t)sumX;
    meanY       =   scale * (float32_t)sumY;
    meanV       =   scale * (float32_t)sumV;
    aveX        =   _spint(meanX);
    clusterInf->xCenter = (int16_t) aveX;
    aveY        =   _spint(meanY);
    clusterInf->yCenter = (int16_t) aveY;
    aveV        =   _spint(meanV);
    clusterInf->aveVelocity = (int16_t) aveV;

    twoScale  =  _ftof2(scale, scale);
    meanXY     =  _ftof2(meanX, meanY);
    packMeanV  = _ftof2(aveV, aveV);
    sizeX      =  0;
    sizeY      = 0;
    results1   = 0;
    /* variance and size */
    for (ii = 0; ii < neighborCount; ii++)
    {
        neighIdx  = neighborList[ii]; /* read in 2 neighbor */
        input1    =  _mem4(&locationXY[neighIdx]);

        inputXY  =  _dinthsp(input1);
        diffXY   =  _dsubsp (inputXY, meanXY);  /* difference */
        results1 =  _daddsp (results1, _dmpysp(diffXY,diffXY)); /* accumulation diff square */

        diffX      =   _spint(_fabsf(_hif2(diffXY)));
        diffY      =   _spint(_fabsf(_lof2(diffXY)));
        if ( diffX > sizeX)
        {
            sizeX = diffX;
        }

        if (diffY > sizeY)
        {
            sizeY = diffY;
        }

    }
    results2       =    _dmpysp(results1,twoScale);
    clusterInf->rangeVar       = _hif2(results2) + _lof2(results2);
    clusterInf->xSize    = (int16_t) sizeX;
    clusterInf->ySize    = (int16_t) sizeY;

    /* speed variance */
    
    results1 = 0;
    for (ii = 0; ii < idx1; ii += 2)
    {
        
        index1  = neighborList[ii];
        index2  = neighborList[ii+1U];
        packWordV  = _pack2(speedArray[index1],speedArray[index2]);

        prackDiffV =   _dsubsp (_dinthsp(packWordV), packMeanV);  /* difference */
        results1   =   _daddsp (results1, _dmpysp(prackDiffV,prackDiffV)); /* accumulation diff square */

    }

    results3  =  _hif2(results1) + _lof2(results1);

    if (neighborCount & 0x1U)
    {
        index1  =  neighborList[neighborCount - 1U];
        diffV   =  (float32_t)speedArray[index1] - meanV;
        results3 += diffV * diffV;

    }
    clusterInf->velocityVar =  results3 * scale;



}


/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_dbscan_skipFoundNeiB
 *
 * \par
 * <b>Description</b>  :    implements DBSCAN clustering method in floating point version by skipping the already
 *                          found neighbors when expanding the cluster
 *
 *
 * @param        [in]       locationXY          : detected object location information
 * @param        [in]       speedArray          : detected object speed array
 * @param        [in]       epsilon             : distance and speed threhold for clustering
 * @param        [in]       weight              : the weight for the speed
 * @param        [in]       numPoints           : the number of detected objects for clustering
 * @param        [in]       minPointsInCluster  : minimum number of neighbour points for a point to be in a cluster
 * @param        [in]       scratchBuf          : scratch memory buffer for visited buffer, and neighbour index for
 *                                                each cluster, the length is 3*ceil(numPoints/8)*8, must be 8 bytes aligned
 * @param        [out]      numClusters         : the detected number of clusters
 * @param        [out]      clusterIdxArray     : the cluster ID for each detected objects, clusterID starts at 1.
 * @param        [out]      clusterInfor        : the cluster information, inlcuding center, and min and max value for each cluster
 *
 *
 *
 * @return                  Number of detected peaks, for this function, it is always 1.
 *
 * @pre                     (1) scratchBuf is 8 byte aligned.
 *                          (2) input speedArray and locationXY are 8 byte aligned.
 *
 * @ingroup                 MMWAVELIB_CLUSTERING
 *
 * cycles                   cgt: 8.1.3
 *                          1 cluster:  2.1052 * numPoints^2 + 33912
 *                          2 clusters: 2.1046 * numPoints^2 + 33828
 *                          4 clusters: 2.1040 * numPoints^2 + 33913
 *                          5 clusters: 2.0981 * numPoints^2 + 39361
 *                          8 clusters: 2.0859 * numPoints^2 + 48195
 *
 *                          2.1023 * numPoints^2+35913
 *******************************************************************************************************************
 */
void mmwavelib_dbscan_skipFoundNeiB(cplx16_t locationXY[],
                         int16_t speedArray[],
                         int32_t epsilon,
                         float32_t weight,
                         uint16_t numPoints,
                         uint16_t minPointsInCluster,
                         int8_t scratchBuf[],
                         uint16_t *numClusters,
                         uint16_t clusterIdxArray[],
                         mmwavelib_clusterInfor clusterInfor[]
                        )
{
    uint16_t idx,idx1;
    uint16_t pointIdx;
    uint16_t newNeighborCount,neighborCount;
    uint16_t clusterId;
    int8_t  *visited;
    uint16_t *neighbor;


    _nassert(((uint32_t)scratchBuf % 8U) == 0); /* 8 byte aligned */
    /* global visited array */
    visited     = (int8_t *) &scratchBuf[0];
    /* save the neighbor for every cluster */
    if (numPoints & 0x7U)
    {
        neighbor   = (uint16_t *)&scratchBuf[(uint16_t)(((uint16_t)(numPoints >> 3U) + 1U) << 3U)];
    }
    else
    {
        neighbor   = (uint16_t *)&scratchBuf[numPoints];
    }

    /* initialize the visited to 0- not visited, and clusterIdxArray to not any cluster */
    idx1= (uint16_t)(((uint16_t)(numPoints >> 3U)) << 3U);
    for (idx = 0; idx < idx1 ; idx += 8U)
    {
        _amem8(&visited[idx]) = 0;
    }

    for (idx = idx1; idx < numPoints; idx++)
    {
        visited[idx] = 0;
    }

    clusterId = 0;
    /* go through all points */
    for (pointIdx = 0; pointIdx < numPoints; pointIdx++)
    {
        if (visited[pointIdx] != POINT_VISITED) /* not visited before */
        {
            /* mark it as visited */
            visited[pointIdx] = POINT_VISITED;

            /* find the neighbour */
            newNeighborCount = mmwavelib_dbscan_findNeighbors2(
                                                               locationXY,
                                                               speedArray,
                                                               pointIdx,
                                                               epsilon,
                                                               weight,
                                                               numPoints,
                                                               visited,
                                                               neighbor);


            /* check if it is a noise or a cluster */
            if (newNeighborCount < minPointsInCluster)
            {
                /* mark it as noise */
                clusterIdxArray[pointIdx] = 0;
            }
            else
            {
                /* it is a new cluster */
                clusterId++;
                /* set the point to clusterId */
                clusterIdxArray[pointIdx] = clusterId;

                for (idx = 0; idx < newNeighborCount; idx++)
                {
                    visited[neighbor[idx]] = 1;
                }


                neighborCount = mmwavelib_dbscan_skipFoundNeiB_expandCluster(locationXY,
                                                  speedArray,
                                                  newNeighborCount,
                                                  epsilon,
                                                  weight,
                                                  minPointsInCluster,
                                                  numPoints,
                                                  visited,
                                                  neighbor,
                                                  clusterId,
                                                  clusterIdxArray);

                /* add the original point into the cluster */
                neighbor[neighborCount] = pointIdx;
                neighborCount++;

                /* find out the cluster center and edge information */
                mmwavelib_dbscan_calCluster (locationXY,
                                             speedArray,
                                             neighborCount,
                                             neighbor,
                                             &clusterInfor[clusterId-1U]);


            }/* end of else new cluster */

        }/* end of if visited[point]!=POINT_VISITED */


    }/* end of for loop */


    *numClusters = clusterId;

}


/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_dbscan_skipFoundNeiB_expandCluster
 *
 * \par
 * <b>Description</b>  :    expands the cluster based on the found neighbors, when expanding, ignore the already found neighbors
 *
 *
 * @param        [in]       locationXY         : two dimension locations of the detected objects
 * @param        [in]       speedArray         : speed array of the detected objects
 * @param        [in]       currentNeighborCount : the number of already detected neighbours
 * @param        [in]       epsilon            : the distancd and speed threshold for clustering
 * @param        [in]       weight             : the weight for the speed
 * @param        [in]       minPointsInCluster : the minimum number of neighbors for a point to be in a cluster
 * @param        [in]       numPoints          : the total number of objects
 * @param        [in]       visited            : the global visited buffer
 * @param        [inout]    neighbor           : the buffer saves the found neighbors
 * @param        [in]       clusterId          : the current cluster ID.
 * @param        [out]      clusterIdxArray    : the updated cluster ID array
 *
 *
 * @return                  the total number of found neighbors for the cluster.
 *
 * @pre
 *
 * @ingroup                 MMWAVELIB_CLUSTERING
 *
 * cycles
 *
 *******************************************************************************************************************
 */

uint16_t mmwavelib_dbscan_skipFoundNeiB_expandCluster(cplx16_t locationXY[],
                                       int16_t speedArray[],
                                       uint16_t currentNeighborCount,
                                       int32_t epsilon,
                                       float32_t weight,
                                       uint16_t minPointsInCluster,
                                       uint16_t numPoints,
                                       int8_t visited[],
                                       uint16_t * neighbor,
                                       uint16_t clusterId,
                                       uint16_t clusterIdxArray[]
                                       )
{
    uint16_t idx,idx1;
    uint16_t neighborCount,newNeighborCount;

    idx=0;
    neighborCount = currentNeighborCount;
    /* expand the new cluster for every neighbour */
    while (idx < neighborCount)
    {
        /*if (!clusterIdxArray[neighbor[idx]])  *//* if it is not any cluster */
        clusterIdxArray[neighbor[idx]] = clusterId; /*assign clusterId to the neighbor */

        /*if (visited[neighbor[idx]]!=POINT_VISITED) */
        {
            visited[neighbor[idx]] = POINT_VISITED;
            /* find its new neighbour */
            newNeighborCount = mmwavelib_dbscan_findNeighbors2(locationXY,
                                                               speedArray,
                                                               neighbor[idx],
                                                               epsilon,
                                                               weight,
                                                               numPoints,
                                                               visited,
                                                               &neighbor[neighborCount]);



            if (newNeighborCount >= minPointsInCluster) /*new neighbor */
            {
                for (idx1 = 0; idx1 < newNeighborCount; idx1++)
                {
                    visited[neighbor[neighborCount+idx1]] = POINT_VISITED;  /* update the new added neighbors */
                }
                neighborCount += newNeighborCount; /* only add in the new neighbor, skip the already add in neighbor */

            }
        }

        idx++;


    }/* end of while */

    return neighborCount;

}
