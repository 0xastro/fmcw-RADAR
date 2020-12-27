/**
 *   @file  cbuff.h
 *
 *   @brief
 *      This is the header file for the CBUFF driver which exposes the
 *      data structures and exported API which can be used by the
 *      applications to use the CBUFF driver.
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

/** @mainpage CBUFF Driver
 *
 *  The CBUFF (Common Buffer Controller) is responsible for the transfer
 *  of data from multiple sources like ADCBUFF, Chirp Parameters (CP),
 *  Chirp Quality (CQ) or any other source to the LVDS Tx or CSI2 Module.
 *
 *  The CBUFF header file should be included in an application as follows:
 *  @code
    #include <ti/drivers/cbuff.h>
    @endcode
 *
 *  ## Customizing the driver #
 *
 *  The default driver library fully supports the following data modes:-
 *  - Interleaved
 *  - Non-Interleaved
 *
 *  Each modes also supports all the data formats. This can cause the default
 *  driver library to become large since all the symbols for all modes & formats
 *  are included. This can be of concern because of the memory constraints on the
 *  system. In most use cases the application developers have already decided upfront
 *  on the data mode & format which is to be used. The driver thus exposes the
 *  following function tables in the corresponding platform file
 *  - @sa gInterleavedTransferFxn
 *  - @sa gNonInterleavedTransferFxn
 *
 *  Application developers are recommended to modify these tables and mark as NULL
 *  the data mode & formats they are not interested in. Rebuilding the driver and now
 *  linking this new library to the application will ensure that the symbols for all
 *  the non used data modes & formats are not linked into the final executable reducing
 *  the total application size.
 *
 *  ## Sessions #
 *
 *  The CBUFF driver introduces a concept called sessions. Sessions are created during
 *  application startup. Sessions will allocate the required EDMA channels and will setup
 *  a virtualized CBUFF linked list to support the data format which is being streamed out.
 *
 *  Sessions can be executed by Hardware or Software triggers. The ADC data being available
 *  is a hardware trigger which will kick start the transfers without any software
 *  intervention. On the other hand software triggered sessions need to be triggered by
 *  the CBUFF Driver API
 *
 *  In order for the sessions to coexist only one of the sessions can be active at a time.
 *  Applications can register a frame done callback function which is invoked once
 *  the transfer associated with the session is complete. This can be used to switch from
 *  one active session to another. It is the responsiblity of the application to guarantee
 *  that the transfers in the sessions do not overlap.
 *
 *  @sa CBUFF_activateSession @sa CBUFF_deactivateSession
 */

/** @defgroup CBUFF_DRIVER      CBUFF Driver
 */
#ifndef CBUFF_DRIVER_H
#define CBUFF_DRIVER_H

#include <ti/common/mmwave_error.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/csi/csi.h>
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/adcbuf/ADCBuf.h>

