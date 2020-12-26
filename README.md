
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

<p align="center">
<img  src="https://github.com/astro7x/fmcw-RADAR/blob/master/figs/sysview1.svg" alt="system architicture" class="inline"/>
</p>


# System Architicture

The FMCW RADAR SOC System is divided into 3 subsystems as illustrated in here:
<p align="center">
<img  src="https://github.com/astro7x/fmcw-RADAR/blob/master/figs/sys1.svg" alt="system architicture" class="inline"/>
</p>


## Master Sub-system [MSS]

<p align="left">
<img  src="https://github.com/astro7x/fmcw-RADAR/blob/master/figs/mss_ccs.png" alt="MSS file architicture in code composer studio" class="inline"/>
</p>

MSS includes an ARM Cortex R4F processor, clocked using a MSS\_VCLK clock with a maximum operating frequency of 200 MHz. User applications executing on this processor control the overall operation of the device, including radar control through well-defined API messages, radar signal processing (assisted by the radar hardware accelerator), and peripherals for external interfaces.



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

<p align="center">
<img  src="https://github.com/astro7x/fmcw-RADAR/blob/master/figs/uart.svg" alt="uart flow" class="inline"/>
</p>

#### MAILBOX


Mailbox relies on virtual communication channels, where they are a fundamental abstraction provided by the abstract machine. Hence, Channel: is a **link** between two tasks used to exchange data. On the other hand, in Hardware machine, the channel is the communication network (ethernet, PCIe, ..etc) which Can have different semantics, and can be used using different syntax, depending on the abstract machine and on its implementation.

<p align="center">
<img  src="https://github.com/astro7x/fmcw-RADAR/blob/master/figs/mbox1.svg" alt="uart flow" class="inline"/>
</p>


#### Mailbox channel semantics
The behaviour of channel used to communicate between tasks depends on many factor: 

* The direction of the communication [Mono-directional/unidirectional or bidirectional]. 
In Mono-directional, the most commonly used cases is pipeline e.g producer consumer mechanism. However, In our case, we use **bidirectional communication channel** for the communication between **MSS** and **DSS**. To be more specific, the Bidirectional channel is implemented using 2 unidirectional virtual channels.

* The number of tasks sending and receiving. In our scenario, we have **only two tasks**, one on the MSS side and another task on the DSS side. Morever, It could be n to m tasks or 1 to m, or n to 1. Here, we have 1 to 1 tasks communicatating through two virtual channels (MSS to DSS) and (DSS to MSS).

* The kind of synchronization [Asynchronous, Synchronous, Extended]. Message queues provide an asynchronous communications protocol, meaning that the sender and receiver of the message do not need to interact with the message queue at the same time. Messages placed onto the queue are stored until the receiver  retrieves them. Message queues have implicit or explicit limits on the size of data that may be transmitted in a single message and the number of messages that may remain outstanding on the queue. The implementation of mailbox message queues function internally: within TI Real-Time operating system. 

**Maibox Message Scheme**: The processor which wishes to send a message to another processor writes the message to the mailbox memory space, then interrupts the receiver processor. The receiver processor acknowledges the interrupt, then reads the message from the mailbox memory space. The receiver informs the sender that the message is read by an interrupt, which is acknowledged back by the sender. The sender must not initiate another message to the same receiver until the previously initiated mailbox interaction with the same receiver is complete. Hence, the the register layer definitions for the Mailbox Module are included in the following path:

```C
mailbox/include/reg\_mailbox.h
```

High level encapsulation (Abstraction) have been defined in the following path:
```C
ti/drivers/mailbox/mailbox.h
```

Mailbox Initialization and Implementation withing the mmWave Radar application is implemented as shown below:

<p align="center">
<img  src="https://github.com/astro7x/fmcw-RADAR/blob/master/figs/mailbox.svg" alt="mailbox flow" class="inline"/>
</p>
Where we rely on the communication between MSS and DSS, multiple instances of the driver can be opened (each instance controls one virtual channel). Note that the mailbox driver is instantiated both in the MSS and DSS. The code sample attached is deployed on the MSS side to open a virtual channel to communicate with the DSS. Accordingly, same code is deployed in the DSS side to open a second virtual channel to communicate with the MSS


#### Recieving and Sending objects

The Task is used to handle the recieved messages from the DSS Peer  over the mailbox virtual communication channel.

