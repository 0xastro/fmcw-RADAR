/**
 *   @file  mmwave_link.c
 *
 *   @brief
 *      The file implements the mmWave control module which interfaces
 *      with the mmWave Link API and communicates with the BSS
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
#include <ti/drivers/osal/HwiP.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/common/sys_common.h>
#include <ti/control/mmwavelink/mmwavelink.h>
#include <ti/control/mmwavelink/include/rl_driver.h>
#include <ti/control/mmwave/mmwave.h>
#include <ti/control/mmwave/include/mmwave_internal.h>

/**************************************************************************
 *************************** Local Definitions ****************************
 **************************************************************************/

/**
 * @brief   This is the bit mask derived from the mmWave Link to indicate
 * that the RF Initialization procedure was successful. As per the
 * documentation the bit mask is derived as follows:-
 *
 *  0       [Reserved]
 *  1       APLL tuning
 *  2       SYNTH VCO1 tuning
 *  3       SYNTH VCO2 tuning
 *  4       LODIST calibration
 *  5       RX ADC DC offset calibration
 *  6       HPF cutoff calibration
 *  7       LPF cutoff calibration
 *  8       Peak detector calibration
 *  9       TX Power calibration
 * 10       RX gain calibration
 * 11       [Reserved]
 * 12       RX IQMM calibration
 * 31:13    [Reserved]
 */
#define MMWAVE_INIT_CALIB_SUCCESS           0x17FEU

/**************************************************************************
 ***************************** Local Functions ****************************
 **************************************************************************/
static void MMWave_mboxCallbackFxn (Mbox_Handle handle, Mailbox_Type remoteEndpoint);
static rlComIfHdl_t MMWave_mboxOpen(rlUInt8_t deviceIndex, uint32_t flags);
static int32_t MMWave_mboxRead(rlComIfHdl_t fd, uint8_t* pBuff, uint16_t len);
static int32_t MMWave_mboxWrite(rlComIfHdl_t fd, uint8_t* pBuff, uint16_t len);
static int32_t MMWave_mboxClose(rlComIfHdl_t fd);
static rlInt32_t MMWave_enableDevice(rlUInt8_t deviceIndex);
static rlInt32_t MMWave_disableDevice(rlUInt8_t deviceIndex);
static void MMWave_maskHostIRQ(rlComIfHdl_t fd);
static void MMWave_unmaskHostIRQ(rlComIfHdl_t fd);
static rlInt32_t MMWave_waitIRQStatus(rlComIfHdl_t fd, rlUInt8_t Level);
static rlInt32_t MMWave_registerInterruptHandler(rlUInt8_t deviceIndex, RL_P_EVENT_HANDLER pHandler, void* pValue);
static rlInt32_t MMWave_spawn(RL_P_OSI_SPAWN_ENTRY fxn, const void* pValue, uint32_t flags);
static rlInt32_t MMWave_computeCRC(rlUInt8_t* ptrData, rlUInt32_t dataLen, rlUInt8_t crcType, rlUInt8_t* crc);
static void MMWave_asyncEventHandler(uint8_t devIndex, uint16_t sbId, uint16_t sbLen, uint8_t* payload);
static int32_t MMWave_initCRC (MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);
static int32_t MMWave_deinitCRC (MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);
static int32_t MMWave_initMMWaveLink (MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);
static int32_t MMWave_deinitMMWaveLink (MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);
static void    MMWave_internalLinkSync(const MMWave_MCB* ptrMMWaveMCB, uint32_t flag);
static int32_t MMWave_configureProfileChirp (MMWave_ProfileHandle* ptrProfileHandle, int32_t* errCode);

/**************************************************************************
 ************************ mmWave Link Functions ***************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      This is the registered callback function which is invoked when the
 *      Mailbox receives data.
 *
 *  @param[in]  handle
 *      Handle to the Mailbox on which data was received
 *  @param[in]  remoteEndpoint
 *      Remote endpoint from which data was received
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void MMWave_mboxCallbackFxn (Mbox_Handle handle, Mailbox_Type remoteEndpoint)
{
    /* Indicate to the Radar Link that a message has been received. */
    rlDriverHostIrqHandler(0, NULL);
}

/**
 *  @b Description
 *  @n
 *      This is the mmWave link registered callback function to open the communication
 *      interface channel
 *
 *  @param[in]  deviceIndex
 *      Device index for which the communication channel to be opened
 *  @param[in]  flags
 *      Flags which are passed to configure the interface
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success - Communicate Interface Channel Handle
 *  @retval
 *      Error   - NULL
 */
static rlComIfHdl_t MMWave_mboxOpen(rlUInt8_t deviceIndex, uint32_t flags)
{
    Mailbox_Config  cfg;
    int32_t         errCode;
    Mbox_Handle  mailBoxHandle = NULL;

    /* Initialize the mailbox configuration: */
    Mailbox_Config_init(&cfg);

    /* Populate the mailbox configuration: */
    cfg.writeMode    = MAILBOX_MODE_POLLING;
    cfg.readMode     = MAILBOX_MODE_CALLBACK;
    cfg.readCallback = &MMWave_mboxCallbackFxn;

    /* Open the Mailbox to the BSS */
    mailBoxHandle = Mailbox_open(MAILBOX_TYPE_BSS, &cfg, &errCode);
    if (mailBoxHandle == NULL)
    {
        /* Error: Unable to open the mailbox */
        DebugP_log1("Error: Unable to open the Mailbox to the BSS [Error code %d]\n", errCode);
        DebugP_assert(0);
    }
    else
    {
        /* Keep track of the mailbox handle */
        gMMWave_MCB.bssMailbox = mailBoxHandle;

        /* Debug Message: */
        DebugP_log1("Debug: BSS Mailbox Handle %p\n", gMMWave_MCB.bssMailbox);
    }
    return (rlComIfHdl_t)mailBoxHandle;
}

/**
 *  @b Description
 *  @n
 *      This is the mmWave link registered callback function to close the communication
 *      interface channel
 *
 *  @param[in]  fd
 *      Opaque Handle to the communication channel which is to be closed
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
static int32_t MMWave_mboxClose(rlComIfHdl_t fd)
{
    int32_t errCode;

    /* Close the Mailbox */
    errCode = Mailbox_close ((Mbox_Handle)fd);
    if (errCode < 0)
    {
        /* Error: Unable to close the mailbox */
        DebugP_log1("Error: Unable to close the BSS Mailbox [Error code %d]\n", errCode);
    }
    else
    {
        /* Debug: Mailbox closed successfully. */
        DebugP_log1("Debug: BSS Mailbox closed successfully\n", errCode);
    }
    return errCode;
}

/**
 *  @b Description
 *  @n
 *      This is the mmWave link registered callback function to read data from the
 *      communication interface channel
 *
 *  @param[in]  fd
 *      Opaque Handle to the communication channel
 *  @param[out]  pBuff
 *      Pointer to the data buffer which is populated with the received message
 *  @param[in]  len
 *      Size of the data buffer
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success - Number of bytes of received data
 *  @retval
 *      Error   - <0
 */
static int32_t MMWave_mboxRead(rlComIfHdl_t fd, uint8_t* pBuff, uint16_t len)
{
    return Mailbox_read((Mbox_Handle)fd, pBuff, (uint32_t)len);
}

/**
 *  @b Description
 *  @n
 *      This is the mmWave link registered callback function to write data to the
 *      communication interface channel
 *
 *  @param[in]  fd
 *      Opaque Handle to the communication channel
 *  @param[in]  pBuff
 *      Pointer to the data buffer which is to be written out
 *  @param[in]  len
 *      Size of the data buffer
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success - Number of bytes of data which is sent
 *  @retval
 *      Error   - <0
 */
