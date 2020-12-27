/*******************************************************************************
 * @file    mss_cli.c
 * @brief   MSS Minimal CLI Implementation
 * @author  A. Astro, a.astro7x@gmail.com
 * @date    Jan 13 2020
 * @version 0.1
 *******************************************************************************/

/*(*)******************************************************************************
 *	 INCLUDE FILES
 **********************************************************************************/

/* Standard Include Files. */
#include <common/app_cfg.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

/* BIOS/XDC Include Files. */
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/heaps/HeapBuf.h>
#include <ti/sysbios/heaps/HeapMem.h>
#include <ti/sysbios/knl/Event.h>

/* mmWave SDK Include Files: */
#include <ti/common/sys_common.h>
#include <ti/drivers/uart/UART.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/control/mmwavelink/mmwavelink.h>
#include <ti/utils/cli/cli.h>

/* Application Include Files: */
#include "common/mmWave_XSS.h"


/*(*)******************************************************************************
 *	 MSS CLI SPECIFIC Functions
 **********************************************************************************/

static int32_t MSS_CLISensorStart (int32_t argc, char* argv[]);
static int32_t MSS_CLISensorStop (int32_t argc, char* argv[]);
static int32_t MSS_CLIBasicCfg (int32_t argc, char* argv[]);
static int32_t MSS_CLIAdvancedFrameCfg (int32_t argc, char* argv[]);


/**
 *  @brief  This is the CLI Handler for starting the sensor
 *  @arg[0] Number of arguments
 *  @arg[1] Arguments
 *  @retval Success 0
 *  @retval Error   -1
 */
static int32_t MSS_CLISensorStart (int32_t argc, char* argv[])
{
    MMWave_CalibrationCfg   calibrationCfg;
    int32_t                 errCode;
    
    if (gMCB.runningStatus == true) {

        /* Already running. */
        return 0; 
    }
    /* The sensor can only be started; if the link has been configured */
    if (gMCB.cfgStatus == true) {

        /* Initialize the calibration configuration: */
        memset ((void *)&calibrationCfg, 0, sizeof(MMWave_CalibrationCfg));

        /* Populate the calibration configuration: */
        calibrationCfg.dfeDataOutputMode                          = 
        MMWave_DFEDataOutputMode_ADVANCED_FRAME;
        calibrationCfg.u.chirpCalibrationCfg.enableCalibration    = true;
        calibrationCfg.u.chirpCalibrationCfg.enablePeriodicity    = true;
        calibrationCfg.u.chirpCalibrationCfg.periodicTimeInFrames = 10U;

        System_printf ("[MSS] \t [DEBUG] Sensor will start momentarily. \n");

        /* Start the mmWave: */
        if (MMWave_start (gMCB.ctrlHandle, &calibrationCfg, &errCode) < 0) {

            /* Error: Unable to start the mmWave control module */
            System_printf ("[MSS] \t [ERROR] mmWave start failed [Error code %d]\n", errCode);
            return -1;
        }
        
        gMCB.runningStatus = true;
        return 0;
    }
    else {
        /* Invalid CLI use case; doing a sensor start without executing the basic or advanced configuration
         * command. Inform the user and return an error code. */
        System_printf ("[MSS] \t [ERROR] Please ensure that the XXXCfg CLI command is invoked before starting the sensor\n");
        return -1;
    }
}

/**
 *  @brief  This is the CLI Handler for stopping the sensor
 *  @arg[0] Number of arguments
 *  @arg[1] Arguments
 *  @retval Success 0
 *  @retval Error   -1
 */
static int32_t MSS_CLISensorStop (int32_t argc, char* argv[])
{
    int32_t errCode;

    if (gMCB.runningStatus == false) {
        return 0; // Already stopped. 
    }

    /* The sensor can only be stopped; if the link has been configured */
    if (gMCB.cfgStatus == true) {
        /* Stop the sensor */
        if (MMWave_stop (gMCB.ctrlHandle, &errCode) < 0) {
            /* Error: Unable to stop the mmWave control module */
            System_printf ("Error: mmWave stop failed [Error code %d]\n", errCode);
            return -1;
        }
        System_printf ("Debug: Sensor has been stopped\n");

        /* The link is no longer configured. */
        gMCB.runningStatus = true; 
        return 0;
    }
    else {
        /* Invalid CLI use case; doing a sensor stop multiple times. Inform the user and return an error code. */
        System_printf ("Error: Sensor has already been stopped. Reconfigure and start the sensor if required\n");
        return -1;
    }
}

/**
 *  @brief  This is the CLI Handler for basic configuration
 *  @arg[0] Number of arguments
 *  @arg[1] Arguments
 *  @retval Success 0
 *  @retval Error   -1
 */
