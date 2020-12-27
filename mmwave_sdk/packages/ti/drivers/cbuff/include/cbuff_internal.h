/**
 *   @file  cbuff_internal.h
 *
 *   @brief
 *      This is the internal header file used by the CBUFF Driver. The
 *      file is NOT exposed to the application developers and should
 *      not be directly included.
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
#ifndef CBUFF_DRIVER_INTERNAL_H
#define CBUFF_DRIVER_INTERNAL_H

#include <ti/drivers/cbuff/cbuff.h>

#ifdef SOC_XWR14XX
#include <ti/drivers/csi/include/reg_csi2protocolengine.h>
#endif

#include <ti/drivers/cbuff/include/reg_cbuff.h>
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/osal/HwiP.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CBUFF_DRIVER_INTERNAL_DATA_STRUCTURE
 @{ */

/**
 * @brief   Maximum number of EDMA channels which can be used by the
 * CBUFF driver.
 */
#define CBUFF_EDMA_MAX_NUM_CHANNELS                 32U

/**
 * @brief   Maximum number of linked list entries which can be tracked
 * by the driver.
 *
 * *NOTE*: This is limited to the number Linked list entries available
 * to the CBUFF IP.
 */
#define CBUFF_MAX_LINKED_LIST_SIZE                  32U

/**
 * @brief   Read threshold configured in the Linked List Entries. This is
 * used by the CBUFF 128bit FIFO
 *
 * *NOTE*: This is the recommended value as per the Application Note
 */
#define CBUFF_LL_READ_THRESHOLD                     0x4U

/**
 * @brief   Write threshold configured in the Linked List Entries. This is
 * used by the CBUFF 128bit FIFO
 *
 * *NOTE*: This is the recommended value as per the Application Note
 */
#define CBUFF_LL_WRITE_THRESHOLD                    0x40U

/**
 * @brief   Maximum transfer size in CBUFF Units
 *
 * *NOTE*: This is the maximum size of the transfer as per the CBUFF IP
 */
#define CBUFF_MAX_TRANSFER_SIZE_CBUFF_UNITS         0x3FFFU

/**
 * @brief   Minimum transfer size in CBUFF Units
 *
 * *NOTE*: This is the minimum frame size which can be transferred by the
 * CBUFF IP. The Frame size is defined to the sum of the transfer size of
 * all the linked list entries
 *
 * The CBUFF FIFO Width for Read Threshold of 4 will imply that 4*128bits
 * i.e. 64 bytes or 32CBUFF Units
 */
#define CBUFF_MIN_TRANSFER_SIZE_CBUFF_UNITS         ((CBUFF_LL_READ_THRESHOLD * 16U) / 2U)

/**
 * @brief
 *  CBUFF Linked List Type
 *
 * @details
 *  Each linked list in the CBUFF holds information about one of
 *  the following types
 */
typedef enum CBUFF_LLType_e
{
    /**
     * @brief   CBUFF Linked List Entry is not being used
     */
    CBUFF_LLType_UNUSED = 0x0,

    /**
     * @brief   CBUFF Linked List Entry holds ADC Data
     */
    CBUFF_LLType_ADCDATA,

    /**
     * @brief   CBUFF Linked List Entry holds Chirp Quality (CQ)
     */
    CBUFF_LLType_CQ,

    /**
     * @brief   CBUFF Linked List Entry holds Chirp Parameters (CQ)
     */
    CBUFF_LLType_CP,

    /**
     * @brief   CBUFF Linked List Entry holds a user buffer
     */
    CBUFF_LLType_USER,

    /**
     * @brief   CBUFF Linked List Entry holds a header
     */
    CBUFF_LLType_HEADER
}CBUFF_LLType;

/**
 * @brief
 *  CBUFF Linked List Tracking Entry
 *
 * @details
 *  The structure holds information about the Linked List entries which
 *  have been added to the CBUFF Linked List Entry.
 */
