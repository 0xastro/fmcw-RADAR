#!/bin/bash
#
# ******************************************************************
# * FILE PURPOSE: Check environment setup
# ******************************************************************
# * FILE NAME: checkenv.sh
# *
# * DESCRIPTION:
# *  Checks the build environment and sets PATH for mmWave SDK build.
# *
# ******************************************************************

. ./print_utilities.sh

# Supported devices
devices_str="awr14xx, awr16xx, awr18xx, iwr14xx, iwr16xx, iwr18xx or iwr68xx"

retval=0

# -------------------------------------------------------------------
# Sanity Check: Ensure that all the required variables are defined
# -------------------------------------------------------------------
if [ -z $MMWAVE_SDK_INSTALL_PATH ]; then
    printErrorMsg "Error: MMWAVE_SDK_INSTALL_PATH [mmWave SDK Package Location] needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export MMWAVE_SDK_INSTALL_PATH=~/ti/mmwave_sdk_03_01_00_02/packages"
    retval=1
fi
if [ -z $R4F_CODEGEN_INSTALL_PATH ]; then
    printErrorMsg "Error: R4F_CODEGEN_INSTALL_PATH [R4F Toochain] needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export R4F_CODEGEN_INSTALL_PATH=~/ti/ti-cgt-arm_16.9.6.LTS"
    retval=1
fi
if [ -z $BIOS_INSTALL_PATH ]; then
    printErrorMsg "Error: BIOS_INSTALL_PATH [SYSBIOS] needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export BIOS_INSTALL_PATH=~/ti/bios_6_73_01_01/packages"
    retval=1
fi
if [ -z $XDC_INSTALL_PATH ]; then
    printErrorMsg "Error: XDC_INSTALL_PATH [XDC Build Tools] needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export XDC_INSTALL_PATH=~/ti/xdctools_3_50_08_24_core"
    retval=1
else
    if [ ! -d $XDC_INSTALL_PATH/jre ]; then
        printErrorMsg "Error: Installed XDC tool version does not have JRE"
        printInfoMsg  "       Please install the XDC tool version with JRE"
        printInfoMsg  "          - The XDC installer with JRE will be larger in size than the one without JRE"
        printInfoMsg  "          - The XDC installer with JRE does not have jre in its name or description"
        retval=1
    fi
fi

# This define is not directly tools related but needed for mmwave sdk build
if [ -z $DOWNLOAD_FROM_CCS ]; then
    printErrorMsg "Error: DOWNLOAD_FROM_CCS needs to be defined to yes or no"
    printInfoMsg  "For example:"
    printInfoMsg  "  export DOWNLOAD_FROM_CCS=yes"
    retval=1
fi
if [ -z $MMWAVE_SDK_DEVICE ]; then
    printErrorMsg "Error: MMWAVE_SDK_DEVICE needs to be defined. Supported devices are ${devices_str}"
    printInfoMsg  "For example:"
    printInfoMsg  "  export MMWAVE_SDK_DEVICE=awr16xx"
    retval=1
fi

# check if the specified device is supported
if [[ "${MMWAVE_SDK_DEVICE}" != @(awr14xx|iwr14xx|awr16xx|iwr16xx|awr18xx|iwr18xx|iwr68xx) ]]; then
    printErrorMsg "Error: MMWAVE_SDK_DEVICE $MMWAVE_SDK_DEVICE is not supported. Supported devices are ${devices_str}"
    retval=1
fi

if [ "$MMWAVE_SDK_DEVICE" == "awr14xx" ] || [ "$MMWAVE_SDK_DEVICE" == "iwr14xx" ]; then
if [ -z $XWR14XX_RADARSS_IMAGE_BIN ]; then
    printErrorMsg "Error: XWR14XX_RADARSS_IMAGE_BIN needs to be defined with the complete path and filename of the radarss binary"
    printInfoMsg  "For example:"
    printInfoMsg  "  export XWR14XX_RADARSS_IMAGE_BIN=$MMWAVE_SDK_INSTALL_PATH/../firmware/radarss/xwr12xx_xwr14xx_radarss_rprc.bin"
    retval=1
else
    if [ ! -f "$XWR14XX_RADARSS_IMAGE_BIN" ]; then
        printErrorMsg "Error: $XWR14XX_RADARSS_IMAGE_BIN does not exist. Check the path"
        retval=1
    fi
fi
fi

if [ "$MMWAVE_SDK_DEVICE" == "awr16xx" ] || [ "$MMWAVE_SDK_DEVICE" == "iwr16xx" ] ||
   [ "$MMWAVE_SDK_DEVICE" == "awr18xx" ] || [ "$MMWAVE_SDK_DEVICE" == "iwr18xx" ] ||
   [ "$MMWAVE_SDK_DEVICE" == "iwr68xx" ] ; then