static int32_t MMWave_mboxWrite(rlComIfHdl_t fd, uint8_t* pBuff, uint16_t len)
{
    int32_t    status;
     /*
      * Currently, the mmwavelink can not detect the error condition where it did not
      * receive a mailbox layer ACK from BSS.
      *
      * For instance:
      * - The mmwavelink may try to send a message before an ACK was received
      *   for the previous message.
      * - The mmwavelink may try to resend a message that did not receive a
      *   mmwavelink layer ACK back from BSS. It is possible that the message did
      *   not receive a mailbox layer ACK as well from BSS.
      * In either case, Mailbox_writeReset() has to be called before another message
      * is sent to BSS. The mmwavelink has no hooks to call the Mailbox_writeReset().
      * Therefore, a write reset is done if it is detected that a mailbox layer ACK
      * was not received for the previous message (MAILBOX_ETXFULL).
      */
    status = Mailbox_write((Mbox_Handle)fd, pBuff, (uint32_t)len);
    if(status == MAILBOX_ETXFULL)
    {
        Mailbox_writeReset((Mbox_Handle)fd);
        gMMWave_MCB.wResetCounter++;
        status = Mailbox_write((Mbox_Handle)fd, pBuff, (uint32_t)len);
    }
    return status;
}

/**
 *  @b Description
 *  @n
 *      This is the mmWave link registered callback function to power on and enable
 *      the XWR1XX device
 *
 *  @param[in]  deviceIndex
 *      Device index which is to be enabled
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
static rlInt32_t MMWave_enableDevice(rlUInt8_t deviceIndex)
{
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the mmWave link registered callback function to power off and disable
 *      the XWR1XX device
 *
 *  @param[in]  deviceIndex
 *      Device index which is to be disabled
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
static rlInt32_t MMWave_disableDevice(rlUInt8_t deviceIndex)
{
    /* Note: halting of the BSS is not currently supported */
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the mmWave link registered callback function to mask the
 *      interrupts. Since the mmWave control module is using the Mailbox
 *      communication interface the driver is handling all the interrupt
 *      management. This function is a dummy stub.
 *
 *  @param[in]  fd
 *      Handle to the communication interface
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void MMWave_maskHostIRQ(rlComIfHdl_t fd)
{
    return;
}

/**
 *  @b Description
 *  @n
 *      This is the mmWave link registered callback function to unmask the
 *      interrupts to indicate that the message has been successfully handled
 *
 *  @param[in]  fd
 *      Handle to the communication interface
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void MMWave_unmaskHostIRQ(rlComIfHdl_t fd)
{
    /* The Mailbox driver indicates to the remote endpoint that the message
     * have been processed. The mmWave is now capable of receiving another message */
    Mailbox_readFlush((Mbox_Handle)fd);
}

/**
 *  @b Description
 *  @n
 *      This is the mmWave link registered callback function to poll the
 *      host interrupt status. In the case of the Mailbox the driver is
 *      responsible for the interrupt management. This function is a
 *      dummy stub
 *
 *  @param[in]  fd
 *      Handle to the communication interface
 *  @param[in]  Level
 *      Wait for IRQ Level(high/low)
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Always returns 0 indicating that the IRQ Line is low
 */
