rem This batch file is used to build the spiTest.exe executables
rem User needs to have Microsoft Visual Studio 1x.0 installed on the PC
rem User needs to adjust path and select which version of Windows is used.
rem User needs to set FTDI_PATH to ftdi files from the release package

echo on
rem Set the path below (MSVC_PATH) to where the following batch file is located: [vcvarsall.bat (windows10) OR vcvars32.bat (windows XP)]
set MSVC_PATH="C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC"

rem Set FTDI Path for FTDI files(FTDI files are under mmwave_sdk_01_xx_xx_xx\tools\ftdi in release packages)
rem For example (update the path to match the installation on your machine):
rem set FTDI_PATH="c:\ti\mmwave_sdk_01_01_00_00\tools\ftdi"

rem ******************************************************
rem      Choose Windows Version
rem ******************************************************
rem IF USING WINDOWS-10/WINDOWS-7 UNCOMMENT LINE BELOW
call %MSVC_PATH%\vcvarsall.bat x86

rem IF USING WINDOWS-XP UNCOMMENT LINE BELOW
rem call %MSVC_PATH%\vcvars32.bat
rem ******************************************************

del spiTest.exe

echo cl spiTest.cpp
cl /I %FTDI_PATH% spiTest.cpp

del *.obj
pause



