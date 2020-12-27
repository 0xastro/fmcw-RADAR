/*
 *  Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/
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
 *
 */

 /**
 *  \file     edma_low_level.h
 *
 *  \brief    This file contains the function prototypes for the device
 *            abstraction layer for EDMA.
 *
 *  \details  Please find the below detailed description of edma dal.
 *            -# Programming sequence for initializing the edma controller
 *               -# Perform the clock configuration of channel controller and
 *                  transfer controllers.
 *               -# To perform initialization of controller use
 *                  -# #EDMA3Init for single CPU interface system.
 *                  -# #EDMAsetRegion for multi CPU interface system.
 *            -# Programming sequence for setting up edma channel.
 *               -# To configure the PaRAM sets use #EDMA3SetPaRAM. For
 *                  custom use
 *               -# To start transfer use #EDMA3EnableTransfer.
 *               -# To stop transfer use #EDMA3DisableTransfer.
 *
 **/

#ifndef EDMA_LOW_LEVEL_H_
#define EDMA_LOW_LEVEL_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <ti/drivers/edma/include/hw_edma_tc.h>
#include <ti/drivers/edma/include/hw_edma_tpcc.h>

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */

/* \brief DMAQNUM bits Clear */
#define EDMA3CC_DMAQNUM_CLR(chNum) \
    (~((uint32_t) 0x7U << (((chNum) % 8U) * 4U)))

/* \brief DMAQNUM bits Set */
#define EDMA3CC_DMAQNUM_SET(chNum, queNum) \
    (((uint32_t) 0x7U & (queNum)) << (((chNum) % 8U) * 4U))

/* \brief QDMAQNUM bits Clear */
#define EDMA3CC_QDMAQNUM_CLR(chNum) \
    (~((uint32_t) 0x7U << ((chNum) * 4U)))

/* \brief QDMAQNUM bits Set
* @{
*/
#define EDMA3CC_QDMAQNUM_SET(chNum, queNum) \
    (((uint32_t) 0x7U & (queNum)) << ((chNum) * 4U))

#define EDMA3CC_QCHMAP_PAENTRY_CLR ((uint32_t) (~EDMA_TPCC_QCHMAPN_PAENTRY_MASK))
/* @} */

/* \brief QCHMAP-PaRAMEntry bitfield Set */
#define EDMA3CC_QCHMAP_PAENTRY_SET(paRAMId)                               \
    (((EDMA_TPCC_QCHMAPN_PAENTRY_MASK >> EDMA_TPCC_QCHMAPN_PAENTRY_SHIFT) \
      & (paRAMId)) << EDMA_TPCC_QCHMAPN_PAENTRY_SHIFT)                    \

/* \brief QCHMAP-TrigWord bitfield Clear */
#define EDMA3CC_QCHMAP_TRWORD_CLR  ((uint32_t) (~EDMA_TPCC_QCHMAPN_TRWORD_MASK))

/* \brief QCHMAP-TrigWord bitfield Set */
#define EDMA3CC_QCHMAP_TRWORD_SET(paRAMId)                                \
    (((EDMA_TPCC_QCHMAPN_TRWORD_MASK >> EDMA_TPCC_QCHMAPN_TRWORD_SHIFT) & \
      (paRAMId)) << EDMA_TPCC_QCHMAPN_TRWORD_SHIFT)

/* \brief OPT-TCC bitfield Clear */
#define EDMA3CC_OPT_TCC_CLR        ((uint32_t) (~EDMA_TPCC_OPT_TCC_MASK))

/* \brief OPT-TCC bitfield Set
* @{
*/
#define EDMA3CC_OPT_TCC_SET(tcc)                                      \
    (((EDMA_TPCC_OPT_TCC_MASK >> EDMA_TPCC_OPT_TCC_SHIFT) & (tcc)) << \
     EDMA_TPCC_OPT_TCC_SHIFT)
#define EDMA3_SET_ALL_BITS                    ((uint32_t) 0xFFFFFFFFU)
#define EDMA3_CLR_ALL_BITS                    ((uint32_t) 0x00000000U)

#define EDMA3CC_COMPL_HANDLER_RETRY_COUNT     ((uint32_t) 10U)
#define EDMA3CC_ERR_HANDLER_RETRY_COUNT       ((uint32_t) 10U)

#define EDMA3_TRIG_MODE_MANUAL                ((uint32_t) 0U)
#define EDMA3_TRIG_MODE_QDMA                  ((uint32_t) 1U)
#define EDMA3_TRIG_MODE_EVENT                 ((uint32_t) 2U)
/* @} */

/* \brief Values that can be used for parameter chType in API's
*  @{
*/
/*! @brief DMA channel type */
#define EDMA3_CHANNEL_TYPE_DMA                ((uint32_t) 0U)

/*! @brief QDMA channel type */
#define EDMA3_CHANNEL_TYPE_QDMA               ((uint32_t) 1U)
/* @} */

/* \brief Values that can be used to specify different event
* status
*  @{
*/
#define EDMA3_XFER_COMPLETE                   ((uint32_t) 0U)
#define EDMA3_CC_DMA_EVT_MISS                 ((uint32_t) 1U)
#define EDMA3_CC_QDMA_EVT_MISS                ((uint32_t) 2U)
/* @} */

/** \defgroup EDMA_TRANSFER_TYPE_DEFS EDMA transfer type definitions
* \brief Values that can be used to specify different
* synchronization events
*  @{
*/
/*! @brief Transfer Type "A" */
#define EDMA3_SYNC_A                          ((uint32_t) 0U)

/*! @brief Transfer Type "AB" */
#define EDMA3_SYNC_AB                         ((uint32_t) 1U)
/* @} */ /* end defgroup EDMA_TRANSFER_TYPE_DEFS */

