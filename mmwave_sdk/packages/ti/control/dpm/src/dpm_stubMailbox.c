/**
 *   @file  dpm_stubMailbox.c
 *
 *   @brief
 *      DPM Remote/Distributed Domain Support through the Mailbox
 *      which is stubbed out for the MSS only devices since these domains
 *      are not supported.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2017 Texas Instruments, Inc.
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
#include <stdint.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/control/dpm/dpm.h>
#include <ti/control/dpm/include/dpm_internal.h>

/**************************************************************************
 **************************** Local Functions *****************************
 **************************************************************************/

static int32_t DPM_mboxStubInit (DPM_MCB* ptrDPM, int32_t* errCode);
static int32_t DPM_mboxStubSynch (DPM_MCB* ptrDPM, int32_t* errCode);
static int32_t DPM_mboxStubMsgSend (DPM_MCB* ptrDPM, DPM_Msg* ptrMessage, int32_t* errCode);
static int32_t DPM_mboxStubSendResult(DPM_MCB* ptrDPM, bool isAckNeeded, bool isRelayResult, DPM_Buffer* ptrResult, int32_t* errCode);
static int32_t DPM_mboxStubDeinit (DPM_MCB* ptrDPM, int32_t* errCode);

/**************************************************************************
 ********************** DPM Mailbox Domain Globals ************************
 **************************************************************************/

/**
 * @brief   Global DPM Remote/Distributed Domain Function Table
 */
DPM_DomainTable gDPMRemoteTable =
{
    &DPM_mboxStubInit,
    &DPM_mboxStubSynch,
    &DPM_mboxStubMsgSend,
    &DPM_mboxStubSendResult,
    &DPM_mboxStubDeinit
};

/**
 * @brief   Global DPM MCB: We need to use this since the
 * Mailbox does not allow an optional argument to be passed to the
 * callback function.
 */
DPM_MCB*  gDPM = NULL;

/**************************************************************************
 ********************** DPM Mailbox Domain Functions **********************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      Stub Function: This is not supported for MSS only devices
 *
 *  @param[in] ptrDPM
 *      Pointer to the DPM module
 *  @param[in] ptrMessage
 *      Pointer to the DPM message to be sent
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success   - 0
 *  @retval
 *      Error     - <0
 */
static int32_t DPM_mboxStubMsgSend (DPM_MCB* ptrDPM, DPM_Msg* ptrMessage, int32_t* errCode)
{
    *errCode = DPM_EINVAL;
    return -1;
}

/**
 *  @b Description
 *  @n
 *      Stub Function: This is not supported for MSS only devices
 *
 *  @param[in] ptrDPM
 *      Pointer to the DPM module
 *  @param[in] isAckNeeded
 *      Flag which indicates if the results need to be acknowledged
 *  @param[in] isRelayResult
 *      Flag which indicates if the results are being relayed or not
 *  @param[in] ptrResult
 *      Results which need to be sent to the remote domain
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     -   0
 *  @retval
 *      Error       -   <0
 */
static int32_t DPM_mboxStubSendResult
(
    DPM_MCB*    ptrDPM,
    bool        isAckNeeded,
    bool        isRelayResult,
    DPM_Buffer* ptrResult,
    int32_t*    errCode
)
{
    *errCode = DPM_EINVAL;
    return -1;
}

/**
 *  @b Description
 *  @n
 *      Stub Function: This is not supported for MSS only devices
 *
 *  @param[in] ptrDPM
 *      Pointer to the DPM module
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Synchronized    -   1
 *  @retval
 *      Unsynchronized  -   0
 *  @retval
 *      Error           -   <0
 */
static int32_t DPM_mboxStubSynch (DPM_MCB* ptrDPM, int32_t* errCode)
{
    *errCode = DPM_EINVAL;
    return -1;
}

/**
 *  @b Description
 *  @n
 *      Stub Function: This is not supported for MSS only devices
 *
 *  @param[in] ptrDPM
 *      Pointer to the DPM module
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Always returns 0
 */
static int32_t DPM_mboxStubInit (DPM_MCB* ptrDPM, int32_t* errCode)
{
    *errCode = DPM_EINVAL;
    return -1;
}

/**
 *  @b Description
 *  @n
 *      Stub Function: This is not supported for MSS only devices
 *
 *  @param[in] ptrDPM
 *      Pointer to the DPM module
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t DPM_mboxStubDeinit (DPM_MCB* ptrDPM, int32_t* errCode)
{
    *errCode = DPM_EINVAL;
    return -1;
}

