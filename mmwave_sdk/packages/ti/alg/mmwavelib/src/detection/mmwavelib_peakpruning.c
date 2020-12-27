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
 *   @file     mmwavelib_peakpruning.c
 *
 *   @brief    This file contains routines related to cfar output peaks grouping/pruning .
 */
#include <c6x.h>
#include <string.h>

#include "mmwavelib_peakpruning.h"

/*!************************************************************************************************
 * \brief
 * Function Name      :  mmwavelib_cfarPeakPruning
 *
 * \par
 * <b>Description</b>  : The function groups neighboring peaks into one. The grouping is done
 *    according to two input flags: groupInRangeDirection and  groupInDopplerDirection. 
 *    For each detected peak the function checks if the peak is greater than its neighbors.
 *    If this is true, the peak is copied to the output list of detected objects. The neighboring
 *    peaks that are used for checking are taken from the detection matrix and copied into 
 *    3x3 kernel regardless of whether they are CFAR detected or not.
 *    Note: Function always reads 9 samples per detected object
 *    from L3 memory into local array tempBuff, but it only needs to read according to input flags.
 *    For example if only the groupInDopplerDirection flag is set, it only needs
 *    to read middle row of the kernel, i.e. 3 samples per target from detection matrix.
 * 
 *  @param[out]   grpPeakIdx      Output array with indices of detected objects after peak pruning
 *  @param[in]    peakRangeIdx    Array of detected objects range index after CFAR detection
 *  @param[in]    peakDopplerIdx  Array of detected objects Doppler index after CFAR detection
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
 *
 
 *  @ingroup       MMWAVELIB_PEAKPRUNING
 * 
 *   cycles       cgt: 8.1.3
 *                 both direction:
 *                     0.27*numDetectedObjects*numDetectedObjects + 39.5*numDetectedObjects + 344         
 *                 Doppler direction :  41 * numDetectedObjects + 90
 *                 range direction:
 *                     0.22*numDetectedObjects*numDetectedObjects + 31*numDetectedObjects + 232
 */

