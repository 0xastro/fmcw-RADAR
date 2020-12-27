/*!
 *  \file   clusteringDBscan.h
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


#ifndef _DBSCAN_H_
#define _DBSCAN_H_


//!  \brief #defines for the state of the detected points w.r.t to the clustering algorithms. 
#define POINT_UNKNOWN 0
#define POINT_VISITED 1
    
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
                    uint16_t dBScanNeighbourLim);

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
               contain information (size and position of the center) of 
               the structure 

   \param[out]    trackingInput
               Pointer to the current cluster's 'tracking input'. 

   \ret       none.

   \pre       none

   \post      none


 */
void clusteringDBscan_calcInfoFixed(
                            uint16_t clusterOriginator,
                            MmwDemo_detectedObjActual * restrict detObj2D,
                            uint16_t * restrict neighStart,
                            uint16_t * restrict neighLast,
                            clusteringDBscanReport_t * restrict report, 
                            trackingInputReport_t * restrict trackingInput,
                            DSS_DataPathObj *obj);

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
 extern int32_t clusteringDBscanRun(clusteringDBscanInstance_t  *inst,
                                    DSS_DataPathObj *obj,
                                    uint16_t numDetetectedObj,
                                    clusteringDBscanOutput_t *output,
                                    trackingInputReport_t *trackingInput);
                                    
#endif //_DBSCAN_H_
