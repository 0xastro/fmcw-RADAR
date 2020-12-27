/*
 * Copyright (c) 2015, Texas Instruments Incorporated
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
/** ============================================================================
*  @file       dma.h
*
*  @brief      DMA driver interface
* 
*/

/** @mainpage DMA Driver
*
*  The DMA driver provides functionaity to copy data around from one memory location
*  to the other using hardware thereby offloading the operation from CPU.
*
*  The DMA header file should be included in an application as follows:
*  @code
*  #include <ti/drivers/dma/dma.h>
*  @endcode
*  
*  ## Initializing the driver #
*  The DMA Driver needs to be initialized once across the System. This is
*  done using the #DMA_init. None of the DMA API can be used without invoking
*  this API
*
*  ## Opening the driver #
*  Once the DMA Driver has been initialized; the DMA Driver instance can be opened
*  using the #DMA_open. The helper function #DMA_Params_init can be used to
*  populate the default parameters. The #DMA_open can be called multiple times from different
*  context to obtain a valid DMA handle. However, only the first call to #DMA_open will 
*  perform the hardware initialization. Other subsequent calls will just return the already
*  opened handle.
*
*  @code
*  @endcode
*
*  ## Using the driver #
*  Following is a psuedo code for using the DMA driver in software trigger mode
*  to copy 4 8-bit data at a time and repeat it for 10 times with PostIncrement addressing mode.
*
*  @code
*   
    
    DMA_Params     params;
    DMA_Handle     handle;

    DMA_init();
    DMA_Params_init(&params);
    handle = DMA_open(0, &params, &errCode);

    // set the channel paramters
    ctrlPacket.srcAddr=(uint32_t)gDMATestSource;
    ctrlPacket.destAddr=(uint32_t)gDMATestDest;
    ctrlPacket.frameXferCnt=10;
    ctrlPacket.elemXferCnt=4;
    ctrlPacket.nextChannel=DMA_CHANNEL_NONE;
    ctrlPacket.srcElemSize=DMA_ElemSize_8bit;
    ctrlPacket.destElemSize=DMA_ElemSize_8bit;
    ctrlPacket.autoInitiation=0;
    ctrlPacket.xferType=DMA_XferType_Frame;
    ctrlPacket.srcAddrMode=DMA_AddrMode_PostIncrement;
    ctrlPacket.destAddrMode=DMA_AddrMode_PostIncrement;
    DMA_setChannelParams(handle, dmaChannelNum,&ctrlPacket);

    // enable completion Interrupt and provide a callback function
    DMA_enableInterrupt(handle,dmaChannelNum,DMA_IntType_FTC,DMA_TestInterruptCb,(void *)dmaTestContext);

    if (hardwareTriggered)
    {
        // if channel is to be hardware triggered, DMA_assignChannel() API need to be called
        // to hook up H/W request line to the channel
        DMA_assignChannel(handle,dmaChannelNum,dmaReqline,errCode);

        // enable the channel
        DMA_enableChannel(handle,dmaChannelNum,DMA_ChTriggerType_HW);
    }
    else
    {
        // enable the channel for software trigger
        DMA_enableChannel(handle,dmaChannelNum,DMA_ChTriggerType_SW);
    }

    //
    //
    // wait for DMA completion interrupt
    //
    //


    //Time to tear down when job is complete
    //disable interrupt
    DMA_disableInterrupt (handle,dmaChannelNum,DMA_IntType_FTC);
    
    // disable channel
    if (hardwareTriggered)
    {
        // if channel was hardware triggered, free the channel from h/w req line hook up
        DMA_freeChannel (handle, dmaChannelNum,dmaReqline);

        // disable the channel
        DMA_disableChannel(handle,dmaChannelNum,DMA_ChTriggerType_HW);
    }
    else
    {
        // disable the channel for software trigger
        DMA_disableChannel(handle,dmaChannelNum,DMA_ChTriggerType_SW);
    }

    // Close the driver
    errCode = DMA_close(handle);

    
*  @endcode
*  ## Instrumentation #
*  Uses DebugP_log functions for debug messages
*
*  ## Hardware Register Map #
*  The hardware register map used by this driver can be found at include/reg_dma.h and reg_dmaram.h
*
* ============================================================================
*/
/**
@defgroup DMA_DRIVER_EXTERNAL_FUNCTION            DMA Driver External Functions
@ingroup DMA_DRIVER
@brief
*   The section has a list of all the exported API which the applications need to
*   invoke in order to use the driver
*/
/**
@defgroup DMA_DRIVER_EXTERNAL_DATA_STRUCTURE      DMA Driver External Data Structures
@ingroup DMA_DRIVER
@brief
*   The section has a list of all the data structures which are exposed to the application
*/
/**
@defgroup DMA_DRIVER_ERROR_CODE                   DMA Driver Error Codes
@ingroup DMA_DRIVER
@brief
*   The section has a list of all the error codes which are generated by the DMA Driver
*   module
*/

