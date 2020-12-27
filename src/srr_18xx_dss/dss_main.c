/*******************************************************************************
 * @file    dss_main.c
 * @brief   The application runs on top of the DSP Sub-system (DSS)
 *          which is powered by a C675x DSP processor
 *          Main tasks is to running Perform DSP tasks.
 *          Must TASKS:     FFT (1D, 2D and 3D),
 *          Optional TASKS: Extended Kalman Filter, and dBScan
 * @author  A. Astro, astro@bridgeit.tech
 * @date    Feb 24, 2020
 * @version 0.1
 *
 * @par
 *  NOTE:
 *      (C) Copyright 2018 Texas Instruments, Inc.
 *******************************************************************************/


/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

// Standard Include Files
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

// BIOS/XDC  Include Files
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/heaps/HeapBuf.h>
#include <ti/sysbios/heaps/HeapMem.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/family/c64p/Cache.h>
#include <ti/sysbios/family/c64p/Hwi.h>
#include <ti/sysbios/family/c64p/EventCombiner.h>
#include <ti/sysbios/utils/Load.h>

// mmWave SDK Include Files
#include <ti/common/sys_common.h>
#include <ti/common/mmwave_sdk_version.h>
#include <ti/utils/cycleprofiler/cycle_profiler.h>

// TI Drivers Include Files
#include <ti/drivers/mailbox/mailbox.h>
#include <ti/drivers/adcbuf/ADCBuf.h>
#include <ti/drivers/esm/esm.h>
#include <ti/drivers/pinmux/pinmux.h>
#include <ti/drivers/soc/soc.h>

// Application Include Files
#include "common/mmWave_XSS.h"
#include "dss_data_path.h"
#include "EKF_XYZ_Interface.h"
#include "common/mmw_messages.h"
#include "common/app_cfg.h"

// Related to linker copy table for copying from L3 to L1PSRAM for example
#include <cpy_tbl.h>


/**
 * @brief
 *  DSS stores the application output and DSS2MSS ISR information
 *  (for fast exception signalling) in HSRAM.
 */

#define SOC_XWR18XX_DSS_HSRAM_SIZE          0x8000U        /* Size: 32KB */

typedef struct DSS2MSS_HSRAM {
#define DATAPATH_DET_PAYLOAD_SIZE (SOC_XWR18XX_DSS_HSRAM_SIZE -  sizeof(uint8_t))
    uint8_t  dataPathDetectionPayload[DATAPATH_DET_PAYLOAD_SIZE];       ///> data path processing/detection related msg payloads, these msgs are signalled through DSS to MSS mailbox
    uint8_t  dss2MssIsrInfo;                                            ///> Information relayed through DSS triggering software interrupt to MSS. It stores one of the exception IDs @ref DSS_TO_MSS_EXCEPTION_IDS
} DSS2MSS_HSRAM_t;


#pragma DATA_SECTION(gHSRAM, ".appSharedMem");
#pragma DATA_ALIGN(gHSRAM, 4);
DSS2MSS_HSRAM_t gHSRAM;

#define ADCBuf_config_Index					0
#define continousMode_OFF					0
#define continousMode_ON					0
#define ADC_OUT_FORMAT_COMPLX				0
#define ADC_OUT_FORMAT_REAL					1
#define CH_INTERLEAVED_MODE_ENABLED    		0
#define CH_INTERLEAVED_MODE_DISABLED    	1
#define SAMPLE_INTERLEAVED_MODE_I_LSB     	0
#define SAMPLE_INTERLEAVED_MODE_Q_LSB    	1

///> @brief Azimuth FFT size
#define MMW_NUM_ANGLE_BINS 64
#define SPEED_OF_LIGHT_IN_METERS_PER_SEC (3.0e8)
#define SPEED_OF_LIGHT_IN_METERS_PER_USEC (3.0e2) 
extern far COPY_TABLE L1PSRAM_copy_table;

//#pragma DATA_ALIGN(gMCB, 16);
/** ---------------------------------------------------
 *  \brief gMCB structure contains the tracking
 *  information required by the design
 *  is aligned using DATA_ALIGN paragma
 *  to increase the performance of DSS core by aligning
 *  the structure gMCB (Master Control Block) to be
 *  divisable by 16.
 *  TI-reference: spnu151j.pdf section #5.10.6
 *  ----------------------------------------------------
 */
MCB gMCB;

volatile cycleLog_t gCycleLog;


/* ******************************************************************************
   mmWave SPECIFIC Functions
 *******************************************************************************/
static void DSS_mmWaveInitTASK(UArg arg0, UArg arg1);
static void DSS_mmWaveCtrlTask(UArg arg0, UArg arg1);
static void DSS_chirpIntCallback(uintptr_t arg);      /* Internal Interrupt handler */
static void DSS_frameStartIntCallback(uintptr_t arg); /* Internal Interrupt handler */
/* ******************************************************************************
   Data Path SPECIFIC Functions to configure ADC and processing path
 *******************************************************************************/
static int32_t DSS_DataPathConfigAdcBuf();
void DSS_dataPathConfigPopulate(DSS_DataPathObj* obj);
void DSS_populateUSRR(DSS_DataPathObj* obj, uint16_t subframeIndx);
void DSS_dBScanConfigBuffers(DSS_DataPathObj * obj);
/* ******************************************************************************
   EDMA and Copy Tables SPECIFIC Functions
 *******************************************************************************/
static void DSS_edmaBlockCopy(EDMA_Handle handle, uint32_t loadAddr, uint32_t runAddr, uint16_t size);
static void DSS_copyTable(EDMA_Handle handle, COPY_TABLE *tp);


/*  DataPath Functions */
int32_t DSS_DataPathInit(void);


/* Output logging.  */
static int32_t DSS_SendProcessOutputToMSS(uint8_t *ptrHsmBuffer, uint32_t outputBufSize, DSS_DataPathObj *obj);
static void DSS_DataPathOutputLogging(DSS_DataPathObj * dataPathObj);
static void mboxIn_uartOut_TASK();
void mboxCallbackFxn_MSS_ch0(Mbox_Handle handle, Mailbox_Type peer);

/* external sleep function when in idle (used in .cfg file) */
void DSP_sleep(void);

/* Utility functions. */
uint16_t convertSNRdBtoThreshold(uint16_t numInteg, float ThresholdIndB, uint16_t bitwidth);
static int32_t DSS_mboxWrite(mmWaveMSG *message);




/**************************************************************************
 ********************** DSS MRR TI Design Functions ***********************
 **************************************************************************/

/*@**********************************************************************
 * DSS_chirpIntCallback
 ************************************************************************/
/**
 *  \brief
 *   This is the callback function registered with the ADC Driver which is invoked
 *   when a chirp is available. This is executed in the ISR context.
 *  \arg[0] arg > Application registered argument
 *  \retval void
 */
static void DSS_chirpIntCallback(uintptr_t arg)
{
    gMCB.chirpProcToken = 1;
    gMCB.stats.chirpIntCounter++;
    return;
}

/*@**********************************************************************
 * DSS_frameStartIntCallback
 ************************************************************************/
/**
 *  \brief
 *  This is the callback function registered when a frame is triggered.
 *  This is executed in the ISR context.
 *  \arg[0]  Application registered argument
 *  \return  void
 */

