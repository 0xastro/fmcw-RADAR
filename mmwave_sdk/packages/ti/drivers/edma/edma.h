/** @file       edma.h
 *
 *  @brief      EDMA external driver interface.
 *
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/** @mainpage EDMA (Enhanced DMA) Driver
 *
 *  @section intro_sec Introduction
 *
 *  The EDMA IP in the ARxxxx SoCs can be programmed at a high-level using
 *  the EDMA driver software that allows hook-up with an operating system.
 *  The EDMA driver exposes programming of most features provided by the IP.
 *  Although the EDMA's user guide for ARxxxx SoCs is not available at present
 *  on the TI public web, because this IP is used in many other older TI parts,
 *  it can be referred to for example at the following url:
 *  http://www.ti.com/lit/ug/sprugs5b/sprugs5b.pdf
 *
 *  Following is a high level description of the APIs/features:
 *
 *  - Query number of EDMA instances on the SoC.
 *  - Initialize a particular instance of EDMA to a known clean disabled state (a reset like state).
 *  - Open an instance of EDMA, which returns instance information such as number
 *    of transfer controllers on that instance.
 *  - Configure a channel to desired configuration and optionally enable channel.
 *    This includes PaRAM set choice and PaRAM set configuration.
 *    User can optionally supply call-back function for channel transfer completion indication.
 *  - Enable/disable a channel.
 *  - Configure a PaRAM set. Typically useful for linked PaRAMs.
 *  - Link PaRAM sets.
 *  - Chain channels.
 *  - Start transfer (manually-triggered) of a previously configured channel.
 *  - Wait until transfer is complete (used in the case when call-back in configuration is not provided).
 *  - APIs to set source or destination address of a PaRAM set, intended for some use
 *    cases on XWR16XX (DSP).
 *  - Error Monitoring configuration for error generation from CC (Channel Controller)
 *    and all TCs (Transfer Controllers). Corresponding error status query for processors
 *    on some devices in which error interrupts are not physically connected to the
 *    processor interrupt map (e.g EDMA instance #1 on xWR16xx's R4F (MSS)).
 *  - Status query.
 *  - Performance control.
 *  - Close an instance of EDMA.
 *
 * Here a channel can be a DMA or QDMA channel.
 *
 *  @section limit Limitations
 *     @subsection hwlimit Unsupported Hardware features.
 *       Following features of the EDMA hardware are presently NOT supported:
 *       - Programming of privilege level and privilege ID is not provided,
 *         these are assumed to be always highest privilege (default settings).
 *       - Querying of source active registers and destination FIFO register in
 *         EDMA TC (Transfer Controller) for advanced debugging not supported.
 *
 *     @subsection swlimit Unsupported Software features.
 *     - No resource management of any sort is provided e.g no DMA or QDMA channel resource management.
 *       User is required to do static or dynamic resource management outside of the driver.
 *       On XWR16xx/XWR18xx/XWR68xx, the driver software does not prevent both (R4F and C674) processors from 
 *       operating the same physical EDMA instance, software instances are not shared
 *       among the processors (software instances are in local memories of processors). 
 *       However, if both processors wish to operate on the same physical instance, 
 *       then they must coordinate their 
 *       activities outside the driver (say using mailbox messages) to ensure 
 *       only one is operating on the same physical instance at a time 
 *       (because read-modify-write operations would become inconsistent 
 *       without multi-processor semaphores) and also coordinate resource 
 *       management for resources like channels and transfer completion codes.
 *
 *  @section api API
 *
 *  The EDMA API header file should be included in an application as follows:
 *  @code
 *  #include <ti/drivers/edma/edma.h>
 *  @endcode
 *
 *  The documentation for all APIs can be referred by clicking at above file or
 *  by going to @ref EDMA_DRIVER_EXTERNAL_FUNCTION. Example usage of the EDMA
 *  driver can be seen by referring to the EDMA unit test code located at
 *  @verbatim ti/drivers/edma/test/ti_rtos/main.c @endverbatim
 *
 *  @section event EDMA Event Mapping Definitions.
 *
 *  Event mapping definitions for the 64 DMA channels can be referred to in
 *  the files: @verbatim ti/common/sys_common_*.h @endverbatim with names:
 *  @verbatim *_REQ_* @endverbatim
 *  For example (on XWR14xx): @verbatim #define EDMA_TPCC0_REQ_HWACC_0     (17U) @endverbatim
 *  indicates that for instance 0 (CC0) of the EDMA, event number 17 (0-based)
 *  is tied to Hardware Accelerator's channel 0 in the SoC. During programming the channelId
 *  parameter when calling configuration API, these defines can be used depending
 *  on what is desired e.g the above example define when used means that when the
 *  Hardware Accelerator channel 0 output line fires, it will trigger the EDMA channel 17
 *  that was configured using this define. Note the
 *  unit test code may not illustrate this because it picks particular numbers to
 *  test certain boundary conditions.
 *
 *  Channels that are not tied to any hardware event are available for
 *  general usage (e.g for chaining purposes),
 *  these channels are marked with the suffix: @verbatim FREE_<n> @endverbatim
 *  For example: @verbatim EDMA_TPCC0_REQ_FREE_0 @endverbatim
 *
 *  @section caution Caution
 *
 *  It may be important to disable channels after transfers are done. One case is:
 *  A QDMA channel with param X was programmed and transfer completed with a trigger word.
 *  Then the same param X that is associated with a DMA channel is used for the DMA channel
 *  configuration because it is free.
 *  When configure API is issued, it will only deal with DMA channel registers
 *  as the channel type is DMA and will leave the QDMA channel untouched (in this case enabled).
 *  When the param is configured for
 *  this DMA channel, it will prematurely trigger the QDMA channel when the trigger
 *  word that was left armed is attempted to be configured. Disabling the QDMA
 *  channel before the param associated with it is reused will prevent the problem.
 *
 *  @section mem_footprint Memory Footprint
 *
 *  The memory footprint of the driver depends on how many APIs are referred to
 *  by an application (all are not necessary for basic operation). The unit test
 *  code's generated memory map file may be used to get an estimate of
 *  the memory foot-print when most APIs are used.
 *
 */

#ifndef ti_drivers_EDMA__include
#define ti_drivers_EDMA__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <ti/common/mmwave_error.h>
#include <ti/drivers/edma/include/edma_low_level.h>

/* common among AR devices, don't waste memory to store these */
/*! @brief Total number of DMA channels */
#define EDMA_NUM_DMA_CHANNELS (64U)

/*! @brief Total number of transfer completion codes */
#define EDMA_NUM_TCC ((uint8_t)64)

/*! @brief Total number of QDMA channels */
#define EDMA_NUM_QDMA_CHANNELS ((uint8_t)8)

/* constants for EDMA IP in any device (not just mmwave ones) */
/*! @brief Maximum number of event queues in any EDMA IP */
#define EDMA_MAX_NUM_EVENT_QUEUES ((uint8_t)8)

