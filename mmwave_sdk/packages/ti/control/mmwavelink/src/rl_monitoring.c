/****************************************************************************************
 * FileName     : rl_monitoring.c
 *
 * Description  : This file defines the functions required to configure monitoring reports from
 * mmWave Device.
 *
 ****************************************************************************************
 * (C) Copyright 2014, Texas Instruments Incorporated. - TI web address www.ti.com
 *---------------------------------------------------------------------------------------
 *
 *  Redistribution and use in source and binary forms, with or without modification,
 *  are permitted provided that the following conditions are met:
 *
 *    Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT  OWNER OR CONTRIBUTORS
 *  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, data, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT,  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/*
****************************************************************************************
* Revision History   :
*---------------------------------------------------------------------------------------
* Version  Date        Author             Defect No               Description
*---------------------------------------------------------------------------------------
* 0.1.0    12May2015   Kaushal Kukkar    -               Initial Version
*
* 0.8.6    24Jul2017   Jitendra Gupta    MMWL-30         RF/Analog Monitoring APIs
*                      Kaushal Kukkar    MMWL-23         Big Endian Support
*                      Kaushal Kukkar    MMWL-13         Fault Injection API
****************************************************************************************
*/

/******************************************************************************
 * INCLUDE FILES
 ******************************************************************************
 */
#include <stdlib.h>
#include <string.h>
#include <ti/control/mmwavelink/mmwavelink.h>
#include <ti/control/mmwavelink/include/rl_driver.h>
#include <ti/control/mmwavelink/include/rl_monitoring.h>
#include <ti/control/mmwavelink/include/rl_trace.h>


/******************************************************************************
 * GLOBAL VARIABLES/DATA-TYPES DEFINITIONS
 ******************************************************************************
 */

/******************************************************************************
 * FUNCTION DEFINITIONS
 ******************************************************************************
 */


/** @fn rlReturnVal_t rlRfDigMonEnableConfig(rlUInt8_t deviceMap, rlMonDigEnables_t* data)
*
*   @brief Sets the consolidated configuration of all digital monitoring
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data - Monitor digital enable configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   This API SB sets the consolidated configuration of all digital monitoring.
*
*   @note 1: Supported in xWR1243 ES3.0 and xWR1642 ES2.0 onwards.
*   @note 2: This API is not supported in IWR6843 ES1.0
*/
/* DesignId : MMWL_DesignId_073 */
/* Requirements : AUTORADAR_REQ-768 */
rlReturnVal_t rlRfDigMonEnableConfig(rlUInt8_t deviceMap, rlMonDigEnables_t* data)
{
    rlReturnVal_t retVal;

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || (RL_NULL_PTR == data))
    {
        /* set return error code */
        retVal = RL_RET_CODE_INVALID_INPUT;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_RF_MONITORING_CONF_SET_MSG,
                                       RL_RF_DIG_MON_EN_SB, (rlUInt8_t*)data,
                                       (rlUInt16_t)sizeof(rlMonDigEnables_t));
    }
    return retVal;
}

/** @fn rlReturnVal_t rlRfDigMonPeriodicConfig(rlUInt8_t deviceMap, rlDigMonPeriodicConf_t* data)
*
*   @brief Sets the consolidated configuration
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data - Digital monitor periodic configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Sets the consolidated configuration of all periodic digital monitoring within
*   radar sub-system.
*
*   @note 1: Supported in xWR1243 ES3.0 and xWR1642 ES2.0 onwards.
*   @note 2: This API is not supported in IWR6843 ES1.0
*/
/* DesignId : MMWL_DesignId_074 */
/* Requirements : AUTORADAR_REQ-768 */
rlReturnVal_t rlRfDigMonPeriodicConfig(rlUInt8_t deviceMap, rlDigMonPeriodicConf_t* data)
{
    rlReturnVal_t retVal;

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || (RL_NULL_PTR == data))
    {
        /* set return error code */
        retVal = RL_RET_CODE_INVALID_INPUT;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_RF_MONITORING_CONF_SET_MSG,
                                       RL_RF_DIG_MON_PERIODIC_CONF_SB, (rlUInt8_t*)data,
                                       (rlUInt16_t)sizeof(rlDigMonPeriodicConf_t));
    }
    return retVal;
}

/** @fn rlReturnVal_t rlRfAnaMonConfig(rlUInt8_t deviceMap, rlMonAnaEnables_t* data)
*
*   @brief This function contains the consolidated configuration of all analog
*   monitoring. The enabled monitoring functions are executed with a periodicity
*   of CAL_MON_TIME_UNITS (rlRfCalMonTimeUntConf_t.calibMonTimeUnit) number
*   of logical frames. The host should ensure that all the enabled monitors
*   can be completed in the available inter-frame times, based on the monitoring
*   durations. The time taken for each monitoring is not defined in this document.

*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data - Monitor analog enable configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Sets the consolidated configuration of all analog monitoring.
*
*   @note : This API is not supported in IWR6843 ES1.0
*/
/* DesignId : MMWL_DesignId_075 */
/* Requirements : AUTORADAR_REQ-769 */
rlReturnVal_t rlRfAnaMonConfig(rlUInt8_t deviceMap, rlMonAnaEnables_t* data)
{
    rlReturnVal_t retVal;

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || (RL_NULL_PTR == data))
    {
        /* set return error code */
        retVal = RL_RET_CODE_INVALID_INPUT;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_RF_MONITORING_CONF_SET_MSG,
                                       RL_RF_ANA_MON_EN_SB, (rlUInt8_t*)data,
                                       (rlUInt16_t)sizeof(rlMonAnaEnables_t));
    }
    return retVal;
}