uint32_t mmwavelib_cfarPeakPruning(
                                uint16_t * grpPeakIdx,
                                uint16_t * peakRangeIdx,
                                uint16_t * peakDopplerIdx,
                                uint32_t numDetectedObjects,
                                uint16_t* detMatrix,
                                uint32_t numDopplerBins,
                                uint32_t maxRangeIdx,
                                uint32_t minRangeIdx,
                                uint32_t groupInDopplerDirection,
                                uint32_t groupInRangeDirection,
                                uint32_t maxNumDetObjs)
{
    uint32_t numObjOut = 0;
    uint32_t rangeIdx, dopplerIdx, peakVal;
    uint32_t rowIdx, i, numRows;
    int32_t columnIdx, columnCount;
    uint16_t detectedObjFlag;
    uint16_t *tempPtr;


    /* both range and Doppler direction */
    if ((groupInDopplerDirection == 1) && (groupInRangeDirection == 1))
    {
        /* Start checking */
        for (i = 0; i < numDetectedObjects; i++)
        {
            detectedObjFlag = 0;
            rangeIdx = peakRangeIdx[i];
            dopplerIdx = peakDopplerIdx[i];
            tempPtr = detMatrix + rangeIdx * numDopplerBins;
            peakVal = tempPtr[dopplerIdx];
            
            numRows = 0;
            if (rangeIdx == minRangeIdx) 
            {
                detectedObjFlag = 1;
                numRows = 2;
                tempPtr = detMatrix + rangeIdx * numDopplerBins;
            }
            else if (rangeIdx == maxRangeIdx)
            {
                detectedObjFlag = 1;
                numRows = 2;
                tempPtr = detMatrix + (rangeIdx - 1) * numDopplerBins;
            }
            else if ((rangeIdx < maxRangeIdx) && (rangeIdx > minRangeIdx))
            {
                detectedObjFlag = 1;
                numRows = 3;
                tempPtr = detMatrix + (rangeIdx - 1) * numDopplerBins;
            }

            for (rowIdx = 0; rowIdx < numRows; rowIdx++)
            {
                for (columnCount = 0; columnCount < 3; columnCount ++)
                {
                    columnIdx = (int32_t ) dopplerIdx + columnCount - 1;
                    if (columnIdx < 0)
                    {
                        columnIdx += numDopplerBins;
                    }
                    else if (columnIdx >= numDopplerBins)
                    {
                        columnIdx -= numDopplerBins;
                    }
                    if (peakVal < tempPtr[columnIdx]) 
                    {
                        detectedObjFlag = 0;
                    }
                }
                tempPtr += numDopplerBins;
            }
            if (detectedObjFlag == 1) 
            {
                grpPeakIdx[numObjOut] = i;
                numObjOut++;
            }
            if (numObjOut >= maxNumDetObjs)
            {
                break;
            }
        }/* end of for all detected peaks loop */

    }/* end of both range and Doppler direction pruning */
    else if ((groupInDopplerDirection == 1) && (groupInRangeDirection == 0))
    {
        /* Start checking */
        for (i = 0; i < numDetectedObjects; i++) 
        {
            detectedObjFlag = 0;
            rangeIdx = peakRangeIdx[i];
            dopplerIdx = peakDopplerIdx[i];

            tempPtr = detMatrix + rangeIdx*numDopplerBins;
            peakVal = tempPtr[dopplerIdx];


            if ((rangeIdx <= maxRangeIdx) && (rangeIdx >= minRangeIdx))
            {
                detectedObjFlag = 1;
                for (columnCount = 0; columnCount < 2; columnCount ++)
                {
                    columnIdx = (int32_t) dopplerIdx + 2 * columnCount - 1;
                    if (columnIdx < 0)
                    {
                        columnIdx += numDopplerBins;
                    }
                    else if (columnIdx >= numDopplerBins)
                    {
                        columnIdx -= numDopplerBins;
                    }
                    if (peakVal < tempPtr[columnIdx]) 
                    {
                        detectedObjFlag = 0;
                    }
                }
                if (detectedObjFlag == 1) 
                {
                    grpPeakIdx[numObjOut] = i;
                    numObjOut++;
                }
                if (numObjOut >= maxNumDetObjs) 
                {
                    break;
                }
            }
        }/* end of for all detected peaks loop */
    }/* end of Doppler direction pruning */
    else if ((groupInDopplerDirection == 0) && (groupInRangeDirection == 1))
    {
        /* Start checking */
        for (i = 0; i < numDetectedObjects; i++)
        {
            rangeIdx = peakRangeIdx[i];
            dopplerIdx = peakDopplerIdx[i];
            tempPtr = detMatrix + rangeIdx * numDopplerBins;
            peakVal = tempPtr[dopplerIdx];

            numRows = 1;
            detectedObjFlag = 1;
            if (rangeIdx == minRangeIdx) 
            {
                tempPtr += numDopplerBins;
            }
            else if (rangeIdx == maxRangeIdx)
            {
                tempPtr -= numDopplerBins;
            }
            else if ((rangeIdx < maxRangeIdx) && (rangeIdx > minRangeIdx))
            {
                numRows = 2;
                tempPtr -= numDopplerBins;
            }
            else
            {
                numRows = 0;
                detectedObjFlag = 0;
            }

            for (rowIdx = 0; rowIdx < numRows; rowIdx ++)
            {
                if (peakVal < tempPtr[dopplerIdx]) 
                {
                    detectedObjFlag = 0;
                }
                tempPtr += (numDopplerBins * 2);
            }
            if (detectedObjFlag == 1) 
            {
                grpPeakIdx[numObjOut] = i;
                numObjOut++;
            }
            if (numObjOut >= maxNumDetObjs) 
            {
                break;
            }
        }/* end of for all detected peaks loop */

    } /* end of range direction pruning */

    return numObjOut;
}

/*!************************************************************************************************
 * \brief
 * Function Name          :  mmwavelib_cfarPeakQualifiedInOrderPruning
 *
 * \par
 * <b>Description</b> :   The function groups neighboring peaks into one. The grouping is done
 *    according to two input flags: groupInRangeDirection and
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
 *  @param[out]   grpPeakIdx          Output array with indices of detected objects after peak pruning
 *  @param[in]    peakRangeIdx        Array of range index for the detected objects after CFAR detection
 *  @param[in]    peakDopplerIdx      Array of doppler index for the detected objects after CFAR detection
 *  @param[in]    peakValue           Array of power values for the detected objects after CFAR detection
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
 *  @ingroup       MMWAVELIB_PEAKPRUNING
 *
 *  @return       Number of detected objects after peak pruning
 * 
 *  cycles         cgt: 8.1.3
 *                 both directions: ~296*numDetectedObjects
 *                 Doppler Direction: ~271*numDetectedObjects
 *                 Range direction: 55.4* numDetectedObjects + 28
 */