/*! @brief Maximum number of transfer controllers in any EDMA IP */
#define EDMA_MAX_NUM_TRANSFER_CONTROLLERS (EDMA_MAX_NUM_EVENT_QUEUES)

/*! @brief Number of entries in each queue in any EDMA IP */
#define EDMA_NUM_QUEUE_ENTRIES ((uint8_t)16)

/******************************************************************************/
/************************ QDMA Trigger Word Offset defines*********************/
/******************************************************************************/
/** @defgroup EDMA_TRIG_WORDS QDMA Trigger Word Offset Definitions
 *
 * @brief
 *  Base error code for the EDMA module is defined in the
 *  \include ti/common/mmwave_error.h
 *
 @{ */

/*! @brief
 * Set the OPT field (Offset Address 0h Bytes)
 * as the QDMA trigger word
 */
#define EDMA_QDMA_TRIG_WORD_OFFSET_OPT            (0U)

/*! @brief
 * Set the SRC field (Offset Address 4h Bytes)
 * as the QDMA trigger word
 */
#define EDMA_QDMA_TRIG_WORD_OFFSET_SRC            (1U)

/*! @brief
 * Set the (ACNT + BCNT) field (Offset Address 8h Bytes)
 * as the QDMA trigger word
 */
#define EDMA_QDMA_TRIG_WORD_OFFSET_ACNT_BCNT      (2U)

/*! @brief
 * Set the DST field (Offset Address Ch Bytes)
 * as the QDMA trigger word
 */
#define EDMA_QDMA_TRIG_WORD_OFFSET_DST            (3U)

/*! @brief
 * Set the (SRCBIDX + DSTBIDX) field (Offset Address 10h Bytes)
 * as the QDMA trigger word
 */
#define EDMA_QDMA_TRIG_WORD_OFFSET_SRC_DST_BIDX   (4U)

/*! @brief
 * Set the (LINK + BCNTRLD) field (Offset Address 14h Bytes)
 * as the QDMA trigger word
 */
#define EDMA_QDMA_TRIG_WORD_OFFSET_LINK_BCNTRLD   (5U)

/*! @brief
 * Set the (SRCCIDX + DSTCIDX) field (Offset Address 18h Bytes)
 * as the QDMA trigger word
 */
#define EDMA_QDMA_TRIG_WORD_OFFSET_SRC_DST_CIDX   (6U)

/*! @brief
 * Set the (CCNT + RSVD) field (Offset Address 1Ch Bytes)
 * as the QDMA trigger word
 */
#define EDMA_QDMA_TRIG_WORD_OFFSET_CCNT           (7U)

/*! @brief Default Trigger Word is CCNT */
#define EDMA_QDMA_TRIG_WORD_OFFSET_DEFAULT        (EDMA_QDMA_TRIG_WORD_OFFSET_CCNT)

/** @}*/ /* end defgroup EDMA_TRIG_WORDS */
/******************************************************************************/

/*! @brief NULL Link Address */
#define EDMA_NULL_LINK_ADDRESS 0xFFFFU

/******************************************************************************/
/************************ Error code defines **********************************/
/******************************************************************************/
/** @defgroup EDMA_ERROR_CODES Error Codes
 *
 * @brief
 *  Base error code for the EDMA module is defined in the
 *  \include ti/common/mmwave_error.h
 *
 @{ */
/*****EDMA_E_INVALID__ : Invalid API parameters or parameter combinations *****/

/*! @brief    Handle argument is NULL pointer. */
#define EDMA_E_INVALID__HANDLE_NULL                   (MMWAVE_ERRNO_EDMA_BASE-0)

/*! @brief    Config argument is NULL pointer. */
#define EDMA_E_INVALID__CONFIG_POINTER_NULL           (MMWAVE_ERRNO_EDMA_BASE-1)

/*! @brief    Config's channelType is invalid. */
#define EDMA_E_INVALID__DMA_CHANNEL_TYPE              (MMWAVE_ERRNO_EDMA_BASE-2)

/*! @brief    Config's channelId out of range. */
#define EDMA_E_INVALID__DMA_CHANNEL_ID                (MMWAVE_ERRNO_EDMA_BASE-3)

/*! @brief    Config's paramId out of range. */
#define EDMA_E_INVALID__PARAM_ID                      (MMWAVE_ERRNO_EDMA_BASE-4)

/*! @brief    Config's eventQueueId out of range */
#define EDMA_E_INVALID__EVENT_QUEUE_ID                (MMWAVE_ERRNO_EDMA_BASE-5)

/*! @brief Config's both interrupt completion flags are false but
    transfer completion call back function is configured (non NULL). */
#define EDMA_E_INVALID__TRNSFR_COMPLETION_PARAMS  (MMWAVE_ERRNO_EDMA_BASE-6)

/*! @brief    Config's transferCompletionCode is invalid. */
#define EDMA_E_INVALID__TRANSFER_COMPLETION_CODE      (MMWAVE_ERRNO_EDMA_BASE-7)

/*! @brief    Config's transferType is invalid. */
#define EDMA_E_INVALID__TRANSFER_TYPE                 (MMWAVE_ERRNO_EDMA_BASE-8)

/*! @brief    Invalid Qdma channelId. */
#define EDMA_E_INVALID__QDMA_CHANNEL_ID               (MMWAVE_ERRNO_EDMA_BASE-9)

/*! @brief    Invalid Instance Id during open. */
#define EDMA_E_INVALID__INSTANCE_ID                   (MMWAVE_ERRNO_EDMA_BASE-10)

/*! @brief    Invalid QDMA trigger word. */
#define EDMA_E_INVALID__QDMA_TRIGGER_WORD             (MMWAVE_ERRNO_EDMA_BASE-11)

/*! @brief    Invalid transfer controller Id. */
#define EDMA_E_INVALID__TRANSFER_CONTROLLER_ID        (MMWAVE_ERRNO_EDMA_BASE-12)

/*! @brief    Invalid transfer controller read rate */
#define EDMA_E_INVALID__TRANSFER_CONTROLLER_READ_RATE (MMWAVE_ERRNO_EDMA_BASE-13)

/*! @brief    Invalid queue priority in performance config. */
#define EDMA_E_INVALID__QUEUE_PRIORITY               (MMWAVE_ERRNO_EDMA_BASE-14)

/*! @brief    During @ref EDMA_startTransfer,
              unexpected QDMA miss event detected. */
#define EDMA_E_INVALID__EVENT_QUEUE_THRESHOLD        (MMWAVE_ERRNO_EDMA_BASE-15)

/*! @brief  During @ref EDMA_configChannel, param ID must be same
 *          as channel ID for DMA channel because channel mapping does not exist
 *          in the hardware. */
#define EDMA_E_INVALID__PARAM_ID_NOT_SAME_AS_CHANNEL_ID (MMWAVE_ERRNO_EDMA_BASE-16)

/*! @brief    Status argument is NULL pointer. */
#define EDMA_E_INVALID__STATUS_POINTER_NULL           (MMWAVE_ERRNO_EDMA_BASE-17)

