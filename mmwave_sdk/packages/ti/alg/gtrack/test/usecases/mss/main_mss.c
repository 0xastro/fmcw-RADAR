/*
 *   @file  main_mss.c
 *
 *   @brief
 *      Unit Test code for the GTRACK
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016 Texas Instruments, Inc.
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

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

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
#include <ti/sysbios/heaps/HeapBuf.h>
#include <ti/sysbios/heaps/HeapMem.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/family/arm/v7a/Pmu.h>

/* mmWave SK Include Files: */
#include <ti/common/sys_common.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/esm/esm.h>
#include <ti/utils/testlogger/logger.h>

#include <ti/alg/gtrack/gtrack.h>

/**************************************************************************
 ************************** External Definitions **************************
 **************************************************************************/
extern void Test_initTask(UArg arg0, UArg arg1);

extern void asm_matrixMultiply66(float *A, float *B, float*C);
/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

/* external sleep function when in idle (used in .cfg file) */
void wfi_sleep(void);

/**
 *  @b Description
 *  @n
 *     Function to sleep the R4F using WFI (Wait For Interrupt) instruction. 
 *     When R4F has no work left to do,
 *     the BIOS will be in Idle thread and will call this function. The R4F will
 *     wake-up on any interrupt (e.g chirp interrupt).
 *
 *  @retval
 *      Not Applicable.
 */
void wfi_sleep(void)
{
    /* issue WFI (Wait For Interrupt) instruction */
    asm(" WFI ");
}

/**
 *  @b Description
 *  @n
 *      This is the entry point into the unit test code
 *
 *  @retval
 *      Not Applicable.
 */
int32_t main (void)
{
	Task_Params     taskParams;
    int32_t         errCode;
    SOC_Handle      socHandle;
    SOC_Cfg         socCfg;

#if 0
    float A[36] = {
       0.1f, 1.2f, 2.3f, 3.4f, 4.5f, 5.5f,
       0.1f, 1.2f, 2.3f, 3.4f, 4.5f, 5.5f,
       0.1f, 1.2f, 2.3f, 3.4f, 4.5f, 5.5f,
       0.1f, 1.2f, 2.3f, 3.4f, 4.5f, 5.5f,
       0.1f, 1.2f, 2.3f, 3.4f, 4.5f, 5.5f,
       0.1f, 1.2f, 2.3f, 3.4f, 4.5f, 5.5f,
    };
	float B[36] = {
	   5.4f, 4.3f, 3.2f, 2.1f, 1.0f, 0.9f,
       5.4f, 4.3f, 3.2f, 2.1f, 1.0f, 0.9f,
       5.4f, 4.3f, 3.2f, 2.1f, 1.0f, 0.9f,
       5.4f, 4.3f, 3.2f, 2.1f, 1.0f, 0.9f,
       5.4f, 4.3f, 3.2f, 2.1f, 1.0f, 0.9f,
       5.4f, 4.3f, 3.2f, 2.1f, 1.0f, 0.9f,
	};
	float C1[36] = {0};
	float C2[36] = {0};
	
	uint32_t start_bench, asm_bench, c_bench;
#endif

    /* Initialize the ESM: Dont clear errors as TI RTOS does it */
    ESM_init(0U);

    /* Initialize the SOC confiugration: */
    memset ((void *)&socCfg, 0, sizeof(SOC_Cfg));

    /* Populate the SOC configuration: */
    socCfg.clockCfg = SOC_SysClock_INIT;

    /* Initialize the SOC Module: This is done as soon as the application is started
     * to ensure that the MPU is correctly configured. */
    socHandle = SOC_init (&socCfg, &errCode);
    if (socHandle == NULL)
    {
        System_printf ("Error: SOC Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Debug Message: */
	System_printf ("******************************************\n");
	System_printf ("Debug: Launching the GTRACK Test Application\n");
	System_printf ("******************************************\n");
#if 0
	gtrack_matrixPrint(6, 6, A);
    gtrack_matrixPrint(6, 6, B);

    start_bench = gtrack_getCycleCount();
    asm_matrixMultiply66(A,B,C1);
    asm_bench = gtrack_getCycleCount();
    gtrack_matrixMultiply66M(A,B,C2);
    c_bench = gtrack_getCycleCount();

	gtrack_matrixPrint(6, 6, C1);
    gtrack_matrixPrint(6, 6, C2);

    /* Debug Message: */
    System_printf ("Benchmarking [6x6] x [6x6] multiplication, cycles: %u (asm), %u (c)\n", asm_bench- start_bench, c_bench - asm_bench);
#endif


    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 8*1024;
    Task_create(Test_initTask, &taskParams, NULL);

    /* Start BIOS */
	BIOS_start();
    return 0;
}