uint32_t mmwavelib_cfarPeakQualifiedInOrderPruning(
                                    uint16_t *restrict grpPeakIdx,
                                    uint16_t *restrict peakRangeIdx,
                                    uint16_t *restrict peakDopplerIdx,
                                    uint16_t *restrict peakValue,
                                    uint32_t numDetectedObjects,
                                    uint32_t numDopplerBins,
                                    uint32_t maxRangeIdx,
                                    uint32_t minRangeIdx,
                                    uint32_t groupInDopplerDirection,
                                    uint32_t groupInRangeDirection,
                                    uint32_t maxNumDetObjs)
{
    int32_t i, j;
    uint32_t numObjOut = 0;
    uint16_t rangeIdx, dopplerIdx;
    uint16_t previousRangeIdx, nextRangeIdx, leftDopplerIdx, rightDopplerIdx;
    uint16_t kernel[9], kernel_4, detectedObjFlag;


    for (i = 0; i < numDetectedObjects; i++) 
    {
        rangeIdx = peakRangeIdx[i];
        dopplerIdx = peakDopplerIdx[i];
        detectedObjFlag = 1;

        if ((rangeIdx <= maxRangeIdx) && (rangeIdx >= minRangeIdx)) 
        {
            _mem8(&kernel[0]) = 0;
            _mem8(&kernel[5]) = 0;

            kernel_4 = peakValue[i];

            if (rangeIdx == minRangeIdx) 
            {
                previousRangeIdx = 0xffff;
            }
            else 
            {
                previousRangeIdx = rangeIdx - 1;
            }

            if (rangeIdx == maxRangeIdx)
            {
                nextRangeIdx = 0xffff;
            } 
            else
            {
                nextRangeIdx = rangeIdx + 1;
            }

            if (dopplerIdx == 0)
            {
                leftDopplerIdx = numDopplerBins - 1;
            } 
            else 
            {
                leftDopplerIdx = dopplerIdx - 1;
            }

            if (dopplerIdx == numDopplerBins - 1) 
            {
                rightDopplerIdx = 0;
            } 
            else 
            {
                rightDopplerIdx = dopplerIdx + 1;
            }

            /* range index is in ascending order*/
            if (i > 0) 
            {
                if ((peakRangeIdx[i - 1] == previousRangeIdx) && (peakDopplerIdx[i - 1] == dopplerIdx))
                {
                    kernel[1] = peakValue[i - 1];
                }
            }

            if (i < (numDetectedObjects - 1)) 
            {
                if ((peakRangeIdx[i + 1] == nextRangeIdx) && (peakDopplerIdx[i + 1] == dopplerIdx))
                {
                    kernel[7] = peakValue[i + 1];
                }
            }

            if (groupInDopplerDirection == 1) /* if both direction or doppler direction */
            {
                /* fill up left column*/
                if (dopplerIdx == 0) 
                {
                    /* Doppler index is in ascending order*/
                    for (j = numDetectedObjects - 1; j >= 0; j--)
                    {
                        if (peakDopplerIdx[j] < (numDopplerBins - 1))
                        {
                            break;
                        }

                        if (peakDopplerIdx[j] == leftDopplerIdx)
                        {
                            if (peakRangeIdx[j] == previousRangeIdx)
                            {
                                kernel[0] = peakValue[j];
                            }

                            if (peakRangeIdx[j] == rangeIdx)
                            {
                                kernel[3] = peakValue[j];
                            }

                            if (peakRangeIdx[j] == nextRangeIdx)
                            {
                                kernel[6] = peakValue[j];
                            }
                        }
                    }
                } 
                else
                {
                    for (j = i; j >= 0; j--)
                    {
                        if (peakDopplerIdx[j] < dopplerIdx - 1) 
                        {
                            break;
                        }

                        if (peakDopplerIdx[j] == leftDopplerIdx)
                        {
                            if (peakRangeIdx[j] == previousRangeIdx)
                            {
                                kernel[0] = peakValue[j];
                            }

                            if (peakRangeIdx[j] == rangeIdx)
                            {
                                kernel[3] = peakValue[j];
                            }

                            if (peakRangeIdx[j] == nextRangeIdx)
                            {
                                kernel[6] = peakValue[j];
                            }
                        }
                    }
                }

                /* fill the right columns*/
                if (dopplerIdx == numDopplerBins - 1) 
                {
                    for (j = 0; j < numDetectedObjects; j++)
                    {
                        if (peakDopplerIdx[j] > 0)
                        {
                            break;
                        }

                        if (peakDopplerIdx[j] == rightDopplerIdx)
                        {
                            if (peakRangeIdx[j] == previousRangeIdx)
                            {
                                kernel[2] = peakValue[j];
                            }

                            if (peakRangeIdx[j] == rangeIdx)
                            {
                                kernel[5] = peakValue[j];
                            }

                            if (peakRangeIdx[j] == nextRangeIdx)
                            {
                                kernel[8] = peakValue[j];
                            }
                        }
                    }
                } 
                else
                {
                    for (j = i; j < numDetectedObjects; j++)
                    {
                        if (peakDopplerIdx[j] > rightDopplerIdx)
                        {
                            break;
                        }

                        if (peakDopplerIdx[j] == rightDopplerIdx)
                        {
                            if (peakRangeIdx[j] == previousRangeIdx)
                            {
                                kernel[2] = peakValue[j];
                            }

                            if (peakRangeIdx[j] == rangeIdx)
                            {
                                kernel[5] = peakValue[j];
                            }

                            if (peakRangeIdx[j] == nextRangeIdx)
                            {
                                kernel[8] = peakValue[j];
                            }
                        }
                    }
                }
                /* Compare the detected object to its neighbors.
                 * Detected object is at index 4*/
                if (groupInRangeDirection == 1) /* both direction */
                {
                    for (j = 0; j < 9; j += 1)
                    {
                        if (j != 4 && kernel[j] > kernel_4)
                        {
                            detectedObjFlag = 0;
                        }
                    }
                }
                else /* doppler direction */
                {
                    if (kernel[3] > kernel_4)
                    {
                        detectedObjFlag = 0;
                    }

                    if (kernel[5] > kernel_4)
                    {
                        detectedObjFlag = 0;
                    }
                }
            }
            else /* range direction */
            {
                if (kernel[1] > kernel_4)
                {
                    detectedObjFlag = 0;
                }
                    
                if (kernel[7] > kernel_4)
                {
                    detectedObjFlag = 0;
                }
            }

            if (detectedObjFlag == 1)
            {
                grpPeakIdx[numObjOut] = i;
                numObjOut++;
            }

            if (numObjOut >= maxNumDetObjs) 
            {
                break;
            }

        }/*end of if rangeIdx in [minRangeIdx, maxRangeIdx] */
    }/* end of i loop to check each peak */

    return numObjOut;
}