static int32_t MSS_CLIBasicCfg (int32_t argc, char* argv[]) {

    MMWave_OpenCfg          openCfg;
    int32_t                 errCode;
    rlProfileCfg_t          profileCfg;
    rlChirpCfg_t            chirpCfg;
    rlFrameCfg_t            frameCfg;
    int32_t                 retVal;

    if (gMCB.cfgStatus == true) {
        /* Radar has already been configured. */
        return 0;
    }

    /* Setup the calibration frequency: */
    openCfg.freqLimitLow  = 760U;
    openCfg.freqLimitHigh = 810U;
    openCfg.defaultAsyncEventHandler = MMWave_DefaultAsyncEventHandler_MSS;
    
    /* Initialize the minimal configuration: */
    Cfg_ChannelCfgInitParams  (&openCfg.chCfg);
    Cfg_LowPowerModeInitParams(&openCfg.lowPowerMode);
    Cfg_ADCOutCfgInitParams   (&openCfg.adcOutCfg);

    /* Open the mmWave module: */
    if (MMWave_open (gMCB.ctrlHandle, &openCfg, NULL, &errCode) < 0) {
        System_printf ("[MSS] \t  [ERROR] mmWave open configuration failed [Error code %d]\n", errCode);
        return -1;
    }

    /********************************************************************************
     * MMWave Link and BSS is operational now. In minimal mode we have access to all
     * the mmWave Link API to perform the configuration
     *
     * Profile configuration:
     ********************************************************************************/
    Cfg_ProfileCfgInitParams (0U, &profileCfg);
    retVal = rlSetProfileConfig (RL_DEVICE_MAP_INTERNAL_BSS, 1U, &profileCfg);
    if (retVal != RL_RET_CODE_OK) {
        System_printf ("[MSS] \t [ERROR] Unable to configure the profile [Error %d]\n", retVal);
        return -1;
    }

    /********************************************************************************
     * Chirp configuration:
     ********************************************************************************/
    Cfg_ChirpCfgInitParams (0U, &chirpCfg);
    retVal = rlSetChirpConfig(RL_DEVICE_MAP_INTERNAL_BSS, 1U, &chirpCfg);

    if (retVal != RL_RET_CODE_OK) {
        System_printf ("[MSS] \t [ERROR] Unable to configure the chirp [Error %d]\n", retVal);
        return -1;
    }

    /********************************************************************************
     * Frame configuration:
     ********************************************************************************/
    Cfg_FrameCfgInitParams (&frameCfg);
    retVal = rlSetFrameConfig(RL_DEVICE_MAP_INTERNAL_BSS, &frameCfg);
    if (retVal != RL_RET_CODE_OK) {
        System_printf ("[MSS] \t [ERROR] Unable to configure the frame [Error %d]\n", retVal);
        return -1;
    }

    /* The link has been configured. */
    gMCB.cfgStatus = true;
    System_printf ("[MSS] \t [DEBUG] Basic configuration completed. Start the sensor...\n");
    return 0;
}

/**
 *  @brief  This is the CLI Handler for advanced frame configuration
 *  @arg[0] Number of arguments
 *  @arg[1] Arguments
 *  @retval Success 0
 *  @retval Error   -1
 */

