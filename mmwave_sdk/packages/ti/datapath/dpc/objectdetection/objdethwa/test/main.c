/**
 *   @file  main.c
 *
 *   @brief
 *      Unit Test code for the object detection DPC.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2018 Texas Instruments, Inc.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

 /** @mainpage DPC Object Detection Unit Test
 *
 *  @section intro_sec Introduction
 *
 *  The data path processing on the radar chip for 1D, 2D and 3D processing consists of
 *  a coordinated execution between
 *  the R4, HWA and EDMA. This is demonstrated as part of object detection demo
 *  located at:
 *  @verbatim ti\datapath\dpc\objectdetection\objdethwa @endverbatim
 *  The demo runs in real-time
 *  and has all the associated framework for BSS control etc with it. There is need to
 *  have a stand-alone unit test like code that allows the data path to be excited
 *  in non real-time. This allows developer to use it as a debug/development aid
 *  towards eventually  making the data path processing real-time with real chirping.
 *  Developer can easily step into the code and test against known input signals.
 *  Most of the documentation is therefore also shared and can be looked up in the
 *  demo documentation. Here we will only describe the test flow at the high level and
 *  document the test generator briefly. The test flow diagram is described in figure below.
 *  The DPC is configured in advance frame mode running 4 sub-frames at a time.
 *  Each sub-frame represents one unit test with different sensor and scene configuration parameters.
 *  In the loop test reads 4 unit test configurations from the reference file, executes them as
 *  one unit test per sub-frame, checks the results and repeats until the end of the
 *  reference file. The reference file is located in
 *  @verbatim ti\datapath\dpc\objectdetection\objdethwa\test\testdata @endverbatim
 *
 *  @image html object_detection_unit_test.png "Object detection test flow diagram"
 *
 *  The unit test configuration parameters are defined by structure @ref frameConfig_t_ which consists of the sensor configuration @ref sensorConfig_t_, and the scene configuration @ref sceneConfig_t_ .
 *
 *  @section testGen Test Generator
 *   The test generator allows user to set arbitrary number of objects artificially at desired
 *   range, doppler and azimuth bins, elevation angle and noise level so that output can be checked
 *   against these settings. It can generate one frame of data.
 *
 *   The test data generator is located in file:
 *   <code> ti\datapath\dpc\objectdetection\common\gen_frame_data.c </code>
 *   The function initFrameData() initializes the structure @ref genFrameStruc_t_ with unit test parameters defined in @ref sensorConfig_t_ , and @ref sceneConfig_t_ .
 *
 *   The function genChirpData() is called once per chirp interval and it generates
 *   reference signal composed of all targets. The reference signal
 *   of one target is generated as:
 *
 *    \f[
 *    x_{k,m,l}(n)=A \exp (j2\pi\frac{f_R}{N_{RFFT}}n + j2\pi\frac{f_A}{N_{AFFT}}(m+m_{azim}(l)) + j\varphi_{elev}(l)+j2\pi\frac{f_D}{N_{Tx}N_{DFFT}}k) + noise
 *    \f]
 *
 *    \f[
 *    m_{azim}(l) = \{0,4,2\}
 *    \f]
 *
 *    \f[
 *    \varphi_{elev}(l) = \left\{
 *      \begin{array}{lr}
 *        0 & : l=0,1\\
 *        \pi\sin(\phi_{deg} / 180 \cdot \pi) & : l=2
 *      \end{array}
 *    \right.
 *    \f]
 *
 *   where
 *   - \f$k\f$ is chirp index in the frame \f$k=0,...,N_{Tx}N_{DFFT}-1\f$,
 *   - \f$m\f$ is receive antenna index \f$m=0,...,N_{Rx}-1\f$,
 *   - \f$l\f$ is transmit antenna index \f$l=0,...,N_{Tx}-1\f$,
 *   - \f$n\f$ is sample index inside the chirp \f$n=0,...,N_{ADC}-1\f$,
 *   - \f$f_{R}\f$ is expected peak index in the range FFT
 *   - \f$f_{D}\f$ is expected peak index in the Doppler FFT
 *   - \f$f_{A}\f$ is expected peak index in the azimuth FFT
 *   - \f$\phi_{deg}\f$ is expected elevation angle in degrees
 *   - \f$N_{ADC}\f$ is number of ADC chirp samples,
 *   - \f$N_{RFFT}\f$ is number of range bins, (range FFT size)
 *   - \f$N_{AFFT}\f$ is number of angle bins (azimuth FFT size)
 *   - \f$N_{Tx}\f$ is number of Tx antennas
 *   - \f$N_{Rx}\f$ is number of Rx antennas
 *   - \f$N_{DFFT}\f$ is number of Doppler bins (Doppler FFT size)
 *
 *
 */
 
/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* BIOS/XDC Include Files. */
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
#include <ti/sysbios/knl/Event.h>
#define DebugP_ASSERT_ENABLED 1
#include <ti/drivers/osal/DebugP.h>
#include <assert.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/osal/SemaphoreP.h>
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/esm/esm.h>
#include <ti/drivers/soc/soc.h>

#include <ti/datapath/dpc/objectdetection/objdethwa/objectdetection.h>
#include <ti/datapath/dpc/objectdetection/common/gen_frame_data.h>
#include <ti/utils/testlogger/logger.h>
#include <ti/utils/mathutils/mathutils.h>
#include <ti/control/dpm/dpm.h>

#include <ti/control/mmwavelink/mmwavelink.h>

#include <ti/utils/cycleprofiler/cycle_profiler.h>

/*!< cumulative average of left+right */
#define MMW_NOISE_AVG_MODE_CFAR_CA       ((uint8_t)0U)
/*!< cumulative average of the side (left or right) that is greater */
#define MMW_NOISE_AVG_MODE_CFAR_CAGO     ((uint8_t)1U)
/*!< cumulative average of the side (left or right) that is smaller */
#define MMW_NOISE_AVG_MODE_CFAR_CASO     ((uint8_t)2U)

 /**
  * @brief
  *  Selects the source of test reference data: "true": reads test reference
  *  data (sensor and scene configuration) from the file,
  *  "false": reads the configuration form  @ref gFrameConfig.
  *
  */
bool gReadTestConfigFromBinFile = true;

 /**
  * @brief
  *  Unit test sensor configuration
  *
  */
typedef struct sensorConfig_t_ {
    uint32_t numRxAntennas;     /*!< @brief Number of Rx antennas */
    uint32_t numAdcSamples;     /*!< @brief Number of ADC samples per chirp */
    uint32_t numTxAntAzim;      /*!< @brief Number of Tx azimuth antennas */
    uint32_t numTxAntElev;      /*!< @brief Number of Tx elevation antennas */
    uint32_t numAngleBins;      /*!< @brief Number of angle bins (azimuth FFT size)*/
    uint32_t numChirpsPerFrame; /*!< @brief Number of chirps per frame */
} sensorConfig_t;

/**
 * @brief
 *  Unit test full configuration, including scene and sensor configuration
 *
 */
typedef struct frameConfig_t_ {
    sensorConfig_t sensorConfig; /*!< @brief Sensor configuration */
    sceneConfig_t sceneConfig;   /*!< @brief Scene configuration */
} frameConfig_t;

uint8_t gNumSubFrames = RL_MAX_SUBFRAMES;

/**
 * @brief
 *  Unit test full configuration, including scene and sensor configuration
 *  Array of 4 subframes for debugging use, when the test reference data are not read from file.
 *
 */