static void DSS_frameStartIntCallback(uintptr_t arg)
{
    gMCB.frameStartIntToken = 1; 
    gMCB.stats.frameStartIntCounter++;
    return;
}


/**
 *  @b Description
 *  @n
 *      This is the task which provides an execution context for
 *      the mmWave control module.
 *
 *  @param[in]  arg0
 *      Argument0 with which the task was created
 *  @param[in]  arg1
 *      Argument1 with which the task was created
 *
 *  @retval
 *      Not Applicable.
 */
static void DSS_mmWaveCtrlTask(UArg arg0, UArg arg1)
{
    DSS_DataPathObj * dataPathObj;
    volatile uint32_t startTime;
    gMCB.subframeIndx = 0;

    while (1)
    {
        if (gMCB.frameStartIntToken == 1)
        {
            gMCB.frameStartIntToken = 0;

            dataPathObj = &gMCB.dataPathObj[gMCB.subframeIndx];
            
            /* Increment event stats */
            gMCB.stats.frameStartEvt++;
            
            /* Check if the previous frame has been completely processed. */
            dssAssert(dataPathObj->chirpCount != 0);
        }
        else if (gMCB.chirpProcToken == 1)
        {
            /* We have begun processing a frame */
            gMCB.frameProcToken = 1;                                
            dataPathObj = &gMCB.dataPathObj[gMCB.subframeIndx];

            /* Increment event stats */
            gMCB.stats.chirpEvt++;
            MmwDemo_processChirp(dataPathObj, gMCB.subframeIndx);
            gMCB.chirpProcToken--;

            dataPathObj->timingInfo.chirpProcessingEndTime = Cycleprofiler_getTimeStamp();

            if (dataPathObj->chirpCount == 0)
            {
                MmwDemo_waitEndOfChirps(dataPathObj, gMCB.subframeIndx);

                dataPathObj->cycleLog.interChirpProcessingTime = gCycleLog.interChirpProcessingTime;
                dataPathObj->cycleLog.interChirpWaitTime = gCycleLog.interChirpWaitTime;
                gCycleLog.interChirpProcessingTime = 0;
                gCycleLog.interChirpWaitTime = 0;

                startTime = Cycleprofiler_getTimeStamp();
                MmwDemo_interFrameProcessing(dataPathObj, gMCB.subframeIndx);
                dataPathObj->timingInfo.interFrameProcCycles = Cycleprofiler_getTimeStamp() - startTime;

                dataPathObj->cycleLog.interFrameProcessingTime = gCycleLog.interFrameProcessingTime;
                dataPathObj->cycleLog.interFrameWaitTime = gCycleLog.interFrameWaitTime;
                gCycleLog.interFrameProcessingTime = 0;
                gCycleLog.interFrameWaitTime = 0;

                /* Sending detected objects to logging buffer */
                DSS_DataPathOutputLogging(dataPathObj);
                dataPathObj->timingInfo.interFrameProcessingEndTime = Cycleprofiler_getTimeStamp();

                /* Update the subframeIndx */
                {
                    gMCB.subframeIndx ++;
                    if (gMCB.subframeIndx >= NUM_SUBFRAMES)
                    {
                        gMCB.subframeIndx = 0;
                    }
                    
                    /* Program the ADC for the next subframe. */
                    dataPathObj = &gMCB.dataPathObj[gMCB.subframeIndx];                    
                }
            }
            
            gMCB.frameProcToken = 0;
        }
        else if (gMCB.mboxProcToken == 1) 
        {
            gMCB.mboxProcToken = 0;
            /* If the mailbox has a message and the frame processing task has finished. */
            mboxIn_uartOut_TASK();
        }
    }
}

/**
 *  @b Description
 *  @n
 *     Function to sleep the DSP using IDLE instruction. When DSP has no work left to do,
 *     the BIOS will be in Idle thread and will call this function. The DSP will
 *     wake-up on any interrupt (e.g chirp interrupt).
 *
 *  @retval
 *      Not Applicable.
 */
void DSP_sleep(void)
{
    /* issue IDLE instruction */
    asm(" IDLE ");
}

/**
 *  @brief
 *  Sends DSS assert information to MSS
 *
 *  @retval void.
 */
void _dssAssert(int32_t expression,const char *file, int32_t line)
{
    mmWaveMSG        message;
    uint32_t         nameSize;

    if (!expression) 
    {
        message.type = MBOX_DSS2MSS_ASSERT_INFO;
        nameSize = strlen(file);
        if(nameSize > MMWAVE_MAX_FILE_NAME_SIZE)
            nameSize = MMWAVE_MAX_FILE_NAME_SIZE;
            
        memcpy((void *) &message.body.assertInfo.file[0], (void *)file, nameSize);
        message.body.assertInfo.line = (uint32_t)line;
        if (DSS_mboxWrite(&message) != 0)
        {
            System_printf ("Error: Failed to send exception information to MSS.\n");
        }
        
    }    
}        


//********************************************************
//* ********************MAIN******************************
//********************************************************
/**
 * @brief
 * Main Program entrypoint.
 * This is the entrypoint for the DSS firmware
 *
 * @arg    N/A
 * @return 0
 */
 //**********************************************************


int32_t main(void)
{
    Task_Params			taskParams;
    DSS_DataPathObj* 	obj;
    int32_t				errCode;

    /* Initialize the global structure Master Control Block MCB */
    memset((void*) &gMCB, 0, sizeof(MCB));
    /* Initialize entire data path object &gMCB.dataPathObj[0] to a known state, with zeros.
     * In this release one subframe is defined.
     * NUM_SUBFRAMES :=1, therefore we consider one data path.
     */
    obj = &gMCB.dataPathObj[0];
    // Initialize the &gMCB.dataPathObj[0]; with zeros
    memset((void *) obj, 0, sizeof(DSS_DataPathObj));


    /* Populate the chirp configuration in the DSS for all the data path objects.
     * Once again, in this design, only one subframe is used. Therefore, DSS_dataPathConfigPopulate
     * calls the DSS_populateUSRR(obj, 0); for USRR use-cases
     */
    DSS_dataPathConfigPopulate(obj);
    /* Initialize the state counters used for 1D processing */
    DSS_dataPathInit1Dstate(obj);
    /* Initialize the EDMA. */
    DSS_dataPathInitEdma(obj);
    /* Copy code from L3 Shared RAM to L1PSRAM, this code related to data path processing */
    DSS_copyTable(obj->edmaHandle[EDMA_INSTANCE_DSS], &L1PSRAM_copy_table);

    /* Initialize the SOC configuration: */
    {
        SOC_Cfg socCfg;
        /* Initialize the SOC configuration: */
        memset((void *) &socCfg, 0, sizeof(SOC_Cfg));
        /* Populate the SOC configuration: */
        socCfg.clockCfg = SOC_SysClock_BYPASS_INIT;
        /* Initialize the SOC Module: This is done as soon as the application is started
         * to ensure that the MPU is correctly configured. */
        gMCB.socHandle = SOC_init(&socCfg, &errCode);
        if (gMCB.socHandle == NULL)
        {
            return -1;
        }
    }

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 3 * 1024;
    Task_create(DSS_mmWaveInitTASK, &taskParams, NULL);

    /* Start BIOS */
    BIOS_start();

    return 0;
}

