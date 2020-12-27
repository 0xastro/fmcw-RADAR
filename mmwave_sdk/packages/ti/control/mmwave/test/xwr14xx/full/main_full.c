/*
 *   @file  main.c
 *
 *   @brief
 *      Unit Test for mmWave control
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
#include <ti/sysbios/family/arm/v7r/vim/Hwi.h>

/* mmWave SDK Include Files: */
#include <ti/common/sys_common.h>
#include <ti/drivers/esm/esm.h>
#include <ti/drivers/crc/crc.h>
#include <ti/drivers/mailbox/mailbox.h>
#include <ti/control/mmwave/mmwave.h>
#include <ti/utils/testlogger/logger.h>

/**************************************************************************
 ****************************** Global Variables **************************
 **************************************************************************/

/* Global Handle which tracks the mmWave control module */
MMWave_Handle   gMMWaveHandle;

/* Global Handle which tracks the SOC Handle */
SOC_Handle      socHandle;

/* Global Variable: The variable detects the test selection as entered on the MSS */
int32_t         gMCPIEnable = 0U;
int32_t         gTestSelection;
char            gSaveRestoreChoice;

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

/**************************************************************************
 ************************** Ctrl Unit Test Functions **********************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      The task is used to provide an execution context for the mmWave
 *      control task
 *
 *  @retval
 *      Not Applicable.
 */
static void Test_mmWaveCtrlTask(UArg arg0, UArg arg1)
{
    int32_t errCode;

    while (1)
    {
        /* Execute the mmWave control module: */
        if (MMWave_execute (gMMWaveHandle, &errCode) < 0)
            System_printf ("Error: mmWave control execution failed [Error code %d]\n", errCode);
    }
}

/**
 *  @b Description
 *  @n
 *      Registered event handler with the mmWave module to handle asynchronous events.
 *
 *  @param[in]  msgId
 *      Message Identifier
 *  @param[in]  sbId
 *      Subblock identifier
 *  @param[in]  sbLen
 *      Length of the subblock
 *  @param[in]  payload
 *      Pointer to the payload buffer
 *
 *  @retval
 *      Always return 0
 */
