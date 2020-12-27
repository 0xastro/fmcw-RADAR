@REM ###############################################################################
@REM #
@REM # Set up tools and build environment variables for mmwave sdk
@REM #
@REM ###############################################################################

@REM ###############################################################################
@REM # Build variables (to be modified based on build need)
@REM ###############################################################################

@REM Select your device. Options (case sensitive) are: awr14xx, iwr14xx, awr16xx, iwr16xx, awr18xx, iwr18xx, iwr68xx
set MMWAVE_SDK_DEVICE=iwr68xx

@REM If download via CCS is needed, set below define to yes else no
@REM   yes: Out file created can be loaded using CCS.
@REM        Binary file created can be used to flash
@REM   no:  Out file created cannot be loaded using CCS.
@REM        Binary file created can be used to flash 
@REM        (additional features: write-protect of TCMA, etc)
set DOWNLOAD_FROM_CCS=yes

@REM If using a secure device this variable needs to be updated with the path to mmwave_secdev_<ver> folder
set MMWAVE_SECDEV_INSTALL_PATH=

@REM If using a secure device, this variable needs to be updated with the path to hsimage.cfg file that
@REM has customer specific certificate/key information. A sample hsimage.cfg file is in the secdev package
set MMWAVE_SECDEV_HSIMAGE_CFG=%MMWAVE_SECDEV_INSTALL_PATH%/hs_image_creator/hsimage.cfg

@REM ###############################################################################
@REM # Customer installed tools variables (to be modified based on the installation)
@REM ###############################################################################


@REM ###############################################################################
@REM # TI tools variables (no changes needed below this line if default mmwave sdk
@REM # installation options are used)
@REM ###############################################################################
 
set MMWAVE_SDK_TOOLS_INSTALL_PATH=C:/ti
 
@REM Common settings for all platforms
@REM Path to <mmwave_sdk installation path>/packages folder
set MMWAVE_SDK_INSTALL_PATH=%MMWAVE_SDK_TOOLS_INSTALL_PATH%/mmwave_sdk_03_01_00_02/packages
@REM TI ARM compiler
set R4F_CODEGEN_INSTALL_PATH=%MMWAVE_SDK_TOOLS_INSTALL_PATH%/ti-cgt-arm_16.9.6.LTS
@REM TI XDC TOOLS
set XDC_INSTALL_PATH=%MMWAVE_SDK_TOOLS_INSTALL_PATH%/xdctools_3_50_08_24_core
@REM TI BIOS
set BIOS_INSTALL_PATH=%MMWAVE_SDK_TOOLS_INSTALL_PATH%/bios_6_73_01_01/packages

@REM awr14xx/iwr14xx radarss firmware
set XWR14XX_RADARSS_IMAGE_BIN=%MMWAVE_SDK_INSTALL_PATH%/../firmware/radarss/xwr12xx_xwr14xx_radarss_rprc.bin
@REM awr16xx/iwr16xx radarss firmware
set XWR16XX_RADARSS_IMAGE_BIN=%MMWAVE_SDK_INSTALL_PATH%/../firmware/radarss/xwr16xx_radarss_rprc.bin
@REM awr18xx/iwr18xx radarss firmware
set XWR18XX_RADARSS_IMAGE_BIN=%MMWAVE_SDK_INSTALL_PATH%/../firmware/radarss/xwr18xx_radarss_rprc.bin
@REM iwr68xx radarss firmware
set XWR68XX_RADARSS_IMAGE_BIN=%MMWAVE_SDK_INSTALL_PATH%/../firmware/radarss/iwr6xxx_radarss_rprc.bin
  
@REM Following only needed for devices with DSP
@REM TI DSP compiler
set C674_CODEGEN_INSTALL_PATH=%MMWAVE_SDK_TOOLS_INSTALL_PATH%/ti-cgt-c6000_8.1.3
@REM DSPlib
set C64Px_DSPLIB_INSTALL_PATH=%MMWAVE_SDK_TOOLS_INSTALL_PATH%/dsplib_c64Px_3_4_0_0
@REM DSPlib C674
set C674x_DSPLIB_INSTALL_PATH=%MMWAVE_SDK_TOOLS_INSTALL_PATH%/dsplib_c674x_3_4_0_0
@REM MATHlib
set C674x_MATHLIB_INSTALL_PATH=%MMWAVE_SDK_TOOLS_INSTALL_PATH%/mathlib_c674x_3_1_2_1


@REM Call script to check env variables and set paths
call checkenv.bat

@REM Check error code
IF %ERRORLEVEL% NEQ 0 (
  REM ERROR: Failure in checkenv.bat !!!
  REM Return error
  EXIT /B 1
)

EXIT /B 0