<p align="center">
<img  src="https://github.com/astro7x/fmcw-RADAR/blob/master/figs/mboxIN_uartOUT.svg" alt="communication on SOC flow" class="inline"/>
</p>



## DSP Sub-system [DSS]

<p align="left">
<img  src="https://github.com/astro7x/fmcw-RADAR/blob/master/figs/dss_ccs.png" alt="DSS file architicture in code composer studio" class="inline"/>
</p>

DSS Includes TI’s standard TMS320C674x (C674x) megamodule and several blocks of internal memory (L1P, L1D, and L2).
The C674x DSP clocked at 600 MHz for advanced Radar signal processing. Core algorithms were implemented on the DSP sub-system such as the Range-FFT, Dopple-FFT, Angle-FFT, CFAR, dBSCAN, and extended kalman filter. Details about the execution flow and the algorithms will be discussed in next sections after introducing the sensing dimensions and the enabling algorithms.

### Processing Chain 

The processing chain for AWR1843 using the ultra short range chirp and frame design, is implemented on the AWR1843 EVM as shown in 
the main processing elements involved in the processing chain consist of the following:

<p align="center">
<img  src="https://github.com/astro7x/fmcw-RADAR/blob/master/figs/sysview2.svg" alt="system architicture" class="inline"/>
</p>



**Front End**
– Represents the antennas and the analog RF transceiver implementing the FMCW transmitter and receiver and various hardware-based signal conditioning operations. This must be properly configured for the chirp and frame settings of the use case. Refere to MSS and BSS sections.

**C674x DSP**
– This is the digital signal processing core that implements the configuration of the front end and executes the low-level signal processing operations on the data. This core has access to several memory resources as noted further in the design description.

**ADC**
– The ADC is the main element that interfaces to the DSP chain. The ADC output samples are buffered in ADC output buffers for access by the digital part of the processing chain.

**EDMA controller**
– This is a user-programmed DMA engine employed to move data from one memory location to another without using another processor. The EDMA can be programmed to be triggered automatically, and can also be configured to reorder some of the data during the movement operations.

**Range processing**
– For each antenna, 1D windowing, and 1D fast Fourier transform (FFT). Range processing is interleaved with the active chirp time of the frame

**Doppler processing**
– For each antenna, 2D windowing, and 2D FFT. Then non-coherent combining of received power across antennas in floating-point precision

**CFAR**
–  Range-Doppler detection algorithm is based on Constant false-alarm rate algorithm, cell averaging smallest of (CASO-CFAR) detection in range domain, plus CFAR-cell averaging (CACFAR) in Doppler domain detection, run on the range-Doppler power mapping to find detection points in range and Doppler space

**Angle estimation**
– For each detected point in range and Doppler space, reconstruct the 2D FFT output with Doppler compensation, then a beamforming algorithm is applied to calculate the angle spectrum on the azimuth direction with multiple peaks detected. After that the elevation angle is estimated for each detected peak angle in azimuth domain.

**Clustering**
– Collect all detected points and perform DBSCAN-based clustering algorithm for every fixed number of frames. The reported output includes the number of clusters and properties for each cluster, like center location and size. After the DSP finishes frame processing, the results consisting of range, doppler, 3D location, and clustering are formatted and written in shared memory \textcolor{red}{(L3RAM)} for R4F to send all the results to the host through UART for visualization.


#### ADC


The ADC buffer is on-chip memory arranged as a ping-pong buffer, with ECC (Error-correcting Code) memory support for each ping and pong memory. The raw ADC output data from RADAR-SS(BSS) is stored on this memory, to be consumed by the DSP processor. Hence, The analog signals received on each of the configured receive (Rx) channels in the device passes through a pre-conditioning over the Analog and Digital Front End (DFE) and the resulting data at the configured sampling rate is stored in the ADC buffer. Data corresponding to all the configured Rx channels is stored within this buffer. Once again, the ADC buffer is implemented as a double buffering (ping-pong) mechanism that allows for one buffer to be written(filled) while the other one is being read out(emptied).

**The size of the ADC buffer is 32 KiB for each ping and pong buffers.**

The ADC buffer can be written from DFE in any of the three modes by configuring the control registers or by using the API as in our case.

- Single-chirp mode
- Multi-chirp mode
- Continuous mode