/** \defgroup EDMA_ADDRESSING_MODE_DEFS EDMA addressing modes definitions.
* \brief Values that can be used to specify different
* addressing modes (relevant for SAM and DAM sub-fields in OPT field).
*  @{
*/
/*! @brief Incremental addressing (INCR), not FIFO */
#define EDMA3_ADDRESSING_MODE_LINEAR          ((uint32_t) 0U)

/*! @brief Constant addressing (CONST) within the FIFO array, wraps around upon
    reaching FIFO width */
#define EDMA3_ADDRESSING_MODE_FIFO_WRAP       ((uint32_t) 1U)
/* @} */ /* end defgroup EDMA_ADDRESSING_MODE_DEFS */

/** \defgroup EDMA_FIFO_WIDTH_DEFS EDMA FIFO width definitions.
* \brief Values that can be used to specify different FIFO widths (FWID in OPT field).
*  @{
*/
/*! @brief 8-bit FIFO width */
#define EDMA3_FIFO_WIDTH_8BIT     ((uint32_t) EDMA_TPCC_OPT_FWID_FIFOWIDTH8BIT)

/*! @brief 16-bit FIFO width */
#define EDMA3_FIFO_WIDTH_16BIT    ((uint32_t) EDMA_TPCC_OPT_FWID_FIFOWIDTH16BIT)

/*! @brief 32-bit FIFO width */
#define EDMA3_FIFO_WIDTH_32BIT    ((uint32_t) EDMA_TPCC_OPT_FWID_FIFOWIDTH32BIT)

/*! @brief 64-bit FIFO width */
#define EDMA3_FIFO_WIDTH_64BIT    ((uint32_t) EDMA_TPCC_OPT_FWID_FIFOWIDTH64BIT)

/*! @brief 128-bit FIFO width */
#define EDMA3_FIFO_WIDTH_128BIT   ((uint32_t) EDMA_TPCC_OPT_FWID_FIFOWIDTH128BIT)

/*! @brief 256-bit FIFO width */
#define EDMA3_FIFO_WIDTH_256BIT   ((uint32_t) DMA_TPCC_OPT_FWID_FIFOWIDTH256BIT)
/* @} */ /* end defgroup EDMA_FIFO_WIDTH_DEFS */

/* \brief Values that can be user to Clear any Channel controller Errors
*  @{
*/
#define EDMA3CC_CLR_TCCERR         ((uint32_t) EDMA_TPCC_CCERRCLR_TCERR_MASK)
#define EDMA3CC_CLR_QTHRQ0         ((uint32_t) EDMA_TPCC_CCERRCLR_QTHRXCD0_MASK)
#define EDMA3CC_CLR_QTHRQ1         ((uint32_t) EDMA_TPCC_CCERRCLR_QTHRXCD1_MASK)
/* @} */

/* \brief Values that is used to Chain the two specified channels
*  @{
*/
#define EDMA3_OPT_TCCHEN_MASK      ((uint32_t) EDMA_TPCC_OPT_TCCHEN_MASK)
#define EDMA3_OPT_ITCCHEN_MASK     ((uint32_t) EDMA_TPCC_OPT_ITCCHEN_MASK)
#define EDMA3_OPT_TCINTEN_MASK     ((uint32_t) EDMA_TPCC_OPT_TCINTEN_MASK)
#define EDMA3_OPT_ITCINTEN_MASK    ((uint32_t) EDMA_TPCC_OPT_ITCINTEN_MASK)

#define EDMA3_OPT_TCC_MASK         ((uint32_t) EDMA_TPCC_OPT_TCC_MASK)
#define EDMA3_OPT_TCC_SHIFT        ((uint32_t) EDMA_TPCC_OPT_TCC_SHIFT)
/* @} */

/* \brief paRAMEntry Fields
* @{
*/
/* \brief The OPT field (Offset Address 0x0 Bytes) */
#define    EDMA3CC_PARAM_ENTRY_OPT            ((uint32_t) 0x0U)

/* \brief The SRC field (Offset Address 0x4 Bytes)*/
#define    EDMA3CC_PARAM_ENTRY_SRC            ((uint32_t) 0x1U)

/* \brief The (ACNT+BCNT) field (Offset Address 0x8 Bytes)*/
#define    EDMA3CC_PARAM_ENTRY_ACNT_BCNT      ((uint32_t) 0x2U)

/* \brief The DST field (Offset Address 0xC Bytes)*/
#define    EDMA3CC_PARAM_ENTRY_DST            ((uint32_t) 0x3U)

/* \brief The (SRCBIDX+DSTBIDX) field (Offset Address 0x10 Bytes)*/
#define    EDMA3CC_PARAM_ENTRY_SRC_DST_BIDX   ((uint32_t) 0x4U)

/* \brief The (LINK+BCNTRLD) field (Offset Address 0x14 Bytes)*/
#define    EDMA3CC_PARAM_ENTRY_LINK_BCNTRLD   ((uint32_t) 0x5U)

/* \brief The (SRCCIDX+DSTCIDX) field (Offset Address 0x18 Bytes)*/
#define    EDMA3CC_PARAM_ENTRY_SRC_DST_CIDX   ((uint32_t) 0x6U)

/* \brief The (CCNT+RSVD) field (Offset Address 0x1C Bytes)*/
#define    EDMA3CC_PARAM_ENTRY_CCNT           ((uint32_t) 0x7U)

/* \brief The offset for each PaRAM Entry field*/
#define    EDMA3CC_PARAM_FIELD_OFFSET         ((uint32_t) 0x4U)