typedef struct CBUFF_LinkListEntry_t
{
    /**
     * @brief   Type of entry
     */
    CBUFF_LLType        type;

    /**
     * @brief   Size of the data associated with the Linked List entry.
     * This is stored in CBUFF Units.
     */
    uint32_t            transferSize;
}CBUFF_LinkListEntry;

/**
 * @brief
 *  CBUFF EDMA Tracking Entry
 *
 * @details
 *  The structure holds information about the EDMA channel which has been
 *  added to correspond to the linked list entry to DMA the data into the
 *  CBUFF.
 */
typedef struct CBUFF_EDMATrackingEntry_t
{
    /**
     * @brief   EDMA Channel Configuration allocated by the application
     * and which is used to program the CBUFF Transfers
     */
    CBUFF_EDMAChannelCfg    cbuffEDMAChannelCfg;

    /**
     * @brief   Source address of the EDMA Transfer
     */
    uint32_t                srcAddress;

    /**
     * @brief   Size of the data being transferred by the EDMA. This is stored
     * in CBUFF Units.
     */
    uint32_t                transferSize;
}CBUFF_EDMATrackingEntry;

/**
 * @brief
 *  CBUFF Linked List parameters
 *
 * @details
 *  The structure contains the parameters which are used to program and setup
 *  the CBUFF linked list entries.
 */
typedef struct CBUFF_LinkListParams_t
{
    /**
     * @brief   Type of data being stored in the linked list entry
     */
    CBUFF_LLType    type;

    /**
     * @brief   CRC Enable: Only ADC Buffers have CRC and these need to be
     * verified
     */
    uint8_t         crcEnable;

    /**
     * @brief   Long packet header enable: This is to be 1 for the first
     * entry in the group.
     */
    uint8_t         lpHdrEn;

    /**
     * @brief   Output data format:
     *  00 - 16bit
     *  01 - 14bit
     *  10 - 12bit
     */
    uint8_t         outputDataFmt;

    /**
     * @brief   Transfer size
     */
    uint16_t        transferSize;

    /**
     * @brief   Cumulative Transfer Size for all linked entries.
     * This is applicable only if the HSI is CSI
     */
    uint16_t        totalTransferSize;

    /**
     * @brief   96 byte transfer mode
     */
    uint8_t         align96;

    /**
     * @brief   Data Format mapping
     */
    uint8_t         dataFmtMap;

    /**
     * @brief   Virtual channel
     */
    uint8_t         vcNum;

    /**
     * @brief   Horizontal sync start
     */
    uint8_t         hsyncStart;

    /**
     * @brief   Horizontal sync end
     */
    uint8_t         hsyncEnd;

    /**
     * @brief   Long Packet Header Value
     */
    uint32_t        lpHeaderValue;

    /**
     * @brief   Thresholds to be configured
     */
    uint32_t        threshold;
}CBUFF_LinkListParams;

/**
 * @brief
 *  CBUFF Hardware Attributes
 *
 * @details
 *  The structure contains the hardware atrributes which are used
 *  to specify the platform specific configurations.
 */