#ifndef ti_drivers_DMA__include
#define ti_drivers_DMA__include
    
#ifdef __cplusplus
    extern "C" {
#endif
    
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <ti/common/mmwave_error.h>


/** @addtogroup DMA_DRIVER_ERROR_CODE
 *
 * @brief
 *  Base error code for the DMA module is defined in the ti/common/mmwave_error.h
 *  \include ti/common/mmwave_error.h
 *
 @{ */

/**
 * @brief   Error Code: Invalid argument
 */
#define DMA_EINVAL                  (MMWAVE_ERRNO_DMA_BASE-1)

/**
 * @brief   Error Code: Operation cannot be done as DMA_init is not done.
 */
#define DMA_ENOINIT                 (MMWAVE_ERRNO_DMA_BASE-2)

/**
 * @brief   Error Code: input argument out of range
 */
#define DMA_EOUTOFRANGE             (MMWAVE_ERRNO_DMA_BASE-3)

/**
 * @brief   Error Code: input argument out of range
 */
#define DMA_EOUTOFMEM               (MMWAVE_ERRNO_DMA_BASE-4)

/**
 * @brief   Error Code: feature not supported
 */
#define DMA_ENOTSUPP                (MMWAVE_ERRNO_DMA_BASE-5)

/**
 * @brief   Error Code: channel or resource in use
 */
#define DMA_EINUSE                  (MMWAVE_ERRNO_DMA_BASE-6)

/**
 * @brief   Error Code: channel or resource in use
 */
#define DMA_ENOTALIGNED             (MMWAVE_ERRNO_DMA_BASE-7)

/**
 * @brief   Error Code: Polling and Interrupts are mutually exclusive
 */
#define DMA_EPOLLINGNOTALLOWED      (MMWAVE_ERRNO_DMA_BASE-8)



/** @}*/


/** @addtogroup DMA_DRIVER_EXTERNAL_DATA_STRUCTURE
 @{ */

/*!
 *  @brief      Define for an unspecified channel.
 */
#define DMA_CHANNEL_NONE  0xFFU


/*!
 *  @brief      A handle that is returned from a DMA_open() call.
 */
typedef void* DMA_Handle;



/*!
 *  @brief    DMA Interrupt Type 
 *
 *  Enum for DMA Interrupt type. 
 *
 */
typedef enum DMA_IntType_e {
    DMA_IntType_FTC = 0U,    /*!<DMA Peripheral's FTC interrupt type */
    DMA_IntType_LFS,        /*!<DMA Peripheral's LFS interrupt type */
    DMA_IntType_HBC,        /*!<DMA Peripheral's HBC interrupt type */
    DMA_IntType_BTC,        /*!<DMA Peripheral's BTC interrupt type */
    DMA_IntType_BER,        /*!<DMA Peripheral's BER interrupt type */
    DMA_IntType_NUM,        /*!<DMA Peripheral's total number of interrupts*/
}DMA_IntType;


/*!
 *  @brief    DMA Priority Scheme
 *
 *  Enum for DMA Peripheral's priority scheme for handling 
 *  requests in its low pri and high pri queue.
 *  
 * Fixed Priority scheme 
 *      - lower the channel number, the higher its priority
 *      - A channel will be arbitrated out whenever there is a higher pending channel. 
 *      - Otherwise a channel is completely serviced until its transfer count reaches zero before the next
 *        highest pending channel is serviced
 *
 * Rotation based Priority scheme 
 *      - round-robin scheme
 *      - Arbitration is performed when the FIFO is empty
 *
 */
typedef enum DMA_PriScheme_e {
    DMA_PriScheme_Fixed = 0U,        /*!< Fixed - lower the channel number, the higher its priority */
    DMA_PriScheme_Rotation,         /*!< round-robin scheme - Arbitration is performed when the FIFO is empty.*/
} DMA_PriScheme;


/*!
 *  @brief    DMA Debug Mode 
 *
 *  Enum for DMA Peripheral's behavior when it is entering suspend  
 *
 */
typedef enum DMA_DebugMode_e {
    DMA_DebugMode_IgnoreSuspend = 0U,        /*!< Ignore the suspend. The DMA continues to be operational 
                                                 as in functional mode when debug mode is active */
    DMA_DebugMode_FinishBlock,              /*!< Finish current block transfer and continue after suspend ends */
    DMA_DebugMode_FinishFrame,              /*!< Finish current frame transfer and continue after suspend ends */
    DMA_DebugMode_Immediate                 /*!< Immediate stop at a DMA channel arbitration boundary */
} DMA_DebugMode;


/*!
 *  @brief    DMA H/W Parameters
 *
 *  DMA parameters are used with the DMA_open() call. 
 *
 */
typedef struct DMA_HWAttrs_t {
    uint32_t                instanceNum;
    volatile uint32_t       ctrlBaseAddr;            /*!< DMA Peripheral's base address for the control register space */
    volatile uint32_t       pktBaseAddr;             /*!< DMA Peripheral's base address for the control packet (RAM) space */
    uint32_t                numDmaChannels;          /*!< Number of DMA channels in this instance */
    uint32_t                numReqLines;             /*!< Number of DMA request lines in this instance */
    uint32_t                defaultPortAssignment;   /*!< Default Port assignment for all the channels */
    uint32_t                intNum[DMA_IntType_NUM]; /*!< DMA Peripheral's interrupt vector for each interrupt type*/
} DMA_HWAttrs;


/*!
 *  @brief    DMA Parameters
 *
 *  DMA parameters are used to with the DMA_open() call. 
 *  These parameters will be applied only in the first DMA_open() call
 *  Subsequent DMA_open() calls will ignore this parameter.
 *
 */
typedef struct DMA_Params_t {
    DMA_PriScheme   lowPriScheme;      /*!< Priority scheme for low priority queue */
    DMA_PriScheme   highPriScheme;     /*!< Priority scheme for high priority queue */
    bool            bypassFifo;        /*!< Set to 0 to have FIFO not bypassed (Recommended)
                                            Set it to 1 to bypass FIFO  
                                                - Setting 1 to this field limits the FIFO depth to the size of one element. 
                                                  That means that after one element is read, the write-out to the destination will begin. 
                                                  This feature is particularly useful to minimize switching latency between channels.
                                                  Note: This feature does not make optimal use of bus bandwidth.
                                         */
    DMA_DebugMode   dmaDebugMode;       /* this field sets the behavior of DMA when system is enterng Debug mode */
} DMA_Params;


/*!
 *  @brief    DMA Interrupt callback function
 *
 *  DMA Interrupt callback function to be used with DMA_enableInterrupt() call
 *
 */
typedef void (*DMA_IntHandlerFuncPTR)(uint32_t channel, DMA_IntType inttype, void * arg);

/*!
 *  @brief    DMA Transfer Type for a given channel
 *
 *  Enum for DMA Transfer type for a given channel.
 *
 */
typedef enum DMA_ElemSize_e {
    DMA_ElemSize_8bit = 0U,     /*!<element size is 8-bit data */
    DMA_ElemSize_16bit,         /*!<element size is 16-bit data */
    DMA_ElemSize_32bit,         /*!<element size is 32-bit data */
    DMA_ElemSize_64bit,         /*!<element size is 64-bit data*/
    DMA_ElemSize_Last
}DMA_ElemSize;



/*!
 *  @brief    DMA Transfer Type for a given channel
 *
 *  Enum for DMA Transfer type for a given channel.
 *
 */
typedef enum DMA_XferType_e {
    DMA_XferType_Frame = 0U,    /*!< frame transfer: 
                                    one DMA request (hw or sw) triggers one complete frame transfer 
                                    (i.e. 'elemXferCnt' number of element transfers) */
    DMA_XferType_Block,         /*!<block transfer: 
                                   one DMA request (hw or sw) triggers one complete block transfer 
                                  (i.e. 'frameXferCnt' number of frame transfers) */
    DMA_XferType_Last
}DMA_XferType;

/*!
 *  @brief    DMA addressing mode for a given transfer
 *
 *  Enum for DMA addressing for a given transfer.
 *
 */
typedef enum DMA_AddrMode_e {
    DMA_AddrMode_Constant       = 0U,      /*!< constant addressing mode: 
                                               source and/or destination address do not change 
                                               between each elemen transfer */
    DMA_AddrMode_PostIncrement  = 1U,      /*!< post increment addressing mode: 
                                               source and/or destination address are post incremented 
                                               by the source and/or destination element size after 
                                               each element transfer */
    DMA_AddrMode_Indexed        = 3U,      /*!< indexed addressing mode: 
                                               source and/or destination address are post incremented 
                                               as defined by the source and/or destination 
                                               element and frame index offset parameters
                                               after each element and frame transfer respectively */
    DMA_AddrMode_Last           = DMA_AddrMode_Indexed+1,
}DMA_AddrMode;


/*!
 *  @brief    DMA Channel Control Packet Parameters
 *
 *  DMA Control Packet parameters are used to with the DMA_setChannelParams() call.
 *
 */
typedef struct DMA_CtrlPktParams_t {

    /*!
     * @brief   initial source address for the transfer \n 
     *          32-bit physical address \n 
     *          must be aligned with respect to element size \n 
     */
    uint32_t srcAddr;

    /*!
     * @brief   initial destination address for the transfer \n 
     *          32-bit physical address \n 
     *          must be aligned with respect to element size \n 
     */
    uint32_t destAddr;

    /*!
     * @brief   Initial frame transfer count
     *              - 13 bits value
     *              - for a valid transfer, minimum value should be 1
     *              - a value of 0 results in total transfer count of 0 
     *                and no DMA transaction is triggered
     *              - 'frameXferCnt' worth of transfers is equal to one block transfer
     *          Total transfer count: (source element size) * (element transfer count) * (frame tranfer count) 
     */
    uint16_t frameXferCnt;
    /*!
     * @brief   initial element transfer count
     *              - 13 bits value
     *              - for a valid transfer, minimum value should be 1
     *              - a value of 0 results in total transfer count of 0 
     *                and no DMA transaction is triggered
     *              - 'elemXferCnt' worth of transfers is equal to one frame transfer
     *          Total transfer count: (source element size) * (element transfer count) * (frame tranfer count) 
     */
    uint16_t elemXferCnt;
    
    /*!
     * @brief   enable channel chaining
     *              - specify the next channel whose control packet to be triggered after
     *                current channel's programmed number of frames (i.e. 1 block transfer) is complete.
     *              - to enable chaining: 
     *                  specify valid channel number i.e. 0 to max number of channels supported by the device
     *              - to disable chaninng: 
     *                  specify DMA_CHANNEL_NONE
     *          NOTE: If channel chain is needed for a transfer, then this field must be set before DMA transfer is initiated
     */
    uint8_t  nextChannel;
    /*!
     * @brief   element size for reading from the source buffer \n 
     *          Value: see enum DMA_ElemSize
     */
    DMA_ElemSize srcElemSize;
    /*!
     * @brief   element size for reading from the destination buffer \n 
     *          Value: see enum DMA_ElemSize
     */
    DMA_ElemSize destElemSize;
    /*!
     * @brief   enable/disable auto initiation mode  \n 
     *          Value: \n 
     *              0 - disable \n 
     *              1 - enable \n 
     *          When enabled:
     *              - S/W triggered channel restarts the transfer automatically after 
     *              one completed block transfer
     *              - H/W triggered channel needs to be re-triggered by H/W each time after 
     *              a block is complete even if this mode is enabled
     */
    uint8_t autoInitiation;
    /*!
     * @brief   transfer type \n 
     *          Value: see enum DMA_XferType
     */
    DMA_XferType xferType;
    /*!
     * @brief   addressing mode for source address to be followed after each element and frame transfer \n 
     *          Value: see enum DMA_AddrMode
     */
    DMA_AddrMode srcAddrMode;
    /*!
     * @brief   addressing mode for source address to be followed after each element and frame transfer \n 
     *          Value: see enum DMA_AddrMode
     */
    DMA_AddrMode destAddrMode;

    /*!
     * @brief   Source address element index.  \n 
     *          13 bit value  \n 
     *          This parameter define the offset to be added to the source
     *          address after each element transfer when indexed addressing mode 
     *          is selected for the source.
     */
    uint16_t srcElemIndexOffset;
    /*!
     * @brief   Destination address element index.  \n 
     *          13 bit value  \n 
     *          This parameter define the offset to be added to the destination
     *          address after each element transfer when indexed addressing mode 
     *          is selected for the destination.
     */
    uint16_t destElemIndexOffset;
    /*!
     * @brief   Source address frame index.  \n 
     *          13 bit value  \n 
     *          This parameter define the offset to be added to the source address
     *          after element count has reached a value of zero. This is used 
     *          when indexed addressing mode is selected for the source.
     */
    uint16_t srcFrameIndexOffset;
    /*!
     * @brief   Destination address frame index.  \n 
     *          13 bit value  \n 
     *          This parameter define the offset to be added to the destination address
     *          after element count has reached a value of zero. This is used 
     *          when indexed addressing mode is selected for the destination.
     */
    uint16_t destFrameIndexOffset;

} DMA_CtrlPktParams;


/*!
 *  @brief    DMA Channel Trigger Type 
 *
 *  Enum for DMA Channel Trigger type. 
 *
 */
typedef enum DMA_ChTriggerType_e {
    DMA_ChTriggerType_HW = 0,    /*!<Channel Triggered via H/W  */
    DMA_ChTriggerType_SW,        /*!<Channel Triggered via S/W */
}DMA_ChTriggerType;


/*!
 *  @brief    DMA Channel Priority Type 
 *
 *  Enum for DMA Channel Priority type. 
 *
 */
typedef enum DMA_ChPriority_e {
    DMA_ChPriority_Low = 0,    /*!<Channel assigned to high Priority  */
    DMA_ChPriority_High,        /*!<Channel assigned to low Priority */
}DMA_ChPriority;


/*!
 *  @brief    DMA Channel Control Status
 *
 *  DMA Channel status structure to be used with the DMA_getChannelStatus() call. 
 *
 */
typedef struct DMA_ChannelStatus_t {
    /*@{*/ 
    /** channel status */
    uint32_t chPend;
    uint32_t chActive;
    /*@}*/
    /*@{*/ 
    /** channel trigger status */
    uint32_t chHwEnable;
    uint32_t chSwEnable;
    /*@}*/
    /*@{*/ 
    /** channel assignment status */
    uint32_t chIsHighPri;
    uint32_t chReqAssign;
    uint32_t chPortAssign;
    /*@}*/
    /*@{*/ 
    /** channel Interrupt Enable status */
    uint32_t chIsIntEn;
    uint32_t chIsFTCIntEn;
    uint32_t chIsLFSIntEn;
    uint32_t chIsHBCIntEn;
    uint32_t chIsBTCIntEn;
    /*@}*/
    /*@{*/ 
    /** channel Interrupt Pending status */
    uint32_t chIsIntPend;
    uint32_t chIsFTCIntPend;
    uint32_t chIsLFSIntPend;
    uint32_t chIsHBCIntPend;
    uint32_t chIsBTCIntPend;
    uint32_t chIsBERIntPend;
    /*@}*/
    /*@{*/ 
    /** channel initial value in control packet ram */
    uint32_t chInitSrcAddr;
    uint32_t chInitDstAddr;
    uint32_t chInitElemCnt;
    uint32_t chInitFrameCnt;
    uint32_t chControlReg;
    uint32_t chSrcElemIndex;
    uint32_t chDstElemIndex;
    uint32_t chSrcFrameIndex;
    uint32_t chDstFrameIndex;
    /*@}*/
    /*@{*/ 
    /** channel working value in control packet ram */
    uint32_t chCurrSrcAddr;
    uint32_t chCurrDstAddr;
    uint32_t chCurrElemCnt;
    uint32_t chCurrFrameCnt;
    /*@}*/
} DMA_ChannelStatus;


/*!
 *  @brief    DMA Status
 *
 *  DMA peripheral status structure to be used with the DMA_getStatus() call. 
 *
 */
typedef struct DMA_Status_t {
    /*@{*/ 
    /** global status */
    uint32_t dmaEnable;
    uint32_t dmaBusBusy;
    uint32_t dmaDbgMode;
    uint32_t dmaReset;
    /*@}*/
    /*@{*/ 
    /** port status */
    uint32_t dmaXferPend;
    uint32_t dmaFIFOBypass;
    uint32_t dmaHighPriSch;
    uint32_t dmaLowPriSch;
    /*@}*/
    /*@{*/ 
    /** Channels status */
    uint32_t dmaChPend;
    uint32_t dmaChActive;
    /*@}*/
    /*@{*/ 
    /** channels trigger enable status */
    uint32_t dmaChHwEnable;
    uint32_t dmaChSwEnable;
    uint32_t dmaChHighPri;
    /*@}*/
    /*@{*/ 
    /** channels to reqLine mapping status */
    uint32_t dmaChReqAssign[8];
    /*@}*/
    /*@{*/ 
    /** channels to port mapping status */
    uint32_t dmaChPortAssign[4];
    /*@}*/
    /*@{*/ 
    /** Channels interrupt Enable status */
    uint32_t dmaChIntEn;
    uint32_t dmaChFTCIntEn;
    uint32_t dmaChLFSIntEn;
    uint32_t dmaChHBCIntEn;
    uint32_t dmaChBTCIntEn;
    /*@}*/
    /*@{*/ 
    /** Channels Interrupt Pending Status */
    uint32_t dmaChIntPend;
    uint32_t dmaChFTCIntPend;
    uint32_t dmaChLFSIntPend;
    uint32_t dmaChHBCIntPend;
    uint32_t dmaChBTCIntPend;
    uint32_t dmaChBERIntPend;
    /*@}*/
    /*@{*/ 
    /** DMA active channel status */
    uint32_t dmaActiveSrcAddr;
    uint32_t dmaActiveDstAddr;
    uint32_t dmaActiveElemCnt;
    uint32_t dmaActiveFrameCnt;
    /*@}*/
} DMA_Status;

/** @}*/

/*******************************************************************************************************
 * DMA Exported API:
 *******************************************************************************************************/
/** @addtogroup DMA_DRIVER_EXTERNAL_FUNCTION
 @{ */

/*!
 *  @brief  Function to initialize the DMA module
 *
 *  
 *
 *  @pre    This function must be called once per system and before
 *          any other DMA driver APIs. It resets all the DMA H/W instances in the system.
 *
 */
extern void DMA_init(void);

/*!
 *  @brief  Function to initialize the DMA params strcuture needed for DMA_open() call
 *
 *  @param  params   DMA params for creating a instance for the first time
 *
 *  @return nothing
 *
 *
 *  
 *
 *  @sa     DMA_open()
 */
extern void DMA_Params_init(DMA_Params *params);


/*!
 *  @brief  Function to initialize a given DMA peripheral instance
 *
 *  Function to initialize a given DMA peripheral instance specified by the
 *  particular index value. Only the first call to this function initializes the peripheral -
 *  subsequent calls just return the already initialized handle.
 *
 *  @pre    DMA_init() has been called
 *
 *  @param  index         DMA instance number
 *
 *  @param  params        DMA params for creating a instance for the first time. This 
 *                        structure can be pre-populated with default values using
 *                        DMA_Params_init() API. This will be used in the first DMA_open() call;
 *                        it will be ignored for subsequent open calls.
 *
 *  @param  errCode       [out] valid errorCode if NULL handle returned.
 *
 *  @return A DMA_Handle upon success. NULL if an error occurs.
 *
 *
 *  
 *
 *  @sa     DMA_init()
 *  @sa     DMA_close()
 */
extern DMA_Handle DMA_open(uint32_t  index, DMA_Params* params, int32_t* errCode);

/*!
 *  @brief  Function to close a DMA peripheral specified by the DMA handle
 *
 *  @pre    DMA_open() has been called.
 *
 *  @param  handle      A DMA_Handle returned from DMA_open()
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     DMA_open()
 */
extern int32_t DMA_close(DMA_Handle handle);


/*!
 *  @brief  Function to assign DMA request lines to channels. This should be used only
 *          if the channel is meant for H/W triggering. For S/W triggered channel, ignore this API
 *
 *  @pre    DMA_open() has been called.
 *
 *  @param  handle          A DMA_Handle returned from DMA_open()
 *
 *  @param  channel         DMA channel. This can be a specific channel
 *
 *  @param  reqline         DMA request line
 *
 *  @param  errCode         [out] valid errorCode if NULL handle returned.
 *
 *  @return The channel number upon success. 'DMA_CHANNEL_NONE' if an error occurs.
 *
 *  @sa     DMA_open()
 */
 extern uint32_t DMA_assignChannel(DMA_Handle handle, uint32_t channel, uint32_t reqline, int32_t* errCode);


/*!
 *  @brief  Function to remove the DMA request lines to channel mapping 
 *          and free the channel for another assignment
 *
 *  @pre    DMA_assignChannel() has been called.
 *
 *  @param  handle          A DMA_Handle returned from DMA_open()
 *
 *  @param  channel         DMA channel. 
 *
 *  @param  reqline         DMA request line
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     DMA_open()
 */
 extern int32_t DMA_freeChannel(DMA_Handle handle, uint32_t channel, uint32_t reqline);

/*!
 *  @brief  Function to enable a given interrupt type for a given channel
 *
 *  @pre    DMA_open() has been called.
 *
 *  @param  handle          A DMA_Handle returned from DMA_open()
 *
 *  @param  channel         DMA channel.
 *
 *  @param  intType         DMA interrupt type
 *
 *  @param  callbackFn      User supplied callback function that the ISR will call
 *                          on receiving this interrupt. Care should be taken by the user to 
 *                          do minimal processing in this callback function. Typical implementation
 *                          of this function would do some kind of semaphore or event post.
 *
 *  @param  callbackArg     User supplied callback functions arg that will be provided back to user 
 *                          when the callback function is called from the ISR.
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     DMA_open()
 */
 extern int32_t DMA_enableInterrupt(DMA_Handle handle, 
                                          uint32_t channel, 
                                          DMA_IntType intType, 
                                          DMA_IntHandlerFuncPTR callbackFn, 
                                          void * callbackArg);


/*!
 *  @brief  Function to disable a given interrupt type for a given channel
 *
 *  @pre    DMA_open() has been called.
 *
 *  @param  handle          A DMA_Handle returned from DMA_open()
 *
 *  @param  channel         DMA channel.
 *
 *  @param  intType         DMA interrupt type
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     DMA_open()
 */
extern int32_t DMA_disableInterrupt(DMA_Handle handle, uint32_t channel, DMA_IntType intType);


/*!
 *  @brief  Function to set the control packet RAM for a given channel
 *
 *  @pre    DMA_open() has been called.
 *
 *  @param  handle          A DMA_Handle returned from DMA_open()
 *
 *  @param  channel         DMA channel.
 *
 *  @param  ctrlPacket      DMA Control Packet Parameters
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     DMA_open()
 */
extern int32_t DMA_setChannelParams(DMA_Handle handle, uint32_t channel, DMA_CtrlPktParams *ctrlPacket);

/*!
 *  @brief  Function to enable the channel given the trigger type
 *
 *  @pre    DMA_open() has been called.
 *
 *  @param  handle          A DMA_Handle returned from DMA_open()
 *
 *  @param  channel         DMA channel.
 *
 *  @param  triggerType     DMA channel trigger type
 *
 *  @sa     DMA_open()
 */
extern int32_t DMA_enableChannel(DMA_Handle handle, uint32_t channel, DMA_ChTriggerType triggerType);

/*!
 *  @brief  Function to disable the channel given the trigger type
 *
 *  @pre    DMA_open() has been called.
 *
 *  @param  handle          A DMA_Handle returned from DMA_open()
 *
 *  @param  channel         DMA channel.
 *
 *  @param  triggerType     DMA channel trigger type
 *
 *  @sa     DMA_open()
 */
extern int32_t DMA_disableChannel(DMA_Handle handle, uint32_t channel, DMA_ChTriggerType triggerType);

/*!
 *  @brief  Function to set the channel priority
 *
 *  @pre    DMA_open() has been called.
 *
 *  @param  handle          A DMA_Handle returned from DMA_open()
 *
 *  @param  channel         DMA channel.
 *
 *  @param  priority        DMA channel priority
 *
 *  @sa     DMA_open()
 */
extern int32_t DMA_setChannelPriority(DMA_Handle handle, uint32_t channel, DMA_ChPriority priority);


/*!
 *  @brief  Function to get the channel status
 *
 *  @pre    DMA_open() has been called.
 *
 *  @param  handle          A DMA_Handle returned from DMA_open()
 *
 *  @param  channel         DMA channel.
 *
 *  @param  pChanStatus     [out] Output block filled by the driver with the channel status
 *
 *  @sa     DMA_open()
 */
extern int32_t DMA_getChannelStatus(DMA_Handle handle, uint32_t channel, DMA_ChannelStatus *pChanStatus);

/*!
 *  @brief  Function to get the DMA peripheral status
 *
 *  @pre    DMA_open() has been called.
 *
 *  @param  handle          A DMA_Handle returned from DMA_open()
 *
 *  @param  channel         DMA channel.
 *
 *  @param  pDmaStatus      [out] Output block filled by the driver with the peripheral status
 *
 *  @sa     DMA_open()
 */
extern int32_t DMA_getStatus(DMA_Handle handle, uint32_t channel, DMA_Status *pDmaStatus);

/*!
 *  @brief  Function to get the DMA channel completion status based on interrupt type. This API
 *          can be used for polling channel transfer completion when interrupts are not
 *          enabled for that channel by using FTC or BTC as the intType
 *
 *  @pre    DMA_open() has been called and DMA_enableInterrupt()
 *          has not been called for this channel/request type.
 *
 *  @param  handle          A DMA_Handle returned from DMA_open()
 *
 *  @param  channel         DMA channel.
 *
 *  @param  intType         DMA interrupt type
 *
 *  @param  errCode         [out] valid errorCode.
 *
 *  @return 1-if the channel's corresponding intType flag is set.
 *          0-if the channel's corresponding intType flag is not set.
 *
 *  @sa     DMA_open()
 */
extern uint32_t DMA_getChannelInterruptFlag(DMA_Handle handle, uint32_t channel, DMA_IntType intType, int32_t* errCode);

/*!
 *  @brief  Function to clear the DMA channel completion status based on interrupt type. This API
 *          should be called before triggering the channel and using DMA_getChannelInterruptFlag() for polling.
 *
 *  @pre    DMA_open() has been called and DMA_enableInterrupt()
 *          has not been called for this channel/request type.
 *
 *  @param  handle          A DMA_Handle returned from DMA_open()
 *
 *  @param  channel         DMA channel.
 *
 *  @param  intType         DMA interrupt type
 *
 *  @return valid errorCode.
 *
 *  @sa     DMA_open()
 */
extern int32_t DMA_clearChannelInterruptFlag(DMA_Handle handle, uint32_t channel, DMA_IntType intType);


/** @}*/

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_DMA__include */