frameConfig_t gFrameConfig[RL_MAX_SUBFRAMES] =
{
    {
         /*Test #1*/
        .sensorConfig.numRxAntennas = 4,
        .sensorConfig.numAdcSamples = 256,
        .sensorConfig.numTxAntAzim = 2,
        .sensorConfig.numTxAntElev = 1,
        .sensorConfig.numAngleBins = 64,
    
        /* Needs to be (power of 2)*(gNumTxAntAzim + gNumTxAntElev). E.g 48 when
          gNumTxAntAzim = 2 and gNumTxAntElev = 1 */
        .sensorConfig.numChirpsPerFrame = 16 * 3,

        .sceneConfig.snrdB = 10,
        .sceneConfig.numTargets = 4,

        .sceneConfig.sceneObj[0].amplitude = 1000,
        .sceneConfig.sceneObj[0].azimPhyBin = 10,
        .sceneConfig.sceneObj[0].rngPhyBin = 35,
        .sceneConfig.sceneObj[0].doppPhyBin = 3,
        .sceneConfig.sceneObj[0].elevationDeg = -5,

        .sceneConfig.sceneObj[1].amplitude = 800,
        .sceneConfig.sceneObj[1].azimPhyBin = 7,
        .sceneConfig.sceneObj[1].rngPhyBin = 77,
        .sceneConfig.sceneObj[1].doppPhyBin = -2,
        .sceneConfig.sceneObj[1].elevationDeg = 0,
    
        .sceneConfig.sceneObj[2].amplitude = 700,
        .sceneConfig.sceneObj[2].azimPhyBin = 60,
        .sceneConfig.sceneObj[2].rngPhyBin = 43,
        .sceneConfig.sceneObj[2].doppPhyBin = -5,
        .sceneConfig.sceneObj[2].elevationDeg = 10,
    
        .sceneConfig.sceneObj[3].amplitude = 600,
        .sceneConfig.sceneObj[3].azimPhyBin = 57,
        .sceneConfig.sceneObj[3].rngPhyBin = 100,
        .sceneConfig.sceneObj[3].doppPhyBin = 7,
        .sceneConfig.sceneObj[3].elevationDeg = -20
    },
    {
         /*Test #2*/
        .sensorConfig.numRxAntennas = 4,
        .sensorConfig.numAdcSamples = 512,
        .sensorConfig.numTxAntAzim = 1,
        .sensorConfig.numTxAntElev = 0,
        .sensorConfig.numAngleBins = 64,

        .sensorConfig.numChirpsPerFrame = 16 * 1,

        .sceneConfig.snrdB = 20,
        .sceneConfig.numTargets = 4,

        .sceneConfig.sceneObj[0].amplitude = 1000/2,
        .sceneConfig.sceneObj[0].azimPhyBin = 15,
        .sceneConfig.sceneObj[0].rngPhyBin = 200,
        .sceneConfig.sceneObj[0].doppPhyBin = 6,
        .sceneConfig.sceneObj[0].elevationDeg = -5,

        .sceneConfig.sceneObj[1].amplitude = 800/2,
        .sceneConfig.sceneObj[1].azimPhyBin = 7,
        .sceneConfig.sceneObj[1].rngPhyBin = 70,
        .sceneConfig.sceneObj[1].doppPhyBin = -2,
        .sceneConfig.sceneObj[1].elevationDeg = 0,

        .sceneConfig.sceneObj[2].amplitude = 700/2,
        .sceneConfig.sceneObj[2].azimPhyBin = 20,
        .sceneConfig.sceneObj[2].rngPhyBin = 43,
        .sceneConfig.sceneObj[2].doppPhyBin = -3,
        .sceneConfig.sceneObj[2].elevationDeg = 11,

        .sceneConfig.sceneObj[3].amplitude = 600/2,
        .sceneConfig.sceneObj[3].azimPhyBin = 55,
        .sceneConfig.sceneObj[3].rngPhyBin = 8,
        .sceneConfig.sceneObj[3].doppPhyBin = 7,
        .sceneConfig.sceneObj[3].elevationDeg = -9
    },
    {
         /*Test #3*/
        .sensorConfig.numRxAntennas = 4,
        .sensorConfig.numAdcSamples = 1024,
        .sensorConfig.numTxAntAzim = 2,
        .sensorConfig.numTxAntElev = 0,
        .sensorConfig.numAngleBins = 64,

        .sensorConfig.numChirpsPerFrame = 16*2,

        .sceneConfig.snrdB = 15,
        .sceneConfig.numTargets = 4,

        .sceneConfig.sceneObj[0].amplitude = 1000/2,
        .sceneConfig.sceneObj[0].azimPhyBin = 15,
        .sceneConfig.sceneObj[0].rngPhyBin = 700,
        .sceneConfig.sceneObj[0].doppPhyBin = 7,
        .sceneConfig.sceneObj[0].elevationDeg = -5,

        .sceneConfig.sceneObj[1].amplitude = 800/2,
        .sceneConfig.sceneObj[1].azimPhyBin = 7,
        .sceneConfig.sceneObj[1].rngPhyBin = 100,
        .sceneConfig.sceneObj[1].doppPhyBin = -2,
        .sceneConfig.sceneObj[1].elevationDeg = 0,

        .sceneConfig.sceneObj[2].amplitude = 700/2,
        .sceneConfig.sceneObj[2].azimPhyBin = 20,
        .sceneConfig.sceneObj[2].rngPhyBin = 500,
        .sceneConfig.sceneObj[2].doppPhyBin = -7,
        .sceneConfig.sceneObj[2].elevationDeg = 11,

        .sceneConfig.sceneObj[3].amplitude = 600/2,
        .sceneConfig.sceneObj[3].azimPhyBin = 55,
        .sceneConfig.sceneObj[3].rngPhyBin = 1023,
        .sceneConfig.sceneObj[3].doppPhyBin = 3,
        .sceneConfig.sceneObj[3].elevationDeg = -9
    },
    {
         /*Test #4*/
        .sensorConfig.numRxAntennas = 4,
        .sensorConfig.numAdcSamples = 64,
        .sensorConfig.numTxAntAzim = 2,
        .sensorConfig.numTxAntElev = 1,
        .sensorConfig.numAngleBins = 64,

        .sensorConfig.numChirpsPerFrame = 32*3,

        .sceneConfig.snrdB = 25,
        .sceneConfig.numTargets = 4,

        .sceneConfig.sceneObj[0].amplitude = 1000/2,
        .sceneConfig.sceneObj[0].azimPhyBin = 15,
        .sceneConfig.sceneObj[0].rngPhyBin = 63,
        .sceneConfig.sceneObj[0].doppPhyBin = 5,
        .sceneConfig.sceneObj[0].elevationDeg = -5,

        .sceneConfig.sceneObj[1].amplitude = 800/2,
        .sceneConfig.sceneObj[1].azimPhyBin = 7,
        .sceneConfig.sceneObj[1].rngPhyBin = 10,
        .sceneConfig.sceneObj[1].doppPhyBin = -12,
        .sceneConfig.sceneObj[1].elevationDeg = 0,

        .sceneConfig.sceneObj[2].amplitude = 700/2,
        .sceneConfig.sceneObj[2].azimPhyBin = 20,
        .sceneConfig.sceneObj[2].rngPhyBin = 20,
        .sceneConfig.sceneObj[2].doppPhyBin = 6,
        .sceneConfig.sceneObj[2].elevationDeg = 11,

        .sceneConfig.sceneObj[3].amplitude = 600/2,
        .sceneConfig.sceneObj[3].azimPhyBin = 55,
        .sceneConfig.sceneObj[3].rngPhyBin = 2,
        .sceneConfig.sceneObj[3].doppPhyBin = -3,
        .sceneConfig.sceneObj[3].elevationDeg = -9
    }
};

SOC_Handle          socHandle;
genFrameStruc_t     gGenFrameDataObj;

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

#ifdef SUBSYS_MSS
/*! L3 RAM buffer */
uint8_t gObjectDetection_Test_L3Heap[SOC_L3RAM_SIZE];
#pragma DATA_SECTION(gObjectDetection_Test_L3Heap, ".l3ram");

uint8_t gObjectDetection_Test_TCMramHeap[64*1024]; //starting with 64 K for now
#pragma DATA_SECTION(gObjectDetection_Test_TCMramHeap, ".tcmbHeap");

#define OBJECT_DETECTION_TEST_CHIRP_DATA_SIZE 16*1024
uint8_t gObjectDetection_Test_chirpData[OBJECT_DETECTION_TEST_CHIRP_DATA_SIZE]; //simulating 16KB of ADCBuf memory
#pragma DATA_SECTION(gObjectDetection_Test_chirpData, ".tcmbHeap");
#else

#define OBJECT_DETECTION_L3RAM_CODE_SIZE 64 * 1024

#define OBJECT_DETECTION_TEST_CHIRP_DATA_SIZE 16*1024
uint8_t gObjectDetection_Test_chirpData[OBJECT_DETECTION_TEST_CHIRP_DATA_SIZE]; //simulating 16KB of ADCBuf memory
#pragma DATA_SECTION(gObjectDetection_Test_chirpData, ".hsram");


/*! L3 RAM buffer */
#define OBJECT_DETECTION_L3RAM_HEAP_SIZE SOC_L3RAM_SIZE - OBJECT_DETECTION_L3RAM_CODE_SIZE
uint8_t gObjectDetection_Test_L3Heap[OBJECT_DETECTION_L3RAM_HEAP_SIZE];
#pragma DATA_SECTION(gObjectDetection_Test_L3Heap, ".l3ram");

uint8_t gObjectDetection_Test_TCMramHeap[40*1024];
#pragma DATA_SECTION(gObjectDetection_Test_TCMramHeap, ".l2Heap");

#endif
ObjectDetection_Test_DataPathObj gDataPathObj;

//volatile cycleLog_t gCycleLog;

//#define USE_BUILT_IN_REFERENCE
//#define USE_REFERENCE_ASCI_FILE
//#define USE_REFERENCE_BIN_FILE
#define USE_BUILT_IN_GENERATOR

#ifdef USE_BUILT_IN_REFERENCE
extern const uint32_t ref_input_frame[];
#endif

#define AZIMUTH_ALLOWED_ERROR 4
#define ELEVATION_ALLOWED_ERROR 0.5 /*in degrees*/

/**
  *  @b Description
  *  @n
  *         Initializes pre-start common configuration structure for DPC
  *
  *  @retval None
  *
  */
void ObjectDetection_Test_setCommonProfile(ObjectDetection_Test_DataPathObj *obj)
{
    uint32_t i;
#define MUX_NUM_RX_VIRT_ANTENNAS (SYS_COMMON_NUM_TX_ANTENNAS * SYS_COMMON_NUM_RX_CHANNEL)

    obj->commonCfg.numSubFrames = gNumSubFrames;

    obj->ADCdataBuf = (cmplx16ImRe_t *) &gObjectDetection_Test_chirpData[0];

    /* Fill common config */
    obj->commonCfg.measureRxChannelBiasCfg.enabled = false;
    /* Initialize range bias and rx channel compensation coefficients */
    obj->commonCfg.compRxChanCfg.rangeBias = 0;

    /* Initialize Rx channel phase compensation coefficients to 1 in Q15 */
    for (i = 0; i < MUX_NUM_RX_VIRT_ANTENNAS; i++)
    {
        obj->commonCfg.compRxChanCfg.rxChPhaseComp[i].real = 32767;
        obj->commonCfg.compRxChanCfg.rxChPhaseComp[i].imag = 0;
    }
}