/* \brief Number of PaRAM Entry fields
* OPT, SRC, A_B_CNT, DST, SRC_DST_BIDX, LINK_BCNTRLD, SRC_DST_CIDX
* and CCNT
* @{
*/
#define    EDMA3CC_PARAM_ENTRY_FIELDS         ((uint32_t) 0x8U)

#define SOC_EDMA3_NUM_QDMACH                ((uint32_t) 8U)

#define SOC_EDMA3_NUM_DMACH                 ((uint32_t) 64U)
/* @} */
/* @} */

/* ========================================================================== */
/*                         Structures                                         */
/* ========================================================================== */
/**
 * \brief EDMA3 Parameter RAM Set in User Configurable format
 *
 * This is a mapping of the EDMA3 PaRAM set provided to the user
 * for ease of modification of the individual fields
 */
typedef struct {
    /** OPT field of PaRAM Set */
    uint32_t opt;

    /**
     * \brief Starting byte address of Source
     * For FIFO mode, srcAddr must be a 256-bit aligned address.
     */
    uint32_t srcAddr;

    /**
     * \brief Number of bytes in each Array (ACNT)
     */
    uint16_t aCnt;

    /**
     * \brief Number of Arrays in each Frame (BCNT)
     */
    uint16_t bCnt;

    /**
     * \brief Starting byte address of destination
     * For FIFO mode, destAddr must be a 256-bit aligned address.
     * i.e. 5 LSBs should be 0.
     */
    uint32_t destAddr;

    /**
     * \brief Index between consec. arrays of a Source Frame (SRCBIDX)
     */
    int16_t  srcBIdx;

    /**
     * \brief Index between consec. arrays of a Destination Frame (DSTBIDX)
     */
    int16_t  destBIdx;

    /**
     * \brief Address for linking (AutoReloading of a PaRAM Set)
     * This must point to a valid aligned 32-byte PaRAM set
     * A value of 0xFFFF means no linking
     */
    uint16_t linkAddr;

    /**
     * \brief Reload value of the numArrInFrame (BCNT)
     * Relevant only for A-sync transfers
     */
    uint16_t bCntReload;

    /**
     * \brief Index between consecutive frames of a Source Block (SRCCIDX)
     */
    int16_t  srcCIdx;

    /**
     * \brief Index between consecutive frames of a Dest Block (DSTCIDX)
     */
    int16_t  destCIdx;

    /**
     * \brief Number of Frames in a block (CCNT)
     */
    uint16_t cCnt;
}EDMA3CCPaRAMEntry;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief   EDMA3 Initialization
 *
 *  This function initializes the EDMA3 Driver
 *  Clears the error specific registers (EMCR/EMCRh, QEMCR, CCERRCLR) &
 *  initialize the Queue Number Registers
 *
 *  \param  baseAddr                  Memory address of the EDMA instance used.\n
 *
 *  \param  queNum                   Event Queue Number to which the channel
 *                                   will be mapped (valid only for the
 *                                   Master Channel (DMA/QDMA) request).\n
 *
 *  \return None
 *
 *  \note   The regionId is the shadow region(0 or 1) used and the,
 *          Event Queue used is either (0 or 1). There are only four shadow
 *          regions and only two event Queues
 */
void EDMA3Init(uint32_t baseAddr, uint32_t queNum);

/**
 * \brief   This API sets the region.
 *
 * \param   i pass the regionId.\n
 *
 **/
void EDMAsetRegion(uint32_t i);

/**
 * \brief  Enable channel to Shadow region mapping
 *
 * This API allocates DMA/QDMA channels or TCCs, and the same resources are
 * enabled in the shadow region specific register (DRAE/DRAEH/QRAE).
 * Here only one shadow region is used since, there is only one Master.
 *
 *  \param   baseAddr     Memory address of the EDMA instance used.\n
 *
 *  \param   chType      (DMA/QDMA) Channel
 *                        For Example: For DMA it is,
 *                        EDMA3_CHANNEL_TYPE_DMA.\n
 *
 *  \param   chNum       Allocated channel number.\n
 *
 *
 *  chType can have values
 *        EDMA3_CHANNEL_TYPE_DMA\n
 *        EDMA3_CHANNEL_TYPE_QDMA
 *
 *  \return  None
 */
void EDMA3EnableChInShadowReg(uint32_t baseAddr,
                              uint32_t chType,
                              uint32_t chNum);

/**
 * \brief  Disable channel to Shadow region mapping
 *
 * This API allocates DMA/QDMA channels or TCCs, and the same resources are
 * enabled in the shadow region specific register (DRAE/DRAEH/QRAE).
 * Here only one shadow region is used since, there is only one Master.
 *
 * \param   baseAddr   Memory address of the EDMA instance used.\n
 *
 * \param   chType    (DMA/QDMA) Channel
 *
 * \param   chNum      Allocated channel number.\n
 *
 *
 *  chType can have values
 *        EDMA3_CHANNEL_TYPE_DMA\n
 *        EDMA3_CHANNEL_TYPE_QDMA
 *
 *  \return  None
 */
void EDMA3DisableChInShadowReg(uint32_t baseAddr,
                               uint32_t chType,
                               uint32_t chNum);

/**
 *  \brief   This function maps DMA channel to any of the PaRAM sets
 *           in the PaRAM memory map.
 *
 *  \param   baseAddr   Memory address of the EDMA instance used.
 *
 *  \param   channel   The DMA channel number required to be mapped.
 *
 *  \param   paramSet  It specifies the paramSet to which DMA channel
 *                     required to be mapped.
 *
 *  \return  None
 */
void EDMA3ChannelToParamMap(uint32_t baseAddr,
                            uint32_t channel,
                            uint32_t paramSet);

