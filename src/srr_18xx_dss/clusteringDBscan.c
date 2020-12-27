/*!
 *  \file   clusteringDBscan.c
 *
 *  \brief   DBscan clustering module.
 *
 *  Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com/
 *
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
 *
*/

/* C674x mathlib */
#include <ti/mathlib/mathlib.h>
#include <string.h>
#include <stdint.h>

#include "dss_data_path.h"
#include "clusteringDBscan.h"





/*!
   \fn     clusteringDBscanRun

   \brief   Run clusteringDBscan module.

   \param[in]    handle
               Pointer to input handle for clusteringDBscan module.

   \param[in]    detObj2D
               Pointer to input data (object list).

    \param[in]    numDetectedObj
               number of Detected Objects .

   \param[out]    output
               Pointer to output data from clusteringDBscan module.

   \param[out]    trackingInput
               Input to the tracking module.

   \ret     Error code.

   \pre       none

   \post      none


 */
int32_t clusteringDBscanRun(clusteringDBscanInstance_t *inst,
                            DSS_DataPathObj *obj,
                            uint16_t numDetetectedObj,
                            clusteringDBscanOutput_t *output,
                            trackingInputReport_t *trackingInput)
{
    uint16_t *neighLast;
    uint16_t *neighCurrent;
    uint16_t neighCount;
    uint16_t newCount;
    uint16_t point, member;
    uint16_t numPoints;
    uint16_t clusterId;
    uint16_t ind;

    float epsilon,epsilon2, weight;
    int32_t epsilon2fixed;
    int32_t epsilonfixed;
    int32_t vFactorfixed;
    
    MmwDemo_detectedObjActual * detObj2D = obj->detObj2D;
    
    numPoints    = numDetetectedObj;
    clusterId    =    0;
    epsilon        =    inst->epsilon;
    epsilon2    =    epsilon*epsilon;
    weight        =    inst->weight;
    
    epsilon2fixed   =   _spint(epsilon2 * (float)(inst->fixedPointScale * inst->fixedPointScale));
    epsilonfixed = _spint(epsilon * inst->fixedPointScale);
    vFactorfixed = (int32_t)(inst->vFactor * inst->fixedPointScale);
    
    memset(inst->visited, POINT_UNKNOWN, numPoints*sizeof(char));

    // scan through all the points to find its neighbors
    for(point = 0; point < numPoints; point++)
    {
        if(inst->visited[point] != POINT_VISITED)
        {
            inst->visited[point] = POINT_VISITED;

            neighCurrent = neighLast = inst->neighbors;
            // scope is the local copy of visit
            memcpy(inst->scope, inst->visited, numPoints*sizeof(char));

            neighCount = clusteringDBscan_findNeighbors2Fixed(
                detObj2D, point, neighLast, numPoints, epsilon2fixed, epsilonfixed, weight,
                vFactorfixed,inst->scope, &newCount, inst->dBScanNeighbourLim);

            /* The cluster consists of the point itself and its neighbours. */    
            if(neighCount < inst->minPointsInCluster - 1) 
            {
                // This point is Noise
                output->IndexArray[point] = 0;
            }
            else
            {
                // This point belongs to a New Cluster
                clusterId++;                                // New cluster ID
                output->IndexArray[point] = clusterId;      // This point belong to this cluster

                // tag all the neighbors as visited in scope so that it will not be found again when searching neighbor's neighbor.
                for (ind = 0; ind < newCount; ind ++)
                {
                    member = neighLast[ind];
                    inst->scope[member] = POINT_VISITED;
                }
                neighLast += newCount;

                while (neighCurrent != neighLast)               // neigh shall be at least minPoints in front of neighborhood pointer
                {
                    // Explore the neighborhood
                    member = *neighCurrent++;               // Take point from the neighborhood
                    output->IndexArray[member] = clusterId; // All points from the neighborhood also belong to this cluster
                    inst->visited[member] = POINT_VISITED;

                    neighCount = clusteringDBscan_findNeighbors2Fixed(
                                            detObj2D,    member, neighLast,     numPoints,
                                            epsilon2fixed, epsilonfixed, weight,
                                            vFactorfixed, inst->scope, &newCount, 
                                            inst->dBScanNeighbourLim);

                    if(neighCount >= inst->minPointsInCluster)
                    {
                        int32_t speed = (int32_t) detObj2D[point].speed;

                        /* Check if the new point's velocity is the same velocity as the  
                         * clusterOriginator. */
                        if (_abs(detObj2D[member].speed - speed) < vFactorfixed)
                        {
                            for (ind = 0; ind < newCount; ind ++)
                            {
                                member = neighLast[ind];
                                inst->scope[member] = POINT_VISITED;
                            }
                            neighLast += newCount;              /* Member is a core point, and its neighborhood is added to the cluster */
                        }
                    }
                }

                if (clusterId >= inst->maxClusters)
                {
                    return DBSCAN_ERROR_CLUSTER_LIMIT_REACHED;
                }

                /* calculate the clustering center and edge information. */
                clusteringDBscan_calcInfoFixed(point, 
                    detObj2D, inst->neighbors, neighLast,
                    (clusteringDBscanReport_t *)&output->report[clusterId - 1], 
                    &trackingInput[clusterId - 1], obj);
            }
        }
    }
    output->numCluster = clusterId;

    return DBSCAN_OK;
}