/*!************************************************************************************************
 * \brief
 * Function Name      :   mmwavelib_cfarPeakQualifiedPruning
 *   
 *   The function implements the same peak pruning algorithm as mmwavelib_cfarPeakQualifiedInOrderPruning, but
 *   with no assumption for the order of  cfar detected peaks.
 * 
 *  @param[out]   grpPeakIdx        Output array with indices of detected objects after peak pruning
 *  @param[in]    peakRangeIdx      Array of range index for the detected objects after CFAR detection
 *  @param[in]    peakDopplerIdx    Array of doppler index for the detected objects after CFAR detection
 *  @param[in]    peakValue         Array of power values for the detected objects after CFAR detection
 *  @param[in]    numDetectedObjects Number of detected objects by CFAR
 *  @param[in]    numDopplerBins     Number of Doppler bins
 *  @param[in]    maxRangeIdx        Maximum range limit
 *  @param[in]    minRangeIdx        Minimum range limit
 *  @param[in]    groupInDopplerDirection Flag enables grouping in Doppler direction
 *  @param[in]    groupInRangeDirection   Flag enables grouping in Range direction
 *  @param[in]    maxNumDetObjs           maximum number of grouped peaks
 *
 *
 *  @ingroup       MMWAVELIB_PEAKPRUNING
 *
 *  @return        Number of detected objects after grouping
 * 
 * *  cycles       cgt: 8.1.3
 *                 both directions: 
 *                     4*numDetectedObjects*numDetectedObjects + 77*numDetectedObjects + 57
 *                 Doppler direction:
 *                     2.5*numDetectedObjects*numDetectedObjects + 69*numDetectedObjects + 52
 *                 Range direction:
 *                     2.5*numDetectedObjects*numDetectedObjects + 71*numDetectedObjects + 59
 *
 */