/*! @brief    InstanceInfo argument is NULL pointer. */
#define EDMA_E_INVALID__INSTANCEINFO_POINTER_NULL    (MMWAVE_ERRNO_EDMA_BASE-18)

/************** EDMA_E_UNEXPECTED__ : Unexpected conditions *******************/
/*! @brief    During @ref EDMA_startTransfer, unexpected DMA miss event detected.
 */
#define EDMA_E_UNEXPECTED__DMA_EVENT_MISS_DETECTED    (MMWAVE_ERRNO_EDMA_BASE-25)

/*! @brief    During @ref EDMA_startTransfer, unexpected QDMA miss event detected.
 */
#define EDMA_E_UNEXPECTED__QDMA_EVENT_MISS_DETECTED  (MMWAVE_ERRNO_EDMA_BASE-26)

/*! @brief    Attempt to reopen and already opened instance. */
#define EDMA_E_UNEXPECTED__EDMA_INSTANCE_REOPEN       (MMWAVE_ERRNO_EDMA_BASE-27)

/*! @brief    Attempt to issue @ref EDMA_isTransferComplete despite non-NULL completion
        call-back function. */
#define EDMA_E_UNEXPECTED__ATTEMPT_TO_TEST_COMPLETION (MMWAVE_ERRNO_EDMA_BASE-28)

/************** EDMA_E_OSAL__ : OSAL API related error conditions *************/
/*! @brief HwiP_create returned NULL on transfer completion Isr creation. */
#define EDMA_E_OSAL__HWIP_CREATE_TRANSFER_COMPLETION_ISR_RETURNED_NULL (MMWAVE_ERRNO_EDMA_BASE-40)

/*! @brief HwiP_create returned NULL on CC error Isr creation. */
#define EDMA_E_OSAL__HWIP_CREATE_ERROR_ISR_RETURNED_NULL (MMWAVE_ERRNO_EDMA_BASE-41)

/*! @brief HwiP_create returned NULL on one of transfer controller error Isrs creation. */
#define EDMA_E_OSAL__HWIP_CREATE_TRANSFER_CONTROLLER_ERROR_ISRS_RETURNED_NULL (MMWAVE_ERRNO_EDMA_BASE-42)

/*! @brief HwiP_delete returned NULL on transfer completion Isr deletion. */
#define EDMA_E_OSAL__HWIP_DELETE_TRANSFER_COMPLETION_ISR_ERROR (MMWAVE_ERRNO_EDMA_BASE-43)

/*! @brief HwiP_delete returned NULL on CC error Isr deletion. */
#define EDMA_E_OSAL__HWIP_DELETE_ERROR_ISR_ERROR (MMWAVE_ERRNO_EDMA_BASE-44)

/*! @brief HwiP_delete returned NULL on one of transfer controller error Isrs deletion. */
#define EDMA_E_OSAL__HWIP_DELETE_TRANSFER_CONTROLLER_ERROR_ISRS_ERROR (MMWAVE_ERRNO_EDMA_BASE-45)

/*! @brief    Operation cannot be implemented because a previous operation
              is still not complete. */
#define EDMA_E_INUSE                 (MMWAVE_ERRNO_EDMA_BASE-60)

/*! @brief    Operation is not implemented. */
#define EDMA_E_NOTIMPL               (MMWAVE_ERRNO_EDMA_BASE-80)

/** @}*/ /* end defgroup EDMA_ERROR_CODES */

/*! @brief    No Error. */
#define EDMA_NO_ERROR ((int32_t)0)

/******************************************************************************/
/************************ Event queue threshold defines************************/
/******************************************************************************/
/** @defgroup EDMA_EVENT_QUEUE_THRESHOLD Event Queue Threshold Configuration Defines
 *
 * @brief
 *  Special defines for event queue threshold configuration.
 *
 @{ */
/*! @brief Disable thresholding */
#define EDMA_EVENT_QUEUE_THRESHOLDING_DISABLED (0x11U)

/*! @brief Queue ever used i.e queue had non-zero occupancy ever */
#define EDMA_EVENT_QUEUE_THRESHOLD_QUEUE_EVER_USED (0U)

/*! @brief Queue ever full i.e was there ever a time when queue was full */
#define EDMA_EVENT_QUEUE_THRESHOLD_QUEUE_EVER_FULL (0x10U)

/*! @brief Max queue threshold, same as queue ever full */
#define EDMA_EVENT_QUEUE_THRESHOLD_MAX     (EDMA_EVENT_QUEUE_THRESHOLD_QUEUE_EVER_FULL)

/** @}*/ /* end defgroup EDMA_EVENT_QUEUE_THRESHOLD */

/******************************************************************************/
/************************ Read rate defines************************************/
/******************************************************************************/
/** @defgroup EDMA_READ_RATE_DEFINES Read Rate Definitions
 *
 * @brief
 *  Read rate defines for performance configuration.
 *
 @{ */

/*! @brief as fast as possible */
#define EDMA_READ_RATE_AS_FAST_AS_POSSIBLE  (EDMA_TC_RDRATE_RDRATE_AFAP)

/*! @brief 4 cycles between reads */
#define EDMA_READ_RATE_EVERY_4_CYCLES       (EDMA_TC_RDRATE_RDRATE_READEVERY4CYCLES)

/*! @brief 8 cycles between reads */
#define EDMA_READ_RATE_EVERY_8_CYCLES       (EDMA_TC_RDRATE_RDRATE_READEVERY8CYCLES)

/*! @brief 16 cycles between reads */
#define EDMA_READ_RATE_EVERY_16_CYCLES      (EDMA_TC_RDRATE_RDRATE_READEVERY16CYCLES)

/*! @brief 32 cycles between reads */
#define EDMA_READ_RATE_EVERY_32_CYCLES      (EDMA_TC_RDRATE_RDRATE_READEVERY32CYCLES)
/** @}*/ /* end defgroup EDMA_READ_RATE_DEFINES */

/*!
 *  @brief      A handle that is returned from a @ref EDMA_open call.
 */
typedef void  *EDMA_Handle;

/*!
*  @brief      Transfer completion call back function definition.
*  @param[out]  Argument that was registered during call back registeration during
*               configuration.
*      Handle to the EDMA instance obtained through call to @ref EDMA_open.
*  @param[out] transferCompletionCode
*      Transfer completion code of the completed transfer.
*/
typedef void (*EDMA_transferCompletionCallbackFxn_t)  (uintptr_t arg,
    uint8_t transferCompletionCode);

/*!
 *  @brief  Error Information structure. Information is from EMR(H), QEMR, CCERR
 *     registers as per EDMA UG.
 */