static int32_t MSS_CLIAdvancedFrameCfg (int32_t argc, char* argv[])
{
    MMWave_OpenCfg          openCfg;
    int32_t                 errCode;
    rlProfileCfg_t          profileCfg;
    rlChirpCfg_t            chirpCfg;
    rlAdvFrameCfg_t         advFrameCfg;
    int32_t                 retVal;
    int32_t                 indx;
    rlRfLdoBypassCfg_t rfLdoBypassCfg[1] = {0};
    
    if (gMCB.cfgStatus == true) {
        /* Radar has already been configured. */
        return 0;
    }

    /* Setup the calibration frequency: */
    openCfg.freqLimitLow  = 760U;
    openCfg.freqLimitHigh = 810U;
    openCfg.defaultAsyncEventHandler = MMWave_DefaultAsyncEventHandler_MSS;
    
    /* Initialize the minimal configuration: */
    Cfg_ChannelCfgInitParams  (&openCfg.chCfg);
    Cfg_LowPowerModeInitParams(&openCfg.lowPowerMode);
    Cfg_ADCOutCfgInitParams   (&openCfg.adcOutCfg);

    /* Open the mmWave module: */
    if (MMWave_open (gMCB.ctrlHandle, &openCfg, NULL, &errCode) < 0) {
        System_printf ("[MSS] \t [ERROR] mmWave open configuration failed [Error code %d]\n", errCode);
        return -1;
    }
    System_printf ("Set LDO Bypass\n");
    rfLdoBypassCfg->ldoBypassEnable = 0x03;
    retVal = rlRfSetLdoBypassConfig(RL_DEVICE_MAP_INTERNAL_BSS, (rlRfLdoBypassCfg_t*)rfLdoBypassCfg);

    if (retVal != RL_RET_CODE_OK) {
        System_printf ("[MSS] \t [ERROR] LDO Bypass config failed [Error %d]\n", retVal);
        return -1;
    }
    
        
    /********************************************************************************
     * MMWave Link and BSS is operational now. In minimal mode we have access to all
     * the mmWave Link API to perform the configuration
     *
     * Profile configurations:
     ********************************************************************************/
    for (indx = 0; indx < NUM_PROFILES; indx++) {
        
        Cfg_ProfileCfgInitParams (indx, &profileCfg);
        retVal = rlSetProfileConfig (RL_DEVICE_MAP_INTERNAL_BSS, 1U, &profileCfg);
        if (retVal != RL_RET_CODE_OK) {
            System_printf ("[MSS] \t [ERROR] Unable to configure the profile %d [Error %d]\n", indx, retVal);
            return -1;
        }
    }
    
    /********************************************************************************
     * Chirp configurations:
     ********************************************************************************/
    for (indx = 0; indx < NUM_CHIRP_PROG; indx++) {

        Cfg_ChirpCfgInitParams (indx, &chirpCfg);
        retVal = rlSetChirpConfig(RL_DEVICE_MAP_INTERNAL_BSS, 1U, &chirpCfg);
        if (retVal != RL_RET_CODE_OK) {

            System_printf ("Error: Unable to configure chirp %d [Error %d]\n", indx, retVal);
            return -1;
        }
    }
    
    /* Advanced Frame configuration. */
    Cfg_AdvFrameCfgInitParams (&advFrameCfg);
    retVal = rlSetAdvFrameConfig(RL_DEVICE_MAP_INTERNAL_BSS, &advFrameCfg);
    if (retVal != RL_RET_CODE_OK)
    {
        System_printf ("[MSS]\t [ERROR] Advanced Frame configuration failed [Error %d]\n", retVal);
        return -1;
    }
    
    /* The link has been configured. */
    gMCB.cfgStatus = true;
    System_printf ("[MSS] \t [DEBUG] MMWave has been configured.\n");
    return 0;
}

/**
 *  @brief  This is the CLI Execution Task
 *  @retval N/A
 */

void MSS_CLIInit (void) {

	CLI_Cfg			cliCfg;
    static char*	dummy[1];

    /* Initialize the CLI configuration: */
    memset((void *)&cliCfg, 0, sizeof(CLI_Cfg));

    /* Populate the CLI configuration: */
    cliCfg.cliPrompt                    = "mmWAVE FMCW Radar Design:/>";
    cliCfg.cliUartHandle                = gMCB.commandUartHandle;
    cliCfg.taskPriority                 = 3;
    cliCfg.mmWaveHandle                 = gMCB.ctrlHandle;
    cliCfg.enableMMWaveExtension        = 0U;
    cliCfg.usePolledMode                = true;
    cliCfg.tableEntry[0].cmd            = "basicCfg";
    cliCfg.tableEntry[0].helpString     = "Basic Cfg [Hardcoded Parameters]";
    cliCfg.tableEntry[0].cmdHandlerFxn  = MSS_CLIBasicCfg;

    cliCfg.tableEntry[1].cmd            = "advFrameCfg";
    cliCfg.tableEntry[1].helpString     = "Advanced Frame Cfg [Hardcoded Parameters]";
    cliCfg.tableEntry[1].cmdHandlerFxn  = MSS_CLIAdvancedFrameCfg;

    cliCfg.tableEntry[2].cmd            = "sensorStart";
    cliCfg.tableEntry[2].helpString     = "Start the sensor; ensure that the configuration is completed";
    cliCfg.tableEntry[2].cmdHandlerFxn  = MSS_CLISensorStart;

    cliCfg.tableEntry[3].cmd            = "sensorStop";
    cliCfg.tableEntry[3].helpString     = "Stop the sensor";
    cliCfg.tableEntry[3].cmdHandlerFxn  = MSS_CLISensorStop;

    #if 0
    /* Open the CLI: */
    if (CLI_open (&cliCfg) < 0)
    {
        System_printf ("[MSS] \t [ERROR] Unable to open the CLI\n");
        return;
    }
    System_printf ("[MSS] \t [DEBUG] CLI is operational\n");
    #endif
    
    /* The link is not configured. */
    gMCB.cfgStatus = false;
    gMCB.runningStatus = false;
    gMCB.isMMWaveOpen = false;
    
    MSS_CLIAdvancedFrameCfg(1, dummy);
    
    MSS_CLISensorStart(2, dummy);
    return;
}


