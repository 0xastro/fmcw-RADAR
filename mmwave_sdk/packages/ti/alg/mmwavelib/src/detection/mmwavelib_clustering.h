/*
 * (C) Copyright 2017, Texas Instruments Incorporated -  http://www.ti.com/
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
 *   @file     mmwavelib_clustering.h
 *
 *   @brief    Header file for DBscan clustering mmwavelib routines
 */

#ifndef MMWAVELIB_CLUSTERING_H
#define MMWAVELIB_CLUSTERING_H 1

#include <ti/alg/mmwavelib/include/mmwavelib_cplx_types.h>

#define POINT_VISITED 1
#define DBSCAN_FIXEDWEIGHTSHIFT 3U

/*! @brief cluster information structure */
typedef struct 
{
    uint16_t numPointsInCluster;       /**< number of points in the cluster */
    int16_t xCenter;          /**< cluster center on real part */
    int16_t yCenter;          /**< cluster center on imag part */
    int16_t xSize;            /**< cluster size on real */
    int16_t ySize;            /**< cluster size on imag */
    int16_t aveVelocity;      /**< average speed */
    float32_t rangeVar;           /**< range variance */
    float32_t velocityVar;        /**< speed variance */

}mmwavelib_clusterInfor;

void mmwavelib_dbscan(cplx16_t locationXY[],
                         int16_t speedArray[],
                         int32_t epsilon,
                         float32_t weight,
                         uint16_t numPoints,
                         uint16_t minPointsInCluster,
                         int8_t scratchBuf[],
                         uint16_t * numClusters,
                         uint16_t clusterIdxArray[],
                         mmwavelib_clusterInfor clusterInfor[]
                        );


uint16_t mmwavelib_dbscan_findNeighbors2(cplx16_t locationXY[],
                                            int16_t speedArray[],
                                            uint16_t pointIdx,
                                            int32_t epsilon2,
                                            float32_t weight,
                                            uint16_t numPoints,
                                            int8_t visited[],
                                            uint16_t * neighbor);


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
                                       uint16_t clusterIdxArray[]);

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
                                            uint16_t * neighbour);

void mmwavelib_dbscan_calCluster (cplx16_t locationXY[],
                                     int16_t speedArray[],
                                     uint16_t neighborCount,
                                     uint16_t neighborList[],
                                     mmwavelib_clusterInfor * clusterInf);



void mmwavelib_dbscan_skipFoundNeiB(cplx16_t locationXY[],
                         int16_t speedArray[],
                         int32_t epsilon,
                         float32_t weight,
                         uint16_t numPoints,
                         uint16_t minPointsInCluster,
                         int8_t scratchBuf[],
                         uint16_t * numClusters,
                         uint16_t clusterIdxArray[],
                         mmwavelib_clusterInfor clusterInfor[]
                        );


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
                                       uint16_t clusterIdxArray[]);

#endif  /* MMWAVELIB_CLUSTERING_H */