/**
 *  \brief  Map channel to Event Queue
 *
 *  This API maps DMA/QDMA channels to the Event Queue
 *
 *  \param  baseAddr    Memory address of the EDMA instance used.\n
 *
 *  \param  chType     (DMA/QDMA) Channel
 *                     For Example: For QDMA it is
 *                     EDMA3_CHANNEL_TYPE_QDMA.\n
 *
 *  \param  chNum      Allocated channel number.\n
 *
 *  \param  evtQNum    Event Queue Number to which the channel
 *                     will be mapped (valid only for the
 *                     Master Channel (DMA/QDMA) request).\n
 *
 *  chtype can have values
 *        EDMA3_CHANNEL_TYPE_DMA\n
 *        EDMA3_CHANNEL_TYPE_QDMA
 *
 *  \return  None
 */
void EDMA3MapChToEvtQ(uint32_t baseAddr,
                      uint32_t chType,
                      uint32_t chNum,
                      uint32_t evtQNum);

/**
 *  \brief  Remove Mapping of channel to Event Queue
 *
 *  This API Unmaps DMA/QDMA channels to the Event Queue allocated
 *
 *  \param  baseAddr    Memory address of the EDMA instance used.\n
 *
 *  \param  chType     (DMA/QDMA) Channel
 *                     For Example: For DMA it is
 *                     EDMA3_CHANNEL_TYPE_DMA.\n
 *
 *  \param  chNum      Allocated channel number.\n
 *
 *  chtype can have values
 *        EDMA3_CHANNEL_TYPE_DMA\n
 *        EDMA3_CHANNEL_TYPE_QDMA
 *
 *  \return  None
 */
void EDMA3UnmapChToEvtQ(uint32_t baseAddr,
                        uint32_t chType,
                        uint32_t chNum);

/**
 *  \brief  Enables the user to map a QDMA channel to PaRAM set
 *          This API Needs to be called before programming the paRAM sets for
 *          the QDMA Channels.Application needs to maitain the paRAMId
 *          provided by this API.This paRAMId is used to set paRAM and get
 *          paRAM. Refer corresponding API's for more details.
 *
 *  \param  baseAddr                  Memory address of the EDMA instance used.\n
 *
 *  \param  chNum                    Allocated QDMA channel number.\n
 *
 *  \param  paRAMId                  PaRAM Id to which the QDMA channel will be
 *                                   mapped to.
 *                                   mapped to.
 *
 *  \return None
 *
 *  Note : The PaRAMId requested must be greater than 32(SOC_EDMA3_NUM_DMACH).
 *         and lesser than SOC_EDMA3_NUM_DMACH + chNum  Because, the first
 *         32 PaRAM's are directly mapped to first 32 DMA channels and (32 - 38)
 *         for QDMA Channels. (32 - 38) is assigned by driver in this API.
 *
 */
void EDMA3MapQdmaChToPaRAM(uint32_t        baseAddr,
                           uint32_t        chNum,
                           const uint32_t *paRAMId);

/**
 * \brief  Assign a Trigger Word to the specified QDMA channel
 *
 * This API sets the Trigger word for the specific QDMA channel in the QCHMAP
 * Register. Default QDMA trigger word is CCNT.
 *
 * \param  baseAddr             Memory address of the EDMA instance used.\n
 *
 * \param  chNum               QDMA Channel which needs to be assigned
 *                             the Trigger Word
 *
 * \param  trigWord            The Trigger Word for the QDMA channel.
 *                             Trigger Word is the word in the PaRAM
 *                             Register Set which, when written to by CPU,
 *                             will start the QDMA transfer automatically.
 *
 * \return  None
 */
void EDMA3SetQdmaTrigWord(uint32_t baseAddr,
                          uint32_t chNum,
                          uint32_t trigWord);

/**
 *  \brief   Enables the user to Clear any missed event
 *
 *  \param   baseAddr                Memory address of the EDMA instance used.\n
 *
 *  \param   chNum                  Allocated channel number.\n
 *
 *  \return  None
 */
void EDMA3ClrMissEvt(uint32_t baseAddr, uint32_t chNum);

/**
 *  \brief   Enables the user to Clear any QDMA missed event
 *
 *
 *  \param   baseAddr                Memory address of the EDMA instance used.\n
 *
 *  \param   chNum                  Allocated channel number.\n
 *
 *
 *  \return  None
 */
void EDMA3QdmaClrMissEvt(uint32_t baseAddr, uint32_t chNum);

/**
 *  \brief   Enables the user to Clear any Channel controller Errors
 *
 *  \param   baseAddr              Memory address of the EDMA instance used.\n
 *
 *  \param   flags                Masks to be passed.\n
 *
 *  flags can have values:
 *
 *  EDMA3CC_CLR_TCCERR            Clears the TCCERR bit in the EDMA3CC
 *                                ERR Reg\n
 *  EDMA3CC_CLR_QTHRQ0            Queue threshold error clear for queue 0.\n
 *  EDMA3CC_CLR_QTHRQ1            Queue threshold error clear for queue 1.
 *
 *  \return  None
 */
void EDMA3ClrCCErr(uint32_t baseAddr, uint32_t flags);

/**
 *  \brief   Enables the user to Set an event. This API helps user to manually
 *           set events to initiate DMA transfer requests.
 *
 *  \param   baseAddr                Memory address of the EDMA instance used.\n
 *
 *  \param   chNum                  Allocated channel number.\n
 *
 *  \return  None
 *
 *  Note :   This API is generally used during Manual transfers.\n
 */
void EDMA3SetEvt(uint32_t baseAddr, uint32_t chNum);

/**
 *  \brief   Enables the user to Clear an event.
 *
 *  \param   baseAddr                Memory address of the EDMA instance used.\n
 *
 *  \param   chNum                  Allocated channel number.\n
 *
 *  \return  None
 *
 *  Note :   This API is generally used during Manual transfers.\n
 */
