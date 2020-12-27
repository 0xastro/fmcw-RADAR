/**
 *   @file  dopplerprochwainternal.h
 *
 *   @brief
 *      Implements Data path doppler processing functionality.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2018 Texas Instruments, Inc.
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
#ifndef DOPPLERPROC_HWA_INTERNAL_H
#define DOPPLERPROC_HWA_INTERNAL_H

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* mmWave SDK Driver/Common Include Files */
#include <ti/drivers/hwa/hwa.h>
#include <ti/utils/cycleprofiler/cycle_profiler.h>

/* DPIF Components Include Files */
#include <ti/datapath/dpif/dpif_detmatrix.h>
#include <ti/datapath/dpif/dpif_radarcube.h>

/* mmWave SDK Data Path Include Files */
#include <ti/datapath/dpif/dp_error.h>
#include <ti/datapath/dpc/dpu/dopplerproc/dopplerprochwa.h>

/** @mainpage Doppler DPU
 *
 * This DPU implements the Doppler FFT (2D FFT) using HWA.\n
 * This DPU expects as input the radar cube with 1D FFT data as described in @ref DPIF_RADARCUBE_FORMAT_1. 
 * The output of this DPU is a detection matrix of format described in @ref DPIF_DETMATRIX_FORMAT_1. 
 * These are the only formats supported by this DPU. Data from radar cube is moved into HWA memory using EDMA,
 * HWA performs the required computations and data is moved by EDMA from HWA internal memory to the detection matrix.\n\n
 * 
 * The following figure shows a high level block diagram of the DPU implementation.\n
 *
 * @image html hwa_doppler_toplevel.png "HWA based Doppler DPU High Level block diagram"
 *
 * A list of resources required by this DPU is described in @ref DPU_DopplerProcHWA_HW_Resources_t.
 * In particular, the number of EDMA channels required is constant and does not depend on the data path configuration. 
 * On the other hand, the number of required HWA paramsets is a function of the number of TX antennas configured in
 * the data path as described in @ref DPU_DopplerProcHWA_HwaCfg_t.\n
 * 
 * Besides the resources described above, other parameters required for the DPU configuration are listed
 * in @ref DPU_DopplerProcHWA_StaticConfig_t.
 * In particular, the DPU takes as input the number of Doppler chirps (numDopplerChirps), which does not
 * need to be a power of two. It produces a detection matrix of Doppler dimension equal to numDopplerBins
 * which must be a power of 2 greater or equal than numDopplerChirps.
 * Another DPU input is the number of range bins, which must be even but does not need to be a power of two.\n\n
 * 
 * Below are details of the DPU implementation:\n
 * 
 * EDMA is used to move data from the radar cube into the HWA internal memory and after processing is done
 * to move data from HWA internal memory to the detection matrix.
 * All 4 HWA memory banks are required by this DPU, regardless of the DPU configuration. In this document, the
 * HWA memory banks are called M0, M1, M2, M3.\n 
 * 
 * A ping/pong buffer scheme is used where:\n
 * 
 * M0 and M2 are used for ping input/processing.\n 
 * M1 and M3 are used for pong input/processing.\n
 * 
 * In each iteration (ping/pong), a column of the radar cube matrix is brought into HWA for processing. 
 * Such column consists of all samples for a fixed range, that is, all received virtual antennas for all
 * Doppler chirps for a fixed range bin.\n
 * 
 * The steps below are executed by HWA on data (one column of the radar cube as described above) sitting 
 * in its M memory. The description below is given for processing the ping buffer. 
 * Pong buffer processing is identical to ping, except that M0 memory is replaced by M1 and M2 memory is replaced by M3.\n\n
 * 
 * **Windowing**\n
 * Before FFT operation, input samples are multiplied by a window function. Window size and coefficients are 
 * defined in @ref DPU_DopplerProcHWA_HwaCfg_t.\n
 * Window coefficients must be provided by application.\n\n
 * 
 * **FFT and Log2|.|**\n
 * In this step 2D FFT and Log2 of the absolute value of each sample is computed. Input samples are in M0
 * and output samples are in M2. Input sample is of type cmplx16ImRe_t and output is of type uint16_t.
 * This step also converts the number of input samples from N = numDopplerChirps 
 * to a number of output samples K = numDopplerBins, which is the size of the Doppler FFT.
 * Both numDopplerBins and numDopplerChirps are input parameters for this DPU and numDopplerBins must be a power
 * of 2 greater or equal than numDopplerChirps. \n\n 
 * 
 * **Summation**\n
 * Summation of all virtual antennas is computed for each Doppler bin. \n
 * The output of the previous step (HWA Log2 magnitude) is in Q11 format.
 * The sum is done using FFT in HWA, the sum is obtained in the DC (0th) bin. 
 * Input of the summation is on M2 and output is on M0. This FFT programming has srcScale of 2, meaning 2
 * redundant bits (sign extension, in this case unsigned) are added to MSB and 6 LSBs are padded with 0, 
 * so input before computation is in Q[11 + 6] format. The dstScale is set to 8, so summation output will
 * have 8-bits dropped, giving a result in Q[11 + 6 - 8] = Q9 format. The FFT size is the next power of 2
 * of number of virtual antennas and the FFT is programmed to enable all butterfly scaling stages, hence the
 * FFT output will be 1/A * sum(.), where A = 2^Ceil(Log2(numVirtualAntennas)). The true average is
 * 1/numVirtualAntennas * sum(.). If numVirtualAntennas is a power of 2, A = numVirtualAntennas. 
 * Otherwise, a correction factor needs to be applied to every element of the detection matrix. 
 * This correction can be done by adjusting the CFAR threshold to compensate for this mismatch.
 * Once the summation is computed for all Doppler bins, EDMA transfers the result from M0 to the detection matrix.\n\n
 * 
 * **HWA memory bank size limitation**\n
 * The following 2 conditions must be satisfied for the parameters in this DPU configuration:\n
 * 4 x numRxAntennas x numTxAntennas x numDopplerChirps <= 16384\n
 * 2 x numRxAntennas x numTxAntennas x numDopplerBins <= 16384\n\n
 *
 * The reason for this limitation is as follows:\n
 * The size of the data that is brought in for processing in HWA (per ping/pong iteration) is a column of the radar cube.
 * The size of radar cube column is\n 
 * 4 x numRxAntennas x  numTxAntennas x numDopplerChirps (I),\n
 * where 4 bytes is sizeof(cmplx16ImRe_t), the sample size.\n
 * 
 * After the 2D FFT and Log2|.| , we go from Doppler chirps to Doppler bins and from cmplx16ImRe_t to uint16_t. \n
 * 
 * Therefore the size after 2D FFT and Log2|.| is\n 
 * 2 x numRxAntennas x numTxAntennas x numDopplerBins  (II),\n
 * where 2 bytes is sizeof(uint16_t).\n\n
 * 
 * Both quantities above (I and II) should fit (independently) in one of the HWA M memory partitions 
 * which has a size of 16KB.\n\n
 * 
 * **Exported APIs**\n
 * DPU initialization is done through @ref DPU_DopplerProcHWA_init.\n\n
 * 
 * DPU configuration is done by @ref DPU_DopplerProcHWA_config. The configuration can only be done after
 * the DPU has been initialized. The configuration parameters are described in @ref DPU_DopplerProcHWA_Config. \n\n
 *
 * The DPU is executed by calling @ref DPU_DopplerProcHWA_process. \n
 * This will trigger the first ping/pong EDMA transfers and from there on, the processing of the Doppler DPU for the
 * full radar cube is driven by hardware: EDMA will move data in and trigger HWA, which will process the data and 
 * trigger EDMA to move data out (to detection matrix) and trigger next EDMA to move data in – and so on. All columns of the radar cube
 * matrix will be processed in this loop and no CPU intervention is needed. When HWA finishes processing all columns of the
 * radar cube it will generate an interrupt. When the last EDMA transfer to the detection matrix has landed, EDMA will generate
 * an interrupt. The DPU processing is done when both interrupts are received.
 * Both interrupts are depicted in green boxes in the figure below.\n\n
 * 
 * 
 * **Detailed block diagram for 3 TX 4 RX**\n
 * 
 * The following figure depicts in detail the DPU implementation for the case of 3 TX and 4RX antennas.
 * The blue boxes connected by blue arrows represent different HWA paramsets.\n\n
 * 
 * @image html hwa_doppler_fft.png "Doppler DPU implementation for 3 TX and 4 RX antennas"
 * 
 * 
 * 
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 *  dopplerProc DPU internal data Object
 *
 * @details
 *  The structure is used to hold dopplerProc internal data object
 *
 *  \ingroup DPU_DOPPLERPROCHWA_INTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_DopplerProcHWA_Obj_t
{
    /*!< @brief HWA Handle */
    HWA_Handle  hwaHandle;
    
    /*! @brief  EDMA driver handle. */
    EDMA_Handle edmaHandle;

    /*! @brief  EDMA configuration for Input data (Radar cube -> HWA memory). */
    DPU_DopplerProc_Edma edmaIn;

    /*! @brief HWA Processing Done semaphore Handle */
    SemaphoreP_Handle  hwaDoneSemaHandle;

    /*! @brief EDMA Done semaphore Handle */
    SemaphoreP_Handle  edmaDoneSemaHandle;
    
    /*! @brief Flag to indicate if DPU is in processing state */
    bool inProgress;
          
    /*! @brief  DMA trigger source channel for Ping param set */
    uint8_t hwaDmaTriggerSourcePing;
    
    /*! @brief  DMA trigger source channel for Pong param set */
    uint8_t hwaDmaTriggerSourcePong;      
            
    /*! @brief  HWA number of loops */
    uint16_t hwaNumLoops;
    
    /*! @brief  HWA start paramset index */
    uint8_t  hwaParamStartIdx;
    
    /*! @brief  HWA stop paramset index */
    uint8_t  hwaParamStopIdx;
    
    /*! @brief  HWA memory bank addresses */
    uint32_t hwaMemBankAddr[DPU_DOPPLERPROCHWA_NUM_HWA_MEMBANKS];

}DPU_DopplerProcHWA_Obj;


#ifdef __cplusplus
}
#endif

#endif
