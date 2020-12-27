/**
 *   @file  crc.c
 *
 *   @brief
 *      The file implements the CRC Driver.
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include <stdint.h>
#include <string.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/crc/crc.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/osal/HwiP.h>
#include <ti/drivers/crc/include/reg_crc.h>

/**************************************************************************
 ***************************** Local Structures ***************************
 **************************************************************************/

/**
 * @brief
 *  CRC Driver
 *
 * @details
 *  The structure describes the CRC Driver and tracks all the information
 *  relevant to the driver instance
 */
typedef struct CRC_Driver_t
{
    /**
     * @brief   CRC Configuration which was used to instantiate the driver
     */
    CRC_Config      cfg;

    /**
     * @brief   Pointer to the CRC Register space
     */
    CRCRegs*        ptrCRCRegs;

    /**
     * @brief   Unique free transaction identifier
     */
    uint32_t        freeTransactionId;

    /**
     * @brief   Transaction identifier which is in use
     */
    uint32_t        inUseTransactionId;

    /**
     * @brief   Flag which indicates if the CRC Driver is being used to
     * compute a signature
     */
    uint32_t        inUse;
}CRC_Driver;

/**************************************************************************
 ************************ CRC Driver Local Functions **********************
 **************************************************************************/

static int32_t            CRC_validateParams (const CRC_Config* ptrCRCCfg);
static uint32_t           CRC_getCRCDataSelectBitValue (CRC_DataLen dataLen);
static uint32_t           CRC_getCRCTypeBitValue (CRC_Type crcType);
static volatile uint32_t* CRC_getSignatureRegister (const CRC_Driver* ptrCRCDriver);
static void               CRC_resetSignature (const CRC_Driver* ptrCRCDriver);
static uint32_t           CRC_isTransactionIdValid (CRC_Driver* ptrCRCDriver, uint32_t transactionId);
static void               CRC_releaseTransactionId(CRC_Driver* ptrCRCDriver);

/**************************************************************************
 ***************************** Extern Definitions *************************
 **************************************************************************/

/* Platform specific definition: */
extern CRCRegs* gCRCRegister;