void EDMA3ClrEvt(uint32_t baseAddr, uint32_t chNum);

/**
 *  \brief   Enables the user to enable an DMA event.
 *
 *  \param   baseAddr                Memory address of the EDMA instance used.\n
 *
 *  \param   chNum                  Allocated channel number.\n
 *
 *  \return  None
 *
 *  Note :   Writes of 1 to the bits in EESR sets the corresponding event
 *           bits in EER. This is generally used for Event Based transfers.\n
 */
void EDMA3EnableDmaEvt(uint32_t baseAddr, uint32_t chNum);

/**
 *  \brief   Enables the user to Disable an DMA event.
 *
 *  \param   baseAddr                Memory address of the EDMA instance used.\n
 *
 *  \param   chNum                  Allocated channel number.\n
 *
 **
 *  \return  None
 *
 *  Note :   Writes of 1 to the bits in EECR clear the corresponding event bits
 *           in EER; writes of 0 have no effect.. This is generally used for
 *           Event Based transfers.\n
 */
void EDMA3DisableDmaEvt(uint32_t baseAddr, uint32_t chNum);

/**
 *  \brief   Enables the user to enable an QDMA event.
 *
 *  \param   baseAddr                Memory address of the EDMA instance used.\n
 *
 *  \param   chNum                  Allocated channel number.\n
 *
 *  \return  None
 *
 *  Note :   Writes of 1 to the bits in QEESR sets the corresponding event
 *            bits in QEER.\n
 */
void EDMA3EnableQdmaEvt(uint32_t baseAddr, uint32_t chNum);

/**
 *  \brief   Enables the user to disable an QDMA event.
 *
 *  \param   baseAddr                Memory address of the EDMA instance used.\n
 *
 *  \param   chNum                  Allocated channel number.\n
 *
 *  \return  None
 *
 *  Note :   Writes of 1 to the bits in QEECR clears the corresponding event
 *            bits in QEER.\n
 */
void EDMA3DisableQdmaEvt(uint32_t baseAddr, uint32_t chNum);

/**
 *  \brief   This function returns interrupts status of those events
 *           which is less than 32.
 *
 *  \param   baseAddr                Memory address of the EDMA instance used.\n
 *
 **/
uint32_t EDMA3GetIntrStatus(uint32_t baseAddr);

/**
 *  \brief   Enables the user to enable the transfer completion interrupt
 *           generation by the EDMA3CC for all DMA/QDMA channels.
 *
 *  \param   baseAddr                Memory address of the EDMA instance used.\n
 *
 *  \param   chNum                  Allocated channel number.\n
 *
 *  \return  None
 *
 *  Note :   To set any interrupt bit in IER, a 1 must be written to the
 *           corresponding interrupt bit in the interrupt enable set register.
 */
void EDMA3EnableEvtIntr(uint32_t baseAddr, uint32_t chNum);

/**
 *  \brief   Enables the user to clear CC interrupts
 *
 *  \param   baseAddr                Memory address of the EDMA instance used.\n
 *
 *  \param   chNum                  Allocated channel number.\n
 *
 *  \return  None
 *
 *  Note :   Writes of 1 to the bits in IECR clear the corresponding interrupt
 *           bits in the interrupt enable registers (IER); writes of 0 have
 *           no effect.\n
 */
void EDMA3DisableEvtIntr(uint32_t baseAddr, uint32_t chNum);

/**
 *  \brief   Enables the user to Clear an Interrupt.
 *
 *  \param   baseAddr                Memory address of the EDMA instance used.
 *
 *  \param   value                  Value to be set to clear the Interrupt
 *                                  Status.
 *
 *  \return  None
 *
 */
void EDMA3ClrIntr(uint32_t baseAddr, uint32_t value);

/**
 *  \brief   Retrieve existing PaRAM set associated with specified logical
 *           channel (DMA/Link).
 *
 *  \param   baseAddr                Memory address of the EDMA instance used.\n
 *
 *  \param   paRAMId                paRAMset ID whose parameter set is
 *                                  requested.\n
 *
 *  \param   currPaRAM              User gets the existing PaRAM here.\n
 *
 *
 *  \return  None
 *
 */
void EDMA3GetPaRAM(uint32_t           baseAddr,
                   uint32_t           paRAMId,
                   EDMA3CCPaRAMEntry *currPaRAM);

/**
 * \brief   Retrieve existing PaRAM set associated with specified logical
 *          channel (QDMA).
 *
 * \param   baseAddr                Memory address of the EDMA instance used.\n
 *
 * \param   paRAMId                paRAMset ID whose parameter set is
 *                                 requested.\n
 *
 * \param   currPaRAM              User gets the existing PaRAM here.\n
 *
 *  \return  None
 *
 */
void EDMA3QdmaGetPaRAM(uint32_t           baseAddr,
                       uint32_t           paRAMId,
                       EDMA3CCPaRAMEntry *currPaRAM);

/**
 * \brief   Copy the user specified PaRAM Set onto the PaRAM Set associated
 *          with the logical channel (DMA/Link).
 *
 * This API takes a PaRAM Set as input and copies it onto the actual PaRAM Set
 * associated with the logical channel. OPT field of the PaRAM Set is written
 * first and the CCNT field is written last.
 *
 *
 * \param   baseAddr                Memory address of the EDMA instance used.\n
 *
 * \param   paRAMId                paRAMset ID whose parameter set has to be
 *                                 updated
 *
 * \param   newPaRAM               Parameter RAM set to be copied onto existing
 *                                 PaRAM.\n
 *
 *  \return  None
 *
 */
