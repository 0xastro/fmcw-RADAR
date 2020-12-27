[![Build Status](https://travis-ci.com/astro7x/fmcw-RADAR.svg?token=gpBqYEnHhMCsw1BpRriN&branch=master)](https://travis-ci.com/astro7x/fmcw-RADAR)

|In this file, there's a minimal explanation for the system design based on TI-AWR1843 SOC. The documentation here only adresses the MSS subsytem, the DSS is not documented here, but as It's mostly designed by TI so you may refer to the Doxygen files for the documentation|
| --- |

# fmcw-RADAR
[mmWave based fmcw radar design files] based on AWR1843 chip operating at 77-GHz to 81-GHz for railway drone use-case.

# Dependencies

- Code Composer Studio (CCS) **version CCSV9_2**
- mmWave software development kit (SDK) **version 3.1.0.02**
- TI-RTOS Kernal SYS/BIOS **version 6.73.01**
- Uniflash **version 5.1.0**
- Matlab Runtime Compiler **version > 8.5.1**
- mmWave Sensing Estimator
- mmWave Demo Visualizer

## Objective
Test & Evaluate the performance of TI mmWave Radar system using the AWR1843 EVM development board.
In this system we detect specifically objects within close proximity the radar and generally evaluate different ranges profiles short, medium and long for further application. We use TI AWR1843 EVM, algorithms run on-board the single-chip device to create Range-Azimuth and Range-Elevation heat-maps, then performs object detection with CFAR, angle of arrival estimation and clustering on configured range rows in the heatmaps.

The mmWave FMCW Radar shows some of the capabilities of the AWR1843 SoC using the drivers in the mmWave SDK (Software Development Kit). It allows user to specify the chirping profile and displays the detected objects in range-doppler map and tracking information in real-time. In this work we exploit the usage of radar in the automotive space by deploying the basic DSP algorithms 1D-FFT, 2D-FFT to detect range and velocity of targets in front of the radar. Accordingly, expanding into more advanced processing chains by deploying unsupervised machine learning algorithm dBScan and tracking the objects using the extended Kaman filter. In this work, we test, verify and validate mmWave FMCW MIMO radar processing chain on AWR1843 platform with different sensing profiles. Different application are possible and could be easily integrated into our design by interfacing into CAN bus. Wide scenarios can be deployed on top of this system i.e object motion direction estimation, precise angular position estimation of obstacles in urban environments and ground vehicle localization in V2V application.


### Processing Chain 

<p align="center">
<img  src="https://github.com/astro7x/fmcw-RADAR/blob/master/figs/sysview1.svg" alt="system architicture" class="inline"/>
</p>

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

## Sensing Profiles
Based on the range of operation, automotive radar sensors are classified into three major categories:

* **LRR**: Long Range Radar for narrow-beam, forward-looking view, like adaptive cruise control applications.
* **MRR**: Medium Range Radar for medium distance and speed profile, like cross traffic alert (CTA) applications.
* **SRR**: Short Range Radar for sensing in direct proximity of the vehicle like obstacle detection and parking aid.

### Chirp configuration

Using TI-Cloud tool mmWave Sensing Estimator,[https://dev.ti.com/gallery/view/1792614/mmWaveSensingEstimator/ver/1.3.0]

The Design Rules, Restrictions and Hardware configuration for the AWR1843 device is shown below

<p align="left">
<img  src="https://github.com/astro7x/fmcw-RADAR/blob/master/figs/mmWaveSensingEstimatorTI2.svg" alt="Sequence calls between MSS and BSS" class="inline"/>
</p>

<p align="left">
<img  src="https://github.com/astro7x/fmcw-RADAR/blob/master/figs/mmWaveSensingEstimatorTI1.svg" alt="Sequence calls between MSS and BSS" class="inline"/>
</p>

Once we setup the design rules for the SRR configuration, the chirp design parameters are auto-calculated with respect to the hardware limitation such as available RAM and sampling frequencyof the sensor, as shown above.

The chirp configuration are hardcoded and located in `/common/profiles` as seen below

    srr_18xx_mss
    ├── common                                
    │   └── profiles   
    │   	└── config_chirp_design_<APPLICATION_NAME>.h       
    │   	└── config_chirp_design_USRR30.h   
    │   	└── config_chirp_design_USRR20.h   
    │   	└── config_chirp_design_MRR80.h   
    │   	└── config_chirp_design_MRR120.h   

After desining the chirp parameters based on your specific application using mmWave Sensing Estimator, you can add your own configuration, by adding a header file under the `/common/profiles` 


```C
/* config_chirp_design_USRR30.h */
/*! @brief 
 *  Ultra short range chirp profile - 20 m range, 4.3cm resolution. 
 *  better angular resolution,  approximately 18kmph max - vel.
 */

#define PROFILE_USRR_PROFILE_ID              (1U)
#define PROFILE_USRR_HPFCORNER_FREQ1_VAL     RL_RX_HPF1_175_KHz
#define PROFILE_USRR_HPFCORNER_FREQ2_VAL     RL_RX_HPF2_350_KHz
#define PROFILE_USRR_RX_GAIN_VAL             (30U)
#define PROFILE_USRR_DIGOUT_SAMPLERATE_VAL   (12500U)
#define PROFILE_USRR_ADC_SAMPLE_VAL          (512U)
#define PROFILE_USRR_IDLE_TIME_VAL           (500U)
#define PROFILE_USRR_RAMP_END_TIME_VAL       (4400U)
#define PROFILE_USRR_START_FREQ_GHZ          (77.01f)
#define PROFILE_USRR_START_FREQ_VAL          (CONV_FREQ_GHZ_TO_CODEWORD(PROFILE_USRR_START_FREQ_GHZ))
#define PROFILE_USRR_TXOUT_POWER_BACKOFF     (0U)
#define PROFILE_USRR_TXPHASESHIFTER_VAL      (0U)
#define PROFILE_USRR_FREQ_SLOPE_MHZ_PER_US   (56.25f)
#define PROFILE_USRR_FREQ_SLOPE_VAL          (CONV_SLOPE_MHZ_PER_US_TO_CODEWORD(PROFILE_USRR_FREQ_SLOPE_MHZ_PER_US))
#define PROFILE_USRR_TX_START_TIME_VAL       (100U)  // 1us
#define PROFILE_USRR_ADC_START_TIME_VAL      (300U)  // 4.8 us
#define PROFILE_USRR_LAMBDA_MILLIMETER       (MMWDEMO_SPEED_OF_LIGHT_IN_METERS_PER_USEC/PROFILE_USRR_START_FREQ_GHZ)                

#define CHIRP_USRR_0_PROFILE_ID                (1U)
#define CHIRP_USRR_0_START_INDEX               (256U + 0U)
#define CHIRP_USRR_0_END_INDEX                 (256U + 0U)
#define CHIRP_USRR_0_START_FREQ_VAL            (0U)
#define CHIRP_USRR_0_FREQ_SLOPE_VAL            (0U)
#define CHIRP_USRR_0_IDLE_TIME_VAL             (0U)
#define CHIRP_USRR_0_ADC_START_TIME_VAL        (0U)
#define CHIRP_USRR_0_TX_CHANNEL                (TX_CHANNEL_1_ENABLE)

#define CHIRP_USRR_1_PROFILE_ID                (1U)
#define CHIRP_USRR_1_START_INDEX               (256U + 1U)
#define CHIRP_USRR_1_END_INDEX                 (256U + 1U)
#define CHIRP_USRR_1_START_FREQ_VAL            (0U)
#define CHIRP_USRR_1_FREQ_SLOPE_VAL            (0U)
#define CHIRP_USRR_1_IDLE_TIME_VAL             (0U)
#define CHIRP_USRR_1_ADC_START_TIME_VAL        (0U)
#define CHIRP_USRR_1_TX_CHANNEL                (TX_CHANNEL_2_ENABLE)


#define CHIRP_USRR_2_PROFILE_ID                (1U)
#define CHIRP_USRR_2_START_INDEX               (256U + 2U)
#define CHIRP_USRR_2_END_INDEX                 (256U + 2U)
#define CHIRP_USRR_2_START_FREQ_VAL            (0U)
#define CHIRP_USRR_2_FREQ_SLOPE_VAL            (0U)
#define CHIRP_USRR_2_IDLE_TIME_VAL             (0U)
#define CHIRP_USRR_2_ADC_START_TIME_VAL        (0U)
#define CHIRP_USRR_2_TX_CHANNEL                (TX_CHANNEL_3_ENABLE)

/* SUBFRAME Configuration */
#define SUBFRAME_USRR_CHIRP_START_IDX           (CHIRP_USRR_0_START_INDEX)
#define SUBFRAME_USRR_CHIRP_END_IDX             (CHIRP_USRR_2_END_INDEX)
#define SUBFRAME_USRR_LOOP_COUNT                (32U)
#define SUBFRAME_USRR_PERIODICITY_VAL           (6000000U) // 30ms
#define SUBFRAME_USRR_TRIGGER_DELAY_VAL         (0U)
#define SUBFRAME_USRR_NUM_REAL_ADC_SAMPLES      (PROFILE_USRR_ADC_SAMPLE_VAL * 2)
#define SUBFRAME_USRR_NUM_CMPLX_ADC_SAMPLES     (PROFILE_USRR_ADC_SAMPLE_VAL)
#define SUBFRAME_USRR_CHIRPTYPE_0_NUM_CHIRPS    ((CHIRP_USRR_0_END_INDEX - CHIRP_USRR_0_START_INDEX + 1)*SUBFRAME_USRR_LOOP_COUNT)
#define SUBFRAME_USRR_CHIRPTYPE_1_NUM_CHIRPS    ((CHIRP_USRR_1_END_INDEX - CHIRP_USRR_1_START_INDEX + 1)*SUBFRAME_USRR_LOOP_COUNT)
#define SUBFRAME_USRR_CHIRPTYPE_2_NUM_CHIRPS    ((CHIRP_USRR_2_END_INDEX - CHIRP_USRR_2_START_INDEX + 1)*SUBFRAME_USRR_LOOP_COUNT)

#define SUBFRAME_USRR_NUM_TX                    (3U)
#define SUBFRAME_USRR_NUM_VIRT_ANT              (SUBFRAME_USRR_NUM_TX*NUM_RX_CHANNELS)
#define SUBFRAME_USRR_NUM_ANGLE_BINS            (64U)
#define SUBFRAME_USRR_NUM_CHIRPS_TOTAL          ((SUBFRAME_USRR_CHIRP_END_IDX - SUBFRAME_USRR_CHIRP_START_IDX + 1) * SUBFRAME_USRR_LOOP_COUNT)

/*! @brief Derived parameters. */
#define PROFILE_USRR_RANGE_RESOLUTION_METERS     ((MMWDEMO_SPEED_OF_LIGHT_IN_METERS_PER_USEC * PROFILE_USRR_DIGOUT_SAMPLERATE_VAL)/ (2000.0f * PROFILE_USRR_FREQ_SLOPE_MHZ_PER_US * SUBFRAME_USRR_NUM_CMPLX_ADC_SAMPLES))

/* The USRR mode uses 3 Tx in TDM MIMO mode. As such every chirp repetition period will be the same. */
#define SUBFRAME_USRR_CHIRP_REPETITION_PERIOD_US ((( PROFILE_USRR_IDLE_TIME_VAL + PROFILE_USRR_RAMP_END_TIME_VAL)/100.0f))

#define SUBFRAME_USRR_VEL_RESOLUTION_M_P_S       (((1000.0f/(3.0f*SUBFRAME_USRR_CHIRP_REPETITION_PERIOD_US))/SUBFRAME_USRR_CHIRPTYPE_0_NUM_CHIRPS)*(PROFILE_USRR_LAMBDA_MILLIMETER/2.0f))
#define SUBFRAME_USRR_MAX_VEL_M_P_S     	 (SUBFRAME_USRR_VEL_RESOLUTION_M_P_S*SUBFRAME_USRR_CHIRPTYPE_0_NUM_CHIRPS/2)

#define SUBFRAME_USRR_MIN_SNR_dB 		 (22.0f)

#define SUBFRAME_USRR_NUM_CHIRPTYPES 		 (3U)
```

After adding your own configuration, you should include it the in `app_cfg.h` which is located in

    srr_18xx_mss
    ├── common                                
    │   └── app_cfg.h   


```C
#include <common/profiles/config_chirp_design_USRR30.h>
```

### Frame Configuration
  
    srr_18xx_mss
    ├── common                                
    │   └── frame_cfg.h   

```C
/*frame_cfg.c*/
    #ifdef SUBFRAME_CONF_USRR
        /* The high resolution 20m subframe 
	 * Hence: We use only one subframe in our design USRR
	 */
        ptrAdvFrameCfg->frameSeq.subFrameCfg[0].forceProfileIdx    = 0;
        ptrAdvFrameCfg->frameSeq.subFrameCfg[0].numLoops           = SUBFRAME_USRR_LOOP_COUNT;
        ptrAdvFrameCfg->frameSeq.subFrameCfg[0].numOfBurst         = 1;
        ptrAdvFrameCfg->frameSeq.subFrameCfg[0].numOfBurstLoops    = 1;
        ptrAdvFrameCfg->frameSeq.subFrameCfg[0].chirpStartIdxOffset= 0;
        ptrAdvFrameCfg->frameSeq.subFrameCfg[0].numOfChirps        = SUBFRAME_USRR_CHIRP_END_IDX - SUBFRAME_USRR_CHIRP_START_IDX + 1;
        ptrAdvFrameCfg->frameSeq.subFrameCfg[0].chirpStartIdx      = SUBFRAME_USRR_CHIRP_START_IDX;
        ptrAdvFrameCfg->frameSeq.subFrameCfg[0].subFramePeriodicity= SUBFRAME_USRR_PERIODICITY_VAL;
        ptrAdvFrameCfg->frameSeq.subFrameCfg[0].burstPeriodicity   = SUBFRAME_USRR_PERIODICITY_VAL;
        ptrAdvFrameCfg->frameData.subframeDataCfg[0].numAdcSamples = PROFILE_USRR_ADC_SAMPLE_VAL*2;
        ptrAdvFrameCfg->frameData.subframeDataCfg[0].totalChirps   = SUBFRAME_USRR_NUM_CHIRPS_TOTAL;
        ptrAdvFrameCfg->frameData.subframeDataCfg[0].numChirpsInDataPacket = 1;
        numOfSubFrame++;
    #endif

void Cfg_FrameCfgInitParams (rlFrameCfg_t* ptrFrameCfg) {
    /* Initialize the configuration: */
    memset ((void*)ptrFrameCfg, 0, sizeof(rlFrameCfg_t));

    /* Populate the default configuration: */
    ptrFrameCfg->chirpEndIdx        = FRAME_CHIRP_END_IDX;
    ptrFrameCfg->chirpStartIdx      = FRAME_CHIRP_START_IDX;
    ptrFrameCfg->numFrames          = FRAME_COUNT_VAL;
    ptrFrameCfg->numLoops           = FRAME_LOOP_COUNT;
    ptrFrameCfg->triggerSelect      = RL_FRAMESTRT_SYNCIN_TRIGGER;
    ptrFrameCfg->framePeriodicity   = FRAME_PERIODICITY_VAL;
    ptrFrameCfg->frameTriggerDelay  = FRAME_TRIGGER_DELAY_VAL;
    ptrFrameCfg->numAdcSamples      = FRAME_NUM_REAL_ADC_SAMPLES;
    return;
}

void Cfg_ProfileCfgInitParams (uint8_t profileNum, rlProfileCfg_t* ptrProfileCfg) {
    /* Initialize the configuration: */
    memset ((void*)ptrProfileCfg, 0, sizeof(rlProfileCfg_t));

    if (profileNum == 0U) {
        /* Populate the default configuration for profile 1  */
        ptrProfileCfg->profileId             = PROFILE_USRR_PROFILE_ID;
        ptrProfileCfg->startFreqConst        = PROFILE_USRR_START_FREQ_VAL;
        ptrProfileCfg->idleTimeConst         = PROFILE_USRR_IDLE_TIME_VAL;
        ptrProfileCfg->adcStartTimeConst     = PROFILE_USRR_ADC_START_TIME_VAL;
        ptrProfileCfg->rampEndTime           = PROFILE_USRR_RAMP_END_TIME_VAL;
        ptrProfileCfg->txOutPowerBackoffCode = PROFILE_USRR_TXOUT_POWER_BACKOFF;
        ptrProfileCfg->txPhaseShifter        = PROFILE_USRR_TXPHASESHIFTER_VAL;
        ptrProfileCfg->freqSlopeConst        = PROFILE_USRR_FREQ_SLOPE_VAL;
        ptrProfileCfg->txStartTime           = PROFILE_USRR_TX_START_TIME_VAL;
        ptrProfileCfg->numAdcSamples         = PROFILE_USRR_ADC_SAMPLE_VAL;
        ptrProfileCfg->digOutSampleRate      = PROFILE_USRR_DIGOUT_SAMPLERATE_VAL;
        ptrProfileCfg->hpfCornerFreq1        = PROFILE_USRR_HPFCORNER_FREQ1_VAL;
        ptrProfileCfg->hpfCornerFreq2        = PROFILE_USRR_HPFCORNER_FREQ2_VAL;
        ptrProfileCfg->rxGain                = PROFILE_USRR_RX_GAIN_VAL;
    }
    return;
}


void Cfg_ChirpCfgInitParams (uint8_t chirpNum, rlChirpCfg_t* ptrChirpCfg) {
    /* Initialize the configuration: */
    memset ((void*)ptrChirpCfg, 0, sizeof(rlChirpCfg_t));



    if (chirpNum == 0U) {
        /* Populate the default configuration for chirp 3
         *  - USRR Tx1 . */
        ptrChirpCfg->profileId       = CHIRP_USRR_0_PROFILE_ID;
        ptrChirpCfg->adcStartTimeVar = CHIRP_USRR_0_ADC_START_TIME_VAL;
        ptrChirpCfg->chirpEndIdx     = CHIRP_USRR_0_END_INDEX;
        ptrChirpCfg->chirpStartIdx   = CHIRP_USRR_0_START_INDEX;
        ptrChirpCfg->idleTimeVar     = CHIRP_USRR_0_IDLE_TIME_VAL;
        ptrChirpCfg->txEnable        = CHIRP_USRR_0_TX_CHANNEL;
        ptrChirpCfg->startFreqVar    = CHIRP_USRR_0_START_FREQ_VAL;
        ptrChirpCfg->freqSlopeVar    = CHIRP_USRR_0_FREQ_SLOPE_VAL;
    }
    else if (chirpNum == 1U) {
        /* Populate the default configuration for chirp 3
         *  - USRR Tx2. */
        ptrChirpCfg->profileId       = CHIRP_USRR_1_PROFILE_ID;
        ptrChirpCfg->adcStartTimeVar = CHIRP_USRR_1_ADC_START_TIME_VAL;
        ptrChirpCfg->chirpEndIdx     = CHIRP_USRR_1_END_INDEX;
        ptrChirpCfg->chirpStartIdx   = CHIRP_USRR_1_START_INDEX;
        ptrChirpCfg->idleTimeVar     = CHIRP_USRR_1_IDLE_TIME_VAL;
        ptrChirpCfg->txEnable        = CHIRP_USRR_1_TX_CHANNEL;
        ptrChirpCfg->startFreqVar    = CHIRP_USRR_1_START_FREQ_VAL;
        ptrChirpCfg->freqSlopeVar    = CHIRP_USRR_1_FREQ_SLOPE_VAL;

    }
    else if (chirpNum == 2U) {
        /* Populate the default configuration for chirp 4
         *  - USRR Tx3. */
        ptrChirpCfg->profileId       = CHIRP_USRR_2_PROFILE_ID;
        ptrChirpCfg->adcStartTimeVar = CHIRP_USRR_2_ADC_START_TIME_VAL;
        ptrChirpCfg->chirpEndIdx     = CHIRP_USRR_2_END_INDEX;
        ptrChirpCfg->chirpStartIdx   = CHIRP_USRR_2_START_INDEX;
        ptrChirpCfg->idleTimeVar     = CHIRP_USRR_2_IDLE_TIME_VAL;
        ptrChirpCfg->txEnable        = CHIRP_USRR_2_TX_CHANNEL;
        ptrChirpCfg->startFreqVar    = CHIRP_USRR_2_START_FREQ_VAL;
        ptrChirpCfg->freqSlopeVar    = CHIRP_USRR_2_FREQ_SLOPE_VAL;

    }

    return;
}

void Cfg_ChannelCfgInitParams (rlChanCfg_t* ptrChannelCfg) {

    /* Initialize the configuration: */
    memset ((void*)ptrChannelCfg, 0, sizeof(rlChanCfg_t));

    /* Populate the default configuration: */
    ptrChannelCfg->rxChannelEn = RX_CHANNEL_1_2_3_4_ENABLE;
    ptrChannelCfg->txChannelEn = TX_CHANNEL_1_2_3_ENABLE;
    ptrChannelCfg->cascading   = 0; /* Single Chip (no cascading) Only available by using AWR1243 SOC*/

    return;
}


void Cfg_ADCOutCfgInitParams (rlAdcOutCfg_t* ptrADCOutCfg) {

    /* Initialize the configuration: */
    memset ((void*)ptrADCOutCfg, 0, sizeof(rlAdcOutCfg_t));

    /* Populate the default configuration: */
    ptrADCOutCfg->fmt.b2AdcBits				= ADC_BITS_16;
    ptrADCOutCfg->fmt.b2AdcOutFmt			= ADC_FORMAT_COMPLEX;
    ptrADCOutCfg->fmt.b8FullScaleReducFctr	= 0;

    return;
}

```


### Device Configuraion

    srr_18xx_mss
    ├── common                                
    │   └── device_cfg.h   

```C

#ifndef DEVICE_CFG_H
#define DEVICE_CFG_H

/* Tx Channel Configuration */
#define TX_CHANNEL_1_ENABLE                 (1U << 0U)
#define TX_CHANNEL_2_ENABLE                 (1U << 1U)
#define TX_CHANNEL_3_ENABLE                 (1U << 2U)
#define TX_CHANNEL_1_2_ENABLE               (TX_CHANNEL_1_ENABLE | TX_CHANNEL_2_ENABLE)
#define TX_CHANNEL_2_3_ENABLE               (TX_CHANNEL_2_ENABLE | TX_CHANNEL_3_ENABLE)
#define TX_CHANNEL_1_3_ENABLE               (TX_CHANNEL_1_ENABLE | TX_CHANNEL_3_ENABLE)
#define TX_CHANNEL_1_2_3_ENABLE             (TX_CHANNEL_1_ENABLE | TX_CHANNEL_2_ENABLE | TX_CHANNEL_3_ENABLE)

/* Rx Channel Configuration */
#define RX_CHANNEL_1_ENABLE                 (1U << 0U)
#define RX_CHANNEL_2_ENABLE                 (1U << 1U)
#define RX_CHANNEL_3_ENABLE                 (1U << 2U)
#define RX_CHANNEL_4_ENABLE                 (1U << 3U)
#define RX_CHANNEL_1_2_ENABLE               (RX_CHANNEL_1_ENABLE | RX_CHANNEL_2_ENABLE)
#define RX_CHANNEL_1_3_ENABLE               (RX_CHANNEL_1_ENABLE | RX_CHANNEL_3_ENABLE)
#define RX_CHANNEL_1_4_ENABLE               (RX_CHANNEL_1_ENABLE | RX_CHANNEL_4_ENABLE)
#define RX_CHANNEL_2_3_ENABLE               (RX_CHANNEL_2_ENABLE | RX_CHANNEL_3_ENABLE)
#define RX_CHANNEL_2_4_ENABLE               (RX_CHANNEL_2_ENABLE | RX_CHANNEL_4_ENABLE)
#define RX_CHANNEL_3_4_ENABLE               (RX_CHANNEL_3_ENABLE | RX_CHANNEL_4_ENABLE)
#define RX_CHANNEL_1_2_3_ENABLE             (RX_CHANNEL_1_ENABLE | RX_CHANNEL_2_ENABLE | RX_CHANNEL_3_ENABLE)
#define RX_CHANNEL_2_3_4_ENABLE             (RX_CHANNEL_2_ENABLE | RX_CHANNEL_3_ENABLE | RX_CHANNEL_4_ENABLE)
#define RX_CHANNEL_1_3_4_ENABLE             (RX_CHANNEL_1_ENABLE | RX_CHANNEL_3_ENABLE | RX_CHANNEL_4_ENABLE)
#define RX_CHANNEL_1_2_3_4_ENABLE           (RX_CHANNEL_1_ENABLE | RX_CHANNEL_2_ENABLE | RX_CHANNEL_3_ENABLE | RX_CHANNEL_4_ENABLE)

/* ADC Config Settings */
#define ADC_BITS_12                         (0U)
#define ADC_BITS_14                         (1U)
#define ADC_BITS_16                         (2U)

#define ADC_FORMAT_REAL                     (0U)
#define ADC_FORMAT_COMPLEX                  (1U)
#define ADC_FORMAT_CPMLEX_WITH_IMG_BAND     (2U)

#define ADC_I_FIRST                         (0U)
#define ADC_Q_FIRST                         (1U)

#define ADC_INTERLEAVED_MODE                (0U)
#define ADC_NON_INTERLEAVED_MODE            (1U)

/* Data Path Configuration */
#define DATA_PATH_CSI2                      (0U)
#define DATA_PATH_LVDS                      (1U)


#define DATA_PATH_FMT1_SUPRESS              (0U)
#define DATA_PATH_FMT1_CP_CQ                (1U)
#define DATA_PATH_FMT1_CQ_CP                (2U)

#define DATA_PATH_FMT0_ADC_DATA_ONLY        (0U)
#define DATA_PATH_FMT0_CP_ADC_DATA          (1U)
#define DATA_PATH_FMT0_ADC_CP_DATA          (2U)
#define DATA_PATH_FMT0_CP_ADC_CQ_DATA       (3U)

#define DATA_PATH_CQ_FMT_BITS_12            (0U)
#define DATA_PATH_CQ_FMT_BITS_14            (1U)
#define DATA_PATH_CQ_FMT_BITS_16            (2U)

/* LVDS Clock Configuration */
#define LVDS_LANE_CLOCK_SDR                 (0U)
#define LVDS_LANE_CLOCK_DDR                 (1U)

#define LVDS_ALL_LANE_EN                    (0xFU)

#define LVDS_DATA_RATE_900                  (0U)
#define LVDS_DATA_RATE_600                  (1U)
#define LVDS_DATA_RATE_450                  (2U)
#define LVDS_DATA_RATE_400                  (3U)
#define LVDS_DATA_RATE_300                  (4U)
#define LVDS_DATA_RATE_225                  (5U)
#define LVDS_DATA_RATE_150                  (6U)

/* LCDS Lane Configuration */
#define LVDS_LANE1_DISABLE                  (0U)
#define LVDS_LANE1_FORMAT_0                 (1U)
#define LVDS_LANE1_FORMAT_1                 (2U)

#define LVDS_LANE2_DISABLE                  (0U)
#define LVDS_LANE2_FORMAT_0                 (1U)
#define LVDS_LANE2_FORMAT_1                 (2U)

#define LVDS_LANE3_DISABLE                  (0U)
#define LVDS_LANE3_FORMAT_0                 (1U)
#define LVDS_LANE3_FORMAT_1                 (2U)

#define LVDS_LANE4_DISABLE                  (0U)
#define LVDS_LANE4_FORMAT_0                 (1U)
#define LVDS_LANE4_FORMAT_1                 (2U)

#define LVDS_LANE_MSB_FIRST_ENABLE          (1U)
#define LVDS_LANE_MSB_FIRST_DISABLE         (0U)
#define LVDS_LANE_PACKET_END_PULSE_ENABLE   (1U)
#define LVDS_LANE_PACKET_END_PULSE_DISABLE  (0U)
#define LVDS_LANE_CRC_ENABLE                (1U)
#define LVDS_LANE_CRC_DISABLE               (0U)
#define LVDS_LANE_TI_MODE_ENABLE            (1U)
#define LVDS_LANE_TI_MODE_DISABLE           (0U)

/* Advanced configuration */
#define ANA_CHANNEL_COMPLEX_CHAIN           (0U)
#define ANA_CHANNEL_REAL_CHAIN              (1U)

#define LP_ADC_MODE_REGULAR                 (0U)
#define LP_ADC_MODE_LOW_POWER               (1U)

#define NOISE_FIGURE_LOW                    (0U)
#define NOISE_FIGURE_HIGH                   (1U)

/* CHIRP Profile Settings */
#define CHIRP_HPF1_CORNER_FREQ_175K         (0U)
#define CHIRP_HPF1_CORNER_FREQ_235K         (1U)
#define CHIRP_HPF1_CORNER_FREQ_350K         (2U)
#define CHIRP_HPF1_CORNER_FREQ_700K         (3U)

#define CHIRP_HPF2_CORNER_FREQ_350K         (0U)
#define CHIRP_HPF2_CORNER_FREQ_700K         (1U)
#define CHIRP_HPF2_CORNER_FREQ_1_4M         (2U)
#define CHIRP_HPF2_CORNER_FREQ_2_8M         (3U)
#define CHIRP_HPF2_CORNER_FREQ_5M           (4U)
#define CHIRP_HPF2_CORNER_FREQ_7_5M         (5U)
#define CHIRP_HPF2_CORNER_FREQ_10M          (6U)
#define CHIRP_HPF2_CORNER_FREQ_15M          (7U)

/* Some MACROS to simplify programming the device */
#define ROUND_TO_INT32(X)    ((int32_t) (X))
#define CONV_FREQ_GHZ_TO_CODEWORD(X) ROUND_TO_INT32(X * (1.0e9/53.644))
#define CONV_SLOPE_MHZ_PER_US_TO_CODEWORD(X) (ROUND_TO_INT32(X * (1000.0/48.279)))

#define LOG2_APPROX(X) ((X <= 1)? 0:((X <= 2)? 1:((X <= 4)? 2:((X <= 8)? 3:((X <= 16)? 4:((X <= 32)? 5:((X <= 64)? 6:((X <= 128)? 7:((X <= 256)? 8:((X <= 512)? 9:((X <= 1024)? 10:11)))))))))))

#define SPEED_OF_LIGHT_IN_METERS_PER_SEC (3.0e8)

#define SPEED_OF_LIGHT_IN_METERS_PER_USEC (3.0e2) 


#endif

```


# Getting Started 

## CCS Debug utility

This utility can be flashed on the QSPI The application is written over SYSBIOS and will loop forever. In the meantime CCS can be attached and the developers can then download the real application which needs to be debugged.
This utility provides while loop application for MSS in XWR14xx and for MSS and DSS in XWR16xx/XWR18xx/XWR68xx.
On MSS, it calls ESM and SOC init functions to initialize the device in known state. ESM init is needed to install the FIQ handler in case there are ESM errors on bootup. SOC init is needed to unhalt DSP in XWR16xx/XWR18xx/XWR68xx. As a byproduct, SOC init also unhalts the BSS and performs the APLL close loop procedure. Hence when this debug utility is executed from flash, the system is running using the APLL clocks.


> Debug mode: Downloading and running the executable (.xer4f image) from CCS. You will need to flash a small CCS debug firmware on the EVM (one time) to allow connecting with CCS. This debug firmware image is provided with the mmWave SDK.

> To prepare the EVM for debug mode, we start with flashing the CCS debug firmware image.

- Power on the EVM using a 5V/2.5A power supply.
- Connect the EVM to your PC and check the COM ports. The EVM exports two virtual COM ports as shown below:
- - XDS110 Class Application/User UART (COMUART): Used for passing configuration data and firmware to the EVM
- - XDS110 Class Auxiliary Data Port (COMAUX): Used to send processed radar data output
- Set the Dip Switch SOP0=ON; SOP1=OFF; SOP2=ON
- Open the UniFlash tool [TI.com/tool/uniflash]
- - In the New Configuration section, locate and select the appropriate device AWR1843BOOST
- - Click Start to proceed
- - In the Program tab, browse and locate the generated binary file 
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


## Connecting EVM to CCS

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


# Continuous Integration and Continuous Development

Travis CI host our CI/CD flow by building the docker image `Dockerfile.ccs.mmwave.ubuntu18`. So far the test flow is to setup the environment and the dependencies for system development. Travis is triggered by every pull request.
**TODO1** Add source files using COPY function, and build DSS and MSS
**TODO2** Add testcases

### Affiliation
 TeCIP Institute, Scuola Superiore Sant'anna.
