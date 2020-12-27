/**
 *   @file  framework_xwr14xx.c
 *
 *   @brief
 *      Framework Ported Layer for the XWR14xx Platform
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
#include <ti/drivers/test/common/framework.h>
#include <ti/drivers/test/common/framework_internal.h>

/**
 * @brief
 *  This is the TestFmk Hardware Attributes ported for the XWR14xx
 */
TestFmk_HWAttribute gTestFmkHwAttrib =
{
    SOC_XWR14XX_DSS_CHIRP_AVAIL_IRQ,
    SOC_XWR14XX_DSS_FRAME_START_IRQ
};

/**************************************************************************
 ********************** TestFmk-IPC Platform Functions ********************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      The function is used to receive a message from the remote IPC Module
 *
 *  @param[in] ptrTestFmk
 *      Pointer to the TestFmk module
 *  @param[out] ptrMessage
 *      Pointer to the TestFmk message populated by the API
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup TEST_FRAMEWORK_INTERNAL_FUNCTION
 *
 *  @retval
 *      0   -   No more message available
 *  @retval
 *      1   -   Message available
 *  @retval
 *      <0  -   Error
 */
int32_t TestFmk_IPCRemoteRecv (TestFmk_MCB* ptrTestFmk, TestFmk_Msg* ptrMessage, int32_t* errCode)
{
    /* On the XWR14xx: There is no remote IPC */
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to send a message to the remote IPC Module
 *
 *  @param[in] ptrTestFmk
 *      Pointer to the TestFmk module
 *  @param[in] ptrMessage
 *      Pointer to the TestFmk message to be sent
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup TEST_FRAMEWORK_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success   - 0
 *  @retval
 *      Error     - <0
 */
int32_t TestFmk_IPCRemoteSend (TestFmk_MCB* ptrTestFmk, TestFmk_Msg* ptrMessage, int32_t* errCode)
{
    /* On the XWR14xx: There is no remote IPC */
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to initialize the TestFmk Remote IPC module.
 *
 *  @param[in] ptrTestFmk
 *      Pointer to the TestFmk module
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup TEST_FRAMEWORK_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t TestFmk_IPCRemoteInit (TestFmk_MCB* ptrTestFmk, int32_t* errCode)
{
    /* On the XWR14xx: There is no remote IPC */
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to deinitialize the TestFmk Remote IPC module.
 *
 *  @param[in] ptrTestFmk
 *      Pointer to the TestFmk module
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup TEST_FRAMEWORK_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t TestFmk_IPCRemoteDeinit (TestFmk_MCB* ptrTestFmk, int32_t* errCode)
{
    /* On the XWR14xx: There is no remote IPC */
    return 0;
}