/** @fn rlReturnVal_t rlRfTempMonConfig(rlUInt8_t deviceMap, rlTempMonConf_t* data)
*
*   @brief  This API configure the on chip temperature monitors and report the
*   soft results from the monitor. The corresponding monitors are collectively
*   named TEMPERATURE_MONITOR. These monitors observe the temperature near
*   various RF analog and digital modules using temperature sensors and GPADC
*   and compare them against configurable thresholds. The report is sent as an
*   async event RL_RF_AE_MON_TEMPERATURE_REPORT_SB. Sets information related to
*   temperature monitoring
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data - Temperature monitor configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Sets information related to temperature monitoring.
*   Once configured, radarSS will send Async event (RL_RF_AE_MON_TEMPERATURE_REPORT_SB) to
*   report monitoring data
*
*   @note : This API is not supported in IWR6843 ES1.0
*/
/* DesignId : MMWL_DesignId_076 */
/* Requirements : AUTORADAR_REQ-857 */
rlReturnVal_t rlRfTempMonConfig(rlUInt8_t deviceMap, rlTempMonConf_t* data)
{
    rlReturnVal_t retVal;

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || (RL_NULL_PTR == data))
    {
        /* set return error code */
        retVal = RL_RET_CODE_INVALID_INPUT;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_RF_MONITORING_CONF_SET_MSG,
                                       RL_RF_TEMP_MON_CONF_SB, (rlUInt8_t*)data,
                                       (rlUInt16_t)sizeof(rlTempMonConf_t));
    }
    return retVal;
}

/** @fn rlReturnVal_t rlRfRxGainPhMonConfig(rlUInt8_t deviceMap, rlRxGainPhaseMonConf_t* data)
*
*   @brief  This API is to set RX gain and phase monitoring config to device.
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data -  Rx gain phase monitoring configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   This is a monitoring configuration API, containing information related to RX gain and phase
*   monitoring. Once configured, radarSS will send Async event (RL_RF_AE_MON_RX_GAIN_PHASE_REPORT)
*   to report monitoring data.
*
*   @note : This API is not supported in IWR6843 ES1.0
*/
/* DesignId : MMWL_DesignId_077 */
/* Requirements : AUTORADAR_REQ-858 */
rlReturnVal_t rlRfRxGainPhMonConfig(rlUInt8_t deviceMap, rlRxGainPhaseMonConf_t* data)
{
    rlReturnVal_t retVal;

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) ||\
       (RL_NULL_PTR == data))
    {
        /* set return error code */
        retVal = RL_RET_CODE_INVALID_INPUT;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_RF_MONITORING_CONF_SET_MSG,
                                       RL_RF_RX_GAIN_PHASE_MON_CONF_SB, (rlUInt8_t*)data,
                                       (rlUInt16_t)sizeof(rlRxGainPhaseMonConf_t));
    }
    return retVal;
}

/** @fn rlReturnVal_t rlRfRxNoiseMonConfig(rlUInt8_t deviceMap, rlRxNoiseMonConf_t* data)
*
*   @brief Sets information related to RX noise monitoring.
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data - Rx noise monitor configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   This is a monitoring configuration API, containing
*   information related to RX noise monitoring of a profile.
*   Once configured, radarSS will send Async event (RL_RF_AE_MON_RX_NOISE_FIG_REPORT) to
*   report monitoring data.
*
*   @note 1: The noise monitor reports the real receivers noise figure. In complex receiver modes
*            (i.e., complex 1x, complex 2x and pseudoreal), the system noise figure is 3dB lower
*            (better) than the reported.
*   @note 2: This API is not supported in IWR6843 ES1.0
*/
/* DesignId : MMWL_DesignId_078 */
/* Requirements : AUTORADAR_REQ-859*/
rlReturnVal_t rlRfRxNoiseMonConfig(rlUInt8_t deviceMap, rlRxNoiseMonConf_t* data)
{
    rlReturnVal_t retVal;

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) ||\
       (RL_NULL_PTR == data))
    {
        /* set return error code */
        retVal = RL_RET_CODE_INVALID_INPUT;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_RF_MONITORING_CONF_SET_MSG,
                                       RL_RF_RX_NOISE_MON_CONF_SB, (rlUInt8_t*)data,
                                       (rlUInt16_t)sizeof(rlRxNoiseMonConf_t));
    }
    return retVal;
}

/** @fn rlReturnVal_t rlRfRxIfStageMonConfig(rlUInt8_t deviceMap, rlRxIfStageMonConf_t* data)
*
*   @brief Sets information related to RX IF filter attenuation monitoring
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data - Rx IF stage monitoring configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   This API is a Monitoring Configuration API, containing information
*   related to RX IF filter attenuation monitoring.
*   Once configured, radarSS will send Async event (RL_RF_AE_MON_RX_IF_STAGE_REPORT) to
*   report monitoring data
*
*   @note : This API is not supported in IWR6843 ES1.0
*/
/* DesignId : MMWL_DesignId_079 */
/* Requirements : AUTORADAR_REQ-860 */
rlReturnVal_t rlRfRxIfStageMonConfig(rlUInt8_t deviceMap, rlRxIfStageMonConf_t* data)
{
    rlReturnVal_t retVal;

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || (RL_NULL_PTR == data))
    {
        /* set return error code */
        retVal = RL_RET_CODE_INVALID_INPUT;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_RF_MONITORING_CONF_SET_MSG,
                                       RL_RF_RX_IFSTAGE_MON_CONF_SB, (rlUInt8_t*)data,
                                       (rlUInt16_t)sizeof(rlRxIfStageMonConf_t));
    }
    return retVal;
}