typedef struct CBUFF_Hw_Attrib_t
{
    /**
     * @brief   Base address of the CBUFF module.
     */
    uint32_t        baseAddress;

    /**
     * @brief   CBUFF FIFO Base address: This is the EDMA3/DSS base address.
     */
    uint32_t        fifoBaseAddress;

    /**
     * @brief   CBUFF EDMA Physical Channel Identifier:
     */
    uint8_t         cbuffChannelId[EDMA_NUM_CC][CBUFF_MAX_NUM_SESSION];

    /**
     * @brief   Base address of the ADC Buffer: This should be the
     * address as seen by the EDMA3/DSS.
     */
    uint32_t        adcBaseAddress;

    /**
     * @brief   Single Chirp Interleaved Base address of the Chirp Parameter:
     * This should be the address as seen by the EDMA3/DSS.
     */
    uint32_t        cpSingleChirpInterleavedAddress[4];

    /**
     * @brief   Single Chirp Non-Interleaved Base address of the Chirp Parameter:
     * This should be the address as seen by the EDMA3/DSS.
     */
    uint32_t        cpSingleChirpNonInterleavedAddress[4];

    /**
     * @brief   Multiple Chirp Non-Interleaved Base address of the Chirp Parameter:
     * This should be the address as seen by the EDMA3/DSS.
     */
    uint32_t        cpMultipleChirpNonInterleavedAddress[8];

    /**
     * @brief   Base address of the CSI Protocol Engine register space.
     */
    uint32_t        csiProtocolEngineAddress;

    /**
     * @brief   Maximum number of LVDS Lanes which are supported
     */
    uint8_t         maxLVDSLanesSupported;

    /**
     * @brief   CBUFF Error Interrupt Number
     */
    uint32_t        errorInterrupt;

    /**
     * @brief   CBUFF Interrupt Number
     */
    uint32_t        cbuffInterrupt;

    /**
     * @brief   CBUFF Frame Start Interrupt
     */
    uint32_t        frameStartInterrupt;

    /**
     * @brief   CBUFF Chirp Mode Start Index: This is the range of chirp
     * modes which are supported by the platform
     */
    uint32_t        chirpModeStartIndex;

    /**
     * @brief   CBUFF Chirp Mode End Index: This is the range of chirp
     * modes which are supported by the platform
     */
    uint32_t        chirpModeEndIndex;
}CBUFF_Hw_Attrib;

/**
 * @brief
 *  CBUFF Session
 *
 * @details
 *  The structure is used to track the relevant configuration and run time
 *  information for each session in the CBUFF driver
 */
typedef struct CBUFF_Session_t
{
    /**
     * @brief   Status flag which indicates if the session is active or not.
     */
    bool                            isValid;

    /**
     * @brief   This is the back pointer to the CBUFF Driver MCB
     */
    struct CBUFF_DriverMCB_t*       ptrDriverMCB;

    /**
     * @brief   Configuration used to create the session.
     */
    CBUFF_SessionCfg                sessionCfg;

    /**
     * @brief   DMA number associated with the session.
     */
    uint8_t                         dmaNum;

    /**
     * @brief   Number of active ADC Channels
     */
    uint8_t                         numActiveADCChannels;

    /**
     * @brief   Number of ADC Sample per chirp * [Complex(2) *or* Real(1)]
     */
    uint16_t                        adcTransferSize;

    /**
     * @brief   ADC Receive Channel Address: This is computed from the ADCBUF
     * driver if the session is operating in HW Triggered mode.
     */
    uint32_t                        rxChannelAddress[SYS_COMMON_NUM_RX_CHANNEL];

    /**
     * @brief   Chirp Quality Buffer Information:
     *  While sending CQ it is possible to only send CQ1 or CQ2 or both. In
     *  order to handle this the following array is used:-
     *
     * Only CQ1:
     *  cqBuffer[0] = CQ1;
     *
     * Only CQ2:
     *  cqBuffer[0] = CQ2;
     *
     * Both
     *  cqBuffer[0] = CQ1;
     *  cqBuffer[1] = CQ2;
     */
    CBUFF_BufferCfg                 cqBuffer[ADCBufMMWave_CQType_MAX_CQ];

    /**
     * @brief   Number of CQ Detected: This works in conjuction with the
     * CQ Buffer described above.
     */
    uint8_t                         numCQ;

    /**
     * @brief   Chirp Parameter Transfer Size: Sum total of the size of all
     * valid chirps
     */
    uint16_t                        cpTotalTransferSize;

    /**
     * @brief   Link Index which tracks the linked list entries being added
     */
    uint8_t                         linkListIndex;

    /**
     * @brief   Linked List Tracker used to keep track of the CBUFF Linked List
     * entries which have been added
     */
    CBUFF_LinkListParams            linkedListTracker[CBUFF_MAX_LINKED_LIST_SIZE];

    /**
     * @brief   Counter which tracks the EDMA channels
     */
    uint8_t                         edmaChannelCount;

    /**
     * @brief   Tracks all the EDMA entries which have been added
     */
    CBUFF_EDMATrackingEntry         edmaTrackingEntry[CBUFF_EDMA_MAX_NUM_CHANNELS];

    /**
     * @brief This is the number of chirps per frame.
     * - Hardware Triggered Execution Mode
     *   This is derived from the Session H/W configuration
     *      - Chirp Mode     : Number of chirps per frame
     *      - Continuous Mode: This field is ignored
     * - Software Triggered Execution Mode
     *   This is always set to 1.
     */
    uint32_t                        numChirpsPerFrame;

    /**
     * @brief   Number of frame done interrupts received
     */
    uint32_t                        numFrameDone;

    /**
     * @brief   Number of chirp done interrupts received
     */
    uint32_t                        numChirpDone;
}CBUFF_Session;