typedef struct EDMA_errorInfo_t_
{
    /*! @brief Each element of the array indicates whether a DMA channel event is missed
     *    for the corresponding DMA channel index. */
    bool isDmaChannelEventMiss[EDMA_NUM_DMA_CHANNELS];

    /*! @brief Each element of the array indicates whether a QDMA channel event is missed
     *    for the corresponding QDMA channel index. */
    bool isQdmaChannelEventMiss[EDMA_NUM_QDMA_CHANNELS];

    /*! @brief CCERR::TCERR in EDMA UG */
    bool isOutstandingTransferCompletionTransfersExceededLimit;

    /*! @brief CCERR::QTHRXCDx bits in EDMA UG */
    bool isEventQueueThresholdExceeded[EDMA_MAX_NUM_EVENT_QUEUES];

    /*! @brief Number of event Queues of the opened CC provided as convenience
        these many elements will be relevent in the @ref isEventQueueThresholdExceeded
        array */
    uint8_t numEventQueues;
} EDMA_errorInfo_t;

/*!
*  @brief  Error call back function.
*  @param[out]  handle
*      Handle to the EDMA instance obtained through call to @ref EDMA_open.
*  @param[out] errorInfo
*      Pointer to error information @ref EDMA_errorInfo_t_.
*/
typedef void (*EDMA_errorCallbackFxn_t)  (EDMA_Handle handle, EDMA_errorInfo_t *errorInfo);

/*!
 *  @brief Transfer controller bus error information, relevant when
 *   @ref EDMA_transferControllerErrorInfo_t_::isBusError is true.
 */
typedef struct EDMA_transferControllerBusErrorInfo_t_
{
    /*! @brief ERRDET::STAT in EDMA UG. */
    uint8_t errorCode;

    /*! @brief ERRDET::TCC in EDMA UG. */
    uint8_t transferCompletionCode;

    /*! @brief ERRDET::TCCHEN in EDMA UG. */
    bool isFinalChainingEnabled;

    /*! @brief ERRDET::TCINTEN in EDMA UG. */
    bool isFinalTransferInterruptEnabled;
} EDMA_transferControllerBusErrorInfo_t;

/*!
 *  @brief Transfer controller error information.
 */
typedef struct EDMA_transferControllerErrorInfo_t_
{
    /*! @brief ERRSTAT::TRERR in EDMA UG. */
    bool isTransferRequestError;

    /*! @brief ERRSTAT::MMRAERR in EDMA UG. */
    bool isWriteToReservedConfigMemoryMap;

    /*!
     *  @brief true if bus error, see @ref busErrorInfo for more details.
     */
    bool isBusError;

    /*!
     *  @brief Bus Error Information, relevant if @ref isBusError is true.
     */
    EDMA_transferControllerBusErrorInfo_t busErrorInfo;

    /*! @brief Transfer controller Id of the errring transfer controller, can be
       between 0 and (number of transfer controllers on the opened CC - 1). */
    uint8_t transferControllerId;
} EDMA_transferControllerErrorInfo_t;

/*!
*  @brief  Transfer controller error call back function. Usually
*          transfer controller errors are fatal.
*  @param[out]  handle
*      Handle to the EDMA instance obtained through call to @ref EDMA_open.
*  @param[out] errorInfo
*      Pointer to error information @ref EDMA_transferControllerErrorInfo_t_.
*/
typedef void (*edmaTransferControllerErrorCallbackFxn_t)  (EDMA_Handle handle,
                EDMA_transferControllerErrorInfo_t *errorInfo);

/*!
 *  @brief    PaRAM Set configuration.
 */
typedef struct EDMA_paramSetConfig_t_
{
    /**
     * \brief Starting byte address of source.
     */
    uint32_t sourceAddress;
    /**
     * \brief Starting byte address of destination.
     */
    uint32_t destinationAddress;

    /**
     * \brief Number of bytes in each Array (ACNT).
     */
    uint16_t aCount;

    /**
     * \brief Number of Arrays in each Frame (BCNT).
     */
    uint16_t bCount;

    /**
     * \brief Number of Frames in a block (CCNT).
     */
    uint16_t cCount;

    /**
     * \brief Reload value of the numArrInFrame (BCNT)
     * Relevant only for A-sync transfers.
     */
    uint16_t bCountReload;

    /**
     * \brief Index between consec. arrays of a Source Frame (SRCBIDX).
     */
    int16_t  sourceBindex;

    /**
     * \brief Index between consec. arrays of a Destination Frame (DSTBIDX).
     */
    int16_t  destinationBindex;

    /**
     * \brief Index between consecutive frames of a Source Block (SRCCIDX).
     */
    int16_t  sourceCindex;

    /**
     * \brief Index between consecutive frames of a Dest Block (DSTCIDX).
     */
    int16_t  destinationCindex;

    /**
     * \brief Address for linking (AutoReloading of a PaRAM Set)
     * This must point to a valid aligned 32-byte PaRAM set
     * A value of 0xFFFF means (@ref EDMA_NULL_LINK_ADDRESS) no linking.
     */
    uint16_t linkAddress;

    /**
     * \brief transfer type which can be one of those defined in
     *   @ref EDMA_TRANSFER_TYPE_DEFS.
     */
    uint8_t transferType;

    /**
     * \brief transfer Completion Code, must be in the range [0,63].
     */
    uint8_t transferCompletionCode;

    /**
     * \brief Source addressing mode, see @ref EDMA_ADDRESSING_MODE_DEFS.
     */
    uint8_t sourceAddressingMode;

    /**
     * \brief Destination addressing mode, see @ref EDMA_ADDRESSING_MODE_DEFS.
     */
    uint8_t destinationAddressingMode;

    /**
     * \brief FIFO width, see @ref EDMA_FIFO_WIDTH_DEFS.
     */
    uint8_t fifoWidth;

    /**
     * \brief STATIC bit of OPT as described in the EDMA UG.
     */
    bool isStaticSet;

    /**
     * \brief This is bit TCCMODE of OPT as defined in EDMA UG.
     */
    bool isEarlyCompletion;

    /**
     * \brief This is bit TCINTEN of OPT as defined in EDMA UG.
     */
    bool isFinalTransferInterruptEnabled;

    /**
     * \brief This is bit ITCINTEN of OPT as defined in EDMA UG.
     */
    bool isIntermediateTransferInterruptEnabled;

    /**
     * \brief This is bit TCCHEN of OPT as defined in EDMA UG.
     */
    bool isFinalChainingEnabled;

    /**
     * \brief This is bit ITCCHEN of OPT as defined in EDMA UG.
     */
    bool isIntermediateChainingEnabled;
} EDMA_paramSetConfig_t;

/*!
 *  @brief Channel configuration. Specifies the full configuration for a DMA or
 *    QDMA channel with associated paramId and param Set configuration.
 */