void EDMA3SetPaRAM(uint32_t           baseAddr,
                   uint32_t           paRAMId,
                   EDMA3CCPaRAMEntry *newPaRAM);

/**
 * \brief   Copy the user specified PaRAM Set onto the PaRAM Set associated
 *          with the logical channel (QDMA only).
 *
 * This API takes a PaRAM Set as input and copies it onto the actual PaRAM Set
 * associated with the logical channel. OPT field of the PaRAM Set is written
 * first and the CCNT field is written last.
 *
 *
 * \param   baseAddr                Memory address of the EDMA instance used.\n
 *
 *
 * \param  paRAMId                 paRaMset ID whose parameter set has to be
 *                                 updated
 *
 * \param   newPaRAM               Parameter RAM set to be copied onto existing
 *                                 PaRAM.\n
 *
 *  \return  None
 *
 */
void EDMA3QdmaSetPaRAM(uint32_t           baseAddr,
                       uint32_t           paRAMId,
                       EDMA3CCPaRAMEntry *newPaRAM);

/**
 * \brief   Set a particular PaRAM set entry of the specified PaRAM set
 *
 * \param   baseAddr           Memory address of the EDMA instance used.\n
 *
 * \param   paRAMId           PaRAM Id to which the QDMA channel is
 *                            mapped to.
 *
 * \param   paRAMEntry        Specify the PaRAM set entry which needs
 *                            to be set.
 *
 * \param   newPaRAMEntryVal  The new field setting. Make sure this field is
 *                            packed for setting certain fields in paRAM.
 *
 *  EDMA3CC_PARAM_ENTRY_OPT
 *  EDMA3CC_PARAM_ENTRY_SRC
 *  EDMA3CC_PARAM_ENTRY_ACNT_BCNT
 *  EDMA3CC_PARAM_ENTRY_DST
 *  EDMA3CC_PARAM_ENTRY_SRC_DST_BIDX
 *  EDMA3CC_PARAM_ENTRY_LINK_BCNTRLD
 *  EDMA3CC_PARAM_ENTRY_SRC_DST_CIDX
 *  EDMA3CC_PARAM_ENTRY_CCNT
 *
 * \return  None
 *
 * \note    This API should be used while setting the PaRAM set entry
 *          for QDMA channels. If EDMA3QdmaSetPaRAMEntry() used,
 *          it will trigger the QDMA channel before complete
 *          PaRAM set entry is written.
 */
void EDMA3QdmaSetPaRAMEntry(uint32_t baseAddr,
                            uint32_t paRAMId,
                            uint32_t paRAMEntry,
                            uint32_t newPaRAMEntryVal);

/**
 * \brief   Get a particular PaRAM entry of the specified PaRAM set
 *
 * \param   baseAddr           Memory address of the EDMA instance used.\n
 *
 * \param   paRAMId           PaRAM Id to which the QDMA channel is
 *                            mapped to.
 *
 * \param   paRAMEntry        Specify the PaRAM set entry which needs
 *                            to be read.
 *
 *  paRAMEntry can have values:
 *
 *  EDMA3CC_PARAM_ENTRY_OPT
 *  EDMA3CC_PARAM_ENTRY_SRC
 *  EDMA3CC_PARAM_ENTRY_ACNT_BCNT
 *  EDMA3CC_PARAM_ENTRY_DST
 *  EDMA3CC_PARAM_ENTRY_SRC_DST_BIDX
 *  EDMA3CC_PARAM_ENTRY_LINK_BCNTRLD
 *  EDMA3CC_PARAM_ENTRY_SRC_DST_CIDX
 *  EDMA3CC_PARAM_ENTRY_CCNT
 *
 * \return  paRAMEntryVal     The value of the paRAM field pointed by the
 *                            paRAMEntry.
 *
 * \note    This API should be used while reading the PaRAM set entry
 *          for QDMA channels. And the paRAMEntryVal is a packed value for
 *          certain fields of paRAMEntry.The user has to make sure the value
 *          is unpacked appropriately.
 *          For example, the third field is A_B_CNT. Hence he will have to
 *          unpack it to two 16 bit fields to get ACNT and BCNT.
 */
uint32_t EDMA3QdmaGetPaRAMEntry(uint32_t baseAddr,
                                uint32_t paRAMId,
                                uint32_t paRAMEntry);

/**
 *  \brief Request a DMA/QDMA/Link channel.
 *
 *  Each channel (DMA/QDMA/Link) must be requested  before initiating a DMA
 *  transfer on that channel.
 *
 *  This API is used to allocate a logical channel (DMA/QDMA/Link) along with
 *  the associated resources. For DMA and QDMA channels, TCC and PaRAM Set are
 *  also allocated along with the requested channel.
 *
 *  User can request a specific logical channel by passing the channel number
 *  in 'chNum'.
 *
 *  For DMA/QDMA channels, after allocating all the EDMA3 resources, this API
 *  sets the TCC field of the OPT PaRAM Word with the allocated TCC. It also
 *  sets the event queue for the channel allocated. The event queue needs to
 *  be specified by the user.
 *
 *  For DMA channel, it also sets the DCHMAP register.
 *
 *  For QDMA channel, it sets the QCHMAP register and CCNT as trigger word and
 *  enables the QDMA channel by writing to the QEESR register.
 *
 *  \param  baseAddr                  Memory address of the EDMA instance used.\n
 *
 *  \param  chType                   (DMA/QDMA) Channel
 *                                    For Example: For DMA it is
 *                                    EDMA3_CHANNEL_TYPE_DMA.\n
 *
 *  \param  chNum                    This is the channel number requested for a
 *                                   particular event.\n
 *
 *  \param  tccNum                   The channel number on which the
 *                                   completion/error interrupt is generated.
 *                                   Not used if user requested for a Link
 *                                   channel.\n
 *
 *  \param  evtQNum                  Event Queue Number to which the channel
 *                                   will be mapped (valid only for the
 *                                   Master Channel (DMA/QDMA) request).\n
 *
 *  \return  TRUE if parameters are valid, else FALSE
 */