As we operate in **continuous mode**, where the FMCW transceiver has been configured to output a single frequency tone in the range of 76-81 GHz, $N$ ADC samples are stored in a ping/pong buffer before the Ping Pong Select toggles and the Chirp Available Interrupt is generated. In real mode, this value $N$ refers to the number of real samples per channel, and in complex mode, this refers to the number of complex samples per channel. This counter increments once for every new sample (as long as 1 or more Rx channels are enabled). Continuous mode is expected to be only used for CZ and ADC buffer test pattern mode.

<p align="center">
<img  src="https://github.com/astro7x/fmcw-RADAR/blob/master/figs/adc.svg" alt="uart flow" class="inline"/>
</p>

#### ADC Data Format

The ADCBuf driver in TI-RTOS samples an analogue waveform at a specified frequency. The resulting samples are transferred to a buffer provided by the application. The driver can either take n samples once, or continuously sample by double-buffering and providing a callback to process each finished buffer.
ADC buffer Data format is written into Non-interleaved data storageto the ADC buffer. Accordingly, each channel data is stored in different memory locations, as shown below.
<p align="center">
<img  src="https://github.com/astro7x/fmcw-RADAR/blob/master/figs/adc_data.png" alt="uart flow" class="inline"/>
</p>


In the **non-interleaved mode** of storage, the ADC data corresponding to each Rx channel are grouped and stored together allowing easy processing of the related data corresponding to each channel. The storage offset for each of the channels is configurable. Also depending on the number of channels configured, the offset to store the data can be moved to allow for larger amount of data to be stored within the same buffer for reduced number of Rx channels.

The configuration of ADC buffer is shown below
<p align="center">
<img  src="https://github.com/astro7x/fmcw-RADAR/blob/master/figs/adc.svg" alt="uart flow" class="inline"/>
</p>



## RADAR Processor Sub-system [BSS]
    
> BSS is controled by MSS through mmWave SDK APIs

BSS is responsible for the RF and analog functionality of the device. The sybsystem contains the FMCW transceiver and ARM Cortex R4F-based radio control system. Hence, the subsystem is equipped with a Built-in firmware with an API interface to the on-chip Cortex-R4F application MCU for configuration, monitoring, and calibration of the low-level RF/Analog components. Access to the Radar subsystem is provided through hardware mailboxes and a well defined API controlled by the MSS sus-system which will be discussed in this document. The RADAR subsystem consists of the RF/Analog subsystem and the radio processor subsystem. The RF/analog subsystem implements the FMCW transceiver system with RF and analog circuitry – namely, the synthesizer, PA, LNA, mixer, IF, and ADC. Additionally, it includes the crystal oscillator and temperature sensors. The three transmit channels as well as the four receive channels can all be operated simultaneously. On the other hand, the radio processor subsystem includes the digital front-end, the ramp generator, and an internal processor for controlling and configuring the low-level RF/analog and ramp generator registers, based on well-defined API messages from the master subsystem. This radio processor is programmed by TI, and addresses both RF calibration needs and some basic self-test and monitoring functions (BIST); this processor is not available directly for customer use. The digital front-end filters and decimates the raw sigma-delta ADC output, and provides the final ADC data samples at a programmable sampling rate.

<p align="center">
<img  src="https://github.com/astro7x/fmcw-RADAR/blob/master/figs/sequence1.svg" alt="Sequence calls between MSS and BSS" class="inline"/>
</p>

As illustrated above, the MSS application invokes ` SOC_init()`  to initialize the device and powers up the mmWave Front End mmWave. 
It then uses the ` MMWave_init()` API to do the basic initialization of the mailbox and the other key drivers.  
Concurrently, DSP application, on the other hand, do the same initialization procedures. After that, it waits for the (FE) front end boot-up completion.  Then it uses the ` MMWave_execute()` API, which sets up the Inter process communication (IPC) to receive the data from the MSS. Both the MSS and the DSS does synchronization to check the health of each other. DSS also initializes the EDMA and the ADC buffer for the data processing. 

Once the initialization is complete and the MSS and the DSS are both synchronized, MSS application use the ` MMWave_config()` API to parse the configuration from the application to the mmWave Front End. mmWave API uses the mmWaveLink API, which constructs the mailbox message and sends it to the mmWave Front End. mmWave Front End, once it receives a message, checks the integrity of the message and sends an acknowledgement back to the mmWaveLink. In this way, all the messages are sent to the front end. 


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