/**
  *  @b Description
  *  @n
  *     Initializes sub-frame test data structure and DPC static
  *     and dynamic configuration structures
  *
  *  @param[in] objComm pointer to common parameters
  *
  *  @param[out] obj pointer to subframe test object
  *
  *  @param[in] sensorConfig pointer to sensor configuration
  *
  *  @param[in] sceneConfig pointer to scene configuration
  *
  *  @retval None
  *
  */
void ObjectDetection_Test_setProfile(ObjectDetection_Test_DataPathObj *objComm,
                     ObjectDetection_Test_SubFr_DataPathObj *obj,
                     sensorConfig_t *sensorConfig,
                     sceneConfig_t *sceneConfig)
{
    uint32_t i;

    /* Hard coded these two values */
    obj->isInterleavedSampInAdcBuf = 0;
    obj->isBpmEnabled = 0;

    obj->numRxAntennas = sensorConfig->numRxAntennas;

    obj->numAdcSamples = sensorConfig->numAdcSamples;
    obj->rxChanOffsetInSamples = MEM_ALIGN(obj->numAdcSamples, 4);
    obj->numRangeBins = mathUtils_pow2roundup(obj->numAdcSamples);
    obj->numTxAntennas = sensorConfig->numTxAntAzim + sensorConfig->numTxAntElev;
    obj->numAngleBins = sensorConfig->numAngleBins;
    obj->numChirpsPerFrame = sensorConfig->numChirpsPerFrame;
    obj->numDopplerChirps = obj->numChirpsPerFrame/obj->numTxAntennas;
    obj->numDopplerBins = mathUtils_pow2roundup(obj->numDopplerChirps);

    obj->numVirtualAntAzim   = sensorConfig->numTxAntAzim * obj->numRxAntennas;
    obj->numVirtualAntElev   = sensorConfig->numTxAntElev * obj->numRxAntennas;
    obj->numVirtualAntennas  = obj->numVirtualAntAzim + obj->numVirtualAntElev;

    /* Workaround for range DPU limitation for FFT size 1024 and 12 virtual antennas case*/
    if ((obj->numVirtualAntennas == 12) && (obj->numRangeBins == 1024))
    {
        obj->numRangeBins = 1022;
    }

    if (obj->numVirtualAntAzim > 2)
    {
        obj->multiObjBeamFormingCfgEnabled = 1;
    }
    else
    {
        obj->multiObjBeamFormingCfgEnabled = 0;
    }

    obj->staticCfg.isValidProfileHasOneTxPerChirp = 0;

    /* Fill static config */
    obj->staticCfg.ADCBufData.dataProperty.adcBits = 2; //16 bits
    obj->staticCfg.ADCBufData.dataProperty.dataFmt = DPIF_DATAFORMAT_COMPLEX16_IMRE;
    obj->staticCfg.ADCBufData.dataProperty.interleave = DPIF_RXCHAN_NON_INTERLEAVE_MODE;
    obj->staticCfg.ADCBufData.dataProperty.numAdcSamples = obj->numAdcSamples;
    obj->staticCfg.ADCBufData.dataProperty.numChirpsPerChirpEvent = 1; // since this is HWA based chain
    obj->staticCfg.ADCBufData.dataProperty.numRxAntennas = obj->numRxAntennas;
    for (i = 0; i < obj->numRxAntennas; i++)
    {
        obj->staticCfg.ADCBufData.dataProperty.rxChanOffset[i] =  i * MEM_ALIGN((obj->numAdcSamples * sizeof(cmplx16ImRe_t)), 16);
    }
    obj->staticCfg.ADCBufData.data = (void *) objComm->ADCdataBuf;
    obj->staticCfg.ADCBufData.dataSize = obj->numRxAntennas * obj->numAdcSamples * sizeof(cmplx16ImRe_t);
    obj->staticCfg.dopplerStep = 1.;
    obj->staticCfg.numTxAntennas = obj->numTxAntennas;
    obj->staticCfg.numChirpsPerFrame = obj->numChirpsPerFrame;
    obj->staticCfg.numDopplerChirps = obj->numChirpsPerFrame/obj->numTxAntennas;
    obj->staticCfg.numDopplerBins = mathUtils_pow2roundup(obj->staticCfg.numDopplerChirps);
    obj->staticCfg.numRangeBins = obj->numRangeBins;
    obj->staticCfg.numVirtualAntAzim = obj->numVirtualAntAzim;
    obj->staticCfg.numVirtualAntElev = obj->numVirtualAntElev;
    obj->staticCfg.numVirtualAntennas = obj->numVirtualAntennas;
    obj->staticCfg.rangeStep = 1.;
    for (i = 0; i < obj->numRxAntennas; i++)
    {
        obj->staticCfg.rxAntOrder[i] = i;
    }
    for (i = 0; i < obj->numTxAntennas; i++)
    {
        obj->staticCfg.txAntOrder[i] = i;
    }


    /* Dynamic configuration */
    obj->dynCfg.calibDcRangeSigCfg.enabled = false;
    obj->dynCfg.cfarCfgDoppler.averageMode = MMW_NOISE_AVG_MODE_CFAR_CA;
    obj->dynCfg.cfarCfgDoppler.cyclicMode = 1;
    obj->dynCfg.cfarCfgDoppler.guardLen = 2;
    obj->dynCfg.cfarCfgDoppler.noiseDivShift = 3;
    obj->dynCfg.cfarCfgDoppler.peakGroupingEn = 1;
    obj->dynCfg.cfarCfgDoppler.peakGroupingScheme = 1;
    obj->dynCfg.cfarCfgDoppler.thresholdScale = 1200;
    obj->dynCfg.cfarCfgDoppler.winLen = 4;
    obj->dynCfg.cfarCfgRange.averageMode = MMW_NOISE_AVG_MODE_CFAR_CASO;
    obj->dynCfg.cfarCfgRange.cyclicMode = 0;
    obj->dynCfg.cfarCfgRange.guardLen = 4;
    obj->dynCfg.cfarCfgRange.noiseDivShift = 3;
    obj->dynCfg.cfarCfgRange.peakGroupingEn = 1;
    obj->dynCfg.cfarCfgRange.peakGroupingScheme = 1;
    obj->dynCfg.cfarCfgRange.thresholdScale = 1200;
    obj->dynCfg.cfarCfgRange.winLen = 8;
    obj->dynCfg.fovAoaCfg.minAzimuthDeg = -80.;
    obj->dynCfg.fovAoaCfg.maxAzimuthDeg = 80.;
    obj->dynCfg.fovAoaCfg.minElevationDeg = -80.;
    obj->dynCfg.fovAoaCfg.maxElevationDeg = 80.;
    obj->dynCfg.fovDoppler.min = -obj->staticCfg.numDopplerBins/2 * obj->staticCfg.dopplerStep;
    obj->dynCfg.fovDoppler.max = (obj->staticCfg.numDopplerBins/2 - 1)  * obj->staticCfg.dopplerStep;
    obj->dynCfg.fovRange.min = 0;
    obj->dynCfg.fovRange.max = (obj->staticCfg.numRangeBins - 1) * obj->staticCfg.rangeStep;
    obj->dynCfg.multiObjBeamFormingCfg.enabled = obj->multiObjBeamFormingCfgEnabled;
    obj->dynCfg.multiObjBeamFormingCfg.multiPeakThrsScal = 0.5;

    if(obj->numVirtualAntAzim > 1)
    {
        obj->dynCfg.prepareRangeAzimuthHeatMap = true;
    }
    else
    {
        obj->dynCfg.prepareRangeAzimuthHeatMap = false;
    }
    obj->dynCfg.staticClutterRemovalCfg.enabled = false;

    if(sceneConfig->numTargets > 4)
    {
        //For large number of targets decrease CFAR window length (needed for testing AoA chunking)
        obj->dynCfg.cfarCfgDoppler.guardLen = 1;
        obj->dynCfg.cfarCfgDoppler.noiseDivShift = 2;
        obj->dynCfg.cfarCfgDoppler.winLen = 2;
        obj->dynCfg.cfarCfgRange.guardLen = 1;
        obj->dynCfg.cfarCfgRange.noiseDivShift = 1;
        obj->dynCfg.cfarCfgRange.winLen = 2;
    }

    System_printf("Number of antennas: Ntx=%d, Nrx=%d\n",obj->numTxAntennas, obj->numRxAntennas);
}

#ifdef USE_BUILT_IN_GENERATOR
/**
  *  @b Description
  *  @n
  *     Initializes generator of test frame data
  *
  *  @retval None
  *
  */
void ObjectDetection_Test_initGenerator(ObjectDetection_Test_SubFr_DataPathObj *obj, sceneConfig_t *sceneConfig)
{
    /* Initialize ref data generator*/
    initFrameData (obj, &gGenFrameDataObj, sceneConfig);
}
#endif