/**
 * @brief
 *  CBUFF Driver
 *
 * @details
 *  The structure is used to track the relevant configuration and run time
 *  information for the CBUFF Driver.
 */
typedef struct CBUFF_DriverMCB_t
{
    /**
     * @brief   Initialization configuration used to setup the driver.
     * This is valid once the driver has been INITIALIZED
     */
    CBUFF_InitCfg                   initCfg;

    /**
     * @brief   CBUFF register space
     */
    DSSCBUFFRegs*                   ptrCBUFFReg;

#ifdef SOC_XWR14XX
    /**
     * @brief   CSI Protocol register space:
     */
    CSI2PROTOCOLENGINERegs*         ptrCSIProtReg;
#endif

    /**
     * @brief   High speed interface function table to be used by the CBUFF Driver
     */
    struct CBUFF_InterfaceFxn_t*    interfaceFxn;

    /**
     * @brief   Registered interrupt handler for the CBUFF Module.
     */
    HwiP_Handle                     hwiISRHandle;

    /**
     * @brief   Registered interrupt handler for the CBUFF Module.
     */
    HwiP_Handle                     hwiErrorISRHandle;

    /**
     * @brief   SOC Listener Handle for the Frame Start System Interrupt
     */
    SOC_SysIntListenerHandle        frameStartListener;

    /**
     * @brief   This is the table which tracks all the sessions which can be created by
     * the CBUFF Driver.
     */
    CBUFF_Session*                  ptrSessionTable;

    /**
     * @brief   This is the pointer to the currently active session. This can be NULL
     * to indicate that no session is currently active.
     */
    CBUFF_Session*                  ptrCurrentActiveSession;

    /**
     * @brief   Number of active LVDS lanes configured
     */
    uint8_t                         numActiveLVDSLanes;

    /**
     * @brief   Total number of frame start interrupts received
     */
    uint32_t                        totalNumFrameStart;

    /**
     * @brief   Total number of frame done interrupts received
     */
    uint32_t                        totalNumFrameDone;

    /**
     * @brief   Total number of chirp done interrupts received
     */
    uint32_t                        totalNumChirpDone;

    /**
     * @brief   Number of interrupts received when there is no active session
     */
    uint32_t                        totalNumNonActiveSessionInterrupts;

    /**
     * @brief   Total number of error interrupts received
     */
    uint32_t                        totalNumErrorInterrupts;
}CBUFF_DriverMCB;

