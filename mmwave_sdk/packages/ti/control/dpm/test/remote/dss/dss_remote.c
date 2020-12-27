/*
 *   @file  dss_remote.c
 *
 *   @brief
 *      Unit Test for the DPM Framework which executes on the DSS
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
#include <ti/sysbios/family/c64p/Cache.h>
#include <ti/sysbios/family/c64p/Hwi.h>
#include <ti/sysbios/family/c64p/EventCombiner.h>
#include <ti/sysbios/utils/Load.h>

/* mmWave SDK Include Files. */
#include <ti/drivers/soc/soc.h>
#include <ti/common/sys_common.h>
#include <ti/control/dpm/dpm.h>
#include <ti/control/mmwave/mmwave.h>
#include <ti/drivers/mailbox/mailbox.h>
#include <ti/common/mmwave_sdk_version.h>
#include <ti/utils/cycleprofiler/cycle_profiler.h>
#include <ti/utils/testlogger/logger.h>

/* Demo Include Files: */
#include <ti/control/dpm/test/remote/dpc/ll/ll_profile_remote.h>

/**************************************************************************
 *************************** Local Structures *****************************
 **************************************************************************/

/**
 * @brief
 *  Unit Test MCB
 *
 * @details
 *  MCB which holds all the relevant information required to execute
 *  the Unit Test.
 */
typedef struct DPMTest_MCB_t
{
    /**
     * @brief   Handle to the SOC Module
     */
    SOC_Handle              socHandle;

    /**
     * @brief   Handle to the mmWave control module
     */
    MMWave_Handle           ctrlHandle;

    /**
     * @brief   Handle to the DPM Instance: This is responsible for
     * executing the LL Data Path Profile
     */
    DPM_Handle              llDPMHandle;

    /**
     * @brief   The LL Data Path Profile Status: Applications track the status since the
     * profile is distributed
     */
    bool                    llProfileStatus;

    /**
     * @brief   Raw Data Counter: This is just used to create a unique raw data for every
     * iteration
     */
    volatile uint32_t       rawDataCounter;

    /**
     * @brief   This is the number of results which have been successfully validated
     * On the DSS we are executing only the LL Profile.
     */
    uint32_t                validationCounter;

    /**
     * @brief   Inject Data: This is input data which is being injected into the DPM Data
     * Path Profile.
     */
    char                    injectData[128];
}DPMTest_MCB;

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/

/**
 * @brief   DSS Unit Test MCB
 */
DPMTest_MCB     gDPMTestMCB;

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

/**
 * @brief   Global variable which controls the data injection on the DSS
 * This is controlled by the MSS.
 */
#pragma DATA_SECTION(gInjectData, ".dataInjectionControl");
volatile uint8_t gInjectData[2] = { 0 };

/**************************************************************************
 **************************** Local Functions *****************************
 **************************************************************************/

/* Unit Test DSS Task(s) */
static void DPMTest_dssInitTask(UArg arg0, UArg arg1);
static void DPMTest_dataInjectionTask (UArg arg0, UArg arg1);
static void DPMTest_dpmTask(UArg arg0, UArg arg1);

/* DPM Reporting API: */
static void DPMTest_reportFxn(DPM_Report reportType, uint32_t profileId, int32_t errCode, uint32_t arg0, uint32_t arg1);