uint32_t EDMA3RequestChannel(uint32_t baseAddr,
                             uint32_t chType,
                             uint32_t chNum,
                             uint32_t tccNum,
                             uint32_t evtQNum);

/**
 *  \brief    Free the specified channel (DMA/QDMA/Link) and its associated
 *            resources (PaRAM Set, TCC etc) and removes various mappings.
 *
 *  For Link channels, this API only frees the associated PaRAM Set.
 *
 *  For DMA/QDMA channels, it does the following operations:
 *  1) Disable any ongoing transfer on the channel,\n
 *  2) Remove the channel to Event Queue mapping,\n
 *  3) For DMA channels, clear the DCHMAP register, if available\n
 *  4) For QDMA channels, clear the QCHMAP register,\n
 *  5) Frees the DMA/QDMA channel in the end.\n
 *
 *  \param  baseAddr                  Memory address of the EDMA instance used.\n
 *
 *  \param  chType              (DMA/QDMA) Channel
 *                     For Example: For QDMA it is,
 *                     EDMA3_CHANNEL_TYPE_QDMA.\n
 *
 *  \param  chNum                    This is the channel number requested for a
 *                      particular event.\n
 *
 *  \param  trigMode                 Mode of triggering start of transfer.\n
 *
 *  \param  tccNum                   The channel number on which the
 *                                   completion/error interrupt is generated.
 *                                   Not used if user requested for a Link
 *                                   channel.\n
 *
 *  \param  evtQNum                  Event Queue Number to which the channel
 *                                   will be unmapped (valid only for the
 *                                   Master Channel (DMA/QDMA) request).\n
 *
 *  trigMode can have values:
 *        EDMA3_TRIG_MODE_MANUAL\n
 *        EDMA3_TRIG_MODE_QDMA\n
 *        EDMA3_TRIG_MODE_EVENT
 *
 *  \return  TRUE if parameters are valid else return FALSE
 */
uint32_t EDMA3FreeChannel(uint32_t baseAddr,
                          uint32_t chType,
                          uint32_t chNum,
                          uint32_t trigMode,
                          uint32_t tccNum,
                          uint32_t evtQNum);

/**
 *  \brief    Start EDMA transfer on the specified channel.
 *
 *  There are multiple ways to trigger an EDMA3 transfer. The triggering mode
 *  option allows choosing from the available triggering modes: Event,
 *  Manual or QDMA.
 *
 *  In event triggered, a peripheral or an externally generated event triggers
 *  the transfer. This API clears the Event and Event Miss Register and then
 *  enables the DMA channel by writing to the EESR.
 *
 *  In manual triggered mode, CPU manually triggers a transfer by writing a 1
 *  in the Event Set Register ESR. This API writes to the ESR to start the
 *  transfer.
 *
 *  In QDMA triggered mode, a QDMA transfer is triggered when a CPU (or other
 *  EDMA3 programmer) writes to the trigger word of the QDMA channel PaRAM set
 *  (auto-triggered) or when the EDMA3CC performs a link update on a PaRAM set
 *  that has been mapped to a QDMA channel (link triggered). This API enables
 *  the QDMA channel by writing to the QEESR register.
 *
 *  \param  baseAddr         Memory address of the EDMA instance used.\n
 *
 *  \param  chNum           Channel being used to enable transfer.\n
 *
 *  \param  trigMode        Mode of triggering start of transfer (Manual,
 *                          QDMA or Event).\n
 *
 *  trigMode can have values:
 *        EDMA3_TRIG_MODE_MANUAL\n
 *        EDMA3_TRIG_MODE_QDMA\n
 *        EDMA3_TRIG_MODE_EVENT\n
 *
 *  \return  retVal         TRUE or FALSE depending on the param passed.\n
 *
 */
uint32_t EDMA3EnableTransfer(uint32_t baseAddr,
                             uint32_t chNum,
                             uint32_t trigMode);

/**
 *  \brief   Disable DMA transfer on the specified channel
 *
 *  There are multiple ways by which an EDMA3 transfer could be triggered.
 *  The triggering mode option allows choosing from the available triggering
 *  modes.
 *
 *  To disable a channel which was previously triggered in manual mode,
 *  this API clears the Secondary Event Register and Event Miss Register,
 *  if set, for the specific DMA channel.
 *
 *  To disable a channel which was previously triggered in QDMA mode, this
 *  API clears the QDMA Event Enable Register, for the specific QDMA channel.
 *
 *  To disable a channel which was previously triggered in event mode, this API
 *  clears the Event Enable Register, Event Register, Secondary Event Register
 *  and Event Miss Register, if set, for the specific DMA channel.
 *
 *
 *  \param  baseAddr         Memory address of the EDMA instance used.\n
 *
 *  \param  chNum           Channel being used to enable transfer.\n
 *
 *  \param  trigMode        Mode of triggering start of transfer (Manual,
 *                          QDMA or Event).\n
 *  \return  retVal         TRUE or FALSE depending on the param passed.\n
 *
 */
uint32_t EDMA3DisableTransfer(uint32_t baseAddr,
                              uint32_t chNum,
                              uint32_t trigMode);

