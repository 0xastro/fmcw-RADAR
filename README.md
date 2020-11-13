
|The project has completed. All documentations, sequence diagrams as well as source codes will be migerated from gitlab as soon as possible|
| --- |



|The System will be reconfigured to LRR (Long Range Radar) but keeping the highst anglular resolution by operating in TDM-3Tx-12vRx Mode. This is to be used as a baseline development for railway drone obstacle avoidance use cases. Trials and field tests will be added by the end of Q4 2020. UPDATED  NOV 13, 2020| 
| --- |

|Docker container based on CCSv9 and TI mmWave SDK is currently under development to enable CI/CD using Jenkins UPDATED  NOV 13, 2020|
| --- |



# fmcw-RADAR
[mmWave based fmcw radar design files] based on AWR1843 chip operating at 76-GHz to 81-GHz.

### Objective
Test & Evaluate the performance of TI mmWave Radar system using the AWR1843 EVM development board.

In this system we detect specifically objects within close proximity the radar and generally evaluate different ranges profiles short, medium and long for further application.

We use TI AWR1843 EVM, algorithms run on-board the single-chip device to create Range-Azimuth and Range-Elevation heat-maps, then performs object detection with CFAR, angle of arrival estimation and clustering on configured range rows in the heatmaps.

### Dependencies

### Getting Started 

#### CCS Debug utility
CCS Debug Utility: This utility can be flashed on the QSPI The application is written over SYSBIOS and will loop forever. In the meantime CCS can be attached and the developers can then download the real application which needs to be debugged.
This utility provides while loop application for MSS in XWR14xx and for MSS and DSS in XWR16xx/XWR18xx/XWR68xx.
On MSS, it calls ESM and SOC init functions to initialize the device in known state. ESM init is needed to install the FIQ handler in case there are ESM errors on bootup. SOC init is needed to unhalt DSP in XWR16xx/XWR18xx/XWR68xx. As a byproduct, SOC init also unhalts the BSS and performs the APLL close loop procedure. Hence when this debug utility is executed from flash, the system is running using the APLL clocks.



> Debug mode: Downloading and running the executable (.xer4f image) from CCS. You will need to flash a small CCS debug firmware on the EVM (one time) to allow connecting with CCS. This debug firmware image is provided with the mmWave SDK.

> To prepare the EVM for debug mode, we start with flashing the CCS debug firmware image.

- Power on the EVM using a 5V/2.5A power supply.
- Connect the EVM to your PC and check the COM ports. The EVM exports two virtual COM ports as shown below:
- - XDS110 Class Application/User UART (COMUART): Used for passing configuration data and firmware to the EVM
- - XDS110 Class Auxiliary Data Port (COMAUX): Used to send processed radar data output
- Set the Dip Switch SOP0=ON; SOP1=OFF; SOP2=ON
- Open the UniFlash tool TI.com/tool/uniflash
- - In the New Configuration section, locate and select the appropriate device AWR1843BOOST
- - Click Start to proceed
- - In the Program tab, browse and locate demo binary file C:\ti\mmwave_sdk_xx_xx_xx_xx\packages\ti\utils\ccsdebug\xwr18xx_ccsdebug.bin
- - In the Settings & Utilities tab, fill the COM Port text box with the Application/User UART COM port number (COMUART) noted earlier

~~~shell
[10/12/2020, 12:31:29 PM] [INFO] Cortex_R4_0: Initialization complete.
[10/12/2020, 12:31:29 PM] [INFO] Cortex_R4_0: Flashing process starting...
[10/12/2020, 12:31:29 PM] [INFO] Cortex_R4_0: Connecting to COM Port COM1...
[10/12/2020, 12:31:29 PM] [INFO] Cortex_R4_0: Reset connection to device
[10/12/2020, 12:31:29 PM] [ERROR] Cortex_R4_0: Serial port COM1 specified does not exist, is already open, or permission is denied!!
[10/12/2020, 12:31:29 PM] [ERROR] Cortex_R4_0: !! Aborting operation!!
[10/12/2020, 12:31:29 PM] [ERROR] Cortex_R4_0: Not able to connect to serial port. Recheck COM port selected and/or permissions.
[10/12/2020, 12:31:29 PM] [INFO] Cortex_R4_0: Flashing instance clean-up initiated...
[10/12/2020, 12:31:30 PM] [INFO] Cortex_R4_0: Instance deinitialized!
[10/12/2020, 12:31:43 PM] [INFO] Cortex_R4_0: Initialization complete.
[10/12/2020, 12:31:43 PM] [INFO] Cortex_R4_0: Flashing process starting...
[10/12/2020, 12:31:43 PM] [INFO] Cortex_R4_0: Connecting to COM Port COM3...
[10/12/2020, 12:31:43 PM] [INFO] Cortex_R4_0: Reset connection to device
[10/12/2020, 12:31:43 PM] [INFO] Cortex_R4_0: Set break signal
[10/12/2020, 12:31:43 PM] [INFO] Cortex_R4_0: Connection to COM port succeeded. Flashing can proceed.
[10/12/2020, 12:31:43 PM] [INFO] Cortex_R4_0: Reading device version info...
[10/12/2020, 12:31:43 PM] [INFO] Cortex_R4_0: ** 1 files specified for flashing.
[10/12/2020, 12:31:43 PM] [INFO] Cortex_R4_0: Checking file C:/ti/mmwave_sdk_03_01_00_02/packages/ti/utils/ccsdebug/xwr18xx_ccsdebug.bin for correct header for AWR1843.
[10/12/2020, 12:31:43 PM] [INFO] Cortex_R4_0: AWR1843 device, fileType=META_IMAGE1 detected -> OK
[10/12/2020, 12:31:43 PM] [INFO] Cortex_R4_0: !! Files are valid for AWR1843.
[10/12/2020, 12:31:43 PM] [INFO] Cortex_R4_0: Format on download was specified. Formatting SFLASH storage...
[10/12/2020, 12:31:43 PM] [INFO] Cortex_R4_0: -->Erasing storage [SFLASH]
[10/12/2020, 12:31:43 PM] [INFO] Cortex_R4_0: -->Sending Erase command to device...
[10/12/2020, 12:31:57 PM] [SUCCESS] Cortex_R4_0: -->Erase storage completed successfully!
[10/12/2020, 12:31:57 PM] [INFO] Cortex_R4_0: Downloading [META_IMAGE1] size [128836]
[10/12/2020, 12:32:22 PM] [INFO] Cortex_R4_0: SUCCESS!! File type META_IMAGE1 downloaded successfully to SFLASH.
[10/12/2020, 12:32:22 PM] [INFO] Cortex_R4_0: Disconnecting from device on COM port COM3...
[10/12/2020, 12:32:22 PM] [INFO] Cortex_R4_0: Flashing instance clean-up initiated...
[10/12/2020, 12:32:22 PM] [INFO] Cortex_R4_0: Instance deinitialized!
[10/12/2020, 12:32:22 PM] [SUCCESS] Program Load completed successfully.
~~~