typedef struct EDMA_channelConfig_t_
{
    /**
     * \brief channelType can be either @ref EDMA3_CHANNEL_TYPE_DMA or
     *  @ref EDMA3_CHANNEL_TYPE_QDMA
     */
    uint8_t channelType;

    /**
     * \brief qdmaParamTriggerWordOffset represents the offset (0-7)
     *   of the PaRAM that is used
     *   when @ref channelType is @ref EDMA3_CHANNEL_TYPE_QDMA,
     *   use one of @ref EDMA_TRIG_WORDS for
     *   convenience of specifying which parameter in the Param is to be used
     *   as the trigger word.
     */
    uint8_t qdmaParamTriggerWordOffset;

    /**
     * \brief
     *   0 to (@ref EDMA_NUM_DMA_CHANNELS - 1) when channelType is @ref EDMA3_CHANNEL_TYPE_DMA. \n
     *   0 to (@ref EDMA_NUM_QDMA_CHANNELS - 1) when channelType is @ref EDMA3_CHANNEL_TYPE_QDMA.
     */
    uint8_t channelId;

    /**
     * \brief Id of the PaRAM set, valid range is from 0 to (N - 1),
     *   where N is the number of  param sets in the EDMA's CC that is opened,
     *   must be identical to @ref channelId for
     *   DMA channel (@ref channelType is @ref EDMA3_CHANNEL_TYPE_DMA) if channel mapping does
     *   not exist in the hardware.
     */
    uint16_t paramId;

    /**
     * \brief Id of the event queue in which the channel transfer request is submitted,
     *   valid range is from 0 to (Q - 1),
     *   where Q is the number of event queues in the EDMA's CC that is opened.
     */
    uint8_t eventQueueId;

    /**
     * \brief configuration of the PaRAM set
     */
    EDMA_paramSetConfig_t paramSetConfig;

    /**
     * \brief call back function for intermediate or final transfer completion
     *   depending on configuration of interrupt related flags in @ref paramSetConfig.
     *   If NULL, it is assumed that the intent of the user is to poll for the
     *   completion of the transfer using @ref EDMA_isTransferComplete.
     */
    EDMA_transferCompletionCallbackFxn_t transferCompletionCallbackFxn;

    /**
     * \brief argument to be returned by the call back function @ref transferCompletionCallbackFxn.
     */
    uintptr_t transferCompletionCallbackFxnArg;
} EDMA_channelConfig_t;

/*!
 *  @brief Param configuration typically used for stand-alone param sets
 *    (param sets not associated with any of the 64 DMA channels) such
 *    as for QDMA channels or for linking purposes.
 */
typedef struct EDMA_paramConfig_t_
{
    /**
     * \brief configuration of the PaRAM set.
     */
    EDMA_paramSetConfig_t paramSetConfig;

    /**
     * \brief call back function for intermediate or final transfer completion
     *   depending on configuration of interrupt related flags in @ref paramSetConfig
     *   if NULL, it is assumed that the intent of the user is to poll for the
     *   completion of the transfer using @ref EDMA_isTransferComplete.
     */
    EDMA_transferCompletionCallbackFxn_t transferCompletionCallbackFxn;

    /**
     * \brief argument to be returned by the call back function @ref transferCompletionCallbackFxn.
     */
    uintptr_t transferCompletionCallbackFxnArg;
} EDMA_paramConfig_t;

/*!
 *  @brief Error configuration for a transfer controller, allows enabling/disabling
 *    of various error conditions.
 */
typedef struct EDMA_transferControllerErrorConfig_t_ {
    /*! @brief ERREN::BUSERR in EDMA UG. */
    bool isBusErrorEnabled;

    /*! @brief ERREN::TRERR in EDMA UG. */
    bool isTransferRequestErrorEnabled;

    /*! @brief ERREN:MMRAERR in EDMA UG. */
    bool isWriteToReservedConfigMemoryMapEnabled;
} EDMA_transferControllerErrorConfig_t;

/*!
 *  @brief Error configuration for transfer controller, allows enabling/disabling
 *    of various error conditions.
 */
typedef struct EDMA_errorConfig_t_
{
    /*! @brief Set to true if want to configure all event queues. */
    bool isConfigAllEventQueues;

    /*! @brief if @ref isConfigAllEventQueues is false, used to specify the
        event queue to be configured. */
    uint8_t eventQueueId;

    /*! @brief Set to true if want to enable event queue thresholding. */
    bool isEventQueueThresholdingEnabled;

    /*! @brief event Queue threshold, should be in the range
     *    [0, @ref EDMA_EVENT_QUEUE_THRESHOLD_MAX], convenient defines in
     *    @ref EDMA_EVENT_QUEUE_THRESHOLD
     */
    uint8_t eventQueueThreshold;

    /*! @brief Set to true if want to configure all transfer controllers. */
    bool isConfigAllTransferControllers;

    /*! @brief If @ref isConfigAllTransferControllers is false, used to
          specify the transfer controler Id to be configured. */
    uint8_t transferControllerId;

    /*! @brief Set to true if want to enable all transfer controller errors,
     *    this is provided for convenience so as not to have to specific each
     *    error in @ref transferControllerErrorConfig. */
    uint8_t isEnableAllTransferControllerErrors;

    /*! @brief If @ref isEnableAllTransferControllerErrors is false, used to
     *    specify which errors to enable. */
    EDMA_transferControllerErrorConfig_t transferControllerErrorConfig;

    /*! @brief Call back function associated with CC errors. */
    EDMA_errorCallbackFxn_t callbackFxn;

    /*! @brief Call back function associated with queue/TC (Transfer Controller)
     *    errors. */
    edmaTransferControllerErrorCallbackFxn_t transferControllerCallbackFxn;
} EDMA_errorConfig_t;

/*!
 *  @brief Queue entry information.
 */
typedef struct EDMA_queueEntryInfo_t_
{
    /*! @brief QxEy::ENUM in EDMA UG. */
    uint8_t eventNumber;

    /*! @brief QxEy::ETYPE in EDMA UG. */
    uint8_t eventType;
} EDMA_queueEntryInfo_t;

/*!
 *  @brief Queue status information.
 */
typedef struct EDMA_queueStatusInfo_t_
{
    /*! @brief Outstanding entries are entries in the queue at the moment of
        status query that have not yet been serviced. The number
        of relevant entries in this array is @ref numOutstandingEntries. */
    EDMA_queueEntryInfo_t outstandingEntries[EDMA_NUM_QUEUE_ENTRIES];

    /*! @brief Dequeued entries are entries in the queue at the moment of
        querying the queue status that have been serviced, the number of
        relevant entries in this array is @ref EDMA_NUM_QUEUE_ENTRIES
        - @ref numOutstandingEntries. */
    EDMA_queueEntryInfo_t dequeuedEntries[EDMA_NUM_QUEUE_ENTRIES];

    /*! @brief status whether queue threshold is exceeded. */
    bool isThresholdExceeded;

    /*! @brief Maximum number of entries ever in the queue since EDMA became operational
        (after reset), if this maximum ever exceeded the set queue threshold and
        queue thresholding were enabled, it would trigger error condition. */
    uint8_t maxQueueEntries;

    /*! @brief Number of outstanding (not yet serviced) entries in the queue,
        QSTATN::NUMVAL in EDMA UG. */
    uint8_t numOutstandingEntries;
} EDMA_queueStatusInfo_t;

/*!
 * @brief EDMA status information at the time of querying status
 *  using @ref EDMA_getStatusInfo. This is not some software state but it is
 *  hardware state, typically used for debugging purposes.
 *  Note CCSTAT::QUEACTVx bits are not reported because they are redundant information
 *  given the @ref EDMA_queueStatusInfo_t_.
 */
