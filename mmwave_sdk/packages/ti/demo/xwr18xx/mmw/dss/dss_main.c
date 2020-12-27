/**
 *   @file  dss_main.c
 *
 *   @brief
 *      This is the main file which implements the millimeter wave Demo
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

/* Standard Include Files. */
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
#include <ti/sysbios/heaps/HeapBuf.h>
#include <ti/sysbios/heaps/HeapMem.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/utils/Load.h>
#include <ti/sysbios/family/c64p/Hwi.h>

/* mmWave SDK Include Files: */
#include <ti/common/sys_common.h>
#include <ti/common/mmwave_sdk_version.h>
#include <ti/control/mmwave/mmwave.h>
#include <ti/control/dpm/dpm.h>
#include <ti/datapath/dpc/objectdetection/objdethwa/objectdetection.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/esm/esm.h>
#include <ti/drivers/crc/crc.h>
#include <ti/drivers/gpio/gpio.h>
#include <ti/drivers/mailbox/mailbox.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/uart/UART.h>
#include <ti/utils/cli/cli.h>
#include <ti/utils/mathutils/mathutils.h>

/* Demo Include Files */
#include <ti/demo/utils/mmwdemo_rfparser.h>
#include <ti/demo/xwr18xx/mmw/include/mmw_output.h>
#include <ti/demo/xwr18xx/mmw/include/mmw_config.h>
#include <ti/demo/xwr18xx/mmw/dss/mmw_dss.h>

/* Demo Profiling Include Files */
#include <ti/utils/cycleprofiler/cycle_profiler.h>

/**
 * @brief Task Priority settings:
 */
#define MMWDEMO_DPC_OBJDET_DPM_TASK_PRIORITY      5

/*! L3 RAM buffer for object detection DPC */
uint8_t gMmwL3[SOC_L3RAM_SIZE];
#pragma DATA_SECTION(gMmwL3, ".l3ram");

 /*! L2 RAM buffer for object detection DPC */
#define MMWDEMO_OBJDET_L2RAM_SIZE (49U * 1024U)
uint8_t gDPC_ObjDetL2Heap[MMWDEMO_OBJDET_L2RAM_SIZE];
#pragma DATA_SECTION(gDPC_ObjDetL2Heap, ".dpc_l2Heap");

 /*! HSRAM for processing results */
#pragma DATA_SECTION(gHSRAM, ".demoSharedMem");
#pragma DATA_ALIGN(gHSRAM, 4);

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/

/**
 * @brief
 *  Global Variable for tracking information required by the mmw Demo
 */
MmwDemo_DSS_MCB    gMmwDssMCB;

/**
 * @brief
 *  Global Variable for DPM result buffer
 */
DPM_Buffer  resultBuffer;

/**
 * @brief
 *  Global Variable for HSRAM buffer used to share results to remote
 */
MmwDemo_HSRAM gHSRAM;

/**************************************************************************
 ******************* Millimeter Wave Demo Functions Prototype *******************
 **************************************************************************/
static void MmwDemo_dssInitTask(UArg arg0, UArg arg1);
static void MmwDemo_DPC_ObjectDetection_reportFxn
(
    DPM_Report  reportType,
    uint32_t    instanceId,
    int32_t     errCode,
    uint32_t    arg0,
    uint32_t    arg1
);
static void MmwDemo_DPC_ObjectDetection_processFrameBeginCallBackFxn(uint8_t subFrameIndx);
static void MmwDemo_DPC_ObjectDetection_processInterFrameBeginCallBackFxn(uint8_t subFrameIndx);
static void MmwDemo_updateObjectDetStats
(
    DPC_ObjectDetection_Stats       *currDpcStats,
    MmwDemo_output_message_stats    *outputMsgStats
);

static int32_t MmwDemo_copyResultToHSRAM
(
    MmwDemo_HSRAM           *ptrHsramBuffer,
    DPC_ObjectDetection_ExecuteResult *result,
    MmwDemo_output_message_stats *outStats
);
static void MmwDemo_DPC_ObjectDetection_dpmTask(UArg arg0, UArg arg1);
static void MmwDemo_sensorStopEpilog(void);

/**************************************************************************
 ************************* Millimeter Wave Demo Functions **********************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      Epilog processing after sensor has stopped
 *
 *  @retval None
 */
