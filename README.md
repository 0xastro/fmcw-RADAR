
|The project has completed. All documentations, sequence diagrams as well as source codes will be migerated from gitlab as soon as possible|
| --- |



|The System will be reconfigured to LRR (Long Range Radar) but keeping the highst anglular resolution by operating in TDM-3Tx-12vRx Mode. This is to be used as a baseline development for railway drone obstacle avoidance use cases. Trials and field tests will be added by the end of Q4 2020. UPDATED  NOV 13, 2020| 
| --- |

|Docker container based on CCSv9 and TI mmWave SDK is currently under development to enable CI/CD using Jenkins UPDATED  NOV 13, 2020|
| --- |



# fmcw-RADAR
[mmWave based fmcw radar design files] based on AWR1843 chip operating at 76-GHz to 81-GHz.

## Objective
Test & Evaluate the performance of TI mmWave Radar system using the AWR1843 EVM development board.
In this system we detect specifically objects within close proximity the radar and generally evaluate different ranges profiles short, medium and long for further application. We use TI AWR1843 EVM, algorithms run on-board the single-chip device to create Range-Azimuth and Range-Elevation heat-maps, then performs object detection with CFAR, angle of arrival estimation and clustering on configured range rows in the heatmaps.

The mmWave FMCW Radar shows some of the capabilities of the AWR1843 SoC using the drivers in the mmWave SDK (Software Development Kit). It allows user to specify the chirping profile and displays the detected objects in range-doppler map and tracking information in real-time. In this work we exploit the usage of radar in the automotive space by deploying the basic DSP algorithms 1D-FFT, 2D-FFT to detect range and velocity of targets in front of the radar. Accordingly, expanding into more advanced processing chains by deploying unsupervised machine learning algorithm dBScan and tracking the objects using the extended Kaman filter. In this work, we test, verify and validate mmWave FMCW MIMO radar processing chain on AWR1843 platform with different sensing profiles. Different application are possible and could be easily integrated into our design by interfacing into CAN bus. Wide scenarios can be deployed on top of this system i.e object motion direction estimation, precise angular position estimation of obstacles in urban environments and ground vehicle localization in V2V application.

## System Architicture

The FMCW RADAR SOC System is divided into 3 subsystems as illustrated in here:
<p align="center">
<img  src="https://github.com/astro7x/fmcw-RADAR/blob/master/figs/sys1.svg" alt="system architicture" class="inline"/>
</p>


### RADAR Processor Sub-system [BSS]
    
> BSS is controled by MSS through mmWave SDK APIs

BSS is responsible for the RF and analog functionality of the device. The sybsystem contains the FMCW transceiver and ARM Cortex R4F-based radio control system. Hence, the subsystem is equipped with a Built-in firmware with an API interface to the on-chip Cortex-R4F application MCU for configuration, monitoring, and calibration of the low-level RF/Analog components. Access to the Radar subsystem is provided through hardware mailboxes and a well defined API controlled by the MSS sus-system which will be discussed in this document. The RADAR subsystem consists of the RF/Analog subsystem and the radio processor subsystem. The RF/analog subsystem implements the FMCW transceiver system with RF and analog circuitry – namely, the synthesizer, PA, LNA, mixer, IF, and ADC. Additionally, it includes the crystal oscillator and temperature sensors. The three transmit channels as well as the four receive channels can all be operated simultaneously. On the other hand, the radio processor subsystem includes the digital front-end, the ramp generator, and an internal processor for controlling and configuring the low-level RF/analog and ramp generator registers, based on well-defined API messages from the master subsystem. This radio processor is programmed by TI, and addresses both RF calibration needs and some basic self-test and monitoring functions (BIST); this processor is not available directly for customer use. The digital front-end filters and decimates the raw sigma-delta ADC output, and provides the final ADC data samples at a programmable sampling rate.

<p align="center">
<img  src="https://github.com/astro7x/fmcw-RADAR/blob/master/figs/sequence1.svg" alt="Sequence calls between MSS and BSS" class="inline"/>
</p>

As illustrated above, the MSS application invokes ` SOC_init()`  to initialize the device and powers up the mmWave Front End mmWave. 
It then uses the ` MMWave_init()` API to do the basic initialization of the mailbox and the other key drivers.  
Concurrently, DSP application, on the other hand, do the same initialization procedures. After that, it waits for the (FE) front end boot-up completion.  Then it uses the ` MMWave_execute()` API, which sets up the Inter process communication (IPC) to receive the data from the MSS. Both the MSS and the DSS does synchronization to check the health of each other. DSS also initializes the EDMA and the ADC buffer for the data processing. 