void rxGainPhaseParam_Init(DSS_DataPathObj* obj)
{
    int32_t indx = 0;
    if (obj->processingPath == MAX_VEL_ENH_PROCESSING)
    {
           
        obj->rxChPhaseComp[indx].imag = 5863;
        obj->rxChPhaseComp[indx].real = 19158;indx++;

        obj->rxChPhaseComp[indx].imag = 2858;
        obj->rxChPhaseComp[indx].real = 23451;indx++;

        obj->rxChPhaseComp[indx].imag = 4569;
        obj->rxChPhaseComp[indx].real = 22093;indx++;

        obj->rxChPhaseComp[indx].imag = 0;
        obj->rxChPhaseComp[indx].real = 16399;indx++;
    }
    else if (obj->processingPath == POINT_CLOUD_PROCESSING)
    {
        
        obj->rxChPhaseComp[indx].imag = 21646;
        obj->rxChPhaseComp[indx].real = 7035;indx++;

        obj->rxChPhaseComp[indx].imag =  21172;
        obj->rxChPhaseComp[indx].real = 18420;indx++;

        obj->rxChPhaseComp[indx].imag = 15490;
        obj->rxChPhaseComp[indx].real = 21080;indx++;

        obj->rxChPhaseComp[indx].imag = -3905;
        obj->rxChPhaseComp[indx].real = 25130;indx++;


        obj->rxChPhaseComp[indx].imag = 0;
        obj->rxChPhaseComp[indx].real = 16399;indx++;

        obj->rxChPhaseComp[indx].imag = -7985;
        obj->rxChPhaseComp[indx].real = 18443;indx++;

        obj->rxChPhaseComp[indx].imag = -10962;
        obj->rxChPhaseComp[indx].real = 15291;indx++;

   
        obj->rxChPhaseComp[indx].imag =-17653;
        obj->rxChPhaseComp[indx].real = 3133;indx++;

        obj->rxChPhaseComp[indx].imag =  386;
        obj->rxChPhaseComp[indx].real = -17208;indx++;

        obj->rxChPhaseComp[indx].imag = 8587;
        obj->rxChPhaseComp[indx].real = -18744;indx++;
  
        obj->rxChPhaseComp[indx].imag = 11857;
        obj->rxChPhaseComp[indx].real =  -15772;indx++;

        obj->rxChPhaseComp[indx].imag = 18493;
        obj->rxChPhaseComp[indx].real = -2907;indx++;

    }
  
}


/**
 *  @b Description
 *  @n
 *      DSS Initialization Task which initializes the various
 *      components in the DSS subsystem.
 *
 *  @param[in]  arg0
 *      Argument0 with which the task was created
 *  @param[in]  arg1
 *      Argument1 with which the task was created
 *
 *  @retval
 *      Not Applicable.
 */
static void DSS_mmWaveInitTASK(UArg arg0, UArg arg1)
{
    DSS_DataPathObj*    obj;
    Mailbox_Config      mboxCfg;
    Task_Params         taskParams;
    int32_t             errCode,
                        ik;
    /*****************************************************************************
     * Initialize the mmWave SDK components:
     *****************************************************************************/

    /* Initialize the Mailbox */
    Mailbox_init (MAILBOX_TYPE_DSS);

    /* Initialize the ADC Buffer */
    {
    	//ADCBuf_Handle adcBufHandle;
    	ADCBuf_Params adcBufParams;
        ADCBuf_init();
        /* ****************************************************************************
         * Open ADCBUF driver:
         *****************************************************************************/
        ADCBuf_Params_init(&adcBufParams);
        adcBufParams.source=ADCBUF_SOURCE_DFE;
        adcBufParams.chirpThresholdPing = 1;
        adcBufParams.chirpThresholdPong = 1;
        adcBufParams.continousMode = continousMode_OFF;

        gMCB.adcBufHandle = ADCBuf_open(ADCBuf_config_Index, &adcBufParams);
        if (gMCB.adcBufHandle == NULL)
        {
            System_printf("[DSS] \t [ADCBuf][ERROR] Unable to open the ADCBUF driver\n");
            return;
        }

        /* One time ADC Buffer configuration.*/
        DSS_DataPathConfigAdcBuf ();
    }

    {
        /* Register interrupts. */
        SOC_SysIntListenerCfg listenerCfg;

        /* Register Chirp Available Listener */
        memset((void*) &listenerCfg, 0, sizeof(SOC_SysIntListenerCfg));
        listenerCfg.systemInterrupt = SOC_XWR18XX_DSS_INTC_EVENT_CHIRP_AVAIL;
        listenerCfg.listenerFxn = DSS_chirpIntCallback;
        listenerCfg.arg = 0;
        gMCB.chirpIntHandle = SOC_registerSysIntListener(gMCB.socHandle, &listenerCfg, &errCode);
        if (gMCB.chirpIntHandle == NULL)
        {
            return;
        }

        /* Register Frame Start Listener */
        memset((void*) &listenerCfg, 0, sizeof(SOC_SysIntListenerCfg));
        listenerCfg.systemInterrupt = SOC_XWR18XX_DSS_INTC_EVENT_FRAME_START;
        listenerCfg.listenerFxn = DSS_frameStartIntCallback;
        listenerCfg.arg = 0;
        gMCB.frameStartIntHandle = SOC_registerSysIntListener(gMCB.socHandle, &listenerCfg, &errCode);

        if (gMCB.frameStartIntHandle == NULL)
        {
            return;
        }
    }

    /* Setup the default mailbox configuration */
    Mailbox_Config_init(&mboxCfg);

    /* Setup the configuration: */
    mboxCfg.chType       = MAILBOX_CHTYPE_MULTI;
    mboxCfg.chId         = MAILBOX_CH_ID_0;
    mboxCfg.writeMode    = MAILBOX_MODE_BLOCKING;
    mboxCfg.readMode     = MAILBOX_MODE_CALLBACK;
    mboxCfg.readCallback = &mboxCallbackFxn_MSS_ch0;

    gMCB.mboxHandle = Mailbox_open(MAILBOX_TYPE_MSS, &mboxCfg, &errCode);
    if (gMCB.mboxHandle == NULL)
    {
        /* Error: Unable to open the mailbox */
        return;
    }
    
    /* Get DataPath Object handle */
    obj = &gMCB.dataPathObj[0];

    /* Data path configuration for both subframes */
    DSS_dataPathConfigBuffers(obj, SOC_XWR18XX_DSS_ADCBUF_BASE_ADDRESS);
   
    for (ik = 0; ik < NUM_SUBFRAMES; ik ++, obj++)
    {
        /* generate the twiddle factors and windowing inputs for subframe 0 and 1. */
        DSS_dataPathConfigFFTs(obj);
        
        /* Configure the dBscan memories for subframe 0 and 1. */
        DSS_dBScanConfigBuffers(obj);
        /*Initialize the compensation params based on subframe index*/
        rxGainPhaseParam_Init(obj);
        
        /* Configure and initialize the tracking (only for MRR subframe). */
        if (obj->processingPath == MAX_VEL_ENH_PROCESSING)
        {
            ekfInit(obj);
        }
        
        /* Initialize the parking assist module . */
        if (obj->processingPath == POINT_CLOUD_PROCESSING)
        {
            parkingAssistInit(obj);
        }
    }
    
    /* Get DataPath Object handle */
    obj = &gMCB.dataPathObj[0];

    /* EDMA Configuration for both subframes. */
    DSS_dataPathConfigEdma(obj);

    /* The logging buffer is marked as available. */
    gMCB.loggingBufferAvailable = 1;

    /* Start data path task */
    Task_Params_init(&taskParams);
    taskParams.priority = 2;
    taskParams.stackSize = 4 * 1024;
    Task_create(DSS_mmWaveCtrlTask, &taskParams, NULL);
    
    return;
}

