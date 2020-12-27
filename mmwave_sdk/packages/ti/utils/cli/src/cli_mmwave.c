/*
 *   @file  cli_mmwave.c
 *
 *   @brief
 *      CLI Extension which handles the mmWave configuration
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

/* mmWave SDK Include Files: */
#include <ti/common/sys_common.h>
#include <ti/common/mmwave_sdk_version.h>
#include <ti/drivers/uart/UART.h>
#include <ti/control/mmwavelink/mmwavelink.h>
#include <ti/control/mmwave/mmwave.h>
#include <ti/utils/cli/cli.h>
#include <ti/utils/cli/include/cli_internal.h>

/**************************************************************************
 ******************* CLI mmWave Extension Local Definitions****************
 **************************************************************************/

/*This is used in several formulas that translate CLI input to mmwavelink units.
  It must be defined as double to achieve the correct precision
  on the formulas (if defined as float there will be small precision errors
  that may result in the computed value being out of mmwavelink range if the 
  CLI input is a borderline value).

  The vaiable is initialized in @ref CLI_MMWaveExtensionInit() */
double gCLI_mmwave_freq_scale_factor;


/**************************************************************************
 ******************* CLI mmWave Extension Local Functions *****************
 **************************************************************************/

/* CLI Command Functions */
static int32_t CLI_MMWaveVersion (int32_t argc, char* argv[]);
static int32_t CLI_MMWaveFlushCfg (int32_t argc, char* argv[]);
static int32_t CLI_MMWaveDataOutputMode (int32_t argc, char* argv[]);
static int32_t CLI_MMWaveChannelCfg (int32_t argc, char* argv[]);
static int32_t CLI_MMWaveADCCfg (int32_t argc, char* argv[]);
static int32_t CLI_MMWaveProfileCfg (int32_t argc, char* argv[]);
static int32_t CLI_MMWaveChirpCfg (int32_t argc, char* argv[]);
static int32_t CLI_MMWaveFrameCfg (int32_t argc, char* argv[]);
static int32_t CLI_MMWaveAdvFrameCfg (int32_t argc, char* argv[]);
static int32_t CLI_MMWaveSubFrameCfg (int32_t argc, char* argv[]);
static int32_t CLI_MMWaveLowPowerCfg (int32_t argc, char* argv[]);
static int32_t CLI_MMWaveContModeCfg (int32_t argc, char* argv[]);
static int32_t CLI_MMWaveBPMCfgAdvanced (int32_t argc, char* argv[]);

#undef CLI_MMWAVE_HELP_SUPPORT

/**************************************************************************
 ************************ CLI mmWave Extension Globals ********************
 **************************************************************************/

/**
 * @brief
 *  This is the mmWave extension table added to the CLI.
 */
CLI_CmdTableEntry gCLIMMWaveExtensionTable[] =
{
    {
        "version",
#ifdef CLI_MMWAVE_HELP_SUPPORT
        "No arguments",
#else
        NULL,
#endif
        CLI_MMWaveVersion
    },
    {
        "flushCfg",
#ifdef CLI_MMWAVE_HELP_SUPPORT
        "No arguments",
#else
        NULL,
#endif
        CLI_MMWaveFlushCfg
    },
    {
        "dfeDataOutputMode",
#ifdef CLI_MMWAVE_HELP_SUPPORT
        "<modeType>   1-Chirp and 2-Continuous",
#else
        NULL,
#endif
        CLI_MMWaveDataOutputMode
    },
    {
        "channelCfg",
#ifdef CLI_MMWAVE_HELP_SUPPORT
        "<rxChannelEn> <txChannelEn> <cascading>",
#else
        NULL,
#endif
        CLI_MMWaveChannelCfg
    },
    {
        "adcCfg",
#ifdef CLI_MMWAVE_HELP_SUPPORT
        "<numADCBits> <adcOutputFmt>",
#else
        NULL,
#endif
        CLI_MMWaveADCCfg
    },
    {
        "profileCfg",
#ifdef CLI_MMWAVE_HELP_SUPPORT
        "<profileId> <startFreq> <idleTime> <adcStartTime> <rampEndTime> <txOutPower> <txPhaseShifter> <freqSlopeConst> <txStartTime> <numAdcSamples> <digOutSampleRate> <hpfCornerFreq1> <hpfCornerFreq2> <rxGain>",
#else
        NULL,
#endif
        CLI_MMWaveProfileCfg
    },
    {
        "chirpCfg",
#ifdef CLI_MMWAVE_HELP_SUPPORT
        "<startIdx> <endIdx> <profileId> <startFreqVar> <freqSlopeVar> <idleTimeVar> <adcStartTimeVar> <txEnable>",
#else
        NULL,
#endif
        CLI_MMWaveChirpCfg
    },
    {
        "frameCfg",
#ifdef CLI_MMWAVE_HELP_SUPPORT
        "<chirpStartIdx> <chirpEndIdx> <numLoops> <numFrames> <framePeriodicity> <triggerSelect> <frameTriggerDelay>",
#else
        NULL,
#endif
        CLI_MMWaveFrameCfg
    },
    {
        "advFrameCfg",
#ifdef CLI_MMWAVE_HELP_SUPPORT
        "<numOfSubFrames> <forceProfile> <numFrames> <triggerSelect> <frameTrigDelay>",
#else
        NULL,
#endif
        CLI_MMWaveAdvFrameCfg
    },
    {
        "subFrameCfg",
#ifdef CLI_MMWAVE_HELP_SUPPORT
        "<subFrameNum> <forceProfileIdx> <chirpStartIdx> <numOfChirps> <numLoops> <burstPeriodicity> <chirpStartIdxOffset> <numOfBurst> <numOfBurstLoops> <subFramePeriodicity>",
#else
        NULL,
#endif
        CLI_MMWaveSubFrameCfg
    },
    {
        "lowPower",
#ifdef CLI_MMWAVE_HELP_SUPPORT
        "<reserved> <lpAdcMode>",
#else
        NULL,
#endif
        CLI_MMWaveLowPowerCfg
    },
    {
        "contModeCfg",
#ifdef CLI_MMWAVE_HELP_SUPPORT
        "<startFreq> <txOutPower> <txPhaseShifter> <digOutSampleRate> <hpfCornerFreq1> <hpfCornerFreq2> <rxGain> <reserved> <numSamples>",
#else
        NULL,
#endif
        CLI_MMWaveContModeCfg,
    },
    {
        "bpmCfgAdvanced",
#ifdef CLI_MMWAVE_HELP_SUPPORT
        "<chirpStartIdx> <chirpEndIdx> <constBpmVal>",
#else
        NULL,
#endif
        CLI_MMWaveBPMCfgAdvanced
    },
    {
        NULL,
        NULL,
        NULL
    }
};