#### Connecting EVM to CCS

> To connect the Radar EVM to CCS, we need to create a target configuration

-  Go to File ► New ► New Target Configuration File
- Name the target configuration accordingly and check the “Use shared location” checkbox. Press Finish
- In the configuration editor window: Select “Texas Instruments XDS110 USB Debug Probe” for Connection and Select AWR1843 device in the Board or Device text box.
- Press the Save button to save the target configuration.
- You can press the Test Connection button to check the connection with the board.

> TEST LOGS

~~~shell
[Start: Texas Instruments XDS110 USB Debug Probe_0]

Execute the command:

%ccs_base%/common/uscif/dbgjtag -f %boarddatafile% -rv -o -S integrity

[Result]


-----[Print the board config pathname(s)]------------------------------------

C:\Users\astro\AppData\Local\TEXASI~1\CCS\
    ccs920\0\0\BrdDat\testBoard.dat

-----[Print the reset-command software log-file]-----------------------------

This utility has selected a 100- or 510-class product.
This utility will load the adapter 'jioxds110.dll'.
The library build date was 'Aug 26 2019'.
The library build time was '13:34:49'.
The library package version is '8.3.0.00003'.
The library component version is '35.35.0.0'.
The controller does not use a programmable FPGA.
Updating the XDS110 firmware ... complete.
The controller has a version number of '5' (0x00000005).
The controller has an insertion length of '0' (0x00000000).
This utility will attempt to reset the controller.
This utility has successfully reset the controller.

-----[Print the reset-command hardware log-file]-----------------------------

The scan-path will be reset by toggling the JTAG TRST signal.
The controller is the XDS110 with USB interface.
The link from controller to target is direct (without cable).
The software is configured for XDS110 features.
The controller cannot monitor the value on the EMU[0] pin.
The controller cannot monitor the value on the EMU[1] pin.
The controller cannot control the timing on output pins.
The controller cannot control the timing on input pins.
The scan-path link-delay has been set to exactly '0' (0x0000).

-----[Perform the Integrity scan-test on the JTAG IR]------------------------

This test will use blocks of 64 32-bit words.
This test will be applied just once.

Do a test using 0xFFFFFFFF.
Scan tests: 1, skipped: 0, failed: 0
Do a test using 0x00000000.
Scan tests: 2, skipped: 0, failed: 0
Do a test using 0xFE03E0E2.
Scan tests: 3, skipped: 0, failed: 0
Do a test using 0x01FC1F1D.
Scan tests: 4, skipped: 0, failed: 0
Do a test using 0x5533CCAA.
Scan tests: 5, skipped: 0, failed: 0
Do a test using 0xAACC3355.
Scan tests: 6, skipped: 0, failed: 0
All of the values were scanned correctly.

The JTAG IR Integrity scan-test has succeeded.

-----[Perform the Integrity scan-test on the JTAG DR]------------------------

This test will use blocks of 64 32-bit words.
This test will be applied just once.

Do a test using 0xFFFFFFFF.
Scan tests: 1, skipped: 0, failed: 0
Do a test using 0x00000000.
Scan tests: 2, skipped: 0, failed: 0
Do a test using 0xFE03E0E2.
Scan tests: 3, skipped: 0, failed: 0
Do a test using 0x01FC1F1D.
Scan tests: 4, skipped: 0, failed: 0
Do a test using 0x5533CCAA.
Scan tests: 5, skipped: 0, failed: 0
Do a test using 0xAACC3355.
Scan tests: 6, skipped: 0, failed: 0
All of the values were scanned correctly.

The JTAG DR Integrity scan-test has succeeded.

[End: Texas Instruments XDS110 USB Debug Probe_0]
~~~


### Future Work
Porting the system to be deployed in Localization and exploration services.
> DARPA competition

### Affiliation
 TeCIP Institute, Scuola Superiore Sant'anna.
