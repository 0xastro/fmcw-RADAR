On top of the DSS core, we start by initializing and configuring the mmWave and Device drivers of the core accordingly.

## mmWave data path initialization and configuration

We start by initiazizing the data path object for signal processing flow.
The first part of ```main()```is used by the application ,on top of the DSS core, to perform its one-time-only application-specific initializations. Once that is completed, ```BIOS_start()``` marks the beginning of the application's operational functioning.

> The system flow inside the ```main``` is as follow:

```C 
/* MAIN */ 
DSS_DataPathObj* obj = &gMCB.dataPathObj[0];
memset((void *) obj, 0, sizeof(DSS_DataPathObj));
/** @brief
 *	Populate the system configuration [SRR, MRR or LRR]
 */
DSS_dataPathConfigPopulate(obj);			
/** @brief
 *	Initializes states counters used for 1D-FFT processing.
 */
DSS_dataPathInit1Dstate(obj);				
/** @brief
 *	EDMA Initialization
 */
DSS_dataPathInitEdma(obj); 
/** @brief
 *	Copy linker generated table code from L3 Shared RAM to the DSS L1PSRAM using EDMA.
 *	L1SPRAM supports parity. Hence, parity diagnostic on DSP’s L1P memory is reported to the CPU
 *	as an interrupt in case of an error.
 *	This is used to page in fast code from L3 to L1PSRAM.
 */
DSS_copyTable(obj->edmaHandle[EDMA_INSTANCE_DSS], &L1PSRAM_copy_table);
/** @brief
 *  Initialize the SOC Module.
 */
SOC_init(&socCfg, &errCode);

/** @brief
 *  Initialize the Task Parameters. 
 */
Task_create(DSS_mmWaveInitTASK, &taskParams, NULL);
/** @brief
 * 	Start BIOS. 
 *	In SYS/BIOS, BIOS_start() starts up task threads which then take over the application's flow of execution.
 *	Hence, the function doesn't return.
 */
BIOS_start();
```

### DSS_dataPathConfigPopulate

Populate the chirp configuration in the DSS for the data path object by invoking ``` DSS_dataPathConfigPopulate(obj)```. Hence, the function is a hierarchical encapsulation to customize the design requirements accoroding to the use case either short, medium or long range radar (srr, mrr, or lrr). Therefore, ``` DSS_populateSRR(obj, 0)```, ``` DSS_populateMRR(obj, 0)```, or ``` DSS_populateMRR(obj, 0)``` is called to populate the data path object array with the appropriate configuration. 
```C
void DSS_dataPathConfigPopulate(DSS_DataPathObj* obj)
{

#ifdef SUBFRAME_CONF_MRR
	DSS_populateSRR(obj, subframeIndx);
#endif 
#ifdef SUBFRAME_CONF_USRR
	DSS_populateMRR(obj, subframeIndx);
#endif
#ifdef SUBFRAME_CONF_LRR
	DSS_populateLRR(obj, subframeIndx);
#endif
}
```

### SRR Profile Configuration

#### DSS_mmWaveInitTASK