/**
  *  @b Description
  *  @n
  *     Convert Cartesian floating x/y/z values into range/doppler/azimuth index
  *     values. Function is called per detected point.
  *
  *  @param[in] obj Pointer to sub-frame test data
  *
  *  @param[in] objCartesian Cartesian coordinates of the object
  *
  *  @param[in] objOutSideInfo Side information of the object
  *
  *  @param[out] objOut Legacy point-cloud structure with range/dopple index fileds
  *
  *  @retval    None
  */
void  ObjectDetection_Test_convertCoordinates(ObjectDetection_Test_SubFr_DataPathObj *obj,
                              DPIF_PointCloudCartesian * objCartesian,
                              DPIF_PointCloudSideInfo *objOutSideInfo,
                              ObjectDetection_Test_detectedObj *objOut)
{
    int32_t numAngleBins = obj->numAngleBins;
    int32_t temp;
    float range;
    float x = objCartesian->x;
    float y = objCartesian->y;
    float z = objCartesian->z;
    float doppler = objCartesian->velocity;

    range = sqrt(x*x + y*y + z*z);

    objOut->rangeIdx = (uint16_t) MATHUTILS_ROUND_FLOAT(range);
    objOut->dopplerIdx = (int16_t) MATHUTILS_ROUND_FLOAT(doppler);
    temp = (int32_t) MATHUTILS_ROUND_FLOAT(x / range * numAngleBins / 2);
    if (temp < 0)
    {
        temp = temp + numAngleBins;
    }
    objOut->detObj2dAzimIdx = (uint8_t) temp;
    objOut->detObjElevationAngle = objCartesian->z / range; /* Wz/pi (see OOB demo doxygen document) */
    objOut->peakVal = (uint16_t) (objOutSideInfo->noise + objOutSideInfo->snr);
}

/**
  *  @b Description
  *  @n
  *     Compare test data to reference data
  *
  *  @param[in] obj Pointer to subframe reference data
  *
  *  @retval    Comparison result: 1: Passed, 0: Failed
  *
  */
uint32_t ObjectDetection_Test_CompareToRef(ObjectDetection_Test_SubFr_DataPathObj *obj)
{
    uint32_t isOk = 1;
    uint32_t refIdx;
    uint32_t j;
    uint32_t objFound;
    int32_t  dopplerIndex;
    float    outputAngle, refAngle;
    ObjectDetection_Test_detectedObj objOut;
    float elevationAllowedError;

    if(gGenFrameDataObj.numTargets <=4)
    {
        elevationAllowedError = ELEVATION_ALLOWED_ERROR;
    }
    else
    {
        elevationAllowedError = 2.5;
    }

    for (refIdx = 0; refIdx < gGenFrameDataObj.numTargets; refIdx++)
    {
        objFound = 0;
        for (j = 0; j < obj->result->numObjOut; j++)
        {
            ObjectDetection_Test_convertCoordinates(obj,
                                    &obj->result->objOut[j],
                                    &obj->result->objOutSideInfo[j],
                                    &objOut);

            dopplerIndex = objOut.dopplerIdx;

            if (obj->numVirtualAntAzim > 1)
            {
                if ((objOut.rangeIdx == gGenFrameDataObj.targetObj[refIdx].rngPhyBin) &&
                        (dopplerIndex == gGenFrameDataObj.targetObj[refIdx].doppPhyBin))
                {
                   /* check if azimuth is within the allowed range due to Log2(abs(val)) imprecision*/
                      if(((objOut.detObj2dAzimIdx <= gGenFrameDataObj.targetObj[refIdx].azimPhyBin + AZIMUTH_ALLOWED_ERROR) &&
                         ((int32_t)objOut.detObj2dAzimIdx >= (int32_t)gGenFrameDataObj.targetObj[refIdx].azimPhyBin - AZIMUTH_ALLOWED_ERROR))
                          
                         ||((gGenFrameDataObj.targetObj[refIdx].azimPhyBin + AZIMUTH_ALLOWED_ERROR >= 64) && 
                         (objOut.detObj2dAzimIdx <= ((gGenFrameDataObj.targetObj[refIdx].azimPhyBin + AZIMUTH_ALLOWED_ERROR)&0x3F)))
                         
                         ||((gGenFrameDataObj.targetObj[refIdx].azimPhyBin < AZIMUTH_ALLOWED_ERROR ) &&
                         (objOut.detObj2dAzimIdx >= (gGenFrameDataObj.targetObj[refIdx].azimPhyBin +64-AZIMUTH_ALLOWED_ERROR))))
                      {
                         objFound = 1;
                         break;
                      }
                      else
                      {
                        /* If multipeak azimuth is enabled, a different object with different azimuth
                         can be found for the same range/doppler. In this case, this is not necessarily
                         an error condition.
                         If there multipeak search is not enabled, then the detected object for this
                         range and doppler must be withing the range for the azimuth imprecision.*/
                        {
                            System_printf("Azimuth error for index [%d] is %d. Maximum allowed error for azimuth is %d.\n",
                                   j,abs(objOut.detObj2dAzimIdx - gGenFrameDataObj.targetObj[refIdx].azimPhyBin),
                                   AZIMUTH_ALLOWED_ERROR);
                        }            
                      }
                }
            }
            else
            {
                if ((objOut.rangeIdx == gGenFrameDataObj.targetObj[refIdx].rngPhyBin) &&
                        (dopplerIndex == gGenFrameDataObj.targetObj[refIdx].doppPhyBin))
                {
                    objFound = 1;
                    break;
                }
            }
        }

        /* Need to check the elevation angle?*/
        if((objFound == 1) && (obj->numVirtualAntElev > 0))
        {
            /* compare output (detected) angle against reference angle*/
            outputAngle = (float)(180/PI_)*asin(objOut.detObjElevationAngle);
            refAngle    = (float)(180/PI_)*asin(gGenFrameDataObj.targetObj[refIdx].elevationPhy/PI_);

            if((outputAngle > refAngle + elevationAllowedError) ||
               (outputAngle < refAngle - elevationAllowedError) )
            {
                isOk = 0;
                printf("Target Reference number[%d]: Error in elevation estimation. outputAngle=%2.2f refAngle=%2.2f\n",refIdx+1,outputAngle,refAngle);
            }                
        }
        
        if (!objFound)
        {
            isOk = 0;
            break;
        }
    }
    
    /* check if found the correct number of targets */
    if(gGenFrameDataObj.numTargets != obj->result->numObjOut)
    {
        System_printf("Expected number of targets = %d. Detected number of targets = %d.\n",
               gGenFrameDataObj.numTargets, obj->result->numObjOut);
    }
    
    return isOk;
}

/**
  *  @b Description
  *  @n
  *         Prints test results, and cheks aginst reference data
  *
  *  @retval Test status: 1: Passed, 0: Failed
  *
  */
uint32_t ObjectDetection_Test_printAndCheckResults(ObjectDetection_Test_SubFr_DataPathObj *obj, char* feature)
{
    uint32_t testStatus;
    System_printf("Frame processing Done!! num obj: %d\n\r", obj->result->numObjOut);
    {
        uint32_t i;
        int32_t dopplerIndex;
        ObjectDetection_Test_detectedObj objOut;

        System_printf("\tPeak,\tRange,\tDoppler,Azimuth,Elevation\n");
        for (i = 0; i < obj->result->numObjOut; i++)
        {
            ObjectDetection_Test_convertCoordinates(obj,
                                    &obj->result->objOut[i],
                                    &obj->result->objOutSideInfo[i],
                                    &objOut);
            dopplerIndex = objOut.dopplerIdx;
            System_printf("[%2d]:\t%5d\t%4d\t%4d\t%4d",i ,
                    objOut.peakVal,
                    objOut.rangeIdx,
                    dopplerIndex,
                    objOut.detObj2dAzimIdx);
                    
            if(obj->numVirtualAntElev > 0)
            {
                /*detObjElevationAngle = Wz/PI.
                  Wz = PI*sin(phy)
                  phy = arcsin(detObjElevationAngle)
                */
                printf("\t%2.2lf\n",180*asin(objOut.detObjElevationAngle)/PI_);
            }            
            else
            {
                System_printf("\tNA\n");
            }
        }

        testStatus = ObjectDetection_Test_CompareToRef(obj);
        if(testStatus == 1)
        {
            System_printf("\nTest PASSED!\n");
            MCPI_setFeatureTestResult(feature, MCPI_TestResult_PASS);
        }
        else
        {
            System_printf("\nTest FAILED!\n");
            MCPI_setFeatureTestResult(feature, MCPI_TestResult_FAIL);
        }

    }
    return testStatus;
}

/**
  *  @b Description
  *  @n
  *     Opens file containing ADC samples of reference data
  *
  *  @retval None
  *
  */
void ObjectDetection_Test_ReferenceFileInit(void)
{
#ifdef USE_REFERENCE_ASCI_FILE
    FILE * fileId = fopen("..\\matlab\\ref_input_frame.dat", "r");
    if (fileId == NULL)
    {
        System_printf("Error:  Cannot open reference file!\n");
        exit(0);
    }
#endif

#ifdef USE_REFERENCE_BIN_FILE
    FILE * fileId = fopen("..\\matlab\\ref_input_frame.bin", "rb");
    if (fileId == NULL)
    {
        System_printf("Error:  Cannot open reference file!\n");
        exit(0);
    }
#endif
}

/* Generate chirp samples */
/**
  *  @b Description
  *  @n
  *     Get chirp test samples
  *  @retval None
  *
  */