/*!
   \fn     clusteringDBscan_findNeighbors2Fixed

   \brief   The core algorithm of dBScan, where for each point, the list of 
            its neighbours are found based on a distance metric. 
            
   \param[in]    detObj2D
               Pointer to input data (object list).
   
   \param[in]    point
               The current point. 
               
   \param[out]   neigh,
               Pointer to the array holding the cluster's points. If the current
               point has enough neighbours, it is upgraded to a core point, and 
               its neighbours are added to the cluster. 

   \param[in]    numPoints
               Number of detected objects. 

   \param[in]    epsilon2
               square of the distance metric (in fixed point)
    
    \param[in]    epsilon
               the distance metric (in fixed point)
    
    \param[in]    weight
                the relative weight assigned to the velocity difference. 
                
    \param[in]    vFactor
                the velocity metric. 
                    
    \param[in]    visited
                An array holding the state of the points. In this context, state
                means whether the point has been visited before. 
    \param[out]    newCount
                The number of neighbours for this point. 
    \param[in]    dBScanNeighbourLim
                terminate the algorithm if the number of neighbours for this 
                point is greater than this number (currently unused)

   \ret     number of neighbours.

   \pre       none

   \post      none

 */
uint16_t clusteringDBscan_findNeighbors2Fixed(
                    MmwDemo_detectedObjActual * restrict detObj2D,
                    uint16_t point,
                    uint16_t *restrict neigh,
                    uint16_t numPoints,
                    int32_t epsilon2,
                    int32_t epsilon,
                    float weight,
                    int32_t vFactor,
                    char * restrict visited,
                    uint16_t * restrict newCount,
                    uint16_t dBScanNeighbourLim)
{
    int32_t a,b,c;
    uint16_t i;
    int32_t sum, epsilon2WithSpeed, itemp;
    uint32_t newCounttmp = 0;
    
    _nassert((uint32_t)detObj2D % 8 == 0);
    int16_t x = detObj2D[point].x;
    int16_t y = detObj2D[point].y;
    int16_t speed = detObj2D[point].speed;
    uint16_t range = detObj2D[point].range;

    itemp        =    vFactor;
    if (_abs(speed) < vFactor)
    {
        itemp = _abs(speed);
    }

    epsilon2WithSpeed = (int32_t)(itemp*itemp*weight + epsilon2);
    
    for(i = 0; i < numPoints; i++)
    {
        if (visited[i] == POINT_VISITED)
        {
            continue; 
        }
            
        if (_abs(detObj2D[i].speed - speed) > itemp)
        {
            continue;
        }    
        
        if (_abs(detObj2D[i].range - range) > epsilon)
        {
            continue;
        }    

        a = detObj2D[i].x - x;
        b = detObj2D[i].y - y;
        c = detObj2D[i].speed - speed;
        
        sum = (a*a) + (b*b) + (int32_t)(weight*(float)(c*c));

        if (sum < epsilon2WithSpeed)
        {
            /* Mark this point as a neighbour in the list of 
             * neighbours. Also increment the number of neighbours
             * for this point. */
            *neigh = i;

            neigh++;
   
            newCounttmp++;
        }
    }

    *newCount = (uint16_t) newCounttmp;
   return newCounttmp;
}