/** @fn rlReturnVal_t rlRfTxPowrMonConfig(rlUInt8_t deviceMap, rlTxPowMonConf_t* data)
*
*   @brief Sets information related to TX power monitoring.
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data - Tx power monitor configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   This API is a Monitoring Configuration API, containing information
*   related to TX0/1/2 power monitoring.
*   Once configured, radarSS will send Async event (RL_RF_AE_MON_TX0_POWER_REPORT,
*   RL_RF_AE_MON_TX1_POWER_REPORT, RL_RF_AE_MON_TX2_POWER_REPORT) to report monitoring data
*
*   @note 1: The TX[0:2] power monitoring accuracy degrades at high TX backoffs and is unreliable
*            for backoffs higher than 20dB
*   @note 2: This API is not supported in IWR6843 ES1.0
*/
/* DesignId : MMWL_DesignId_080 */
/* Requirements : AUTORADAR_REQ-861 */
rlReturnVal_t rlRfTxPowrMonConfig(rlUInt8_t deviceMap, rlAllTxPowMonConf_t *data)
{
    rlReturnVal_t retVal;
    /* Initialize Command and Response Sub Blocks */
    rlDriverMsg_t inMsg;
    rlDriverMsg_t outMsg = {0};
    rlUInt16_t numSbc = 0U;

    /* Initialize Command and Response Sub Blocks */
    rlPayloadSb_t inPayloadSb[RL_MAX_SB_IN_MSG] = {0};

    RL_LOGV_ARG0("rlRfTxPowrMonConfig starts...\n");

    /* Construct command packet */
    rlDriverConstructInMsg(RL_RF_MONITORING_CONF_SET_MSG, &inMsg, &inPayloadSb[0U]);

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
       (RL_NULL_PTR == data))
    {
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("rlRfTxPowrMonConfig, Invalid input \n");
    }
    else
    {
        /* check for NULL pointer */
        if ((RL_NULL_PTR != data->tx0PowrMonCfg))
        {
            /* Fill in-message Payload */
            rlDriverFillPayload(RL_RF_MONITORING_CONF_SET_MSG,
                                RL_RF_TX0_POW_MON_CONF_SB,
                                &inPayloadSb[numSbc],
                                (rlUInt8_t* )data->tx0PowrMonCfg,
                                (rlUInt16_t)sizeof(rlTxPowMonConf_t));
            numSbc++;
        }
        /* check for NULL pointer */
        if ((RL_NULL_PTR != data->tx1PowrMonCfg))
        {
            /* Fill in-message Payload */
            rlDriverFillPayload(RL_RF_MONITORING_CONF_SET_MSG,
                                RL_RF_TX1_POW_MON_CONF_SB,
                                &inPayloadSb[numSbc],
                                (rlUInt8_t* )data->tx1PowrMonCfg,
                                (rlUInt16_t)sizeof(rlTxPowMonConf_t));
            numSbc++;
        }
        /* check for NULL pointer */
        if ((RL_NULL_PTR != data->tx2PowrMonCfg))
        {
            /* Fill in-message Payload */
            rlDriverFillPayload(RL_RF_MONITORING_CONF_SET_MSG,
                                RL_RF_TX2_POW_MON_CONF_SB,
                                &inPayloadSb[numSbc],
                                (rlUInt8_t* )data->tx2PowrMonCfg,
                                (rlUInt16_t)sizeof(rlTxPowMonConf_t));
            numSbc++;
        }

        if (numSbc > 0U)
        {
            /* setting num of sub-block to inMsg */
            inMsg.opcode.nsbc = numSbc;

            /* Send Command to mmWave Radar Device */
            retVal = rlDriverCmdInvoke(deviceMap, inMsg, &outMsg);
        }
        else
        {
            retVal = RL_RET_CODE_INVALID_INPUT;
            RL_LOGE_ARG0("sub block is NULL\n");
        }
    }

    return retVal;
}