void ObjectDetection_Test_getChirpData(ObjectDetection_Test_DataPathObj *objComm,
                       ObjectDetection_Test_SubFr_DataPathObj *obj)
{
#ifndef USE_BUILT_IN_GENERATOR
    uint32_t refReadIndex = 0;
#endif
#ifndef USE_BUILT_IN_GENERATOR
    uint32_t sampIdx, antIndx;
    uint32_t * cmplxSampPtr;
    cmplxSampPtr = (uint32_t *) obj->ADCdataBuf;
#endif

#ifdef USE_BUILT_IN_REFERENCE
    for (antIndx = 0; antIndx < obj->numRxAntennas;  antIndx++)
    {
        for (sampIdx = 0; sampIdx < obj->numRangeBins;  sampIdx++)
        {
            cmplxSampPtr[(antIndx * obj->numRangeBins) + sampIdx] = ref_input_frame[refReadIndex++];
        }
    }
#endif

#ifdef USE_REFERENCE_ASCI_FILE
    uint32_t refReadIndex = 0;
    for (antIndx = 0; antIndx < obj->numRxAntennas;  antIndx++)
    {
        for (sampIdx = 0; sampIdx < obj->numRangeBins;  sampIdx++)
        {
            fscanf(fileId, "%x\n", &cmplxSampPtr[(antIndx * obj->numRangeBins) + sampIdx]);
        }
    }
#endif

#ifdef USE_REFERENCE_BIN_FILE
    fread( (uint8_t *) cmplxSampPtr, sizeof( uint32_t ), obj->numRxAntennas * obj->numRangeBins, fileId );
#endif

#ifdef USE_BUILT_IN_GENERATOR
    /* The chirp will be placed/written into HWA's M0 memory to be processed by HWA in this test.
       This is done because CPU can not write to real ADC buffer, therefore, in this test
       the chirp is written directly into the HWA memory.*/
    genChirpData (obj, &gGenFrameDataObj, (uint8_t *) objComm->ADCdataBuf);
#endif
}





/**
 *  @b Description
 *  @n
 *      Print assert information to console.
 */
void _ObjectDetection_Test_debugAssert(int32_t expression, const char *file, int32_t line)
{
    if (!expression) {
        System_printf ("Exception: %s, line %d.\n",file,line);
    }
}

#define ObjectDetection_Test_debugAssert(expression) {                                      \
                     _ObjectDetection_Test_debugAssert(expression,      \
                              __FILE__, __LINE__);         \
                     DebugP_assert(expression);             \
                    }
                                        


/**
 *  @b Description
 *  @n
 *      Call back function for EDMA CC (Channel controller) error as per EDMA API.
 *      Declare fatal error if happens, the output errorInfo can be examined if code
 *      gets trapped here.
 */
void ObjectDetection_Test_EDMA_errorCallbackFxn(EDMA_Handle handle, EDMA_errorInfo_t *errorInfo)
{
    ObjectDetection_Test_debugAssert(0);
}

/**
 *  @b Description
 *  @n
 *      Call back function for EDMA transfer controller error as per EDMA API.
 *      Declare fatal error if happens, the output errorInfo can be examined if code
 *      gets trapped here.
 */
void ObjectDetection_Test_EDMA_transferControllerErrorCallbackFxn(EDMA_Handle handle,
                EDMA_transferControllerErrorInfo_t *errorInfo)
{
    ObjectDetection_Test_debugAssert(0);
}

/**
 *  @b Description
 *  @n
 *      DPM Registered Report Handler. The DPM Module uses this registered function to notify
 *      the application about DPM reports.
 *
 *  @param[in]  reportType
 *      Report Type
 *  @param[in]  instanceId
 *      Instance Identifier which generated the report
 *  @param[in]  errCode
 *      Error code if any.
 *  @param[in] arg0
 *      Argument 0 interpreted with the report type
 *  @param[in] arg1
 *      Argument 1 interpreted with the report type
 *
 *  @retval
 *      Not Applicable.
 */
uint16_t gCurrTestId;
uint32_t gMaxL3RamUsage = 0;
uint16_t gMaxL3RamUsageTestId = 0;

uint32_t gMaxTCMUsage = 0;
uint16_t gMaxTCMUsageTestId = 0;

/**
  *  @b Description
  *  @n
  *     Report function registered with DPM to report events
  *     and DPM processing status
  *
  *  @retval None
  *
  */