/**
 * @brief
 *  This is the function invoked by the CBUFF driver to initialize the high
 *  speed interface.
 *
 *  @param[in]  ptrDriverMCB
 *      Pointer to the CBUFF Driver MCB
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
typedef int32_t (*CBUFF_initFxn) (CBUFF_DriverMCB* ptrDriverMCB, int32_t* errCode);

/**
 * @brief
 *  This is the function invoked by the CBUFF driver to deinitialize and shutdown
 *  the high speed interface
 *
 *  @param[in]  ptrDriverMCB
 *      Pointer to the CBUFF Driver MCB
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
typedef int32_t (*CBUFF_deinitFxn) (CBUFF_DriverMCB* ptrDriverMCB, int32_t* errCode);

/**
 * @brief
 *  This is the function invoked by the CBUFF driver to open the high
 *  speed interface for the specific session
 *
 *  @param[in]  ptrSession
 *      Pointer to the session
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
typedef int32_t (*CBUFF_openFxn) (CBUFF_Session* ptrSession, int32_t* errCode);

/**
 * @brief
 *  This is the function invoked by the CBUFF driver to close the high
 *  speed interface for the specific session
 *
 *  @param[in]  ptrSession
 *      Pointer to the session
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
typedef int32_t (*CBUFF_closeFxn) (CBUFF_Session* ptrSession, int32_t* errCode);

/**
 * @brief
 *  This is the function prototype invoked by the CBUFF Driver to initialize the
 *  linked list parameters for each high speed interface
 *
 *  @param[in]  ptrSession
 *      Pointer to the CBUFF Session
 *  @param[out] ptrLinkListParams
 *      Pointer to the linked list configuration populated by the API
 *
 *  @retval
 *      Not applicable
 */
typedef void (*CBUFF_initLinkListParamsFxn) (CBUFF_Session* ptrSession, CBUFF_LinkListParams* ptrLinkListParams);

/**
 * @brief
 *  This is the function prototype invoked by the CBUFF Driver to finalize the linked
 *  list parameters for the high speed interface. Once the parameters have been finalized
 *  they can be written to the CBUFF IP.
 *
 *  @param[in]  ptrSession
 *      Pointer to the CBUFF Session
 *  @param[in] ptrLinkListParams
 *      Pointer to the linked list configuration
 *
 *  @retval
 *      Not applicable
 */
typedef void (*CBUFF_finalizeLinkListParamsFxn) (CBUFF_Session* ptrSession, CBUFF_LinkListParams* ptrLinkListParams);

/**
 * @brief
 *  CBUFF Interface Function
 *
 * @details
 *  This is the call table which encapsulates the CBUFF Driver from the high
 *  speed interface.
 */
typedef struct CBUFF_InterfaceFxn_t
{
    /**
     * @brief   High speed interface for which the interface functions are defined
     */
    CBUFF_Interface                 interface;

    /**
     * @brief   This is the function which is used to initialize the high speed
     * interface
     */
    CBUFF_initFxn                   initFxn;

    /**
     * @brief   This is the function which is used to deinitialize and shutdown
     * the high speed interface
     */
    CBUFF_deinitFxn                 deinitFxn;

    /**
     * @brief   This is the function which is used to open and setup the high
     * speed interface
     */
    CBUFF_openFxn                   openFxn;

    /**
     * @brief   This is the function which is used to close the high speed interface
     */
    CBUFF_closeFxn                  closeFxn;

    /**
     * @brief   This is the function which is used to initialize the linked list
     * parameters to the default values for the high speed interface
     */
    CBUFF_initLinkListParamsFxn     initLinkListParamsFxn;

    /**
     * @brief   This is the function which is used to finalize and setup the linked
     * list parameters after which they can be wriiten to the CBUFF IP.
     */
    CBUFF_finalizeLinkListParamsFxn finalizeLinkListParamsFxn;
}CBUFF_InterfaceFxn;