static rlInt32_t MMWave_waitIRQStatus(rlComIfHdl_t fd, rlUInt8_t Level)
{
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the mmWave link registered callback function to register the
 *      interrupt handler. In the case of the Mailbox the driver is responsible
 *      for the interrupt management. This function is a dummy stub
 *
 *  @param[in]  deviceIndex
 *      Device for which the interrupt is to be registered
 *  @param[in]  pHandler
 *      ISR Handler
 *  @param[in]  pValue
 *      Argument to the ISR
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Always returns 0
 */
static rlInt32_t MMWave_registerInterruptHandler
(
    rlUInt8_t           deviceIndex,
    RL_P_EVENT_HANDLER  pHandler,
    void*               pValue
)
{
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the mmWave link registered callback function which is invoked
 *      to ensure that the spawn function is invoked in a different execution
 *      context
 *
 *  @param[in]  fxn
 *      Pointer to the function to be executed in a different context
 *  @param[in]  pValue
 *      Pointer of data to be passed to the function
 *  @param[in]  flags
 *      Flag to indicate preference
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
static rlInt32_t MMWave_spawn
(
    RL_P_OSI_SPAWN_ENTRY    fxn,
    const void*             pValue,
    uint32_t                flags
)
{
    MMWave_SpawnFxnNode*    ptrSpawnFxnNode;
    uintptr_t               key;
    int32_t                 retVal = 0;

    /* Critical Section: The spawn free list is a critical resource which is accessed
     * from multiple contexts */
    key = HwiP_disable();
    ptrSpawnFxnNode = (MMWave_SpawnFxnNode*)MMWave_listRemove ((MMWave_ListNode**)&gMMWave_MCB.ptrSpawnFxnFreeList);
    HwiP_restore (key);

    /* Did we get an entry? */
    if (ptrSpawnFxnNode != NULL)
    {
        /* YES: Populate the entry */
        ptrSpawnFxnNode->spawnEntry = fxn;
        ptrSpawnFxnNode->arg        = pValue;

        /* Critical Section: The spawn active list is a critical resource which is accessed
         * from multiple contexts */
        key = HwiP_disable();
        MMWave_listAdd ((MMWave_ListNode**)&gMMWave_MCB.ptrSpawnFxnActiveList, (MMWave_ListNode*)ptrSpawnFxnNode);
        HwiP_restore (key);

        /* Keep track of the number of spawn messages which have been received */
        gMMWave_MCB.spawnCounter++;

        /* Wake up the mmWave execution thread  */
        SemaphoreP_post (gMMWave_MCB.linkSemHandle);
    }
    else
    {
        /* Error: No span free node was present. This can happen if all the spawn functions
         * have been taken up and the execute mmWave control API has not been invoked. Increment
         * statistics to report this condition */
        gMMWave_MCB.spawnOverflow++;

        /* Setup the return value to indicate an error. */
        retVal = MINUS_ONE;
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      This is the mmWave link registered callback function to compute
 *      the CRC for the specified data buffer.
 *
 *  @param[in]  ptrData
 *      Pointer to the data buffer for which the CRC is to be computed
 *  @param[in]  dataLen
 *      Length of the data buffer for which the CRC is to be computed
 *  @param[in]  crcType
 *      CRC Type i.e. 16bit, 32bit or 64bit.
 *  @param[out] crc
 *      Computed CRC
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
static rlInt32_t MMWave_computeCRC
(
    rlUInt8_t*  ptrData,
    rlUInt32_t  dataLen,
    rlUInt8_t   crcType,
    rlUInt8_t*  crc
)
{
    CRC_SigGenCfg   signGenCfg;
    int32_t         errCode;
    uint64_t        signature;
    uint32_t        index;
    uint8_t*        ptrSignature;
    int32_t         retVal    = 0;
    uint8_t         crcLength = 0U;

    /* Sanity Check: Validate the parameters; determine the length of the CRC */
    switch (crcType)
    {
        case RL_CRC_TYPE_16BIT_CCITT:
        {
            /* CRC is 2 bytes */
            crcLength = 2U;
            break;
        }
        case RL_CRC_TYPE_32BIT:
        {
            /* CRC is 4 bytes */
            crcLength = 4U;
            break;
        }
        case RL_CRC_TYPE_64BIT_ISO:
        {
            /* CRC is 8 bytes */
            crcLength = 8U;
            break;
        }
        default:
        {
            /* Error: The mmWave link has passed an invalid CRC Type */
            retVal = MINUS_ONE;
            break;
        }
    }

    /* Sanity Check: Did the link pass a valid CRC Type? */
    if (retVal == MINUS_ONE)
    {
        /* NO: Invalid CRC Type has been detected */
        DebugP_assert (0);
        goto exit;
    }

    /* Initialize the signature generation configuration */
    memset ((void *)&signGenCfg, 0, sizeof(CRC_SigGenCfg));

    /* Allocate a unique transaction identifier */
    retVal = CRC_getTransactionId (gMMWave_MCB.crcHandle, &signGenCfg.transactionId, &errCode);
    if (retVal < 0)
    {
        /* Error: Unable to allocate a transaction identifier. This should never happen. */
        DebugP_log1 ("Error: Allocation of transaction identifier failed [Error code %d]\n", errCode);
        DebugP_assert (0);
        goto exit;
    }

    /* Populate the signature generation configuration: */
    signGenCfg.ptrData = (uint8_t*)ptrData;
    signGenCfg.dataLen = (uint32_t)dataLen;

    /* Compute the signature for the specific data */
    retVal = CRC_computeSignature (gMMWave_MCB.crcHandle, &signGenCfg, &errCode);
    if (retVal < 0)
    {
        /* Error: Unable to compute the CRC. This should never happen */
        DebugP_log1 ("Error: CRC Driver compute CRC failed [Error code %d]\n", errCode);
        DebugP_assert (0);
        goto exit;
    }

    /* Get the computed signature */
    retVal = CRC_getSignature (gMMWave_MCB.crcHandle, signGenCfg.transactionId, &signature, &errCode);
    if (retVal < 0)
    {
        /* Error: Unable to get the computed signature. This should never happen */
        DebugP_log1 ("Error: Unable to get the CRC signature [Error code %d]\n", errCode);
        DebugP_assert (0);
        goto exit;
    }

    /* Get the pointer to the CRC Signature: */
    ptrSignature = (uint8_t*)&signature;

    /* Copy the CRC signature into CRC output array*/
    for(index = 0U; index < crcLength; index++)
    {
        *(crc + index) = *(ptrSignature + index);
    }

    /* CRC signature has been computed correctly */
    retVal = 0;

exit:
    return (rlInt32_t)retVal;
}

/**
 *  @b Description
 *  @n
 *      This is the mmWave link registered callback function which
 *      is used to handle asynchronous events which are received from
 *      the BSS.
 *
 *  @param[in]  devIndex
 *      Device Index
 *  @param[in]  sbId
 *      Subblock identifier
 *  @param[in]  sbLen
 *      Length of the subblock
 *  @param[in]  payload
 *      Pointer to the payload buffer
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void MMWave_asyncEventHandler
(
    uint8_t     devIndex,
    uint16_t    sbId,
    uint16_t    sbLen,
    uint8_t*    payload
)
{
    int32_t             retVal  = 0;
    uint16_t            asyncSB;
    uint16_t            msgId;

    /* Get the sub-block and message identifier. */
    asyncSB = RL_GET_SBID_FROM_UNIQ_SBID(sbId);
    msgId   = RL_GET_MSGID_FROM_SBID(sbId);

    /* Process the received message: */
    switch (msgId)
    {
        case RL_RF_ASYNC_EVENT_MSG:
        {
            /*******************************************************************
             * Received Asychronous Message
             *******************************************************************/
            switch (asyncSB)
            {
                case RL_RF_AE_INITCALIBSTATUS_SB:
                {
		    rlRfInitComplete_t*  ptrRFInitCompleteMessage;
		    uint32_t            calibrationStatus;

                    /* Get the RF-Init completion message: */
                    ptrRFInitCompleteMessage = (rlRfInitComplete_t*)payload;

                    /* Were we successful?  */
                    calibrationStatus = ptrRFInitCompleteMessage->calibStatus & 0xFFFU;
                    if (calibrationStatus != 0)
                    {
                        /* YES: Mark the link as operational */
                        gMMWave_MCB.linkStatus = gMMWave_MCB.linkStatus | MMWAVE_RF_INITIALIZED;
                    }
                    else
                    {
                        /* NO: Link initialization failed. */
                        gMMWave_MCB.linkStatus = gMMWave_MCB.linkStatus | MMWAVE_RF_INIT_FAILED;
                    }
                    break;
                }
                case RL_RF_AE_RUN_TIME_CALIB_REPORT_SB:
                {
                    /* Calibration has been completed. */
                    gMMWave_MCB.linkStatus = gMMWave_MCB.linkStatus | MMWAVE_RF_CALIBRATION_DONE;
                    break;
                }
                case RL_RF_AE_MON_TIMING_FAIL_REPORT_SB:
                {
                    /* Calibration has failed. */
                    gMMWave_MCB.linkStatus = gMMWave_MCB.linkStatus | MMWAVE_RF_CALIBRATION_FAILED;
                    break;
                }
                case RL_RF_AE_CPUFAULT_SB:
                {
                    /* BSS reports CPU Fault: */
                    gMMWave_MCB.linkStatus = gMMWave_MCB.linkStatus | MMWAVE_RF_CPU_FAULT;
                    break;
                }
                case RL_RF_AE_ESMFAULT_SB:
                {
                    /* BSS reports ESM Fault: */
                    gMMWave_MCB.linkStatus = gMMWave_MCB.linkStatus | MMWAVE_RF_ESM_FAULT;
                    break;
                }
                case RL_RF_AE_ANALOG_FAULT_SB:
                {
                    /* BSS reports Analog Fault: */
                    gMMWave_MCB.linkStatus = gMMWave_MCB.linkStatus | MMWAVE_RF_ANALOG_FAULT;
                    break;
                }
                default:
                {
                    /* Catch condition for any other sub block which the mmWave is NOT interested
                     * in. This is just a fall through. Pass the event to the application registered
                     * event function. */
                    break;
                }
            }
            break;
        }
        case RL_RF_ASYNC_EVENT_1_MSG:
        {
            /*Just fwd message to application*/
            break;
        }
        default:
        {
            /* Error: Received an invalid message identifier in the ASYNC Handler */
            DebugP_log1 ("Error: Invalid message %d is received in the async handler\n", msgId);
            DebugP_assert (0);
            break;
        }
    }

    /* Pass the event to the application registered handler */
    retVal = gMMWave_MCB.initCfg.eventFxn (msgId, sbId, sbLen, payload);

    /* Has the asynchronous event been hijacked by the application? */
    if (retVal == 0)
    {
        /* NO: Pass the message to the SOC asynchronous event handler */
        MMWave_deviceEventFxn (&gMMWave_MCB, msgId, sbId, sbLen, payload);
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to initialize the CRC channel
 *
 *  @param[in]  ptrMMWaveMCB
 *      Pointer to the control MCB
 *  @param[out] errCode
 *      Error code populated by the API on an error
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t MMWave_initCRC (MMWave_MCB* ptrMMWaveMCB, int32_t* errCode)
{
    CRC_Config  crcCfg;
    int32_t     retVal = MINUS_ONE;

    /* Do we need to use the CRC Driver? */
    if (ptrMMWaveMCB->initCfg.linkCRCCfg.useCRCDriver == 1U)
    {
        /* YES: Setup the default configuration: */
        CRC_initConfigParams(&crcCfg);

        /* Setup the CRC Configuration: The mmWave Link uses 16bit CRC */
        crcCfg.channel  = ptrMMWaveMCB->initCfg.linkCRCCfg.crcChannel;
        crcCfg.mode     = CRC_Operational_Mode_FULL_CPU;
        crcCfg.type     = CRC_Type_16BIT;
        crcCfg.bitSwap  = CRC_BitSwap_MSB;
        crcCfg.byteSwap = CRC_ByteSwap_ENABLED;
        crcCfg.dataLen  = CRC_DataLen_16_BIT;

        /* Open the CRC Driver */
        ptrMMWaveMCB->crcHandle = CRC_open (&crcCfg, errCode);
        if (ptrMMWaveMCB->crcHandle == NULL)
        {
            /* Error: Unable to open the CRC Driver */
            *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EINIT, *errCode);
            goto exit;
        }
    }
    else
    {
        /* NO: The CRC Driver is not being used. */
        ptrMMWaveMCB->crcHandle = NULL;
    }

    /* Control comes here implies that the CRC driver has been setup successfully */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to deinitialize the CRC channel which had
 *      been configured.
 *
 *  @param[in]  ptrMMWaveMCB
 *      Pointer to the control MCB
 *  @param[out] errCode
 *      Error code populated by the API on an error
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t MMWave_deinitCRC (MMWave_MCB* ptrMMWaveMCB, int32_t* errCode)
{
    int32_t     retVal;

    /* Was there a CRC Channel to be deinitialized? */
    if (ptrMMWaveMCB->crcHandle != NULL)
    {
        /* YES: Close the CRC Driver */
        retVal = CRC_close (ptrMMWaveMCB->crcHandle, errCode);
        if (retVal < 0)
        {
            /* Error: Unable to close the CRC Driver. */
            *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EDEINIT, *errCode);
            retVal   = MINUS_ONE;
        }
    }
    else
    {
        /* NO CRC channel was configured: */
        retVal = 0;
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to perform an internal synchronization waiting
 *      for the specific flag to be set. The function will loop around until
 *      the condition is met
 *
 *  @param[in]  ptrMMWaveMCB
 *      Pointer to the control MCB
 *  @param[in]  flag
 *      Flag for which we are synchronizing
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void MMWave_internalLinkSync
(
    const MMWave_MCB*   ptrMMWaveMCB,
    uint32_t            flag
)
{
    /* Loop around till the condition is met: */
    while (!(ptrMMWaveMCB->linkStatus & flag))
    {
    }
}

/**
 *  @b Description
 *  @n
 *      The function is used to initialize the mmWave Link
 *
 *  @param[in]  ptrMMWaveMCB
 *      Pointer to the control MCB
 *  @param[out] errCode
 *      Error code populated by the API on an error
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t MMWave_initMMWaveLink (MMWave_MCB* ptrMMWaveMCB, int32_t* errCode)
{
    rlClientCbs_t   RlApp_ClientCtx;
    int32_t         retVal = MINUS_ONE;

    /* Initialize the link status: */
    ptrMMWaveMCB->linkStatus = 0U;

    /* Reset the client context: */
    memset ((void *)&RlApp_ClientCtx, 0, sizeof(rlClientCbs_t));

    /* Populate the mmWave link context configuration: */
    RlApp_ClientCtx.ackTimeout = MMWAVE_ACK_TIMEOUT;

    /* Are we allowed to use the CRC Driver? */
    if (ptrMMWaveMCB->initCfg.linkCRCCfg.useCRCDriver == 1U)
    {
        /* YES: Use the 16bit CRC */
        RlApp_ClientCtx.crcType = RL_CRC_TYPE_16BIT_CCITT;
    }
    else
    {
        /* NO: No CRC is used. */
        RlApp_ClientCtx.crcType = RL_CRC_TYPE_NO_CRC;
    }

    /* Setup the platform on which the mmWave Link executes */
    MMWave_deviceGetDeviceInfo (ptrMMWaveMCB, &RlApp_ClientCtx.arDevType, &RlApp_ClientCtx.platform);

    /* Initialize the Communication Interface API: */
    RlApp_ClientCtx.comIfCb.rlComIfOpen  = &MMWave_mboxOpen;
    RlApp_ClientCtx.comIfCb.rlComIfClose = &MMWave_mboxClose;
    RlApp_ClientCtx.comIfCb.rlComIfRead  = &MMWave_mboxRead;
    RlApp_ClientCtx.comIfCb.rlComIfWrite = &MMWave_mboxWrite;

    /* Initialize OSI Mutex Interface */
    RlApp_ClientCtx.osiCb.mutex.rlOsiMutexCreate = &MMWave_osalMutexCreate;
    RlApp_ClientCtx.osiCb.mutex.rlOsiMutexLock   = &MMWave_osalMutexLock;
    RlApp_ClientCtx.osiCb.mutex.rlOsiMutexUnLock = &MMWave_osalMutexUnlock;
    RlApp_ClientCtx.osiCb.mutex.rlOsiMutexDelete = &MMWave_osalMutexDelete;

    /* Initialize OSI Semaphore Interface */
    RlApp_ClientCtx.osiCb.sem.rlOsiSemCreate = &MMWave_osalSemCreate;
    RlApp_ClientCtx.osiCb.sem.rlOsiSemWait   = &MMWave_osalSemWait;
    RlApp_ClientCtx.osiCb.sem.rlOsiSemSignal = &MMWave_osalSemSignal;
    RlApp_ClientCtx.osiCb.sem.rlOsiSemDelete = &MMWave_osalSemDelete;

    /* Initialize OSI Queue Interface */
    RlApp_ClientCtx.osiCb.queue.rlOsiSpawn = &MMWave_spawn;

    /* Initialize OSI Timer Interface */
    RlApp_ClientCtx.timerCb.rlDelay = NULL;

    /* Initialize the CRC Interface */
    RlApp_ClientCtx.crcCb.rlComputeCRC = &MMWave_computeCRC;

    /* Initialize Device Control Interface */
    RlApp_ClientCtx.devCtrlCb.rlDeviceDisable            = &MMWave_disableDevice;
    RlApp_ClientCtx.devCtrlCb.rlDeviceEnable             = &MMWave_enableDevice;
    RlApp_ClientCtx.devCtrlCb.rlDeviceMaskHostIrq        = &MMWave_maskHostIRQ;
    RlApp_ClientCtx.devCtrlCb.rlDeviceUnMaskHostIrq      = &MMWave_unmaskHostIRQ;
    RlApp_ClientCtx.devCtrlCb.rlDeviceWaitIrqStatus      = &MMWave_waitIRQStatus;
    RlApp_ClientCtx.devCtrlCb.rlRegisterInterruptHandler = &MMWave_registerInterruptHandler;

    /* Initialize the Asynchronous Event Handler: */
    RlApp_ClientCtx.eventCb.rlAsyncEvent = &MMWave_asyncEventHandler;

    /* Power on the Device: */
    retVal = (int32_t)rlDevicePowerOn(1U, RlApp_ClientCtx);
    if (retVal != RL_RET_CODE_OK)
    {
        /* Error: Unable to initialize and power on the BSS. Encode the error code to account
         * for the subsystem error code. */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EINIT, retVal);
        retVal   = MINUS_ONE;
        goto exit;
    }

    /* Wait for BSS powerup */
    if (SOC_waitBSSPowerUp (gMMWave_MCB.initCfg.socHandle, &retVal) < 0)
    {
        /* Error: Unable to initialize and power on the BSS */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EINIT, retVal);
        retVal   = MINUS_ONE;
        goto exit;
    }

    /* Get the version information: */
    retVal = rlDeviceGetVersion(RL_DEVICE_MAP_INTERNAL_BSS, &ptrMMWaveMCB->version);
    if (retVal != RL_RET_CODE_OK)
    {
        /* Error: Unable to get the device version */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EVERSION, retVal);
        retVal   = MINUS_ONE;
        goto exit;
    }

    /* Link has been setup successfully. */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to deinitialize the mmWave Link
 *
 *  @param[in]  ptrMMWaveMCB
 *      Pointer to the control MCB
 *  @param[out] errCode
 *      Error code populated by the API on an error
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t MMWave_deinitMMWaveLink (MMWave_MCB* ptrMMWaveMCB, int32_t* errCode)
{
    int32_t     retVal;

    /* Power off the Device: */
    retVal = (int32_t)rlDevicePowerOff();
    if (retVal != RL_RET_CODE_OK)
    {
        /* Error: Unable to initialize and power on the BSS */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EDEINIT, retVal);
        retVal   = MINUS_ONE;
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to configure BPM
 *
 *  @param[in]  ptrMMWaveMCB
 *      Pointer to the MCB
 *  @param[in]  ptrControlCfg
 *      Pointer to the control config
 *  @param[out] errCode
 *      Error code populated by the API on an error
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MMWave_configBPM
(
    MMWave_MCB*         ptrMMWaveMCB,
    MMWave_CtrlCfg*     ptrControlCfg,
    int32_t*            errCode
)
{
    int32_t                 retVal;
    uint32_t                numBpmChirps = 0;
    uint32_t                index;
    MMWave_BpmChirpHandle   bpmChirpHandle;
    rlBpmChirpCfg_t**       bpmPtrArray;
    rlBpmCommonCfg_t        bpmCommonCfg;
    uint32_t                arraySize;
    MMWave_BpmChirp*        ptrMMWaveBpmChirp;


    /* Get the number of BPM chirps configured */
    if (MMWave_getNumBpmChirp ((MMWave_Handle)ptrMMWaveMCB, &numBpmChirps, errCode) < 0)
    {
        /* Error: Unable to get the number of BPM chirps. Error code is already setup */
        retVal = MINUS_ONE;
        goto end;
    }

    if(numBpmChirps == 0)
    {
        /* No BPM chirp configured. Nothing to be done.*/
        retVal = 0;
        goto end;
    }

    arraySize = numBpmChirps * sizeof(rlBpmChirpCfg_t*);

    /* Allocate array to store pointers to BPM configurations*/
    bpmPtrArray = (rlBpmChirpCfg_t**) MemoryP_ctrlAlloc (arraySize, 0);

    if (bpmPtrArray == NULL)
    {
        /* Error: Out of memory */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_ENOMEM, 0);
        retVal   = MINUS_ONE;
        goto end;
    }

    /* Initialize the allocated memory for the chirp: */
    memset ((void*)bpmPtrArray, 0, arraySize);

    /* Select source of BPM pattern to be from BPM chirp cfg defined in bpmChirpCfg*/
    memset ((void *)&bpmCommonCfg, 0, sizeof(rlBpmCommonCfg_t));
    bpmCommonCfg.mode.b2SrcSel = 0U;

    /* Set the BPM common config */
    retVal = rlSetBpmCommonConfig(RL_DEVICE_MAP_INTERNAL_BSS, &bpmCommonCfg);
    if (retVal != RL_RET_CODE_OK)
    {
        /* Error: Setting the BPM configuration failed */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_ECOMMONBPMCFG, retVal);
        retVal   = MINUS_ONE;
        goto end;
    }

    /* Cycle through all the BPM configurations and populate array. */
    for (index = 1U; index <= numBpmChirps; index++)
    {
        /* Get the Handle associated to the specified index */
        if (MMWave_getBpmChirpHandle ((MMWave_Handle)ptrMMWaveMCB, index, &bpmChirpHandle, errCode) < 0)
        {
            /* Error: Unable to get the handle. Error code is already setup */
            retVal = MINUS_ONE;
            goto end;
        }

        /* Populate the BPM cfg array. Note that index starts from 1 and array starts from zero. */
        ptrMMWaveBpmChirp = (MMWave_BpmChirp*)bpmChirpHandle;
        bpmPtrArray[index-1] = (rlBpmChirpCfg_t*)(&ptrMMWaveBpmChirp->bpmChirp);
    }

    /* Set the BPM chirp configuration in the mmWave link */
    retVal = rlSetMultiBpmChirpConfig(RL_DEVICE_MAP_INTERNAL_BSS,
                                     (rlUInt16_t)numBpmChirps,
                                     bpmPtrArray);

    /* Free the memory used by the config array) */
    MemoryP_ctrlFree ((void *)bpmPtrArray, arraySize);

    if (retVal != RL_RET_CODE_OK)
    {
        /* Error: Setting the BPM configuration failed */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EBPMCFG, retVal);
        retVal   = MINUS_ONE;
        goto end;
    }

end:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to configure the mmWave link with the specified profile
 *      and corresponding chirp configuration. The following order is preserved in
 *      the function:
 *          - Profile configuration
 *          - Chirp configuration
 *
 *  @param[in]  ptrProfileHandle
 *      Pointer to profile handle
 *  @param[out] errCode
 *      Error code populated by the API on an error
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t MMWave_configureProfileChirp (MMWave_ProfileHandle* ptrProfileHandle, int32_t* errCode)
{
    int32_t                 retVal;
    int32_t                 index;
    rlProfileCfg_t          profileCfg;
    MMWave_ChirpHandle      chirpHandle;
    rlChirpCfg_t            chirpCfg;
    uint32_t                numChirps;
    uint32_t                chirpIndex;

    /* Cycle through all the profile(s) which have been specified. */
    for (index = 0U; index < MMWAVE_MAX_PROFILE; index++)
    {
        /* Do we have a valid profile? */
        if (ptrProfileHandle[index] == NULL)
        {
            /* NO: Skip to the next profile */
            continue;
        }

        /* YES: Get the profile configuration */
        if (MMWave_getProfileCfg (ptrProfileHandle[index], &profileCfg, errCode) < 0)
        {
            /* Error: Unable to get the profile configuration. Setup the return value */
            retVal = MINUS_ONE;
            goto end;
        }

        /* Configure the profile using the mmWave Link API */
        retVal = rlSetProfileConfig (RL_DEVICE_MAP_INTERNAL_BSS, 1U, &profileCfg);
        if (retVal != RL_RET_CODE_OK)
        {
            /* Error: Setting the profile configuration failed */
            *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EPROFILECFG, retVal);
            retVal   = MINUS_ONE;
            goto end;
        }

        /* Get the number of chirps configured and attached to the profile: */
        if (MMWave_getNumChirps (ptrProfileHandle[index], &numChirps, errCode) < 0)
        {
            /* Error: Unable to get the number of chirps. Error code is already setup */
            retVal = MINUS_ONE;
            goto end;
        }

        /* For the profile; Cycle through all the chirps and configure them. */
        for (chirpIndex = 1U; chirpIndex <= numChirps; chirpIndex++)
        {
            /* Get the Chirp Handle associated at the specified index */
            if (MMWave_getChirpHandle (ptrProfileHandle[index], chirpIndex, &chirpHandle, errCode) < 0)
            {
                /* Error: Unable to get the chirp handle. Error code is already setup */
                retVal = MINUS_ONE;
                goto end;
            }

            /* Get the chirp configuration: */
            if (MMWave_getChirpCfg (chirpHandle, &chirpCfg, errCode) < 0)
            {
                /* Error: Unable to get the chirp configuration. Error code is already setup */
                retVal = MINUS_ONE;
                goto end;
            }

            /* Set the chirp configuration in the mmWave link */
            retVal = rlSetChirpConfig(RL_DEVICE_MAP_INTERNAL_BSS, 1U, &chirpCfg);
            if (retVal != RL_RET_CODE_OK)
            {
                /* Error: Setting the chirp configuration failed */
                *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_ECHIRPCFG, retVal);
                retVal   = MINUS_ONE;
                goto end;
            }
        }
    }

    /* Control comes here implies that the profile & chirp was configured successfully */
    retVal = 0;

end:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to configure the mmWave link with the supplied
 *      configuration
 *
 *  @param[in]  ptrMMWaveMCB
 *      Pointer to the control MCB
 *  @param[in]  ptrControlCfg
 *      Pointer to the control configuration
 *  @param[out] errCode
 *      Error code populated by the API on an error
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MMWave_configLink
(
    MMWave_MCB*         ptrMMWaveMCB,
    MMWave_CtrlCfg*     ptrControlCfg,
    int32_t*            errCode
)
{
    int32_t retVal;

    /* Determine the DFE Output mode? */
    switch (ptrControlCfg->dfeDataOutputMode)
    {
        case MMWave_DFEDataOutputMode_FRAME:
        {
            /**************************************************************************
             * Frame Mode:
             * Order of operations as specified by the mmWave Link are
             *  - Profile configuration
             *  - Chirp configuration
             *  - Frame configuration
             **************************************************************************/
            retVal = MMWave_configureProfileChirp (&ptrControlCfg->u.frameCfg.profileHandle[0], errCode);
            if (retVal < 0)
            {
                /* Error: Unable to configure the profile & chirps. Error code is already setup */
                goto end;
            }

            if (MMWave_configBPM (ptrMMWaveMCB, ptrControlCfg, errCode) < 0)
            {
                /* Error: Unable to configure BPM chirps. Error code is already setup */
                retVal = -1;
                goto end;
            }

            /* Set the frame configuration: */
            retVal = rlSetFrameConfig(RL_DEVICE_MAP_INTERNAL_BSS, &ptrControlCfg->u.frameCfg.frameCfg);
            if (retVal != RL_RET_CODE_OK)
            {
                /* Error: Setting the frame configuration failed */
                *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EFRAMECFG, retVal);
                retVal   = MINUS_ONE;
                goto end;
            }
            break;
        }
        case MMWave_DFEDataOutputMode_CONTINUOUS:
        {
            /**************************************************************************
             * Continuous Mode:
             **************************************************************************/
            retVal = rlSetContModeConfig (RL_DEVICE_MAP_INTERNAL_BSS, &ptrControlCfg->u.continuousModeCfg.cfg);
            if (retVal != RL_RET_CODE_OK)
            {
                /* Error: Unable to setup the continuous mode */
                *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_ECONTMODECFG, retVal);
                retVal   = MINUS_ONE;
                goto end;
            }
            break;
        }
        case MMWave_DFEDataOutputMode_ADVANCED_FRAME:
        {
            /**************************************************************************
             * Advanced Frame Configuration:
             * Order of operations as specified by the mmWave Link are
             *  - Profile configuration
             *  - Chirp configuration
             *  - Advanced Frame configuration
             **************************************************************************/
            retVal = MMWave_configureProfileChirp (&ptrControlCfg->u.advancedFrameCfg.profileHandle[0], errCode);
            if (retVal < 0)
            {
                /* Error: Unable to configure the profile & chirps. Error code is already setup */
                goto end;
            }

            if (MMWave_configBPM (ptrMMWaveMCB, ptrControlCfg, errCode) < 0)
            {
                /* Error: Unable to configure BPM chirps. Error code is already setup */
                retVal = -1;
                goto end;
            }

            /* Set the advanced frame configuration: */
            retVal = rlSetAdvFrameConfig(RL_DEVICE_MAP_INTERNAL_BSS, &ptrControlCfg->u.advancedFrameCfg.frameCfg);
            if (retVal != RL_RET_CODE_OK)
            {
                /* Error: Setting the frame configuration failed */
                *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EFRAMECFG, retVal);
                retVal   = MINUS_ONE;
                goto end;
            }
            break;
        }
        default:
        {
            /* Error: This should never occur and the user seems to have ignored a warning. */
            *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EINVAL, 0);
            retVal   = MINUS_ONE;
            goto end;
        }
    }

    /* Set the return value to be success. */
    retVal = 0;

end:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to start the mmWave link. This function can only be
 *      invoked once the configuration has been completed successfully.
 *
 *  @param[in]  ptrMMWaveMCB
 *      Pointer to the control MCB
 *  @param[out] errCode
 *      Error code populated by the API on an error
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MMWave_startLink (MMWave_MCB* ptrMMWaveMCB, int32_t* errCode)
{
    int32_t                 retVal;
    rlRunTimeCalibConf_t    runTimeCalib;
    rlContModeEn_t          contModeEnable;

    /* Sensor is being started; reset the link status to get rid of the calibration flags. */
    ptrMMWaveMCB->linkStatus = ptrMMWaveMCB->linkStatus & (~(uint32_t)MMWAVE_RF_CALIBRATION_DONE);
    ptrMMWaveMCB->linkStatus = ptrMMWaveMCB->linkStatus & (~(uint32_t)MMWAVE_RF_CALIBRATION_FAILED);

    /* Initialize the runtime calibration configuration: */
    memset ((void *)&runTimeCalib, 0, sizeof(rlRunTimeCalibConf_t));

    /* Enable calibration: Always enable one time calibration and all calibrations are enabled */
    runTimeCalib.reportEn           = 0x1;
#ifndef SOC_XWR68XX
    runTimeCalib.oneTimeCalibEnMask = CSL_FMKR (4U,  4U,  1U) | /* LODIST calibration   */
                                      CSL_FMKR (8U,  8U,  1U) | /* PD calibration */
                                      CSL_FMKR (9U,  9U,  1U) | /* TX Power calibration */
                                      CSL_FMKR (10U, 10U, 1U);  /* RX gain calibration  */
#else
    runTimeCalib.oneTimeCalibEnMask = CSL_FMKR (4U,  4U,  1U); /* LODIST calibration   */
#endif
    /* Are we operating in Chirp or Continuous mode? */
    if ((ptrMMWaveMCB->calibrationCfg.dfeDataOutputMode == MMWave_DFEDataOutputMode_FRAME) ||
        (ptrMMWaveMCB->calibrationCfg.dfeDataOutputMode == MMWave_DFEDataOutputMode_ADVANCED_FRAME))
    {
        /******************************************************************************
         * FRAME Mode: Is calibration enabled?
         ******************************************************************************/
        if (ptrMMWaveMCB->calibrationCfg.u.chirpCalibrationCfg.enableCalibration == true)
	    {
            /* Do we need to configure periodic calibration? */
            if (ptrMMWaveMCB->calibrationCfg.u.chirpCalibrationCfg.enablePeriodicity == true)
            {
                /* YES: Enable all calibrations with the configured periodicity */
                runTimeCalib.periodicCalibEnMask = runTimeCalib.oneTimeCalibEnMask;
                runTimeCalib.calibPeriodicity    = ptrMMWaveMCB->calibrationCfg.u.chirpCalibrationCfg.periodicTimeInFrames;
            }

            /* Configure the calibrations: */
            retVal = rlRfRunTimeCalibConfig(RL_DEVICE_MAP_INTERNAL_BSS, &runTimeCalib);
            if (retVal != RL_RET_CODE_OK)
            {
                /* Error: Runtime calibration failed */
                *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_ECALTRIG, retVal);

                /* Setup the return value to indicate an error. */
                retVal   = MINUS_ONE;
                goto exit;
            }

            /***************************************************************************
             * SYNCHRONIZATION: We need to loop around till the BSS has completed the
             * ONE SHOT calibration [Done or Failed]. The application would have
             * received an asynchronous event through the callback function about the
             * failure.
             ***************************************************************************/
            MMWave_internalLinkSync(ptrMMWaveMCB, MMWAVE_RF_CALIBRATION_DONE | MMWAVE_RF_CALIBRATION_FAILED);

            /* Did the calibration fail? */
            if (ptrMMWaveMCB->linkStatus & MMWAVE_RF_CALIBRATION_FAILED)
            {
                /* YES: Calibration failed. Abort and dont start the sensor. */
                *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_ECALFAIL, 0);

                /* Setup the return value to indicate an error. */
                retVal   = MINUS_ONE;
                goto exit;
            }
        }

        /* Start the sensor: */
        retVal = rlSensorStart (RL_DEVICE_MAP_INTERNAL_BSS);
        if (retVal != RL_RET_CODE_OK)
        {
            /* Error: Starting the sensor failed */
            *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_ESENSOR, retVal);

            /* Setup the return value to indicate an error. */
            retVal   = MINUS_ONE;
            goto exit;
        }
	}
    else
    {
        /******************************************************************************
         * Continuous Mode: Is calibration enabled?
         ******************************************************************************/
        if (ptrMMWaveMCB->calibrationCfg.u.contCalibrationCfg.enableOneShotCalibration == true)
        {
            /* Configure the calibrations: */
            retVal = rlRfRunTimeCalibConfig(RL_DEVICE_MAP_INTERNAL_BSS, &runTimeCalib);
            if (retVal != RL_RET_CODE_OK)
            {
                /* Error: Runtime calibration failed */
                *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_ECALTRIG, retVal);

                /* Setup the return value to indicate an error. */
                retVal   = MINUS_ONE;
                goto exit;
            }

            /***************************************************************************
             * SYNCHRONIZATION: We need to loop around till the BSS has completed the
             * ONE SHOT calibration [Done or Failed]. The application would have
             * received an asynchronous event through the callback function about the
             * failure.
             ***************************************************************************/
            MMWave_internalLinkSync(ptrMMWaveMCB, MMWAVE_RF_CALIBRATION_DONE | MMWAVE_RF_CALIBRATION_FAILED);

            /* Did the calibration fail? */
            if (ptrMMWaveMCB->linkStatus & MMWAVE_RF_CALIBRATION_FAILED)
            {
                /* YES: Calibration failed. Abort and dont start enable the continuous mode. */
                *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_ECALFAIL, 0);

                /* Setup the return value to indicate an error. */
                retVal   = MINUS_ONE;
                goto exit;
            }
        }

        /* Start the sensor in continuous mode: */
        memset ((void*)&contModeEnable, 0, sizeof(rlContModeEn_t));

        /* Populate the continuous mode configuration: */
        contModeEnable.contModeEn = 1U;
        retVal = rlEnableContMode (RL_DEVICE_MAP_INTERNAL_BSS, &contModeEnable);
        if (retVal != RL_RET_CODE_OK)
        {
            /* Error: Unable to enable the continuous mode */
            *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_ECONTMODE, retVal);

            /* Setup the return value to indicate an error. */
            retVal   = MINUS_ONE;
            goto exit;
        }
    }

    /* Control comes here indicates that the sensor has been started successfully */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to stop the mmWave link.
 *
 *  @param[in]  ptrMMWaveMCB
 *      Pointer to the control MCB
 *  @param[out] errCode
 *      Error code populated by the API on an error
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MMWave_stopLink (const MMWave_MCB* ptrMMWaveMCB, int32_t* errCode)
{
    int32_t         retVal;
    rlContModeEn_t  contModeEnable;

    /* Are we operating in Chirp or Continuous mode? */
    if ((ptrMMWaveMCB->calibrationCfg.dfeDataOutputMode == MMWave_DFEDataOutputMode_FRAME) ||
        (ptrMMWaveMCB->calibrationCfg.dfeDataOutputMode == MMWave_DFEDataOutputMode_ADVANCED_FRAME))
    {
        /******************************************************************************
         * CHIRP or ADVANCED: Stop the sensor
         ******************************************************************************/
        retVal = rlSensorStop (RL_DEVICE_MAP_INTERNAL_BSS);
        if (retVal != RL_RET_CODE_OK)
        {
            /* Stopping the sensor failed. */
            if (retVal == (int32_t)RL_RET_CODE_FRAME_ALREADY_ENDED)
            {
                /* Special Case: This is treated as a warning */
                *errCode = MMWave_encodeError (MMWave_ErrorLevel_WARNING, MMWAVE_ESENSOR, retVal);
                retVal   = 0;
                goto exit;
            }
            else
            {
                /* All other are treated as FATAL error */
                *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_ESENSOR, retVal);
                retVal = MINUS_ONE;
                goto exit;
            }
        }
        else
        {
            /* Successfully stopped the sensor: */
            retVal = 0;
        }
    }
    else
    {
        /******************************************************************************
         * Continuous Mode: Disable continuous mode
         ******************************************************************************/
        memset ((void*)&contModeEnable, 0, sizeof(rlContModeEn_t));

        /* Populate the continuous mode configuration: */
        contModeEnable.contModeEn = 0U;
        retVal = rlEnableContMode (RL_DEVICE_MAP_INTERNAL_BSS, &contModeEnable);
        if (retVal != RL_RET_CODE_OK)
        {
            /* Error: Unable to disable the continuous mode */
            *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_ECONTMODE, retVal);
            retVal   = MINUS_ONE;
            goto exit;
        }
        else
        {
            /* Successfully stopped the sensor: */
            retVal = 0;
        }
    }

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to execute the mmWave link.
 *
 *  @param[in]  ptrMMWaveMCB
 *      Pointer to the control MCB
 *  @param[out] errCode
 *      Error code populated by the API on an error
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MMWave_executeLink (MMWave_MCB* ptrMMWaveMCB, int32_t* errCode)
{
    MMWave_SpawnFxnNode*    ptrSpawnFxnNode;
    uintptr_t               key;

    /* Semaphore has been posted; process any active jobs in the spawn active list.  */
    while (1)
    {
        /* Critical Section: The spawn active list is a critical resource which is accessed
         * from multiple contexts */
        key = HwiP_disable();
        ptrSpawnFxnNode = (MMWave_SpawnFxnNode*)MMWave_listRemove((MMWave_ListNode**)&ptrMMWaveMCB->ptrSpawnFxnActiveList);
        HwiP_restore (key);

        /* Is there an active entry to be processed */
        if (ptrSpawnFxnNode == NULL)
        {
            /* No other spawn nodes to be processed; we are done break out of the loop*/
            break;
        }
        else
        {
            /* Execute the spawn function */
            ptrSpawnFxnNode->spawnEntry (ptrSpawnFxnNode->arg);

            /* Critical Section: The spawn free list is a critical resource which is accessed
             * from multiple contexts */
            key = HwiP_disable();
            MMWave_listAdd ((MMWave_ListNode**)&ptrMMWaveMCB->ptrSpawnFxnFreeList, (MMWave_ListNode*)ptrSpawnFxnNode);
            HwiP_restore (key);
        }
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to open the mmWave Link module.
 *
 *  @param[in]  ptrMMWaveMCB
 *      Pointer to the control MCB
 *  @param[in] ptrCalibData
 *      Optional pointer to the calibration data which needs to be
 *      restored.
 *  @param[out] errCode
 *      Error code populated by the API on an error
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MMWave_openLink (MMWave_MCB* ptrMMWaveMCB, rlCalibrationData_t* ptrCalibData, int32_t* errCode)
{
    int32_t                     retVal = MINUS_ONE;
    rlRfCalMonFreqLimitConf_t   freqLimit;
    rlRfInitCalConf_t           rfInitCalib;
    rlRfCalMonTimeUntConf_t     timeCfg;

    /* Initialize the configurations: */
    memset ((void *)&freqLimit,   0, sizeof(rlRfCalMonFreqLimitConf_t));
    memset ((void *)&rfInitCalib, 0, sizeof(rlRfInitCalConf_t));
    memset ((void *)&timeCfg,     0, sizeof(rlRfCalMonTimeUntConf_t));

    /* Link is not operational: */
    ptrMMWaveMCB->linkStatus = 0U;

    /****************************************************************************************
     * Setup the RF Calibration Time unit:
     * - Periodicity is set to 1 Frame
     ****************************************************************************************/
    timeCfg.calibMonTimeUnit = 1U;
    retVal = rlRfSetCalMonTimeUnitConfig(RL_DEVICE_MAP_INTERNAL_BSS, &timeCfg);
    if (retVal != RL_RET_CODE_OK)
    {
        /* Error: Unable to set the calibration time unit */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_ECALPERIOD, retVal);
        retVal   = MINUS_ONE;
        goto exit;
    }

    /****************************************************************************************
     * Setup the RF Calibration Frequency limit:
     ****************************************************************************************/
#ifndef SOC_XWR68XX
    freqLimit.freqLimitLow  = ptrMMWaveMCB->openCfg.freqLimitLow;
    freqLimit.freqLimitHigh = ptrMMWaveMCB->openCfg.freqLimitHigh;
    retVal = rlRfSetCalMonFreqLimitConfig(RL_DEVICE_MAP_INTERNAL_BSS, &freqLimit);
    if (retVal != RL_RET_CODE_OK)
    {
        /* Error: Frequency Limit Calibration  */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_ECALCFG, retVal);
        retVal   = MINUS_ONE;
        goto exit;
    }
#endif

    /* Are we supporting custom calibration or not? */
    if (ptrMMWaveMCB->openCfg.useCustomCalibration == true)
    {
        /*********************************************************************
         * Custom Calibration: Use the application provided calibration mask
         *********************************************************************/
        rfInitCalib.calibEnMask = ptrMMWaveMCB->openCfg.customCalibrationEnableMask;
    }
    else
    {
        /*********************************************************************
         * Default: Enable all calibrations
         *********************************************************************/
        rfInitCalib.calibEnMask = CSL_FMKR (4U, 4U, 1U)     |   /* LODIST calibration            */
                                  CSL_FMKR (5U, 5U, 1U)     |   /* RX ADC DC offset calibration  */
                                  CSL_FMKR (6U, 6U, 1U)     |   /* HPF cutoff calibration        */
                                  CSL_FMKR (7U, 7U, 1U)     |   /* LPF cutoff calibration        */
                                  CSL_FMKR (8U, 8U, 1U)     |   /* Peak detector calibration     */
                                  CSL_FMKR (9U, 9U, 1U)     |   /* TX Power calibration          */
                                  CSL_FMKR (10U, 10U, 1U)   |   /* RX gain calibration           */
                                  CSL_FMKR (12U, 12U, 1U);      /* RX IQMM calibration           */
    }
    retVal = rlRfInitCalibConfig(RL_DEVICE_MAP_INTERNAL_BSS, &rfInitCalib);
    if (retVal != RL_RET_CODE_OK)
    {
        /* Error: Calibration init failed */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_ECALINIT, retVal);
        retVal   = MINUS_ONE;
        goto exit;
    }

    /****************************************************************************************
     * Set the channel configuration:
     ****************************************************************************************/
    retVal = rlSetChannelConfig (RL_DEVICE_MAP_INTERNAL_BSS, &ptrMMWaveMCB->openCfg.chCfg);
    if (retVal != RL_RET_CODE_OK)
    {
        /* Error: Set the channel configuration failed */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_ECHCFG, retVal);
        retVal   = MINUS_ONE;
        goto exit;
    }

    /****************************************************************************************
     * Set the ADC Output configuration:
     ****************************************************************************************/
    retVal = rlSetAdcOutConfig (RL_DEVICE_MAP_INTERNAL_BSS, &ptrMMWaveMCB->openCfg.adcOutCfg);
    if (retVal != RL_RET_CODE_OK)
    {
        /* Error: Set the ADC configuration failed */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EADCCFG, retVal);
        retVal   = MINUS_ONE;
        goto exit;
    }

    /****************************************************************************************
     * Set the low power mode configuration:
     ****************************************************************************************/
    retVal = rlSetLowPowerModeConfig(RL_DEVICE_MAP_INTERNAL_BSS, &ptrMMWaveMCB->openCfg.lowPowerMode);
    if (retVal != RL_RET_CODE_OK)
    {
        /* Error: Set the Low power configuration failed */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EPOWERCFG, retVal);
        retVal   = MINUS_ONE;
        goto exit;
    }

    /****************************************************************************************
     * Asynchronous event configuration:
     ****************************************************************************************/
    retVal = MMWave_deviceCfgAsyncEvent (ptrMMWaveMCB, errCode);
    if (retVal < 0)
    {
        /* Error: Asynchronous Event configuration failed; error code is already setup */
        goto exit;
    }

    /****************************************************************************************
     * Do we need to restore calibration data?
     ****************************************************************************************/
    if (ptrCalibData != NULL)
    {
        /* YES: Restore the calibration data */
        retVal = rlRfCalibDataRestore(RL_DEVICE_MAP_INTERNAL_BSS, ptrCalibData);
        if (retVal != RL_RET_CODE_OK)
        {
            /* Error: Calibration data restore failed */
            *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_ECALFAIL, retVal);
            retVal   = MINUS_ONE;
            goto exit;
        }

        /* As per the Link documentation; the BSS will consume the data and will send an
         * asynchronous message to indicate the result of the calibration based on the
         * calibration data which has been restored. So we need to wait on the status of
         * the operation. */
        MMWave_internalLinkSync(ptrMMWaveMCB, MMWAVE_RF_INITIALIZED | MMWAVE_RF_INIT_FAILED);

        /* Was the calibration restored succesfully? */
        if (ptrMMWaveMCB->linkStatus & MMWAVE_RF_INIT_FAILED)
        {
            /* NO: Calibration was not restored. The asynchronous message has been
             * passed to the application too. Setup the error code */
            *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_ECALFAIL, 0);
            retVal = MINUS_ONE;
            goto exit;
        }

        /* Calibration was restored successfully so we need to reset the link operational
         * status again. */
        ptrMMWaveMCB->linkStatus = 0U;
    }

    /****************************************************************************************
     * Initialize the RF:
     ****************************************************************************************/
    retVal = rlRfInit(RL_DEVICE_MAP_INTERNAL_BSS);
    if (retVal != RL_RET_CODE_OK)
    {
        /* Error: RF Initialization failed */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_ERFINIT, retVal);
        retVal   = MINUS_ONE;
        goto exit;
    }

    /***************************************************************************
     * SYNCHRONIZATION: We need to loop around till the BSS has performed the
     * RF Initialization; either a success or failure.
     ***************************************************************************/
    MMWave_internalLinkSync(ptrMMWaveMCB, MMWAVE_RF_INITIALIZED | MMWAVE_RF_INIT_FAILED);

    /* Was the RF Initialized succesfully? */
    if (ptrMMWaveMCB->linkStatus & MMWAVE_RF_INIT_FAILED)
    {
        /* NO: The RF initialization failed. The asynchronous event was passed to the application
         * The application can decode the exact reason for the failure. We simply set the error
         * code and report failure */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_ERFINIT, 0);
        retVal = MINUS_ONE;
        goto exit;
    }

    /* Setup the return value: */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to close the mmWave Link module.
 *
 *  @param[in]  ptrMMWaveMCB
 *      Pointer to the control MCB
 *  @param[out] errCode
 *      Error code populated by the API on an error
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MMWave_closeLink (MMWave_MCB* ptrMMWaveMCB, int32_t* errCode)
{
    /* Link is not operational: */
    ptrMMWaveMCB->linkStatus = 0U;
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to initialize the mmWave link.
 *
 *  @param[in]  ptrMMWaveMCB
 *      Pointer to the control MCB
 *  @param[out] errCode
 *      Error code populated by the API on an error
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MMWave_initLink (MMWave_MCB* ptrMMWaveMCB, int32_t* errCode)
{
    int32_t                 retVal = 0;
    MMWave_SpawnFxnNode*    ptrSpawnFxnNode;
    uint32_t                index;

    /* Initialize and setup the spawn lists */
    for (index = 0U; index < MMWAVE_MAX_NUM_SPAWN_LIST; index++)
    {
        /* Get the pointer to the spawn node */
        ptrSpawnFxnNode = &ptrMMWaveMCB->spawnTable[index];

        /* Initialize the spawn node */
        memset ((void*)ptrSpawnFxnNode, 0, sizeof(MMWave_SpawnFxnNode));

        /* Add the node to the free list: */
        MMWave_listAdd ((MMWave_ListNode**)&ptrMMWaveMCB->ptrSpawnFxnFreeList, (MMWave_ListNode*)ptrSpawnFxnNode);
    }

    /* Reset the active list: */
    ptrMMWaveMCB->ptrSpawnFxnActiveList = NULL;

    /* Setup and Initialize the CRC Driver: */
    if (MMWave_initCRC (ptrMMWaveMCB, errCode) == 0)
    {
        /* YES: Setup the mmWave Link */
        if (MMWave_initMMWaveLink (ptrMMWaveMCB, errCode) == 0)
        {
            /* Successfully initialized the mmWave Link: */
            retVal = 0;
        }
        else
        {
            /* Error: Unable to setup the mmWave Link */
            retVal = MINUS_ONE;
        }
    }
    else
    {
        /* Error: Unable to setup the CRC Driver */
        retVal = MINUS_ONE;
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to deinitialize the mmWave link.
 *
 *  @param[in]  ptrMMWaveMCB
 *      Pointer to the control MCB
 *  @param[out] errCode
 *      Error code populated by the API on an error
 *
 *  \ingroup  MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MMWave_deinitLink (MMWave_MCB* ptrMMWaveMCB, int32_t* errCode)
{
    int32_t                 retVal = 0;
    MMWave_SpawnFxnNode*    ptrSpawnFxnNode;
    uintptr_t               key;

    /* Deinitialize the mmWave Link: */
    retVal = MMWave_deinitMMWaveLink (ptrMMWaveMCB, errCode);
    if (retVal < 0)
    {
        /* Error: Unable to deinitialize the mmWave link; error code is already setup */
        goto exit;
    }

    /* Deinitialize the CRC channel: */
    retVal = MMWave_deinitCRC (ptrMMWaveMCB, errCode);
    if (retVal < 0)
    {
        /* Error: Unable to deinitialize the CRC channel; error code is already setup */
        goto exit;
    }

    /* Cycle through and cleanup the active spawn lists: There might be some entries in
     * the Active list which still need to be handled but because we are shutting down
     * the module we simply flush out the entries. */
    key = HwiP_disable();
    ptrSpawnFxnNode = (MMWave_SpawnFxnNode*)MMWave_listRemove ((MMWave_ListNode**)&gMMWave_MCB.ptrSpawnFxnActiveList);
    while (ptrSpawnFxnNode != NULL)
    {
        /* Add this back to the free list: */
        MMWave_listAdd ((MMWave_ListNode**)&ptrMMWaveMCB->ptrSpawnFxnFreeList, (MMWave_ListNode*)ptrSpawnFxnNode);

        /* Get the next entry from the active list: */
        ptrSpawnFxnNode = (MMWave_SpawnFxnNode*)MMWave_listRemove ((MMWave_ListNode**)&gMMWave_MCB.ptrSpawnFxnActiveList);
    }
    HwiP_restore (key);

    /* Control comes here implies that the deinitialization of the module was successful. */
    retVal = 0;

exit:
    return retVal;
}