static void MmwDemo_sensorStopEpilog(void)
{
    Hwi_StackInfo   stackInfo;
    Task_Stat       stat;
    bool            hwiStackOverflow;

    System_printf("Data Path Stopped (last frame processing done)\n");

    /* Print DSS task statistics */
    System_printf("DSS Task Stack Usage (Note: Task Stack Usage) ==========\n");

    Task_stat(gMmwDssMCB.initTaskHandle, &stat);
    System_printf("%20s %12d %12d %12d\n", "initTask",
                  stat.stackSize,
                  stat.used,
                  stat.stackSize - stat.used);

    Task_stat(gMmwDssMCB.objDetDpmTaskHandle, &stat);
    System_printf("%20s %12s %12s %12s\n", "Task Name", "Size", "Used", "Free");
    System_printf("%20s %12d %12d %12d\n", "ObjDet DPM",
                  stat.stackSize,
                  stat.used,
                  stat.stackSize - stat.used);

    System_printf("HWI Stack (same as System Stack) Usage ============\n");
    hwiStackOverflow = Hwi_getStackInfo(&stackInfo, TRUE);
    if (hwiStackOverflow == TRUE)
    {
        System_printf("DSS HWI Stack overflowed\n");
        MmwDemo_debugAssert(0);
    }
    else
    {
        System_printf("%20s %12s %12s %12s\n", " ", "Size", "Used", "Free");
        System_printf("%20s %12d %12d %12d\n", " ",
                      stackInfo.hwiStackSize,
                      stackInfo.hwiStackPeak,
                      stackInfo.hwiStackSize - stackInfo.hwiStackPeak);
    }
}

/**
 *  @b Description
 *  @n
 *      DPM Registered Report Handler. The DPM Module uses this registered function to notify
 *      the application about DPM reports.
 *
 *  @param[in]  reportType
 *      Report Type
 *  @param[in]  instanceId
 *      Instance Identifier which generated the report
 *  @param[in]  errCode
 *      Error code if any.
 *  @param[in] arg0
 *      Argument 0 interpreted with the report type
 *  @param[in] arg1
 *      Argument 1 interpreted with the report type
 *
 *  @retval
 *      Not Applicable.
 */
