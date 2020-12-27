/*
  * (C) Copyright 2018, Texas Instruments Incorporated -  http://www.ti.com/
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
  *   @file     lib_peakpruning_debug.c
  *
  *   @brief    This file contains routines related to peak grouping/pruning routines.
*/
#include <c6x.h>
#include <string.h>

#include "lib_peakpruning_debug.h"

/*!************************************************************************************************
 * \brief
 * Function Name      :  lib_cfarPeakPruning
 *
 * \par
 * <b>Description</b>  : The function groups neighboring peaks into one. The grouping is done
 *    according to two input flags: groupInDopplerDirection and
 *    groupInDopplerDirection. For each detected peak the function checks
 *    if the peak is greater than its neighbors. If this is true, the peak is
 *    copied to the output list of detected objects. The neighboring peaks that are used
 *    for checking are taken from the detection matrix and copied into 3x3 kernel
 *    regardless of whether they are CFAR detected or not.
 *    Note: Function always reads 9 samples per detected object
 *    from L3 memory into local array tempBuff, but it only needs to read according to input flags.
 *    For example if only the groupInDopplerDirection flag is set, it only needs
 *    to read middle row of the kernel, i.e. 3 samples per target from detection matrix.
 * 
 *  @param[out]   objOut      Output array of  detected objects after peak grouping
 *  @param[in]    objRaw      Array of detected objects after CFAR detection
 *  @param[in]    numDetectedObjects Number of detected objects by CFAR
 *  @param[in]    detMatrix          Detection Range/Doppler matrix
 *  @param[in]    numDopplerBins     Number of Doppler bins
 *  @param[in]    maxRangeIdx        Maximum range limit
 *  @param[in]    minRangeIdx        Minimum range limit
 *  @param[in]    groupInDopplerDirection Flag enables grouping in Doppler direction
 *  @param[in]    groupInRangeDirection   Flag enables grouping in Range direction
 *  @param[in]    maxNumDetObjs      maximum number of grouped objects

*  @return
*      Number of detected objects after grouping
*  cycles: cgt (8.1.3)
*          both direction: 1123*numDetectedObjects+34, if numDetectedObjects < 60
*               numDetectedObjects, cycles
*                  60,             67591
*                  70,             79042
*                  80,             90575
*                  90,             102087
*          doppler/range direction: 903*numDetectedObjects
* 
*/

uint32_t lib_cfarPeakPruning(
                                mmwavelib_detObj*  objOut,
                                mmwavelib_detObj * objRaw,
                                uint32_t numDetectedObjects,
                                uint16_t* detMatrix,
                                uint32_t numDopplerBins,
                                uint32_t maxRangeIdx,
                                uint32_t minRangeIdx,
                                uint32_t groupInDopplerDirection,
                                uint32_t groupInRangeDirection,
                                uint32_t maxNumDetObjs)
                                
{
    int32_t i, j;
    int32_t rowStart, rowEnd;
    uint32_t numObjOut = 0;
    uint32_t rangeIdx, dopplerIdx, peakVal;
    uint16_t *tempPtr;
    uint16_t kernel[9], detectedObjFlag;
    int32_t k, l;
    uint32_t startInd, stepInd, endInd;

    /* set up kernel search index */
    if ((groupInDopplerDirection == 1) && (groupInRangeDirection == 1))
    {
        /* Grouping both in Range and Doppler direction */
        startInd = 0;
        stepInd = 1;
        endInd = 8;
    }
    else if ((groupInDopplerDirection == 0) && (groupInRangeDirection == 1))
    {
        /* Grouping only in Range direction */
        startInd = 1;
        stepInd = 3;
        endInd = 7;
    }
    else if  ((groupInDopplerDirection == 1) && (groupInRangeDirection == 0))
    {
        /* Grouping only in Doppler direction */
        startInd = 3;
        stepInd = 1;
        endInd = 5;
    }


    /* Start checking */
    for(i = 0; i < numDetectedObjects; i++)
    {
        detectedObjFlag = 0;
        rangeIdx = objRaw[i].rangeIdx;
        dopplerIdx = objRaw[i].dopplerIdx;
        peakVal = objRaw[i].peakVal;
        if((rangeIdx <= maxRangeIdx) && (rangeIdx >= minRangeIdx))
        {
            detectedObjFlag = 1;

            /* Fill local 3x3 kernel from detection matrix in L3*/
            tempPtr = detMatrix + (rangeIdx-1)*numDopplerBins;  /* not wrap around range bin */
            rowStart = 0;
            rowEnd = 2;
            if (rangeIdx == minRangeIdx)
            {
                tempPtr = detMatrix + (rangeIdx)*numDopplerBins;
                rowStart = 1;
                memset((void *) kernel, 0, 3 * sizeof(uint16_t));  /* first row in kernel is all zero */
            }
            else if (rangeIdx == maxRangeIdx)
            {
                rowEnd = 1;
                memset((void *) &kernel[6], 0, 3 * sizeof(uint16_t)); /* last row in kernel is all zero */
            }


            for (j = rowStart; j <= rowEnd; j++)
            {
                for (k = 0; k < 3; k++)
                {
                    l =  (int32_t) dopplerIdx + (k - 1); /* add (int32_t) cast, otherwise, test fails when -o3 is enabled*/
                    if(l < 0)
                    {
                        l +=  numDopplerBins;
                    }
                    else if(l >= numDopplerBins)
                    {
                        l -=  numDopplerBins;
                    }
           
                    kernel[j*3+k] = tempPtr[l];
                }
                tempPtr += numDopplerBins;
            }
            /* Compare the detected object to its neighbors.
             * Detected object is at index 4*/
            for (k = startInd; k <= endInd; k += stepInd)
            {
                if(kernel[k] > kernel[4])
                {
                    detectedObjFlag = 0;
                }
            }
        }

        if (detectedObjFlag == 1)
        {
            objOut[numObjOut].rangeIdx = rangeIdx;
            objOut[numObjOut].dopplerIdx = dopplerIdx;
            objOut[numObjOut].peakVal = peakVal;
            numObjOut++;
        }
        if (numObjOut >= maxNumDetObjs)
        {
            break;
        }

    }

    return(numObjOut);
}