/**************************************************************************
 *************************** CRC Driver Functions *************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      The function is used to validate the CRC configuration
 *
 *  @param[in]  ptrCRCCfg
 *      Pointer to the CRC configuration which is validated
 *
 *  \ingroup CRC_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t CRC_validateParams (const CRC_Config* ptrCRCCfg)
{
    int32_t     retVal = 0;

    /***************************************************************
     * Validate the CRC Channels:
     ***************************************************************/
    switch (ptrCRCCfg->channel)
    {
        case CRC_Channel_CH1:
        case CRC_Channel_CH2:
        {
            /* Valid CRC Channels: */
            break;
        }
        default:
        {
            /* Bad enumeration: */
            retVal = MINUS_ONE;
            break;
        }
    }

    /***************************************************************
     * Validate the CRC Type:
     ***************************************************************/
    switch (ptrCRCCfg->type)
    {
        case CRC_Type_16BIT:
        case CRC_Type_32BIT:
        case CRC_Type_64BIT:
        {
            /* Valid CRC Type: */
            break;
        }
        default:
        {
            /* Bad enumeration: */
            retVal = MINUS_ONE;
            break;
        }
    }

    /***************************************************************
     * Validate the Bit Swap:
     ***************************************************************/
    switch (ptrCRCCfg->bitSwap)
    {
        case CRC_BitSwap_MSB:
        case CRC_BitSwap_LSB:
        {
            /* Valid Bit Swap: */
            break;
        }
        default:
        {
            /* Bad enumeration: */
            retVal = MINUS_ONE;
            break;
        }
    }

    /***************************************************************
     * Validate the Byte Swap:
     ***************************************************************/
    switch (ptrCRCCfg->byteSwap)
    {
        case CRC_ByteSwap_DISABLED:
        case CRC_ByteSwap_ENABLED:
        {
            /* Valid Byte Swap: */
            break;
        }
        default:
        {
            /* Bad enumeration: */
            retVal = MINUS_ONE;
            break;
        }
    }

    /***************************************************************
     * Validate the Data Length:
     ***************************************************************/
    switch (ptrCRCCfg->dataLen)
    {
        case CRC_DataLen_16_BIT:
        case CRC_DataLen_32_BIT:
        case CRC_DataLen_64_BIT:
        {
            /* Valid Data Length: */
            break;
        }
        default:
        {
            /* Bad enumeration: */
            retVal = MINUS_ONE;
            break;
        }
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to map the data length enumeration
 *      into the value which is to be written into the CRC_CTRL0 register. This
 *      is coded as per the CRC register definitions.
 *
 *  @param[in]  dataLen
 *      Data Length to be coded
 *
 *  \ingroup CRC_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Translated bit value
 */
static uint32_t CRC_getCRCDataSelectBitValue (CRC_DataLen dataLen)
{
    uint32_t dataSelectValue = 0;

    switch (dataLen)
    {
        case CRC_DataLen_16_BIT:
        {
            dataSelectValue = 1U;
            break;
        }
        case CRC_DataLen_32_BIT:
        {
            dataSelectValue = 2U;
            break;
        }
        case CRC_DataLen_64_BIT:
        {
            dataSelectValue = 0U;
            break;
        }
        default:
        {
            /* Error: Bad Value */
            DebugP_assert (0);
            break;
        }
    }
    return dataSelectValue;
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to map the CRC Type enumeration
 *      into the value which is to be written into the CRC_CTRL0 register. This
 *      is coded as per the CRC register definitions.
 *
 *  @param[in]  crcType
 *      CRC Type to be coded
 *
 *  \ingroup CRC_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Translated bit value
 */
static uint32_t CRC_getCRCTypeBitValue (CRC_Type crcType)
{
    uint32_t crcTypeValue = 0;

    switch (crcType)
    {
        case CRC_Type_16BIT:
        {
            crcTypeValue = 1U;
            break;
        }
        case CRC_Type_32BIT:
        {
            crcTypeValue = 2U;
            break;
        }
        case CRC_Type_64BIT:
        {
            crcTypeValue = 0U;
            break;
        }
        default:
        {
            /* Error: Bad Value */
            DebugP_assert (0);
            break;
        }
    }
    return crcTypeValue;
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to get the signature register corresponding
 *      to the CRC Channel.
 *
 *  @param[in]  ptrCRCDriver
 *      Pointer to the CRC Driver
 *
 *  \ingroup CRC_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Signature register
 */
static volatile uint32_t* CRC_getSignatureRegister (const CRC_Driver* ptrCRCDriver)
{
    volatile uint32_t*   ptrSignatureRegister;

    /* Which is the channel being used? */
    if (ptrCRCDriver->cfg.channel == CRC_Channel_CH1)
    {
        /* Channel-1: Use the correct register */
        ptrSignatureRegister = (volatile uint32_t*)&ptrCRCDriver->ptrCRCRegs->PSA_SIGREGL1;
    }
    else
    {
        /* Channel-2: Use the correct register */
        ptrSignatureRegister = (volatile uint32_t*)&ptrCRCDriver->ptrCRCRegs->PSA_SIGREGL2;
    }
    return ptrSignatureRegister;
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to reset the signature channel
 *
 *  @param[in]  ptrCRCDriver
 *      Pointer to the CRC Driver
 *
 *  \ingroup CRC_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CRC_resetSignature (const CRC_Driver* ptrCRCDriver)
{
    if (ptrCRCDriver->cfg.channel == CRC_Channel_CH1)
    {
        /* Channel-1: Reset & then bring out of reset */
        ptrCRCDriver->ptrCRCRegs->CRC_CTRL0 = CSL_FINSR (ptrCRCDriver->ptrCRCRegs->CRC_CTRL0, 0U, 0U, 1U);
        ptrCRCDriver->ptrCRCRegs->CRC_CTRL0 = CSL_FINSR (ptrCRCDriver->ptrCRCRegs->CRC_CTRL0, 0U, 0U, 0U);
    }
    else
    {
        /* Channel-2: Reset & then bring out of reset */
        ptrCRCDriver->ptrCRCRegs->CRC_CTRL0 = CSL_FINSR (ptrCRCDriver->ptrCRCRegs->CRC_CTRL0, 8U, 8U, 1U);
        ptrCRCDriver->ptrCRCRegs->CRC_CTRL0 = CSL_FINSR (ptrCRCDriver->ptrCRCRegs->CRC_CTRL0, 8U, 8U, 0U);
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to allocate a unique transaction identifier for
 *      the CRC operation. Transaction identifiers are used to identify the CRC
 *      signature generation procedure since it is possible that the CRC compute
 *      signature API can be invoked multiple times. If there are multiple users
 *      of the same CRC Driver we should ensure that the User1 compute signature
 *      is not interrupted with the User2 request.
 *
 *      The transaction identifers are automatically released once the result
 *      of the signature is requested *OR* if the signature request is cancelled.
 *
 *  @param[in]  crcHandle
 *      CRC Driver Handle
 *  @param[out]  transactionId
 *      Unique Transaction Identifier allocated
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CRC_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t CRC_getTransactionId
(
    CRC_Handle      crcHandle,
    uint32_t*       transactionId,
    int32_t*        errCode
)
{
    CRC_Driver*     ptrCRCDriver;
    uintptr_t       key;
    int32_t         retVal = 0;

    /* Setup the error code: */
    *errCode = 0;

    /* Get the CRC Driver: */
    ptrCRCDriver = (CRC_Driver*)crcHandle;

    /* Sanity Check: Validate the arguments */
    if ((ptrCRCDriver == NULL) || (transactionId == NULL))
    {
        /* Error: Invalid argument */
        *errCode = CRC_EINVAL;
        retVal   = MINUS_ONE;
    }
    else
    {
        /* Critical Section Protection: Transaction Identifer allocation needs to be
         * protected against multiple threads */
        key = HwiP_disable();

        /* Is the driver being used? */
        if (ptrCRCDriver->inUse == 1U)
        {
            /* YES: Driver is being used and it cannot be used. */
            *errCode = CRC_EINUSE;
            retVal   = MINUS_ONE;
        }
        else
        {
            /* NO: Give the unique transaction identifier */
            *transactionId = ptrCRCDriver->freeTransactionId;

            /* Track the transaction identifier which is being used */
            ptrCRCDriver->inUseTransactionId = *transactionId;

            /* Increment the free transaction identifier */
            ptrCRCDriver->freeTransactionId = ptrCRCDriver->freeTransactionId + 1U;

            /* The driver is now being used: */
            ptrCRCDriver->inUse = 1U;
        }

        /* Release the critical section: */
        HwiP_restore(key);

        /* Debug Message: */
        DebugP_log3 ("Debug: CRC Driver %p allocated Transaction Id: %d free Transaction Id: %d\n",
                    (uintptr_t)ptrCRCDriver, (uintptr_t)ptrCRCDriver->inUseTransactionId, (uintptr_t)ptrCRCDriver->freeTransactionId);
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to release a transaction identifier which
 *      had been allocated earlier and is used to mark the driver as
 *      free.
 *
 *  @param[in]  ptrCRCDriver
 *      Pointer to the CRC Driver
 *
 *  \ingroup CRC_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CRC_releaseTransactionId(CRC_Driver* ptrCRCDriver)
{
    uintptr_t   key;

    /* Critical Section Protection: Transaction Identifer release needs to be
     * protected against multiple threads */
    key = HwiP_disable();

    /* Is the driver being used? */
    if (ptrCRCDriver->inUse == 1U)
    {
        /* YES: Display the debug message */
        DebugP_log2 ("Debug: CRC Driver %p releasing Transaction Id: %d\n",
                    (uintptr_t)ptrCRCDriver, (uintptr_t)ptrCRCDriver->inUseTransactionId);

        /* Driver is being used so release it. */
        ptrCRCDriver->inUseTransactionId = 0;

        /* The driver is not being used: */
        ptrCRCDriver->inUse = 0U;
    }

    /* Release the critical section: */
    HwiP_restore(key);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to match the transaction identifer to determine if it is
 *      valid or not. The Transaction identifier is considered to be valid only under
 *      the following conditions:-
 *          - Valid CRC Driver Handle
 *          - CRC Driver is in use
 *          - Transaction Id matches the Driver Active Transaction Id.
 *
 *  @param[in]  ptrCRCDriver
 *      Pointer to the CRC Driver
 *  @param[in]  transactionId
 *      Transaction Id to be validated
 *
 *  \ingroup CRC_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      0   -   Transaction Id is not valid
 *  @retval
 *      1   -   Transaction Id is valid
 */
static uint32_t CRC_isTransactionIdValid (CRC_Driver* ptrCRCDriver, uint32_t transactionId)
{
    uintptr_t       key;
    uint32_t        isValid = 0U;

    /* Do we have a valid driver handle? */
    if (ptrCRCDriver != NULL)
    {
        /* Critical Section Protection: Transaction Identifer access needs to be protected
         * against multiple threads */
        key = HwiP_disable();

        /* Is the driver being used? */
        if (ptrCRCDriver->inUse == 0U)
        {
            /* NO: The driver is NOT being used so the transaction identifier is not valid */
            isValid = 0U;
        }
        else
        {
            /* YES: Driver was being used. Ensure the transaction id matches */
            if (ptrCRCDriver->inUseTransactionId == transactionId)
            {
                /* YES: The CRC Driver was being used by the current user */
                isValid = 1U;
            }
            else
            {
                /* NO: The CRC Driver was being used by another user. */
                isValid = 0U;
            }
        }
        /* Release the critical section: */
        HwiP_restore(key);
    }
    return isValid;
}

/**
 *  @b Description
 *  @n
 *      The function is used to compute the CRC signature for the specified data.
 *      It is possible to invoke the API multiple times in which case the CRC
 *      signature is computed across multiple data buffers.
 *
 *  @param[in]  crcHandle
 *      CRC Driver Handle
 *  @param[in]  ptrSigGenCfg
 *      Pointer to the signature generation configuration
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CRC_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t CRC_computeSignature
(
    CRC_Handle      crcHandle,
    CRC_SigGenCfg*  ptrSigGenCfg,
    int32_t*        errCode
)
{
    CRC_Driver*     ptrCRCDriver;
    uint32_t        index;
    uint32_t        dataAddress;

    /* Setup the error code: */
    *errCode = 0;

    /* Get the pointer to the CRC Driver */
    ptrCRCDriver = (CRC_Driver*)crcHandle;

    /* Sanity Check: Validate the arguments */
    if ((ptrCRCDriver == NULL)              ||
        (ptrSigGenCfg == NULL)              ||
        (ptrSigGenCfg->ptrData == NULL)     ||
        (ptrSigGenCfg->dataLen == 0U)       ||
        (CRC_isTransactionIdValid (ptrCRCDriver, ptrSigGenCfg->transactionId) == 0U))
    {
        /* Error: Invalid arguments */
        *errCode = CRC_EINVAL;
    }
    else
    {
        /* Get the data address for which the CRC is to be calculated: */
        dataAddress = (uint32_t)ptrSigGenCfg->ptrData;

        /* Sanity Check: We need to ensure that the data buffer address is aligned as
         * per the CRC Type. If this is not satisfied; then we can perform CRC over an
         * invalid address; when we are copying the data */
        switch (ptrCRCDriver->cfg.type)
        {
            case CRC_Type_16BIT:
            {
                if ((dataAddress % 2U) != 0U)
                {
                    *errCode = CRC_EINVAL;
                }
                break;
            }
            case CRC_Type_32BIT:
            {
                if ((dataAddress % 4U) != 0U)
                {
                    *errCode = CRC_EINVAL;
                }
                break;
            }
            case CRC_Type_64BIT:
            {
                if ((dataAddress % 8U) != 0U)
                {
                    *errCode = CRC_EINVAL;
                }
                break;
            }
            default:
            {
                /* Error: Invalid configuration type. This should never occur since the parameter
                 * validation has been completed by now. */
                DebugP_assert (0);
                break;
            }
        }

        if (*errCode == 0)
        {
            switch (ptrCRCDriver->cfg.dataLen)
            {
                case CRC_DataLen_16_BIT:
                {
                    volatile uint16_t*  ptr16BitSignature;
                    uint16_t*           ptr16BitData;

                    /* Sanity Check: Ensure that the data length is a multiple of 16 bits.
                     * We need to ensure that we do *not* access the data pointer access
                     * goes beyond the limits */
                    if ((ptrSigGenCfg->dataLen % 2U) != 0U)
                    {
                        /* Error: Invalid data length specified. */
                        *errCode = CRC_EINVAL;
                    }
                    else
                    {
                        /* Initialize and setup the pointers */
                        ptr16BitSignature = (volatile uint16_t*)CRC_getSignatureRegister (ptrCRCDriver);
                        ptr16BitData      = (uint16_t*)ptrSigGenCfg->ptrData;

                        /* Cycle through and write all the data to the registers: */
                        for (index = 0U; index < ptrSigGenCfg->dataLen; index = index + 2U)
                        {
                            /* Write the data to the register */
                            *ptr16BitSignature = *ptr16BitData;

                            /* Increment the data to the next address */
                            ptr16BitData++;
                        }
                    }
                    break;
                }
                case CRC_DataLen_32_BIT:
                {
                    volatile uint32_t*  ptr32BitSignature;
                    uint32_t*           ptr32BitData;

                    /* Sanity Check: Ensure that the data length is a multiple of 32 bits.
                     * We need to ensure that we do *not* access the data pointer access
                     * goes beyond the limits */
                    if ((ptrSigGenCfg->dataLen % 4U) != 0U)
                    {
                        /* Error: Invalid data length specified. */
                        *errCode = CRC_EINVAL;
                    }
                    else
                    {
                        /* Initialize and setup the pointers */
                        ptr32BitSignature = (volatile uint32_t*)CRC_getSignatureRegister (ptrCRCDriver);
                        ptr32BitData      = (uint32_t*)ptrSigGenCfg->ptrData;

                        /* Cycle through and write all the data to the registers: */
                        for (index = 0U; index < ptrSigGenCfg->dataLen; index = index + 4U)
                        {
                            /* Write the data to the register */
                            *ptr32BitSignature = *ptr32BitData;

                            /* Increment the data to the next address */
                            ptr32BitData++;
                        }
                    }
                    break;
                }
                case CRC_DataLen_64_BIT:
                {
                    volatile uint64_t*  ptr64BitSignature;
                    uint64_t*           ptr64BitData;

                    /* Sanity Check: Ensure that the data length is a multiple of 64 bits.
                     * We need to ensure that we do *not* access the data pointer access
                     * goes beyond the limits */
                    if ((ptrSigGenCfg->dataLen % 8U) != 0U)
                    {
                        /* Error: Invalid data length specified. */
                        *errCode = CRC_EINVAL;
                    }
                    else
                    {
                        /* Initialize and setup the pointers */
                        ptr64BitSignature = (volatile uint64_t*)CRC_getSignatureRegister (ptrCRCDriver);
                        ptr64BitData      = (uint64_t*)ptrSigGenCfg->ptrData;

                        /* Cycle through and write all the data to the registers: */
                        for (index = 0U; index < ptrSigGenCfg->dataLen; index = index + 8U)
                        {
                            /* Write the data to the register */
                            *ptr64BitSignature = *ptr64BitData;

                            /* Increment the data to the next address */
                            ptr64BitData++;
                        }
                    }
                    break;
                }
                default:
                {
                    /* Error: Invalid data length. This should never occur since the parameter
                     * validation has been completed by now. */
                    DebugP_assert (0);
                    break;
                }
            }
        }
    }
    return *errCode;
}

/**
 *  @b Description
 *  @n
 *      The function is used to get the generated signature.
 *
 *  @param[in]  crcHandle
 *      CRC Driver Handle
 *  @param[out] transactionId
 *      Transaction Id for which the CRC signature is being retreived
 *  @param[out] ptrSignature
 *      Pointer to the generated signature
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CRC_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t CRC_getSignature
(
    CRC_Handle  crcHandle,
    uint32_t    transactionId,
    uint64_t*   ptrSignature,
    int32_t*    errCode
)
{
    CRC_Driver*     ptrCRCDriver;

    /* Setup the error code: */
    *errCode = 0;

    /* Get the pointer to the CRC Driver */
    ptrCRCDriver = (CRC_Driver*)crcHandle;

    /* Sanity Check: Validate the arguments */
    if ((ptrCRCDriver == NULL) ||
        (ptrSignature == NULL) ||
        (CRC_isTransactionIdValid (ptrCRCDriver, transactionId) == 0U))
    {
        /* Error: Invalid arguments */
        *errCode = CRC_EINVAL;
    }
    else
    {
        /* Depending upon the CRC Channel configuration: */
        switch (ptrCRCDriver->cfg.type)
        {
            case CRC_Type_64BIT:
            {
                volatile uint64_t* ptr64BitSignature;

                /* Get the signature register: */
                ptr64BitSignature = (volatile uint64_t*)CRC_getSignatureRegister(ptrCRCDriver);

                /* Read the signature */
                *ptrSignature = *ptr64BitSignature;
                break;
            }
            default:
            {
                volatile uint32_t* ptr32BitSignature;

                /* Get the signature register */
                ptr32BitSignature = (volatile uint32_t*)CRC_getSignatureRegister(ptrCRCDriver);

                /* Read the signature */
                *ptrSignature = (uint64_t)(*ptr32BitSignature);
                break;
            }
        }

        /* Once the signature has been retreived; we can reset the signature: */
        CRC_resetSignature (ptrCRCDriver);

        /* Release the transaction id */
        CRC_releaseTransactionId (ptrCRCDriver);
    }
    return *errCode;
}

/**
 *  @b Description
 *  @n
 *      The function is used to cancel the signature generation.
 *
 *  @param[in]  crcHandle
 *      CRC Driver Handle
 *  @param[in]  transactionId
 *      Transaction Id for which the signature generation is being canceled
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CRC_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t CRC_cancelSignature (CRC_Handle crcHandle, uint32_t transactionId, int32_t* errCode)
{
    CRC_Driver* ptrCRCDriver;

    /* Setup the error code: */
    *errCode = 0;

    /* Get the pointer to the CRC Driver */
    ptrCRCDriver = (CRC_Driver*)crcHandle;

    /* Sanity Check: Validate the arguments */
    if ((ptrCRCDriver == NULL) ||
        (CRC_isTransactionIdValid (ptrCRCDriver, transactionId) == 0U))
    {
        /* Error: Invalid arguments */
        *errCode = CRC_EINVAL;
    }
    else
    {
        /* Reset the signature */
        CRC_resetSignature (ptrCRCDriver);

        /* Release the transaction id */
        CRC_releaseTransactionId (ptrCRCDriver);
    }
    return *errCode;
}

/**
 *  @b Description
 *  @n
 *      The function is used to setup the default configuration parameters for
 *      the CRC Driver.
 *
 *  @param[out]  ptrCRCCfg
 *      Pointer to the default configuration which is populated by the API
 *
 *  \ingroup CRC_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void CRC_initConfigParams(CRC_Config* ptrCRCCfg)
{
    ptrCRCCfg->channel  = CRC_Channel_CH1;
    ptrCRCCfg->mode     = CRC_Operational_Mode_FULL_CPU;
    ptrCRCCfg->type     = CRC_Type_16BIT;
    ptrCRCCfg->bitSwap  = CRC_BitSwap_MSB;
    ptrCRCCfg->byteSwap = CRC_ByteSwap_ENABLED;
    ptrCRCCfg->dataLen  = CRC_DataLen_16_BIT;
}

/**
 *  @b Description
 *  @n
 *      The function is used to open the CRC Driver with the specific configuration.
 *
 *  @param[in]  ptrCRCCfg
 *      Pointer to the CRC configuration
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CRC_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - CRC Driver Handle
 *  @retval
 *      Error       - NULL
 */
CRC_Handle CRC_open (const CRC_Config* ptrCRCCfg, int32_t* errCode)
{
    CRC_Driver*     ptrCRCDriver;
    CRC_Handle      retHandle = NULL;

    /* Sanity Check: Validate the arguments */
    if ((ptrCRCCfg == NULL) || (CRC_validateParams(ptrCRCCfg) < 0))
    {
        /* Error: Invalid argument or usage */
        *errCode = CRC_EINVAL;
    }
    else
    {
        /* Allocate memory for the CRC Driver */
        ptrCRCDriver = MemoryP_ctrlAlloc ((uint32_t)sizeof(CRC_Driver), 0);
        if (ptrCRCDriver == NULL)
        {
            /* Error: No memory available. */
            *errCode = CRC_ENOMEM;
        }
        else
        {
            /* Initialize the allocated memory: */
            memset ((void *)ptrCRCDriver, 0, sizeof(CRC_Driver));

            /* Populate the driver: */
            memcpy ((void *)&ptrCRCDriver->cfg, (void *)ptrCRCCfg, sizeof(CRC_Config));

            /* The driver is not being used: */
            ptrCRCDriver->inUse = 0U;

            /* Setup the register space for the CRC Driver: */
            ptrCRCDriver->ptrCRCRegs = gCRCRegister;

            /* Setup the return handle: */
            retHandle = (CRC_Handle)ptrCRCDriver;

            switch (ptrCRCDriver->cfg.channel)
            {
                case CRC_Channel_CH1:
                {
                    /* Channel 1: Reset the signature */
                    ptrCRCDriver->ptrCRCRegs->CRC_CTRL0 |= CSL_FMKR (0U, 0U, 1U);

                    /* Setup Pattern Count Preload */
                    ptrCRCDriver->ptrCRCRegs->CRC_PCOUNT_REG1 = CSL_FMKR (19U, 0U, 0U);

                    /* Setup Sector Count Preload */
                    ptrCRCDriver->ptrCRCRegs->CRC_SCOUNT_REG1 = CSL_FMKR (15U, 0U, 0U);

                    /* Setup the Watchdog timeout */
                    ptrCRCDriver->ptrCRCRegs->CRC_WDTOPLD1 = CSL_FMKR (23U, 0U, 0U);

                    /* Setup the Block timeout */
                    ptrCRCDriver->ptrCRCRegs->CRC_BCTOPLD1 = CSL_FMKR (23U, 0U, 0U);

                    /* Set the CRC Value */
                    ptrCRCDriver->ptrCRCRegs->CRC_REGL1 = 0;
                    ptrCRCDriver->ptrCRCRegs->CRC_REGH1 = 0;

                    /* Full CPU Mode: */
                    ptrCRCDriver->ptrCRCRegs->CRC_CTRL2 |= CSL_FMKR (1U, 0U, 3U);

                    /* Setup the Data Size: */
                    ptrCRCDriver->ptrCRCRegs->CRC_CTRL0 = CSL_FINSR (ptrCRCDriver->ptrCRCRegs->CRC_CTRL0, 2U, 1U,
                                                                 CRC_getCRCDataSelectBitValue(ptrCRCDriver->cfg.dataLen));

                    /* Setup the CRC Polynomial Type: */
                    ptrCRCDriver->ptrCRCRegs->CRC_CTRL0 = CSL_FINSR (ptrCRCDriver->ptrCRCRegs->CRC_CTRL0, 4U, 3U,
                                                                 CRC_getCRCTypeBitValue(ptrCRCDriver->cfg.type));

                    /* Setup the Bit Swap: */
                    if (ptrCRCDriver->cfg.bitSwap == CRC_BitSwap_MSB)
                    {
                        ptrCRCDriver->ptrCRCRegs->CRC_CTRL0 = CSL_FINSR (ptrCRCDriver->ptrCRCRegs->CRC_CTRL0, 5U, 5U, 0U);
                    }
                    else
                    {
                        ptrCRCDriver->ptrCRCRegs->CRC_CTRL0 = CSL_FINSR (ptrCRCDriver->ptrCRCRegs->CRC_CTRL0, 5U, 5U, 1U);
                    }

                    /* Setup the Byte Swap: */
                    if (ptrCRCDriver->cfg.byteSwap == CRC_ByteSwap_DISABLED)
                    {
                        ptrCRCDriver->ptrCRCRegs->CRC_CTRL0 = CSL_FINSR (ptrCRCDriver->ptrCRCRegs->CRC_CTRL0, 6U, 6U, 0U);
                    }
                    else
                    {
                        ptrCRCDriver->ptrCRCRegs->CRC_CTRL0 = CSL_FINSR (ptrCRCDriver->ptrCRCRegs->CRC_CTRL0, 6U, 6U, 1U);
                    }

                    /* Bring the channel out of reset: */
                    ptrCRCDriver->ptrCRCRegs->CRC_CTRL0 = CSL_FINSR(ptrCRCDriver->ptrCRCRegs->CRC_CTRL0, 0U, 0U, 0U);
                    break;
                }
                case CRC_Channel_CH2:
                {
                    /* Channel 2: Reset the signature */
                    ptrCRCDriver->ptrCRCRegs->CRC_CTRL0 |= CSL_FMKR (8U, 8U, 1U);

                    /* Setup Pattern Count Preload */
                    ptrCRCDriver->ptrCRCRegs->CRC_PCOUNT_REG2 = CSL_FMKR (19U, 0U, 0U);

                    /* Setup Sector Count Preload */
                    ptrCRCDriver->ptrCRCRegs->CRC_SCOUNT_REG2 = CSL_FMKR (15U, 0U, 0U);

                    /* Setup the Watchdog timeout */
                    ptrCRCDriver->ptrCRCRegs->CRC_WDTOPLD2 = CSL_FMKR (23U, 0U, 0U);

                    /* Setup the Block timeout */
                    ptrCRCDriver->ptrCRCRegs->CRC_BCTOPLD2 = CSL_FMKR (23U, 0U, 0U);

                    /* Set the CRC Value */
                    ptrCRCDriver->ptrCRCRegs->CRC_REGL2 = 0;
                    ptrCRCDriver->ptrCRCRegs->CRC_REGH2 = 0;

                    /* Full CPU Mode: */
                    ptrCRCDriver->ptrCRCRegs->CRC_CTRL2 |= CSL_FMKR (9U, 8U, 3U);

                    /* Setup the Data Size: */
                    ptrCRCDriver->ptrCRCRegs->CRC_CTRL0 = CSL_FINSR (ptrCRCDriver->ptrCRCRegs->CRC_CTRL0, 10U, 9U,
                                                                 CRC_getCRCDataSelectBitValue(ptrCRCDriver->cfg.dataLen));

                    /* Setup the CRC Polynomial Type: */
                    ptrCRCDriver->ptrCRCRegs->CRC_CTRL0 = CSL_FINSR (ptrCRCDriver->ptrCRCRegs->CRC_CTRL0, 12U, 11U,
                                                                 CRC_getCRCTypeBitValue(ptrCRCDriver->cfg.type));

                    /* Setup the Bit Swap: */
                    if (ptrCRCDriver->cfg.bitSwap == CRC_BitSwap_MSB)
                    {
                        ptrCRCDriver->ptrCRCRegs->CRC_CTRL0 = CSL_FINSR (ptrCRCDriver->ptrCRCRegs->CRC_CTRL0, 13U, 13U, 0U);
                    }
                    else
                    {
                        ptrCRCDriver->ptrCRCRegs->CRC_CTRL0 = CSL_FINSR (ptrCRCDriver->ptrCRCRegs->CRC_CTRL0, 13U, 13U, 1U);
                    }

                    /* Setup the Byte Swap: */
                    if (ptrCRCDriver->cfg.byteSwap == CRC_ByteSwap_DISABLED)
                    {
                        ptrCRCDriver->ptrCRCRegs->CRC_CTRL0 = CSL_FINSR (ptrCRCDriver->ptrCRCRegs->CRC_CTRL0, 14U, 14U, 0U);
                    }
                    else
                    {
                        ptrCRCDriver->ptrCRCRegs->CRC_CTRL0 = CSL_FINSR (ptrCRCDriver->ptrCRCRegs->CRC_CTRL0, 14U, 14U, 1U);
                    }

                    /* Bring the channel out of reset: */
                    ptrCRCDriver->ptrCRCRegs->CRC_CTRL0 = CSL_FINSR (ptrCRCDriver->ptrCRCRegs->CRC_CTRL0, 8U, 8U, 0U);
                    break;
                }
                default:
                {
                    /* Error: Invalid argument: Clean up the allocated memory */
                    *errCode = CRC_EINVAL;
                    MemoryP_ctrlFree (ptrCRCDriver, (uint32_t)sizeof(CRC_Driver));
                    break;
                }
            }
        }
    }
    return retHandle;
}

/**
 *  @b Description
 *  @n
 *      The function is used to close a previously opened CRC Driver
 *
 *  @param[in]  crcHandle
 *      Handle to the CRC Driver to be closed
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CRC_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t CRC_close (CRC_Handle crcHandle, int32_t* errCode)
{
    CRC_Driver*     ptrCRCDriver;

    /* Setup the error code: */
    *errCode = 0;

    /* Sanity Check: Validate the arguments */
    if (crcHandle == NULL)
    {
        /* Error: Invalid arguments */
        *errCode = CRC_EINVAL;
    }
    else
    {
        /* Get the CRC Driver Instance which is to be closed */
        ptrCRCDriver = (CRC_Driver*)crcHandle;

        /* Reset the signature */
        CRC_resetSignature (ptrCRCDriver);

        /* Release any transaction id */
        CRC_releaseTransactionId (ptrCRCDriver);

        /* Cleanup the memory: */
        MemoryP_ctrlFree (ptrCRCDriver, (uint32_t)sizeof(CRC_Driver));
    }
    return *errCode;
}