static void ObjectDetection_Test_reportFxn
(
    DPM_Report  reportType,
    uint32_t    instanceId,
    int32_t     errCode,
    uint32_t    arg0,
    uint32_t    arg1
)
{

    /* Only errors are logged on the console: */
    if (errCode != 0)
    {
        /* Error: Detected log on the console and die all errors are FATAL currently. */
        System_printf ("Error: DPM Report %d received with error:%d arg0:0x%x arg1:0x%x\n",
                        reportType, errCode, arg0, arg1);
        DebugP_assert (0);
    }

    /* Processing further is based on the reports received: This is the control of the profile
     * state machine: */
    switch (reportType)
    {
        case DPM_Report_IOCTL:
        {
            /*****************************************************************
             * DPC has been configured without an error:
             * - This is an indication that the profile configuration commands
             *   went through without any issues.
             *****************************************************************/

            if (arg0 == DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG)
            {
                DPC_ObjectDetection_PreStartCfg *cfg;
                DPC_ObjectDetection_DPC_IOCTL_preStartCfg_memUsage *memUsage;

                /* Get memory usage from preStartCfg */
                cfg = (DPC_ObjectDetection_PreStartCfg*)arg1;
                memUsage = &cfg->memUsage;

                if(memUsage->L3RamUsage > gMaxL3RamUsage)
                {
                    gMaxL3RamUsage = memUsage->L3RamUsage;
                    gMaxL3RamUsageTestId = gCurrTestId;
                }
                if(memUsage->CoreLocalRamUsage > gMaxTCMUsage)
                {
                    gMaxTCMUsage = memUsage->CoreLocalRamUsage;
                    gMaxTCMUsageTestId = gCurrTestId;
                }
                System_printf("DPC ObjDet Memory usage (SubFrameIndx = %d): L3: %d Bytes, TCM: %d Bytes\n",
                              cfg->subFrameNum,
                              memUsage->L3RamUsage,
                              memUsage->CoreLocalRamUsage);

            }
            break;
        }
        case DPM_Report_DPC_STARTED:
        {
            /*****************************************************************
             * DPC has been started without an error:
             * - we can start chirping
             *****************************************************************/

            break;
        }
        case DPM_Report_NOTIFY_DPC_RESULT:
        {
            /*****************************************************************
             * DPC Results have been passed:
             * - This implies that we have valid profile results which have
             *   been received from the profile.
             *****************************************************************/

            break;
        }
        case DPM_Report_DPC_ASSERT:
        {
            DPM_DPCAssert*  ptrAssert;

            /*****************************************************************
             * DPC Fault has been detected:
             * - This implies that the DPC has crashed.
             * - The argument0 points to the DPC assertion information
             *****************************************************************/
            ptrAssert = (DPM_DPCAssert*)arg0;
            System_printf ("DSS Exception: %s, line %d.\n", ptrAssert->fileName,
                       ptrAssert->lineNum);
            break;
        }
        case DPM_Report_DPC_STOPPED:
        {
            /*****************************************************************
             * DPC has been stopped without an error:
             * - This implies that the DPC can either be reconfigured or
             *   restarted. 
             *****************************************************************/
            break;
        }
        case DPM_Report_NOTIFY_DPC_RESULT_ACKED:
        {
            /*****************************************************************
             * DPC Results have been acked:
             * - This implies that the results are received.
             *****************************************************************/
            break;
        }
        case DPM_Report_DPC_INFO:
        {
            /* Currently objDetHwa does not use this feature. */
            break;
        }
        default:
        {
            DebugP_assert (0);
            break;
        }
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      DPM Execution Task
 *
 *  @retval
 *      Not Applicable.
 */
static void ObjectDetection_Test_dpmTask(UArg arg0, UArg arg1)
{
    int32_t     errCode;
    DPM_Buffer  resultBuffer;

    while (1)
    {
        /* Execute the DPM module: */
        errCode = DPM_execute (gDataPathObj.dpmHandle, &resultBuffer);
        if (errCode < 0) {
            System_printf ("Error: DPM execution failed [Error code %d]\n", errCode);
            ObjectDetection_Test_debugAssert (0);
        }
        else
        {
            //System_printf ("DPM executed!size=%d\n", resultBuffer.size[0]);
            if (resultBuffer.size[0] == sizeof(DPC_ObjectDetection_ExecuteResult))
            {
                DPC_ObjectDetection_ExecuteResult *res = (DPC_ObjectDetection_ExecuteResult *)resultBuffer.ptrBuffer[0];
                uint32_t subFrameIdx = res->subFrameIdx;
                gDataPathObj.testSubFrDataPathObj[subFrameIdx].result = (DPC_ObjectDetection_ExecuteResult *)resultBuffer.ptrBuffer[0];

                /* Indicate to test task that the sub-frame is completed */
                SemaphoreP_post (gDataPathObj.semaphoreHandle);
            }

        }
    }
}


/**
 *  @b Description
 *  @n
 *      Initializes of HWA.
 *
 *  @param[in] obj Pointer to main test data structure
 *
 *
 */
void ObjectDetection_Test_hwaInit(ObjectDetection_Test_DataPathObj *obj)
{
    /* Initialize the HWA */
    HWA_init();
}

/**
 *  @b Description
 *  @n
 *      Initializes  EDMA.
 */
void ObjectDetection_Test_edmaInit(ObjectDetection_Test_DataPathObj *obj)
{
    uint8_t edmaNumInstances, inst;
    //Semaphore_Params       semParams;
    int32_t errorCode;

    edmaNumInstances = EDMA_getNumInstances();
    for (inst = 0; inst < edmaNumInstances; inst++)
    {
        errorCode = EDMA_init(inst);
        if (errorCode != EDMA_NO_ERROR)
        {
            //System_printf ("Debug: EDMA instance %d initialization returned error %d\n", errorCode);
            ObjectDetection_Test_debugAssert(0);
            return;
        }
        //System_printf ("Debug: EDMA instance %d has been initialized\n", inst);
    }

}

/**
 *  @b Description
 *  @n
 *      Open HWA instance.
 */
void ObjectDetection_Test_hwaOpen(ObjectDetection_Test_DataPathObj *obj, SOC_Handle socHandle)
{
    int32_t             errCode;

    /* Open the HWA Instance */
    obj->hwaHandle = HWA_open(0, socHandle, &errCode);
    if (obj->hwaHandle == NULL)
    {
        //System_printf("Error: Unable to open the HWA Instance err:%d\n",errCode);
        ObjectDetection_Test_debugAssert (0);
        return;
    }
    //System_printf("Debug: HWA Instance %p has been opened successfully\n", obj->hwaHandle);
}

/**
 *  @b Description
 *  @n
 *      Close HWA instance.
 */
void ObjectDetection_Test_hwaClose(ObjectDetection_Test_DataPathObj *obj)
{
    int32_t             errCode;

    /* Close the HWA Instance */
    errCode = HWA_close(obj->hwaHandle);
    if (errCode != 0)
    {
        //System_printf("Error: Unable to close the HWA Instance err:%d\n",errCode);
        ObjectDetection_Test_debugAssert (0);
        return;
    }
    //System_printf("Debug: HWA Instance %p has been closed successfully\n", obj->hwaHandle);
}


/**
 *  @b Description
 *  @n
 *      Open EDMA.
 */
void ObjectDetection_Test_edmaOpen(ObjectDetection_Test_DataPathObj *obj)
{
    int32_t             errCode;
    EDMA_instanceInfo_t edmaInstanceInfo;
    EDMA_errorConfig_t  errorConfig;
    uint32_t            edmaCCIdx=0;

    for (edmaCCIdx=0;edmaCCIdx<EDMA_NUM_CC;edmaCCIdx++)
    {
        obj->edmaHandle[edmaCCIdx] = EDMA_open(edmaCCIdx, &errCode, &edmaInstanceInfo);

        if (obj->edmaHandle[edmaCCIdx] == NULL)
        {
            //System_printf("Error: Unable to open the EDMA Instance err:%d\n",errCode);
            ObjectDetection_Test_debugAssert (0);
            return;
        }
        //System_printf("Debug: EDMA Instance %p has been opened successfully\n", obj->edmaHandle[edmaCCIdx]);

        errorConfig.isConfigAllEventQueues = true;
        errorConfig.isConfigAllTransferControllers = true;
        errorConfig.isEventQueueThresholdingEnabled = true;
        errorConfig.eventQueueThreshold = EDMA_EVENT_QUEUE_THRESHOLD_MAX;
        errorConfig.isEnableAllTransferControllerErrors = true;
        errorConfig.callbackFxn = ObjectDetection_Test_EDMA_errorCallbackFxn;
        errorConfig.transferControllerCallbackFxn = ObjectDetection_Test_EDMA_transferControllerErrorCallbackFxn;
        if ((errCode = EDMA_configErrorMonitoring(obj->edmaHandle[edmaCCIdx], &errorConfig)) != EDMA_NO_ERROR)
        {
            //System_printf("Error: EDMA_configErrorMonitoring() failed with errorCode = %d\n", errCode);
            ObjectDetection_Test_debugAssert (0);
            return;
        }
    }

}

/**
 *  @b Description
 *  @n
 *      Close EDMA.
 */
void ObjectDetection_Test_edmaClose(ObjectDetection_Test_DataPathObj *obj)
{
    uint32_t            edmaCCIdx=0;

    for (edmaCCIdx=0;edmaCCIdx<EDMA_NUM_CC;edmaCCIdx++)
    {
        EDMA_close(obj->edmaHandle[edmaCCIdx]);
    }
}





/**
  *  @b Description
  *  @n     Initializes DPM, and launches DPM task
  *
  *  @param[in] dataPathObj Pointer to main test data structure
  *
  *  @retval    None
  */
void ObjectDetection_Test_DPCInit(ObjectDetection_Test_DataPathObj *dataPathObj)
{
    DPM_InitCfg                         dpmInitCfg;
    int32_t                             errCode;
    Task_Params                         taskParams;
    uint32_t                            edmaCCIdx;
    
    DPC_ObjectDetection_InitParams      objDetInitParams;

    /*****************************************************************************
     * Initialization of the DPM Module:
     *****************************************************************************/
    memset ((void *)&objDetInitParams, 0, sizeof(DPC_ObjectDetection_InitParams));
    objDetInitParams.hwaHandle = dataPathObj->hwaHandle;
    for (edmaCCIdx = 0; edmaCCIdx < EDMA_NUM_CC; edmaCCIdx++)
    {
        objDetInitParams.edmaHandle[edmaCCIdx] = dataPathObj->edmaHandle[edmaCCIdx];
    }

    /* Memory related config */
    objDetInitParams.L3ramCfg.addr        = &gObjectDetection_Test_L3Heap;
    objDetInitParams.L3ramCfg.size        = sizeof(gObjectDetection_Test_L3Heap);
    objDetInitParams.CoreLocalRamCfg.addr = &gObjectDetection_Test_TCMramHeap;
    objDetInitParams.CoreLocalRamCfg.size = sizeof(gObjectDetection_Test_TCMramHeap);

    /* Call-back config, below initializations not needed due to memset above */
    //objDetInitParams.processCallBackCfg.processFrameBeginCallBackFxn = NULL;
    //objDetInitParams.processCallBackCfg.processInterFrameBeginCallBackFxn = NULL;
    
    memset ((void *)&dpmInitCfg, 0, sizeof(DPM_InitCfg));
    
    /* Setup the configuration: */
    dpmInitCfg.socHandle        = socHandle;
    dpmInitCfg.ptrProcChainCfg  = &gDPC_ObjectDetectionCfg;
    dpmInitCfg.instanceId       = 0xFEEDFEED;
    dpmInitCfg.domain           = DPM_Domain_LOCALIZED;
    dpmInitCfg.reportFxn        = ObjectDetection_Test_reportFxn;
    dpmInitCfg.arg              = &objDetInitParams;
    dpmInitCfg.argSize          = sizeof(DPC_ObjectDetection_InitParams);
    

    /* Initialize the DPM Module: */
    dataPathObj->dpmHandle = DPM_init (&dpmInitCfg, &errCode);
    if (dataPathObj->dpmHandle == NULL)
    {
        System_printf ("Error: Unable to initialize the DPM Module [Error: %d]\n", errCode);
        ObjectDetection_Test_debugAssert (0);
        return;
    }

    /* Launch the DPM Task */
    Task_Params_init(&taskParams);
    taskParams.priority  = 5;
    taskParams.stackSize = 4*1024;
    Task_create(ObjectDetection_Test_dpmTask, &taskParams, NULL);
}

/**
  *  @b Description
  *  @n
  *     DPM Deinitialization
  *
  *  @param[in] dataPathObj Pointer to main test data structure
  *
  *  @retval    None
  *
  */
void ObjectDetection_Test_DPCDeinit(ObjectDetection_Test_DataPathObj *dataPathObj)
{
    int32_t                             errCode;

    /* Initialize the DPM Module: */
    errCode = DPM_deinit (dataPathObj->dpmHandle);
    if (errCode<0)
    {
        System_printf ("Error: Unable to de-initialize the DPM Module [Error: %d]\n", errCode);
        ObjectDetection_Test_debugAssert (0);
        return;
    }

}

/**
  *  @b Description
  *  @n
  *     Initializes test components: EDMA, HWA, DPM/DPC
  *
  *  @param[in] dataPathObj Pointer to main test data structure
  *
  *  @retval    None
  *
  */
void ObjectDetection_Test_initComponents(ObjectDetection_Test_DataPathObj *dataPathObj)
{
    SemaphoreP_Params   semParams;

    /* Initialize the HWA */
    ObjectDetection_Test_hwaInit(dataPathObj);
    ObjectDetection_Test_hwaOpen(dataPathObj, socHandle);

    /* EDMA init */
    ObjectDetection_Test_edmaInit(dataPathObj);
    ObjectDetection_Test_edmaOpen(dataPathObj);

    /* DPM/DPC init */
    ObjectDetection_Test_DPCInit(dataPathObj);

    /* Initialize semaphore for sub-frame unit test completion indication */
    SemaphoreP_Params_init (&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    dataPathObj->semaphoreHandle = SemaphoreP_create(0, &semParams);

}

/**
  *  @b Description
  *  @n
  *         DeInitializes test components: HWA, EDMA, DPC
  *
  *  @param[in] dataPathObj Pointer to main test data structure
  *
  *  @retval    None
  *
  */
void ObjectDetection_Test_deinitComponents(ObjectDetection_Test_DataPathObj *dataPathObj)
{

    /* HWA close*/
    ObjectDetection_Test_hwaClose(dataPathObj);

    /* EDMA close */
    ObjectDetection_Test_edmaClose(dataPathObj);

    /* DPM/DPC deinit */
    ObjectDetection_Test_DPCDeinit(dataPathObj);

}

/**
  *  @b Description
  *  @n
  *     Sends ioctl trigger start frame to DPC to simulate the frame interrupt.
  *
  *  @param[in] dataPathObj Pointer to main test data structure
  *
  *  @retval    None
  *
  */
void ObjectDetection_Test_trigFrameStart(ObjectDetection_Test_DataPathObj *dataPathObj)
{
    int32_t                         errCode;


    /* send DPC_OBJDET_IOCTL__TRIGGER_FRAME */
    errCode = DPM_ioctl (dataPathObj->dpmHandle,
                         DPC_OBJDET_IOCTL__TRIGGER_FRAME,
                         NULL,
                         0);
    if (errCode < 0)
    {
        System_printf ("Error: Unable to send DPC_OBJDET_IOCTL__TRIGGER_FRAME [Error:%d]\n", errCode);
        ObjectDetection_Test_debugAssert (0);
        return;
    }
    else
    {
        System_printf ("TEST: Sent to DPC: DPC_OBJDET_IOCTL__TRIGGER_FRAME\n");
    }


}

/**
  *  @b Description
  *  @n
  *     Configures DPC for 4 subframes. Sends one ioctl for
  *     common configuration and one ioctal per each subframe.
  *
  *  @param[in] dataPathObj Pointer to main test data structure
  *
  *  @retval    None
  */
void ObjectDetection_Test_configDPC(ObjectDetection_Test_DataPathObj *dataPathObj)
{
    int32_t                         errCode;
    DPC_ObjectDetection_PreStartCommonCfg objDetPreStartCommonCfg;
    DPC_ObjectDetection_PreStartCfg objDetPreStartCfg;
    int32_t subFrameIdx;

    memset(&objDetPreStartCommonCfg, 0, sizeof(DPC_ObjectDetection_PreStartCommonCfg));
    memset(&objDetPreStartCfg, 0, sizeof(DPC_ObjectDetection_PreStartCfg));

    /**************************************/
    /* Prepare and send the pre start common configuration */
    /**************************************/

    /* Fill configuration */
    objDetPreStartCommonCfg = dataPathObj->commonCfg;

    /* send DPC_OBJDET_IOCTL__STATIC_PRE_START_COMMON_CFG */
    errCode = DPM_ioctl (dataPathObj->dpmHandle,
                         DPC_OBJDET_IOCTL__STATIC_PRE_START_COMMON_CFG,
                         &objDetPreStartCommonCfg,
                         sizeof (DPC_ObjectDetection_PreStartCommonCfg));
    if (errCode < 0)
    {
        System_printf ("Error: Unable to send DPC_OBJDET_IOCTL__STATIC_PRE_START_COMMON_CFG [Error:%d]\n", errCode);
        ObjectDetection_Test_debugAssert (0);
        return;
    }
    else
    {
        System_printf ("TEST: Sent to DPC: DPC_OBJDET_IOCTL__STATIC_PRE_START_COMMON_CFG\n");
    }

    for (subFrameIdx = 0; subFrameIdx < gNumSubFrames; subFrameIdx++)
    {
        /**************************************/
        /* Prepare and send the pre start cfg */
        /**************************************/
        objDetPreStartCfg.subFrameNum = subFrameIdx;

        /* Fill static configuration */
        objDetPreStartCfg.staticCfg = dataPathObj->testSubFrDataPathObj[subFrameIdx].staticCfg;

        /* Fill dynamic configuration */
        objDetPreStartCfg.dynCfg = dataPathObj->testSubFrDataPathObj[subFrameIdx].dynCfg;

        /* send DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG */
        errCode = DPM_ioctl (dataPathObj->dpmHandle,
                             DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG,
                             &objDetPreStartCfg,
                             sizeof (DPC_ObjectDetection_PreStartCfg));
        if (errCode < 0)
        {
            System_printf ("Error: Unable to send DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG [Error:%d]\n", errCode);
            ObjectDetection_Test_debugAssert (0);
            return;
        }
        else
        {
            System_printf ("TEST: Sent to DPC: DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG, subFrame %d\n", subFrameIdx);
        }
    }
    
}


/**
  *  @b Description
  *  @n
  *     Invokes DPM_start
  *
  *  @retval None
  *
  */
void ObjectDetection_Test_startDPC(ObjectDetection_Test_DataPathObj *dataPathObj)
{
    int32_t                         errCode;

    
    /* Start the DPM Profile: */
    errCode = DPM_start (dataPathObj->dpmHandle);
    if (errCode < 0)
    {
        /* Error: Unable to start the profile */
        System_printf ("Error: Unable to start the DPM Profile [Error: %d]\n", errCode);
        ObjectDetection_Test_debugAssert(0);
    }

}

/**
  *  @b Description
  *  @n
  *         Invokes DPM_stop
  *
  *  @retval    None
  *
  */
void ObjectDetection_Test_stopDPC(ObjectDetection_Test_DataPathObj *dataPathObj)
{
    int32_t                         errCode;

    
    /* Start the DPM Profile: */
    errCode = DPM_stop (dataPathObj->dpmHandle);
    if (errCode < 0)
    {
        /* Error: Unable to start the profile */
        System_printf ("Error: Unable to stop the DPM Profile [Error: %d]\n", errCode);
        ObjectDetection_Test_debugAssert(0);
    }

}


/**
 *  @b Description
 *  @n
 *      Test task - Main task that performs ObjectDetecion DPC unit test.
 *
 *  @retval
 *      Not Applicable.
 */
void ObjectDetection_Test_task(UArg arg0, UArg arg1)
{
    ObjectDetection_Test_DataPathObj *dataPathObj = &gDataPathObj;
    int32_t  errCode; 
    uint32_t i;

    FILE * fileId;    
    uint32_t numTests;
    uint32_t testIndx;
    char tempMsg[20]; 
    char featureName[200];
    bool allTestsPassed = true;
    uint32_t testStatus;
    DPC_ObjectDetection_ExecuteResultExportedInfo exportInfo;
    uint8_t subFrameIndx;
     
    ObjectDetection_Test_ReferenceFileInit();

    /* Initialize HWA, EDMA, DPM/DPC */
    ObjectDetection_Test_initComponents(dataPathObj);

    if(gReadTestConfigFromBinFile)
    {
#if defined(SOC_XWR18XX) && defined(SUBSYS_MSS)
        fileId = fopen("testdata\\xwr18xx_mss_data_path_test_config.bin", "rb");
#elif defined(SOC_XWR18XX) && defined(SUBSYS_DSS)
        fileId = fopen("testdata\\xwr18xx_dss_data_path_test_config.bin", "rb");
#elif defined(SOC_XWR68XX) && defined(SUBSYS_MSS)
        fileId = fopen("testdata\\xwr68xx_mss_data_path_test_config.bin", "rb");
#elif defined(SOC_XWR68XX) && defined(SUBSYS_DSS)
        fileId = fopen("testdata\\xwr68xx_dss_data_path_test_config.bin", "rb");
#else
#error "Error: Unsupported platform"
#endif
        if (fileId == NULL)
        {
            System_printf("Error:  Cannot open xwr68xx_data_path_test_config.bin!\n");
            exit(0);
        }
        /* read in number of tests to be run */
        fread(&numTests, sizeof(uint32_t),1,fileId);
    }
    else
    {
        numTests = 4;
    }
    System_printf("%d Tests to be run\n\r", numTests);
    if (numTests % gNumSubFrames)
    {
        System_printf("numTests = %d must be a multiple of gNumSubFrames = %d\n", numTests, gNumSubFrames);
        BIOS_exit(0);
    }


    /* config common */
    ObjectDetection_Test_setCommonProfile(dataPathObj);
    
    /* process each test */
    subFrameIndx = 0;
    testIndx = 0;
    while (testIndx < numTests)
    {
        sensorConfig_t *sensorConfig;
        sceneConfig_t *sceneConfig;
        uint32_t sceneObjIndx;

        gCurrTestId = testIndx+1;
        for (subFrameIndx = 0; subFrameIndx < gNumSubFrames; subFrameIndx++)
        {
            sensorConfig = &gFrameConfig[subFrameIndx].sensorConfig;
            sceneConfig = &gFrameConfig[subFrameIndx].sceneConfig;

            if (gReadTestConfigFromBinFile)
            {
                /* read in test config */
                fread(&sensorConfig->numRxAntennas, sizeof(uint32_t),1,fileId);
                fread(&sensorConfig->numAdcSamples, sizeof(uint32_t),1,fileId);
                fread(&sensorConfig->numTxAntAzim, sizeof(uint32_t),1,fileId);
                fread(&sensorConfig->numTxAntElev, sizeof(uint32_t),1,fileId);
                fread(&sensorConfig->numAngleBins, sizeof(uint32_t),1,fileId);
                fread(&sensorConfig->numChirpsPerFrame, sizeof(uint32_t),1,fileId);

                fread(&sceneConfig->numTargets, sizeof(uint32_t),1,fileId);
                fread(&sceneConfig->snrdB, sizeof(int32_t),1,fileId);

                for (sceneObjIndx = 0; sceneObjIndx < sceneConfig->numTargets; sceneObjIndx++)
                {
                    fread(&sceneConfig->sceneObj[sceneObjIndx].amplitude, sizeof(uint32_t),1,fileId);
                    fread(&sceneConfig->sceneObj[sceneObjIndx].azimPhyBin, sizeof(uint32_t),1,fileId);
                    fread(&sceneConfig->sceneObj[sceneObjIndx].rngPhyBin, sizeof(uint32_t),1,fileId);
                    fread(&sceneConfig->sceneObj[sceneObjIndx].doppPhyBin, sizeof(int32_t),1,fileId);
                    fread(&sceneConfig->sceneObj[sceneObjIndx].elevationDeg, sizeof(int32_t),1,fileId);
                }
            }
            /* check validity of arguments */
            if((sensorConfig->numChirpsPerFrame %
                (sensorConfig->numTxAntAzim + sensorConfig->numTxAntElev)) != 0)
            {
                System_printf("\n#### BAD argument. numChirpsPerFrame needs to be (power of 2)*(numTxAntAzim + numTxAntElev)\n\n");
            }

            /* config datapathObj from sensorConfig */
            ObjectDetection_Test_setProfile(dataPathObj,
                            &dataPathObj->testSubFrDataPathObj[subFrameIndx],
                            &gFrameConfig[subFrameIndx].sensorConfig,
                            sceneConfig);
        }

        /* config DPC from dataPathObj */
        ObjectDetection_Test_configDPC(dataPathObj);

        /* start DPC */
        ObjectDetection_Test_startDPC(dataPathObj);

        for (subFrameIndx = 0; subFrameIndx < gNumSubFrames; subFrameIndx++)
        {
            /* Simulate the trigger of frame start event for DPC */
            ObjectDetection_Test_trigFrameStart(dataPathObj);

            sensorConfig = &gFrameConfig[subFrameIndx].sensorConfig;
            sceneConfig = &gFrameConfig[subFrameIndx].sceneConfig;

            System_printf("Read Test #%d \r", testIndx+1);

            System_printf("numRxAntennas = %d\r", sensorConfig->numRxAntennas);
            System_printf("numAdcSamples = %d\r", sensorConfig->numAdcSamples);
            System_printf("numTxAntAzim = %d\r", sensorConfig->numTxAntAzim);
            System_printf("numTxAntElev = %d\r", sensorConfig->numTxAntElev);
            System_printf("numAngleBins = %d\r",  sensorConfig->numAngleBins);
            System_printf("numChirpsPerFrame = %d\r", sensorConfig->numChirpsPerFrame);
            System_printf("numTargets = %d\r", sceneConfig->numTargets);
            System_printf("snrdB = %d\r", sceneConfig->snrdB);

            for (sceneObjIndx = 0; sceneObjIndx < sceneConfig->numTargets; sceneObjIndx++)
            {
                System_printf("sceneObj[%d] = \r", sceneObjIndx);
                System_printf("    amplitude = %d\r", sceneConfig->sceneObj[sceneObjIndx].amplitude);
                System_printf("    rngPhyBin = %d\r", sceneConfig->sceneObj[sceneObjIndx].rngPhyBin);
                System_printf("    doppPhyBin = %d\r", sceneConfig->sceneObj[sceneObjIndx].doppPhyBin);
                System_printf("    azimPhyBin = %d\r", sceneConfig->sceneObj[sceneObjIndx].azimPhyBin);
                System_printf("    elevationDeg = %d\r", sceneConfig->sceneObj[sceneObjIndx].elevationDeg);
            }
            System_printf("\n");


            /* prepare the feature to be printed in test report */
            if (sceneConfig->numTargets == 2)
            {
                sprintf(tempMsg, "%s", " m");
            }
            else
            {
                sprintf(tempMsg, "%s", " r");
            }
            sprintf(featureName, "#%d%s%d%s%d%s%d%s%d%s%d%s",testIndx+1,
                ":TxA", sensorConfig->numTxAntAzim,
                " TxE", sensorConfig->numTxAntElev,
                " Rx", sensorConfig->numRxAntennas,
                " R", sensorConfig->numAdcSamples,
                " D", sensorConfig->numChirpsPerFrame /
                    (sensorConfig->numTxAntAzim + sensorConfig->numTxAntElev),
                tempMsg);


#ifdef USE_BUILT_IN_GENERATOR
            ObjectDetection_Test_initGenerator(&dataPathObj->testSubFrDataPathObj[subFrameIndx], &gFrameConfig[subFrameIndx].sceneConfig);
            System_printf("ObjectDetection_Test_initGenerator done\n");
#endif


            /* populate all chirps per frame and trigger chirp avail event*/
            for(i=0; i<dataPathObj->testSubFrDataPathObj[subFrameIndx].numChirpsPerFrame; i++)
            {
                /* Generate one chirp of ADC samples and to put data in TCMB */
                ObjectDetection_Test_getChirpData(dataPathObj,
                                  &dataPathObj->testSubFrDataPathObj[subFrameIndx]);

                /* Simulate the trigger of chirp avail event for Range Process EDMA */
                errCode = EDMA_startTransfer(dataPathObj->edmaHandle[0],
                                             EDMA_TPCC0_REQ_DFE_CHIRP_AVAIL,
                                             EDMA3_CHANNEL_TYPE_DMA);
                if (errCode != 0)
                {
                    System_printf("Error: EDMA start Transfer returned %d\n",errCode);
                    ObjectDetection_Test_debugAssert(0);
                }
                /* make sure Range DPU finishes before triggering next chirp */
                Task_sleep(1);
            }

            /* Wait for sub-frame to complete: wait for semaphore post from ObjectDetection_Test_dpmTask */
            SemaphoreP_pend (dataPathObj->semaphoreHandle, SemaphoreP_WAIT_FOREVER);

            testStatus = ObjectDetection_Test_printAndCheckResults(&dataPathObj->testSubFrDataPathObj[subFrameIndx], (char*)&featureName);
            if (testStatus != 1)
            {
                allTestsPassed = false;
            }
            System_printf("Test #%d finished!\n\r", testIndx+1);

            /* indicate result consumed and end of frame/sub-frame processing */
            exportInfo.subFrameIdx = dataPathObj->testSubFrDataPathObj[subFrameIndx].result->subFrameIdx;
            errCode = DPM_ioctl (dataPathObj->dpmHandle,
                                 DPC_OBJDET_IOCTL__DYNAMIC_EXECUTE_RESULT_EXPORTED,
                                 &exportInfo,
                                 sizeof (DPC_ObjectDetection_ExecuteResultExportedInfo));
            if (errCode < 0) {
                System_printf ("Error: DPM DPC_OBJDET_IOCTL__DYNAMIC_EXECUTE_RESULT_EXPORTED failed [Error code %d]\n",
                               errCode);
                ObjectDetection_Test_debugAssert (0);
            }
            testIndx++;
        }/* end of subframe loop */

        /* stop DPC */
        ObjectDetection_Test_stopDPC(dataPathObj);

    }/* end of frame loop */

    System_printf("DPC ObjDet Maximum Memory usage: L3: %d Bytes (Test #%d), TCM: %d Bytes (Test #%d)\n",
                  gMaxL3RamUsage,
                  gMaxL3RamUsageTestId,
                  gMaxTCMUsage,
                  gMaxTCMUsageTestId);
    System_printf("All %d Tests finished!\n\r", testIndx);
    if (allTestsPassed)
    {
        System_printf("All tests PASSED\n\r");
    }
    else
    {
        System_printf("Some tests FAILED\n\r");
    }

    /* de init components */
    ObjectDetection_Test_deinitComponents(dataPathObj);

    /* close file */
    if(gReadTestConfigFromBinFile)
    {
        fclose(fileId);
    }

    MCPI_setTestResult ();

    /* Exit BIOS */
    BIOS_exit(0);
}

/**
 *  @b Description
 *  @n
 *      Entry point into the test code.
 *
 *  @retval
 *      Not Applicable.
 */
int main (void)
{
    Task_Params taskParams;
    int32_t     errCode;
    SOC_Cfg     socCfg;

    /* Initialize test logger */
    MCPI_Initialize();

#ifdef SUBSYS_MSS
    /* Initialize the ESM: Dont clear errors as TI RTOS does it */
    ESM_init(0U);
#endif
#if 1
    Cycleprofiler_init();
#endif

    /* Initialize the SOC confiugration: */
    memset ((void *)&socCfg, 0, sizeof(SOC_Cfg));

    /* Populate the SOC configuration: */
    socCfg.clockCfg = SOC_SysClock_INIT;

    /* Initialize the SOC Module: This is done as soon as the application is started
     * to ensure that the MPU is correctly configured. */
    socHandle = SOC_init (&socCfg, &errCode);
    if (socHandle == NULL)
    {
        System_printf ("Error: SOC Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 4*1024;//4*1024;
    taskParams.priority = 1; //Default
    Task_create(ObjectDetection_Test_task, &taskParams, NULL);

    /* Start BIOS */
    BIOS_start();
    
    return 0;
}


