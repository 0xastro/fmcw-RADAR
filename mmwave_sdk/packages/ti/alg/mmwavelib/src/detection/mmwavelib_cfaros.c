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
  *   @file     mmwavelib_cfaros.c
  *
  *   @brief    This file contains routines for OS(Ordered Statistics) CFAR detection.
  */

#include <stdint.h>
#include <c6x.h>
#include "mmwavelib_detection.h"

#define CURR_INDX_INC1 ( ((WINDOW_SIZE/2)+1) + (((WINDOW_SIZE/2)+2)<<16) )
#define CURR_INDX_INC2 ( ((WINDOW_SIZE/2)+3) + (((WINDOW_SIZE/2)+4)<<16) )


/*!************************************************************************************************
 * \brief
 * Function Name      :    mmwavelib_cfarOS
 *
 * \par
 * <b>Description</b> :    Performs Ordered Statistics CFAR on 16-bit input data.
 *                         For a CUT(Cell Under Test) to be detected as peak, it has to be greater
 *                         than the k-th OS i.e. the k-th smallest sample in the search window
 *                         by certain threshold scaling factor. 
 *                         The search window includes WINDOW_SIZE/2 samples on each side of CUT. 
 *                         The first sample of the input array will be used to pre-fill the window 
 *                         since the first few CUTs don't have enough samples in left-side window. 
 *
 * @param        [in]      data  : pointer to input data array (16 bit unsigned integers)
 * @param        [in]      len   : length of input data.
 * @param        [in]      lenCUTend: <=len, length where Cell Under Test ends. The last
 *                                  (len-lenCUTend) samples will not be considered by the detector.
 * @param        [in]      k     : statistic order. k-th ordered statistic is the k-th smallest
 *                                 sample in the search window of WINDOW_SIZE.
 * @param        [in]      alpha : threshold scaling factor.
                                   CUT > (alpha/(2^N_FRAC_BITS_ALPHA))*kth_statistic e.g.
                                   CUT > (alpha/64)*kth_statistic is the peak detection criteria.
 * @param        [out]     detectedList : output array with indices of the detected peaks
 * @param        [out]     kosList      : array with corresponding k-th OS of the detected peaks
 *
 * @return                 Number of detected peaks (i.e length of detectedList and kosList)
 *
 * @pre                    1) 8-byte aligned data array; each input data sample <= 32767.
 * @pre                    2) lenCUTend <= len.
 * @pre                    3) 1 <= k <= WINDOW_SIZE. Normally it's 1/2 to 3/4 of WINDOW_SIZE.
 * @pre                    4) Input and output arrays are non-aliased.
 * @pre                    5) WINDOW_SIZE is multiple of 8.
 * @pre                    6) Enough memory should be allocated to detectedList and kosList.
 *
 * @ingroup                MMWAVELIB_DETECT
 *
 * Cycles (cgt 8.1.3)      62.375*lenCUTend +734 with WINDOW_SIZE=32
 *                         30*lenCUTend +244 with WINDOW_SIZE=16
 *                         16.25*lenCUTend +145 with WINDOW_SIZE=8
 **************************************************************************************************
 */
