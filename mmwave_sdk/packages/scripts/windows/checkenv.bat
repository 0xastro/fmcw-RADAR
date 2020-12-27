@REM ******************************************************************
@REM * FILE PURPOSE: Check environment setup
@REM ******************************************************************
@REM * FILE NAME: checkenv.bat
@REM *
@REM * DESCRIPTION:
@REM *  Checks the build environment and sets PATH for mmWave SDK build.
@REM *
@REM ******************************************************************
@echo off

@REM Supported devices
set devices_str=awr14xx, awr16xx, awr18xx, iwr14xx, iwr16xx, iwr18xx or iwr68xx

REM -------------------------------------------------------------------
REM Sanity Check: Ensure that all the required variables are defined
REM -------------------------------------------------------------------
if [%MMWAVE_SDK_INSTALL_PATH%] == [] (
@echo Error: MMWAVE_SDK_INSTALL_PATH [mmWave SDK Package Location] needs to be defined
@echo For Example:
@echo   set MMWAVE_SDK_INSTALL_PATH=C:/ti/mmwave_sdk_03_01_00_02/packages
goto fail
)
if [%R4F_CODEGEN_INSTALL_PATH%] == [] (
@echo Error: R4F_CODEGEN_INSTALL_PATH [R4F Toochain] needs to be defined
@echo For Example:
@echo   set R4F_CODEGEN_INSTALL_PATH=C:/ti/ti-cgt-arm_16.9.6.LTS
goto fail
)
if [%BIOS_INSTALL_PATH%] == [] (
@echo Error: BIOS_INSTALL_PATH [SYSBIOS] needs to be defined
@echo For Example:
@echo   set BIOS_INSTALL_PATH=C:/ti/bios_6_73_01_01/packages
goto fail
)
if [%XDC_INSTALL_PATH%] == [] (
@echo Error: XDC_INSTALL_PATH [XDC Build Tools] needs to be defined
@echo For Example:
@echo   set XDC_INSTALL_PATH=C:/ti/xdctools_3_50_08_24_core
goto fail
)
IF NOT EXIST %XDC_INSTALL_PATH%\jre (
@echo Error: Installed XDC tool version does not have JRE
@echo        Please install the XDC tool version with JRE
@echo           - The XDC installer with JRE will be larger in size than the one without JRE
@echo           - The XDC installer with JRE does not have jre in its name or description
goto fail
)
if [%DOWNLOAD_FROM_CCS%] == [] (
@echo Error: DOWNLOAD_FROM_CCS needs to be defined to yes or no
@echo For Example:
@echo   set DOWNLOAD_FROM_CCS=yes
goto fail
)
if [%MMWAVE_SDK_DEVICE%] == [] (
@echo Error: MMWAVE_SDK_DEVICE needs to be defined. Supported devices are %devices_str%
@echo For Example:
@echo   set MMWAVE_SDK_DEVICE=awr16xx
goto fail
)

IF [%MMWAVE_SDK_DEVICE%] == [awr14xx] (
    set MMWAVE_SDK_DEVICE_TYPE=xwr14xx
) ELSE (
IF [%MMWAVE_SDK_DEVICE%] == [iwr14xx] (
    set MMWAVE_SDK_DEVICE_TYPE=xwr14xx
) ELSE (
IF [%MMWAVE_SDK_DEVICE%] == [awr16xx] (
    set MMWAVE_SDK_DEVICE_TYPE=xwr16xx
) ELSE (
IF [%MMWAVE_SDK_DEVICE%] == [iwr16xx] (
    set MMWAVE_SDK_DEVICE_TYPE=xwr16xx
) ELSE (
IF [%MMWAVE_SDK_DEVICE%] == [awr18xx] (
    set MMWAVE_SDK_DEVICE_TYPE=xwr18xx
) ELSE (
IF [%MMWAVE_SDK_DEVICE%] == [iwr18xx] (
    set MMWAVE_SDK_DEVICE_TYPE=xwr18xx
) ELSE (
IF [%MMWAVE_SDK_DEVICE%] == [iwr68xx] (
    set MMWAVE_SDK_DEVICE_TYPE=xwr68xx
) ELSE (
    @echo Error: MMWAVE_SDK_DEVICE %MMWAVE_SDK_DEVICE% is not supported. Supported devices are %devices_str%
    goto fail
)))))))

if [%MMWAVE_SDK_DEVICE_TYPE%] == [xwr14xx] (
if [%XWR14XX_RADARSS_IMAGE_BIN%] == [] (
@echo Error: XWR14XX_RADARSS_IMAGE_BIN needs to be defined with the complete path and filename of the radarss binary
@echo For Example:
@echo   set XWR14XX_RADARSS_IMAGE_BIN=%MMWAVE_SDK_INSTALL_PATH%/../firmware/radarss/xwr12xx_xwr14xx_radarss_rprc.bin
goto fail
)
if NOT EXIST "%XWR14XX_RADARSS_IMAGE_BIN%" (
@echo Error: %XWR14XX_RADARSS_IMAGE_BIN% does not exist. Check the path
goto fail
)
)