/** @fn rlReturnVal_t rlRfTxBallbreakMonConfig(rlUInt8_t deviceMap, rlAllTxBallBreakMonCfg_t* data)
*
*   @brief Sets information related to TX ball break detection.
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data - Tx ballbreak monitor config
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   This API is a Monitoring Configuration API, containing information
*   related to TX ball break detection.
*   Once configured, radarSS will send Async event (RL_RF_AE_MON_TX0_BALLBREAK_REPORT,
*   RL_RF_AE_MON_TX1_BALLBREAK_REPORT, RL_RF_AE_MON_TX2_BALLBREAK_REPORT) to report monitoring data
*
*   @note : This API is not supported in IWR6843 ES1.0
*/
/* DesignId : MMWL_DesignId_081 */
/* Requirements : AUTORADAR_REQ-862 */
rlReturnVal_t rlRfTxBallbreakMonConfig(rlUInt8_t deviceMap, rlAllTxBallBreakMonCfg_t* data)
{
    rlReturnVal_t retVal;
    /* Initialize Command and Response Sub Blocks */
    rlDriverMsg_t inMsg;
    rlDriverMsg_t outMsg = {0};
    rlUInt16_t numSbc = 0U;

    /* Initialize Command and Response Sub Blocks */
    rlPayloadSb_t inPayloadSb[RL_MAX_SB_IN_MSG] = {0};

    RL_LOGV_ARG0("rlRfTxBallBreakMonConfig starts...\n");

    /* Construct command packet */
    rlDriverConstructInMsg(RL_RF_MONITORING_CONF_SET_MSG, &inMsg, &inPayloadSb[0U]);

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
       (RL_NULL_PTR == data) )
    {
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("rlRfTxBallBreakMonConfig, Invalid input \n");
    }
    else
    {
        /* check for NULL pointer */
        if ((RL_NULL_PTR != data->tx0BallBrkMonCfg))
        {
            /* Fill in-message Payload */
            rlDriverFillPayload(RL_RF_MONITORING_CONF_SET_MSG,
                                RL_RF_TX0_BALLBREAK_MON_CONF_SB,
                                &inPayloadSb[numSbc],
                                (rlUInt8_t* )data->tx0BallBrkMonCfg,
                                (rlUInt16_t)sizeof(rlTxBallbreakMonConf_t));
            numSbc++;
        }
        /* check for NULL pointer */
        if ((RL_NULL_PTR != data->tx1BallBrkMonCfg))
        {
            /* Fill in-message Payload */
            rlDriverFillPayload(RL_RF_MONITORING_CONF_SET_MSG,
                                RL_RF_TX1_BALLBREAK_MON_CONF_SB,
                                &inPayloadSb[numSbc],
                                (rlUInt8_t* )data->tx1BallBrkMonCfg,
                                (rlUInt16_t)sizeof(rlTxBallbreakMonConf_t));
            numSbc++;
        }
        /* check for NULL pointer */
        if ((RL_NULL_PTR != data->tx2BallBrkMonCfg))
        {
            /* Fill in-message Payload */
            rlDriverFillPayload(RL_RF_MONITORING_CONF_SET_MSG,
                                RL_RF_TX2_BALLBREAK_MON_CONF_SB,
                                &inPayloadSb[numSbc],
                                (rlUInt8_t* )data->tx2BallBrkMonCfg,
                                (rlUInt16_t)sizeof(rlTxBallbreakMonConf_t));
            numSbc++;
        }

        if (numSbc > 0U)
        {
            /* setting num of sub-block to inMsg */
            inMsg.opcode.nsbc = numSbc;

            /* Send Command to mmWave Radar Device */
            retVal = rlDriverCmdInvoke(deviceMap, inMsg, &outMsg);
        }
        else
        {
            retVal = RL_RET_CODE_INVALID_INPUT;
            RL_LOGE_ARG0("sub block is NULL\n");
        }
    }

    return retVal;

}

/** @fn rlReturnVal_t rlRfTxGainPhaseMismatchMonConfig(rlUInt8_t deviceMap,
                                                        rlTxGainPhaseMismatchMonConf_t* data)
*
*   @brief Sets information related to TX gain and phase mismatch monitoring
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data - Tx gain phase mismatch monitor configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   This API is a Monitoring Configuration API, containing information
*   related to TX gain and phase mismatch monitoring.
*   Once configured, radarSS will send Async event (RL_RF_AE_MON_TX_GAIN_MISMATCH_REPORT) to
*   report monitoring data
*
*   @note 1: The TX2 has a fixed offset of -8dB gain and -8 degree phase with respect to TX0 and 
*            TX1 by design, user has to compensate these values in the gain and phase offset 
*            fields of this API for TX2.
*   @note 2: This API is not supported in IWR6843 ES1.0
*/
/* DesignId : MMWL_DesignId_082 */
/* Requirements : AUTORADAR_REQ-863 */
rlReturnVal_t rlRfTxGainPhaseMismatchMonConfig(rlUInt8_t deviceMap,
                                                rlTxGainPhaseMismatchMonConf_t* data)
{
    rlReturnVal_t retVal;

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || (RL_NULL_PTR == data))
    {
        /* set return error code */
        retVal = RL_RET_CODE_INVALID_INPUT;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_RF_MONITORING_CONF_SET_MSG,
                                       RL_RF_TX_GAIN_PHASE_MISMATCH_MON_CONF_SB,
                                       (rlUInt8_t*)data,
                                       (rlUInt16_t)sizeof(rlTxGainPhaseMismatchMonConf_t));
    }
    return retVal;
}