uint16_t mmwavelib_cfarOS(const uint16_t * restrict data,
                        const uint32_t len,
                        const uint32_t lenCUTend,
                        const int16_t k,
                        const int16_t alpha,
                        uint16_t * restrict detectedList,
                        uint16_t * restrict kosList)
{
    int16_t  sorted_indices[WINDOW_SIZE];
    uint16_t circbuffer[WINDOW_SIZE];

    uint16_t kth_statistic;
    int32_t  curr_indx;
    int32_t  n_detected_obj = 0;
    int32_t  circbuffer_position;
    int32_t  ik;
    uint32_t cell_under_test_val;
    int32_t  cell_under_test_pos;

    uint32_t prev_val;
    uint32_t new_sample;
    int32_t  new_indx, i;
    uint32_t new_sample_w, prev_val_w, k_actual_w;

    uint64_t * restrict pcircbuffer_store1;
    uint64_t * restrict pcircbuffer_store2;

    /* For Loop 1 */
    uint64_t  * restrict pindices_store1;

    uint32_t const_data0, curr_indx_w;
    int64_t  const_data0_dw;

    /* For Loop 2 &3 */
    uint64_t * pcircbuffer_dw0;
    uint64_t * pcircbuffer_dw1;
    uint64_t * psorted_indices_dw0;
    uint64_t * psorted_indices_dw1;

    int64_t circbuffer_dw0;
    int64_t circbuffer_dw1;

    int64_t sorted_indices_dw0;
    int64_t sorted_indices_dw1;

    uint32_t circbuffer_lo0, circbuffer_hi0;
    uint32_t circbuffer_lo1, circbuffer_hi1;

    uint32_t compare_result_hi0, compare_result_lo0;
    uint32_t compare_result_hi1, compare_result_lo1;

    uint32_t sorted_indices_hi0, sorted_indices_lo0;
    uint32_t sorted_indices_hi1, sorted_indices_lo1;

    uint32_t increment_val_hi0, increment_val_lo0;
    uint32_t increment_val_hi1, increment_val_lo1;

    uint32_t decrement_val_hi0, decrement_val_lo0;
    uint32_t decrement_val_hi1, decrement_val_lo1;
    int32_t new_indx0, new_indx1;

    uint32_t kth_statistic0,    kth_statistic1;
    int32_t k_w;
#if 0 /* comment out because no need to calc k+1 any more*/
    int32_t k_plus_1, k_plus_1_w,

    k_plus_1 = k + 1;
    k_plus_1_w = k_plus_1| (k_plus_1 << 16);
#endif

    /* Pre-compute and store doubled copies of k */
    k_w = (int32_t)k| ((int32_t)k << 16);


    /* The CFAR-OS algorithm uses the kth-ordered statistic to represent noise power to compare
     * against the cell-under-test. In other words, windows of samples on either side of the
     * the cell-under-test is sorted (together), and the k-th element of that sorted array is 
     * taken and scaled to form the threshold. The threshold is then compared against the 
     * cell-under-test. In this implementation, the window size (per side) is given by 
     * (WINDOW_SIZE/2). The threshold is computed by scaling the kth-statistic by alpha, which 
     * has N_FRAC_BITS_ALPHA fractional bits. 
     * 
     * The following assumptions are taken by the optimized implementation. 
     * 1. WINDOW_SIZE is expected to be a multiple of 8.
     * 2. max(data) ) <= ( (2^15)-1 ).
     * 3. The array 'detected_list' is potentially as long as 'data' since every sample could be
     *    detected. Hence, care should be taken to allocate enough memory to detected_list. */
     
    /* While the implementation may feel complex, it is fairly simple to explain at a high-level.
     * Assume that a sorted-array of the window samples already exists. (The size of this window
     * is given by WINDOW_SIZE). If a new sample were to be introduced into this array , then,
     * we need to do only a few things. We need to find out where in this sorted array does the
     * new sample fit in. Once that is found, we need to compute the kth statistic. (Note that
     * the window size is now WINDOW_SIZE + 1). Once we have computed the kth statistc, we need
     * to remove a sample (the last sample in the unsorted window) to allow the window to slide.
     * 
     * Note that a naive implementation will be quite slow, because, even though we have a sorted
     * array, inserting an element into the array will require that all subsequent elements to be
     * shifted by one position. Also the shifting out the 'last sample' requires knowledge of which
     * sample is the 'last sample'. A better implementation that is amenable to parallelisation by 
     * the DSP is to have a seperate array containing the sorted indices of the elements in the 
     * window. Inserting an element is simply computing the index of that element in the sorted 
     * array and updating the other entries based on its postion. Removing an element is simply 
     * overwriting that element's index with the index of the  element to be  inserted, and 
     * updating the other entries based on that elements position.
     *
     * The DSP has requirement that efficient loads be 64-bit word aligned, which requires that 
     * the window size be a multiple of 4. In this implementation, the windows on either side of 
     * the cell-under-test are of equal size (WINDOW_SIZE/2) and are considered together along 
     * with the cell-under-test as a single window. This would make the actual window to be 
     * (WINDOW_SIZE+1) long, making efficient access impossible.
     *
     * The way we overcome the above issue, is by considering the sample to be inserted 
     * seperately from the window (of size WINDOW_SIZE + 1). A buffer (called the circ-buffer)
     * is used to hold the window and is accessed as a circular  window. A circular buffer (of 
     * length 'len') is simply an array organized such that an access of the len+X is the same
     * as X. i.e. data[len+X] = data[X]. This is accomplished by taking the modulo X of the 
     * index and using it as the circbuffer index. 
     *
     * The current position representing the position where a sample (correspond to the end of 
     * the window) is to be removed and the incoming sample is to be inserted is called 
     * circbuffer_position. 
     *
     * The CFAR-OS algorithm works using the circbufferr and the array of sorted indices (called 
     * sorted_indices). 'sorted_indices' is an array that stores the indices of the circbuffer, 
     * if they were sorted. 
     *
     * There are three major loops in the algorithm. The first loop pre-initializes the
     * the circbuffer with data[0] and half of sorted_indices with indices. The second loop
     * initializes the latter half of the circbuffer with data[0:WINDOW_SIZE/2]. The final loop
     * performs the cfar-os routine. */

    /* 1. Initialize the circbuffer with data[0] for all the cells. We do this because the 
     * buffer is uninitialized and we would like to initialize the window with some 
     * reasonable parameters. 
    *
    * Since all the entries in the initialized circbuffer are the same, we can directly
     * populate the sorted indices of this array as ((WINDOW_SIZE/2)+1) to (WINDOW_SIZE).
     */

    _nassert(((uint32_t)data & 7U) ==0);
    _nassert(((uint32_t)sorted_indices & 7U) ==0);
    _nassert(((uint32_t)circbuffer & 7U) ==0);

    const_data0 = (uint32_t) data[0];
    const_data0 = const_data0 | (const_data0 << 16U);
    const_data0_dw = _itoll(const_data0,const_data0);

    pcircbuffer_store1 = (uint64_t *)circbuffer;
    pcircbuffer_store2 = (uint64_t *) (circbuffer + WINDOW_SIZE/2);
    pindices_store1 = (uint64_t *) (sorted_indices);

    for (curr_indx = 0; curr_indx < (WINDOW_SIZE/2); curr_indx+=4) 
    {
        /*  circbuffer[curr_indx] = data[0];
        circbuffer[curr_indx+ (WINDOW_SIZE / 2)] = data[0]; */
        *pcircbuffer_store1 = const_data0_dw;
        *pcircbuffer_store2 = const_data0_dw;

        /* sorted_indices[curr_indx] = curr_indx + (WINDOW_SIZE/2) + 1;*/
        curr_indx_w = curr_indx | (curr_indx << 16);
        *pindices_store1 = _itoll(_add2(curr_indx_w,CURR_INDX_INC2),_add2(curr_indx_w,CURR_INDX_INC1));

        pcircbuffer_store1++;
        pcircbuffer_store2++;
        pindices_store1++;
    }

    circbuffer_position = (WINDOW_SIZE / 2);

    /* 2. Insert the first WINDOW_SIZE/2 samples into the circbuffer while
     *    simultaneously sorting them. */

    for (curr_indx = 0; curr_indx < (WINDOW_SIZE/2); curr_indx++) 
    {
        new_sample = data[curr_indx];
        prev_val = circbuffer[circbuffer_position];

        new_sample_w = new_sample | (new_sample<<16U);
        prev_val_w = prev_val | (prev_val<<16U);;

    /* This single loop should accomplish the following three objectives.
        * a. Insert new_sample at circbuffer_position. Based on its position in the
        *    sorted circbuffer, the other elements in the sorted_indices should be
        *    incremented (if they are greater) or left as is (if they are
        *    smaller).
        * b. Compute the index of the new sample in the sorted array. Increment
        *    the index if the new_sample is greater (or equal to) than the other 
        *     elements of the circbuffer. 
        * c. Remove prev_val from the array. Based on its position in the sorted 
        *    circbuffer, the other indices in the sorted_indices should be 
        *    decremented (if they are greater or equal ) or left unchanged (if 
        *    they are smaller). 
        */

        /* Initialise the location of the new sample to the 1st position. However
         * The central computation involves an increment if the (new_sample >=
         * circbuffer[i]). However, the actual comparison performed by the DSP is
         * (circbuffer[i] > new_sample). To compute a not we do
         * 1-(circbuffer[i] > new_sample). Since the 1 is a common computation, it
         * is done outslde the loop. */
        new_indx0 = 1 + 4*(WINDOW_SIZE/8);
        new_indx1 = 4*(WINDOW_SIZE/8);

        /* The simple comparator loop. */
        pcircbuffer_dw0 = (uint64_t * ) circbuffer;
        pcircbuffer_dw1 = (uint64_t * ) &circbuffer[4];
        psorted_indices_dw0 = (uint64_t * ) sorted_indices;
        psorted_indices_dw1 = (uint64_t * ) &sorted_indices[4];
        for (i = 0; i < WINDOW_SIZE; i+=8) {

            circbuffer_dw0 = *pcircbuffer_dw0;
            circbuffer_dw1 = *pcircbuffer_dw1;

            circbuffer_hi0 = _hill(circbuffer_dw0);
            circbuffer_lo0 = _loll(circbuffer_dw0);

            circbuffer_hi1 = _hill(circbuffer_dw1);
            circbuffer_lo1 = _loll(circbuffer_dw1);

            /* 
                increment_val = 0;
                if (new_sample >= circbuffer[i]) {
                    new_indx++;
                }    else {
                    increment_val = 1;
                }
            */
            compare_result_hi0 = _cmpgt2(circbuffer_hi0,new_sample_w);
            compare_result_lo0 = _cmpgt2(circbuffer_lo0,new_sample_w);

            compare_result_hi1 = _cmpgt2(circbuffer_hi1,new_sample_w);
            compare_result_lo1 = _cmpgt2(circbuffer_lo1,new_sample_w);

            new_indx0 = new_indx0 - (_bitc4(compare_result_lo0) + _bitc4(compare_result_hi0)) ;
            new_indx1 = new_indx1 - (_bitc4(compare_result_lo1) + _bitc4(compare_result_hi1)) ;

            increment_val_hi0 = _deal((compare_result_hi0));
            increment_val_lo0 = _deal((compare_result_lo0));

            increment_val_hi1 = _deal((compare_result_hi1));
            increment_val_lo1 = _deal((compare_result_lo1));

            sorted_indices_dw0 = *psorted_indices_dw0;
            sorted_indices_dw1 = *psorted_indices_dw1;

            sorted_indices_hi0 = _hill(sorted_indices_dw0);
            sorted_indices_lo0 = _loll(sorted_indices_dw0);

            sorted_indices_hi1 = _hill(sorted_indices_dw1);
            sorted_indices_lo1 = _loll(sorted_indices_dw1);

            sorted_indices_hi0 = _add2(sorted_indices_hi0,increment_val_hi0);
            sorted_indices_lo0 = _add2(sorted_indices_lo0,increment_val_lo0);

            sorted_indices_hi1 = _add2(sorted_indices_hi1,increment_val_hi1);
            sorted_indices_lo1 = _add2(sorted_indices_lo1,increment_val_lo1);

            /* 
            if (prev_val <= circbuffer[i]) {
                increment_val--;
            }
            */
            compare_result_hi0 = _cmpgt2(prev_val_w,circbuffer_hi0);
            compare_result_lo0 = _cmpgt2(prev_val_w,circbuffer_lo0);

            compare_result_hi1 = _cmpgt2(prev_val_w,circbuffer_hi1);
            compare_result_lo1 = _cmpgt2(prev_val_w,circbuffer_lo1);

            decrement_val_hi0 = _deal(3-(compare_result_hi0));
            decrement_val_lo0 = _deal(3-(compare_result_lo0));

            decrement_val_hi1 = _deal(3-(compare_result_hi1));
            decrement_val_lo1 = _deal(3-(compare_result_lo1));

            sorted_indices_hi0 = _sub2(sorted_indices_hi0,decrement_val_hi0);
            sorted_indices_lo0 = _sub2(sorted_indices_lo0,decrement_val_lo0);

            sorted_indices_hi1 = _sub2(sorted_indices_hi1,decrement_val_hi1);
            sorted_indices_lo1 = _sub2(sorted_indices_lo1,decrement_val_lo1);

            *psorted_indices_dw0 = _itoll(sorted_indices_hi0,sorted_indices_lo0);
            *psorted_indices_dw1 = _itoll(sorted_indices_hi1,sorted_indices_lo1);

            psorted_indices_dw0 +=2;
            psorted_indices_dw1 +=2;
            pcircbuffer_dw0 +=2;
            pcircbuffer_dw1 +=2;
        }

         /* We need to take care of the case where the new_sample is greater than or
         * equal to the prev_val. This is condition 3 in the above loop which hasn't 
         * been applied to the new_indx. */
        new_indx = new_indx0 + new_indx1;
        if (prev_val <= new_sample)
        {
            new_indx = new_indx - 1;
        }
        /* Insert the new_sample into the circbuffer position. These two lines also 
         * expel the prev_val */
        sorted_indices[circbuffer_position] = new_indx;
        circbuffer[circbuffer_position] = new_sample;

        /* Update the circbuffer position. No overflow given the loop bounds */
        circbuffer_position = (circbuffer_position + 1);

    }/* for loop curr_indx from 0 to (WINDOW_SIZE/2) */

    /* circbuffer_position is WINDOW_SIZE exiting the loop, round it to 0 */
    circbuffer_position = 0;

    /* 3. The CFAR-OS routine. */ 
    /* CUT will be from 0 to curr_indx-(WINDOW_SIZE/2) */
    for (curr_indx = (WINDOW_SIZE/2); curr_indx < (lenCUTend+WINDOW_SIZE/2); curr_indx++) 
    {
        new_sample = data[curr_indx];
        if (curr_indx >= len)
        {
            new_sample = data[curr_indx - len];
        }

        /* Find the current 'cell under test' which is at a WINDOW_SIZE/2 distance from the 
         * circbuffer_position. i.e. in the middle of the window */
        /* 
          #ifdef WINDOW_SIZE_IS_A_POWER_OF_TWO
          ik = (circbuffer_position + (WINDOW_SIZE / 2)) & (WINDOW_SIZE -1);;
          #else */
        ik = (circbuffer_position + (WINDOW_SIZE / 2)) ;
        if (ik >= WINDOW_SIZE)
        {
            ik = ik - WINDOW_SIZE;
        }

        cell_under_test_val = circbuffer[ik];
        cell_under_test_pos = sorted_indices[ik];

        /* Find the cell that is to be evicted and then filled with the new_sample. */
        prev_val = circbuffer[circbuffer_position];

        /* sorted_indices contains the sorted indices of circ_buffer which includes 
         * the 'cell under test'. Since the 'cell under test' is to be ignored when 
         * finding the k^th statistic, we need to modify 'k' based on the sorted 
         * position of the 'cell under test' in the sorted array. This position
         * has to be updated based on the new_sample by considering whether the 
         * new_sample is greater (no change in location) or lesser (position lowered 
         * by 1). 
         * 
         * if the 'cell under test' is smaller than the kth statistic, i.e. 
         * k >= cell_under_test_pos, we increase k by 1. */
        if (cell_under_test_val > new_sample)
        {
            cell_under_test_pos++;
        }

#if 0   /*note: when CUT is already small, not the peak for sure, no need to do k+1 anymore*/
        /*if (k >= cell_under_test_pos) {
            k_actual_w = k_plus_1_w;
        }
        else */
#endif
        {
            k_actual_w = k_w;
        }

        /* Initialize the location of the new sample to the 1st position. However
         * The central computation involves an increment if the (new_sample >=
         * circbuffer[i]). However, the actual comparison performed by the DSP is
         * (circbuffer[i] > new_sample). To compute a not we do
         * 1-(circbuffer[i] > new_sample). Since the 1 is a common computation, it
         * is done outslde the loop. */
        new_indx0 = 1 + 4*(WINDOW_SIZE/8);
        new_indx1 = 4*(WINDOW_SIZE/8);

        /* Initialise the kth statistic to be 0 */
        kth_statistic0 = 0;
        kth_statistic1 = 0;

        /* Construct the 32 bit numbers for new_sample, prev_val, and k_actual */
        new_sample_w = new_sample | (new_sample<<16U);
        prev_val_w   = prev_val   | (prev_val<<16U);

        /* This single loop should accomplish multiple objectives.
        * a. Insert new_sample at circbuffer_position. Based on its position in the
        *    sorted circbuffer, the other elements in the sorted_indices should be
        *    incremented (if they are greater ) or left as is (if they are smaller).
        *
        * b. Compute the index of the new sample in the sorted array. Increment
        *    the index if the new_sample is greater than (or equal to) the other
        *     elements of the circbuffer.
        *
        * c. Compute the k^th statistic. The k^th statistic is the value circbuffer[i]
        *    where sorted_indices[i] = k_actual. This is accomplished before step 4
        *    because, we are considering a composite buffer of size WINDOW_SIZE + 1. 
        *    which includes the new_sample as well as the prev_sample. Step 4, removes
        *    prev_sample and hence computing the kth statistic has to be done before. 
        *
        * d. Remove the element at circbuffer_position. Based on its position in the
        *    sorted circbuffer, the other elements in the sorted_indices should be
        *    decremented (if they are greater) or left as is (if they are
        *    smaller).
        */

        pcircbuffer_dw0 = (uint64_t * ) circbuffer;
        pcircbuffer_dw1 = (uint64_t * ) &circbuffer[4];
        psorted_indices_dw0 = (uint64_t * ) sorted_indices;
        psorted_indices_dw1 = (uint64_t * ) &sorted_indices[4];
        for (i = 0; i < WINDOW_SIZE; i+=8) 
        {
            circbuffer_dw0 = *pcircbuffer_dw0;
            circbuffer_dw1 = *pcircbuffer_dw1;

            /* 
            if (new_sample >= circbuffer[i]) {
                new_indx++;
            }    else    {
                increment_val = 1;
            }
            */
            circbuffer_hi0 = _hill(circbuffer_dw0);
            circbuffer_lo0 = _loll(circbuffer_dw0);

            circbuffer_hi1 = _hill(circbuffer_dw1);
            circbuffer_lo1 = _loll(circbuffer_dw1);

            compare_result_hi0 = _cmpgt2(circbuffer_hi0,new_sample_w);
            compare_result_lo0 = _cmpgt2(circbuffer_lo0,new_sample_w);

            compare_result_hi1 = _cmpgt2(circbuffer_hi1,new_sample_w);
            compare_result_lo1 = _cmpgt2(circbuffer_lo1,new_sample_w);

            new_indx0 = new_indx0 - (_bitc4(compare_result_lo0 + (compare_result_hi0<<2))) ;
            new_indx1 = new_indx1 - (_bitc4(compare_result_lo1 + (compare_result_hi1<<2)));

            increment_val_hi0 = _deal(compare_result_hi0);
            increment_val_lo0 = _deal(compare_result_lo0);

            increment_val_hi1 = _deal(compare_result_hi1);
            increment_val_lo1 = _deal(compare_result_lo1);

            /* 
            sorted_indices[i] += increment_val; 
            */
            sorted_indices_dw0 = *psorted_indices_dw0;
            sorted_indices_dw1 = *psorted_indices_dw1;

            sorted_indices_hi0 = _hill(sorted_indices_dw0);
            sorted_indices_lo0 = _loll(sorted_indices_dw0);

            sorted_indices_hi1 = _hill(sorted_indices_dw1);
            sorted_indices_lo1 = _loll(sorted_indices_dw1);

            sorted_indices_hi0 = _add2(sorted_indices_hi0,increment_val_hi0);
            sorted_indices_lo0 = _add2(sorted_indices_lo0,increment_val_lo0);

            sorted_indices_hi1 = _add2(sorted_indices_hi1,increment_val_hi1);
            sorted_indices_lo1 = _add2(sorted_indices_lo1,increment_val_lo1);

            /*
            if (sorted_indices[i] == k_actual) {
                kth_statistic = circbuffer[i];
            }
            */
            compare_result_hi0 = _cmpeq2(sorted_indices_hi0,k_actual_w);
            compare_result_lo0 = _cmpeq2(sorted_indices_lo0,k_actual_w);

            compare_result_hi1 = _cmpeq2(sorted_indices_hi1,k_actual_w);
            compare_result_lo1 = _cmpeq2(sorted_indices_lo1,k_actual_w);

            compare_result_hi0 = _deal(compare_result_hi0);
            compare_result_lo0 = _deal(compare_result_lo0);

            compare_result_hi1 = _deal(compare_result_hi1);
            compare_result_lo1 = _deal(compare_result_lo1);

            kth_statistic0 = kth_statistic0 | _dotp2(circbuffer_hi0, compare_result_hi0);
            kth_statistic0 = kth_statistic0 | _dotp2(circbuffer_lo0, compare_result_lo0);

            kth_statistic1 = kth_statistic1 | _dotp2(circbuffer_hi1, compare_result_hi1);
            kth_statistic1 = kth_statistic1 | _dotp2(circbuffer_lo1, compare_result_lo1);

            /* 
            increment_val = 0;
            if (prev_val <= circbuffer[i]) {
                increment_val = -1;
            }    */

            compare_result_hi0 = 3U - _cmpgt2(prev_val_w,circbuffer_hi0);
            compare_result_lo0 = 3U - _cmpgt2(prev_val_w,circbuffer_lo0);

            compare_result_hi1 = 3U - _cmpgt2(prev_val_w,circbuffer_hi1);
            compare_result_lo1 = 3U - _cmpgt2(prev_val_w,circbuffer_lo1);

            /*
            sorted_indices[i] += increment_val;
            */
            sorted_indices_hi0 = _sub2(sorted_indices_hi0,_deal(compare_result_hi0));
            sorted_indices_lo0 = _sub2(sorted_indices_lo0,_deal(compare_result_lo0));

            sorted_indices_hi1 = _sub2(sorted_indices_hi1,_deal(compare_result_hi1));
            sorted_indices_lo1 = _sub2(sorted_indices_lo1,_deal(compare_result_lo1));

            *psorted_indices_dw0 = _itoll(sorted_indices_hi0,sorted_indices_lo0);
            *psorted_indices_dw1 = _itoll(sorted_indices_hi1,sorted_indices_lo1);

            psorted_indices_dw0 +=2;
            psorted_indices_dw1 +=2;
            pcircbuffer_dw0 +=2;
            pcircbuffer_dw1 +=2;
        }/*for i=0 to WINDOW_SIZE*/
        /* There can be only one kth_statistic by design, so ORring the two kth_statistic 
            * should only select one valid kth_statistic. */
        kth_statistic = kth_statistic0 | kth_statistic1;

        /* Compute the index of the new_sample */
        new_indx = new_indx0 + new_indx1;

        /* Check if the new sample is actually the kth statistic*/
        if (_cmpeq2(new_indx, (int32_t)k_actual_w))
        {
            kth_statistic = new_sample;
        }

        /* We need to take care of the case where the new_sample is greater than or
        * equal to the prev_val. This is condition 3 in the above loop which hasn't
        * been applied to the new_indx. */
        if (prev_val <= new_sample)
        {
            new_indx = new_indx - 1;
        }

        /* Insert the new_sample into the circbuffer position. These two lines also
        * expel the prev_val */
        sorted_indices[circbuffer_position] = new_indx;
        circbuffer[circbuffer_position] = new_sample;

        /* Update the circbuffer position.*/
        circbuffer_position = (circbuffer_position + 1);
        if (circbuffer_position == WINDOW_SIZE)
        {
            circbuffer_position = 0;
        }

        /* detector. */
        if ( (cell_under_test_val << N_FRAC_BITS_ALPHA) > ((uint32_t)kth_statistic*alpha)) 
        {
            detectedList[n_detected_obj] = curr_indx - (WINDOW_SIZE/2);
            kosList[n_detected_obj]      = kth_statistic;
            n_detected_obj = n_detected_obj + 1;
        }

    } /*for curr idx from WINDOW_SIZE/2 to (lenCUTend+WINDOW_SIZE/2), CUT from 0 to lenCUTend */

    return n_detected_obj;

}/* mmwavelib_cfarOS */


