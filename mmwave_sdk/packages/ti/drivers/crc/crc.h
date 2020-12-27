/**
 *   @file  crc.h
 *
 *   @brief
 *      This is the header file for the CRC driver which exposes the
 *      data structures and exported API which can be used by the
 *      applications to use the CRC driver.
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

/** @mainpage CRC Driver
 *
 *  The CRC Driver allows the calculation of the CRC on a specified data.
 *
 *  The CRC header file should be included in an application as follows:
 *  @code
    #include <ti/drivers/crc.h>
    @endcode
 *
 *  The crc/include/reg_crc.h has the register layer definitons for the
 *  CRC Module.
 *
 * ## Opening the driver #
 * Once the CRC Driver has been initialized; the CRC Driver instance can be opened
 * using the #CRC_open. The helper function #CRC_initConfigParams can be used to
 * populate the default parameters
 *
 * ## Transaction Id #
 * Each driver requires access to a unique transaction identifier before it can
 * start using the driver. Transaction identifiers ensure that if the CRC Driver
 * instance is shared between multiple threads the usage from one thread does not
 * corrupt the usage from another thread.
 *
 * Use the #CRC_getTransactionId to get a unique transaction identifer. On success
 * the driver is marked as owned and belongs to the callee. The function can fail
 * with an error code CRC_EINUSE to indicate the driver belongs to another enrity
 * and cannot be used right now.
 *
 * Transaction identifiers are released on a call to #CRC_getSignature when the computed
 * CRC signature is passed back to the callee *OR* on the #CRC_cancelSignature when
 * the callee is no longer interested in calculating the CRC operation.
 *
 * ## Using the driver #
 *
 * As mentioned above a successful call to #CRC_getTransactionId will return a unique
 * transaction id. Applications can then invoke the #CRC_computeSignature API from 1 to N
 * times passing different data buffers over which the CRC is to be computed. The following
 * pseudo code reflects the API usage:-
 *
 * @code

    // Acquire a unique transaction identifer
    status = CRC_getTransactionId (crcDriverHandle, &transId, ...)
    if (status == 0)
    {
        // Successfully retreived a transaction id. We own the CRC
        // driver now!

        // Perform signature computation for Data Buffer-1
        sigGenCfg.transactionId = transId;
        sigGenCfg.ptrData       = &data_buffer1[0];
        sigGenCfg.dataLen       = 128;
        CRC_computeSignature (crcDriverHandle, )
        ...
        // Perform signature computation for Data Buffer-2
        sigGenCfg.transactionId = transId;
        sigGenCfg.ptrData       = &data_buffer2[0];
        sigGenCfg.dataLen       = 512;
        CRC_computeSignature (crcDriverHandle, )
        ...
        // Get the CRC Signature. This will also release the transaction identifer
        CRC_getSignature (crcDriverHandle, transId, &crc, ...)
    }

   @endcode
 *
 * The results of the CRC signature can be retreived using the #CRC_getSignature API.
 * This will also release the transaction identifier and another user of the CRC Driver
 * can start using the driver now.
 *
 * It is also possible to cancel requests for CRC signature generation by using the
 * #CRC_cancelSignature API. The API will also release the transaction identifer.
 */

/** @defgroup CRC_DRIVER      CRC Driver
 */
#ifndef CRC_DRIVER_H
#define CRC_DRIVER_H

#include <ti/common/mmwave_error.h>