/** @fn rlReturnVal_t rlRfTxBpmMonConfig(rlUInt8_t deviceMap, rlAllTxBpmMonConf_t* data)
*
*   @brief Sets information related to TX BPM monitoring.
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data - Tx BPM monitor configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   This API is a Monitoring Configuration API, containing information
*   related to TX BPM monitoring.
*   Once configured, radarSS will send Async event (RL_RF_AE_MON_TX0_BPM_REPORT,
*   RL_RF_AE_MON_TX1_BPM_REPORT, RL_RF_AE_MON_TX2_BPM_REPORT) to report monitoring data
*
*   @note : This API is not supported in IWR6843 ES1.0
*/
/* DesignId : MMWL_DesignId_083 */
/* Requirements : AUTORADAR_REQ-864 */
rlReturnVal_t rlRfTxBpmMonConfig(rlUInt8_t deviceMap, rlAllTxBpmMonConf_t* data)
{
    rlReturnVal_t retVal;
    /* Initialize Command and Response Sub Blocks */
    rlDriverMsg_t inMsg;
    rlDriverMsg_t outMsg = {0};
    rlUInt16_t numSbc = 0U;

    /* Initialize Command and Response Sub Blocks */
    rlPayloadSb_t inPayloadSb[RL_MAX_SB_IN_MSG] = {0};

    RL_LOGV_ARG0("rlRfTxBpmMonConfig starts...\n");

    /* Construct command packet */
    rlDriverConstructInMsg(RL_RF_MONITORING_CONF_SET_MSG, &inMsg, &inPayloadSb[0U]);

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
       (RL_NULL_PTR == data) )
    {
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("rlRfTxBpmMonConfig, Invalid input \n");
    }
    else
    {
        /* check for NULL pointer */
        if ((RL_NULL_PTR != data->tx0BpmMonCfg))
        {
            /* Fill in-message Payload */
            rlDriverFillPayload(RL_RF_MONITORING_CONF_SET_MSG,
                                RL_RF_TX0_BPM_MON_CONF_SB,
                                &inPayloadSb[numSbc],
                                (rlUInt8_t* )data->tx0BpmMonCfg,
                                (rlUInt16_t)sizeof(rlTxBpmMonConf_t));
            numSbc++;
        }
        /* check for NULL pointer */
        if ((RL_NULL_PTR != data->tx1BpmMonCfg))
        {
            /* Fill in-message Payload */
            rlDriverFillPayload(RL_RF_MONITORING_CONF_SET_MSG,
                                RL_RF_TX1_BPM_MON_CONF_SB,
                                &inPayloadSb[numSbc],
                                (rlUInt8_t* )data->tx1BpmMonCfg,
                                (rlUInt16_t)sizeof(rlTxBpmMonConf_t));
            numSbc++;
        }
        /* check for NULL pointer */
        if ((RL_NULL_PTR != data->tx2BpmMonCfg))
        {
            /* Fill in-message Payload */
            rlDriverFillPayload(RL_RF_MONITORING_CONF_SET_MSG,
                                RL_RF_TX2_BPM_MON_CONF_SB,
                                &inPayloadSb[numSbc],
                                (rlUInt8_t* )data->tx2BpmMonCfg,
                                (rlUInt16_t)sizeof(rlTxBpmMonConf_t));
            numSbc++;
        }

        if (numSbc > 0U)
        {
            /* setting num of sub-block to inMsg */
            inMsg.opcode.nsbc = numSbc;

            /* Send Command to mmWave Radar Device */
            retVal = rlDriverCmdInvoke(deviceMap, inMsg, &outMsg);
        }
        else
        {
            retVal = RL_RET_CODE_INVALID_INPUT;
            RL_LOGE_ARG0("sub block is NULL\n");
        }
    }

    return retVal;
}

/** @fn rlReturnVal_t rlRfSynthFreqMonConfig(rlUInt8_t deviceMap, rlSynthFreqMonConf_t* data)
*
*   @brief Sets information related to synthesizer frequency
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data -Synthesizer frequency monitor configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   This API is a Monitoring Configuration API, containing information
*   related to synthesizer frequency monitoring during chirping.
*   Once configured, radarSS will send Async event (RL_RF_AE_MON_SYNTHESIZER_FREQ_REPORT) to
*   report monitoring data
*
*   @note : This API is not supported in IWR6843 ES1.0
*/
/* DesignId : MMWL_DesignId_084 */
/* Requirements : AUTORADAR_REQ-865 */
rlReturnVal_t rlRfSynthFreqMonConfig(rlUInt8_t deviceMap, rlSynthFreqMonConf_t* data)
{
    rlReturnVal_t retVal;

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || (RL_NULL_PTR == data))
    {
        /* set return error code */
        retVal = RL_RET_CODE_INVALID_INPUT;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_RF_MONITORING_CONF_SET_MSG,
                                       RL_RF_SYNTH_FREQ_MON_CONF_SB, (rlUInt8_t*)data,
                                       (rlUInt16_t)sizeof(rlSynthFreqMonConf_t));
    }
    return retVal;
}

/** @fn rlReturnVal_t rlRfExtAnaSignalsMonConfig(rlUInt8_t deviceMap,
                                                  rlExtAnaSignalsMonConf_t* data)
*
*   @brief Sets information related to external DC signals monitoring
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data - External anlog signal monitor config
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   This API is a Monitoring Configuration API, containing information
*   related to external DC signals monitoring (available only in xWR1642 & xWR1843).
*   Once configured, radarSS will send Async event (RL_RF_AE_MON_EXT_ANALOG_SIG_REPORT) to
*   report monitoring data
*
*   @note : This API is not supported in IWR6843 ES1.0
*/
/* DesignId : MMWL_DesignId_085 */
/* Requirements : AUTORADAR_REQ-866 */
rlReturnVal_t rlRfExtAnaSignalsMonConfig(rlUInt8_t deviceMap,
                                                    rlExtAnaSignalsMonConf_t* data)
{
    rlReturnVal_t retVal;

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) ||\
       (RL_NULL_PTR == data))
    {
        /* set return error code */
        retVal = RL_RET_CODE_INVALID_INPUT;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_RF_MONITORING_CONF_SET_MSG,
                                       RL_RF_EXT_ANA_SIGNALS_MON_CONF_SB, (rlUInt8_t*)data,
                                       (rlUInt16_t)sizeof(rlExtAnaSignalsMonConf_t));
    }
    return retVal;
}