/* For reference, C Natural function corresponding to the optimized function is kept here
   to assist understanding of the implementation. */
uint16_t mmwavelib_cfarOS_cn(const uint16_t * restrict data,
                        const uint32_t len,
                        const uint32_t lenCUTend,
                        const int16_t k,
                        const int16_t alpha,
                        uint16_t * restrict detected_list,
                        uint16_t * restrict k_os_list)
{

    int16_t sorted_indices[WINDOW_SIZE];
    uint16_t circbuffer[WINDOW_SIZE];
    uint16_t kth_statistic =0;
    uint32_t curr_indx = 0;
    uint16_t n_detected_obj = 0;
    int32_t circbuffer_position;
    int32_t ik;
    uint16_t cell_under_test_val;
    int32_t  cell_under_test_pos;
    uint16_t prev_val;
    uint16_t new_sample, k_actual;
    int16_t  new_indx, i;
    int16_t  increment_val;


    /* 1. Initialize the circbuffer with data[0] for all the cells. We do this because the 
     * buffer is uninitialized and we would like to initialize the window with some 
     * reasonable parameters. 
    *
    * Since all the entries in the initialized circbuffer are the same, we can directly
     * populate the sorted indices of this array as ((WINDOW_SIZE/2)+1) to (WINDOW_SIZE).
     */
    for (curr_indx = 0;    curr_indx < (WINDOW_SIZE/2U); curr_indx++)
    {
        circbuffer[curr_indx] = data[0];
        circbuffer[curr_indx+(WINDOW_SIZE/2U)] = data[0];
        sorted_indices[curr_indx] = curr_indx + (WINDOW_SIZE/2) + 1;
    }

    circbuffer_position = (WINDOW_SIZE / 2);

    /* 2. Insert the first WINDOW_SIZE/2 samples into the circbuffer while
     *    simultaneously sorting them. */
    for ( curr_indx = 0; curr_indx < (WINDOW_SIZE/2);    curr_indx++) 
    {
        new_sample = data[curr_indx];
        prev_val = circbuffer[circbuffer_position];

        new_indx = 1;
        
        /* The simple comparator loop. */
        for (i = 0; i < WINDOW_SIZE; i++) 
        {
            increment_val = 0;
            if (new_sample >= circbuffer[i])
            {
                new_indx++;
            }
            else 
            {
                increment_val = 1;
            }

            if (prev_val <= circbuffer[i])
            {
                increment_val--;
            }
            
            sorted_indices[i] += increment_val;
        }

        new_indx = new_indx - (prev_val <= new_sample);

        /* Insert the new_sample into the circbuffer position. These two lines also 
         * expel the prev_val */
        sorted_indices[circbuffer_position] = new_indx;
        circbuffer[circbuffer_position] = new_sample;

        /* Update the circbuffer position.*/
        circbuffer_position = (circbuffer_position + 1);
        if (circbuffer_position >= WINDOW_SIZE) 
        {
            circbuffer_position = circbuffer_position - WINDOW_SIZE;
        }
    }

    /* 3. The CFAR-OS routine . */
    for (; /* curr_indx is already WINDOW_SIZE/2. */
        curr_indx < (lenCUTend + WINDOW_SIZE/2) ;
        curr_indx++) 
    {
        new_sample = data[curr_indx];
        if (curr_indx >= len)
        {
            new_sample = data[curr_indx - len];
        }

        /* Find the current 'cell under test' which is at a WINDOW_SIZE/2 distance from the 
         * circbuffer_position. i.e. in the middle of the window */
        ik = (circbuffer_position + (WINDOW_SIZE / 2)) ;
        if (ik >= WINDOW_SIZE) {
            ik = ik - WINDOW_SIZE;
        }
        cell_under_test_val = circbuffer[ik];
        cell_under_test_pos = sorted_indices[ik];

        /* Find the cell that is to be evicted and then filled with the new_sample. */
        prev_val = circbuffer[circbuffer_position];

        if (cell_under_test_val > new_sample) {
            cell_under_test_pos++;
        }

#if 0/*note: when CUT is already small, not the peak for sure, no need to do k+1 anymore*/
        if (k >= cell_under_test_pos) {
            k_actual = k + 1;        }
        else
#endif
        {
            k_actual = k;
        }

        /* Initialise the location of the new sample to the 1st position. */
        new_indx = 1;
        
        for (i = 0; i < WINDOW_SIZE; i++) 
        {
            increment_val = 0;

            if (new_sample >= circbuffer[i]) {
                new_indx++;
            }    else    {
                increment_val = 1;
            }

            sorted_indices[i] += increment_val;

            if (sorted_indices[i] == k_actual) {
                kth_statistic = circbuffer[i];
            }
            increment_val = 0;
            if (prev_val <= circbuffer[i]) {
                increment_val = -1;
            }

            sorted_indices[i] += increment_val;

        }

        /* Check if the new sample is actually the kth statistic*/
        if (new_indx == k_actual) 
        {
            kth_statistic = new_sample;
        }

        /* We need to take care of the case where the new_sample is greater than or
        * equal to the prev_val. This is condition 3 in the above loop which hasn't
        * been applied to the new_indx. */
        new_indx = new_indx - (prev_val <= new_sample);

        /* Insert the new_sample into the circbuffer position. These two lines also
        * expel the prev_val */
        sorted_indices[circbuffer_position] = new_indx;
        circbuffer[circbuffer_position] = new_sample;

        /* Update the circbuffer position.*/
        circbuffer_position = (circbuffer_position + 1); 
        if (circbuffer_position >= WINDOW_SIZE) 
        {
            circbuffer_position = circbuffer_position - WINDOW_SIZE;
        }

        /* Detector. */
        if ( (cell_under_test_val << N_FRAC_BITS_ALPHA) > (kth_statistic*alpha) ) 
        {
            detected_list[n_detected_obj] = curr_indx - (WINDOW_SIZE/2);
            k_os_list[n_detected_obj]     = kth_statistic;
            n_detected_obj = n_detected_obj + 1;
        }

    } /*for curr idx from WINDOW_SIZE/2 to (lenCUTend+WINDOW_SIZE/2), CUT from 0 to lenCUTend */

    return n_detected_obj;

} /* C natural function mmwavelib_cfarOS_cn*/
