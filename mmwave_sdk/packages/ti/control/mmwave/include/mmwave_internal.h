/**
 *   @file  mmwave_internal.h
 *
 *   @brief
 *      This is the internal Header for the mmWave module. This header file
 *      should *NOT* be directly included by applications.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016 Texas Instruments, Inc.
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

/** @mainpage mmWave API
 *
 *  The mmWave API allow application developers to be abstracted from the lower
 *  layer drivers and the mmWave link API.
 *
 *  The mmWave file should be included in an application as follows:
 *
 *  @code

    #include <ti/control/mmwave/mmwave.h>

    @endcode
 *
 *  ## Initializing the module #
 *
 *  The mmWave module is initialized by using the #MMWave_init API
 *  The module can execute in either of the following domains:
 *  - XWR14xx: The module executes on the MSS
 *  - XWR16xx/XWR18xx/XWR68xx: The module executes on both the MSS and DSS
 *
 *  ## Synchronizing the module #
 *
 *  Once the module has been initialized it has to be synchronized before it
 *  can be used. This is done using the #MMWave_sync API. This is required
 *  because on the XWR16xx/XWR18xx/XWR68xx the module can execute on both the DSS and MSS. We
 *  need to ensure that the modules on each domain are operational before
 *  they can be used.
 *
 *  On the XWR14xx; the synchronization is not really required but in order to
 *  maintain the same API behavior between XWR14xx and XWR16xx/XWR18xx/XWR68xx the API needs to
 *  be invoked.
 *
 *  ## Opening the module #
 *
 *  After the mmWave module has been synchronized; the mmWave module needs to be
 *  opened. This will initialize the mmWave link to the BSS. While operating
 *  in minimal mode applications can invoke the mmWave link API directly after
 *  this step has been initiated.
 *
 *  In cooperative mode; only one of the domains should be deemed responsible
 *  for opening the mmWave module.
 *
 *  ## Configuration of the module #
 *
 *  Applications are responsible for populate and configuring the BSS using
 *  the configuration #MMWave_config API. The API will take the application
 *  supplied configuration and will pass this to the BSS via the mmWave link
 *  API(s). Application developers are abstracted from the calling sequence and
 *  the various synchronization events which are required.
 *
 *  Once the configuration has been completed; the application can setup the
 *  data path. After the data path has been successfully configured application
 *  developers can start the mmWave.
 *
 *  The mmWave module can be configured by multiple domains but the applications
 *  should ensure that the configuration done by a domain be completed before the
 *  other domain initiates the configuration. Failure to do so will result in
 *  unpredictable behavior.
 *
 *  ## Starting the mmWave #
 *
 *  After successful configuration the mmWave needs to be started using the
 *  #MMWave_start API. On successful execution of the API the data path
 *  is being excercised.
 *
 *  ## Executing the mmWave module #
 *
 *  The mmWave module requires an execution context which needs to be provided
 *  by the application. This is because there are asynchronous events and
 *  response messages which are received by the BSS using the mmWave Link
 *  infrastructure. Thes need to be handled and processed in the application
 *  supplied execution context.
 *
 *  Failure to provide and execution context and not invoking the
 *  #MMWave_execute API can result in the mmWave API getting stuck
 *  and the application loosing synchronization with the other domains in the
 *  system.
 *
 *  ## Callback functions #
 *
 *  While working in the cooperative mode the mmWave is executing on both the
 *  MSS and DSS. Each domain registers a callback function which is invoked by
 *  the mmWave module if the peer domain does an equivalent operation.
 *
 *  For example:-
 *  The table below illustrates an example flow of the mmWave API
 *  and the invocation of the callback function in the peer domain
 *
 *   |  MSS                      |           DSS              |
 *   |---------------------------|----------------------------|
 *   | MMWave_open(&openCfg)     | openFxn (ptrOpenCfg)       |
 *   | cfgFxn (ptrControlCfg)    | MMWave_config (&ctrlCfg)   |
 *   | MMWave_start (&startCfg)  | startFxn (ptrStartCfg)     |
 *   | MMWave_stop ()            | stopFxn ()                 |
 *   | MMWave_close ()           | closeFxn ()                |
 *
 *  Callback functions allow an application to be notified when an action
 *  has been taken. Along with this the equivalent parameters are also passed
 *  to the peer domain. This allows both the domains to remain synchronized
 *
 *  ## Error Code #
 *  The mmWave API return an encoded error code. The encoded error code has
 *  the following information:-
 *  - Error or Informational message
 *  - mmWave error code
 *  - Subsystem error code
 *  The mmWave module is a high level control module which is basically layered
 *  over multiple modules like the mmWave Link, Mailbox etc. When an mmWave API
 *  reports a failure it could be because of a multitude of reasons. Also the
 *  mmWave Link API reports certains errors as not fatal but informational. In
 *  order to satisfy these requirements the error code returned by the mmWave API
 *  is encoded. There exists a #MMWave_decodeError which can be used to determine
 *  the exact error code and error level.
 */