if NOT  [%MMWAVE_SDK_DEVICE_TYPE%] == [xwr14xx] (
if [%C674_CODEGEN_INSTALL_PATH%] == [] (
@echo Error: C674_CODEGEN_INSTALL_PATH [DSP Toolchain] needs to be defined
@echo For Example:
@echo   set C674_CODEGEN_INSTALL_PATH=C:/ti/ti-cgt-c6000_8.1.3
goto fail
)
if [%C64Px_DSPLIB_INSTALL_PATH%] == [] (
@echo Error: C64Px_DSPLIB_INSTALL_PATH needs to be defined
@echo For Example:
@echo   set C64Px_DSPLIB_INSTALL_PATH=C:/ti/dsplib_c64Px_3_4_0_0
goto fail
)
if [%C674x_DSPLIB_INSTALL_PATH%] == [] (
@echo Error: C674x_DSPLIB_INSTALL_PATH needs to be defined
@echo For Example:
@echo   set C674x_DSPLIB_INSTALL_PATH=C:/ti/dsplib_c674x_3_4_0_0
goto fail
)
if [%C674x_MATHLIB_INSTALL_PATH%] == [] (
@echo Error: C674x_MATHLIB_INSTALL_PATH needs to be defined
@echo For Example:
@echo   set C674x_MATHLIB_INSTALL_PATH=C:/ti/mathlib_c674x_3_1_2_1
goto fail
)
if [%MMWAVE_SDK_DEVICE_TYPE%] == [xwr16xx] (
    if [%XWR16XX_RADARSS_IMAGE_BIN%] == [] (
    @echo Error: XWR16XX_RADARSS_IMAGE_BIN needs to be defined with the complete path and filename of the radarss binary
    @echo For Example:
    @echo   set XWR16XX_RADARSS_IMAGE_BIN=%MMWAVE_SDK_INSTALL_PATH%/../firmware/radarss/xwr16xx_radarss_rprc.bin
    goto fail
    )
    if NOT EXIST "%XWR16XX_RADARSS_IMAGE_BIN%" (
        @echo Error: %XWR16XX_RADARSS_IMAGE_BIN% does not exist. Check the path
    goto fail
    )
)
if [%MMWAVE_SDK_DEVICE_TYPE%] == [xwr18xx] (
    if [%XWR18XX_RADARSS_IMAGE_BIN%] == [] (
    @echo Error: XWR18XX_RADARSS_IMAGE_BIN needs to be defined with the complete path and filename of the radarss binary
    @echo For Example:
    @echo   set XWR18XX_RADARSS_IMAGE_BIN=%MMWAVE_SDK_INSTALL_PATH%/../firmware/radarss/xwr18xx_radarss_rprc.bin
    goto fail
    )
    if NOT EXIST "%XWR18XX_RADARSS_IMAGE_BIN%" (
        @echo Error: %XWR18XX_RADARSS_IMAGE_BIN% does not exist. Check the path
    goto fail
    )
)
if [%MMWAVE_SDK_DEVICE_TYPE%] == [xwr68xx] (
    if [%XWR68XX_RADARSS_IMAGE_BIN%] == [] (
    @echo Error: XWR68XX_RADARSS_IMAGE_BIN needs to be defined with the complete path and filename of the radarss binary
    @echo For Example:
    @echo   set XWR68XX_RADARSS_IMAGE_BIN=%MMWAVE_SDK_INSTALL_PATH%/../firmware/radarss/iwr6xxx_radarss_rprc.bin
    goto fail
    )
    if NOT EXIST "%XWR68XX_RADARSS_IMAGE_BIN%" (
        @echo Error: %XWR68XX_RADARSS_IMAGE_BIN% does not exist. Check the path
    goto fail
    )
)
)

REM ----------------------------------------------------------
REM Setup the PATH configuration:
REM Please do not change anything below this
REM ----------------------------------------------------------

REM XDC Tools including gmake
REM Add XDC to path if not already in PATH
echo "%PATH%" | find /i "%XDC_INSTALL_PATH%">nul || set "PATH=%XDC_INSTALL_PATH%;%XDC_INSTALL_PATH%/bin;%PATH%"

REM Following tools are optional and needed only for doxygen doc generation
REM Add to path only if these are defined and not already in PATH
if NOT ["%DOXYGEN_INSTALL_PATH%"] == [""] (
    echo "%PATH%" | find /i "%DOXYGEN_INSTALL_PATH%\bin">nul || set "PATH=%PATH%;%DOXYGEN_INSTALL_PATH%\bin"
)
if NOT ["%GRAPHVIZ_INSTALL_PATH%"] == [""] (
    echo "%PATH%" | find /i "%GRAPHVIZ_INSTALL_PATH%\bin">nul || set "PATH=%PATH%;%GRAPHVIZ_INSTALL_PATH%\bin"
)
if NOT ["%MIKTEX_INSTALL_PATH%"] == [""] (
    echo "%PATH%" | find /i "%MIKTEX_INSTALL_PATH%">nul || set "PATH=%PATH%;%MIKTEX_INSTALL_PATH%"
)
if NOT ["%GHOSTSCRIPT_INSTALL_PATH%"] == [""] (
    echo "%PATH%" | find /i "%GHOSTSCRIPT_INSTALL_PATH%\bin">nul || set "PATH=%PATH%;%GHOSTSCRIPT_INSTALL_PATH%\bin"
)

@echo -----------------------------------------------
@echo mmWave Build Environment Configured
@echo -----------------------------------------------

REM Set the Title Window appropriately.
Title mmWave Build Environment
goto end

:fail
@echo Error: Failed to setup mmWave Build Environment !
Title Error: Failed to setup mmWave Build Environment
REM Return error
EXIT /B 1

:end
REM Return success
EXIT /B 0