/** @fn rlReturnVal_t rlRfTxIntAnaSignalsMonConfig(rlUInt8_t deviceMap,
                                                    rlAllTxIntAnaSignalsMonConf_t* data)
*
*   @brief Sets information related to TX Internal Analog Signals monitoring
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data - Tx internal analog signal monitoring configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   This API is a Monitoring Configuration API, containing information
*   related to TX Internal Analog Signals monitoring.
*   Once configured, radarSS will send Async event (RL_RF_AE_MON_TX0_INT_ANA_SIG_REPORT
*   RL_RF_AE_MON_TX1_INT_ANA_SIG_REPORT, RL_RF_AE_MON_TX2_INT_ANA_SIG_REPORT) to
*   report monitoring data
*
*   @note : This API is not supported in IWR6843 ES1.0
*/
/* DesignId : MMWL_DesignId_086 */
/* Requirements : AUTORADAR_REQ-867 */
rlReturnVal_t rlRfTxIntAnaSignalsMonConfig(rlUInt8_t deviceMap,
                                            rlAllTxIntAnaSignalsMonConf_t* data)
{
    rlReturnVal_t retVal;
    /* Initialize Command and Response Sub Blocks */
    rlDriverMsg_t inMsg;
    rlDriverMsg_t outMsg = {0};
    rlUInt16_t numSbc = 0U;

    /* Initialize Command and Response Sub Blocks */
    rlPayloadSb_t inPayloadSb[RL_MAX_SB_IN_MSG] = {0};

    RL_LOGV_ARG0("rlRfTxIntAnaSignalsMonConfig starts...\n");

    /* Construct command packet */
    rlDriverConstructInMsg(RL_RF_MONITORING_CONF_SET_MSG, &inMsg, &inPayloadSb[0U]);

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
       (RL_NULL_PTR == data) )
    {
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("rlRfTxIntAnaSignalsMonConfig, Invalid input \n");
    }
    else
    {
        /* check for NULL pointer */
        if ((RL_NULL_PTR != data->tx0IntAnaSgnlMonCfg))
        {
            /* Fill in-message Payload */
            rlDriverFillPayload(RL_RF_MONITORING_CONF_SET_MSG,
                                RL_RF_TX0_INT_ANA_SIGNALS_MON_CONF_SB,
                                &inPayloadSb[numSbc],
                                (rlUInt8_t* )data->tx0IntAnaSgnlMonCfg,
                                (rlUInt16_t)sizeof(rlTxIntAnaSignalsMonConf_t));
            numSbc++;
        }
        /* check for NULL pointer */
        if ((RL_NULL_PTR != data->tx1IntAnaSgnlMonCfg))
        {
            /* Fill in-message Payload */
            rlDriverFillPayload(RL_RF_MONITORING_CONF_SET_MSG,
                                RL_RF_TX1_INT_ANA_SIGNALS_MON_CONF_SB,
                                &inPayloadSb[numSbc],
                                (rlUInt8_t* )data->tx1IntAnaSgnlMonCfg,
                                (rlUInt16_t)sizeof(rlTxIntAnaSignalsMonConf_t));
            numSbc++;
        }
        /* check for NULL pointer */
        if ((RL_NULL_PTR != data->tx2IntAnaSgnlMonCfg))
        {
            /* Fill in-message Payload */
            rlDriverFillPayload(RL_RF_MONITORING_CONF_SET_MSG,
                                RL_RF_TX2_INT_ANA_SIGNALS_MON_CONF_SB,
                                &inPayloadSb[numSbc],
                                (rlUInt8_t* )data->tx2IntAnaSgnlMonCfg,
                                (rlUInt16_t)sizeof(rlTxIntAnaSignalsMonConf_t));
            numSbc++;
        }

        if (numSbc > 0U)
        {
            /* setting num of sub-block to inMsg */
            inMsg.opcode.nsbc = numSbc;

            /* Send Command to mmWave Radar Device */
            retVal = rlDriverCmdInvoke(deviceMap, inMsg, &outMsg);
        }
        else
        {
            retVal = RL_RET_CODE_INVALID_INPUT;
            RL_LOGE_ARG0("sub block is NULL\n");
        }
    }

    return retVal;
}

/** @fn rlReturnVal_t rlRfRxIntAnaSignalsMonConfig(rlUInt8_t deviceMap,
                                                    rlRxIntAnaSignalsMonConf_t* data)
*
*   @brief Sets information related to RX Internal Analog Signals monitoring
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data - Rx internal analog signal monitoring config
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   This API is a Monitoring Configuration API, containing information
*   related to RX Internal Analog Signals monitoring.
*   Once configured, radarSS will send Async event (RL_RF_AE_MON_RX_INT_ANALOG_SIG_REPORT) to
*   report monitoring data
*
*   @note : This API is not supported in IWR6843 ES1.0
*/
/* DesignId : MMWL_DesignId_087 */
/* Requirements : AUTORADAR_REQ-868 */
rlReturnVal_t rlRfRxIntAnaSignalsMonConfig(rlUInt8_t deviceMap,
                                                rlRxIntAnaSignalsMonConf_t* data)
{
    rlReturnVal_t retVal;

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) ||\
       (RL_NULL_PTR == data))
    {
        /* set return error code */
        retVal = RL_RET_CODE_INVALID_INPUT;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_RF_MONITORING_CONF_SET_MSG,
                                       RL_RF_RX_INT_ANA_SIGNALS_MON_CONF_SB, (rlUInt8_t*)data,
                                       (rlUInt16_t)sizeof(rlRxIntAnaSignalsMonConf_t));
    }
    return retVal;
}