typedef struct EDMA_statusInfo_t_
{
    /*! @brief Queue status of each queue, number of entries that are relevant are
        the number of queues in the EDMA instance from call to @ref EDMA_open
        in the returned structure @ref EDMA_instanceInfo_t_::numEventQueues. */
    EDMA_queueStatusInfo_t queue[EDMA_MAX_NUM_EVENT_QUEUES];

    /*! @brief CCSTAT::COMPACTV in EDMA UG. */
    uint8_t numOutstandingCompletionRequests;

    /*! @brief CCSTAT::EVTACTV in EDMA UG. */
    bool isAnyDmaChannelActive;

    /*! @brief CCSTAT::QEVTACTV in EDMA UG. */
    bool isAnyQdmaChannelActive;

    /*! @brief CCSTAT::WSTATACTV in EDMA UG. */
    bool isWriteStatusActive;

    /*! @brief CCSTAT::TRACTV in EDMA UG. */
    bool isAnyTransferActive;

    /*! @brief CCSTAT::ACTV in EDMA UG. */
    bool isAnythingActive;
} EDMA_statusInfo_t;

/*!
 * @brief EDMA performance configuration.
 */
typedef struct EDMA_performanceConfig_t_
{
    /*! @brief If set to true, configures all transfer controllers with specified
        configuration (@ref transferControllerId is not relevant). */
    bool isConfigAllTransferControllers;

    /*! @brief If @ref isConfigAllTransferControllers is false, then configures the
        transfer controller of this Id. */
    uint8_t transferControllerId;

    /*! @brief Read rate is used to specify using @ref EDMA_READ_RATE_DEFINES the
        aggressiveness of read by the transfer controller,
        less cycles means more aggressive, see RDRATE in EDMA UG. */
    uint8_t transferControllerReadRate;

    /*! @brief Queue priority, the defines EDMA_TPCC_QUEPRI_PRIQ0_PRIORITYX can be used
        here, where X is in the range [0,7], Note: 0 is highest priority and
        7 is lowest. Refer to QUEPRI in EDMA UG. */
    uint8_t queuePriority;
} EDMA_performanceConfig_t;

/*!
 * @brief EDMA instance properties information.
 */
typedef struct EDMA_instanceInfo_t_
{
    /*! @brief Number of event queues, same as number of transfer controllers */
    uint8_t  numEventQueues;

    /*! @brief Number of PaRAM sets. */
    uint16_t numParamSets;

    /*! @brief Shows if channel mapping feature is supported. If this is supported (true)
        then it is possible to associate any DMA channel with any PaRAM set,
        otherwise (false), the PaRAM set must be identical to DMA channel i.e
        DMA channels 0 to 63 correspond to PaRAM sets 0 to 63 respectively. */
    bool isChannelMapExist;

    /*! @brief true if transfer completion interrupt is routed from EDMA to the
            processor interrupt controller. */
    bool isTransferCompletionInterruptConnected;

    /*! @brief true if error interrupt is is routed from EDMA to the
            processor interrupt controller. */
    bool isErrorInterruptConnected;

    /*! @brief true if a transfer controller error interrupt is is routed from EDMA to the
            processor interrupt controller. */
    bool isTransferControllerErrorInterruptConnected[EDMA_MAX_NUM_EVENT_QUEUES];

} EDMA_instanceInfo_t;

/**
@defgroup EDMA_DRIVER_EXTERNAL_FUNCTION       EDMA Driver External Functions
@brief
*   The section has a list of all the exported APIs which the application can
*   invoke in order to use the driver.
 @{ */

/**
*  @b Description
*  @n
*      Configure a channel.
*
*  @param[in]  handle
*      Handle to the EDMA instance obtained through call to @ref EDMA_open.
*  @param[in] config
*      pointer to @ref EDMA_channelConfig_t_
*  @param[in] isEnableChannel
*      set to true if you want to enable the channel after configuration
*      else set to false. For false case, it is expected that the
*      channel will be enabled at a later time
*      using @ref EDMA_enableChannel for event triggered DMA channel events or 
*      QDMA channels.
*      Note for DMA events that are intended not to be event triggered i.e
*      manual triggered or chain triggered only, this flag should be set to 
*      false else a real event appearing on the event line may unintentionally
*      trigger the DMA channel.
*  @retval
*      Success     - @ref EDMA_NO_ERROR
*  @retval
*      Error       - one of @ref EDMA_ERROR_CODES
*/
extern int32_t EDMA_configChannel(EDMA_Handle handle,
    EDMA_channelConfig_t const *config, bool isEnableChannel);

/**
*  @b Description
*  @n
*      Enable a channel.
*
*  @param[in]  handle
*      Handle to the EDMA instance obtained through call to @ref EDMA_open.
*  @param[in] channelId
*      0 to (@ref EDMA_NUM_DMA_CHANNELS - 1) when channelType is @ref EDMA3_CHANNEL_TYPE_DMA. \n
*      0 to (@ref EDMA_NUM_QDMA_CHANNELS - 1) when channelType is @ref EDMA3_CHANNEL_TYPE_QDMA.
*  @param[in] channelType
*      channelType can be either @ref EDMA3_CHANNEL_TYPE_DMA or @ref EDMA3_CHANNEL_TYPE_QDMA.
*  @retval
*      Success     - @ref EDMA_NO_ERROR
*  @retval
*      Error       - one of @ref EDMA_ERROR_CODES
*/
extern int32_t EDMA_enableChannel(EDMA_Handle handle,
    uint8_t channelId, uint8_t channelType);

/**
*  @b Description
*  @n
*      Disable a channel.
*
*  @param[in]  handle
*      Handle to the EDMA instance obtained through call to @ref EDMA_open
*  @param[in] channelId
*      0 to (@ref EDMA_NUM_DMA_CHANNELS - 1) when channelType is @ref EDMA3_CHANNEL_TYPE_DMA. \n
*      0 to (@ref EDMA_NUM_QDMA_CHANNELS - 1) when channelType is @ref EDMA3_CHANNEL_TYPE_QDMA.
*  @param[in] channelType
*      channelType can be either @ref EDMA3_CHANNEL_TYPE_DMA or @ref EDMA3_CHANNEL_TYPE_QDMA.
*  @retval
*      Success     - @ref EDMA_NO_ERROR
*  @retval
*      Error       - one of @ref EDMA_ERROR_CODES
*/
extern int32_t EDMA_disableChannel(EDMA_Handle handle,
    uint8_t channelId, uint8_t channelType);

/**
*  @b Description
*  @n
*      Chain channels.
*
*  @param[in]  handle
*      Handle to the EDMA instance obtained through call to @ref EDMA_open.
*  @param[in] fromParamId
*      Param Id of the channel from which to chain.
*  @param[in] toChannelId
*      channel Id of the channel to which to chain to.
*  @retval
*      Success     - @ref EDMA_NO_ERROR
*  @retval
*      Error       - one of @ref EDMA_ERROR_CODES
*/
extern int32_t EDMA_chainChannels(EDMA_Handle handle,
    uint16_t fromParamId, uint8_t toChannelId);