/**
 *  @brief
 *  populates the data path object array with the
 *  USRR/SRR/MRR/LRR configuration.
 *  @param[in]  obj: pointer to the data path object.
 *  @return     void.
 */
void DSS_dataPathConfigPopulate(DSS_DataPathObj* obj)
{

#ifdef SUBFRAME_CONF_MRR
        DSS_populateMRR(obj, 0);
#endif 
#ifdef SUBFRAME_CONF_USRR
        DSS_populateUSRR(obj, 0);
#endif
#ifdef SUBFRAME_CONF_LRR
        DSS_populateLRR(obj, 0);
#endif
}

/**
 *  @b Description
 *  @n
 *      Function to send a message to peer through Mailbox virtural channel
 *
 *  @param[in]  message
 *      Pointer to the Capture demo message.
 *
 *  @return
 *      Success    - 0
 *      Fail       < -1
 */
static int32_t DSS_mboxWrite(mmWaveMSG *message)
{
    int32_t retVal = -1;

    retVal = Mailbox_write(gMCB.mboxHandle, (uint8_t*) message,
                           sizeof(mmWaveMSG));
    if (retVal == sizeof(mmWaveMSG))
    {
        retVal = 0;
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The Task is used to handle  the mmw demo messages received from Mailbox virtual channel.
 *
 *  @param[in]  arg0
 *      arg0 of the Task. Not used
 *  @param[in]  arg1
 *      arg1 of the Task. Not used
 *
 *  @retval
 *      Not Applicable.
 */

static void mboxIn_uartOut_TASK()
{
    mmWaveMSG      message;
    int32_t              retVal = 0;

    
    /* Read the message from the peer mailbox: We are not trying to protect the read
     * from the peer mailbox because this is only being invoked from a single thread */
    retVal = Mailbox_read(gMCB.mboxHandle, (uint8_t*)&message, sizeof(mmWaveMSG));
    if (retVal < 0)
    {
        /* Error: Unable to read the message. Setup the error code and return values */
        return;
    }
    else if (retVal == 0)
    {
          /* We are done: There are no messages available from the peer execution domain. */
        return;
    }
    else
    {
        /* Flush out the contents of the mailbox to indicate that we are done with the message. This will
         * allow us to receive another message in the mailbox while we process the received message. */
        Mailbox_readFlush (gMCB.mboxHandle);

        /* Process the received message: */
        switch (message.type)
        {
            case MBOX_MSS2DSS_DETOBJ_SHIPPED:
            {
                uint8_t prevSubframeIndx;
                DSS_DataPathObj *   dataPathObj;

                if (gMCB.subframeIndx == 0)
                {
                    prevSubframeIndx = NUM_SUBFRAMES - 1;
                } 
                else
                {
                    prevSubframeIndx = gMCB.subframeIndx - 1;
                }

                dataPathObj = &gMCB.dataPathObj[prevSubframeIndx];
                dataPathObj->timingInfo.transmitOutputCycles = Cycleprofiler_getTimeStamp() - dataPathObj->timingInfo.interFrameProcessingEndTime;

                gMCB.loggingBufferAvailable = 1;

                break;
            }
            default:
            {
                /* Message not support */
                // System_printf ("Error: unsupport Mailbox message id=%d\n", message.type);
                dssAssert(0);
                break;
            }
        }
    }
}

/**
 *  @b Description
 *  @n
 *      This function is a callback function that invoked when a message is received from the peer.
 *
 *  @param[in]  handle
 *      Handle to the Mailbox on which data was received
 *  @param[in]  peer
 *      Peer from which data was received

 *  @retval
 *      Not Applicable.
 */
void mboxCallbackFxn_MSS_ch0
(
    Mbox_Handle  handle,
    Mailbox_Type    peer
)
{
    /* Message has been received from the peer endpoint. */
    gMCB.mboxProcToken = 1;
}

/**
 *  @b Description
 *  @n
 *      Function to send detected objects to MSS logger.
 *
 *  @param[in]  ptrOutputBuffer
 *      Pointer to the output buffer
 *  @param[in]  outputBufSize
 *      Size of the output buffer
 *  @param[in]  obj
 *      Handle to the Data Path Object
 *
 *  @retval
 *      =0    Success
 *      <0    Failed
 */
int32_t DSS_SendProcessOutputToMSS(uint8_t *ptrHsmBuffer,
                                          uint32_t outputBufSize,
                                          DSS_DataPathObj *obj)
{
    uint8_t *ptrCurrBuffer;
    uint32_t totalHsmSize = 0;
    uint32_t totalPacketLen = sizeof(mmWave_OUT_MSG_header);
    uint32_t itemPayloadLen;
    int32_t retVal = 0;
    mmWaveMSG message;
    mmWave_OUT_MSG_stats_dataObjDescr descr;
    uint32_t tlvIdx = 0;

    /* Set pointer to HSM buffer */
    ptrCurrBuffer = ptrHsmBuffer;

    /* Clear message to MSS */
    memset((void *) &message, 0, sizeof(mmWaveMSG));
    message.type = MBOX_DSS2MSS_DETOBJ_READY;

    /* Header: */
    message.body.detObj.header.platform = 0xA1642 ;
    message.body.detObj.header.magicWord[0] = 0x0102;
    message.body.detObj.header.magicWord[1] = 0x0304;
    message.body.detObj.header.magicWord[2] = 0x0506;
    message.body.detObj.header.magicWord[3] = 0x0708;
    message.body.detObj.header.numDetectedObj = obj->numDetObj;
    message.body.detObj.header.version = MMWAVE_SDK_VERSION_BUILD | (MMWAVE_SDK_VERSION_BUGFIX << 8) | (MMWAVE_SDK_VERSION_MINOR << 16)  | (MMWAVE_SDK_VERSION_MAJOR << 24);

    /* Put detected Objects in HSM buffer: sizeof(MmwDemo_objOut_t) * numDetObj  */
    if  (obj->numDetObj > 0)
    {
        /* Add objects descriptor */
        descr.numDetetedObj = obj->numDetObj;
        descr.xyzQFormat = obj->xyzOutputQFormat;

        itemPayloadLen = sizeof(mmWave_OUT_MSG_stats_dataObjDescr);
        totalHsmSize += itemPayloadLen;
        if (totalHsmSize > outputBufSize)
        {
            retVal = -1;
            goto Exit;
        }
        memcpy(ptrCurrBuffer, (void *) &descr, itemPayloadLen);

        /* Add array of objects */
        itemPayloadLen = sizeof(MmwDemo_detectedObjForTx) * obj->numDetObj;
        totalHsmSize += itemPayloadLen;
        if (totalHsmSize > outputBufSize)
        {
            retVal = -1;
            goto Exit;
        }
        memcpy(&ptrCurrBuffer[sizeof(mmWave_OUT_MSG_stats_dataObjDescr)],
               (void *) obj->detObjFinal, itemPayloadLen);

        message.body.detObj.tlv[tlvIdx].length = itemPayloadLen
                + sizeof(mmWave_OUT_MSG_stats_dataObjDescr);
        message.body.detObj.tlv[tlvIdx].type =
                OUTPUT_MSG_DETECTED_POINTS;
        message.body.detObj.tlv[tlvIdx].address = (uint32_t) ptrCurrBuffer;
        tlvIdx++;

        /* Incrementing pointer to HSM buffer */
        ptrCurrBuffer += itemPayloadLen
                + sizeof(mmWave_OUT_MSG_stats_dataObjDescr);
        totalPacketLen += sizeof(mmWave_OUT_MSG_tl) + itemPayloadLen
                + sizeof(mmWave_OUT_MSG_stats_dataObjDescr);
    }

    if ((obj->processingPath == POINT_CLOUD_PROCESSING) && (obj->dbScanReport.numCluster > 0))  
    {
        /* Add objects descriptor */
        /* In the point cloud processing path, the dbScanReport holds the number of clusters. */
        descr.numDetetedObj = obj->dbScanReport.numCluster;
            
        descr.xyzQFormat = obj->xyzOutputQFormat;

        itemPayloadLen = sizeof(mmWave_OUT_MSG_stats_dataObjDescr);
        totalHsmSize += itemPayloadLen;
        if (totalHsmSize > outputBufSize)
        {
            retVal = -1;
            goto Exit;
        }
        memcpy(ptrCurrBuffer, (void *) &descr, itemPayloadLen);

        /* Add array of cluster reports. */
        itemPayloadLen = sizeof(clusteringDBscanReportForTx) * obj->dbScanReport.numCluster;
        totalHsmSize += itemPayloadLen;
        if (totalHsmSize > outputBufSize)
        {
            retVal = -1;
            goto Exit;
        }
        memcpy(&ptrCurrBuffer[sizeof(mmWave_OUT_MSG_stats_dataObjDescr)],
               (void *) obj->clusterOpFinal, itemPayloadLen);

        message.body.detObj.tlv[tlvIdx].length = itemPayloadLen
                + sizeof(mmWave_OUT_MSG_stats_dataObjDescr);
        message.body.detObj.tlv[tlvIdx].type =
                MMWDEMO_OUTPUT_MSG_CLUSTERS;
        message.body.detObj.tlv[tlvIdx].address = (uint32_t) ptrCurrBuffer;
        tlvIdx++;

        /* Incrementing pointer to HSM buffer */
        ptrCurrBuffer += itemPayloadLen
                + sizeof(mmWave_OUT_MSG_stats_dataObjDescr);
        totalPacketLen += sizeof(mmWave_OUT_MSG_tl) + itemPayloadLen
                + sizeof(mmWave_OUT_MSG_stats_dataObjDescr);
    }
    
    if (obj->processingPath == POINT_CLOUD_PROCESSING) 
    {
        /* Add objects descriptor */
        descr.numDetetedObj = obj->parkingAssistNumBins;
        descr.xyzQFormat = obj->xyzOutputQFormat;

        itemPayloadLen = sizeof(mmWave_OUT_MSG_stats_dataObjDescr);
        totalHsmSize += itemPayloadLen;
        if (totalHsmSize > outputBufSize)
        {
            retVal = -1;
            goto Exit;
        }
        memcpy(ptrCurrBuffer, (void *) &descr, itemPayloadLen);

        /* Add array of c;uster reports. */
        itemPayloadLen = sizeof(uint16_t) * obj->parkingAssistNumBins;
        totalHsmSize += itemPayloadLen;
        if (totalHsmSize > outputBufSize)
        {
            retVal = -1;
            goto Exit;
        }
        memcpy(&ptrCurrBuffer[sizeof(mmWave_OUT_MSG_stats_dataObjDescr)],
               (void *) obj->parkingAssistBins, itemPayloadLen);

        message.body.detObj.tlv[tlvIdx].length = itemPayloadLen
                + sizeof(mmWave_OUT_MSG_stats_dataObjDescr);
        message.body.detObj.tlv[tlvIdx].type =
                MMWDEMO_OUTPUT_MSG_PARKING_ASSIST;
        message.body.detObj.tlv[tlvIdx].address = (uint32_t) ptrCurrBuffer;
        tlvIdx++;

        /* Incrementing pointer to HSM buffer */
        ptrCurrBuffer += itemPayloadLen
                + sizeof(mmWave_OUT_MSG_stats_dataObjDescr);
        totalPacketLen += sizeof(mmWave_OUT_MSG_tl) + itemPayloadLen
                + sizeof(mmWave_OUT_MSG_stats_dataObjDescr);
    }

    if ((obj->processingPath == MAX_VEL_ENH_PROCESSING) && 
        (obj->numActiveTrackers > 0))
    {
        /* Add objects descriptor */
        descr.numDetetedObj = obj->numActiveTrackers;
        descr.xyzQFormat = obj->xyzOutputQFormat;

        itemPayloadLen = sizeof(mmWave_OUT_MSG_stats_dataObjDescr);
        totalHsmSize += itemPayloadLen;
        if (totalHsmSize > outputBufSize)
        {
            retVal = -1;
            goto Exit;
        }
        memcpy(ptrCurrBuffer, (void *) &descr, itemPayloadLen);

        /* Add array of tracked objects. */
        itemPayloadLen = sizeof(trackingReportForTx) * obj->numActiveTrackers;
        totalHsmSize += itemPayloadLen;
        if (totalHsmSize > outputBufSize)
        {
            retVal = -1;
            goto Exit;
        }
        memcpy(&ptrCurrBuffer[sizeof(mmWave_OUT_MSG_stats_dataObjDescr)],
               (void *) obj->trackerOpFinal, itemPayloadLen);

        message.body.detObj.tlv[tlvIdx].length = itemPayloadLen
                + sizeof(mmWave_OUT_MSG_stats_dataObjDescr);
        message.body.detObj.tlv[tlvIdx].type =
                MMWDEMO_OUTPUT_MSG_TRACKED_OBJECTS;
        message.body.detObj.tlv[tlvIdx].address = (uint32_t) ptrCurrBuffer;
        tlvIdx++;

        /* Incrementing pointer to HSM buffer */
        ptrCurrBuffer += itemPayloadLen + sizeof(mmWave_OUT_MSG_stats_dataObjDescr);
        totalPacketLen += sizeof(mmWave_OUT_MSG_tl) + itemPayloadLen + sizeof(mmWave_OUT_MSG_stats_dataObjDescr);
    }

    if (tlvIdx >= OUTPUT_MSG_MAX)
    {
        retVal = -1;
    }

    if (retVal == 0)
    {
        
        message.body.detObj.header.numTLVs = tlvIdx;
        /* Round up packet length to multiple of MMWDEMO_OUTPUT_MSG_SEGMENT_LEN. */
        message.body.detObj.header.totalPacketLen =
                MMW_OUTPUT_MSG_SEGMENT_LEN
                        * ((totalPacketLen
                                + (MMW_OUTPUT_MSG_SEGMENT_LEN - 1))
                                / MMW_OUTPUT_MSG_SEGMENT_LEN);
        message.body.detObj.header.timeCpuCycles = Cycleprofiler_getTimeStamp();
        message.body.detObj.header.frameNumber   = gMCB.stats.frameStartIntCounter;

        /* The GUI reads the subframe number to decide on the plotting type.
         *  a 0 => MAX_VEL_ENH_PROCESSING. 
         *  a 1 => POINT_CLOUD_PROCESSING. */
        message.body.detObj.header.subFrameNumber  = gMCB.dataPathObj[gMCB.subframeIndx].processingPath;
        if (DSS_mboxWrite(&message) != 0)
        {
            retVal = -1;
        }
    }
    Exit: return retVal;
}

/**
 *  @b Description
 *  @n
 *      Function to send data path detection output.
 *
 *  @retval
 *      Not Applicable.
 */
void DSS_DataPathOutputLogging(DSS_DataPathObj* dataPathObj)
{
    volatile int32_t waitCounter = 0;
    /* if the logging buffer is not available, wait a little for the transfer message to come from 
     * the MSS. */
    if(gMCB.loggingBufferAvailable == 0) 
    {
        while((waitCounter < 6000000) && (gMCB.loggingBufferAvailable == 0))
        {
            waitCounter++;
            
            if (gMCB.mboxProcToken == 1) 
            {
                gMCB.mboxProcToken = 0;
                /* If the mailbox has a message and the frame processing task has finished. */
                mboxIn_uartOut_TASK();
            }
        }
    }
    
    /* Sending detected objects to logging buffer and shipped out from MSS UART */
    if (gMCB.loggingBufferAvailable == 1)
    {
        /* Set the logging buffer available flag to be 0 */
        gMCB.loggingBufferAvailable = 0;

        /* Save output in logging buffer - HSRAM memory and a message is sent to MSS to notify
         logging buffer is ready */
        if (DSS_SendProcessOutputToMSS((uint8_t *) &gHSRAM, (uint32_t) SOC_XWR18XX_DSS_HSRAM_SIZE,
                                            dataPathObj) < 0)
        {
            /* Increment logging error */
            dssAssert(0);
            gMCB.stats.detObjLoggingErr++;
        }
        
    }
    else
    {
        /* Logging buffer is not available, skip saving detected objectes to logging buffer */
        gMCB.stats.detObjLoggingSkip++;
    }
}


/*@**********************************************************************
 * DSS_DataPathConfigAdcBuf
 ************************************************************************/
/**
 * \brief
 *   Configure ADCBuf driver based on CLI inputs
 *
 *  @param[out] numRxChannels Number of receive channels.
 *  \return  	void
 */
//***********************************************************************/
static int32_t DSS_DataPathConfigAdcBuf()
{
    ADCBuf_dataFormat	dataFormat;
    ADCBuf_RxChanConf	rxChanConf;
    int32_t 			retVal;
    uint8_t				channel;
    uint32_t 			chirpThreshold;
    uint32_t 			offset;
    uint32_t 			rxChanMask = 0xF; /* All channels are enabled. */

    chirpThreshold = 1;


    /* Divide the 32 kB ADC buffer into 4 parts. Assign each to one channel */
    offset = ((32 * 1024)/4);

    /* ****************************************************************************
     * Data path :: ADCBUF driver Configuration
     *****************************************************************************/
    /* Populate data format from configuration */
    dataFormat.adcOutFormat = 		ADC_OUT_FORMAT_COMPLX;
    dataFormat.channelInterleave =	CH_INTERLEAVED_MODE_DISABLED;
    dataFormat.sampleInterleave =	SAMPLE_INTERLEAVED_MODE_I_LSB;
;

    /* Disable all ADCBuf channels */
    if ((retVal = ADCBuf_control(gMCB.adcBufHandle,
                                 ADCBufMMWave_CMD_CHANNEL_DISABLE,
                                 (void *) &rxChanMask)) < 0)
    {
        // ("Error: Disable ADCBuf channels failed with [Error=%d]\n", retVal);
        return retVal;
    }

    retVal = ADCBuf_control(gMCB.adcBufHandle,
                            ADCBufMMWave_CMD_CONF_DATA_FORMAT,
                            (void *) &dataFormat);
    if (retVal < 0)
    {
        // ("Error: MMWDemoDSS Unable to configure the data formats\n");
        return -1;
    }

    memset((void*) &rxChanConf, 0, sizeof(ADCBuf_RxChanConf));

    /* Enable all Rx Channels */
    for (channel = 0; channel < SYS_COMMON_NUM_RX_CHANNEL; channel++)
    {
        /* Populate the receive channel configuration: */
        rxChanConf.channel = channel;
        retVal = ADCBuf_control(gMCB.adcBufHandle,
                                ADCBufMMWave_CMD_CHANNEL_ENABLE,
                                (void *) &rxChanConf);

        if (retVal < 0)
        {
            // ("Error: MMWDemoDSS ADCBuf Control for Channel %d Failed with error[%d]\n", channel, retVal);
            return -1;
        }

        rxChanConf.offset += offset;
    }

    /* Set ping/pong chirp threshold: */
    retVal = ADCBuf_control(gMCB.adcBufHandle,
                            ADCBufMMWave_CMD_SET_PING_CHIRP_THRESHHOLD,
                            (void *) &chirpThreshold);
    if (retVal < 0)
    {
        // System_printf("Error: ADCbuf Ping Chirp Threshold Failed with Error[%d]\n", retVal);
        return -1;
    }
    retVal = ADCBuf_control(gMCB.adcBufHandle,
                            ADCBufMMWave_CMD_SET_PONG_CHIRP_THRESHHOLD,
                            (void *) &chirpThreshold);
    if (retVal < 0)
    {
        // System_printf("Error: ADCbuf Pong Chirp Threshold Failed with Error[%d]\n", retVal);
        return -1;
    }

    return 0;
}

/**
 *  @brief
 *   Performs linker generated copy table copy using EDMA.
 *   Currently this is used to page in fast code from L3 to L1PSRAM.
 *  @param[in]  handle: EDMA handle
 *  @param[in]  tp:     Pointer to copy table
 *
 *  @retval     void
 */
static void DSS_copyTable(EDMA_Handle handle, COPY_TABLE *tp)
{
    uint16_t        i;
    COPY_RECORD     crp;
    uint32_t        loadAddr;
    uint32_t        runAddr;

    for (i = 0; i < tp->num_recs; i++)
    {
        crp = tp->recs[i];
        loadAddr = (uint32_t)crp.load_addr;
        runAddr = (uint32_t)crp.run_addr;

        /* currently we use only one count of EDMA which is 16-bit so we cannot
           handle tables bigger than 64 KB */
        dssAssert(crp.size <= 65536U);

        if (crp.size)
        {
            DSS_edmaBlockCopy(handle, loadAddr, runAddr, crp.size);
        }
    }
}
/**
 *  @brief
 *   Performs simple block copy using EDMA. Used for the purpose of copying
 *   linker table for L3 to L1PSRAM copy. memcpy cannot be used because there is
 *   no data bus access to L1PSRAM.
 *
 *  @param[in]  handle EDMA handle
 *  @param[in]  loadAddr load address
 *  @param[in]  runAddr run address
 *  @param[in]  size size in bytes
 *
 *  @retval     void.
 */
static void DSS_edmaBlockCopy(EDMA_Handle handle, uint32_t loadAddr, uint32_t runAddr, uint16_t size)
{
    EDMA_channelConfig_t config;
    volatile bool isTransferDone;

    config.channelId = EDMA_TPCC0_REQ_FREE_0;
    config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA;
    config.paramId = (uint16_t)EDMA_TPCC0_REQ_FREE_0;
    config.eventQueueId = 0;

    config.paramSetConfig.sourceAddress = (uint32_t) SOC_translateAddress((uint32_t)loadAddr,
        SOC_TranslateAddr_Dir_TO_EDMA, NULL);
    config.paramSetConfig.destinationAddress = (uint32_t) SOC_translateAddress((uint32_t)runAddr,
        SOC_TranslateAddr_Dir_TO_EDMA, NULL);

    config.paramSetConfig.aCount = size;
    config.paramSetConfig.bCount = 1U;
    config.paramSetConfig.cCount = 1U;
    config.paramSetConfig.bCountReload = 0U;

    config.paramSetConfig.sourceBindex = 0U;
    config.paramSetConfig.destinationBindex = 0U;

    config.paramSetConfig.sourceCindex = 0U;
    config.paramSetConfig.destinationCindex = 0U;

    config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS;
    config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A;
    config.paramSetConfig.transferCompletionCode = (uint8_t) EDMA_TPCC0_REQ_FREE_0;
    config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR;
    config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR;

    /* don't care because of linear addressing modes above */
    config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT;

    config.paramSetConfig.isStaticSet = false;
    config.paramSetConfig.isEarlyCompletion = false;
    config.paramSetConfig.isFinalTransferInterruptEnabled = true;
    config.paramSetConfig.isIntermediateTransferInterruptEnabled = false;
    config.paramSetConfig.isFinalChainingEnabled = false;
    config.paramSetConfig.isIntermediateChainingEnabled = false;
    config.transferCompletionCallbackFxn = NULL;
    config.transferCompletionCallbackFxnArg = NULL;

    if (EDMA_configChannel(handle, &config, false) != EDMA_NO_ERROR)
    {
        dssAssert(0);
    }

    if (EDMA_startDmaTransfer(handle, config.channelId) != EDMA_NO_ERROR)
    {
        dssAssert(0);
    }

    /* wait until transfer done */
    do 
    {
        if (EDMA_isTransferComplete(handle,
                config.paramSetConfig.transferCompletionCode,
                (bool *)&isTransferDone) != EDMA_NO_ERROR)
        {
            dssAssert(0);
        }
    } 
    while (isTransferDone == false);

    /* make sure to disable channel so it is usable later */
    EDMA_disableChannel(handle, config.channelId, config.channelType);
}

/**
 *  @b Description
 *  @n
 *      Converts an SNR (in dB) to an SNR Threshold that the CFAR algo
 *      can use. 
 *  @param[in]  number of integrations in the detection matrix 
 *              Typically the number of virtual antennas
 *  @param[in]  Threshold in dB (float)
 *
 *  @retval
 *      Threshold for CFAR algorithm.
 */
uint16_t convertSNRdBtoThreshold(uint16_t numInteg, float ThresholdIndB,uint16_t bitwidth)
{
    float scaleFac = (float) ((1 << bitwidth) * numInteg);
    float convertFrom_10Log10_to_20Log2 = ThresholdIndB * (1.0f / 6.0f);
    
    return (uint16_t) (scaleFac * convertFrom_10Log10_to_20Log2);
}

/**
 *  @b Description
 *  @n
 *      Configures the dBScan buffers.  
 *  @param[in]  data path Object
 *
 *  @retval
 *      Not applicable.
 */
void DSS_dBScanConfigBuffers(DSS_DataPathObj * obj)
{

    obj->dbScanInstance.scratchPad    =   (char *)obj->dBscanScratchPad;
    obj->dbScanInstance.visited       =   (char *)& obj->dBscanScratchPad[0];
    obj->dbScanInstance.scope         =   (char *)& obj->dBscanScratchPad[obj->dbScanInstance.maxPoints];
    obj->dbScanInstance.neighbors     =   (uint16_t *)& obj->dBscanScratchPad[2 * obj->dbScanInstance.maxPoints];
    
    obj->dbScanReport.IndexArray        =    obj->dbscanOutputDataIndexArray;
    obj->dbScanReport.report    =    obj->dbscanOutputDataReport;
    obj->dbScanReport.numCluster    =    0;
    
    if (obj->processingPath == POINT_CLOUD_PROCESSING)
    {
        uint32_t i;
        for (i = 0; i < obj->dbScanInstance.maxClusters; i++)
        {
            obj->dbScanState[i].numPoints = 0;    
        }
    }
}


/**
 *  @brief
 *  Populates the configuration for the USRR point cloud subframe.
 *  @param[in]  obj: data path Object
 *  @param[in]  subframeIndx: Number of Subframe
 *  @return     void.
 */
void DSS_populateUSRR(DSS_DataPathObj* obj, uint16_t subframeIndx)
{
    obj->processingPath = POINT_CLOUD_PROCESSING;
    obj->subframeIndx = subframeIndx;
    obj->numRxAntennas = NUM_RX_CHANNELS;
    obj->chirpThreshold = ADCBUFF_CHIRP_THRESHOLD;
    obj->numTxAntennas = SUBFRAME_USRR_NUM_TX;
    obj->numVirtualAntennas = SUBFRAME_USRR_NUM_VIRT_ANT;
    obj->numVirtualAntAzim = 8;
    obj->numVirtualAntElev = 4; 
    obj->numAdcSamples = SUBFRAME_USRR_NUM_CMPLX_ADC_SAMPLES;
    obj->numRangeBins = SUBFRAME_USRR_NUM_CMPLX_ADC_SAMPLES;
    obj->numChirpsPerChirpType = SUBFRAME_USRR_CHIRPTYPE_0_NUM_CHIRPS;
    obj->numAngleBins = SUBFRAME_USRR_NUM_ANGLE_BINS;
    obj->invNumAngleBins = 1.0f/((float)SUBFRAME_USRR_NUM_ANGLE_BINS);
    obj->numDopplerBins = SUBFRAME_USRR_CHIRPTYPE_0_NUM_CHIRPS;
    obj->maxNumObj2DRaw = MAX_DET_OBJECTS_RAW_POINT_CLOUD_PROCESSING;
    
#if (SUBFRAME_USRR_CHIRPTYPE_0_NUM_CHIRPS != SUBFRAME_USRR_CHIRPTYPE_1_NUM_CHIRPS)
#error "the number of chirps in each chirp type is not equal"
#endif
    obj->log2NumDopplerBins = LOG2_APPROX(SUBFRAME_USRR_CHIRPTYPE_0_NUM_CHIRPS);
    obj->rangeResolution = PROFILE_USRR_RANGE_RESOLUTION_METERS;
    
    /* For the point-cloud chirp, both chirptypes offer the same velocity resolution. */
    obj->velResolution = SUBFRAME_USRR_VEL_RESOLUTION_M_P_S;

    obj->maxUnambiguousVel = SUBFRAME_USRR_MAX_VEL_M_P_S;
    
    /*! @brief Q format of the output x/y/z coordinates */
    obj->xyzOutputQFormat = 7;
    obj->invOneQFormat = 1.0f/((float)(1U << obj->xyzOutputQFormat));
    obj->sinAzimQFormat = 14;
    obj->invOneSinAzimFormat = 1.0f/((float)(1U << obj->sinAzimQFormat));
    
    /*! @brief CFAR configuration in Doppler direction */
    obj->cfarCfgDoppler.averageMode = MMW_NOISE_AVG_MODE_CFAR_CA;
    obj->cfarCfgDoppler.winLen = 8;
    obj->cfarCfgDoppler.guardLen = 4;
    obj->cfarCfgDoppler.noiseDivShift = 4; /* Should be log2(2*winLen) */ 
    obj->cfarCfgDoppler.cyclicMode = 0;
    obj->cfarCfgDoppler.thresholdScale =  convertSNRdBtoThreshold(obj->numRxAntennas*obj->numTxAntennas, 15.0f, CFARTHRESHOLD_N_BIT_FRAC);

    /*! @brief CFAR configuration in Range direction */
    obj->cfarCfgRange.averageMode = MMW_NOISE_AVG_MODE_CFAR_CASO;
    obj->cfarCfgRange.winLen = 8;
    obj->cfarCfgRange.guardLen = 4;
    obj->cfarCfgRange.noiseDivShift = 4; /* Should be log2(2*winLen) */
    obj->cfarCfgRange.cyclicMode = 0;
    obj->cfarCfgRange.thresholdScale =  convertSNRdBtoThreshold(obj->numRxAntennas*obj->numTxAntennas, 12.0f, CFARTHRESHOLD_N_BIT_FRAC);
    obj->cfarCfgRange_minIndxToIgnoreHPF = 54; /* The left side of the CFAR sum is ignored upto  ~800 kHz (54+12)*6222/512* */
    /*! @brief min and max range configuration */
    obj->minRange = (uint16_t) (0.10f * (1U << obj->xyzOutputQFormat));
    obj->maxRange = (uint16_t) ROUND_TO_INT32(obj->rangeResolution*SUBFRAME_USRR_NUM_CMPLX_ADC_SAMPLES * (0.9f) * (1U << obj->xyzOutputQFormat));

    /*! @brief Multi object beam forming configuration */
    obj->multiObjBeamFormingCfg.enabled = 1;
    obj->multiObjBeamFormingCfg.multiPeakThrsScal = 0.85f;

    /*! @brief DC Range antenna signature calibration configuration. */
    obj->calibDcRangeSigCfg.enabled = 0;
    obj->txAntennaCount = SUBFRAME_USRR_NUM_TX;

    obj->log2numVirtAnt = LOG2_APPROX(SUBFRAME_USRR_NUM_VIRT_ANT);
#ifdef LOW_THRESHOLD_FOR_USRR
    /*! @brief CFAR thresholds are varied as a function of range */
    obj->SNRThresholds[0].rangelim = (uint16_t) (6.0f * (float)(1U << obj->xyzOutputQFormat));
    obj->SNRThresholds[0].threshold = convertSNRdBtoThreshold(1U, 13.0f, CFARTHRESHOLD_N_BIT_FRAC);

    obj->SNRThresholds[1].rangelim = (uint16_t) (10.0f * (float)(1U << obj->xyzOutputQFormat));
    obj->SNRThresholds[1].threshold = convertSNRdBtoThreshold(1U, 12.0f, CFARTHRESHOLD_N_BIT_FRAC);

    obj->SNRThresholds[2].rangelim = 65535;
    obj->SNRThresholds[2].threshold = convertSNRdBtoThreshold(1U, 12.0f, CFARTHRESHOLD_N_BIT_FRAC);
    
    /*! @brief peakVal thresholds are varied as a function of range (meant to remove cases of 
     * clutter being detected too when we drive the car.) Thresholds were derived from experiments.*/
    obj->peakValThresholds[0].rangelim = (uint16_t) (3.0f * (float) (1U << obj->xyzOutputQFormat));
    obj->peakValThresholds[0].threshold = (27000 >> obj->log2numVirtAnt);
    
    obj->peakValThresholds[1].rangelim = 65535;
    obj->peakValThresholds[1].threshold = 0;

    obj->peakValThresholds[2].rangelim = 65535;
    obj->peakValThresholds[2].threshold = 0;
#else
    /*! @brief CFAR thresholds are varied as a function of range */
    obj->SNRThresholds[0].rangelim = (uint16_t) (6.0f * (float)(1U << obj->xyzOutputQFormat));
    obj->SNRThresholds[0].threshold = convertSNRdBtoThreshold(1, 16.0f, CFARTHRESHOLD_N_BIT_FRAC);

    obj->SNRThresholds[1].rangelim = (uint16_t) (10.0f * (float)(1U << obj->xyzOutputQFormat));
    obj->SNRThresholds[1].threshold = convertSNRdBtoThreshold(1U, 15.0f, CFARTHRESHOLD_N_BIT_FRAC);

    obj->SNRThresholds[2].rangelim = 65535;
    obj->SNRThresholds[2].threshold = convertSNRdBtoThreshold(1U, 14.0f, CFARTHRESHOLD_N_BIT_FRAC);
    
    /*! @brief peakVal thresholds are varied as a function of range (meant to remove cases of 
     * clutter being detected too when we drive the car.) Thresholds were derived from experiments.*/
    obj->peakValThresholds[0].rangelim = (uint16_t) (3.0f * (float) (1U << obj->xyzOutputQFormat));
    obj->peakValThresholds[0].threshold = (350 >> obj->log2numVirtAnt);
    
    obj->peakValThresholds[1].rangelim = 65535;
    obj->peakValThresholds[1].threshold = 0;

    obj->peakValThresholds[2].rangelim = 65535;
    obj->peakValThresholds[2].threshold = 0;
#endif
    
    /*! Configuring the dbSCan for car like objects. These numbers are derived from field tests
     *  Copywrite TI  */
    obj->dbScanInstance.epsilon       =   1.7f;
    obj->dbScanInstance.vFactor       =   3.0f;
    obj->dbScanInstance.weight        =   1.3f * 1.3f * obj->dbScanInstance.epsilon;
    obj->dbScanInstance.maxClusters   =   MAX_NUM_CLUSTER_USRR;
    obj->dbScanInstance.minPointsInCluster    =   3;
    obj->dbScanInstance.maxPoints     =   MRR_MAX_OBJ_OUT;
    obj->dbScanInstance.fixedPointScale      =   (1 << obj->xyzOutputQFormat);    
    obj->dbScanInstance.dBScanNeighbourLim = 7; 
    
    /*! Configuring the 'parking assist' occupancy detect. */
    obj->parkingAssistNumBins = 32;
    obj->parkingAssistMaxRange  = 20 * (1 << obj->xyzOutputQFormat);
    obj->parkingAssistNumBinsLog2 = LOG2_APPROX(32);
    
    /* Azimuth SNR computation depend on the following computation. */
    dssAssert ((obj->numAngleBins%obj->numVirtualAntAzim) == 0);
}