/** @fn rlReturnVal_t rlRfPmClkLoIntAnaSignalsMonConfig(rlUInt8_t deviceMap,
                                                         rlPmClkLoIntAnaSignalsMonConf_t* data)
*
*   @brief Sets information related to Power Management, Clock generation and LO distribution
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data - PMCLK internal analog signal monitoring config
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   This API is a Monitoring Configuration API, containing information
*   related to Power Management,Clock generation and LO distribution
*   circuits' Internal Analog Signals monitoring.
*   Once configured, radarSS will send Async event (RL_RF_AE_MON_PMCLKLO_INT_ANA_SIG_REPORT) to
*   report monitoring data
*
*   @note : This API is not supported in IWR6843 ES1.0
*/
/* DesignId : MMWL_DesignId_088 */
/* Requirements : AUTORADAR_REQ-869 */
rlReturnVal_t rlRfPmClkLoIntAnaSignalsMonConfig(rlUInt8_t deviceMap,
                                            rlPmClkLoIntAnaSignalsMonConf_t* data)
{
    rlReturnVal_t retVal;

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) ||\
       (RL_NULL_PTR == data))
    {
        /* set return error code */
        retVal = RL_RET_CODE_INVALID_INPUT;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_RF_MONITORING_CONF_SET_MSG,
                                       RL_RF_PMCLKLO_INT_ANA_SIGNALS_MON_CONF_SB,
                                       (rlUInt8_t*)data,
                                       (rlUInt16_t)sizeof(rlPmClkLoIntAnaSignalsMonConf_t));
    }
    return retVal;
}

/** @fn rlReturnVal_t rlRfGpadcIntAnaSignalsMonConfig(rlUInt8_t deviceMap,
                                                       rlGpadcIntAnaSignalsMonConf_t* data)
*
*   @brief Sets information related to GPADC Internal Analog Signals monitoring
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data - GPADC internal analog signal monitoring config
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   This API is a Monitoring Configuration API, containing information
*   related to GPADC Internal Analog Signals monitoring.
*   Once configured, radarSS will send Async event (RL_RF_AE_MON_GPADC_INT_ANA_SIG_REPORT) to
*   report monitoring data.
*
*   @note : This API is not supported in IWR6843 ES1.0
*/
/* DesignId : MMWL_DesignId_089 */
/* Requirements : AUTORADAR_REQ-870 */
rlReturnVal_t rlRfGpadcIntAnaSignalsMonConfig(rlUInt8_t deviceMap,
                                            rlGpadcIntAnaSignalsMonConf_t* data)
{
    rlReturnVal_t retVal;

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) ||\
       (RL_NULL_PTR == data))
    {
        /* set return error code */
        retVal = RL_RET_CODE_INVALID_INPUT;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_RF_MONITORING_CONF_SET_MSG,
                                       RL_RF_GPADC_INT_ANA_SIGNALS_MON_CONF_SB,
                                       (rlUInt8_t*)data,
                                       (rlUInt16_t)sizeof(rlGpadcIntAnaSignalsMonConf_t));
    }
    return retVal;
}

/** @fn rlReturnVal_t rlRfPllContrlVoltMonConfig(rlUInt8_t deviceMap,
                                                  rlPllContrVoltMonConf_t* data)
*
*   @brief Sets information related to APLL and Synthesizer's control voltage signals monitoring.
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data - PLL control voltage monitoring config
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   This API is a Monitoring Configuration API, containing information
*   related to APLL and Synthesizer's control voltage signals monitoring
*   Once configured, radarSS will send Async event (RL_RF_AE_MON_PLL_CONTROL_VOLT_REPORT) to
*   report monitoring data
*
*   @note : This API is not supported in IWR6843 ES1.0
*/
/* DesignId : MMWL_DesignId_090 */
/* Requirements : AUTORADAR_REQ-871 */
rlReturnVal_t rlRfPllContrlVoltMonConfig(rlUInt8_t deviceMap,
                                                    rlPllContrVoltMonConf_t* data)
{
    rlReturnVal_t retVal;

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) ||\
       (RL_NULL_PTR == data))
    {
        /* set return error code */
        retVal = RL_RET_CODE_INVALID_INPUT;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_RF_MONITORING_CONF_SET_MSG,
                                       RL_RF_PLL_CONTROL_VOLT_MON_CONF_SB,
                                       (rlUInt8_t*)data,
                                       (rlUInt16_t)sizeof(rlPllContrVoltMonConf_t));
    }
    return retVal;
}

/** @fn rlReturnVal_t rlRfDualClkCompMonConfig(rlUInt8_t deviceMap, rlDualClkCompMonConf_t* data)
*
*   @brief Sets information related to the DCC based clock frequency monitoring.
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data - Dual clock comp monitoring config
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   This API is a Monitoring Configuration API, containing information
*   related to the DCC based clock frequency monitoring.
*   Once configured, radarSS will send Async event (RL_RF_AE_MON_DCC_CLK_FREQ_REPORT) to report
*   monitoring data
*
*   @note : This API is not supported in IWR6843 ES1.0
*/
/* DesignId : MMWL_DesignId_091 */
/* Requirements : AUTORADAR_REQ-872 */
rlReturnVal_t rlRfDualClkCompMonConfig(rlUInt8_t deviceMap,
                                                    rlDualClkCompMonConf_t* data)
{
    rlReturnVal_t retVal;

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) ||\
       (RL_NULL_PTR == data))
    {
        /* set return error code */
        retVal = RL_RET_CODE_INVALID_INPUT;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_RF_MONITORING_CONF_SET_MSG,
                                       RL_RF_DUAL_CLOCK_COMP_MON_CONF_SB,
                                       (rlUInt8_t*)data,
                                       (rlUInt16_t)sizeof(rlDualClkCompMonConf_t));
    }
    return retVal;
}

