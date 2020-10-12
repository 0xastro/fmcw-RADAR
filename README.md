
|The project has completed. All documentations, sequence diagrams as well as source codes will be migerated from gitlab as soon as possible|
| --- |


# fmcw-RADAR
[mmWave based fmcw radar design files] based on AWR1843 chip operating at 76-GHz to 81-GHz.

### Objective
Test & Evaluate the performance of TI mmWave Radar system using the AWR1843 EVM development board.

In this system we detect specifically objects within close proximity the radar and generally evaluate different ranges profiles short, medium and long for further application.

We use TI AWR1843 EVM, algorithms run on-board the single-chip device to create Range-Azimuth and Range-Elevation heat-maps, then performs object detection with CFAR, angle of arrival estimation and clustering on configured range rows in the heatmaps.

### Dependencies

### Getting Started 

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

``` bash
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
```

### Future Work
Porting the system to be deployed in Localization and exploration services.
> DARPA competition

### Affiliation
 TeCIP Institute, Scuola Superiore Sant'anna.