static int32_t Test_eventFxn(uint16_t msgId, uint16_t sbId, uint16_t sbLen, uint8_t *payload)
{
    System_printf ("Debug: BSS Event MsgId: %d [Sub Block Id: %d Sub Block Length: %d]\n",
                    msgId, sbId, sbLen);
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to test the mmWave module in Chirp Configuration Mode
 *
 *  @retval
 *      Not applicable
 */
static void Test_chirpConfigurationMode(void)
{
    MMWave_OpenCfg          openCfg;
    int32_t                 errCode;
    rlProfileCfg_t          profileCfg;
    rlChirpCfg_t            chirpCfg;
    rlBpmChirpCfg_t         bpmChirpCfg;
    MMWave_ChirpHandle      chirpHandle;
    MMWave_BpmChirpHandle   bpmChirpHandle;
    MMWave_CalibrationCfg   calibrationCfg;
    MMWave_CtrlCfg          ctrlCfg;

    /* Initialize the open configuration: */
    memset ((void*)&openCfg, 0, sizeof(MMWave_OpenCfg));

    /* Setup the calibration frequency: */
    openCfg.freqLimitLow  = 760U;
    openCfg.freqLimitHigh = 810U;

    /* Enable start/stop async events */
    openCfg.disableFrameStartAsyncEvent = false;
    openCfg.disableFrameStopAsyncEvent  = false;

    /* Initialize the low power mode configuration: */
    openCfg.lowPowerMode.lpAdcMode     = 1;

    /* Populate the ADC output configuration: */
    openCfg.adcOutCfg.fmt.b2AdcBits   = 2;
    openCfg.adcOutCfg.fmt.b2AdcOutFmt = 2;

    /* Populate the channel configuration: */
    openCfg.chCfg.rxChannelEn = 0xF;
    openCfg.chCfg.txChannelEn = 1;
    openCfg.chCfg.cascading   = 0;

    /* No custom calibration: */
    openCfg.useCustomCalibration        = false;
    openCfg.customCalibrationEnableMask = 0x0;

    /* Open the mmWave module: */
    if (MMWave_open (gMMWaveHandle, &openCfg, NULL, &errCode) < 0)
    {
        System_printf ("Error: Unable to get open the mmWave module [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("MMWave Open", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave Open", MCPI_TestResult_PASS);

    /* Initialize the control configuration: */
    memset ((void*)&ctrlCfg, 0, sizeof(MMWave_CtrlCfg));

    /* Populate the configuration: */
    ctrlCfg.dfeDataOutputMode = MMWave_DFEDataOutputMode_FRAME;

    /* Create a profile using the mmWave Full Exported API: */
    memset ((void*)&profileCfg, 0, sizeof(rlProfileCfg_t));

    /* Populate the profile configuration: */
    profileCfg.profileId             = 0;
    profileCfg.startFreqConst        = (uint32_t) ((float)77 * (1U << 26) / 3.6);
    profileCfg.idleTimeConst         = 20 * 1000 / 10;
    profileCfg.adcStartTimeConst     = 5 * 1000 / 10;
    profileCfg.rampEndTime           = 50 * 1000 /10;
    profileCfg.txOutPowerBackoffCode = 0;
    profileCfg.txPhaseShifter        = 0;
    profileCfg.freqSlopeConst        = 20 * 1000 / 48;
    profileCfg.txStartTime           = 1 * 1000 / 10;
    profileCfg.numAdcSamples         = 256;
    profileCfg.digOutSampleRate      = 6000;
    profileCfg.hpfCornerFreq1        = 0;
    profileCfg.hpfCornerFreq2        = 0;
    profileCfg.rxGain                = 30;

    /* Add the profile: */
    ctrlCfg.u.frameCfg.profileHandle[0] = MMWave_addProfile (gMMWaveHandle, &profileCfg, &errCode);
    if (ctrlCfg.u.frameCfg.profileHandle[0] == NULL)
    {
        System_printf ("Error: Unable to add the profile [Error code %d]\n", errCode);
        return;
    }

    /**************************************************************************************************
     * Unit Test: Verify the Full Configuration Profile API
     **************************************************************************************************/
    {
        rlProfileCfg_t          profileCfgTmp;
        uint32_t                numProfiles;
        MMWave_ProfileHandle    tmpProfileHandle;

        /* Verify the number of profiles */
        if (MMWave_getNumProfiles (gMMWaveHandle, &numProfiles, &errCode) < 0)
        {
            System_printf ("Error: Unable to get the number of profiles [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave Get Number Profile", MCPI_TestResult_FAIL);
            return;
        }
        if (numProfiles != 1U)
        {
            System_printf ("Error: Invalid number of profiles detected [%d]\n", numProfiles);
            MCPI_setFeatureTestResult ("MMWave Get Number Profile", MCPI_TestResult_FAIL);
        }
        MCPI_setFeatureTestResult ("MMWave Get Number Profile", MCPI_TestResult_PASS);

        /* Get the profile handle: */
        if (MMWave_getProfileHandle (gMMWaveHandle, profileCfg.profileId, &tmpProfileHandle, &errCode) < 0)
        {
            System_printf ("Error: Unable to get the profile handle [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave Get Profile Handle", MCPI_TestResult_FAIL);
        }
        if (tmpProfileHandle != ctrlCfg.u.frameCfg.profileHandle[0])
        {
            System_printf ("Error: Invalid profile handle detected\n");
            MCPI_setFeatureTestResult ("MMWave Get Profile Handle", MCPI_TestResult_FAIL);
        }
        MCPI_setFeatureTestResult ("MMWave Get Profile Handle", MCPI_TestResult_PASS);

        /* Get the profile configuration */
        if (MMWave_getProfileCfg (ctrlCfg.u.frameCfg.profileHandle[0], &profileCfgTmp, &errCode) < 0)
        {
            System_printf ("Error: Unable to get the profile configuration [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave Get Profile", MCPI_TestResult_FAIL);
            return;
        }
        if (memcmp ((void*)&profileCfg, (void*)&profileCfgTmp, sizeof(rlProfileCfg_t)) != 0)
        {
            System_printf ("Error: Invalid profile configuration detected\n");
            MCPI_setFeatureTestResult ("MMWave Get Profile", MCPI_TestResult_FAIL);
            return;
        }
        MCPI_setFeatureTestResult ("MMWave Get Profile", MCPI_TestResult_PASS);
    }

    /* Create a chirp using the mmWave Full Exported API: */
    memset ((void*)&chirpCfg, 0, sizeof(rlChirpCfg_t));

    /* Populate the chirp configuration: */
    chirpCfg.chirpStartIdx   = 0;
    chirpCfg.chirpEndIdx     = 0;
    chirpCfg.profileId       = 0;
    chirpCfg.startFreqVar    = 0;
    chirpCfg.freqSlopeVar    = 0;
    chirpCfg.idleTimeVar     = 0;
    chirpCfg.adcStartTimeVar = 0;
    chirpCfg.txEnable        = 1;

    /* Add the chirp to the existing profile */
    chirpHandle = MMWave_addChirp (ctrlCfg.u.frameCfg.profileHandle[0], &chirpCfg, &errCode);
    if (chirpHandle == NULL)
    {
        System_printf ("Error: Unable to add the chirp [Error code %d]\n", errCode);
        return;
    }

    /* Create the BPM cfg */
    memset ((void*)&bpmChirpCfg, 0, sizeof(rlBpmChirpCfg_t));

    /* Populate the configuration: */
    bpmChirpCfg.chirpStartIdx   = 0;
    bpmChirpCfg.chirpEndIdx     = 1;
    bpmChirpCfg.constBpmVal     = 2;

    /* Add config to the list: */
    bpmChirpHandle = MMWave_addBpmChirp (gMMWaveHandle, &bpmChirpCfg, &errCode);
    if (bpmChirpHandle == NULL)
    {
        System_printf ("Error: Unable to add BPM cfg [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("MMWave Add BPM cfg", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave Add BPM cfg", MCPI_TestResult_PASS);

    /**************************************************************************************************
     * Unit Test: Verify the Full Configuration Chirp API
     **************************************************************************************************/
    {
        rlChirpCfg_t        chirpCfgTmp;
        uint32_t            numChirps;
        MMWave_ChirpHandle  chirpHandleTmp;

        /* Get the number of chirps attached to the profile */
        if (MMWave_getNumChirps (ctrlCfg.u.frameCfg.profileHandle[0], &numChirps, &errCode) < 0)
        {
            System_printf ("Error: Unable to get the number of chirps [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave Get Number of Chirps", MCPI_TestResult_FAIL);
            return;
        }
        if (numChirps != 1U)
        {
            System_printf ("Error: Invalid number of chirps detected [%d]\n", numChirps);
            MCPI_setFeatureTestResult ("MMWave Get Number of Chirps", MCPI_TestResult_FAIL);
            return;
        }
        MCPI_setFeatureTestResult ("MMWave Get Number of Chirps", MCPI_TestResult_PASS);

        /* Get the Chirp Handle */
        if (MMWave_getChirpHandle (ctrlCfg.u.frameCfg.profileHandle[0], 1U, &chirpHandleTmp, &errCode) < 0)
        {
            System_printf ("Error: Unable to get the chirp handle [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave Get Chirp Handle", MCPI_TestResult_FAIL);
            return;
        }
        if (chirpHandleTmp != chirpHandle)
        {
            System_printf ("Error: Chirp handle validation failed [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave Get Chirp Handle", MCPI_TestResult_FAIL);
            return;
        }
        MCPI_setFeatureTestResult ("MMWave Get Chirp Handle", MCPI_TestResult_PASS);

        /* Get the chirp configuration */
        if (MMWave_getChirpCfg (chirpHandle, &chirpCfgTmp, &errCode) < 0)
        {
            System_printf ("Error: Unable to get the profile configuration [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave Get Chirp", MCPI_TestResult_FAIL);
            return;
        }
        if (memcmp ((void*)&chirpCfg, (void*)&chirpCfgTmp, sizeof(rlChirpCfg_t)) != 0)
        {
            System_printf ("Error: Invalid chirp configuration detected\n");
            MCPI_setFeatureTestResult ("MMWave Get Chirp Configuration", MCPI_TestResult_FAIL);
            return;
        }
        MCPI_setFeatureTestResult ("MMWave Get Chirp Configuration", MCPI_TestResult_PASS);
    }

    /**************************************************************************************************
     * Unit Test: Verify the BPM configuration API
     **************************************************************************************************/
    {
         uint32_t                numBpmChirps;
         MMWave_BpmChirpHandle   bpmChirpHandleTmp;
         rlBpmChirpCfg_t         bpmChirpCfgTmp;

         /* check BPM configuration */
         /* Get the number of BPM configurations */
         if (MMWave_getNumBpmChirp (gMMWaveHandle, &numBpmChirps, &errCode) < 0)
         {
             System_printf ("Error: Unable to get the number of BPM cfgs [Error code %d]\n", errCode);
             MCPI_setFeatureTestResult ("MMWave Get Number of BPM cfg", MCPI_TestResult_FAIL);
             return;
         }
         if (numBpmChirps != 1U)
         {
             System_printf ("Error: Invalid number of BPM cfgs detected [%d]\n", numBpmChirps);
             MCPI_setFeatureTestResult ("MMWave Get Number of BPM cfg", MCPI_TestResult_FAIL);
             return;
         }
         MCPI_setFeatureTestResult ("MMWave Get Number of BPM cfg", MCPI_TestResult_PASS);
         /* Get BPM cfg Handle */
         if (MMWave_getBpmChirpHandle (gMMWaveHandle, 1U, &bpmChirpHandleTmp, &errCode) < 0)
         {
             System_printf ("Error: Unable to get the BPM chirp handle [Error code %d]\n", errCode);
             MCPI_setFeatureTestResult ("MMWave Get BPM  Chirp Handle", MCPI_TestResult_FAIL);
             return;
         }
         if (bpmChirpHandleTmp != bpmChirpHandle)
         {
             System_printf ("Error: BPM Chirp handle validation failed [Error code %d]\n", errCode);
             MCPI_setFeatureTestResult ("MMWave Get BPM Chirp Handle", MCPI_TestResult_FAIL);
             return;
         }
         MCPI_setFeatureTestResult ("MMWave Get BPM Chirp Handle", MCPI_TestResult_PASS);

         /* Get the chirp configuration */
         if (MMWave_getBpmChirpCfg (bpmChirpHandle, &bpmChirpCfgTmp, &errCode) < 0)
         {
             System_printf ("Error: Unable to get the BPM configuration [Error code %d]\n", errCode);
             MCPI_setFeatureTestResult ("MMWave Get BPM configuration", MCPI_TestResult_FAIL);
             return;
         }
         if (memcmp ((void*)&bpmChirpCfg, (void*)&bpmChirpCfgTmp, sizeof(rlBpmChirpCfg_t)) != 0)
         {
             System_printf ("Error: Invalid BPM chirp configuration detected\n");
             MCPI_setFeatureTestResult ("MMWave Get BPM Chirp Configuration", MCPI_TestResult_FAIL);
             return;
         }
         MCPI_setFeatureTestResult ("MMWave Get BPM Chirp Configuration", MCPI_TestResult_PASS);
    }

    /* Populate the frame configuration: */
    ctrlCfg.u.frameCfg.frameCfg.chirpStartIdx      = 0;
    ctrlCfg.u.frameCfg.frameCfg.chirpEndIdx        = 0;
    ctrlCfg.u.frameCfg.frameCfg.numLoops           = 128;
    ctrlCfg.u.frameCfg.frameCfg.numFrames          = 1;
    ctrlCfg.u.frameCfg.frameCfg.numAdcSamples      = 256;
    ctrlCfg.u.frameCfg.frameCfg.framePeriodicity   = 20 * 1000000 / 5;
    ctrlCfg.u.frameCfg.frameCfg.triggerSelect      = 1;
    ctrlCfg.u.frameCfg.frameCfg.frameTriggerDelay  = 0;

    /* Configure the mmWave control: */
    if (MMWave_config (gMMWaveHandle, &ctrlCfg, &errCode) < 0)
    {
        /* Error: Unable to configure the mmWave */
        System_printf ("Error: mmWave Configuration Failed [Error code %d]\n", errCode);

        /* Log the failure into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave MSS Configuration", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave MSS Configuration", MCPI_TestResult_PASS);

    /* Initialize the calibration configuration: */
    memset ((void *)&calibrationCfg, 0, sizeof(MMWave_CalibrationCfg));

    /* Populate the calibration configuration: */
    calibrationCfg.dfeDataOutputMode = MMWave_DFEDataOutputMode_FRAME;
    calibrationCfg.u.chirpCalibrationCfg.enableCalibration    = true;
    calibrationCfg.u.chirpCalibrationCfg.enablePeriodicity    = true;
    calibrationCfg.u.chirpCalibrationCfg.periodicTimeInFrames = 10U;

    /* Start the mmWave control module: The configuration has been applied successfully. */
    if (MMWave_start (gMMWaveHandle, &calibrationCfg, &errCode) < 0)
    {
        /* Error: Unable to start the mmWave control */
        System_printf ("Error: mmWave Control Start failed [Error code %d]\n", errCode);

        /* Log the failure into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave MSS Start", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave MSS Start", MCPI_TestResult_PASS);

    /* Stop the mmWave control module: */
    if (MMWave_stop (gMMWaveHandle, &errCode) < 0)
    {
        /* Error: Unable to stop the mmWave control */
        System_printf ("Error: mmWave Control Stop failed [Error code %d]\n", errCode);

        /* Log the failure into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave MSS Stop", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave MSS Stop", MCPI_TestResult_PASS);

    /* Close the mmWave control module: */
    if (MMWave_close (gMMWaveHandle, &errCode) < 0)
    {
        /* Error: Unable to stop the mmWave control */
        System_printf ("Error: mmWave Close failed [Error code %d]\n", errCode);

        /* Log the failure into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave MSS Close", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave MSS Close", MCPI_TestResult_PASS);

    /* Deinitialize the mmWave control module: */
    if (MMWave_deinit (gMMWaveHandle, &errCode) < 0)
    {
        /* Error: Unable to deinitialize the mmWave control */
        System_printf ("Error: mmWave Control Deinit failed [Error code %d]\n", errCode);

        /* Log the failure into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave MSS Deinit", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave MSS Deinit", MCPI_TestResult_PASS);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to test the mmWave module and save/restore
 *      the calibration data.
 *
 *  @retval
 *      Not applicable
 */
static void Test_calibDataSaveRestore(void)
{
    MMWave_OpenCfg      openCfg;
    char                inputString[3U];
    rlCalibrationData_t calibrationData;
    FILE*               fCalibDataFile;
    uint8_t*            ptrCalibData;
    uint32_t            sizeCalibData;
    rlReturnVal_t       linkRetVal;
    int32_t             errCode;

    /* User Input: */
    System_printf ("*****************************************************\n");
    System_printf ("Are we Saving(S) or Restoring(R) the Calibration Data\n");
    System_printf ("*****************************************************\n");
    System_printf ("Enter your choice: ");
    if (gMCPIEnable == 0) /* Run test from CCS */
    {
        fgets (inputString, 3U, stdin);
        gSaveRestoreChoice = inputString[0];
    }

    /* Initialize the open configuration: */
    memset ((void*)&openCfg, 0, sizeof(MMWave_OpenCfg));

    /* Setup the calibration frequency: */
    openCfg.freqLimitLow  = 760U;
    openCfg.freqLimitHigh = 810U;

    /* Enable start/stop async events */
    openCfg.disableFrameStartAsyncEvent = false;
    openCfg.disableFrameStopAsyncEvent  = false;

    /* Initialize the low power mode configuration: */
    openCfg.lowPowerMode.lpAdcMode     = 1;

    /* Populate the ADC output configuration: */
    openCfg.adcOutCfg.fmt.b2AdcBits   = 2;
    openCfg.adcOutCfg.fmt.b2AdcOutFmt = 2;

    /* Populate the channel configuration: */
    openCfg.chCfg.rxChannelEn = 0xF;
    openCfg.chCfg.txChannelEn = 1;
    openCfg.chCfg.cascading   = 0;

    /* Processing is done on the user choice: */
    if (gSaveRestoreChoice == 'R')
    {
        /********************************************************************
         * Restoring the Calibration Data:
         * - Read the calibration data from the file
         ********************************************************************/
        fCalibDataFile = fopen ("XWR14xxCalibrationData.bin", "rb");
        if (fCalibDataFile == NULL)
        {
            System_printf ("Error: Unable to open the file to restore the calibration data\n");
            MCPI_setFeatureTestResult ("MMWave Restore Calibration Data", MCPI_TestResult_FAIL);
            return;
        }

        /* Cycle through and write all the data into the file: */
        sizeCalibData = sizeof(calibrationData);
        ptrCalibData  = (uint8_t*)&calibrationData;
        while (sizeCalibData > 0U)
        {
            /* Read each byte at a time: */
            if (fread (ptrCalibData, 1U, sizeof(uint8_t), fCalibDataFile) != 1U)
            {
                System_printf ("Error: Unable to write to the file to save the calibration data\n");
                MCPI_setFeatureTestResult ("MMWave Restore Calibration Data", MCPI_TestResult_FAIL);
                fclose (fCalibDataFile);
                return;
            }

            /* Skip to the next set of data: */
            ptrCalibData++;
            sizeCalibData--;
        }
        MCPI_setFeatureTestResult ("MMWave Restore Calibration Data", MCPI_TestResult_PASS);

        /* Update the "default" open configuration:
         * - We are using custom calibration with a custom enable mask
         * - Pass the pointer to the restore calibration data */
        openCfg.useCustomCalibration            = true;
        openCfg.customCalibrationEnableMask     = 0x0010;

        /* Close the file: */
        fclose (fCalibDataFile);

        /********************************************************************
         * Restore the Calibration data:
         * - Pass the calibration data which has been read from the file
         ********************************************************************/
        if (MMWave_open (gMMWaveHandle, &openCfg, &calibrationData, &errCode) < 0)
        {
            /* Error: Unable to configure the mmWave control module */
            System_printf ("Error: mmWave open failed [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave MSS Open", MCPI_TestResult_FAIL);
            return;
        }
        MCPI_setFeatureTestResult ("MMWave MSS Open", MCPI_TestResult_PASS);
    }
    else
    {
        /********************************************************************
         * Saving the Calibration data:
         * - Use the default configuration and let the mmWave module perform
         *   all the calibrations.
         ********************************************************************/
         openCfg.useCustomCalibration        = false;
         openCfg.customCalibrationEnableMask = 0x0;

        /* Open the mmWave Module: */
        if (MMWave_open (gMMWaveHandle, &openCfg, NULL, &errCode) < 0)
        {
            /* Error: Unable to configure the mmWave control module */
            System_printf ("Error: mmWave open failed [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave MSS Open", MCPI_TestResult_FAIL);
            return;
        }
        MCPI_setFeatureTestResult ("MMWave MSS Open", MCPI_TestResult_PASS);
    }

    /* Processing is done on the user choice: */
    if (gSaveRestoreChoice == 'R')
    {
        /********************************************************************
         * Restore the Calibration data:
         * - There is nothing to be done here
         * - The calibration data has already been passed to the MMWave
         *   module.
         ********************************************************************/
    }
    else
    {
        /********************************************************************
         * Saving the Calibration data:
         * - The MMWave module has been opened succesfully.
         * - Use the Link API to get the calibration data
         * - Save the calibration data into a file.
         ********************************************************************/
        linkRetVal = rlRfCalibDataStore(RL_DEVICE_MAP_INTERNAL_BSS, &calibrationData);
        if (linkRetVal != RL_RET_CODE_OK)
        {
            System_printf ("Error: Unable to save the calibration data [Error: %d]\n", linkRetVal);
            MCPI_setFeatureTestResult ("MMWave Save Calibration Data", MCPI_TestResult_FAIL);
            return;
        }

        /* Open the file in which the calibration data is to be written: */
        fCalibDataFile = fopen ("XWR14xxCalibrationData.bin", "wb");
        if (fCalibDataFile == NULL)
        {
            System_printf ("Error: Unable to open the file to save the calibration data\n");
            MCPI_setFeatureTestResult ("MMWave Save Calibration Data", MCPI_TestResult_FAIL);
            return;
        }

        /* Cycle through and write all the data into the file: */
        sizeCalibData = sizeof(calibrationData);
        ptrCalibData  = (uint8_t*)&calibrationData;
        while (sizeCalibData > 0U)
        {
            /* Write each byte at a time: */
            if (fwrite (ptrCalibData, 1U, sizeof(uint8_t), fCalibDataFile) != 1U)
            {
                System_printf ("Error: Unable to write to the file to save the calibration data\n");
                MCPI_setFeatureTestResult ("MMWave Save Calibration Data", MCPI_TestResult_FAIL);
                fclose (fCalibDataFile);
                return;
            }

            /* Skip to the next set of data: */
            ptrCalibData++;
            sizeCalibData--;
        }
        MCPI_setFeatureTestResult ("MMWave Save Calibration Data", MCPI_TestResult_PASS);
        fclose (fCalibDataFile);
    }

    /************************************************************************
     * Close the mmWave:
     ************************************************************************/
    if (MMWave_close (gMMWaveHandle, &errCode) < 0)
    {
        /* Error: Unable to configure the mmWave control module */
        System_printf ("Error: mmWave close failed [Error code %d]\n", errCode);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave MSS Close", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave MSS Close", MCPI_TestResult_PASS);
    return;
}

/**
 *  @b Description
 *  @n
 *      The task uses the mmWave configuration API to configure the mmWave Link and
 *      the BSS
 *
 *  @retval
 *      Not Applicable.
 */
static void Test_mmWaveConfigTask(UArg arg0, UArg arg1)
{
    int32_t     gTestSelection;
    char        inputString[3U];

    System_printf ("*************************************************\n");
    System_printf ("Enter the test to execute:\n");
    System_printf ("1. MSS executes the mmWave link: Chirp Configuration Mode\n");
    System_printf ("2. MSS Calibration Data Save/Restore\n");
    System_printf ("*************************************************\n");
    System_printf ("Enter your choice: ");
    if (gMCPIEnable == 0)/* Run test from CCS */
    {
        fgets (inputString, 3U, stdin);
        gTestSelection = inputString[0] - '0';
    }

    if (gTestSelection == 1U)
    {
        /* Execute the mmWave Module in Chirp Configuration: */
        Test_chirpConfigurationMode();
    }
    else
    {
        /* Execute the mmWave Calibration Data Save & Restore: */
        Test_calibDataSaveRestore();
    }

    /* MCPI Logger Test Completion */
    MCPI_setTestResult ();

    /* Exit BIOS */
    BIOS_exit(0);
    return;
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
static void Test_initTask(UArg arg0, UArg arg1)
{
    int32_t             errCode;
    MMWave_InitCfg      initCfg;
    Task_Params         taskParams;

    /* Debug Message: */
    System_printf("Debug: Launched the Initialization Task\n");

    /*****************************************************************************
     * Initialize the mmWave SDK components:
     *****************************************************************************/

    /* Initialize the test logger framework: */
    MCPI_Initialize ();

    /* Initialize the Mailbox */
    Mailbox_init(MAILBOX_TYPE_MSS);

    /* Initialize the mmWave control init configuration */
    memset ((void*)&initCfg, 0 , sizeof(MMWave_InitCfg));

    /* Populate the init configuration: */
    initCfg.domain                  = MMWave_Domain_MSS;
    initCfg.socHandle               = socHandle;
    initCfg.eventFxn                = Test_eventFxn;
    initCfg.linkCRCCfg.useCRCDriver = 1U;
    initCfg.linkCRCCfg.crcChannel   = CRC_Channel_CH1;
    initCfg.cfgMode                 = MMWave_ConfigurationMode_FULL;

    /* Initialize and setup the mmWave Control module */
    gMMWaveHandle = MMWave_init (&initCfg, &errCode);
    if (gMMWaveHandle == NULL)
    {
        /* Error: Unable to initialize the mmWave control module */
        System_printf ("Error: MMWave MSS Initialization Failed [Error code %d]\n", errCode);

        /* Log the failure into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave MSS Initialization", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave MSS Initialization", MCPI_TestResult_PASS);

    /* Synchronization: This will synchronize the execution of the control module
     * between the domains. This is a prerequiste and always needs to be invoked. */
    if (MMWave_sync (gMMWaveHandle, &errCode) < 0)
    {
        /* Error: Unable to synchronize the mmWave control module */
        System_printf ("Error: mmWave Control Synchronization failed [Error code %d]\n", errCode);

        /* Log the failure into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave MSS Synchronization", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave MSS Synchronization", MCPI_TestResult_PASS);

    /*****************************************************************************
     * Launch the mmWave control execution task
     * - This should have a higher priroity than any other task which uses the
     *   mmWave control API
     *****************************************************************************/
    Task_Params_init(&taskParams);
    taskParams.priority = 4;
    Task_create(Test_mmWaveCtrlTask, &taskParams, NULL);

    /*****************************************************************************
     * Launch the mmWave configuration task
     *****************************************************************************/
    Task_Params_init(&taskParams);
    taskParams.priority  = 2;
    taskParams.stackSize = 8*1024;
    Task_create(Test_mmWaveConfigTask, &taskParams, NULL);

    return;
}

/**
 *  @b Description
 *  @n
 *      Entry point into the mmWave Control Unit Test
 *
 *  @retval
 *      Not Applicable.
 */
int main (void)
{
    Task_Params     taskParams;
    SOC_Cfg         socCfg;
    int32_t         errCode;

    /* Initialize the ESM */
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
    System_printf ("**********************************************\n");
    System_printf ("Debug: Launching the mmWave Control Unit Test\n");
    System_printf ("**********************************************\n");

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    Task_create(Test_initTask, &taskParams, NULL);

    /* Start BIOS */
    BIOS_start();
    return 0;
}