/*!************************************************************************************************
* \brief
* Function Name          :  lib_cfarPeakQualifiedInOrderPruning
*
* \par
* <b>Description</b> :   The function groups neighboring peaks into one. The grouping is done
*    according to two input flags: groupInDopplerDirection and
*    groupInDopplerDirection. For each detected peak the function checks
*    if the peak is greater than its neighbors. If this is true, the peak is
*    copied to the output list of detected objects. The neighboring peaks that
*    are used for checking are taken from the list of CFAR detected objects,
*    (not from the detection matrix), and copied into 3x3 kernel that has been
*    initialized to zero for each peak under test. If the neighboring cell has
*    not been detected by CFAR, its peak value is not copied into the kernel.
*    Note: Function always search for 8 peaks in the list, but it only needs to search
*    according to input flags.
* 
* 
*  @param[out]   objOut             Output array of  detected objects after peak grouping
*  @param[in]    objRaw             Array of detected objects after CFAR detection
*  @param[in]    numDetectedObjects  Number of detected objects by CFAR
*  @param[in]    numDopplerBins      Number of Doppler bins
*  @param[in]    maxRangeIdx         Maximum range limit
*  @param[in]    minRangeIdx         Minimum range limit
*  @param[in]    groupInDopplerDirection Flag enables grouping in Doppler direction
*  @param[in]    groupInRangeDirection   Flag enables grouping in Range direction
*  @param[in]    maxNumDetObjs           maximum number of grouped peaks
*
*  @pre          the cfar output peaks must be in the orders as ascending Doppler bin first, the ascend 
*                range bin 
*
*
*  @return       Number of detected objects after peak pruning
* 
  cycles         cgt: 8.1.3 123*numDetectedObjects*numDetectedObjects+96519
*                 
*/
uint32_t lib_cfarPeakQualifiedInOrderPruning(
                                mmwavelib_detObj*  objOut,
                                mmwavelib_detObj * objRaw,
                                uint32_t numDetectedObjects,
                                uint32_t numDopplerBins,
                                uint32_t maxRangeIdx,
                                uint32_t minRangeIdx,
                                uint32_t groupInDopplerDirection,
                                uint32_t groupInRangeDirection,
                                uint32_t maxNumDetObjs)
{
    int32_t i;
    uint32_t numObjOut = 0;
    uint32_t rangeIdx, dopplerIdx, peakVal;
    uint16_t kernel[9], detectedObjFlag;
    int32_t k;
    int32_t l;
    uint32_t startInd, stepInd, endInd;

    #define WRAP_DOPPLER_IDX(_x) ((_x) & (numDopplerBins-1))
    #define WRAP_DWN_LIST_IDX(_x) ((_x) >= numDetectedObjects ? ((_x) - numDetectedObjects) : (_x))
    #define WRAP_UP_LIST_IDX(_x) ((_x) < 0 ? ((_x) + numDetectedObjects) : (_x))

    if ((groupInDopplerDirection == 1) && (groupInRangeDirection == 1))
    {
        /* Grouping both in Range and Doppler direction */
        startInd = 0;
        stepInd = 1;
        endInd = 8;
    }
    else if ((groupInDopplerDirection == 0) && (groupInRangeDirection == 1))
    {
        /* Grouping only in Range direction */
        startInd = 1;
        stepInd = 3;
        endInd = 7;
    }
    else if ((groupInDopplerDirection == 1) && (groupInRangeDirection == 0))
    {
        /* Grouping only in Doppler direction */
        startInd = 3;
        stepInd = 1;
        endInd = 5;
    }
    

    /* Start checking  */
    for(i = 0; i < numDetectedObjects ; i++)
    {
        detectedObjFlag = 0;
        rangeIdx = objRaw[i].rangeIdx;
        dopplerIdx = objRaw[i].dopplerIdx;
        peakVal = objRaw[i].peakVal;

        if((rangeIdx <= maxRangeIdx) && (rangeIdx >= minRangeIdx))
        {
            detectedObjFlag = 1;
            memset((void *) kernel, 0, 9*sizeof(uint16_t));

            /* Fill the middle column of the kernel */
            kernel[4] = peakVal;

            if (i > 0)   /* cfar output ascending order for range index*/
            {
                if ((objRaw[i-1].rangeIdx == (rangeIdx-1)) &&
                    (objRaw[i-1].dopplerIdx == dopplerIdx))
                {
                    kernel[1] = objRaw[i-1].peakVal;
                }
            }

            if (i < (numDetectedObjects - 1))
            {
                if ((objRaw[i+1].rangeIdx == (rangeIdx+1)) &&
                    (objRaw[i+1].dopplerIdx == dopplerIdx))
                {
                    kernel[7] = objRaw[i+1].peakVal;
                }
            }

            /* Fill the left column of the kernel */
            k = WRAP_UP_LIST_IDX(i-1); /* cfar is done in ascending doppler bin order */
            for (l = 0; l < numDetectedObjects; l++)
            {
                if (objRaw[k].dopplerIdx == WRAP_DOPPLER_IDX(dopplerIdx - 2))   /*doppler index is wrapped, range index not */
                {
                    break;
                }
                if ((objRaw[k].rangeIdx == (rangeIdx + 1)) &&
                    (objRaw[k].dopplerIdx == WRAP_DOPPLER_IDX(dopplerIdx - 1)))
                {
                    kernel[6] = objRaw[k].peakVal;
                }
                else if ((objRaw[k].rangeIdx == (rangeIdx)) &&
                    (objRaw[k].dopplerIdx == WRAP_DOPPLER_IDX(dopplerIdx - 1)))
                {
                    kernel[3] = objRaw[k].peakVal;
                }
                else if ((objRaw[k].rangeIdx == (rangeIdx - 1)) &&
                    (objRaw[k].dopplerIdx == WRAP_DOPPLER_IDX(dopplerIdx - 1)))
                {
                    kernel[0] = objRaw[k].peakVal;
                }
                k = WRAP_UP_LIST_IDX(k-1);
            }

            /* Fill the right column of the kernel */
            k = WRAP_DWN_LIST_IDX(i+1);
            for (l = 0; l < numDetectedObjects; l++)
            {
                if (objRaw[k].dopplerIdx == WRAP_DOPPLER_IDX(dopplerIdx + 2))
                {
                    break;
                }
                if ((objRaw[k].rangeIdx == (rangeIdx - 1)) &&
                    (objRaw[k].dopplerIdx == WRAP_DOPPLER_IDX(dopplerIdx + 1)))
                {
                    kernel[2] = objRaw[k].peakVal;
                }
                else if ((objRaw[k].rangeIdx == (rangeIdx)) &&
                    (objRaw[k].dopplerIdx == WRAP_DOPPLER_IDX(dopplerIdx + 1)))
                {
                    kernel[5] = objRaw[k].peakVal;
                }
                else if ((objRaw[k].rangeIdx == (rangeIdx + 1)) &&
                    (objRaw[k].dopplerIdx == WRAP_DOPPLER_IDX(dopplerIdx + 1)))
                {
                    kernel[8] = objRaw[k].peakVal;
                }
                k = WRAP_DWN_LIST_IDX(k+1);
            }

            /* Compare the detected object to its neighbors.
             * Detected object is at index 4*/
            for (k = startInd; k <= endInd; k += stepInd)
            {
                if(kernel[k] > kernel[4])
                {
                    detectedObjFlag = 0;
                }
            }
        }
        if(detectedObjFlag == 1)
        {
            objOut[numObjOut].rangeIdx = rangeIdx;
            objOut[numObjOut].dopplerIdx = dopplerIdx;
            objOut[numObjOut].peakVal = peakVal;
            numObjOut++;
        }
        if (numObjOut >= maxNumDetObjs)
        {
            break;
        }
    }

    return(numObjOut);

}
