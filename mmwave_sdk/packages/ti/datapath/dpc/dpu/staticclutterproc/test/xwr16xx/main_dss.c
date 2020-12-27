/**
 *   @file  main_dss.c
 *
 *   @brief
 *      Unit Test code for static clutter removal.
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
 
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* BIOS/XDC Include Files. */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Event.h>
#define DebugP_ASSERT_ENABLED 1
#include <ti/drivers/osal/DebugP.h>
#include <assert.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/osal/SemaphoreP.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/esm/esm.h>
#include <ti/drivers/soc/soc.h>

#include <ti/utils/testlogger/logger.h>

#include <ti/utils/cycleprofiler/cycle_profiler.h> 

#include <ti/datapath/dpc/dpu/staticclutterproc/staticclutterproc.h>
#include "../common/test_common.h"
 
 /* EDMA resource partitioning */
#define EDMA_SHADOW_LNK_PARAM_BASE_ID       EDMA_NUM_DMA_CHANNELS
/* clutter removal EDMA channels */
#define MMW_EDMA_CLUTTER_IN_PING     EDMA_TPCC0_REQ_FREE_14
#define MMW_EDMA_CLUTTER_IN_PONG     EDMA_TPCC0_REQ_FREE_15
#define MMW_EDMA_CLUTTER_OUT_PING    EDMA_TPCC0_REQ_FREE_16
#define MMW_EDMA_CLUTTER_OUT_PONG    EDMA_TPCC0_REQ_FREE_17

#define MMW_EDMA_CLUTTER_IN_PING_SHADOW     (EDMA_SHADOW_LNK_PARAM_BASE_ID + 21U)
#define MMW_EDMA_CLUTTER_IN_PONG_SHADOW     (EDMA_SHADOW_LNK_PARAM_BASE_ID + 22U)
#define MMW_EDMA_CLUTTER_OUT_PING_SHADOW    (EDMA_SHADOW_LNK_PARAM_BASE_ID + 23U)
#define MMW_EDMA_CLUTTER_OUT_PONG_SHADOW    (EDMA_SHADOW_LNK_PARAM_BASE_ID + 24U)

/*! Scratch buffer size in samples.
    Scratch buffer max size = (max number of Doppler chirps)*2[ping/pong]*sizeof(complex_16) */
#define TEST_STATICCLUTTERPROC_SCRATCH_SIZE 256*2*4 


#pragma DATA_SECTION(scratchBuf, ".l2data");
#pragma DATA_ALIGN(scratchBuf, DPU_STATICCLUTTERPROC_SCRATCHBUFFER_BYTE_ALIGNMENT_DSP)

//#pragma DATA_SECTION(scratchBuf, ".l1data");
/* Allocate space for Ping and Pong scratch buffers*/
uint8_t scratchBuf[TEST_STATICCLUTTERPROC_SCRATCH_SIZE];

#define L3usable   SOC_XWR16XX_DSS_L3RAM_SIZE

/*! L3 RAM buffer */
uint8_t gMmwL3[L3usable];
#pragma DATA_SECTION(gMmwL3, ".l3ram");

#define TEST_RADAR_CUBE_SIZE (L3usable - 4*1024)/2 


/* Definition of test scenarios. Depending on the available size for the 
   radar cube for the specific platform, more scenarios can be added 
   with larger sizes for # Doppler and Range bins.*/
#define TEST_NUM_OF_SCENARIOS 13
testConfiguration testConfigArray[TEST_NUM_OF_SCENARIOS] = 
{
    /*TX ant  RX ant  Dop chirp  Ran bin*/
    3,         4,       16,      128,
    3,         4,       128,     16,
    3,         1,       256,     32,
    3,         1,       16,      256,
    2,         4,       32,      64,
    2,         4,       16,      128,
    2,         2,       32,      128,
    2,         1,       64,      64,    
    1,         1,       256,     64,
    1,         1,       32,      256,
    /* Non power of 2 number of doppler chirps*/
    3,         4,       130,     16,
    2,         2,       34,      128,
    1,         1,       250,     64
};