/**
*  @b Description
*  @n
*      Configure Param Set
*
*  @param[in]  handle
*      Handle to the EDMA instance obtained through call to @ref EDMA_open.
*  @param[in] paramId PaRAM Set Id of the PaRAM Set to be configured.
*  @param[in] config
*      Pointer to @ref EDMA_paramConfig_t
*  @retval
*      Success     - @ref EDMA_NO_ERROR
*  @retval
*      Error       - one of @ref EDMA_ERROR_CODES
*/
extern int32_t EDMA_configParamSet(EDMA_Handle handle,
    uint16_t paramId,
    EDMA_paramConfig_t const *config);

/**
*  @b Description
*  @n
*      Link Param Sets.
*
*  @param[in]  handle
*      Handle to the EDMA instance obtained through call to @ref EDMA_open.
*  @param[in] fromParamId
*      Param Id from which to link
*  @param[in] toParamId
*      Param Id to which to link
*  @retval
*      Success     - @ref EDMA_NO_ERROR
*  @retval
*      Error       - one of @ref EDMA_ERROR_CODES
*/
extern int32_t EDMA_linkParamSets(EDMA_Handle handle,
    uint16_t fromParamId, uint16_t toParamId);

/**
*  @b Description
*  @n
*     Start a transfer.
*
*  @param[in]  handle
*      Handle to the EDMA instance obtained through call to @ref EDMA_open.
*  @param[in] channelId
*      0 to (@ref EDMA_NUM_DMA_CHANNELS - 1) when channelType is @ref EDMA3_CHANNEL_TYPE_DMA \n
*      0 to (@ref EDMA_NUM_QDMA_CHANNELS - 1) when channelType is @ref EDMA3_CHANNEL_TYPE_QDMA
*  @param[in] channelType
*      channelType can be either @ref EDMA3_CHANNEL_TYPE_DMA or @ref EDMA3_CHANNEL_TYPE_QDMA
*  @retval
*      Success     - @ref EDMA_NO_ERROR
*  @retval
*      Error       - one of @ref EDMA_ERROR_CODES
*/
extern int32_t EDMA_startTransfer(EDMA_Handle handle,
    uint8_t channelId, uint8_t channelType);

/**
*  @b Description
*  @n
*     Start a DMA channel transfer. It is a simpler/faster version of @ref EDMA_startTransfer
*     when channel is known to be of type DMA.
*
*  @param[in]  handle
*      Handle to the EDMA instance obtained through call to @ref EDMA_open.
*  @param[in] channelId
*      0 to (@ref EDMA_NUM_DMA_CHANNELS - 1)
*  @retval
*      Success     - @ref EDMA_NO_ERROR
*  @retval
*      Error       - one of @ref EDMA_ERROR_CODES
*/
extern int32_t EDMA_startDmaTransfer(EDMA_Handle handle, uint8_t channelId);

/**
*  @b Description
*  @n
*     Start a QDMA channel transfer. It is a simpler/faster version of
*     @ref EDMA_startTransfer when channel is known to be of type QDMA.
*
*  @param[in]  handle
*      Handle to the EDMA instance obtained through call to @ref EDMA_open
*  @param[in] channelId
*      0 to (@ref EDMA_NUM_QDMA_CHANNELS - 1)
*  @retval
*      Success     - @ref EDMA_NO_ERROR
*  @retval
*      Error       - one of @ref EDMA_ERROR_CODES
*/
extern int32_t EDMA_startQdmaTransfer(EDMA_Handle handle, uint8_t channelId);

/**
*  @b Description
*  @n
*     Start final QDMA transfer. It modifies OPT field's STATIC bit to 1 before
*     triggering QDMA, useful for multi-transfer type configurations (A-type transfer
*     where the product of bCount and cCount is greater than 1 or AB-type transfer where
*     cCount is greater than 1) where last but one transfer must have STATIC bit
*     to be 0 and the last transfer needs STATIC to be 1 to prevent a NULL transfer error.
*
*  @param[in]  handle
*      Handle to the EDMA instance obtained through call to @ref EDMA_open
*  @param[in] channelId
*      0 to (@ref EDMA_NUM_QDMA_CHANNELS - 1)
*  @retval
*      Success     - @ref EDMA_NO_ERROR
*  @retval
*      Error       - one of @ref EDMA_ERROR_CODES
*/
extern int32_t EDMA_startFinalQdmaTransfer(EDMA_Handle handle,
    uint8_t channelId);

/**
*  @b Description
*  @n
*     Query for transfer completion. This function will return an error 
*     if transfer completion callback function for the supplied transferCompletionCode
*     is configured as non NULL. This is because the burden of clearing the channel
*     is not in the call back but in the driver after the user supplied call back is called.
*     So polling on a cleared channel would give wrong result i.e indicate transfer
*     is not complete even when it has completed. The call back completion 
*     and this API are exclusionary.
*
*  @param[in]  handle
*      Handle to the EDMA instance obtained through call to @ref EDMA_open
*  @param[in] transferCompletionCode
*      which transfer completion code to query, range is [0,(@ref EDMA_NUM_TCC - 1)]
*  @param[out] isTransferComplete
*      true if transfer is complete, false otherwise. <B>NOTE</B>: If true, then
*      IPR(H) bit of the completed transfer will be cleared, allowing for a new
*      transfer to complete.
*
*  @retval
*      Success     - @ref EDMA_NO_ERROR
*  @retval
*      Error       - one of @ref EDMA_ERROR_CODES
*/
extern int32_t EDMA_isTransferComplete(EDMA_Handle handle,
    uint8_t transferCompletionCode,
    bool *isTransferComplete);

/**
*  @b Description
*  @n
*     Set Destination Address of PaRAM.
*
*  @param[in]  handle
*      Handle to the EDMA instance obtained through call to @ref EDMA_open
*  @param[in] paramId
*      Id of the the PaRAM
*  @param[in] destinationAddress
*      Destination address.
*
*  @retval
*      Success     - @ref EDMA_NO_ERROR
*  @retval
*      Error       - one of @ref EDMA_ERROR_CODES
*/
extern int32_t EDMA_setDestinationAddress(EDMA_Handle handle,
    uint16_t paramId,
    uint32_t destinationAddress);

/**
*  @b Description
*  @n
*     Set Source Address of PaRAM.
*
*  @param[in]  handle
*      Handle to the EDMA instance obtained through call to @ref EDMA_open
*  @param[in] paramId
*      Id of the the PaRAM
*  @param[in] sourceAddress
*      Source address.
*
*  @retval
*      Success     - @ref EDMA_NO_ERROR
*  @retval
*      Error       - one of @ref EDMA_ERROR_CODES
*/
extern int32_t EDMA_setSourceAddress(EDMA_Handle handle,
    uint16_t paramId,
    uint32_t sourceAddress);