/** @fn rlReturnVal_t rlRfRxIfSatMonConfig(rlUInt8_t deviceMap, rlRxSatMonConf_t* data)
*
*   @brief Sets information related to RX saturation detector monitoring.
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data - Rx saturation monitoring config
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   This API is a Monitoring Configuration API, containing information
*   related to RX saturation detector monitoring.
*
*   @note : This API is not supported in IWR6843 ES1.0
*/
/* DesignId : MMWL_DesignId_092 */
/* Requirements : AUTORADAR_REQ-873 */
rlReturnVal_t rlRfRxIfSatMonConfig(rlUInt8_t deviceMap,
                                                rlRxSatMonConf_t* data)
{
    rlReturnVal_t retVal;

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) ||\
       (RL_NULL_PTR == data))
    {
        /* set return error code */
        retVal = RL_RET_CODE_INVALID_INPUT;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_RF_MONITORING_CONF_SET_MSG,
                                       RL_RF_RX_SATURATION_MON_CONF_SB,
                                       (rlUInt8_t*)data,
                                       (rlUInt16_t)sizeof(rlRxSatMonConf_t));
    }
    return retVal;
}

/** @fn rlReturnVal_t rlRfRxSigImgMonConfig(rlUInt8_t deviceMap, rlSigImgMonConf_t* data)
*
*   @brief Sets information related to signal and image band energy.
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data - Signal img monitoring config
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   This API is a Monitoring Configuration API, containing information
*   related to signal and image band energy. The Monitoring report is available as CQ1
*   (part of CQ) in CQ RAM. The application should transfer the report every chirp.
*
*   @note : This API is not supported in IWR6843 ES1.0
*/
/* DesignId : MMWL_DesignId_093 */
/* Requirements : AUTORADAR_REQ-874 */
rlReturnVal_t rlRfRxSigImgMonConfig(rlUInt8_t deviceMap,
                                                 rlSigImgMonConf_t* data)
{
    rlReturnVal_t retVal;

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) ||\
       (RL_NULL_PTR == data))
    {
        /* set return error code */
        retVal = RL_RET_CODE_INVALID_INPUT;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_RF_MONITORING_CONF_SET_MSG,
                                       RL_RF_RX_SIGIMG_MON_CONF_SB,
                                       (rlUInt8_t*)data,
                                       (rlUInt16_t)sizeof(rlSigImgMonConf_t));
    }
    return retVal;
}

/** @fn rlReturnVal_t rlRfRxMixerInPwrConfig(rlUInt8_t deviceMap, rlSigImgMonConf_t* data)
*
*   @brief Sets information related to RX mixer input power monitoring.
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data - RX mixer input power config
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   This API is a Monitoring Configuration API, containing information
*   related to RX mixer input power monitoring.
*
*   @note : This API is not supported in IWR6843 ES1.0
*/
/* DesignId : MMWL_DesignId_094 */
/* Requirements : AUTORADAR_REQ-875 */
rlReturnVal_t rlRfRxMixerInPwrConfig(rlUInt8_t deviceMap,
                                                 rlRxMixInPwrMonConf_t* data)
{
    rlReturnVal_t retVal;

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) ||\
       (RL_NULL_PTR == data))
    {
        /* set return error code */
        retVal = RL_RET_CODE_INVALID_INPUT;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_RF_MONITORING_CONF_SET_MSG,
                                       RL_RF_RX_MIXER_IN_POWR_CONF_SB,
                                       (rlUInt8_t*)data,
                                       (rlUInt16_t)sizeof(rlRxMixInPwrMonConf_t));
    }
    return retVal;
}

/** @fn rlReturnVal_t rlRfAnaFaultInjConfig(rlUInt8_t deviceMap, rlAnaFaultInj_t* data)
*
*   @brief Sets information related to RF fault injection
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data - Fault injection config
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   This API is a fault injection configuration API.
*
*   @note 1: Supported in xWR1243 ES3.0 and xWR1642 ES2.0 onwards.
*   @note 2: This API is not supported in IWR6843 ES1.0
*/
/* DesignId : MMWL_DesignId_095 */
/* Requirements : AUTORADAR_REQ-876 */
rlReturnVal_t rlRfAnaFaultInjConfig(rlUInt8_t deviceMap,
                                    rlAnaFaultInj_t* data)
{
    rlReturnVal_t retVal;

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) ||\
       (RL_NULL_PTR == data))
    {
        /* set return error code */
        retVal = RL_RET_CODE_INVALID_INPUT;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_RF_MONITORING_CONF_SET_MSG,
                                       RL_RF_FAULT_INJECTION_CONF_SB,
                                       (rlUInt8_t*)data,
                                       (rlUInt16_t)sizeof(rlAnaFaultInj_t));
    }
    return retVal;
}

/*
 * END OF rl_monitoring.c FILE
 */
