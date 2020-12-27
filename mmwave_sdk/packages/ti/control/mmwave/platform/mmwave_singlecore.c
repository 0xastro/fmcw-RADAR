/**
 *   @file  mmwave_singlecore.c
 *
 *   @brief
 *      The file implements the single core port for the mmWave control module
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
#include <ti/drivers/osal/DebugP.h>
#include <ti/control/mmwavelink/mmwavelink.h>
#include <ti/control/mmwave/mmwave.h>
#include <ti/control/mmwave/include/mmwave_internal.h>
#include <ti/common/sys_common.h>

/**************************************************************************
 *********************** Single Core mmWave Functions **************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      The function is used to get the mmWave link device information. On
 *      the Single Core the link always executes only on the MSS.
 *
 *  @param[in]  ptrMMWaveMCB
 *      Pointer to the mmWave control block
 *  @param[out]  devType
 *      Device Type populated by the API
 *  @param[out]  platform
 *      Platform populated by the API
 *
 *  \ingroup MMWAVE_CTRL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Device Type as specified by the mmWave Link
 */
void MMWave_deviceGetDeviceInfo (const MMWave_MCB* ptrMMWaveMCB, rlUInt8_t* devType, rlUInt8_t* platform)
{
    *devType  = RL_AR_DEVICETYPE_14XX;
    *platform = RL_PLATFORM_MSS;
}