void Test_task(UArg arg0, UArg arg1)
{
    DPU_StaticClutterProc_Config      initialCfg;
    DPIF_RadarCube                    cube;
    DPU_StaticClutterProc_ScratchBuf  scratch;

    
    memset((void*)&initialCfg, 0, sizeof(DPU_StaticClutterProc_Config));
    
    /* Setup EDMA channels.  These are fixed for all scenarios in this test,
    but may vary per platform.*/       

    /* Input EDMA configuration */
    initialCfg.hwRes.edmaIn.ping.channel       = MMW_EDMA_CLUTTER_IN_PING;
    initialCfg.hwRes.edmaIn.ping.channelShadow = MMW_EDMA_CLUTTER_IN_PING_SHADOW;
    initialCfg.hwRes.edmaIn.ping.eventQueue    = 0;
    initialCfg.hwRes.edmaIn.pong.channel       = MMW_EDMA_CLUTTER_IN_PONG;
    initialCfg.hwRes.edmaIn.pong.channelShadow = MMW_EDMA_CLUTTER_IN_PONG_SHADOW;
    initialCfg.hwRes.edmaIn.pong.eventQueue    = 0;
    
    /* Output EDMA configuration */
    initialCfg.hwRes.edmaOut.ping.channel       = MMW_EDMA_CLUTTER_OUT_PING;
    initialCfg.hwRes.edmaOut.ping.channelShadow = MMW_EDMA_CLUTTER_OUT_PING_SHADOW;
    initialCfg.hwRes.edmaOut.ping.eventQueue    = 0;
    initialCfg.hwRes.edmaOut.pong.channel       = MMW_EDMA_CLUTTER_OUT_PONG;
    initialCfg.hwRes.edmaOut.pong.channelShadow = MMW_EDMA_CLUTTER_OUT_PONG_SHADOW;   
    initialCfg.hwRes.edmaOut.pong.eventQueue    = 0;

    /* radar cube */
    cube.datafmt                = DPIF_RADARCUBE_FORMAT_1;
    cube.data                   = (void*)&gMmwL3[TEST_RADAR_CUBE_SIZE];   
    initialCfg.hwRes.radarCube  = cube;
    
    /* scratch buffer */
    scratch.bufSize             = TEST_STATICCLUTTERPROC_SCRATCH_SIZE;
    scratch.buf                 = (void*)&scratchBuf[0];
    initialCfg.hwRes.scratchBuf = scratch;

    /* Fill up scratch buffer before start of tests */
    memset((void*)&scratchBuf[0], 0xAB, TEST_STATICCLUTTERPROC_SCRATCH_SIZE);
    
    /* Note that in this test we need to saturate (not overflow) the clutter removal 
    computation to match the mmwavelib implementation where
    the subtraction step is saturated if it goes beyond the int16_t range*/
    Test_main(TEST_NUM_OF_SCENARIOS, 
              testConfigArray,
              &initialCfg,
              0, /*EDMA instance ID*/
              (uint8_t*)&gMmwL3[0],/*Reference cube*/
              (uint8_t*)&gMmwL3[TEST_RADAR_CUBE_SIZE], /*calculated/tested cube*/
              TEST_RADAR_CUBE_SIZE /*cube size*/
              );
}

/**
 *  @b Description
 *  @n
 *      Entry point into the test code.
 *
 *  @retval
 *      Not Applicable.
 */
int main (void)
{
    Task_Params taskParams;
    int32_t     errCode;
    SOC_Cfg     socCfg;
    SOC_Handle  socHandle;

    /* Initialize test logger */
    MCPI_Initialize();

    Cycleprofiler_init();

    /* Initialize the SOC confiugration: */
    memset ((void *)&socCfg, 0, sizeof(SOC_Cfg));

    /* Populate the SOC configuration: */
    socCfg.clockCfg = SOC_SysClock_INIT;

    /* Initialize the SOC Module: This is done as soon as the application is started
     * to ensure that the MPU is correctly configured. */
    socHandle = SOC_init (&socCfg, &errCode);
    if (socHandle == NULL)
    {
        printf ("Error: SOC Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 8*1024;
    Task_create(Test_task, &taskParams, NULL);

    /* Start BIOS */
    BIOS_start();
    
    return 0;
}