/**
*  @b Description
*  @n
*      Configure Error monitoring. The EDMA IP has several features
*      for monitoring errors in the CC and TC as described in EDMA UG :
*      - Missed events.\n
*      - Exceeding of queue threshold.\n
*      - Bus error caused due to read/writes in transfers where addresses go
*        out of bounds of physical memory (i.e where EDMA IP
*        does not get any ACK back).\n
*      - Read/writes to invalid/reserved addresses in CC/TC register memory map.\n
*      - Exceeding of outstanding transfer controller codes.\n
*
*      This API allows selective control (where available in h/w)
*      of what to monitor. It should be called during init time.
*      It is important to issue this API for safety, besides being useful in debugging.
*
*  @param[in]  handle
*      Handle to the EDMA instance obtained through call to @ref EDMA_open.
*  @param[in] config
*      pointer to error configuration @ref EDMA_errorConfig_t_.
*
*  @retval
*      Success     - @ref EDMA_NO_ERROR
*  @retval
*      Error       - one of @ref EDMA_ERROR_CODES
*/
extern int32_t EDMA_configErrorMonitoring(EDMA_Handle handle,
    EDMA_errorConfig_t const *config);

/**
*  @b Description
*  @n
*     Query Error Status of EDMA. This API is used for polling and getting
*     EDMA error status if poll indicates there was an error. This is useful
*     in some devices where the EDMA's error interrupt is not physically wired
*     to the interrupt controller of the processor but EDMA peripheral registers are
*     accessible. When interrupt is connected, this API is not necessary because the error
*     ISR will call out the user provided call back function (@ref EDMA_errorConfig_t::callbackFxn)
*     when issuing @ref EDMA_configErrorMonitoring.
*
*  @param[in]  handle
*      Handle to the EDMA instance obtained through call to @ref EDMA_open
*  @param[out] isAnyError
*      true if any EDMA error is detected, else false
*  @param[out] errorInfo
*      Pointer to error information, this is populated if any EDMA error is detected.
*
*  @retval
*      Success     - @ref EDMA_NO_ERROR
*  @retval
*      Error       - one of @ref EDMA_ERROR_CODES
*
*/
extern int32_t EDMA_getErrorStatus(EDMA_Handle handle, bool *isAnyError,
    EDMA_errorInfo_t *errorInfo);

/**
*  @b Description
*  @n
*     Query Error Status of EDMA's transfer controller. This API is used for
*     polling and getting EDMA transfer controller error status if poll indicates
*     there was an error. This is useful in some devices where the EDMA's transfer controller
*     interrupt is not physically wired to the interrupt controller of the processor
*     but EDMA peripheral registers are accessible. When interrupt is connected,
*     this API is not necessary because the error ISR will call out the user
*     provided call back function (@ref EDMA_errorConfig_t::transferControllerCallbackFxn)
*     when issuing @ref EDMA_configErrorMonitoring.
*
*  @param[in]  handle
*      Handle to the EDMA instance obtained through call to @ref EDMA_open
*  @param[in]  transferControllerId
*      Transfer Controller Id
*  @param[out] isAnyError
*      true if any EDMA transfer controller error is detected, else false
*  @param[out] errorInfo
*      Pointer to error information, this is populated if any EDMA error is detected.
*
*  @retval
*      Success     - @ref EDMA_NO_ERROR
*  @retval
*      Error       - one of @ref EDMA_ERROR_CODES
*
*/
extern int32_t EDMA_getTransferControllerErrorStatus(EDMA_Handle handle,
    uint8_t transferControllerId, bool *isAnyError, EDMA_transferControllerErrorInfo_t *errorInfo);


/**
*  @b Description
*  @n
*     Query Status of EDMA. This is useful for debugging/diagnostic purposes.
*     Typically called in the implementation of the error call back function
*     of the application supplied in the @ref EDMA_errorConfig_t_::callbackFxn
*     and/or @ref EDMA_errorConfig_t_::transferControllerCallbackFxn when
*     issuing @ref EDMA_configErrorMonitoring, for additional post-mortem analysis.
*
*  @param[in]  handle
*      Handle to the EDMA instance obtained through call to @ref EDMA_open
*  @param[out] status
*      status output
*
*  @retval
*      Success     - @ref EDMA_NO_ERROR
*  @retval
*      Error       - one of @ref EDMA_ERROR_CODES
*/
extern int32_t EDMA_getStatusInfo(EDMA_Handle handle,
    EDMA_statusInfo_t *status);

/**
*  @b Description
*  @n
*     Configure Performance parameters. This is useful for performance tuning
*     of transfer controllers relative to each other and priority of EDMA
*     issue on the bus with respect to rest of IPs for memory access.
*
*  @param[in]  handle
*      Handle to the EDMA instance obtained through call to @ref EDMA_open.
*  @param[in] config
*      Performance configuration input.
*
*  @retval
*      Success     - @ref EDMA_NO_ERROR
*  @retval
*      Error       - one of @ref EDMA_ERROR_CODES
*/
extern int32_t EDMA_configPerformance(EDMA_Handle handle,
    EDMA_performanceConfig_t const *config);

/**
*  @b Description
*  @n
*     Open a specified EDMA instance.
*
*  @param[in]  instanceId
*      Instance Id to open, range is [0, number of instances-1], where
*      number of instances is obtained from a call to @ref EDMA_getNumInstances.
*  @param[out] errorCode
*      One of @ref EDMA_ERROR_CODES if there is an error, otherwise @ref EDMA_NO_ERROR.
*  @param[out] instanceInfo
*      Instance information.
*
*  @retval
*      Handle to the EDMA instance. If errorCode returns an error i.e
*      is not @ref EDMA_NO_ERROR, then handle is NULL.
*/
extern EDMA_Handle EDMA_open(uint8_t instanceId, int32_t *errorCode,
    EDMA_instanceInfo_t *instanceInfo);

/**
*  @b Description
*  @n
*     Close EDMA instance.
*
*  @param[in]  handle
*      Handle to the EDMA instance obtained through call to @ref EDMA_open.
*
*  @retval
*      Success     - @ref EDMA_NO_ERROR
*  @retval
*      Error       - one of @ref EDMA_ERROR_CODES
*/
extern int32_t EDMA_close(EDMA_Handle handle);

/**
*  @b Description
*  @n
*     Get number of EDMA instances on the device.
*  @retval
*      Number of EDMA instances.
*/
extern uint8_t EDMA_getNumInstances(void);

/**
*  @b Description
*  @n
*     Initialize specified EDMA instance. Performs the following:
*      - Disables and clears events.
*      - Disables and clears interrupts.
*      - Clears error state.
*      - Sets all PaRAM sets to all zeroes (hardware reset state).
*
*  @param[in]  instanceId
*      Id of the instance to be initialized.
*  @retval
*      Success     - @ref EDMA_NO_ERROR
*  @retval
*      Error       - one of @ref EDMA_ERROR_CODES
*/
extern int32_t EDMA_init(uint8_t instanceId);


/** @}*/

#ifdef __cplusplus
}
#endif
#endif /* ti_drivers_EDMA__include */
