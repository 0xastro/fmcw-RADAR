/**
 *   @file  staticclutterproc_internal.h
 *
 *   @brief
 *       staticclutterproc DPU internal include file
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

/** @mainpage Static Clutter Removal DPU
 *
 * Static Clutter Removal DPU
 *
 * This DPU implements static clutter removal in the following way:\n
 * For each range bin, per each antenna, the mean value of the samples is calculated and subtracted from the samples.\n
 * The subtraction operation saturates if needed (no overflow).\n
 *
 * The static clutter removal implementation is expensive if performed directly on L3 memory space due
 * to slow memory access from R4F and/or DSP cores. Therefore, the DPU will use EDMA to bring data from
 * radar cube (which supposedly is allocated from L3) into a scratch buffer provided by application
 * @ref DPU_StaticClutterProc_ScratchBuf. The scratch buffer should be allocated from internal memory
 * (TCMB in case of R4F and L1/L2 SRAM in case of DSP).\n
 *
 * Then static clutter removal is performed on the scratch buffer and data is moved back by EDMA from scratch buffer to radarCube.\n\n
 * 
 * The following figure shows a high level block diagram of the DPU implementation.\n
 *
 * @image html clutter_removal_toplevel.png "Static Clutter Removal High level block diagram"
 *
 * The algorithm is executed in a ping/pong fashion to parallelize the EDMA transfer and the algorithm computation.
 * All hardware resources (EDMA properties), buffers and static configuration are provided by application through
 * @ref DPU_StaticClutterProc_config.\n
 *
 * This DPU expects as input the radar cube with 1D FFT data as described in @ref DPIF_RADARCUBE_FORMAT_1.
 * This is the only supported format. \n
 *
 * The following figure depicts in some detail the DPU implementation for the case of 3 TX and 4RX antennas.\n
 *
 * @image html clutter_removal.png "Static Clutter Removal DPU implementation"
 *
 *
 * DPU initialization is done through @ref DPU_StaticClutterProc_init.\n
 *
 * DPU configuration is done by @ref DPU_StaticClutterProc_config. The configuration can only be done after
 * the DPU has been initialized using @ref DPU_StaticClutterProc_init.\n
 *
 * DPU processing is done by @ref DPU_StaticClutterProc_process. This will execute the static clutter removal
 * algorithm over the full radar cube. This processing can only be done after the DPU has been configured through
 * @ref DPU_StaticClutterProc_config. If the parameters used by the @ref DPU_StaticClutterProc_config do not
 * change from one frame to the next, @ref DPU_StaticClutterProc_config can be called only once, for the first
 * frame and for every frame @ref DPU_StaticClutterProc_process can be executed without the need of reconfiguring the DPU.
 *
 */

/** @defgroup STATICCLUTTER_PROC_DPU_INTERNAL       staticClutterProc DPU Internal
 */

/**
@defgroup DPU_STATICCLUTTERPROC_INTERNAL_FUNCTION            staticclutterproc DPU Internal Functions
@ingroup STATICCLUTTER_PROC_DPU_INTERNAL
@brief
*   The section has a list of all internal API which are not exposed to the external
*   applications.
*/
/**
@defgroup DPU_STATICCLUTTERPROC_INTERNAL_DATA_STRUCTURE      staticclutterproc DPU Internal Data Structures
@ingroup STATICCLUTTER_PROC_DPU_INTERNAL
@brief
*   The section has a list of all internal data structures which are used internally
*   by the staticclutterproc DPU module.
*/
/**
@defgroup DPU_STATICCLUTTERPROC_INTERNAL_DEFINITION          staticclutterproc DPU Internal Definitions
@ingroup STATICCLUTTER_PROC_DPU_INTERNAL
@brief
*   The section has a list of all internal definitions which are used internally
*   by the staticclutterproc DPU.
*/

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#ifndef STATICCLUTTERPROC_INTERNAL_H
#define STATICCLUTTERPROC_INTERNAL_H

/* Data Path include files */
#include <ti/datapath/dpc/dpu/staticclutterproc/staticclutterproc.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup DPU_STATICCLUTTERPROC_INTERNAL_DEFINITION
 *  HWA memory bank indices for various input/output operations
 *
@{ */

/*! Ping index used for EDMA-CPU processing parallelism */
#define DPU_STATICCLUTTERPROC_PING_IDX 0

/*! Pong index used for EDMA-CPU processing parallelism */
#define DPU_STATICCLUTTERPROC_PONG_IDX 1

/** @} */

/**
 * @brief
 *  staticclutterproc DPU internal data Object
 *
 * @details
 *  The structure is used to hold staticclutterproc internal data object
 *
 *  \ingroup DPU_STATICCLUTTERPROC_INTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_StaticClutterProc_Obj_t
{
    /*! @brief  Configuration parameters. */
    DPU_StaticClutterProc_Config cfg;

    /*! @brief  Size (in samples) of Ping (or Pong) scratch buffer.\n 
        Size is equal to:\n
        For R4F: numDopplerChirps\n
        For DSP (C674X): The smallest multiple of 4 greater or equal to numDopplerChirps\n*/
    uint16_t sizePingBufSamples;
    
}DPU_StaticClutterProc_Obj;


#ifdef __cplusplus
}
#endif

#endif