/**
@defgroup CRC_DRIVER_EXTERNAL_FUNCTION            CRC Driver External Functions
@ingroup CRC_DRIVER
@brief
*   The section has a list of all the exported API which the applications need to
*   invoke in order to use the driver
*/
/**
@defgroup CRC_DRIVER_EXTERNAL_DATA_STRUCTURE      CRC Driver External Data Structures
@ingroup CRC_DRIVER
@brief
*   The section has a list of all the data structures which are exposed to the application
*/
/**
@defgroup CRC_DRIVER_ERROR_CODE                   CRC Driver Error Codes
@ingroup CRC_DRIVER
@brief
*   The section has a list of all the error codes which are generated by the CRC Driver
*   module
*/
/**
@defgroup CRC_DRIVER_INTERNAL_FUNCTION            CRC Driver Internal Functions
@ingroup CRC_DRIVER
@brief
*   The section has a list of all internal API which are not exposed to the external
*   applications.
*/
/**
@defgroup CRC_DRIVER_INTERNAL_DATA_STRUCTURE      CRC Driver Internal Data Structures
@ingroup CRC_DRIVER
@brief
*   The section has a list of all internal data structures which are used internally
*   by the CRC module.
*/

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CRC_DRIVER_ERROR_CODE
 *  Base error code for the CRC module is defined in the
 *  \include ti/common/mmwave_error.h
 @{ */

/**
 * @brief   Error Code: Invalid argument
 */
#define CRC_EINVAL                  (MMWAVE_ERRNO_CRC_BASE-1)

/**
 * @brief   Error Code: Out of memory
 */
#define CRC_ENOMEM                  (MMWAVE_ERRNO_CRC_BASE-2)

/**
 * @brief   Error Code: Operation cannot be implemented because the CRC driver
 * is in use
 */
#define CRC_EINUSE                  (MMWAVE_ERRNO_CRC_BASE-3)

/**
@}
*/

/** @addtogroup CRC_DRIVER_EXTERNAL_DATA_STRUCTURE
 @{ */

/**
 * @brief
 *  CRC Driver Handle
 */
typedef void*   CRC_Handle;

/**
 * @brief
 *  CRC Channel
 *
 * @details
 *  Enumeration which describes the CRC channel numbers which are available
 */
typedef enum CRC_Channel_e
{
    /**
     * @brief   Channel-1
     */
    CRC_Channel_CH1 = 0x1,

    /**
     * @brief   Channel-2
     */
    CRC_Channel_CH2
}CRC_Channel;

/**
 * @brief
 *  CRC Operational Mode
 *
 * @details
 *  Enumeration which describes the operational mode.
 */
typedef enum CRC_Operational_Mode_e
{
    /**
     * @brief   CRC channel is operating in FULL CPU Mode. The CPU is
     * responsible for the data pattern tranfers and signature verification
     */
    CRC_Operational_Mode_FULL_CPU = 0x1
}CRC_Operational_Mode;

/**
 * @brief
 *  CRC Type
 *
 * @details
 *  Enumeration which describes the different CRC polynomials which
 *  are supported by the CRC driver.
 */
typedef enum CRC_Type_e
{
    /**
     * @brief   CCITT
     */
    CRC_Type_16BIT  =   0x1,

    /**
     * @brief   Ethernet
     */
    CRC_Type_32BIT,

    /**
     * @brief   ISO 3309
     */
    CRC_Type_64BIT
}CRC_Type;

/**
 * @brief
 *  CRC Data Length
 *
 * @details
 *  Enumeration which describes the data lengths which are supported by
 *  the CRC Driver.
 */
typedef enum CRC_DataLen_e
{
    /**
     * @brief   16-bit
     */
    CRC_DataLen_16_BIT = 1,

    /**
     * @brief   32-bit
     */
    CRC_DataLen_32_BIT,

    /**
     * @brief   64-bit
     */
    CRC_DataLen_64_BIT
}CRC_DataLen;

/**
 * @brief
 *  Bit Swapping
 *
 * @details
 *  Enumeration which describes the supported bit swapping modes
 */
typedef enum CRC_BitSwap_t
{
    /**
     * @brief   Most Significant Bit is first
     */
    CRC_BitSwap_MSB     = 1,

    /**
     * @brief   Least Significant Bit is first
     */
    CRC_BitSwap_LSB
}CRC_BitSwap;

/**
 * @brief
 *  Byte Swapping
 *
 * @details
 *  Enumeration which describes the supported byte swapping modes
 */
typedef enum CRC_ByteSwap_t
{
    /**
     * @brief   Byte swap is disabled
     */
    CRC_ByteSwap_DISABLED     = 1,

    /**
     * @brief   Byte swap is enabled
     */
    CRC_ByteSwap_ENABLED
}CRC_ByteSwap;

/**
 * @brief
 *  CRC Configuration
 *
 * @details
 *  The structure describes the configuration information which is needed
 *  to open the handle to the CRC module.
 */
typedef struct CRC_Config_t
{
    /**
     * @brief   CRC Channel number
     */
    CRC_Channel                 channel;

    /**
     * @brief   Operational Mode
     */
    CRC_Operational_Mode        mode;

    /**
     * @brief   CRC Polynomial type
     */
    CRC_Type                    type;

    /**
     * @brief   CRC Bit Swapping mode
     */
    CRC_BitSwap                 bitSwap;

    /**
     * @brief   CRC Byte Swapping mode
     */
    CRC_ByteSwap                byteSwap;

    /**
     * @brief   Data Length
     */
    CRC_DataLen                 dataLen;
}CRC_Config;

/**
 * @brief
 *  CRC signature generation configuration
 *
 * @details
 *  The structure describes the configuration which needs to be provided
 *  to generate the signature.
 */
typedef struct CRC_SigGenCfg_t
{
    /**
     * @brief   Transaction identifier which has been allocated
     * @sa CRC_getTransactionId
     */
    uint32_t    transactionId;

    /**
     * @brief   Pointer to the data buffer for which the CRC is to be
     * computed
     */
    uint8_t*    ptrData;

    /**
     * @brief   Data buffer length (in bytes)
     */
    uint32_t    dataLen;
}CRC_SigGenCfg;

/**
@}
*/

/*******************************************************************************************************
 * CRC Exported API:
 *******************************************************************************************************/

extern CRC_Handle CRC_open (const CRC_Config* ptrCRCCfg, int32_t* errCode);
extern int32_t    CRC_close (CRC_Handle crcHandle, int32_t* errCode);
extern int32_t    CRC_getTransactionId(CRC_Handle crcHandle, uint32_t* transactionId, int32_t* errCode);
extern int32_t    CRC_computeSignature(CRC_Handle crcHandle, CRC_SigGenCfg* ptrSigGenCfg, int32_t* errCode);
extern int32_t    CRC_getSignature (CRC_Handle crcHandle, uint32_t transactionId, uint64_t* ptrSignature, int32_t* errCode);
extern int32_t    CRC_cancelSignature (CRC_Handle crcHandle, uint32_t transactionId, int32_t* errCode);
extern void       CRC_initConfigParams(CRC_Config* ptrCRCCfg);

#ifdef __cplusplus
}
#endif

#endif /* CRC_DRIVER_H */

