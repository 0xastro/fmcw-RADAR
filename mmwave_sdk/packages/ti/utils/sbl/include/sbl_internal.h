/*!
 *   @file  sbl_internal.h
 *
 *   @brief
 *      This is an internal header file used by the SBL module.
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

#ifndef SBL_INTERNAL_H
#define SBL_INTERNAL_H

#include <ti/drivers/osal/SemaphoreP.h>
#include <ti/drivers/crc/crc.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/uart/UART.h>
#include <ti/drivers/qspiflash/qspiflash.h>

#include <ti/utils/sbl/sbl.h>
#include <ti/utils/sbl/include/buffer_mgmt.h>
#include <ti/utils/sbl/include/image_parser.h>
#include <ti/utils/sbl/include/image_validity.h>
#include <ti/utils/sbl/include/sbl_error.h>
#include <ti/utils/sbl/include/transport.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
@defgroup SBL_INTERNAL_DATA_STRUCTURE      SBL Internal Data Structures
@ingroup SBL
@brief
*   The section has a list of all internal data structures which are used internally
*   by the SBL module.
*/
/**
@defgroup SBL_INTERNAL_FUNCTION             SBL Internal Functions
@ingroup SBL
@brief
*   The section has a list of all internal functions which are used internally
*   by the SBL module.
*/

/*
 * @brief   Macro to check the error status flags have been set.
 */
#define SBL_CHECK_ERROR_STATUS          ((gSblMCB.errorStatus & 0x0000003FFFFFFFFFU) != 0U)

/** @addtogroup SBL_INTERNAL_DATA_STRUCTURE
 @{ */

/**
 * @brief   Size of the vector table
 */
#define SBL_VECTOR_TABLE_SIZE           64U

/*!
 * @brief
 *      SBL Transport related data structure.
 *
 * @details
 *      The structure describes the transport data fields used to download the meta image.
 */
typedef struct SBL_Transport_t
{
    /**
     * @brief   Number of packets received.
     */
    uint32_t        numRxPackets;

    /**
     * @brief   Number of ACKs sent.
     */
    uint32_t        numACKS;

    /**
     * @brief   Number of NACKs sent.
     */
    uint32_t        numNAKS;

    /**
     * @brief   Number of errors detected.
     */
    uint32_t        retransErrors;
}SBL_Transport;

/*!
 * @brief
 *  SBL Master Control Block
 *
 * @details
 *  The structure describes the SBL internal data structures and is used to hold the relevant
 *  information with respect to the SBL module.
 */
typedef struct SBL_MCB_t
{
    /**
     * @brief   Meta header validation status.
     */
    volatile uint8_t        metaHdrAbsent;

    /**
     * @brief   The multicore meta image must have atleast an MSS image present.
     *          MSS image validation status.
     */
    volatile uint8_t        mssImagePresent;

    /*
     * @brief   Track the BSS MPU configuration.
     */
    volatile uint8_t        bssMpuInit;

    /**
     * @brief   Track the error status.
     */
    volatile uint64_t       errorStatus;

    /**
     * @brief   Number of files done parsing.
     */
    uint8_t                 numFilesWritten;

    /**
     * @brief   Track the buffer parser state.
     */
    volatile uint32_t       bufferParserState;

    /**
     * @brief   SOC handle.
     */
    SOC_Handle              socHandle;

    /**
     * @brief   CRC handle.
     */
    CRC_Handle              crcHandle;

    /**
     * @brief   QSPI handle.
     */
    QSPI_Handle             qspiHandle;

    /**
     * @brief   UART handle.
     */
    UART_Handle             uartHandle;

    /**
     * @brief   Semaphore for thread 1.
     */
    SemaphoreP_Handle       threadCommSem1;

    /**
     * @brief   Semaphore for thread 2.
     */
    SemaphoreP_Handle       threadCommSem2;

    /**
     * @brief   Interrupt vector table that is saved and restored.
     */
    uint8_t                 sblIntVecTable[SBL_VECTOR_TABLE_SIZE];

    /**
     * @brief   Internal buffer for holding config data.
     */
    uint8_t                 configFile[SBL_CONFIG_FILE_SIZE];

    /**
     * @brief   Processing is done in chunks of 2k.
     * Variable to track special handling for less than 2k bytes.
     */
    uint8_t                 lessThan2048;

    /**
     * @brief   Pointer to ping buffer.
     */
    uint8_t*                pingAddr;

    /**
     * @brief   Pointer to pong buffer.
     */
    uint8_t*                pongAddr;

    /**
     * @brief   Pointer to current ping/pong buffer in use.
     */
    uint8_t*                currBuff;

    /**
     * @brief   Structure containing all the metaheader details.
     */
    SBL_MetaHeader          metaHeader;

    /**
     * @brief Authentication state machine parameters.
     */
    SBL_AuthProcess         authProcess;

    /**
     * @brief Decryption state machine parameters.
     */
    SBL_DecryptProcess      decryptProcess;

    /**
     * @brief Image parser parameters.
     */
    SBL_ImageParams         imageParams;

    /**
     * @brief Input buffer parameters.
     */
    SBL_InputBuffer         inputBuffers;

    /**
     * @brief   Transport related statistics.
     */
    SBL_Transport           trans;
}SBL_MCB;

/**
 * @brief
 *  Global SBL MCB Pointer.
 */
extern SBL_MCB         gSblMCB;

/**
@}
*/

#ifdef __cplusplus
}
#endif

#endif /* SBL_INTERNAL_H */