uint32_t mmwavelib_cfarPeakQualifiedPruning(
                                        uint16_t * grpPeakIdx,
                                        uint16_t * peakRangeIdx,
                                        uint16_t * peakDopplerIdx,
                                        uint16_t * peakValue,
                                        uint32_t numDetectedObjects,
                                        uint32_t numDopplerBins,
                                        uint32_t maxRangeIdx,
                                        uint32_t minRangeIdx,
                                        uint32_t groupInDopplerDirection,
                                        uint32_t groupInRangeDirection,
                                        uint32_t maxNumDetObjs)
{
    int32_t i, j;
    uint32_t numObjOut = 0;
    uint16_t rangeIdx, dopplerIdx, rangeIdxLast, dopplerIdxLast;
    uint16_t previousRangeIdx, nextRangeIdx, leftDopplerIdx, rightDopplerIdx;
    uint16_t kernel[9], kernel_4, detectedObjFlag;
    uint32_t * peakRangeIdxPtr;
    uint32_t * peakDopplerIdxPtr;

    uint32_t packRangePrevious, packRange, packRangeNext, packDopplerLeft, packDoppler, packDopplerRight;
    uint32_t tempRangeIdx, tempDopplerIdx;
    uint32_t tempRangeResult0, tempRangeResult1, tempRangeResult2, tempRangeResult3, tempRangeResult5, tempRangeResult6, tempRangeResult7, tempRangeResult8;
    uint32_t tempDopplerResult0, tempDopplerResult1, tempDopplerResult2, tempDopplerResult3, tempDopplerResult5, tempDopplerResult6, tempDopplerResult7, tempDopplerResult8;
    uint32_t tempResult0, tempResult1, tempResult2, tempResult3, tempResult5, tempResult6, tempResult7, tempResult8;
    int32_t  kernel_0_index, kernel_1_index, kernel_2_index, kernel_3_index, kernel_5_index, kernel_6_index, kernel_7_index, kernel_8_index;


    if ((groupInDopplerDirection == 1) && (groupInRangeDirection == 1)) 
    {

        for (i = 0; i < numDetectedObjects; i++) 
        {
            rangeIdx = peakRangeIdx[i];
            dopplerIdx = peakDopplerIdx[i];
            detectedObjFlag = 0;


            if ((rangeIdx <= maxRangeIdx) && (rangeIdx >= minRangeIdx))
            {
                detectedObjFlag = 1;
                _mem8(&kernel[0]) = 0;
                _mem8(&kernel[5]) = 0;
                kernel_4 = peakValue[i];

                if (rangeIdx == minRangeIdx) 
                {
                    previousRangeIdx = 0xffff;
                }
                else
                {
                    previousRangeIdx = rangeIdx - 1;
                }
                if (rangeIdx == maxRangeIdx) 
                {
                    nextRangeIdx = 0xffff;
                }
                else
                {
                    nextRangeIdx = rangeIdx + 1;
                }

                if (dopplerIdx == 0)
                {
                    leftDopplerIdx = numDopplerBins - 1;
                }
                else 
                {
                    leftDopplerIdx = dopplerIdx - 1;
                }
                if (dopplerIdx == numDopplerBins - 1) 
                {
                    rightDopplerIdx = 0;
                } 
                else
                {
                    rightDopplerIdx = dopplerIdx + 1;
                }

                packRangePrevious = _pack2(previousRangeIdx, previousRangeIdx);
                packRange = _pack2(rangeIdx, rangeIdx);
                packRangeNext = _pack2(nextRangeIdx, nextRangeIdx);

                packDopplerLeft = _pack2(leftDopplerIdx, leftDopplerIdx);
                packDoppler = _pack2(dopplerIdx, dopplerIdx);
                packDopplerRight = _pack2(rightDopplerIdx, rightDopplerIdx);


                peakRangeIdxPtr = (uint32_t *) peakRangeIdx;
                peakDopplerIdxPtr = (uint32_t *) peakDopplerIdx;

                /* remember peakValue indices for each kernel[] */
                /* default to i: the peakValue index of kernel[4] */
                kernel_0_index = i;
                kernel_1_index = i;
                kernel_2_index = i;
                kernel_3_index = i;
                kernel_5_index = i;
                kernel_6_index = i;
                kernel_7_index = i;
                kernel_8_index = i;

                for (j = 0; j < numDetectedObjects / 2; j++)
                {
                    /* read in range and doppler index */
                    tempRangeIdx = _mem8(peakRangeIdxPtr);
                    peakRangeIdxPtr++;
                    tempDopplerIdx = _mem8(peakDopplerIdxPtr);
                    peakDopplerIdxPtr++;

                    /* kernel 0 */
                    tempRangeResult0 = _cmpeq2(tempRangeIdx, packRangePrevious);
                    tempDopplerResult0 = _cmpeq2(tempDopplerIdx, packDopplerLeft);
                    tempResult0 = tempRangeResult0 & tempDopplerResult0;

                    /* if (tempResult0==2)
                    {
                        kernel[0] = peakValue[(j<<1)+1];
                    }
                    if (tempResult0==1)
                    {
                        kernel[0]=peakValue[(j<<1)];
                    }
                     */
                    if (tempResult0) 
                    {
                        kernel_0_index = (j << 1) + (tempResult0 - 1);
                    }


                    /* kernel 1 */
                    tempRangeResult1 = _cmpeq2(tempRangeIdx, packRangePrevious);
                    tempDopplerResult1 = _cmpeq2(tempDopplerIdx, packDoppler);
                    tempResult1 = tempRangeResult1 & tempDopplerResult1;
                    /*
                    if (tempResult1==2)
                    {
                        kernel[1] = peakValue[(j<<1)+1];
                    }
                    if (tempResult1==1)
                    {
                        kernel[1]=peakValue[(j<<1)];
                    }
                     */
                    if (tempResult1) 
                    {
                        kernel_1_index = (j << 1) + (tempResult1 - 1);
                    }

                    /* kernel 2 */
                    tempRangeResult2 = _cmpeq2(tempRangeIdx, packRangePrevious);
                    tempDopplerResult2 = _cmpeq2(tempDopplerIdx, packDopplerRight);
                    tempResult2 = tempRangeResult2 & tempDopplerResult2;
                    /*
                    if (tempResult2==2)
                    {
                        kernel[2] = peakValue[(j<<1)+1];
                    }
                    if (tempResult2==1)
                    {
                        kernel[2]=peakValue[(j<<1)];
                    }
                     */
                    if (tempResult2) 
                    {
                        kernel_2_index = (j << 1) + (tempResult2 - 1);
                    }


                    /* kernel 3 */
                    tempRangeResult3 = _cmpeq2(tempRangeIdx, packRange);
                    tempDopplerResult3 = _cmpeq2(tempDopplerIdx, packDopplerLeft);
                    tempResult3 = tempRangeResult3 & tempDopplerResult3;
                    /*
                    if (tempResult3==2)
                    {
                        kernel[3] = peakValue[(j<<1)+1];
                    }
                    if (tempResult3==1)
                    {
                        kernel[3]=peakValue[(j<<1)];
                    }
                     */
                    if (tempResult3) 
                    {
                        kernel_3_index = (j << 1) + (tempResult3 - 1);
                    }


                    /* kernel 5 */
                    tempRangeResult5 = _cmpeq2(tempRangeIdx, packRange);
                    tempDopplerResult5 = _cmpeq2(tempDopplerIdx, packDopplerRight);
                    tempResult5 = tempRangeResult5 & tempDopplerResult5;
                    /*
                    if (tempResult5==2)
                    {
                        kernel[5] = peakValue[(j<<1)+1];
                    }
                    if (tempResult5==1)
                    {
                        kernel[5]=peakValue[(j<<1)];
                    }
                     */
                    if (tempResult5) 
                    {
                        kernel_5_index = (j << 1) + (tempResult5 - 1);
                    }

                    /* kernel 6 */
                    tempRangeResult6 = _cmpeq2(tempRangeIdx, packRangeNext);
                    tempDopplerResult6 = _cmpeq2(tempDopplerIdx, packDopplerLeft);
                    tempResult6 = tempRangeResult6 & tempDopplerResult6;
                    /*
                    if (tempResult6==2)
                    {
                        kernel[6] = peakValue[(j<<1)+1];
                    }
                    if (tempResult6==1)
                    {
                        kernel[6]=peakValue[(j<<1)];
                    }
                     */
                    if (tempResult6) 
                    {
                        kernel_6_index = (j << 1) + (tempResult6 - 1);
                    }


                    /* kernel 7 */
                    tempRangeResult7 = _cmpeq2(tempRangeIdx, packRangeNext);
                    tempDopplerResult7 = _cmpeq2(tempDopplerIdx, packDoppler);
                    tempResult7 = tempRangeResult7 & tempDopplerResult7;
                    /*
                    if (tempResult7==2)
                    {
                        kernel[7] = peakValue[(j<<1)+1];
                    }
                    if (tempResult7==1)
                    {
                        kernel[7]=peakValue[(j<<1)];
                    }
                     */
                    if (tempResult7) 
                    {
                        kernel_7_index = (j << 1) + (tempResult7 - 1);
                    }


                    /* kernel 8 */
                    tempRangeResult8 = _cmpeq2(tempRangeIdx, packRangeNext);
                    tempDopplerResult8 = _cmpeq2(tempDopplerIdx, packDopplerRight);
                    tempResult8 = tempRangeResult8 & tempDopplerResult8;
                    /*
                    if (tempResult8==2)
                    {
                        kernel[8] = peakValue[(j<<1)+1];
                    }
                    if (tempResult8==1)
                    {
                        kernel[8]=peakValue[(j<<1)];
                    }
                     */
                    if (tempResult8) 
                    {
                        kernel_8_index = (j << 1) + (tempResult8 - 1);
                    }


                } /* end of j loop */

                /* assign kernel[] values using the searched indices */
                kernel[0] = peakValue[kernel_0_index];
                kernel[1] = peakValue[kernel_1_index];
                kernel[2] = peakValue[kernel_2_index];
                kernel[3] = peakValue[kernel_3_index];
                kernel[5] = peakValue[kernel_5_index];
                kernel[6] = peakValue[kernel_6_index];
                kernel[7] = peakValue[kernel_7_index];
                kernel[8] = peakValue[kernel_8_index];

                if (numDetectedObjects & 0x1) 
                {
                    rangeIdxLast = peakRangeIdx[numDetectedObjects - 1];
                    dopplerIdxLast = peakDopplerIdx[numDetectedObjects - 1];
                    if (rangeIdxLast == previousRangeIdx) 
                    {
                        if (dopplerIdxLast == leftDopplerIdx) 
                        {
                            kernel[0] = peakValue[numDetectedObjects - 1];
                        }

                        if (dopplerIdxLast == dopplerIdx) 
                        {
                            kernel[1] = peakValue[numDetectedObjects - 1];
                        }

                        if (dopplerIdxLast == rightDopplerIdx) 
                        {
                            kernel[2] = peakValue[numDetectedObjects - 1];
                        }
                    } 
                    else if (rangeIdxLast == rangeIdx)
                    {
                        if (dopplerIdxLast == leftDopplerIdx)
                        {
                            kernel[3] = peakValue[numDetectedObjects - 1];
                        }

                        if (dopplerIdxLast == rightDopplerIdx) 
                        {
                            kernel[5] = peakValue[numDetectedObjects - 1];
                        }
                    } 
                    else if (rangeIdxLast == nextRangeIdx)
                    {
                        if (dopplerIdxLast == leftDopplerIdx)
                        {
                            kernel[6] = peakValue[numDetectedObjects - 1];
                        }

                        if (dopplerIdxLast == dopplerIdx) 
                        {
                            kernel[7] = peakValue[numDetectedObjects - 1];
                        }

                        if (dopplerIdxLast == rightDopplerIdx) 
                        {
                            kernel[8] = peakValue[numDetectedObjects - 1];
                        }
                    }
                }
                /* Compare the detected object to its neighbors.
                 * Detected object is at index 4*/
                for (j = 0; j < 9; j += 1)
                {
                    if (j != 4 && kernel[j] > kernel_4)
                    {
                        detectedObjFlag = 0;
                    }
                }
            }/* end of if rangeIdx */

            if (detectedObjFlag == 1) 
            {
                grpPeakIdx[numObjOut] = i;
                numObjOut++;
            }

            if (numObjOut >= maxNumDetObjs) 
            {
                break;
            }

        }/* end of i loop to check each peak */
    }/* end of groupInRange and inDoppler direction */
    else if ((groupInRangeDirection == 1) && (groupInDopplerDirection == 0)) 
    {
        for (i = 0; i < numDetectedObjects; i++) 
        {
            rangeIdx = peakRangeIdx[i];
            dopplerIdx = peakDopplerIdx[i];
            detectedObjFlag = 0;

            if ((rangeIdx <= maxRangeIdx) && (rangeIdx >= minRangeIdx)) 
            {
                detectedObjFlag = 1;
                kernel[1] = 0;
                kernel[7] = 0;
                kernel_4 = peakValue[i];

                if (rangeIdx == minRangeIdx) 
                {
                    previousRangeIdx = 0xffff;
                } 
                else 
                {
                    previousRangeIdx = rangeIdx - 1;
                }
                if (rangeIdx == maxRangeIdx) 
                {
                    nextRangeIdx = 0xffff;
                } 
                else 
                {
                    nextRangeIdx = rangeIdx + 1;
                }

                packDoppler = _pack2(dopplerIdx, dopplerIdx);
                packRangePrevious = _pack2(previousRangeIdx, previousRangeIdx);
                packRangeNext = _pack2(nextRangeIdx, nextRangeIdx);

                peakRangeIdxPtr = (uint32_t *) peakRangeIdx;
                peakDopplerIdxPtr = (uint32_t *) peakDopplerIdx;

                for (j = 0; j < numDetectedObjects / 2; j++) 
                {
                    /* read in range and doppler index */
                    tempRangeIdx = _mem8(peakRangeIdxPtr);
                    peakRangeIdxPtr++;
                    tempDopplerIdx = _mem8(peakDopplerIdxPtr);
                    peakDopplerIdxPtr++;

                    /* kernel 1 */
                    tempRangeResult1 = _cmpeq2(tempRangeIdx, packRangePrevious);
                    tempDopplerResult1 = _cmpeq2(tempDopplerIdx, packDoppler);
                    tempResult1 = tempRangeResult1 & tempDopplerResult1;
                    /*
                    if (tempResult1 == 2) 
                    {
                        kernel[1] = peakValue[(j << 1) + 1];
                    }
                    if (tempResult1 == 1) 
                    {
                        kernel[1] = peakValue[(j << 1)];
                    }
                    */
                    if (tempResult1)
                    {
                        kernel[1] = peakValue[(j<<1) + (tempResult1-1)];
                    }


                    /* kernel 7 */
                    tempRangeResult7 = _cmpeq2(tempRangeIdx, packRangeNext);
                    tempDopplerResult7 = _cmpeq2(tempDopplerIdx, packDoppler);
                    tempResult7 = tempRangeResult7 & tempDopplerResult7;
                    /*
                    if (tempResult7 == 2) {
                        kernel[7] = peakValue[(j << 1) + 1];
                    }
                    if (tempResult7 == 1) {
                        kernel[7] = peakValue[(j << 1)];
                    }*/
                    if (tempResult7)
                    {
                        kernel[7] = peakValue[(j << 1)+(tempResult7-1)];
                    }
                }/* end of j loop */

                if (numDetectedObjects & 0x1) 
                {
                    rangeIdxLast = peakRangeIdx[numDetectedObjects - 1];
                    dopplerIdxLast = peakDopplerIdx[numDetectedObjects - 1];
                    if (dopplerIdxLast == dopplerIdx) 
                    {
                        if (rangeIdxLast == previousRangeIdx) 
                        {
                            kernel[1] = peakValue[numDetectedObjects - 1];
                        }

                        if (rangeIdxLast == nextRangeIdx) 
                        {
                            kernel[7] = peakValue[numDetectedObjects - 1];
                        }
                    }
                }

                if (kernel[1] > kernel_4)
                {
                    detectedObjFlag = 0;
                }

                if (kernel[7] > kernel_4)
                {
                    detectedObjFlag = 0;
                }
            }/* end of if rangeIdx */
            if (detectedObjFlag == 1) 
            {
                grpPeakIdx[numObjOut] = i;
                numObjOut++;
            }
            if (numObjOut >= maxNumDetObjs) 
            {
                break;
            }
        } /* end of i loop */
    }/* end of rangeDirection */
    else if ((groupInRangeDirection == 0) && (groupInDopplerDirection == 1)) 
    {
        for (i = 0; i < numDetectedObjects; i++) 
        {
            rangeIdx = peakRangeIdx[i];
            dopplerIdx = peakDopplerIdx[i];
            detectedObjFlag = 0;

            if ((rangeIdx <= maxRangeIdx) && (rangeIdx >= minRangeIdx)) 
            {
                detectedObjFlag = 1;
                kernel[3] = 0;
                kernel[5] = 0;
                kernel_4 = peakValue[i];

                if (dopplerIdx == 0) 
                {
                    leftDopplerIdx = numDopplerBins - 1;
                }
                else
                {
                    leftDopplerIdx = dopplerIdx - 1;
                }
                if (dopplerIdx == numDopplerBins - 1)
                {
                    rightDopplerIdx = 0;
                } 
                else
                {
                    rightDopplerIdx = dopplerIdx + 1;
                }

                packRange = _pack2(rangeIdx, rangeIdx);
                packDopplerLeft = _pack2(leftDopplerIdx, leftDopplerIdx);
                packDopplerRight = _pack2(rightDopplerIdx, rightDopplerIdx);

                peakRangeIdxPtr = (uint32_t *) peakRangeIdx;
                peakDopplerIdxPtr = (uint32_t *) peakDopplerIdx;

                for (j = 0; j < numDetectedObjects / 2; j++) 
                {
                    /* read in range and doppler index */
                    tempRangeIdx = _mem8(peakRangeIdxPtr);
                    peakRangeIdxPtr++;
                    tempDopplerIdx = _mem8(peakDopplerIdxPtr);
                    peakDopplerIdxPtr++;

                    /* kernel 3 */
                    tempRangeResult3 = _cmpeq2(tempRangeIdx, packRange);
                    tempDopplerResult3 = _cmpeq2(tempDopplerIdx, packDopplerLeft);
                    tempResult3 = tempRangeResult3 & tempDopplerResult3;
                    /*
                    if (tempResult3 == 2) {
                        kernel[3] = peakValue[(j << 1) + 1];
                    }
                    if (tempResult3 == 1) {
                        kernel[3] = peakValue[(j << 1)];
                    }
                    */
                    if (tempResult3)
                    {
                        kernel[3] = peakValue[(j << 1) + (tempResult3-1)];
                    }


                    /* kernel 5 */
                    tempRangeResult5 = _cmpeq2(tempRangeIdx, packRange);
                    tempDopplerResult5 = _cmpeq2(tempDopplerIdx, packDopplerRight);
                    tempResult5 = tempRangeResult5 & tempDopplerResult5;
                    /*
                    if (tempResult5 == 2) {
                        kernel[5] = peakValue[(j << 1) + 1];
                    }
                    if (tempResult5 == 1) {
                        kernel[5] = peakValue[(j << 1)];
                    }
                    */
                    if (tempResult5)
                    {
                         kernel[5] = peakValue[(j << 1) + (tempResult5-1)];
                    }
                }/* end of j loop */


                if (numDetectedObjects & 0x1) 
                {
                    rangeIdxLast = peakRangeIdx[numDetectedObjects - 1];
                    dopplerIdxLast = peakDopplerIdx[numDetectedObjects - 1];
                    if (rangeIdxLast == rangeIdx) 
                    {
                        if (dopplerIdxLast == leftDopplerIdx) 
                        {
                            kernel[3] = peakValue[numDetectedObjects - 1];
                        }

                        if (dopplerIdxLast == rightDopplerIdx) 
                        {
                            kernel[5] = peakValue[numDetectedObjects - 1];
                        }
                    }
                }

                if (kernel[3] > kernel_4)
                {
                    detectedObjFlag = 0;
                }

                if (kernel[5] > kernel_4)
                {
                    detectedObjFlag = 0;
                }
            }/* end of if rangeIdx */
            if (detectedObjFlag == 1)
            {
                grpPeakIdx[numObjOut] = i;
                numObjOut++;
            }
            if (numObjOut >= maxNumDetObjs)
            {
                break;
            }
        } /* end of i loop */
    }/* end of doppler direction group */
    return numObjOut;
}