/**
 *  @b Description
 *  @n
 *      The function is used to configure the asynchronous events. On the Single Core
 *      the MSS is the default handler for all asynchronous events and this is
 *      the default behavior for the BSS.
 *      We only need to configure if the start/stop asynchronous events are 
 *      enabled or disabled.
 *
 *  @param[in]  ptrMMWaveMCB
 *      Pointer to the mmWave control block
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  \ingroup MMWAVE_CTRL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MMWave_deviceCfgAsyncEvent(const MMWave_MCB* ptrMMWaveMCB, int32_t* errCode)
{
    int32_t         retVal;
    rlRfDevCfg_t    asyncEventCfg;

    /* Initialize the configuration: */
    memset ((void*)&asyncEventCfg, 0, sizeof(rlRfDevCfg_t));
    
    /*asyncEventCfg.aeDirection = 0 which means all async events are routed to MSS.
      asyncEventCfg.aeControl needs to be configured - see below.*/
    
    /*Disable Frame Start async event? */
    if(ptrMMWaveMCB->openCfg.disableFrameStartAsyncEvent)
    {
        asyncEventCfg.aeControl |= 0x1U;
    }   
    
    /*Disable Frame Stop async event? */
    if(ptrMMWaveMCB->openCfg.disableFrameStopAsyncEvent)
    {
        asyncEventCfg.aeControl |= 0x2U;
    }   

    /* Set the configuration in the link: */
    retVal = rlRfSetDeviceCfg(RL_DEVICE_MAP_INTERNAL_BSS, (rlRfDevCfg_t*)&asyncEventCfg);
    if (retVal != RL_RET_CODE_OK)
    {
        /* Error: Set the Async Event Direction Failed */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EASYNCEVENT, retVal);
        retVal   = MINUS_ONE;
        goto exit;
    }

    /* Control comes here implies either of the following:-
     * - Asynchronous event direction was configured successfully
     * - We are not the default event handler.
     * Either case we report success */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to initialize mmWave on Single Core.
 *
 *  @param[in]  ptrMMWaveMCB
 *      Pointer to the mmWave MCB
 *  @param[out] errCode
 *      Error code populated by the API on an error
 *
 *  \ingroup MMWAVE_CTRL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MMWave_deviceInitFxn(MMWave_MCB* ptrMMWaveMCB, int32_t* errCode)
{
    /* There are no other Single Core specific initializations to be done. */
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to deinitialize the mmWave on Single Core
 *
 *  @param[in]  ptrMMWaveMCB
 *      Pointer to the mmWave MCB
 *  @param[out] errCode
 *      Error code populated by the API on an error
 *
 *  \ingroup MMWAVE_CTRL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MMWave_deviceDeinitFxn(MMWave_MCB* ptrMMWaveMCB, int32_t* errCode)
{
    /* There are no other Single Core specific deinitializations to be done. */
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to handle the configuration for the Single Core. There
 *      is only the MSS executing in the system here so we dont need to have any
 *      additional handling of the configuration.
 *
 *  @param[in]  ptrMMWaveMCB
 *      Pointer to the mmWave MCB
 *  @param[in]  ptrControlCfg
 *      Pointer to the mmWave control configuration
 *  @param[out] errCode
 *      Error code populated by the API on an error
 *
 *  \ingroup MMWAVE_CTRL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Always success
 */
int32_t MMWave_deviceCfgFxn (const MMWave_MCB* ptrMMWaveMCB, MMWave_CtrlCfg* ptrControlCfg, int32_t* errCode)
{
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to open the mmWave control module on Single Core.
 *
 *  @param[in]  ptrMMWaveMCB
 *      Pointer to the mmWave MCB
 *  @param[out] errCode
 *      Error code populated by the API on an error
 *
 *  \ingroup MMWAVE_CTRL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Always returns 0
 */
int32_t MMWave_deviceOpenFxn(const MMWave_MCB* ptrMMWaveMCB, int32_t* errCode)
{
    /* Nothing additional to be done. */
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to close the mmWave control module on Single Core
 *
 *  @param[in]  ptrMMWaveMCB
 *      Pointer to the mmWave MCB
 *  @param[out] errCode
 *      Error code populated by the API on an error
 *
 *  \ingroup MMWAVE_CTRL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Always returns 0
 */
int32_t MMWave_deviceCloseFxn(const MMWave_MCB* ptrMMWaveMCB, int32_t* errCode)
{
    /* Nothing additional to be done. */
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to handle the link start for the Single Core. There
 *      is only the MSS executing in the system here so nothing additional needs
 *      to be done here.
 *
 *  @param[in]  ptrMMWaveMCB
 *      Pointer to the mmWave MCB
 *  @param[out] errCode
 *      Error code populated by the API on an error
 *
 *  \ingroup MMWAVE_CTRL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Always success
 */
int32_t MMWave_deviceStartFxn(const MMWave_MCB* ptrMMWaveMCB, int32_t* errCode)
{
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to handle the link stop for the Single Core. There
 *      is only the MSS executing in the system here so nothing additional needs
 *      to be done here.
 *
 *  @param[in]  ptrMMWaveMCB
 *      Pointer to the mmWave MCB
 *  @param[out] errCode
 *      Error code populated by the API on an error
 *
 *  \ingroup MMWAVE_CTRL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Always success
 */
int32_t MMWave_deviceStopFxn(const MMWave_MCB* ptrMMWaveMCB, int32_t* errCode)
{
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to synchronize mmWave on Single Core. Since only the
 *      MSS executes on the system the module is always synchronized.
 *
 *  @param[in]  ptrMMWaveMCB
 *      Pointer to the mmWave MCB
 *  @param[out] errCode
 *      Error code populated by the API on an error
 *
 *  \ingroup MMWAVE_CTRL_INTERNAL_FUNCTION
 *
 *  @retval
 *      1 - Always synchronized
 */
int32_t MMWave_deviceSyncFxn(const MMWave_MCB* ptrMMWaveMCB, int32_t* errCode)
{
    return 1;
}

/**
 *  @b Description
 *  @n
 *      The function is used to handle the link asynchronous events for the Single Core.
 *      There is only the MSS executing in the system here so we dont need to have
 *      any additional handling.
 *
 *  @param[in]  ptrMMWaveMCB
 *      Pointer to the mmWave control block
 *  @param[in]  msgId
 *      Message Identifier
 *  @param[in]  sbId
 *      Subblock identifier
 *  @param[in]  sbLen
 *      Length of the subblock
 *  @param[in]  payload
 *      Pointer to the payload buffer
 *
 *  \ingroup MMWAVE_CTRL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void MMWave_deviceEventFxn
(
    const MMWave_MCB*   ptrMMWaveMCB,
    uint16_t            msgId,
    uint16_t            sbId,
    uint16_t            sbLen,
    uint8_t*            payload
)
{
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to execute the Single Core SOC specific execution
 *      for mmWave. There is nothing to be done here.
 *
 *  @param[in]  ptrMMWaveMCB
 *      Pointer to the mmWave MCB
 *  @param[out] errCode
 *      Error code populated by the API on an error
 *
 *  \ingroup MMWAVE_CTRL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success
 */
int32_t MMWave_deviceExecuteFxn(MMWave_MCB* ptrMMWaveMCB, int32_t* errCode)
{
    return 0;
}