Once the initialization is complete and the MSS and the DSS are both synchronized, MSS application use the ` MMWave_config()` API to parse the configuration from the application to the mmWave Front End. mmWave API uses the mmWaveLink API, which constructs the mailbox message and sends it to the mmWave Front End. mmWave Front End, once it receives a message, checks the integrity of the message and sends an acknowledgement back to the mmWaveLink. In this way, all the messages are sent to the front end. 

### DSP Sub-system [DSS]

DSS Includes TI’s standard TMS320C674x (C674x) megamodule and several blocks of internal memory (L1P, L1D, and L2).
The C674x DSP clocked at 600 MHz for advanced Radar signal processing. Core algorithms were implemented on the DSP sub-system such as the Range-FFT, Dopple-FFT, Angle-FFT, CFAR, dBSCAN, and extended kalman filter. Details about the execution flow and the algorithms will be discussed in next sections after introducing the sensing dimensions and the enabling algorithms.

<p align="left">
<img  src="https://github.com/astro7x/fmcw-RADAR/blob/master/figs/dss_ccs.png" alt="DSS file architicture in code composer studio" class="inline"/>
</p>


### Master Sub-system [MSS]

MSS includes an ARM Cortex R4F processor, clocked using a MSS\_VCLK clock with a maximum operating frequency of 200 MHz. User applications executing on this processor control the overall operation of the device, including radar control through well-defined API messages, radar signal processing (assisted by the radar hardware accelerator), and peripherals for external interfaces.

<p align="left">
<img  src="https://github.com/astro7x/fmcw-RADAR/blob/master/figs/mss_ccs.png" alt="MSS file architicture in code composer studio" class="inline"/>
</p>


Below here is the entrypoint for the MSS firmware which describe the startup sequence for the AWR1843 application running on top of MSS.
The MSS application must have the following

* startup sequence at bootup time.
*  Clear all the ESM group errors and register
*  Disable the firewall for JTAG and LOGGER (UART)
*  Start the BIOS and further initialize required peripherals.

```C
int32_t main(void) {

    Task_Params     taskParams; //<  define Task_Params instance config structure
	int32_t         errCode;    //<  define an integer to retrieve error messeges
	SOC_Cfg         socCfg;     //<  define SOC_Cfg structure to to open & initialize the SOC Driver.

	/* Initialize Error Signaling Module MSS_ESM for hardware diagnostics within the MSS core */
	ESM_init(0U);
	//MCPI_Initialize();
	/* Initialize the global structure Master Control Block MCB */
	memset((void*)&gMCB, 0, sizeof(MCB));

	/* Initialize the SOC configuration: */
	memset((void *)&socCfg, 0, sizeof(SOC_Cfg));

	/* Populate the SOC configuration: Set the clock source on 200MHz */
	socCfg.clockCfg = SOC_SysClock_INIT;

	/* Initialize the SOC Module to ensure that the Memory Protection Unit
	 * (MPU) settings  are correctly configured.
	 */

	gMCB.socHandle = SOC_init (&socCfg, &errCode);
	if (gMCB.socHandle == NULL) {
		System_printf ("[MSS] \t [Error][SOC] SOC Module Initialization failed [Error code %d]\n", errCode);
		return -1;
	}

	/* Check if the SOC is a secure device */
	if (SOC_isSecureDevice(gMCB.socHandle, &errCode)) {
		/* Disable firewall for JTAG and LOGGER (UART) */
		SOC_controlSecureFirewall(gMCB.socHandle,
		                          (uint32_t)(SOC_SECURE_FIREWALL_JTAG | SOC_SECURE_FIREWALL_LOGGER),
		                          SOC_SECURE_FIREWALL_DISABLE, &errCode);
	}

    System_printf ("**********************************************************\n");
    System_printf ("[MSS] \t [DEBUG] Launching the Millimeter Wave Application\n");
    System_printf ("**********************************************************\n");

	/* Initialize the Task Parameters. */
	Task_Params_init(&taskParams);
	taskParams.priority = TASK_PRIO_1; // Default 3
	Task_create(MSS_mmWaveInitTASK, &taskParams, NULL);

	/* Start BIOS */
	BIOS_start();
	return 0;
}


```
The main task **MSS_mmWaveInitTASK** is the mss initialization task which initializes the various components in the MSS subsystem including:

* UART for logging and configurations by calling ` UART_Params_init` and `UART_open`
* Mailbox for bi-directional communication between MSS and DSS subsystems by calling `Mailbox_Config_init` and ` Mailbox_open(MAILBOX_TYPE_DSS, &mboxCfg, &errCode);`

#### UART

#### MAILBOX



Based on the range of operation, automotive radar sensors are classified into three major categories:

* **LRR**: Long Range Radar for narrow-beam, forward-looking view, like adaptive cruise control applications.
* **MRR**: Medium Range Radar for medium distance and speed profile, like cross traffic alert (CTA) applications.
* **SRR**: Short Range Radar for sensing in direct proximity of the vehicle like obstacle detection and parking aid.

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