/**
 *  \brief  Clears Event Register and Error Register for a specific
 *          DMA channel and brings back EDMA3 to its initial state.
 *
 *  This API clears the Event register, Event Miss register, Event Enable
 *  register for a specific DMA channel. It also clears the CC Error register.
 *
 *  \param  baseAddr         Memory address of the EDMA instance used.\n
 *
 *  \param  chNum           This is the channel number requested for a
 *                          particular event.\n
 *
 *  \param  evtQNum         Event Queue Number to which the channel
 *                          will be unmapped (valid only for the
 *                          Master Channel (DMA/QDMA) request).\n
 *
 *  \return none.\n
 */
void EDMA3ClearErrorBits(uint32_t baseAddr,
                         uint32_t chNum,
                         uint32_t evtQNum);

/**
 *  \brief   This returns EDMA3 CC error status.
 *
 *  \param   baseAddr                Memory address of the EDMA instance used.\n
 *
 *  \return  value                  Status of the Interrupt Pending Register
 *
 */
uint32_t EDMA3GetCCErrStatus(uint32_t baseAddr);

/**
 *  \brief   This returns error interrupt status for those events whose
 *           event number is less than 32.
 *
 *  \param   baseAddr                Memory address of the EDMA instance used.\n
 *
 *  \return  value                  Status of the Interrupt Pending Register
 *
 */
uint32_t EDMA3GetErrIntrStatus(uint32_t baseAddr);

/**
 *  \brief   This returns QDMA error interrupt status.
 *
 *  \param   baseAddr            Memory address of the EDMA instance used.\n
 *
 *  \return  value              Status of the QDMA Interrupt Pending Register
 *
 */
uint32_t EDMA3QdmaGetErrIntrStatus(uint32_t baseAddr);

/**
 *  \brief   EDMA3 Deinitialization
 *
 *  This function deinitializes the EDMA3 Driver
 *  Clears the error specific registers (EMCR/EMCRh, QEMCR, CCERRCLR) &
 *  deinitialize the Queue Number Registers
 *
 *  \param  baseAddr         Memory address of the EDMA instance used.\n
 *
 *  \param  queNum          Event Queue used
 *  \return  None
 *
 *  \note     The regionId is the shadow region(0 or 1) used and the,
 *            Event Queue used is either (0 or 1). There are only two shadow
 *            regions and only two event Queues
 */
void EDMA3Deinit(uint32_t baseAddr, uint32_t queNum);

/** Bug fix removed this function **/
uint32_t EDMAVersionGet(void);

/**
 * \brief   This API return the revision Id of the peripheral.
 *
 * \param   baseAddr     Memory address of the EDMA instance used.\n
 *
 **/
uint32_t EDMA3PeripheralIdGet(uint32_t baseAddr);

/**
 *  \brief   This function returns interrupts status of those events
 *           which is greater than 32.
 *
 *  \param   baseAddr                Memory address of the EDMA instance used.\n
 *
 **/
uint32_t EDMA3IntrStatusHighGet(uint32_t baseAddr);

/**
 *  \brief   This returns error interrupt status for those events whose
 *           event number is greater than 32.
 *
 *  \param   baseAddr                Memory address of the EDMA instance used.\n
 *  \return  value                  Status of the Interrupt Pending Register
 *
 */
uint32_t EDMA3ErrIntrHighStatusGet(uint32_t baseAddr);

/**
 *  \brief  Chain the two specified channels
 *
 *  This API is used to chain a DMA channel to a previously allocated DMA/QDMA
 *  channel
 *
 *  Chaining is different from Linking. The EDMA3 link feature reloads the
 *  current channel parameter set with the linked parameter set. The EDMA3
 *  chaining feature does not modify or update any channel parameter set;
 *  it provides a synchronization event (or trigger) to the chained DMA channel,
 *  as soon as the transfer (final or intermediate) completes on the main
 *  DMA/QDMA channel.
 *
 *  \param  baseAddr         Memory address of the EDMA instance used.\n
 *
 *  \param  chId1           DMA/QDMA channel to which a particular
 *                          DMA channel will be chained
 *
 *  \param  chId2           DMA channel which needs to be chained to
 *                          the first DMA/QDMA channel.
 *
 *  \param   chainOptions   Options such as intermediate interrupts
 *                          are required or not, intermediate/final
 *                          chaining is enabled or not etc.
 *
 *  \return none.\n
 */
void EDMA3ChainChannel(uint32_t baseAddr,
                       uint32_t chId1,
                       uint32_t chId2,
                       uint32_t chainOptions);

/**
 *  \brief  Link two channels.
 *
 *  This API is used to link two previously allocated logical (DMA/QDMA/Link)
 *  channels.
 *
 *  It sets the Link field of the PaRAM set associated with first
 *  channel (chId1) to point it to the PaRAM set associated with second
 *  channel (chId2).
 *
 *  It also sets the TCC field of PaRAM set of second channel to the
 *  same as that of the first channel.
 *
 *  \param  baseAddr         Memory address of the EDMA instance used.\n
 *
 *  \param  paRAMId1        PaRAM set ID of physical channel1 to which
 *                          particular paRAM set will be linked
 *                          or
 *                          PaRAM set ID in case another PaRAM set is being
 *                          linked to this PaRAM set
 *
 *  \param  paRAMId2        PaRAM set ID which is linked to
 *                          channel with parameter ID paRAMId1
 *
 *                          After the transfer based on the PaRAM set
 *                          of channel1 is over, the PaRAM set paRAMId2 will
 *                          be copied to the PaRAM set of channel1 and
 *                          transfer will resume.
 *                          For DMA channels, another sync event is
 *                          required to initiate the transfer on the
 *                          Link channel.
 *
 *  \return none.\n
 */
void EDMA3LinkChannel(uint32_t baseAddr, uint32_t paRAMId1, uint32_t paRAMId2);

#ifdef __cplusplus
}
#endif
#endif

/********************************* End of file ******************************/