/**************************************************************************
 ************************* Unit Test DSS Functions ************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      DPM Registered Report Handler. The DPM Module uses this registered function to notify
 *      the application about DPM reports.
 *
 *  @param[in]  reportType
 *      Report Type
 *  @param[in]  profileId
 *      Profile Identifier which generated the report
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
static void DPMTest_reportFxn
(
    DPM_Report  reportType,
    uint32_t    profileId,
    int32_t     errCode,
    uint32_t    arg0,
    uint32_t    arg1
)
{
    char    logString[128];

    /* Sanity Checking: */
    DebugP_assert ((profileId == LL_DSS_DPM_INSTANCE_ID) || (profileId == LL_MSS_DPM_INSTANCE_ID));

    /* Only errors are logged on the console: */
    if (errCode != 0)
    {
        /* Error: Detected log on the console and die all errors are FATAL currently. */
        System_printf ("Error: DPM Report %d received with error:%d arg0:0x%x arg1:0x%x\n",
                        reportType, errCode, arg0, arg1);
        MCPI_setFeatureTestResult ("DPM Remote Domain Report", MCPI_TestResult_FAIL);
        DebugP_assert (0);
    }

    /* Processing further is based on the reports received: This is the control of the profile
     * state machine: */
    switch (reportType)
    {
        case DPM_Report_IOCTL:
        {
            /*****************************************************************
             * Profile has been configured without an error:
             * - This is an indication that the profile configuration commands
             *   went through without any issues.
             *****************************************************************/
            break;
        }
        case DPM_Report_DPC_STARTED:
        {
            /*****************************************************************
             * Profile has been started without an error:
             * - Setup the application status
             *****************************************************************/
            gDPMTestMCB.llProfileStatus = true;
            MCPI_setFeatureTestResult ("LL DPC Started", MCPI_TestResult_PASS);
            break;
        }
        case DPM_Report_NOTIFY_DPC_RESULT:
        {
            break;
        }
        case DPM_Report_DPC_ASSERT:
        {
            /* Profile asserted: Kill the system */
            DebugP_assert (0);
            break;
        }
        case DPM_Report_DPC_STOPPED:
        {
            /*****************************************************************
             * Profile has been stopped without an error:
             *****************************************************************/
            gDPMTestMCB.llProfileStatus = false;

            /* Create the MCPI Logging String: */
            sprintf (logString, "LL DPC Stopped Results validated=%d", gDPMTestMCB.validationCounter);
            MCPI_setFeatureTestResult (logString, MCPI_TestResult_PASS);

            /* Reset the counter: */
            gDPMTestMCB.validationCounter = 0U;
            break;
        }
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to validate the results.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t DPMTest_validateResults (DPM_Buffer* ptrResult)
{
    char        resultString[256];

    /* Sanity Checking: The Test Profiles only generate one result */
    if ((ptrResult->ptrBuffer[0] == NULL) || (ptrResult->size[0] == 0U))
    {
        /* Error: Invalid result detected */
        return -1;
    }

    /* Sanity Checking: The Test Profiles only generate one result */
    if ((ptrResult->ptrBuffer[1] != NULL) || (ptrResult->size[1] != 0U))
    {
        /* Error: Invalid result detected */
        return -1;
    }

    /* Sanity Checking: The Test Profiles only generate one result */
    if ((ptrResult->ptrBuffer[2] != NULL) || (ptrResult->size[2] != 0U))
    {
        /* Error: Invalid result detected */
        return -1;
    }

    /* Construct the expected result: The raw data counter has already been incremented so
     * we need to discount it here. All the configurations are hardcoded here. */
    snprintf (resultString, sizeof(resultString),
              "[Raw Data: %d]LL Processing [FFTC 10:20] [Peak Grouping: 30]",
              (gDPMTestMCB.rawDataCounter - 1));

    /* Compare the strings: */
    if (strcmp (resultString, (char*)ptrResult->ptrBuffer[0]) == 0)
    {
        /* Increment the number of successful validations: */
        gDPMTestMCB.validationCounter++;
        return 0;
    }

    /* No match: */
    System_printf ("Result String  : '%s'\n", resultString);
    System_printf ("Expected String: '%s'\n", (char*)ptrResult->ptrBuffer[0]);
    return -1;
}

/**
 *  @b Description
 *  @n
 *      LL Profile Injection Task:The  Task periodically executes
 *      and injects data into the profile. This is to simulate the
 *      reception of raw data.
 *
 *  @retval
 *      Not Applicable.
 */
static void DPMTest_dataInjectionTask (UArg arg0, UArg arg1)
{
    DPM_Buffer      buffer;
    int32_t         errCode;
    uint8_t         injectDataState;

    /* Loop around forever: */
    while (1)
    {
        /* Take a snapshot of the Inject Data State: */
        injectDataState = gInjectData[0];

        /* Do we need to inject the data? */
        if (injectDataState == 1U)
        {
            /* YES: Initialize the buffer */
            memset ((void *)&buffer, 0, sizeof(DPM_Buffer));

            /* Setup the data buffer: */
            sprintf (gDPMTestMCB.injectData, "Raw Data: %d", gDPMTestMCB.rawDataCounter++);
            buffer.ptrBuffer[0] = (uint8_t*)&gDPMTestMCB.injectData[0];
            buffer.size[0]      = strlen (gDPMTestMCB.injectData);

            /* Inject the buffer: */
            errCode = DPM_ioctl (gDPMTestMCB.llDPMHandle,
                                 DPM_CMD_INJECT_DATA,
                                 &buffer,
                                 sizeof(DPM_Buffer));
            if (errCode < 0)
            {
                printf ("Error: Unable to inject the data [Error code %d]\n", errCode);
                MCPI_setFeatureTestResult ("DPM Inject Data", MCPI_TestResult_FAIL);
            }
        }

        /* Wait for some time: */
        Task_sleep(30);

        /* Did the MSS change the state? */
        if (injectDataState != gInjectData[0])
        {
            /* YES: State Change Detected; so acknowledge it. */
            MCPI_setFeatureTestResult ("Injecting Data State Change Detected", MCPI_TestResult_PASS);
            gInjectData[1] = 1U;
        }
    }
}

/**
 *  @b Description
 *  @n
 *      DPM Execution Task
 *
 *  @retval
 *      Not Applicable.
 */
static void DPMTest_dpmTask(UArg arg0, UArg arg1)
{
    int32_t     errCode;
    DPM_Buffer  result;

    while (1)
    {
        /* Execute the DPM module: */
        errCode = DPM_execute (gDPMTestMCB.llDPMHandle, &result);
        if (errCode < 0)
        {
            System_printf ("Error: DPM execution failed [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("DPM Execute", MCPI_TestResult_FAIL);
        }

        /* Do we have results which are available and is the DPC started? */
        if ((result.size[0] != 0U) && (gDPMTestMCB.llProfileStatus == true))
        {
            /* Sanity Checking: Validate the received results */
            if (DPMTest_validateResults (&result) < 0)
            {
                /* Error: Validation Failed. Log this error */
                MCPI_setFeatureTestResult ("DPM Validate Results", MCPI_TestResult_FAIL);
            }

            /* YES: Results are available send them. */
            errCode = DPM_sendResult (gDPMTestMCB.llDPMHandle, false, &result);
            if (errCode < 0)
            {
                System_printf ("Error: LL Profile failed to send results [Error: %d]\n", errCode);
                MCPI_setFeatureTestResult ("DPM Send Result", MCPI_TestResult_FAIL);
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
static void DPMTest_dssInitTask (UArg arg0, UArg arg1)
{
    int32_t             errCode;
    Task_Params         taskParams;
    DPM_InitCfg         dpmInitCfg;

    /*****************************************************************************
     * Initialize the mmWave SDK components:
     *****************************************************************************/

    /* Initialize the Mailbox */
    Mailbox_init(MAILBOX_TYPE_DSS);

    /* Initialize the Cycle Profiler: */
    Cycleprofiler_init();

    /* Initialize the MCPI framework: */
    MCPI_Initialize ();

    /*****************************************************************************
     * Initialization of the DPM Module:
     *****************************************************************************/
    memset ((void *)&dpmInitCfg, 0, sizeof(DPM_InitCfg));

    /* Setup the configuration: */
    dpmInitCfg.socHandle       = gDPMTestMCB.socHandle;
    dpmInitCfg.domain          = DPM_Domain_REMOTE;
    dpmInitCfg.ptrProcChainCfg = &gLLProfileCfg;
    dpmInitCfg.instanceId      = LL_DSS_DPM_INSTANCE_ID;
    dpmInitCfg.reportFxn       = DPMTest_reportFxn;

    /* Initialize the DPM Module: */
    gDPMTestMCB.llDPMHandle = DPM_init (&dpmInitCfg, &errCode);
    if (gDPMTestMCB.llDPMHandle == NULL)
    {
        System_printf ("Error: Unable to initialize the DPM Module [Error: %d]\n", errCode);
        MCPI_setFeatureTestResult ("DPM Remote Domain Initialization", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("DPM Remote Domain Initialization", MCPI_TestResult_PASS);

    /* Launch the DPM Task:
     * - This is always a lower priority than the mmWave control task because
     *   control events should always be higher priority. */
    Task_Params_init(&taskParams);
    taskParams.priority  = 5;
    taskParams.stackSize = 4*1024;
    Task_create(DPMTest_dpmTask, &taskParams, NULL);

    /* Synchronization: This will synchronize the execution of the control module
     * between the domains. This is a prerequiste and always needs to be invoked. */
    while (1)
    {
        int32_t syncStatus;

        /* Get the synchronization status: */
        syncStatus = DPM_synch (gDPMTestMCB.llDPMHandle, &errCode);
        if (syncStatus < 0)
        {
            /* Error: Unable to synchronize the framework */
            System_printf ("Error: DPM Synchronization failed [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("DPM Remote Domain Synchronization", MCPI_TestResult_FAIL);
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
    MCPI_setFeatureTestResult ("DPM Remote Domain Synchronization", MCPI_TestResult_PASS);

    /* Launch the Data Simulation Task: */
    Task_Params_init(&taskParams);
    taskParams.priority  = 3;
    taskParams.stackSize = 4*1024;
    Task_create(DPMTest_dataInjectionTask, &taskParams, NULL);

    return;
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
    Task_Params    taskParams;
    SOC_Cfg        socCfg;
    int32_t        errCode;

    /* Initialize and populate the demo MCB */
    memset ((void*)&gDPMTestMCB, 0, sizeof(DPMTest_MCB));

    /* Initialize the SOC confiugration: */
    memset ((void *)&socCfg, 0, sizeof(SOC_Cfg));

    /* Populate the SOC configuration: */
    socCfg.clockCfg = SOC_SysClock_BYPASS_INIT;

    /* Initialize the SOC Module: This is done as soon as the application is started
     * to ensure that the MPU is correctly configured. */
    gDPMTestMCB.socHandle = SOC_init (&socCfg, &errCode);
    if (gDPMTestMCB.socHandle == NULL)
    {
        System_printf ("Error: SOC Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.priority  = 2;
    taskParams.stackSize = 3*1024;
    Task_create(DPMTest_dssInitTask, &taskParams, NULL);

    /* Start BIOS */
    BIOS_start();
    return 0;
}