/**
 * @brief
 *  This is the function prototype invoked by the CBUFF Driver to setup the
 *  transfers in the CBUFF IP. This will handle the configuration of the
 *  linked list as well as the EDMA Transfers
 *
 *  @param[in]  ptrDriverMCB
 *      Pointer to the CBUFF Driver
 *  @param[in]  ptrSession
 *      Pointer to the CBUFF Session
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
typedef int32_t (*CBUFF_setupTransferFxn) (CBUFF_DriverMCB* ptrDriverMCB, CBUFF_Session* ptrSession, int32_t* errCode);

/***************************************************************************************
 * Internal Exported Defintions:
 ***************************************************************************************/
extern CBUFF_InterfaceFxn       gCBUFFInterfaceFxn[];
extern CBUFF_Hw_Attrib          gCBUFFHwAttribute;
extern CBUFF_setupTransferFxn   gInterleavedTransferFxn[];
extern CBUFF_setupTransferFxn   gNonInterleavedTransferFxn[];
extern CBUFF_setupTransferFxn   gMultipleChirpNonInterleavedTransferFxn[];

/***************************************************************************************
 * Internal Exported API:
 ***************************************************************************************/

/* LVDS High Speed Interface Exported API: */
extern int32_t CBUFF_initLVDS(CBUFF_DriverMCB* ptrDriverMCB, int32_t* errCode);
extern int32_t CBUFF_deinitLVDS(CBUFF_DriverMCB* ptrDriverMCB, int32_t* errCode);
extern int32_t CBUFF_openLVDS(CBUFF_Session* ptrSession, int32_t* errCode);
extern int32_t CBUFF_closeLVDS(CBUFF_Session* ptrSession, int32_t* errCode);
extern void    CBUFF_initLVDSLinkListParams(CBUFF_Session* ptrSession, CBUFF_LinkListParams* ptrLinkListParams);
extern void    CBUFF_finalizeLVDSLinkListParams(CBUFF_Session* ptrSession, CBUFF_LinkListParams* ptrLinkListParams);

/* CSI High Speed Interface Exported API: */
extern int32_t CBUFF_initCSI(CBUFF_DriverMCB* ptrDriverMCB, int32_t* errCode);
extern int32_t CBUFF_deinitCSI(CBUFF_DriverMCB* ptrDriverMCB, int32_t* errCode);
extern int32_t CBUFF_openCSI(CBUFF_Session* ptrSession, int32_t* errCode);
extern int32_t CBUFF_closeCSI(CBUFF_Session* ptrSession, int32_t* errCode);
extern void    CBUFF_initCSILinkListParams(CBUFF_Session* ptrSession, CBUFF_LinkListParams* ptrLinkListParams);
extern void    CBUFF_finalizeCSILinkListParams(CBUFF_Session* ptrSession, CBUFF_LinkListParams* ptrLinkListParams);

/* CBUFF Core Exported API: */
extern void CBUFF_setVC (DSSCBUFFRegs* ptrCBUFFReg, uint8_t vc, uint8_t vcStart, uint8_t vcEnd);

/* CBUFF Transfer Exported API: */
extern int32_t CBUFF_setupTransfer (CBUFF_Session* ptrSession, int32_t* errCode);
extern int32_t CBUFF_shutdownTransfer (CBUFF_Session* ptrSession, int32_t* errCode);

/* EDMA3 Exported API: */
extern int32_t CBUFF_configEDMA(CBUFF_Session* ptrSession, uint32_t srcAddress, uint32_t transferSize, bool isLast);
extern void CBUFF_closeEDMA(CBUFF_Session* ptrSession);

/*********************************************************************************
 * CBUFF Interleaved Transfer API for different data formats:
 *********************************************************************************/