#ifndef MMWAVE_INTERNAL_H
#define MMWAVE_INTERNAL_H

#include <ti/common/mmwave_error.h>
#include <ti/drivers/crc/crc.h>
#include <ti/drivers/osal/SemaphoreP.h>
#include <ti/drivers/mailbox/mailbox.h>
#include <ti/control/mmwave/include/mmwave_listlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup MMWAVE_INTERNAL_DEFINITIONS
 @{ */

/**
 * @brief
 *  This defines the maximum number of entries in the spawn list.
 */
#define MMWAVE_MAX_NUM_SPAWN_LIST           4U

/**
 * @brief   Status flag to indicate the open state
 */
#define MMWAVE_STATUS_OPENED                1U

/**
 * @brief   Status flag to indicate the configured state
 */
#define MMWAVE_STATUS_CONFIGURED            2U

/**
 * @brief   Status flag to indicate the synchronization state
 */
#define MMWAVE_STATUS_SYNCHRONIZED          4U

/**
 * @brief   Status flag to indicate the started state
 */
#define MMWAVE_STATUS_STARTED               8U

/**
 * @brief   Link Status flag used to indicate that the RF was initialized successfully
 */
#define MMWAVE_RF_INITIALIZED               1U

/**
 * @brief   Link Status flag used to indicate that the RF initialization failed.
 */
#define MMWAVE_RF_INIT_FAILED               2U

/**
 * @brief   Link Status flag used to indicate that the RF Calibration completed
 */
#define MMWAVE_RF_CALIBRATION_DONE          4U

/**
 * @brief   Link Status flag used to indicate that the Calibration failed
 */
#define MMWAVE_RF_CALIBRATION_FAILED        8U

/**
 * @brief   Link Status flag used to indicate that the ESM fault was detected
 */
#define MMWAVE_RF_ESM_FAULT                 16U

/**
 * @brief   Link Status flag used to indicate that the CPU fault was detected
 */
#define MMWAVE_RF_CPU_FAULT                 32U

/**
 * @brief   Link Status flag used to indicate that the Analog fault was detected
 */
#define MMWAVE_RF_ANALOG_FAULT              64U

/**
@}
*/

/** @addtogroup MMWAVE_INTERNAL_DATA_STRUCTURE
 @{ */

/**
 * @brief
 *  mmWave Spawn function entry
 *
 * @details
 *  The structure is used to keep track of the spawn functions which
 *  need to be executed by the control module.
 */
typedef struct MMWave_SpawnFxnNode_t
{
    /**
     * @brief   Links to the other elements in the list
     */
    MMWave_ListNode         links;

    /**
     * @brief   Spawn function entry
     */
    RL_P_OSI_SPAWN_ENTRY    spawnEntry;

    /**
     * @brief   Argument passed to the spawn function
     */
    const void*             arg;
}MMWave_SpawnFxnNode;

/**
 * @brief
 *  mmWave Chirp configuration
 *
 * @details
 *  The structure is used to keep track of the chirps which
 *  have been configured in the system.
 */
typedef struct MMWave_Chirp_t
{
    /**
     * @brief   Links to the other elements in the list
     */
    MMWave_ListNode             links;

    /**
     * @brief   Chirp configuration
     */
    rlChirpCfg_t                chirpCfg;

    /**
     * @brief   Profile for which the chirp is configured
     */
    struct MMWave_Profile_t*    ptrMMWaveProfile;
}MMWave_Chirp;

/**
 * @brief
 *  mmWave Profile configuration
 *
 * @details
 *  The structure is used to keep track of the profiles
 */
typedef struct MMWave_Profile_t
{
    /**
     * @brief   Links to the other elements in the list
     */
    MMWave_ListNode         links;

    /**
     * @brief   Profile configuration
     */
    rlProfileCfg_t          profileCfg;

    /**
     * @brief   Link to the mmWave
     */
    struct MMWave_MCB_t*    ptrMMWaveMCB;

    /**
     * @brief   Number of chirps
     */
    uint32_t                numChirps;

    /**
     * @brief   List of all the chirps associated with the profile
     */
    MMWave_Chirp*           ptrChirpList;
}MMWave_Profile;

/**
 * @brief
 *  mmWave BPM configuration
 *
 * @details
 *  The structure is used to keep track of the BPM configurations
 */
typedef struct MMWave_BpmChirp_t
{
    /**
     * @brief   Links to the other elements in the list
     */
    MMWave_ListNode         links;

    /**
     * @brief   Profile configuration
     */
    rlBpmChirpCfg_t         bpmChirp;

    /**
     * @brief   Link to the mmWave
     */
    struct MMWave_MCB_t*    ptrMMWaveMCB;
    
}MMWave_BpmChirp;

/**
 * @brief
 *  mmWave Control MCB
 *
 * @details
 *  The structure is used to store all the relevant information required
 *  to execute the mmWave control module.
 */
typedef struct MMWave_MCB_t
{
    /**
     * @brief   Initialization configuration which was used to initialize the
     * control module.
     */
    MMWave_InitCfg              initCfg;

    /**
     * @brief   Configuration which is used to open the control module.
     */
    MMWave_OpenCfg              openCfg;

    /**
     * @brief   Calibration configuration which is used to start the control module
     */
    MMWave_CalibrationCfg       calibrationCfg;

    /**
     * @brief   DFE Data output mode: This is valid only when the mmWave module is in
     * the following states:-
     *  Full Configuration    - Configured State
     *  Minimal Configuration - Start State
     */
    MMWave_DFEDataOutputMode    dfeDataOutputMode;

    /**
     * @brief   Handle to the BSS Mailbox: This is used to communicate the mmWave
     * link messages to the BSS
     */
    Mbox_Handle              bssMailbox;

    /**
     * @brief   CRC Driver channel handle: This is the handle to the CRC driver
     * if configured to use the driver else this is set to NULL.
     */
    CRC_Handle                  crcHandle;

    /**
     * @brief   This is the semaphore handle which is used to handle the messages exchanged
     * between the MSS/DSS and BSS.
     */
    SemaphoreP_Handle           linkSemHandle;

    /**
     * @brief   This is the semaphore handle which is used to protect the chirp/profile
     * configuration while operating in FULL configuration mode.
     */
    SemaphoreP_Handle           cfgSemHandle;

    /**
     * @brief   This is the spawn table used by the free & active lists.
     */
    MMWave_SpawnFxnNode         spawnTable[MMWAVE_MAX_NUM_SPAWN_LIST];

    /**
     * @brief   List which tracks all the nodes in the spawn free list.
     */
    MMWave_SpawnFxnNode*        ptrSpawnFxnFreeList;

    /**
     * @brief   List which tracks all the nodes in the spawn list which are
     * to be executed.
     */
    MMWave_SpawnFxnNode*        ptrSpawnFxnActiveList;

    /**
     * @brief   List which tracks all the profiles which have been created
     */
    MMWave_Profile*             ptrProfileList;
    
    /**
     * @brief   List which tracks all the BPM configurations which have been created
     */
    MMWave_BpmChirp*              ptrBpmChirpList;

    /**
     * @brief   Status of the mmWave Link: This can be changed from multiple execution
     * contexts
     */
    volatile uint32_t           linkStatus;

    /**
     * @brief   Status of the mmWave control module:
     */
    uint32_t                    status;

    /**
     * @brief   Counter which tracks the number of times the spawn list overflowed
     */
    uint32_t                    spawnOverflow;

    /**
     * @brief   Counter which tracks the number of times the spawn was invoked
     */
    uint32_t                    spawnCounter;

    /**
     * @brief   Counter which tracks the number of times the write reset was invoked
     */
    uint32_t                    wResetCounter;

    /**
     * @brief   Version Information which is stored once the link is operational
     */
    rlVersion_t                 version;
}MMWave_MCB;

/**
@}
*/

/* Global mmWave Control MCB: */
extern MMWave_MCB           gMMWave_MCB;

/***************************************************************************************
 * Internal Link Exported API:
 ***************************************************************************************/
extern int32_t MMWave_initLink (MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);
extern int32_t MMWave_deinitLink (MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);
extern int32_t MMWave_openLink (MMWave_MCB* ptrMMWaveMCB, rlCalibrationData_t* ptrCalibData, int32_t* errCode);
extern int32_t MMWave_closeLink (MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);
extern int32_t MMWave_executeLink (MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);
extern int32_t MMWave_configLink (MMWave_MCB* ptrMMWaveMCB, MMWave_CtrlCfg* ptrControlCfg, int32_t* errCode);
extern int32_t MMWave_startLink (MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);
extern int32_t MMWave_stopLink (const MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);

/***************************************************************************************
 * Internal Platform specific Exported API:
 ***************************************************************************************/
extern void    MMWave_deviceGetDeviceInfo (const MMWave_MCB* ptrMMWaveMCB, rlUInt8_t* devType, rlUInt8_t* platform);
extern int32_t MMWave_deviceCfgAsyncEvent(const MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);
extern int32_t MMWave_deviceInitFxn(MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);
extern int32_t MMWave_deviceDeinitFxn(MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);
extern int32_t MMWave_deviceOpenFxn(const MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);
extern int32_t MMWave_deviceCloseFxn(const MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);
extern int32_t MMWave_deviceSyncFxn(const MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);
extern int32_t MMWave_deviceCfgFxn (const MMWave_MCB* ptrMMWaveMCB, MMWave_CtrlCfg* ptrControlCfg, int32_t* errCode);
extern int32_t MMWave_deviceStartFxn(const MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);
extern int32_t MMWave_deviceStopFxn(const MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);
extern int32_t MMWave_deviceExecuteFxn(MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);
extern void    MMWave_deviceEventFxn(const MMWave_MCB* ptrMMWaveMCB, uint16_t msgId, uint16_t sbId, uint16_t sbLen, uint8_t* payload);

/***************************************************************************************
 * Error Management API:
 ***************************************************************************************/
extern int32_t MMWave_encodeError (MMWave_ErrorLevel errorLevel, int32_t mmWaveError, int32_t subSysError);
extern MMWave_ErrorLevel MMWave_decodeErrorLevel (int32_t errCode);

/***************************************************************************************
 * Link OSAL Exported API:
 ***************************************************************************************/
extern rlInt32_t MMWave_osalMutexCreate(rlOsiMutexHdl_t* mutexHandle, rlInt8_t* name);
extern rlInt32_t MMWave_osalMutexLock(rlOsiMutexHdl_t* mutexHandle, rlOsiTime_t timeout);
extern rlInt32_t MMWave_osalMutexUnlock(rlOsiMutexHdl_t* mutexHandle);
extern rlInt32_t MMWave_osalMutexDelete(rlOsiMutexHdl_t* mutexHandle);
extern rlInt32_t MMWave_osalSemCreate(rlOsiSemHdl_t* semHandle, rlInt8_t* name);
extern rlInt32_t MMWave_osalSemWait(rlOsiSemHdl_t* semHandle, rlOsiTime_t timeout);
extern rlInt32_t MMWave_osalSemSignal(rlOsiSemHdl_t* semHandle);
extern rlInt32_t MMWave_osalSemDelete(rlOsiSemHdl_t* semHandle);

#ifdef __cplusplus
}
#endif

#endif /* MMWAVE_INTERNAL_H */

