###############################################################################
#
# Set up tools and build environment variables for mmwave sdk
#
###############################################################################

###############################################################################
# Build variables (to be modified based on build need)
###############################################################################

# Select your device. Options (case sensitive) are: awr14xx, iwr14xx, awr16xx, iwr16xx, awr18xx, iwr18xx, iwr68xx
export MMWAVE_SDK_DEVICE=iwr68xx

# If download via CCS is needed, set below define to yes else no
#   yes: Out file created can be loaded using CCS.
#        Binary file created can be used to flash
#   no:  Out file created cannot be loaded using CCS.
#        Binary file created can be used to flash 
#        (additional features: write-protect of TCMA, etc)
export DOWNLOAD_FROM_CCS=yes

# If using a secure device, this variable needs to be updated with the path to mmwave_secdev_<ver> folder
export MMWAVE_SECDEV_INSTALL_PATH=

# If using a secure device, this variable needs to be updated with the path to hsimage.cfg file that
# has customer specific certificate/key information. A sample hsimage.cfg file is in the secdev package
export MMWAVE_SECDEV_HSIMAGE_CFG=${MMWAVE_SECDEV_INSTALL_PATH}/hs_image_creator/hsimage.cfg

###############################################################################
# Customer installed tools variables (to be modified based on the installation)
###############################################################################


###############################################################################
# TI tools variables (no changes needed below this line if default mmwave sdk
# installation options are used)
###############################################################################

export MMWAVE_SDK_TOOLS_INSTALL_PATH=__MMWAVE_SDK_TOOLS_INSTALL_PATH__
 
# Common settings for all platforms
# Path to <mmwave_sdk installation path>/packages folder
export MMWAVE_SDK_INSTALL_PATH=${MMWAVE_SDK_TOOLS_INSTALL_PATH}/mmwave_sdk_03_01_00_02/packages
# TI ARM compiler
export R4F_CODEGEN_INSTALL_PATH=${MMWAVE_SDK_TOOLS_INSTALL_PATH}/ti-cgt-arm_16.9.6.LTS
# TI XDC TOOLS
export XDC_INSTALL_PATH=${MMWAVE_SDK_TOOLS_INSTALL_PATH}/xdctools_3_50_08_24_core
# TI BIOS
export BIOS_INSTALL_PATH=${MMWAVE_SDK_TOOLS_INSTALL_PATH}/bios_6_73_01_01/packages

# awr14xx/iwr14xx radarss firmware
export XWR14XX_RADARSS_IMAGE_BIN=${MMWAVE_SDK_INSTALL_PATH}/../firmware/radarss/xwr12xx_xwr14xx_radarss_rprc.bin
# awr16xx/iwr16xx radarss firmware
export XWR16XX_RADARSS_IMAGE_BIN=${MMWAVE_SDK_INSTALL_PATH}/../firmware/radarss/xwr16xx_radarss_rprc.bin
# awr18xx/iwr18xx radarss firmware
export XWR18XX_RADARSS_IMAGE_BIN=${MMWAVE_SDK_INSTALL_PATH}/../firmware/radarss/xwr18xx_radarss_rprc.bin
# iwr68xx radarss firmware
export XWR68XX_RADARSS_IMAGE_BIN=${MMWAVE_SDK_INSTALL_PATH}/../firmware/radarss/iwr6xxx_radarss_rprc.bin
  
# Following only needed for devices with DSP
# TI DSP compiler
export C674_CODEGEN_INSTALL_PATH=${MMWAVE_SDK_TOOLS_INSTALL_PATH}/ti-cgt-c6000_8.1.3
# DSPlib
export C64Px_DSPLIB_INSTALL_PATH=${MMWAVE_SDK_TOOLS_INSTALL_PATH}/dsplib_c64Px_3_4_0_0
# DSPlib c674
export C674x_DSPLIB_INSTALL_PATH=${MMWAVE_SDK_TOOLS_INSTALL_PATH}/dsplib_c674x_3_4_0_0
# MATHlib
export C674x_MATHLIB_INSTALL_PATH=${MMWAVE_SDK_TOOLS_INSTALL_PATH}/mathlib_c674x_3_1_2_1


# Call script to check env variables and set paths
source ./checkenv.sh
retval=$?
if [[ $retval != 0 ]]; then
    printErrorMsg "ERROR: Failure in checkenv !!!"
fi
return $retval;