static void MmwDemo_DPC_ObjectDetection_reportFxn
(
    DPM_Report  reportType,
    uint32_t    instanceId,
    int32_t     errCode,
    uint32_t    arg0,
    uint32_t    arg1
)
{

    /* Only errors are logged on the console: */
    if (errCode != 0)
    {
        /* Error: Detected log on the console and die all errors are FATAL currently. */
        System_printf ("Error: DPM Report %d received with error:%d arg0:0x%x arg1:0x%x\n",
                        reportType, errCode, arg0, arg1);
        DebugP_assert (0);
    }

    /* Processing further is based on the reports received: This is the control of the profile
     * state machine: */
    switch (reportType)
    {
        case DPM_Report_IOCTL:
        {
            /*****************************************************************
             * DPC has been configured without an error:
             * - This is an indication that the profile configuration commands
             *   went through without any issues.
             *****************************************************************/
            DebugP_log1("DSSApp: DPM Report IOCTL, command = %d\n", arg0);
            break;
        }
        case DPM_Report_DPC_STARTED:
        {
            /*****************************************************************
             * DPC has been started without an error:
             * - notify sensor management task that DPC is started.
             *****************************************************************/
            DebugP_log0("DSSApp: DPM Report start\n");
            break;
        }
        case DPM_Report_NOTIFY_DPC_RESULT:
        {
            /*****************************************************************
             * DPC Results have been passed:
             * - This implies that we have valid profile results which have
             *   been received from the profile.
             *****************************************************************/

            break;
        }
        case DPM_Report_NOTIFY_DPC_RESULT_ACKED:
        {
            /*****************************************************************
             * DPC Results have been acked:
             * - This implies that MSS received the results.
             *****************************************************************/

            break;
        }
        case DPM_Report_DPC_ASSERT:
        {
            DPM_DPCAssert*  ptrAssert;

            /*****************************************************************
             * DPC Fault has been detected:
             * - This implies that the DPC has crashed.
             * - The argument0 points to the DPC assertion information
             *****************************************************************/
            ptrAssert = (DPM_DPCAssert*)arg0;
            System_printf ("DSS Exception: %s, line %d.\n", ptrAssert->fileName,
                       ptrAssert->lineNum);
            break;
        }
        case DPM_Report_DPC_STOPPED:
        {
            /*****************************************************************
             * DPC has been stopped without an error:
             * - This implies that the DPC can either be reconfigured or
             *   restarted.
             *****************************************************************/
            DebugP_log0("DSSApp: DPM Report stop\n");

            MmwDemo_sensorStopEpilog();
            break;
        }
        case DPM_Report_DPC_INFO:
        {
            /* Currently objDetHwa does not use this feature. */
            break;
        }
        default:
        {
            DebugP_assert (0);
            break;
        }
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      Call back function that was registered during config time and is going
 *      to be called in DPC processing at the beginning of frame/sub-frame processing,
 *      we use this to issue BIOS calls for computing CPU load during inter-frame
 *
 *  @param[in] subFrameIndx Sub-frame index of the sub-frame during which processing
 *             this function was called.
 *
 *  @retval None
 */
static void MmwDemo_DPC_ObjectDetection_processFrameBeginCallBackFxn(uint8_t subFrameIndx)
{
    Load_update();
    gMmwDssMCB.dataPathObj.subFrameStats[subFrameIndx].interFrameCPULoad = Load_getCPULoad();
}

/**
 *  @b Description
 *  @n
 *      Call back function that was registered during config time and is going
 *      to be called in DPC processing at the beginning of
 *      inter-frame/inter-sub-frame processing,
 *      we use this to issue BIOS calls for computing CPU load during active frame
 *      (chirping)
 *
 *  @param[in] subFrameIndx Sub-frame index of the sub-frame during which processing
 *             this function was called.
 *
 *  @retval None
 */
static void MmwDemo_DPC_ObjectDetection_processInterFrameBeginCallBackFxn(uint8_t subFrameIndx)
{
    Load_update();
    gMmwDssMCB.dataPathObj.subFrameStats[subFrameIndx].activeFrameCPULoad = Load_getCPULoad();
}


/**
 *  @b Description
 *  @n
 *      Update stats based on the stats from DPC
 *
 *  @param[in]  currDpcStats        Pointer to DPC status
 *  @param[in]  outputMsgStats      Pointer to Output message stats 
 *
 *  @retval
 *      Not Applicable.
 */
 void MmwDemo_updateObjectDetStats
(
    DPC_ObjectDetection_Stats       *currDpcStats,
    MmwDemo_output_message_stats    *outputMsgStats
)
{
    static uint32_t prevInterFrameEndTimeStamp = 0U;

    /* Calculate interframe proc time */
    outputMsgStats->interFrameProcessingTime =
            (currDpcStats->interFrameEndTimeStamp - currDpcStats->interFrameStartTimeStamp)/DSP_CLOCK_MHZ; /* In micro seconds */

    outputMsgStats->interChirpProcessingMargin = currDpcStats->interChirpProcessingMargin/DSP_CLOCK_MHZ;

    /* Calculate interFrame processing Margin for previous frame, but saved to current frame */
    outputMsgStats->interFrameProcessingMargin =
        (currDpcStats->frameStartTimeStamp - prevInterFrameEndTimeStamp - currDpcStats->subFramePreparationCycles)/DSP_CLOCK_MHZ;

    prevInterFrameEndTimeStamp = currDpcStats->interFrameEndTimeStamp;
}


/**
 *  @b Description
 *  @n
 *      Copy DPC results and output stats to HSRAM to share with MSS
 *
 *  @param[in]  ptrHsramBuffer      Pointer to HSRAM buffer memory
 *  @param[in]  result              Pointer to DPC results
 *  @param[in]  outStats            Pointer to Output message stats
 *
 *  @retval
 *      Not Applicable.
 */
static int32_t MmwDemo_copyResultToHSRAM
(
    MmwDemo_HSRAM           *ptrHsramBuffer,
    DPC_ObjectDetection_ExecuteResult *result,
    MmwDemo_output_message_stats *outStats
)
{
    uint8_t             *ptrCurrBuffer;
    uint32_t            totalHsramSize;
    uint32_t            itemPayloadLen;

    /* Save result in HSRAM */
    if(ptrHsramBuffer == NULL)
    {
        return -1;
    }

    /* Save result in HSRAM */
    if(result != NULL)
    {
        itemPayloadLen = sizeof(DPC_ObjectDetection_ExecuteResult);
        memcpy((void *)&ptrHsramBuffer->result, (void *)result, itemPayloadLen);
    }
    else
    {
        return -1;
    }

    /* Save output Stats in HSRAM */
    if(outStats != NULL)
    {
        itemPayloadLen = sizeof(MmwDemo_output_message_stats);
        memcpy((void *)&ptrHsramBuffer->outStats, (void *)outStats, itemPayloadLen);
    }

    /* Set payload pointer to HSM buffer */
    ptrCurrBuffer = &ptrHsramBuffer->payload[0];
    totalHsramSize = MMWDEMO_HSRAM_PAYLOAD_SIZE;

    /* Save ObjOut in HSRAM */
    if(result->objOut != NULL)
    {
        itemPayloadLen = sizeof(DPIF_PointCloudCartesian) * result->numObjOut;
        if((totalHsramSize- itemPayloadLen) > 0)
        {
            memcpy(ptrCurrBuffer, (void *)result->objOut, itemPayloadLen);

            ptrHsramBuffer->result.objOut = (DPIF_PointCloudCartesian *)ptrCurrBuffer;
            ptrCurrBuffer+= itemPayloadLen;
            totalHsramSize -=itemPayloadLen;
        }
        else
        {
            return -1;
        }
    }

    /* Save ObjOutSideInfo in HSRAM */
    if(result->objOutSideInfo != NULL)
    {
        itemPayloadLen = sizeof(DPIF_PointCloudSideInfo) * result->numObjOut;
        if((totalHsramSize- itemPayloadLen) > 0)
        {
            memcpy(ptrCurrBuffer, (void *)result->objOutSideInfo, itemPayloadLen);
            ptrHsramBuffer->result.objOutSideInfo = (DPIF_PointCloudSideInfo *)ptrCurrBuffer;
            ptrCurrBuffer+= itemPayloadLen;
            totalHsramSize -=itemPayloadLen;
        }
        else
        {
            return -1;
        }
    }

    /* Save DPC_ObjectDetection_Stats in HSRAM */
    if(result->stats != NULL)
    {
        itemPayloadLen = sizeof(DPC_ObjectDetection_Stats);
        if((totalHsramSize- itemPayloadLen) > 0)
        {
            memcpy(ptrCurrBuffer, (void *)result->stats, itemPayloadLen);
            ptrHsramBuffer->result.stats = (DPC_ObjectDetection_Stats *)ptrCurrBuffer;
            ptrCurrBuffer+= itemPayloadLen;
            totalHsramSize -=itemPayloadLen;
        }
        else
        {
            return -1;
        }
    }

    /* Save compRxChanBiasMeasurement in HSRAM */
    if(result->compRxChanBiasMeasurement != NULL)
    {
        itemPayloadLen = sizeof(DPU_AoAProc_compRxChannelBiasCfg);
        if((totalHsramSize- itemPayloadLen) > 0)
        {
            memcpy(ptrCurrBuffer, (void *)result->compRxChanBiasMeasurement, itemPayloadLen);
            ptrHsramBuffer->result.compRxChanBiasMeasurement = (DPU_AoAProc_compRxChannelBiasCfg *)ptrCurrBuffer;
            ptrCurrBuffer+= itemPayloadLen;
            totalHsramSize -=itemPayloadLen;
        }
        else
        {
            return -1;
        }
    }

    return totalHsramSize;
}

/**
 *  @b Description
 *  @n
 *      DPM Execution Task. DPM execute results are processed here:
 *      a) Update states based on timestamp from DPC.
 *      b) Copy results to shared memory to be shared with MSS.
 *      c) Send Results to MSS by calling DPM_sendResult()
 *
 *  @retval
 *      Not Applicable.
 */
static void MmwDemo_DPC_ObjectDetection_dpmTask(UArg arg0, UArg arg1)
{
    int32_t     retVal;
    DPC_ObjectDetection_ExecuteResult *result;
    volatile uint32_t              startTime;

    while (1)
    {
        /* Execute the DPM module: */
        retVal = DPM_execute (gMmwDssMCB.dataPathObj.objDetDpmHandle, &resultBuffer);
        if (retVal < 0) {
            System_printf ("Error: DPM execution failed [Error code %d]\n", retVal);
            MmwDemo_debugAssert (0);
        }
        else
        {
            if ((resultBuffer.size[0] == sizeof(DPC_ObjectDetection_ExecuteResult)))
            {
                result = (DPC_ObjectDetection_ExecuteResult *)resultBuffer.ptrBuffer[0];

                /* Get the time stamp before copy data to HSRAM */
                startTime = Cycleprofiler_getTimeStamp();

                /* Update processing stats and added it to buffer 1*/
                MmwDemo_updateObjectDetStats(result->stats,
                                                &gMmwDssMCB.dataPathObj.subFrameStats[result->subFrameIdx]);

                /* Copy result data to HSRAM */
                if ((retVal = MmwDemo_copyResultToHSRAM(&gHSRAM, result, &gMmwDssMCB.dataPathObj.subFrameStats[result->subFrameIdx])) >= 0)
                {
                    /* Update interframe margin with HSRAM copy time */
                    gHSRAM.outStats.interFrameProcessingMargin -= ((Cycleprofiler_getTimeStamp() - startTime)/DSP_CLOCK_MHZ);

                    /* Update DPM buffer */
                    resultBuffer.ptrBuffer[0] = (uint8_t *)&gHSRAM.result;
                    resultBuffer.ptrBuffer[1] = (uint8_t *)&gHSRAM.outStats;
                    resultBuffer.size[1] = sizeof(MmwDemo_output_message_stats);

                    /* YES: Results are available send them. */
                    retVal = DPM_sendResult (gMmwDssMCB.dataPathObj.objDetDpmHandle, true, &resultBuffer);
                    if (retVal < 0)
                    {
                        System_printf ("Error: Failed to send results [Error: %d] to remote\n", retVal);
                    }
                }
                else
                {
                    System_printf ("Error: Failed to copy processing results to HSRAM, error=%d\n", retVal);
                    MmwDemo_debugAssert (0);
                }
            }
        }
    }
}

/**
 *  @b Description
 *  @n
 *      System Initialization Task which initializes the various
 *      components in the system.
 *
 *  @retval
 *      Not Applicable.
 */
static void MmwDemo_dssInitTask(UArg arg0, UArg arg1)
{
    int32_t             errCode;
    Task_Params         taskParams;
    DPM_InitCfg         dpmInitCfg;
    DPC_ObjectDetection_InitParams      objDetInitParams;
    uint32_t            edmaCCIdx=0;

    /*****************************************************************************
     * Driver Init:
     *****************************************************************************/

    /* Initialize the Mailbox */
    Mailbox_init(MAILBOX_TYPE_DSS);

    /*****************************************************************************
     * Driver Open/Configuraiton:
     *****************************************************************************/

    /* Initialize the Data Path: */
    MmwDemo_dataPathInit(&gMmwDssMCB.dataPathObj);
    MmwDemo_dataPathOpen(&gMmwDssMCB.dataPathObj);

    /*****************************************************************************
     * Initialization of the DPM Module:
     *****************************************************************************/
    memset ((void *)&objDetInitParams, 0, sizeof(DPC_ObjectDetection_InitParams));

    /* Note this must be after MmwDemo_dataPathOpen() above which opens the hwa */
    objDetInitParams.hwaHandle = gMmwDssMCB.dataPathObj.hwaHandle;
    objDetInitParams.L3ramCfg.addr = (void *)&gMmwL3[0];
    objDetInitParams.L3ramCfg.size = sizeof(gMmwL3);
    objDetInitParams.CoreLocalRamCfg.addr = &gDPC_ObjDetL2Heap[0];
    objDetInitParams.CoreLocalRamCfg.size = sizeof(gDPC_ObjDetL2Heap);
    for (edmaCCIdx=0;edmaCCIdx<EDMA_NUM_CC;edmaCCIdx++)
    {
        objDetInitParams.edmaHandle[edmaCCIdx] = gMmwDssMCB.dataPathObj.edmaHandle[edmaCCIdx];
    }

    /* DPC Call-back config */
    objDetInitParams.processCallBackCfg.processFrameBeginCallBackFxn =
        MmwDemo_DPC_ObjectDetection_processFrameBeginCallBackFxn;
    objDetInitParams.processCallBackCfg.processInterFrameBeginCallBackFxn =
        MmwDemo_DPC_ObjectDetection_processInterFrameBeginCallBackFxn;

    memset ((void *)&dpmInitCfg, 0, sizeof(DPM_InitCfg));

    /* Setup the configuration: */
    dpmInitCfg.socHandle        = gMmwDssMCB.socHandle;
    dpmInitCfg.ptrProcChainCfg  = &gDPC_ObjectDetectionCfg;
    dpmInitCfg.instanceId       = 0xFEEDFEED;
    dpmInitCfg.domain           = DPM_Domain_REMOTE;
    dpmInitCfg.reportFxn        = MmwDemo_DPC_ObjectDetection_reportFxn;
    dpmInitCfg.arg              = &objDetInitParams;
    dpmInitCfg.argSize          = sizeof(DPC_ObjectDetection_InitParams);

    /* Initialize the DPM Module: */
    gMmwDssMCB.dataPathObj.objDetDpmHandle = DPM_init (&dpmInitCfg, &errCode);
    if (gMmwDssMCB.dataPathObj.objDetDpmHandle == NULL)
    {
        System_printf ("Error: Unable to initialize the DPM Module [Error: %d]\n", errCode);
        MmwDemo_debugAssert (0);
        return;
    }

    /* Synchronization: This will synchronize the execution of the control module
     * between the domains. This is a prerequiste and always needs to be invoked. */
    while (1)
    {
        int32_t syncStatus;

        /* Get the synchronization status: */
        syncStatus = DPM_synch (gMmwDssMCB.dataPathObj.objDetDpmHandle, &errCode);
        if (syncStatus < 0)
        {
            /* Error: Unable to synchronize the framework */
            System_printf ("Error: DPM Synchronization failed [Error code %d]\n", errCode);
            MmwDemo_debugAssert (0);
            return;
        }
        if (syncStatus == 1)
        {
            /* Synchronization acheived: */
            break;
        }
        /* Sleep and poll again: */
        Task_sleep(1);
    }
    System_printf ("Debug: DPM Module Sync is done\n");

    /* Launch the DPM Task */
    Task_Params_init(&taskParams);
    taskParams.priority  = MMWDEMO_DPC_OBJDET_DPM_TASK_PRIORITY;
    taskParams.stackSize = 4*1024;
    gMmwDssMCB.objDetDpmTaskHandle = Task_create(MmwDemo_DPC_ObjectDetection_dpmTask, &taskParams, NULL);

    return;
}

/**
 *  @b Description
 *  @n
 *     Function to sleep the DSP using IDLE instruction.
 *     When DSP has no work left to do,
 *     the BIOS will be in Idle thread and will call this function.
 *
 *  @retval
 *      Not Applicable.
 */
void MmwDemo_sleep(void)
{
    /* issue WFI (Wait For Interrupt) instruction */
    asm(" IDLE ");
}

/**
 *  @b Description
 *  @n
 *      Entry point into the Millimeter Wave Demo
 *
 *  @retval
 *      Not Applicable.
 */
int main (void)
{
    Task_Params     taskParams;
    int32_t         errCode;
    SOC_Handle      socHandle;
    SOC_Cfg         socCfg;

    /* Initialize and populate the demo MCB */
    memset ((void*)&gMmwDssMCB, 0, sizeof(MmwDemo_DSS_MCB));

    /* Initialize the SOC confiugration: */
    memset ((void *)&socCfg, 0, sizeof(SOC_Cfg));

    /* Populate the SOC configuration: */
    socCfg.clockCfg = SOC_SysClock_BYPASS_INIT;

    /* Initialize the SOC Module: This is done as soon as the application is started
     * to ensure that the MPU is correctly configured. */
    socHandle = SOC_init (&socCfg, &errCode);
    if (socHandle == NULL)
    {
        System_printf ("Error: SOC Module Initialization failed [Error code %d]\n", errCode);
        MmwDemo_debugAssert (0);
        return -1;
    }

    gMmwDssMCB.socHandle = socHandle;

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 4*1024;
    gMmwDssMCB.initTaskHandle = Task_create(MmwDemo_dssInitTask, &taskParams, NULL);

    /* Start BIOS */
    BIOS_start();
    return 0;
}