/**
@defgroup CBUFF_DRIVER_EXTERNAL_FUNCTION            CBUFF Driver External Functions
@ingroup CBUFF_DRIVER
@brief
*   The section has a list of all the exported API which the applications need to
*   invoke in order to use the driver
*/
/**
@defgroup CBUFF_DRIVER_EXTERNAL_DATA_STRUCTURE      CBUFF Driver External Data Structures
@ingroup CBUFF_DRIVER
@brief
*   The section has a list of all the data structures which are exposed to the application
*/
/**
@defgroup CBUFF_DRIVER_EXTERNAL_DEFINITION          CBUFF Driver External Definition
@ingroup CBUFF_DRIVER
@brief
*   The section has a list of all the exported definitions
*/
/**
@defgroup CBUFF_DRIVER_ERROR_CODE                   CBUFF Driver Error Codes
@ingroup CBUFF_DRIVER
@brief
*   The section has a list of all the error codes which are generated by the CBUFF Driver
*   module
*/
/**
@defgroup CBUFF_DRIVER_INTERNAL_FUNCTION            CBUFF Driver Internal Functions
@ingroup CBUFF_DRIVER
@brief
*   The section has a list of all internal API which are not exposed to the external
*   applications.
*/
/**
@defgroup CBUFF_DRIVER_INTERNAL_DATA_STRUCTURE      CBUFF Driver Internal Data Structures
@ingroup CBUFF_DRIVER
@brief
*   The section has a list of all internal data structures which are used internally
*   by the CBUFF module.
*/

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CBUFF_DRIVER_ERROR_CODE
 *  Base error code for the CBUFF module is defined in the
 *  \include ti/common/mmwave_error.h
 @{ */

/**
 * @brief   Error Code: Invalid argument
 */
#define CBUFF_EINVAL                    (MMWAVE_ERRNO_CBUFF_BASE-1)

/**
 * @brief   Error Code: Out of memory
 */
#define CBUFF_ENOMEM                    (MMWAVE_ERRNO_CBUFF_BASE-2)

/**
 * @brief   Error Code: Operation cannot be implemented because the CBUFF driver
 * is in use
 */
#define CBUFF_EINUSE                    (MMWAVE_ERRNO_CBUFF_BASE-3)

/**
 * @brief   Error Code: Operation cannot be implemented because the CBUFF driver
 * is not supporting this currently
 */
#define CBUFF_ENOTSUP                   (MMWAVE_ERRNO_CBUFF_BASE-4)

/**
 * @brief   Error Code: The application failed to provide the EDMA resources
 * which are required by the CBUFF driver to support the application use case
 */
#define CBUFF_EDMA_FAIL                 (MMWAVE_ERRNO_CBUFF_BASE-5)

/**
 * @brief   Error Code: Limit exceeded
 */
#define CBUFF_ELIMIT                    (MMWAVE_ERRNO_CBUFF_BASE-6)

/**
@}
*/

/** @addtogroup CBUFF_DRIVER_EXTERNAL_DEFINITION
 @{ */

/**
 * @brief   Maximum number of user supplied data buffers which can be configured
 * and sent over the interface
 */
#define CBUFF_MAX_USER_BUFFER               3U

/**
 * @brief   Maximum number of sessions which can be supported
 * by the CBUFF Driver.
 *
 * *NOTE*: This is limited to the number of DMA channels available
 * to the CBUFF IP.
 */
#define CBUFF_MAX_NUM_SESSION               7U

/**
@}
*/

/** @addtogroup CBUFF_DRIVER_EXTERNAL_DATA_STRUCTURE
 @{ */

/**
 * @brief
 *  Opaque handle to the CBUFF Driver
 */
typedef void*   CBUFF_Handle;

/**
 * @brief
 *  Opaque handle to the CBUFF Session
 */
typedef void*   CBUFF_SessionHandle;

/**
 * @brief
 *  High Speed Interface
 *
 * @details
 *  The enumeration describes the high speed interface which is to be used
 *  by the CBUFF to send out the data
 */
typedef enum CBUFF_Interface_e
{
    /**
     * @brief   The CBUFF driver will send out the packets using the CSI Interface
     */
    CBUFF_Interface_CSI     = 0x1,

    /**
     * @brief   The CBUFF driver will send out the packets using the LVDS Interface
     */
    CBUFF_Interface_LVDS
}CBUFF_Interface;

/**
 * @brief
 *  CBUFF Command
 *
 * @details
 *  The enumeration describes commands which are used to get/set information
 *  from the CBUFF Driver
 *
 * @sa
 *  CBUFF_control
 */
typedef enum CBUFF_Command_e
{
    /**
     * @brief   The command is used to get the statistics associated with the
     * CBUFF driver. While using this command in the CBUFF_control API;
     * please ensure the following:-
     *  arg    = &CBUFF_Stats
     *  argLen = sizeof(CBUFF_Stats)
     * @sa CBUFF_Stats
     */
    CBUFF_Command_GET_CBUFF_STATS       = 0x1,

    /**
     * @brief   The command is used to clear the statistics associated with the
     * CBUFF driver. While using this command in the CBUFF_control API;
     * please ensure the following:-
     *  arg    = NULL
     *  argLen = 0
     */
    CBUFF_Command_CLEAR_CBUFF_STATS,

    /**
     * @brief   The command is used to get the current active session in the CBUFF
     * Driver. While using this command in the CBUFF_control API;
     * please ensure the following:-
     *  arg    = &CBUFF_SessionHandle
     *  argLen = sizeof(CBUFF_SessionHandle)
     *
     * If session handle is marked as NULL it implies that there was no active session
     * in the CBUFF driver.
     */
    CBUFF_Command_GET_ACTIVE_SESSION
}CBUFF_Command;

/**
 * @brief
 *  Output Data Format
 *
 * @details
 *  The enumeration describes the LVDS/CSI2 output data format.
 */
typedef enum CBUFF_OutputDataFmt_e
{
    /**
     * @brief   12bit output format
     */
    CBUFF_OutputDataFmt_12bit     = 0x0,

    /**
     * @brief   14bit output format
     */
    CBUFF_OutputDataFmt_14bit,

    /**
     * @brief   16bit output format
     */
    CBUFF_OutputDataFmt_16bit
}CBUFF_OutputDataFmt;

/**
 * @brief
 *  Data Format
 *
 * @details
 *  The enumeration describes the data format.
 */
typedef enum CBUFF_DataFmt_e
{
    /**
     * @brief   Only ADC Data is to sent out
     */
    CBUFF_DataFmt_ADC_DATA     = 0x0,

    /**
     * @brief   Chirp Parameters + ADC Data
     */
    CBUFF_DataFmt_CP_ADC,

    /**
     * @brief   ADC Data + Chirp Parameters
     */
    CBUFF_DataFmt_ADC_CP,

    /**
     * @brief   Chirp Parameters + ADC Data + Chirp Quality
     */
    CBUFF_DataFmt_CP_ADC_CQ,

    /**
     * @brief   ADC + User Data
     */
    CBUFF_DataFmt_ADC_USER,

    /**
     * @brief   Chirp Parameters + ADC Data + Chirp Quality + User Data
     */
    CBUFF_DataFmt_CP_ADC_CQ_USER,

    /**
     * @brief   Maximum Data format value.
     */
    CBUFF_DataFmt_MAX
}CBUFF_DataFmt;

/**
 * @brief
 *  Data Type
 *
 * @details
 *  The enumeration describes the data type
 */
typedef enum CBUFF_DataType_e
{
    /**
     * @brief   Real Data
     */
    CBUFF_DataType_REAL         = 0x0,

    /**
     * @brief   Complex Data
     */
    CBUFF_DataType_COMPLEX
}CBUFF_DataType;

/**
 * @brief
 *  CBUFF Operational Mode
 *
 * @details
 *  The enumeration describes the operational mode for the CBUFF
 */
typedef enum CBUFF_OperationalMode_e
{
    /**
     * @brief   The CBUFF sends out the data on the High speed interface
     * based on the number of chirps.
     */
    CBUFF_OperationalMode_CHIRP         = 0x1,

    /**
     * @brief   The CBUFF sends out the data on the High speed interface
     * based on the threshold. This is only supported on LVDS.
     */
    CBUFF_OperationalMode_CONTINUOUS
}CBUFF_OperationalMode;

/**
 * @brief
 *  LVDS Lane Format Map
 *
 * @details
 *  The enumeration describes the LVDS Lane Format maps which are available
 *  and which need to be used.
 */
typedef enum CBUFF_LVDSLaneFmtMap_e
{
    /**
     * @brief   Use the LVDS Lane Format-0
     */
    CBUFF_LVDSLaneFmtMapLANEx_FMT_0_y = 0U,

    /**
     * @brief   Use the LVDS Lane Format-1
     */
    CBUFF_LVDSLaneFmtMapLANEx_FMT_1_y
}CBUFF_LVDSLaneFmtMap;

/**
 * @brief
 *  Data Storage Mode
 *
 * @details
 *  The enumeration describes the storage mode in which the data is stored
 *  in the ADC Buffer.
 */
typedef enum CBUFF_DataMode_e
{
    /**
     * @brief   Data is stored in interleaved mode. This implies that sample of
     * each receive channel is stored one after another.
     */
    CBUFF_DataMode_INTERLEAVED          = 0,

    /**
     * @brief   Data is stored in non-interleaved mode. This implies that all the
     * samples of a receive channel are grouped together.
     */
    CBUFF_DataMode_NON_INTERLEAVED
}CBUFF_DataMode;

/**
 * @brief
 *  Header Mode
 *
 * @details
 *  The enumeration describes the header mode. The CBUFF driver can add headers
 *  to the data before it is sent out over the CSI/LVDS.
 */
typedef enum CBUFF_HeaderMode_e
{
    /**
     * @brief   No header is added
     */
    CBUFF_HeaderMode_NONE = 0x1,

    /**
     * @brief   Custom application specific headers.
     */
    CBUFF_HeaderMode_CUSTOM
}CBUFF_HeaderMode;

/**
 * @brief
 *  Session Execution Mode
 *
 * @details
 *  The enumeration describes the execution mode for the sessions.
 *  Sessions can be automatically triggered by the hardware *or* these
 *  need to be triggered manually by the CBUFF Driver.
 */
typedef enum CBUFF_SessionExecuteMode_e
{
    /**
     * @brief   CBUFF Transfers are triggered by the hardware.
     */
    CBUFF_SessionExecuteMode_HW  = 0x0,

    /**
     * @brief   CBUFF Transfers are triggered by the driver
     */
    CBUFF_SessionExecuteMode_SW
}CBUFF_SessionExecuteMode;

/**
 * @brief
 *  CBUFF LVDS Initialization configuration
 *
 * @details
 *  The structure describes the configuration which is required to configure
 *  the LVDS
 */
typedef struct CBUFF_LVDSCfg_t
{
    /**
     * @brief   Enable/Disable CRC on LVDS
     */
    uint8_t                 crcEnable;

    /**
     * @brief   LVDS Lane configuration: The bit mask here is used to indicate
     * the active LVDS lanes i.e. Bit 0 implies Lane-0, Bit 1 implies Lanel-1 etc
     *
     * The number of LVDS lanes is platform specific and is defined in the CBUFF
     * platform file.
     */
    uint8_t                 lvdsLaneEnable;

    /**
     * @brief   Set the flag to 1 to indicate that the MSB is sent first or LSB
     */
    uint8_t                 msbFirst;

    /**
     * @brief   Set the flag to 1 for DDR Clock Mode and 0 for SDR
     */
    uint8_t                 ddrClockMode;

    /**
     * @brief   Set the flag to 1 for DDR Mode Clock Mux and 0 for SDR Mode Clock Mux
     */
    uint8_t                 ddrClockModeMux;

    /**
     * @brief   LVDS Lane Format:
     */
    CBUFF_LVDSLaneFmtMap    laneFormat;
}CBUFF_LVDSCfg;

/**
 * @brief
 *  CBUFF CSI configuration
 *
 * @details
 *  The structure describes the configuration which is required to use
 *  the CBUFF with the CSI as a high speed interface
 */
typedef struct CBUFF_CSICfg_t
{
    /**
     * @brief   Handle to the CSI Driver:
     */
    CSI_Handle      handle;
}CBUFF_CSICfg;

/**
 * @brief
 *  CBUFF Buffer configuration
 *
 * @details
 *  This is a generic data structure which is exposed to the application
 *  to pass buffer configuration to the drivers. This is used to populate
 *  the User Buffers and Headers which can be streamed out via CBUFF to
 *  the selected High speed interface.
 */
typedef struct CBUFF_BufferCfg_t
{
    /**
     * @brief   Size of the Buffer: This can be set to 0 to indicate
     * that no buffer is specified. The size is specified in CBUFF units
     * i.e. 16 bits.
     */
    uint16_t    size;

    /**
     * @brief   Address of the Buffer:
     * Please be aware that the address should be in a memory range
     * which is accessible by the EDMA.
     */
    uint32_t    address;
}CBUFF_BufferCfg;

/**
 * @brief
 *  CBUFF Statistics
 *
 * @details
 *  The structure describes the CBUFF statistics which can be used
 *  to determine the behavior of the CBUFF module.
 */
typedef struct CBUFF_Stats_t
{
    /**
     * @brief   Number of frame start interrupts received: This is available
     * only if the driver is initialized in the debug mode. Else this is
     * always set to 0.
     *
     * @sa CBUFF_InitCfg_t::enableDebugMode
     */
    uint32_t    numFrameStart;

    /**
     * @brief   Number of frame done interrupts received: This is available
     * only in the following cases:
     *  (a) Enable debug mode
     *  (b) Sessions with the Frame done callback
     * If either of the above cases is not met the value of this is always 0.
     *
     * @sa CBUFF_InitCfg_t::enableDebugMode
     * @sa CBUFF_SessionCfg_t::frameDoneCallbackFxn
     */
    uint32_t    numFrameDone;

    /**
     * @brief   Number of chirp done interrupts received:  This is available
     * only in the following cases:
     *  (a) Enable debug mode
     *  (b) Sessions with the Frame done callback
     * If either of the above cases is not met the value of this is always 0.
     *
     * @sa CBUFF_InitCfg_t::enableDebugMode
     * @sa CBUFF_SessionCfg_t::frameDoneCallbackFxn
     */
    uint32_t    numChirpDone;

    /**
     * @brief   Number of error interrupts received. This is always available
     */
    uint32_t    numErrorInterrupts;

    /**
     * @brief   Flag which indicated if a frame start error was detected.
     * This is always available
     */
    uint8_t     frameStartError;

    /**
     * @brief   Flag which indicated if a chirp error was detected.
     * This is always available
     */
    uint8_t     chirpError;
}CBUFF_Stats;

/**
 * @brief
 *  CBUFF EDMA channel resource configuration
 *
 * @details
 *  The structure describes the EDMA channel resources which are needed by the
 *  CBUFF driver in order to stream out the data over the high speed interface.
 */
typedef struct CBUFF_EDMAChannelCfg_t
{
    /**
     * @brief   EDMA Chain Channels Identifier
     */
    uint8_t     chainChannelsId;

    /**
     * @brief   EDMA Shadow link channels Identifier
     */
    uint16_t    shadowLinkChannelsId;
}CBUFF_EDMAChannelCfg;

/**
 * @brief
 *  CBUFF EDMA Information block
 *
 * @details
 *  The structure describes the EDMA informational block which is passed
 *  to the application. The information here requires to be used by the
 *  application before performing an EDMA channel allocation.
 */
typedef struct CBUFF_EDMAInfo_t
{
    /**
     * @brief   EDMA Instance handle: This is the configuration which was passed
     * by the application. Ensure that the EDMA channels are allocated from the
     * the specified EDMA instance.
     */
    EDMA_Handle     edmaHandle;

    /**
     * @brief   Flag which if set indicates that this is the first EDMA channel which
     * is being allocated. The first EDMA channel allocation is a special case explained
     * below.
     */
    bool            isFirstEDMAChannel;

    /**
     * @brief   DMA Number: There are multiple DMA in the CBUFF driver. There exists
     * a mapping between the CBUFF DMA number and the corresponding hardwired EDMA channel.
     *
     * Please be aware that the CBUFF EDMA transfers are kicked in automatically by the
     * hardware. The XWR1xx has a special CBUFF EDMA Physical channel which has been allocated
     * for this purpose and which should not be used for any other reason. Each DMA Number is
     * associated with a corresponding EDMA channel.
     *
     *  DMA Number  | EDMA Physical Channel(s)
     *  ------------|-----------------------
     *    0         | EDMA_TPCC0_REQ_CBUFF_0, EDMA_TPCC1_REQ_CBUFF_0
     *    1         | EDMA_TPCC0_REQ_CBUFF_1, EDMA_TPCC1_REQ_CBUFF_1
     *    2         | EDMA_TPCC0_REQ_CBUFF_2, EDMA_TPCC1_REQ_CBUFF_2
     *    3         | EDMA_TPCC0_REQ_CBUFF_3, EDMA_TPCC1_REQ_CBUFF_3
     *    4         | EDMA_TPCC0_REQ_CBUFF_4, EDMA_TPCC1_REQ_CBUFF_4
     *    5         | EDMA_TPCC0_REQ_CBUFF_5, EDMA_TPCC1_REQ_CBUFF_5
     *    6         | EDMA_TPCC0_REQ_CBUFF_6, EDMA_TPCC1_REQ_CBUFF_6
     *
     *  The XXX_TPCC0_YYY are for EDMA Instance 0 while the XXX_TPCC1_YYY are for the
     *  EDMA Instance 1.
     *
     *  Please ensure that the *first* EDMA channel for the specified DMA number has to be
     *  from the table above. There is no restriction on subsequent EDMA channel allocations.
     *  This table needs to be enforced if the 'isFirstEDMAChannel' is set to true.
     *
     *  *NOTE*: The CBUFF driver will fail the session creation if the above table is not
     *  enforced.
     */
    uint8_t         dmaNum;
}CBUFF_EDMAInfo;

/**
 *  @b Description
 *  @n
 *      This is the function which is registered with the CBUFF driver and is invoked
 *      by the driver whenever it needs to allocate and use an EDMA channel.
 *
 *  @param[in]  dmaNum
 *      DMA Number to be used for EDMA channel allocation
 *  @param[in]  ptrEDMAInfo
 *      Pointer to the EDMA informational block which needs to be used by
 *      the application to perform the EDMA channel allocation
 *  @param[out] ptrEDMAChannelCfg
 *      Pointer to the EDMA Channel configuration which is to be populated
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0 [Implies that the application is unable to allocate a channel]
 *
 * @b NOTE:
 *  The EDMA transfer completion codes are identical to the CBUFF Physical & chain channel
 *  resources therefore these transfer completion codes are considered to be reserved
 *  for CBUFF Usage.
 */
typedef int32_t (*CBUFF_EDMAChannelAllocateFxn) (CBUFF_EDMAInfo* ptrEDMAInfo, CBUFF_EDMAChannelCfg* ptrEDMAChannelCfg);

/**
 *  @b Description
 *  @n
 *      This is the function which is registered with the CBUFF driver and is invoked
 *      by the driver whenever it needs to free an allocated EDMA channel.
 *
 *  @param[in] ptrEDMAChannelCfg
 *      Pointer to the EDMA chnanel configuration which is to be released
 *
 *  @retval
 *      Not applicable
 */
typedef void (*CBUFF_EDMAChannelFreeFxn) (CBUFF_EDMAChannelCfg* ptrEDMAChannelCfg);

/**
 *  @b Description
 *  @n
 *      Sessions can register for Frame Done interrupts. This callback function
 *      is invoked if the session is currently active and the CBUFF Driver receives
 *      a frame done interrupt. Application should use this to activate another
 *      session. Switching between active sessions while the frame is not done can
 *      lead to unexpected results.
 *
 *  @param[in] sessionHandle
 *      Session Handle which was active for which the Frame done interrupt was
 *      received
 *
 *  @retval
 *      Not applicable
 */
typedef void (*CBUFF_FrameDoneCallbackFxn) (CBUFF_SessionHandle sessionHandle);

/**
 * @brief
 *  CBUFF Hardware Triggered Session configuration
 *
 * @details
 *  The structure describes the configuration required to be specified if
 *  the session is created in hardware triggered mode.
 */
typedef struct CBUFF_HwSessionCfg_t
{
    /**
     * @brief   ADCBUF Driver Handle: Ensure that the ADC Channels are
     * enabled and configured.
     */
    ADCBuf_Handle           adcBufHandle;

    /**
     * @brief   Data Format: This is used to describe the format of the data
     * which is being sent out via the HSI.
     */
    CBUFF_DataFmt           dataFormat;

    /**
     * @brief   ADC Buffer data storage mode: Interleaved or Non-Interleaved
     * This is only used if the data format is configured to send out ADC Data
     */
    CBUFF_DataMode          dataMode;

    /**
     * @brief   Operational mode for the driver:
     */
    CBUFF_OperationalMode   opMode;

    /**
     * @brief This field is described as follows:-
     *
     *  - Chirp Mode     : Number of chirps per frame
     *  - Continuous Mode: This field is ignored
     */
    uint32_t                numChirpsPerFrame;

    /**
     * @brief This field is described as follows:-
     *  - Single Chirp Mode : Set this to 1
     *  - Multi-Chirp  Mode : Set this between 2 to 8
     *  - Continuous Mode   : Set this to 0
     *
     * *NOTE*: Multi-Chirp mode is only supported on the XWR16xx/XWR18xx/XWR68xx.
     */
    uint32_t                chirpMode;

    /**
     * @brief This field is described as follows:-
     *
     *  - Chirp Mode      : This is the number of ADC samples per chirp per channel
     *  - Continuous Mode : This is the number of samples per channel which is configured
     *                      in the ADC Buffer
     *
     * This is only used if the data format is configured to send out ADC data.
     */
    uint16_t                numADCSamples;

    /**
     * @brief   Chirp Quality Size: The size is specified in CBUFF units
     * If the size is set to 0; CQx is ignored.
     */
    uint16_t                cqSize[ADCBufMMWave_CQType_MAX_CQ];

    /**
     * @brief   User supplied data buffers which can be transmitted over the interface
     */
    CBUFF_BufferCfg         userBufferInfo[CBUFF_MAX_USER_BUFFER];
}CBUFF_HwSessionCfg;

/**
 * @brief
 *  CBUFF Software Triggered Session configuration
 *
 * @details
 *  The structure describes the configuration required to be specified if
 *  the session is created in software triggered mode.
 */
typedef struct CBUFF_SwSessionCfg_t
{
    /**
     * @brief   User supplied data buffers which can be transmitted over the interface
     */
    CBUFF_BufferCfg     userBufferInfo[CBUFF_MAX_USER_BUFFER];
}CBUFF_SwSessionCfg;

/**
 * @brief
 *  CBUFF configuration
 *
 * @details
 *  The structure describes the configuration which is required to configure
 *  the CBUFF Driver.
 */
typedef struct CBUFF_SessionCfg_t
{
    /**
     * @brief   Session execution mode: Hardware or Software triggered
     */
    CBUFF_SessionExecuteMode        executionMode;

    /**
     * @brief   This is the callback function which is triggered once the frame has
     * been sent over the HSI. This can be set to NULL if the application only uses
     * a single session. But if multiple sessions are being used application should
     * register this callback function in order to switch from one session to another
     *
     * *NOTE*: Applications are responsible for ensuring that the switch between the
     * sessions is done between the inter-frame boundaries. Failure to enforce this
     * will result in unpredictable behavior.
     */
    CBUFF_FrameDoneCallbackFxn      frameDoneCallbackFxn;

    /**
     * @brief   EDMA Instance Handle: The session will allocate EDMA channels
     */
    EDMA_Handle                     edmaHandle;

    /**
     * @brief   Application provided EDMA Channel allocation function.
     */
    CBUFF_EDMAChannelAllocateFxn    allocateEDMAChannelFxn;

    /**
     * @brief   Application provided EDMA Channel free function.
     */
    CBUFF_EDMAChannelFreeFxn        freeEDMAChannelFxn;

    /**
     * @brief   Type of Data: Real or Complex which is going to be streamed out
     * via the CBUFF High speed interface
     */
    CBUFF_DataType                  dataType;

    /**
     * @brief   This is the header which needs to be added to the stream. If the
     * size in the header is set to 0 then no header will be appended.
     */
    CBUFF_BufferCfg                 header;

    union
    {
        /**
         * @brief   Configuration used if the session is executing in hardware
         * trigerred mode
         */
        CBUFF_HwSessionCfg      hwCfg;

        /**
         * @brief   Configuration used if the session is executing in software
         * trigerred mode
         */
        CBUFF_SwSessionCfg      swCfg;
    }u;
}CBUFF_SessionCfg;

/**
 * @brief
 *  CBUFF Initialization Configuration
 *
 * @details
 *  The structure describes the configuration which is required to initialize
 *  the CBUFF Driver.
 */
typedef struct CBUFF_InitCfg_t
{
    /**
     * @brief   SOC Handle
     */
    SOC_Handle                      socHandle;

    /**
     * @brief   This is used to specify the LVDS/CSI2 output format.
     */
    CBUFF_OutputDataFmt             outputDataFmt;

    /**
     * @brief   Enable/Disable the ECC in the CBUFF module
     */
    uint8_t                         enableECC;

    /**
     * @brief   In order to ensure data integrity of data transfer from ADC buffer to CBUFF
     * there is a CRC computed and this checked for integrity in the hardware at both the
     * source (ADC Buffer) and destination (CBUFF)
     */
    uint8_t                         crcEnable;

    /**
     * @brief   This is the maximum number of sessions which can be supported
     * for the CBUFF Instance. This value needs to be >= 1 and < CBUFF_MAX_NUM_SESSION.
     * Any other value will result in an invalid argument error.
     */
    uint8_t                         maxSessions;

    /**
     * @brief   This is a flag which if set to true will register the ISR to track
     * Frame Start/Done and Chirp done.
     *
     * *NOTE*: Enable the debug mode will allow the statistics to increment and can
     * be useful to ensure that the streaming is working. But this can overwhelm the
     * system with a large number of interrupts.
     *
     * Certain interrupt such as the Frame Done interrupts are required to be registered
     * if multiple sessions are created. Switching from one session to another can
     * only be done with the intra-frame boundary.
     */
    bool                            enableDebugMode;

    /**
     * @brief   The interface over which the CBUFF module will send out the data
     */
    CBUFF_Interface                 interface;

    /**
     * @brief   Interface specific configuration:
     */
    union
    {
        /**
         * @brief   LVDS Initializaton configuration: This needs to be specified
         * if the interface is configured to be LVDS.
         */
        CBUFF_LVDSCfg       lvdsCfg;

        /**
         * @brief   CSI Initializaton configuration: This needs to be specified
         * if the interface is configured to be CSI.
         */
        CBUFF_CSICfg        csiCfg;
    }u;
}CBUFF_InitCfg;

/**
@}
*/

/*******************************************************************************************************
 * CBUFF Exported API:
 *******************************************************************************************************/
extern CBUFF_Handle CBUFF_init (CBUFF_InitCfg* ptrInitCfg, int32_t* errCode);
extern int32_t CBUFF_deinit (CBUFF_Handle cBuffHandle, int32_t* errCode);
extern int32_t CBUFF_control(CBUFF_Handle cBuffHandle, CBUFF_Command cmd, void* arg, uint32_t argLen, int32_t* errCode);

/* Session Management API: */
extern CBUFF_SessionHandle CBUFF_createSession (CBUFF_Handle cbuffHandle, CBUFF_SessionCfg* ptrSessionCfg, int32_t* errCode);
extern int32_t CBUFF_activateSession (CBUFF_SessionHandle sessionHandle, int32_t* errCode);
extern int32_t CBUFF_deactivateSession (CBUFF_SessionHandle sessionHandle, int32_t* errCode);
extern int32_t CBUFF_deleteSession (CBUFF_SessionHandle sessionHandle, int32_t* errCode);

#ifdef __cplusplus
}
#endif

#endif /* CBUFF_DRIVER_H */