extern int32_t CBUFF_setupInterleaved_ADC
(
    CBUFF_DriverMCB*    ptrDriverMCB,
    CBUFF_Session*      ptrSession,
    int32_t*            errCode
);
extern int32_t CBUFF_setupInterleaved_CP_ADC
(
    CBUFF_DriverMCB*    ptrDriverMCB,
    CBUFF_Session*      ptrSession,
    int32_t*            errCode
);
extern int32_t CBUFF_setupInterleaved_ADC_CP
(
    CBUFF_DriverMCB*    ptrDriverMCB,
    CBUFF_Session*      ptrSession,
    int32_t*            errCode
);
extern int32_t CBUFF_setupInterleaved_CP_ADC_CQ
(
    CBUFF_DriverMCB*    ptrDriverMCB,
    CBUFF_Session*      ptrSession,
    int32_t*            errCode
);
extern int32_t CBUFF_setupInterleaved_ADC_USER
(
    CBUFF_DriverMCB*    ptrDriverMCB,
    CBUFF_Session*      ptrSession,
    int32_t*            errCode
);
extern int32_t CBUFF_setupInterleaved_CP_ADC_CQ_USER
(
    CBUFF_DriverMCB*    ptrDriverMCB,
    CBUFF_Session*      ptrSession,
    int32_t*            errCode
);

/*********************************************************************************
 * CBUFF Non-Interleaved Transfer API for different data formats:
 *********************************************************************************/
extern int32_t CBUFF_setupNonInterleaved_ADC
(
    CBUFF_DriverMCB*    ptrDriverMCB,
    CBUFF_Session*      ptrSession,
    int32_t*            errCode
);
extern int32_t CBUFF_setupNonInterleaved_CP_ADC
(
    CBUFF_DriverMCB*    ptrDriverMCB,
    CBUFF_Session*      ptrSession,
    int32_t*            errCode
);
extern int32_t CBUFF_setupNonInterleaved_ADC_CP
(
    CBUFF_DriverMCB*    ptrDriverMCB,
    CBUFF_Session*      ptrSession,
    int32_t*            errCode
);
extern int32_t CBUFF_setupNonInterleaved_CP_ADC_CQ
(
    CBUFF_DriverMCB*    ptrDriverMCB,
    CBUFF_Session*      ptrSession,
    int32_t*            errCode
);
extern int32_t CBUFF_setupNonInterleaved_ADC_USER
(
    CBUFF_DriverMCB*    ptrDriverMCB,
    CBUFF_Session*      ptrSession,
    int32_t*            errCode
);
extern int32_t CBUFF_setupNonInterleaved_CP_ADC_CQ_USER
(
    CBUFF_DriverMCB*    ptrDriverMCB,
    CBUFF_Session*      ptrSession,
    int32_t*            errCode
);

/*********************************************************************************
 * CBUFF Non-Interleaved Multi Chirp Transfer API for different data formats:
 *********************************************************************************/
extern int32_t CBUFF_setupNonInterleaved_MultiChirp_CP_ADC
(
    CBUFF_DriverMCB*    ptrDriverMCB,
    CBUFF_Session*      ptrSession,
    int32_t*            errCode
);
extern int32_t CBUFF_setupNonInterleaved_MultiChirp_ADC_CP
(
    CBUFF_DriverMCB*    ptrDriverMCB,
    CBUFF_Session*      ptrSession,
    int32_t*            errCode
);
extern int32_t CBUFF_setupNonInterleaved_MultiChirp_CP_ADC_CQ
(
    CBUFF_DriverMCB*    ptrDriverMCB,
    CBUFF_Session*      ptrSession,
    int32_t*            errCode
);
extern int32_t CBUFF_setupNonInterleaved_MultiChirp_CP_ADC_CQ_USER
(
    CBUFF_DriverMCB*    ptrDriverMCB,
    CBUFF_Session*      ptrSession,
    int32_t*            errCode
);

/* CBUFF Transfer API for user: This is agnostic of interleaved or non-interleaved mode */
extern int32_t CBUFF_setup_USER (CBUFF_DriverMCB* ptrDriverMCB, CBUFF_Session* ptrSession, int32_t* errCode);

/**
@}
*/

#ifdef __cplusplus
}
#endif

#endif /* CBUFF_DRIVER_INTERNAL_H */