/**
 * @brief
 *  Global MMWave configuration tracked by the module.
 */
MMWave_CtrlCfg      gCLIMMWaveControlCfg;

/**
 * @brief
 *  Global MMWave open configuration tracked by the module.
 */
MMWave_OpenCfg      gCLIMMWaveOpenCfg;

/**
 * @brief
 *  Global mmwave device part number and string table
 */
CLI_partInfoString partInfoStringArray[] =
{
    /* Maximum part number string size is CLI_MAX_PARTNO_STRING_LEN */
#ifdef SOC_XWR14XX
    {SOC_AWR14XX_nonSecure_PartNumber,  "AWR14xx non-secure"},
    {SOC_IWR14XX_nonSecure_PartNumber,  "IWR14xx non-secure"},
#endif

#ifdef SOC_XWR16XX
    {SOC_AWR16XX_nonSecure_PartNumber,  "AWR16xx non-secure"},
    {SOC_IWR16XX_nonSecure_PartNumber,  "IWR16xx non-secure"},
    {SOC_AWR16XX_Secure_PartNumber,     "AWR16xx secure"},
    {SOC_IWR16XX_Secure_PartNumber,     "IWR16xx secure"},
#endif

#ifdef SOC_XWR18XX
    {SOC_AWR18XX_nonSecure_PartNumber,  "AWR18xx non-secure"},
    {SOC_IWR18XX_nonSecure_PartNumber,  "IWR18xx non-secure"},
    {SOC_AWR18XX_Secure_PartNumber,     "AWR18xx secure"},
#endif

#ifdef SOC_XWR68XX
    {SOC_IWR68XX_nonSecure_PartNumber,  "IWR68xx non-secure"},
#endif

    /* last entry of the table */
    {0U,                                "Not Supported"}
};

/**************************************************************************
 ********************** CLI mmWave Extension Functions ********************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      This is the CLI Handler for the version command
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  \ingroup CLI_UTIL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t CLI_MMWaveVersion (int32_t argc, char* argv[])
{
    rlVersion_t    verArgs;
    int32_t        retVal;
    uint8_t        rfPatchBuildVer, rfPatchDebugVer;
    rlRfDieIdCfg_t  dieId = { 0 };
    SOC_PartNumber  devicePartNumber;
    uint8_t         index;
    int32_t         errCode;

    /* print the platform */
#ifdef SOC_XWR14XX
    CLI_write ("Platform                : xWR14xx\n");
#elif defined(SOC_XWR16XX)              
    CLI_write ("Platform                : xWR16xx\n");
#elif defined(SOC_XWR18XX)              
    CLI_write ("Platform                : xWR18xx\n");
#elif defined(SOC_XWR68XX)
    CLI_write ("Platform                : xWR68xx\n");
#else                                   
    CLI_write ("Platform                : unknown\n");