if [ -z $C674_CODEGEN_INSTALL_PATH ]; then
    printErrorMsg "Error: C674_CODEGEN_INSTALL_PATH [DSP Toolchain] needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export C674_CODEGEN_INSTALL_PATH=~/ti/ti-cgt-c6000_8.1.3"
    retval=1
fi
if [ -z $C64Px_DSPLIB_INSTALL_PATH ]; then
    printErrorMsg "Error: C64Px_DSPLIB_INSTALL_PATH needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export C64Px_DSPLIB_INSTALL_PATH=~/ti/dsplib_c64Px_3_4_0_0"
    retval=1
fi
if [ -z $C674x_DSPLIB_INSTALL_PATH ]; then
    printErrorMsg "Error: C674x_DSPLIB_INSTALL_PATH needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export C674x_DSPLIB_INSTALL_PATH=~/ti/dsplib_c674x_3_4_0_0"
    retval=1
fi
if [ -z $C674x_MATHLIB_INSTALL_PATH ]; then
    printErrorMsg "Error: C674x_MATHLIB_INSTALL_PATH needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export C674x_MATHLIB_INSTALL_PATH=~/ti/mathlib_c674x_3_1_2_1"
    retval=1
fi
fi

if [ "$MMWAVE_SDK_DEVICE" == "awr16xx" ] || [ "$MMWAVE_SDK_DEVICE" == "iwr16xx" ]; then
if [ -z $XWR16XX_RADARSS_IMAGE_BIN ]; then
    printErrorMsg "Error: XWR16XX_RADARSS_IMAGE_BIN needs to be defined with the complete path and filename of the radarss binary"
    printInfoMsg  "For example:"
    printInfoMsg  "  export XWR16XX_RADARSS_IMAGE_BIN=$MMWAVE_SDK_INSTALL_PATH/../firmware/radarss/xwr16xx_radarss_rprc.bin"
    retval=1
else
    if [ ! -f "$XWR16XX_RADARSS_IMAGE_BIN" ]; then
        printErrorMsg "Error: $XWR16XX_RADARSS_IMAGE_BIN does not exist. Check the path"
        retval=1
    fi
fi
fi

if [ "$MMWAVE_SDK_DEVICE" == "awr18xx" ] || [ "$MMWAVE_SDK_DEVICE" == "iwr18xx" ]; then
if [ -z $XWR18XX_RADARSS_IMAGE_BIN ]; then
    printErrorMsg "Error: XWR18XX_RADARSS_IMAGE_BIN needs to be defined with the complete path and filename of the radarss binary"
    printInfoMsg  "For example:"
    printInfoMsg  "  export XWR18XX_RADARSS_IMAGE_BIN=$MMWAVE_SDK_INSTALL_PATH/../firmware/radarss/xwr18xx_radarss_rprc.bin"
    retval=1
else
    if [ ! -f "$XWR18XX_RADARSS_IMAGE_BIN" ]; then
        printErrorMsg "Error: $XWR18XX_RADARSS_IMAGE_BIN does not exist. Check the path"
        retval=1
    fi
fi
fi

if [ "$MMWAVE_SDK_DEVICE" == "iwr68xx" ]; then
if [ -z $XWR68XX_RADARSS_IMAGE_BIN ]; then
    printErrorMsg "Error: XWR68XX_RADARSS_IMAGE_BIN needs to be defined with the complete path and filename of the radarss binary"
    printInfoMsg  "For example:"
    printInfoMsg  "  export XWR68XX_RADARSS_IMAGE_BIN=$MMWAVE_SDK_INSTALL_PATH/../firmware/radarss/iwr6xxx_radarss_rprc.bin"
    retval=1
else
    if [ ! -f "$XWR68XX_RADARSS_IMAGE_BIN" ]; then
        printErrorMsg "Error: $XWR68XX_RADARSS_IMAGE_BIN does not exist. Check the path"
        retval=1
    fi
fi
fi

if [ $retval -eq 1 ]; then
    printErrorMsg "Error: One of more build variables are not set correctly. Exiting...."
    return 1
fi

# ---------------------------------------------------------
# Setup the PATH configuration:
# ----------------------------------------------------------
# Add XDC to path if it is not already in $PATH
[[ ":$PATH:" != *":$XDC_INSTALL_PATH:"* ]] && export PATH="$XDC_INSTALL_PATH:$XDC_INSTALL_PATH/bin:${PATH}"

printInfoMsg "-----------------------------------------------"
printInfoMsg "mmWave Build Environment Configured"
printInfoMsg "-----------------------------------------------"
return 0