/*!
   \fn     clusteringDBscan_calcInfoFixed

   \brief   When this function is called it means that one cluster has been found. This function
            then computes the center and the size of the cluster. Optionally it will also computes
            the input to the tracking module from this cluster. 
            

   \param[in]    clusterOriginator
               Index to the first entry into the cluster. 

   \param[in]    detObj2D
               Pointer to input data (object list).
               
   \param[in]    neighStart
               Pointer to the array holding the cluster's points.

   \param[in]    neighLast
               Pointer to the end of the array of the cluster's points.

   \param[out]    report
               Pointer to the current cluster's 'report'. The 'report' 
               contains information (size and position of the center) of 
               the structure 

   \param[out]    trackingInput
               Pointer to the current cluster's 'tracking input'. 

   \ret       none.

   \pre       none

   \post      none


 */void clusteringDBscan_calcInfoFixed(
                            uint16_t clusterOriginator,
                            MmwDemo_detectedObjActual * restrict detObj2D,
                            uint16_t * restrict neighStart,
                            uint16_t * restrict neighLast,
                            clusteringDBscanReport_t * restrict report, 
                            trackingInputReport_t * restrict trackingInput,
                            DSS_DataPathObj *obj)
{
    int16_t ind, length, member, strongestMember;
    float sumx, sumy, sumVel, xCenter, yCenter, xSize, ySize, avgVel, temp;
    float lengthInv;
    uint16_t strongestPeakVal;
    length = (neighLast - neighStart);
    float   maxSizeflt;
    sumx = detObj2D[clusterOriginator].x;
    sumy = detObj2D[clusterOriginator].y;
    sumVel = detObj2D[clusterOriginator].speed;
    
    for (ind = 0; ind < length; ind++)
    {
        member = neighStart[ind];
        sumx += (float)(detObj2D[member].x);
        sumy += (float)(detObj2D[member].y);
        sumVel += (float)(detObj2D[member].speed);
    }
    
    lengthInv = recipsp((float)(length + 1));
    xCenter = sumx * lengthInv;
    yCenter = sumy * lengthInv;
    avgVel  = sumVel * lengthInv;
    xSize = 0;
    ySize = 0;
    
    strongestPeakVal = detObj2D[clusterOriginator].peakVal;
    strongestMember = clusterOriginator;

    temp = ( ((float)detObj2D[strongestMember].x) - xCenter);
    temp = ((temp > 0)? (temp): (-temp)); //abs
    xSize =(xSize > temp)? (xSize):(temp); //max
    temp = ( ((float)detObj2D[strongestMember].y) - yCenter);
    temp = ((temp > 0)? (temp): (-temp)); //abs
    ySize = ((ySize > temp)? (ySize):(temp));//max

    for (ind = 0; ind < length; ind++)
    {
        member = neighStart[ind];
        temp = ( ((float)detObj2D[member].x) - xCenter);
        temp = ((temp > 0)? (temp): (-temp)); //abs
        xSize =(xSize > temp)? (xSize):(temp); //max
        temp = ( ((float)detObj2D[member].y) - yCenter);
        temp = ((temp > 0)? (temp): (-temp)); //abs
        ySize = ((ySize > temp)? (ySize):(temp));//max
        
        if(detObj2D[member].peakVal > strongestPeakVal)
        {
            strongestPeakVal = detObj2D[member].peakVal;
            strongestMember = member;
        }
    }
    
    /* If the size is absurd, due to the fact that the cluster has only 
     * one point, put in a more reasonable number. */
    if ((xSize == 0) || (ySize == 0))
    {
        uint32_t oneQFormat = (1 << obj->xyzOutputQFormat);
        xSize = (uint16_t) (3.0f * obj->rangeResolution * oneQFormat); 
        ySize = (uint16_t) (3.0f * obj->rangeResolution * oneQFormat); 
    }
    /* The clusterOriginator is also added to the number of points in 
     * the cluster */
    report->numPoints = length + 1;
    report->xCenter = (int16_t)(xCenter); 
    report->yCenter = (int16_t)(yCenter);  
    report->xSize  = (int16_t)(xSize);     
    report->ySize  = (int16_t)(ySize);    
    report->avgVel = (int16_t)(avgVel);

    /* Populate the inputs to the Kalman filter, if necessary. */
    
}
   