#endif

    /* Get the version string: */
    retVal = SOC_getDevicePartNumber(gCLI.cfg.socHandle, &devicePartNumber, &errCode);
    if (retVal < 0)
    {
        CLI_write ("Error: Unable to get the device version from SOC[Error %d]\n", errCode);
        return -1;
    }

    /* Get the version string: */
    retVal = rlDeviceGetVersion(RL_DEVICE_MAP_INTERNAL_BSS, &verArgs);
    if (retVal < 0)
    {
        CLI_write ("Error: Unable to get the device version from mmWave link [Error %d]\n", retVal);
        return -1;
    }

    /* Display the version information on the CLI Console: */
    CLI_write ("mmWave SDK Version      : %02d.%02d.%02d.%02d\n",
                            MMWAVE_SDK_VERSION_MAJOR,
                            MMWAVE_SDK_VERSION_MINOR,
                            MMWAVE_SDK_VERSION_BUGFIX,
                            MMWAVE_SDK_VERSION_BUILD);
    index = 0;
    while(partInfoStringArray[index].partNumber !=0U)
    {
        if(devicePartNumber == partInfoStringArray[index].partNumber)
        {
            /* found device in supported list */
            break;
        }
        index++;
    }
    if(partInfoStringArray[index].partNumber ==0U)
    {
        /* Device is not found in the list: */
        CLI_write ("mmWave device [%2x] is not supported\n",
                                    devicePartNumber);
    }
    else
    {
        /* Display the device information on the CLI Console: */
        CLI_write ("Device Info             : %s ES %02d.%02d\n",
                   partInfoStringArray[index].partNumString,verArgs.rf.hwMajor, verArgs.rf.hwMinor);
    }

    CLI_write ("RF F/W Version          : %02d.%02d.%02d.%02d.%02d.%02d.%02d\n",
                verArgs.rf.fwMajor, verArgs.rf.fwMinor, verArgs.rf.fwBuild, verArgs.rf.fwDebug,
                verArgs.rf.fwYear, verArgs.rf.fwMonth, verArgs.rf.fwDay);
                                
   	rfPatchDebugVer = ((verArgs.rf.patchBuildDebug) & 0x0F);
	rfPatchBuildVer = (((verArgs.rf.patchBuildDebug) & 0xF0) >> 4);
    
    CLI_write ("RF F/W Patch            : %02d.%02d.%02d.%02d.%02d.%02d.%02d\n",
                verArgs.rf.patchMajor, verArgs.rf.patchMinor, rfPatchBuildVer, rfPatchDebugVer,
                verArgs.rf.patchYear, verArgs.rf.patchMonth, verArgs.rf.patchDay);            
    CLI_write ("mmWaveLink Version      : %02d.%02d.%02d.%02d\n",
                verArgs.mmWaveLink.major, verArgs.mmWaveLink.minor,
                verArgs.mmWaveLink.build, verArgs.mmWaveLink.debug);
                
    /* Get the die ID: */
    retVal = rlGetRfDieId(RL_DEVICE_MAP_INTERNAL_BSS, &dieId);
    if (retVal < 0)
    {
        CLI_write ("Error: Unable to get the device die ID from mmWave link [Error %d]\n", retVal);
        return 0;
    }

    CLI_write ("Lot number              : %d\n", dieId.lotNo);
    CLI_write ("Wafer number            : %d\n", dieId.waferNo);
    CLI_write ("Die coordinates in wafer: X = %d, Y = %d\n",
               dieId.devX, dieId.devY);

    /* Version string has been formatted successfully. */
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the CLI Handler for the flush configuration command
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  \ingroup CLI_UTIL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t CLI_MMWaveFlushCfg (int32_t argc, char* argv[])
{
    int32_t errCode;

    /* Flush the configuration in the MMWave */
    if (MMWave_flushCfg (gCLI.cfg.mmWaveHandle, &errCode) < 0)
    {
        /* Error: Flushing the configuration failed. Return the error code back to the callee */
        return errCode;
    }

    /* Reset the global configuration: */
    memset ((void*)&gCLIMMWaveControlCfg, 0, sizeof(MMWave_CtrlCfg));

    /* Reset the open configuration: */
    memset ((void*)&gCLIMMWaveOpenCfg, 0, sizeof(MMWave_OpenCfg));
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the CLI Handler for the DFE Data Output mode.
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  \ingroup CLI_UTIL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t CLI_MMWaveDataOutputMode (int32_t argc, char* argv[])
{
    uint32_t cfgMode;

    /* Sanity Check: Minimum argument check */
    if (argc != 2)
    {
        CLI_write ("Error: Invalid usage of the CLI command\n");
        return -1;
    }

    /* Get the configuration mode: */
    cfgMode = atoi (argv[1]);
    switch (cfgMode)
    {
        case 1U:
        {
            gCLIMMWaveControlCfg.dfeDataOutputMode = MMWave_DFEDataOutputMode_FRAME;
            break;
        }
        case 2U:
        {
            gCLIMMWaveControlCfg.dfeDataOutputMode = MMWave_DFEDataOutputMode_CONTINUOUS;
            break;
        }
        case 3U:
        {
            gCLIMMWaveControlCfg.dfeDataOutputMode = MMWave_DFEDataOutputMode_ADVANCED_FRAME;
            break;
        }
        default:
        {
            /* Error: Invalid argument. */
            CLI_write ("Error: Invalid mode.\n");
            return -1;
        }
    }

    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the CLI Handler for the channel configuration command
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  \ingroup CLI_UTIL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t CLI_MMWaveChannelCfg (int32_t argc, char* argv[])
{
    rlChanCfg_t     chCfg;

    /* Sanity Check: Minimum argument check */
    if (argc != 4)
    {
        CLI_write ("Error: Invalid usage of the CLI command\n");
        return -1;
    }

    /* Initialize the channel configuration: */
    memset ((void *)&chCfg, 0, sizeof(rlChanCfg_t));

    /* Populate the channel configuration: */
    chCfg.rxChannelEn = atoi (argv[1]);
    chCfg.txChannelEn = atoi (argv[2]);
    chCfg.cascading   = atoi (argv[3]);

    /* Save Configuration to use later */
    memcpy((void *)&gCLIMMWaveOpenCfg.chCfg, (void *)&chCfg, sizeof(rlChanCfg_t));
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the CLI Handler for the ADC configuration command
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  \ingroup CLI_UTIL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t CLI_MMWaveADCCfg (int32_t argc, char* argv[])
{
    rlAdcOutCfg_t   adcOutCfg;
    int32_t         retVal = 0;

    /* Sanity Check: Minimum argument check */
    if (argc != 3)
    {
        CLI_write ("Error: Invalid usage of the CLI command\n");
        return -1;
    }

    /* Initialize the ADC Output configuration: */
    memset ((void *)&adcOutCfg, 0, sizeof(rlAdcOutCfg_t));

    /* Populate the ADC Output configuration: */
    adcOutCfg.fmt.b2AdcBits   = atoi (argv[1]);
    adcOutCfg.fmt.b2AdcOutFmt = atoi (argv[2]);

    /* Save Configuration to use later */
    memcpy((void *)&gCLIMMWaveOpenCfg.adcOutCfg, (void *)&adcOutCfg, sizeof(rlAdcOutCfg_t));
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      This is the CLI Handler for the profile configuration command
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  \ingroup CLI_UTIL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t CLI_MMWaveProfileCfg (int32_t argc, char* argv[])
{
    rlProfileCfg_t          profileCfg;
    uint8_t                 index;
    int32_t                 errCode;
    MMWave_ProfileHandle    profileHandle;
    MMWave_ProfileHandle*   ptrBaseCfgProfileHandle;

    /* Sanity Check: Minimum argument check */
    if (argc != 15)
    {
        CLI_write ("Error: Invalid usage of the CLI command\n");
        return -1;
    }

    /* Sanity Check: Profile configuration is valid only for the Frame or 
                     Advanced Frame Mode: */
    if ((gCLIMMWaveControlCfg.dfeDataOutputMode != MMWave_DFEDataOutputMode_FRAME) &&
        (gCLIMMWaveControlCfg.dfeDataOutputMode != MMWave_DFEDataOutputMode_ADVANCED_FRAME))
    {
        CLI_write ("Error: Configuration is valid only if the DFE Output Mode is Frame or Advanced Frame\n");
        return -1;
    }
    
    if (gCLIMMWaveControlCfg.dfeDataOutputMode == MMWave_DFEDataOutputMode_FRAME)
    {
        ptrBaseCfgProfileHandle = &gCLIMMWaveControlCfg.u.frameCfg.profileHandle[0U];
    }
    else
    {
        ptrBaseCfgProfileHandle = &gCLIMMWaveControlCfg.u.advancedFrameCfg.profileHandle[0U];
    }

    /* Initialize the profile configuration: */
    memset ((void *)&profileCfg, 0, sizeof(rlProfileCfg_t));

    /* Populate the profile configuration: */
    profileCfg.profileId             = atoi (argv[1]);

    /* Translate from GHz to [1 LSB = gCLI_mmwave_freq_scale_factor * 1e9 / 2^26 Hz] units
     * of mmwavelink format */
    profileCfg.startFreqConst        = (uint32_t) (atof(argv[2]) * (1U << 26) /
                                                   gCLI_mmwave_freq_scale_factor);

    /* Translate below times from us to [1 LSB = 10 ns] units of mmwavelink format */
    profileCfg.idleTimeConst         = (uint32_t)((float)atof(argv[3]) * 1000 / 10);
    profileCfg.adcStartTimeConst     = (uint32_t)((float)atof(argv[4]) * 1000 / 10);
    profileCfg.rampEndTime           = (uint32_t)((float)atof(argv[5]) * 1000 / 10);

    profileCfg.txOutPowerBackoffCode = atoi (argv[6]);
    profileCfg.txPhaseShifter        = atoi (argv[7]);

    /* Translate from MHz/us to [1 LSB = (gCLI_mmwave_freq_scale_factor * 1e6 * 900) / 2^26 kHz/uS]
     * units of mmwavelink format */
    profileCfg.freqSlopeConst        = (int16_t)(atof(argv[8]) * (1U << 26) /
                                                 ((gCLI_mmwave_freq_scale_factor * 1e3) * 900.0));

    /* Translate from us to [1 LSB = 10 ns] units of mmwavelink format */
    profileCfg.txStartTime           = (int32_t)((float)atof(argv[9]) * 1000 / 10);

    profileCfg.numAdcSamples         = atoi (argv[10]);
    profileCfg.digOutSampleRate      = atoi (argv[11]);
    profileCfg.hpfCornerFreq1        = atoi (argv[12]);
    profileCfg.hpfCornerFreq2        = atoi (argv[13]);
    profileCfg.rxGain                = atoi (argv[14]);

    /* Search for a free space in the mmWave configuration block: */
    for (index = 0U; index < MMWAVE_MAX_PROFILE; index++)
    {
        /* Did we get a free entry? */
        if (ptrBaseCfgProfileHandle[index] == NULL)
        {
            /* YES: We can add the profile. */
            break;
        }
    }
    if (index == MMWAVE_MAX_PROFILE)
    {
        /* Error: All the profiles have been exhausted */
        return -1;
    }

    /* Add the profile to the mmWave module: */
    profileHandle = MMWave_addProfile (gCLI.cfg.mmWaveHandle, &profileCfg, &errCode);
    if (profileHandle == NULL)
    {
        /* Error: Unable to add the profile. Return the error code back */
        return errCode;
    }

    /* Record the profile: */
    ptrBaseCfgProfileHandle[index] = profileHandle;
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the CLI Handler for the chirp configuration command
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  \ingroup CLI_UTIL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t CLI_MMWaveChirpCfg (int32_t argc, char* argv[])
{
    rlChirpCfg_t            chirpCfg;
    MMWave_ProfileHandle    profileHandle;
    int32_t                 errCode;

    /* Sanity Check: Minimum argument check */
    if (argc != 9)
    {
        CLI_write ("Error: Invalid usage of the CLI command\n");
        return -1;
    }

    /* Sanity Check: Chirp configuration is valid only for the Frame or
                     Advanced Frame Mode: */
    if ((gCLIMMWaveControlCfg.dfeDataOutputMode != MMWave_DFEDataOutputMode_FRAME) &&
        (gCLIMMWaveControlCfg.dfeDataOutputMode != MMWave_DFEDataOutputMode_ADVANCED_FRAME))
    {
        CLI_write ("Error: Configuration is valid only if the DFE Output Mode is Chirp\n");
        return -1;
    }

    /* Initialize the chirp configuration: */
    memset ((void *)&chirpCfg, 0, sizeof(rlChirpCfg_t));

    /* Populate the chirp configuration: */
    chirpCfg.chirpStartIdx   = atoi (argv[1]);
    chirpCfg.chirpEndIdx     = atoi (argv[2]);
    chirpCfg.profileId       = atoi (argv[3]);

    /* Translate from Hz to number of [1 LSB = (gCLI_mmwave_freq_scale_factor * 1e9) / 2^26 Hz]
     * units of mmwavelink format */
    chirpCfg.startFreqVar    = (uint32_t) ((float)atof(argv[4]) * (1U << 26) /
                                            (gCLI_mmwave_freq_scale_factor * 1e9));

    /* Translate from KHz/us to number of [1 LSB = (gCLI_mmwave_freq_scale_factor * 1e6) * 900 /2^26 KHz/us]
     * units of mmwavelink format */
    chirpCfg.freqSlopeVar    = (uint16_t) ((float)atof(argv[5]) * (1U << 26) /
                                           ((gCLI_mmwave_freq_scale_factor * 1e6) * 900.0));

    /* Translate from us to [1 LSB = 10ns] units of mmwavelink format */
    chirpCfg.idleTimeVar     = (uint32_t)((float)atof (argv[6]) * 1000.0 / 10.0);

    /* Translate from us to [1 LSB = 10ns] units of mmwavelink format */
    chirpCfg.adcStartTimeVar = (uint32_t)((float)atof (argv[7]) * 1000.0 / 10.0);

    chirpCfg.txEnable        = atoi (argv[8]);

    /* Get the profile handle to which the chirp is to be added: */
    if (MMWave_getProfileHandle (gCLI.cfg.mmWaveHandle, chirpCfg.profileId,
                                 &profileHandle, &errCode) < 0)
    {
        /* Error: Unable to get the profile handle. Return the error code */
        return errCode;
    }

    /* Add the chirp to the profile */
    if (MMWave_addChirp (profileHandle, &chirpCfg, &errCode) == NULL)
    {
        /* Error: Unable to add the chirp. Return the error code. */
        return errCode;
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the CLI Handler for the frame configuration command
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  \ingroup CLI_UTIL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t CLI_MMWaveFrameCfg (int32_t argc, char* argv[])
{
    rlFrameCfg_t    frameCfg;

    /* Sanity Check: Minimum argument check */
    if (argc != 8)
    {
        CLI_write ("Error: Invalid usage of the CLI command\n");
        return -1;
    }

    /* Sanity Check: Frame configuration is valid only for the Frame or
                     Advanced Frame Mode: */
    if (gCLIMMWaveControlCfg.dfeDataOutputMode != MMWave_DFEDataOutputMode_FRAME)
    {
        CLI_write ("Error: Configuration is valid only if the DFE Output Mode is Chirp\n");
        return -1;
    }

    /* Initialize the frame configuration: */
    memset ((void *)&frameCfg, 0, sizeof(rlFrameCfg_t));

    /* Populate the frame configuration: */
    frameCfg.chirpStartIdx      = atoi (argv[1]);
    frameCfg.chirpEndIdx        = atoi (argv[2]);
    frameCfg.numLoops           = atoi (argv[3]);
    frameCfg.numFrames          = atoi (argv[4]);
    frameCfg.framePeriodicity   = (uint32_t)((float)atof(argv[5]) * 1000000 / 5);
    frameCfg.triggerSelect      = atoi (argv[6]);
    frameCfg.frameTriggerDelay  = (uint32_t)((float)atof(argv[7]) * 1000000 / 5);

    /* Save Configuration to use later */
    memcpy((void *)&gCLIMMWaveControlCfg.u.frameCfg.frameCfg, (void *)&frameCfg, sizeof(rlFrameCfg_t));
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the CLI Handler for the advanced frame configuration command
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  \ingroup CLI_UTIL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t CLI_MMWaveAdvFrameCfg (int32_t argc, char* argv[])
{
    rlAdvFrameCfg_t  advFrameCfg;

    /* Sanity Check: Minimum argument check */
    if (argc != 6)
    {
        CLI_write ("Error: Invalid usage of the CLI command\n");
        return -1;
    }
    
    /* Sanity Check: Frame configuration is valid only for the Frame or
                     Advanced Frame Mode: */
    if (gCLIMMWaveControlCfg.dfeDataOutputMode != MMWave_DFEDataOutputMode_ADVANCED_FRAME)
    {
        CLI_write ("Error: Configuration is valid only if the DFE Output Mode is Advanced Frame\n");
        return -1;
    }

    /* Initialize the frame configuration: */
    memset ((void *)&advFrameCfg, 0, sizeof(rlAdvFrameCfg_t));

    /* Populate the frame configuration: */
    advFrameCfg.frameSeq.numOfSubFrames      = atoi (argv[1]);
    advFrameCfg.frameSeq.forceProfile        = atoi (argv[2]);
    advFrameCfg.frameSeq.numFrames           = atoi (argv[3]);
    advFrameCfg.frameSeq.triggerSelect       = atoi (argv[4]);
    advFrameCfg.frameSeq.frameTrigDelay      = (uint32_t)((float)atof(argv[5]) * 1000000 / 5);

    /* Save Configuration to use later */
    memcpy ((void *)&gCLIMMWaveControlCfg.u.advancedFrameCfg.frameCfg,
            (void *)&advFrameCfg, sizeof(rlAdvFrameCfg_t));
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the CLI Handler for the subframe configuration command.
 *      Only valid when used in conjunction with the advanced frame configuration.
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  \ingroup CLI_UTIL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t CLI_MMWaveSubFrameCfg (int32_t argc, char* argv[])
{
    rlSubFrameCfg_t  subFrameCfg;
    uint8_t          subFrameNum;

    /* Sanity Check: Minimum argument check */
    if (argc != 11)
    {
        CLI_write ("Error: Invalid usage of the CLI command\n");
        return -1;
    }
    
    /* Sanity Check: Sub Frame configuration is valid only for the Advanced Frame Mode: */
    if (gCLIMMWaveControlCfg.dfeDataOutputMode != MMWave_DFEDataOutputMode_ADVANCED_FRAME)
    {
        CLI_write ("Error: Configuration is valid only if the DFE Output Mode is Advanced Frame\n");
        return -1;
    }

    /* Initialize the frame configuration: */
    memset ((void *)&subFrameCfg, 0, sizeof(rlSubFrameCfg_t));

    /* Populate the frame configuration: */
    subFrameNum                                  = (uint8_t)atoi (argv[1]);
    if (subFrameNum > gCLIMMWaveControlCfg.u.advancedFrameCfg.frameCfg.frameSeq.numOfSubFrames)
    {
        CLI_write ("Error: Invalid subframe number.\n");
        return -1;
    }
    subFrameCfg.forceProfileIdx     = atoi (argv[2]);
    subFrameCfg.chirpStartIdx       = atoi (argv[3]);
    subFrameCfg.numOfChirps         = atoi (argv[4]);
    subFrameCfg.numLoops            = atoi (argv[5]);
    subFrameCfg.burstPeriodicity    = (uint32_t)((float)atof(argv[6]) * 1000000 / 5);
    subFrameCfg.chirpStartIdxOffset = atoi (argv[7]);
    subFrameCfg.numOfBurst          = atoi (argv[8]);
    subFrameCfg.numOfBurstLoops     = atoi (argv[9]);
    subFrameCfg.subFramePeriodicity = (uint32_t)((float)atof(argv[10]) * 1000000 / 5);
    
    /* Save Configuration to use later */
    memcpy((void *)&gCLIMMWaveControlCfg.u.advancedFrameCfg.frameCfg.frameSeq.subFrameCfg[subFrameNum],
        (void *)&subFrameCfg, sizeof(rlSubFrameCfg_t));
    return 0;
    
}

/**
 *  @b Description
 *  @n
 *      This is the CLI Handler for the low power command
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  \ingroup CLI_UTIL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t CLI_MMWaveLowPowerCfg (int32_t argc, char* argv[])
{
    rlLowPowerModeCfg_t     lowPowerCfg;

    /* Sanity Check: Minimum argument check */
    if (argc != 3)
    {
        CLI_write ("Error: Invalid usage of the CLI command\n");
        return -1;
    }

    /* Initialize the channel configuration: */
    memset ((void *)&lowPowerCfg, 0, sizeof(rlLowPowerModeCfg_t));

    /* Populate the channel configuration: */
    lowPowerCfg.lpAdcMode     = atoi (argv[2]);

    /* Save Configuration to use later */
    memcpy((void *)&gCLIMMWaveOpenCfg.lowPowerMode, (void *)&lowPowerCfg, sizeof(rlLowPowerModeCfg_t));
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the CLI Handler for the continuous mode
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  \ingroup CLI_UTIL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t CLI_MMWaveContModeCfg (int32_t argc, char* argv[])
{
    MMWave_ContModeCfg *contCfg = &gCLIMMWaveControlCfg.u.continuousModeCfg;

    /* Sanity Check: Minimum argument check */
    if (argc != 10)
    {
        CLI_write ("Error: Invalid usage of the CLI command\n");
        return -1;
    }

    /* Sanity Check: Continuous configuration is valid only for the Continuous Mode: */
    if (gCLIMMWaveControlCfg.dfeDataOutputMode != MMWave_DFEDataOutputMode_CONTINUOUS)
    {
        CLI_write ("Error: Configuration is valid only if the DFE Output Mode is Continuous\n");
        return -1;
    }

    /* Populate the configuration: */
    contCfg->cfg.startFreqConst        = (uint32_t) (atof(argv[1]) * (1U << 26) /
                                                     gCLI_mmwave_freq_scale_factor);
    contCfg->cfg.txOutPowerBackoffCode = (uint32_t) atoi (argv[2]);
    contCfg->cfg.txPhaseShifter        = (uint32_t) atoi (argv[3]);
    contCfg->cfg.digOutSampleRate      = (uint16_t) atoi (argv[4]);
    contCfg->cfg.hpfCornerFreq1        = (uint8_t)  atoi (argv[5]);
    contCfg->cfg.hpfCornerFreq2        = (uint8_t)  atoi (argv[6]);
    contCfg->cfg.rxGain                = (uint16_t) atoi (argv[7]);
    /*argv[8] is reserved*/
    contCfg->dataTransSize             = (uint16_t) atoi (argv[9]);
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the CLI Handler for the BPM configuration.
 *
 *  @param[in] argc
 *      Number of arguments
 *  @param[in] argv
 *      Arguments
 *
 *  \ingroup CLI_UTIL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t CLI_MMWaveBPMCfgAdvanced (int32_t argc, char* argv[])
{   
    rlBpmChirpCfg_t         bpmChirpCfg;
    int32_t                 errCode;
       
    /* Sanity Check: Minimum argument check */
    if (argc != 4)
    {
        CLI_write ("Error: Invalid usage of the CLI command\n");
        return -1;
    }

    /* Sanity Check: BPM Chirp configuration is valid only for the Frame or
                     Advanced Frame Mode: */
    if ((gCLIMMWaveControlCfg.dfeDataOutputMode != MMWave_DFEDataOutputMode_FRAME) &&
        (gCLIMMWaveControlCfg.dfeDataOutputMode != MMWave_DFEDataOutputMode_ADVANCED_FRAME))
    {
        CLI_write ("Error: BPM Configuration is valid only if the DFE Output Mode is frame or advanced frame\n");
        return -1;
    }

    /* Initialize the chirp configuration: */
    memset ((void *)&bpmChirpCfg, 0, sizeof(rlBpmChirpCfg_t));

    /* Populate the chirp configuration: */
    bpmChirpCfg.chirpStartIdx   = atoi (argv[1]);
    bpmChirpCfg.chirpEndIdx     = atoi (argv[2]);
    bpmChirpCfg.constBpmVal     = atoi (argv[3]);
   
    /* Add the BPM chirp configuration to the list */
    if (MMWave_addBpmChirp (gCLI.cfg.mmWaveHandle, &bpmChirpCfg, &errCode) == NULL)
    {
        /* Error: Unable to add the BPM configuration. Return the error code. */
        return errCode;
    }

    return 0;

}

/**
 *  @b Description
 *  @n
 *      This is the mmWave extension initialization API
 *
 *  \ingroup CLI_UTIL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t CLI_MMWaveExtensionInit(CLI_Cfg* ptrCLICfg)
{
    int32_t         errCode;

    /* Sanity Check: We need the mmWave handle to work. */
    if (ptrCLICfg->mmWaveHandle == NULL)
    {
        return -1;
    }

    /* Initialize the mmWave control configuration: */
    memset ((void *)&gCLIMMWaveControlCfg, 0, sizeof(MMWave_CtrlCfg));

    gCLI_mmwave_freq_scale_factor = SOC_getDeviceRFFreqScaleFactor(ptrCLICfg->socHandle, &errCode);
    if(errCode < 0)
    {
        return -1;
    }

    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the mmWave extension handler which executes mmWave extension
 *      commands. This is invoked by the main CLI wrapper only if the extension
 *      was enabled.
 *
 *  @param[in]  argc
 *      Number of detected arguments
 *  @param[in] argv
 *      Detected arguments
 *
 *  \ingroup CLI_UTIL_INTERNAL_FUNCTION
 *
 *  @retval
 *      0   -   Matching mmWave extension command found
 *  @retval
 *      -1  -   No Matching mmWave extension command
 */
int32_t CLI_MMWaveExtensionHandler(int32_t argc, char* argv[])
{
    CLI_CmdTableEntry*  ptrCLICommandEntry;
    int32_t             cliStatus;
    int32_t             retVal = 0;

    /* Get the pointer to the mmWave extension table */
    ptrCLICommandEntry = &gCLIMMWaveExtensionTable[0];

    /* Cycle through all the registered externsion CLI commands: */
    while (ptrCLICommandEntry->cmdHandlerFxn != NULL)
    {
        /* Do we have a match? */
        if (strcmp(ptrCLICommandEntry->cmd, argv[0]) == 0)
        {
            /* YES: Pass this to the CLI registered function */
            cliStatus = ptrCLICommandEntry->cmdHandlerFxn (argc, argv);
            if (cliStatus == 0)
            {
                /* Successfully executed the CLI command: */
                CLI_write ("Done\n");
            }
            else
            {
                /* Error: The CLI command failed to execute */
                CLI_write ("Error %d\n", cliStatus);
            }
            break;
        }

        /* Get the next entry: */
        ptrCLICommandEntry++;
    }

    /* Was this a valid CLI command? */
    if (ptrCLICommandEntry->cmdHandlerFxn == NULL)
    {
        /* NO: The command was not a valid CLI mmWave extension command. Setup
         * the return value correctly. */
        retVal = -1;
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      This is the mmWave extension handler which is invoked by the
 *      CLI Help command handler only if the extension was enabled.
 *
 *  \ingroup CLI_UTIL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void CLI_MMWaveExtensionHelp(void)
{
    CLI_CmdTableEntry*  ptrCLICommandEntry;

    /* Get the pointer to the mmWave extension table */
    ptrCLICommandEntry = &gCLIMMWaveExtensionTable[0];

    /* Display the banner: */
    CLI_write ("****************************************************\n");
    CLI_write ("mmWave Extension Help\n");
    CLI_write ("****************************************************\n");

    /* Cycle through all the registered externsion CLI commands: */
    while (ptrCLICommandEntry->cmdHandlerFxn != NULL)
    {
        /* Display the help string*/
        CLI_write ("%s: %s\n",
                    ptrCLICommandEntry->cmd,
                   (ptrCLICommandEntry->helpString == NULL) ?
                    "No help available" :
                    ptrCLICommandEntry->helpString);

        /* Get the next entry: */
        ptrCLICommandEntry++;
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      This is an API provided by the CLI mmWave extension handler to get
 *      the mmWave control configuration.
 *
 *  @param[out]  ptrCtrlCfg
 *      Pointer to the control configuration populated by the API
 *
 *  \ingroup CLI_UTIL_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void CLI_getMMWaveExtensionConfig(MMWave_CtrlCfg* ptrCtrlCfg)
{
    memcpy ((void*)ptrCtrlCfg, (void*)&gCLIMMWaveControlCfg, sizeof(MMWave_CtrlCfg));
    return;
}

/**
 *  @b Description
 *  @n
 *      This is an API provided by the CLI mmWave extension handler to get
 *      the mmWave control configuration.
 *
 *  @param[out]  ptrOpenCfg
 *      Pointer to the open configuration populated by the API
 *
 *  \ingroup CLI_UTIL_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void CLI_getMMWaveExtensionOpenConfig(MMWave_OpenCfg* ptrOpenCfg)
{
    memcpy ((void*)ptrOpenCfg, (void*)&gCLIMMWaveOpenCfg, sizeof(MMWave_OpenCfg));
    return;
}

