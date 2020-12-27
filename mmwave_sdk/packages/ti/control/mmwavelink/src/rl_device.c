/****************************************************************************************
 * FileName     : rl_device.c
 *
 * Description  : This file defines the functions required to Control mmwave radar Device.
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
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
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
 * 0.5.2    23Sep2016   Kaushal Kukkar    AUTORADAR-541   xWR1642 Support
 *
 * 0.6.0    15Nov2016   Kaushal Kukkar    AUTORADAR-666   Logging Feature
 *                      Kaushal Kukkar    AUTORADAR-716   Cascade API change
 *
 * 0.7.0    11May2017   Kaushal Kukkar    MMWSDK-362      LDRA static analysis Issue Fix
 *
 * 0.8.6    24Jul2017   Jitendra Gupta    MMWL-19         MSS Test pattern, Monitoring APIs
 *                      Kaushal Kukkar    MMWL-23         Big Endian Support
 *                      Jitendra Gupta    MMWL-26         MSS Data path Get APIs
 *
 * 0.9.1       -        Jitendra Gupta    MMWL-5          Code size optimization
 ****************************************************************************************
 */

/******************************************************************************
 * INCLUDE FILES
 ******************************************************************************
 */
#include <stdlib.h>
#include <string.h>
#include <ti/control/mmwavelink/mmwavelink.h>
#include <ti/control/mmwavelink/include/rl_device.h>
#include <ti/control/mmwavelink/include/rl_driver.h>
#include <ti/control/mmwavelink/include/rl_messages.h>
#include <ti/control/mmwavelink/include/rl_controller.h>
#include <ti/control/mmwavelink/include/rl_trace.h>

/******************************************************************************
 * GLOBAL VARIABLES/DATA-TYPES DEFINITIONS
 ******************************************************************************
 */

/******************************************************************************
 * FUNCTION DEFINITIONS
 ******************************************************************************
 */

/** @fn rlReturnVal_t rlDevicePowerOn(rlUInt8_t deviceMap, rlClientCbs_t clientCb)
*
*   @brief Bring mmwave Device Out of Reset
*   @param[in] deviceMap - bitmap of all the connected Device
*   @param[in] clientCb - Client Callbacks for OS/SPI/Interrupts etc
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Bring mmwave Device Out of Reset. Application should wait for async event of
*   subBlock RL_DEV_AE_MSSPOWERUPDONE_SB (in case of AWR1243) before issuing any other APIs
*/
/* DesignId : MMWL_DesignId_004 */
/* Requirements : AUTORADAR_REQ-707 */
rlReturnVal_t rlDevicePowerOn(rlUInt8_t deviceMap, rlClientCbs_t clientCb)
{
    rlReturnVal_t retVal;
    rlUInt8_t index = 0U;
    /* get rlDriver global structure pointer */
    rlDriverData_t *rlDrvData = rlDriverGetHandle();

    /* if driver is already initialized */
    if ((rlDrvData != NULL) && (rlDrvData->isDriverInitialized == 1U))
    {
        /* DeInitialize Device */
        retVal = rlDevicePowerOff();
    }
    else
    {
        retVal = RL_RET_CODE_OK;
    }

    /* if there is no error found from above conditioning then go ahead and poweron
        mmwavelink & device */
    if (retVal == RL_RET_CODE_OK)
    {
        /* Initialize Host Communication Protocol Driver */
        if (rlDriverInit(deviceMap, clientCb) < 0)
        {
            /* if driver Init failed then set return error code */
            retVal += RL_RET_CODE_RADAR_IF_ERROR;
        }
        else
        {
            /* Power up mmwave Device */
            do
            {
                /* loop for all devices connected to device/Host */
                if ((deviceMap & (1U << index)) != 0U)
                {
                    /* Enable the 12xx device where it will power up the device and read
                        first Async Event */
                    if (clientCb.devCtrlCb.rlDeviceEnable(index) < 0)
                    {
                        /* set return error code */
                        retVal += RL_RET_CODE_RADAR_IF_ERROR;
                        RL_LOGE_ARG0("mmWaveLink: Enabling device failed \n");
                        /* if device enable is failed the de-init mmwavelink driver */
                        (void)rlDriverDeInit();
                        RL_LOGW_ARG0("mmWaveLink Driver DeInit done\n");
                    }
                    /* Reset device Index in DeviceMap for which device has been enabled */
                    deviceMap &= ~(1U << index);
                }
                /* increment device index */
                index++;
            }
            while ((deviceMap != 0U) && (index < RL_DEVICE_CONNECTED_MAX));
        }
    }
    RL_LOGV_ARG0("mmWaveLink Power Up completes\n");

    return retVal;
}

/** @fn rlReturnVal_t rlDeviceAddDevices(rlUInt8_t deviceMap)
*
*   @brief Bring mmwave Device Out of Reset
*   @param[in] deviceMap - bitmap of devices to be connected
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Bring mmwave Device Out of Reset. Application should wait for async event subBlock
*   RL_DEV_AE_MSSPOWERUPDONE_SB before issuing any other APIs. This API is
*   valid only for casecade mode of AWR1243 mmWave device when mmWaveLink instance
*   is running on External Host Processor.
*/
/* DesignId : MMWL_DesignId_021 */
/* Requirements : AUTORADAR_REQ-757 */
rlReturnVal_t rlDeviceAddDevices(rlUInt8_t deviceMap)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDeviceAddDevices starts\n");

    /* add device for requested deviceMap */
    retVal = rlDriverAddDevice(deviceMap);

    RL_LOGV_ARG0("rlDeviceAddDevices completes\n");

    return retVal;
}

/** @fn rlReturnVal_t rlDeviceRemoveDevices(rlUInt8_t deviceMap)
*
*   @brief Removes connected mmwave devices
*   @param[in] deviceMap - Bitmap of mmwave devices to be disconnected
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   @brief Removes mmwave devices and also shuts down the devices. This API is
*   valid only for casecade mode of AWR1243 mmWave device when mmWaveLink instance
*   is running on External Host Processor.
*/
/* DesignId : MMWL_DesignId_034 */
/* Requirements : AUTORADAR_REQ-757 */
rlReturnVal_t rlDeviceRemoveDevices(rlUInt8_t deviceMap)
{
    rlReturnVal_t retVal = RL_RET_CODE_OK;
    rlUInt8_t index = 0U;
    /* get rlDriver global structure pointer */
    rlDriverData_t *rlDrvData = rlDriverGetHandle();

    /* if driver is already initialized and deviceDisable API is not NULL */
    if ((rlDrvData != NULL) && (rlDrvData->isDriverInitialized == 1U) && \
        (rlDrvData->clientCtx.devCtrlCb.rlDeviceDisable != RL_NULL_PTR))
    {
        rlUInt8_t lclDeviceMap = deviceMap;
        do
        {
            /* loop for device index connected to device/Host */
            if ((lclDeviceMap & (1U << index)) != 0U)
            {
                /* Enable the 12xx device where it will power up the
                 * device and read first Async Event
                 */
                if (rlDrvData->clientCtx.devCtrlCb.rlDeviceDisable(index)
                   < 0)
                {
                    /* Device Power Off Failed */
                    retVal += RL_RET_CODE_RADAR_IF_ERROR;
                    RL_LOGE_ARG0("mmWave device Power Off failed\n");
                }
                /* Reset device Index in DeviceMap for which device has been disabled */
                lclDeviceMap &= ~(1U << index);
            }
            /* increment device index */
            index++;
        }
        while ((lclDeviceMap != 0U) && (index < RL_DEVICE_CONNECTED_MAX));

        RL_LOGI_ARG0("mmwave device Power Off Successful\n");
        /* Remove the devices from Driver */
        retVal += rlDriverRemoveDevices(deviceMap);
    }
    else
    {
        /* set return error code */
        retVal += RL_RET_CODE_SELF_ERROR;
        RL_LOGD_ARG0("Either rlDrvData is NULL or not initialised\n");
    }

    RL_LOGV_ARG0("rlDeviceRemoveDevices complete...\n");
    return retVal;
}

/** @fn rlReturnVal_t rlDevicePowerOff(void)
*
*   @brief Shutdown mmwave Device
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   @brief Stops mmwave operations and also shuts down mmwave device
*/
/* DesignId :  MMWL_DesignId_005 */
/* Requirements : AUTORADAR_REQ-711 */
rlReturnVal_t rlDevicePowerOff(void)
{
    rlReturnVal_t retVal = RL_RET_CODE_OK;
    rlUInt8_t index = 0U;
    /* get rlDriver global structure pointer */
    rlDriverData_t *rlDrvData = rlDriverGetHandle();

    /* if driver is already initialized and deviceDisable API is not NULL */
    if ((rlDrvData != NULL) && (rlDrvData->isDriverInitialized == 1U) && \
        (rlDrvData->clientCtx.devCtrlCb.rlDeviceDisable != RL_NULL_PTR))
    {
        rlUInt8_t deviceMap = rlDrvData->deviceMap;
        /* disable all connected devices */
        do
        {
            /* if device Index is valid out of deviceMap */
            if ((deviceMap & (1U << index)) != 0U)
            {
                /* Enable the 12xx device where it will power up the
                 * device and read first Async Event
                 */
                if (rlDrvData->clientCtx.devCtrlCb.rlDeviceDisable(index)
                   < 0)
                {
                    /* Device Power Off Failed */
                    retVal += RL_RET_CODE_RADAR_IF_ERROR;
                    RL_LOGE_ARG0("mmWave device Power Off failed\n");
                }
                /* Reset device Index in DeviceMap for which device has been disabled */
                deviceMap &= ~(1U << index);
            }
            /* increment device index */
            index++;
        }
        while ((deviceMap != 0U) || (index < RL_DEVICE_CONNECTED_MAX));

        RL_LOGI_ARG0("Power Off Successful\n");
        /* DeInitialize Host Communication Protocol Driver */
        retVal += rlDriverDeInit();
        RL_LOGV_ARG0("Driver De-initialization complete\n");
    }
    else
    {
        /* set return error code */
        retVal += RL_RET_CODE_SELF_ERROR;
        RL_LOGD_ARG0("Either rlDrvData is NULL or not initialised\n");
    }

    RL_LOGV_ARG0("Device power-off complete...\n");
    return retVal;
}

/** @fn rlReturnVal_t rlDeviceRfStart(rlUInt8_t deviceMap)
*
*   @brief Enables mmwave RF/Analog Sub system
*   @param[in] deviceMap - Bitmap of devices to send the message
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   @brief It enables RF/Analog Subsystem.
*/
/* DesignId : MMWL_DesignId_027 */
/* Requirements : AUTORADAR_REQ-761 */
rlReturnVal_t rlDeviceRfStart(rlUInt8_t deviceMap)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDeviceRfStart starts...\n");

    /* check if deviceIndex is out of defined value */
    if (rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK)
    {
        /* set return error code */
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("Device map id is invalid\n");
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_DEV_POWERUP_MSG,
                                       RL_SYS_RF_POWERUP_SB, NULL, 0U);
    }

    RL_LOGV_ARG0("rlDeviceRfStart complete...\n");

    return retVal;
}

/** @fn rlReturnVal_t rlDeviceFileDownload(rlUInt8_t deviceMap,
*                          rlFileData_t* data, rlUInt16_t remChunks)
*
*   @brief Download mmwave Firmware/Patches over SPI
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data  - File Chunk
*   @param[in] remChunks -  Number of remaining Chunks
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Download mmwave Firmware/Patches over SPI. This API is valid only for AWR1243
*   mmWave device when mmWaveLink instance is running on External Host Processor.
*/
/* DesignId :  MMWL_DesignId_006 */
/* Requirements : AUTORADAR_REQ-708 */
rlReturnVal_t rlDeviceFileDownload(rlUInt8_t deviceMap, rlFileData_t* data,
                                  rlUInt16_t remChunks)
{
    rlReturnVal_t retVal;

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
        (RL_NULL_PTR == data))
    {
        /* set error code if DeviceMAP is invalid or data pointer is null */
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("Invalid input");
    }
    /* This API is valid only when mmWaveLink instance is running on
       External Host Processor */
    else if (RL_PLATFORM_HOST != rlDriverGetPlatformId())
    {
        /* set error code of Platform is not set to HOST */
        retVal = RL_RET_CODE_API_NOT_SUPPORTED;
    }
    else
    {
        /* LDRA waiver   8 D - DD data flow anomalies found- */
        /* Initialize in-message structure to zero */
        rlDriverMsg_t inMsg = {0};
        /* Initialize out-message structure to zero */
        rlDriverMsg_t outMsg = {0};
        /* Initialize in-payload sub-block structure to zero */
        rlPayloadSb_t inPayloadSb = {0};

        /* Construct command packet */
        rlDriverConstructInMsg(RL_DEV_FILE_DOWNLOAD_MSG, &inMsg, &inPayloadSb);

        /* set the remaining chunk in command message */
        inMsg.remChunks = remChunks;
        /* Fill in-message Payload */
        /* AR_CODE_REVIEW MR:R.11.1  <APPROVED> "conversion required." */
        /*LDRA_INSPECTED 95 S */
        rlDriverFillPayload(RL_DEV_FILE_DOWNLOAD_MSG, RL_SYS_FILE_DWLD_SB, &inPayloadSb,\
                            (rlUInt8_t*)&data->fData[0U], (rlUInt16_t)data->chunkLen);

        /* Send Command to mmWave Radar Device */
        /* LDRA waiver 45 D - can't be NULL */
        /* LDRA waiver 45 D - can't be NULL */
        retVal = rlDriverCmdInvoke(deviceMap, inMsg, &outMsg);
    }
    return retVal;
}

/** @fn rlReturnVal_t rlDeviceGetMssVersion(rlUInt8_t deviceMap, rlFwVersionParam_t *data)
*
*   @brief Get mmWave Master SS version
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[out] data  - Container for Version Information
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*    Get mmWave Master SS version. This API is valid only for AWR1243 mmWave
*    device when mmWaveLink instance is running on External Host Processor.
*/
rlReturnVal_t rlDeviceGetMssVersion(rlUInt8_t deviceMap, rlFwVersionParam_t *data)
{
    rlReturnVal_t retVal;

    /* check for NULL pointer */
    if (data == RL_NULL_PTR)
    {
        /* set error code if data pointer is passed NULL */
        retVal = RL_RET_CODE_NULL_PTR;
    }
    /* This API is valid only when mmWaveLink instance is running on
       External Host Processor */
    else if (RL_PLATFORM_HOST != rlDriverGetPlatformId())
    {
        /* set error code of Platform is not set to HOST */
        retVal = RL_RET_CODE_API_NOT_SUPPORTED;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteGetApi(deviceMap, RL_DEV_STATUS_GET_MSG,
                                       RL_SYS_VERSION_SB, (rlUInt8_t*)data, 0U);
    }

    return retVal;
}

/** @fn rlReturnVal_t rlDeviceGetRfVersion(rlUInt8_t deviceMap, rlFwVersionParam_t *data)
*
*   @brief Get mmWave RF ROM and patch version
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[out] data  - Container for Version Information
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*    Get mmWave RF ROM and patch version
*/
rlReturnVal_t rlDeviceGetRfVersion(rlUInt8_t deviceMap, rlFwVersionParam_t *data)
{
    rlReturnVal_t retVal;

    /* check for NULL pointer */
    if (data == RL_NULL_PTR)
    {
        /* set error code if data pointer is passed NULL */
        retVal = RL_RET_CODE_NULL_PTR;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteGetApi(deviceMap, RL_RF_STATUS_GET_MSG,
                                        RL_RF_RFVERSION_SB, (rlUInt8_t*)data, 0U);
    }

    return retVal;
}

/** @fn rlReturnVal_t rlDeviceGetVersion(rlUInt8_t deviceMap, rlVersion_t* data)
*
*   @brief Get mmWave Hardware, Firmware/patch and mmWaveLink version
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[out] data  - Container for Version Information
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*    Get mmWave Hardware, Firmware/patch and mmWaveLink version
*/
/* DesignId : MMWL_DesignId_007 */
/* Requirements : AUTORADAR_REQ-709 */
rlReturnVal_t rlDeviceGetVersion(rlUInt8_t deviceMap, rlVersion_t* data)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDeviceGetVersion starts...\n");

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
       (RL_NULL_PTR == data))
    {
        /* set error code */
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("Invalid inputs");
    }
    else
    {
        /* If mmWaveLink is runing on ext host */
        if (RL_PLATFORM_HOST == rlDriverGetPlatformId())
        {
            /* get mmwave MSS ROM or Application version */
            retVal = rlDeviceGetMssVersion(deviceMap, &data->master);
        }
        else
        {
            /* if mmwavelink is not running on Host then no need to get MSS version */
            retVal = RL_RET_CODE_OK;
            RL_LOGD_ARG0(" rlPlatform is not host \n");
        }

        if (retVal == RL_RET_CODE_OK)
        {
            /* get RF (RadarSS) version */
            retVal += rlDeviceGetRfVersion(deviceMap, &data->rf);
        }

        if (retVal == RL_RET_CODE_OK)
        {
            /* get mmwavelink library version */
            retVal += rlDeviceGetMmWaveLinkVersion(&data->mmWaveLink);
        }
    }

    RL_LOGV_ARG0("rlDeviceGetVersion complete...\n");

    return retVal;
}
/** @fn rlReturnVal_t rlDeviceGetMmWaveLinkVersion(rlSwVersionParam_t* data)
*
*   @brief Get mmWaveLink Version
*   @param[out] data  - Container for Version Information
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*    Get mmWaveLink Version
*/
/* DesignId : MMWL_DesignId_008 */
/* Requirements : AUTORADAR_REQ-709 */
rlReturnVal_t rlDeviceGetMmWaveLinkVersion(rlSwVersionParam_t* data)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDeviceGetMmWaveLinkVersion starts...\n");

    if (RL_NULL_PTR != data)
    {
        /* mmWaveLink Version */
        /* mmWaveLink SW Major verison */
        data->major = RL_MMWAVELINK_VERSION_MAJOR;
        /* mmWaveLink SW Minor verison */
        data->minor = RL_MMWAVELINK_VERSION_MINOR;
        /* mmWaveLink SW Build verison */
        data->build = RL_MMWAVELINK_VERSION_BUILD;
        /* mmWaveLink SW Debug verison */
        data->debug = RL_MMWAVELINK_VERSION_DEBUG;
        /* mmWaveLink SW Release Year */
        data->year  = RL_MMWAVELINK_VERSION_YEAR;
        /* mmWaveLink SW Release Month */
        data->month = RL_MMWAVELINK_VERSION_MONTH;
        /* mmWaveLink SW Release date */
        data->day   = RL_MMWAVELINK_VERSION_DAY;
        /* Set error code */
        retVal      = RL_RET_CODE_OK;
        RL_LOGD_ARG0("Extracted MmWavelink version\n");
    }
    else
    {
        /* set error code */
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("rlDeviceGetMmWaveLinkVersion, Output param is NULL \n");
    }

    RL_LOGV_ARG0("rlDeviceGetMmWaveLinkVersion complete...\n");

    return retVal;
}

/** @fn rlReturnVal_t rlDeviceSetDataFmtConfig(rlUInt8_t deviceMap, rlDevDataFmtCfg_t* data)
*
*   @brief Sets LVDS/CSI2 Data output format
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data  - Container for LVDS/CSI2 Data output format
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*    Sets LVDS/CSI2 Data output format. This API is valid only for AWR1243
*    mmWave device when mmWaveLink instance is running on External Host Processor
*/
/* DesignId : MMWL_DesignId_104 */
/* Requirements : AUTORADAR_REQ-762 */
rlReturnVal_t rlDeviceSetDataFmtConfig(rlUInt8_t deviceMap, rlDevDataFmtCfg_t* data)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDeviceSetDataFmtConfig starts...\n");

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
        (RL_NULL_PTR == data))
    {
        /* set error code if DeviceMAP is invalid or data pointer is null */
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("Invalid device mapping\n");
    }
    /* This API is valid only when mmWaveLink instance is running on
       External Host Processor */
    else if (RL_PLATFORM_HOST != rlDriverGetPlatformId())
    {
        /* set error code of Platform is not set to HOST */
        retVal = RL_RET_CODE_API_NOT_SUPPORTED;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_DEV_CONFIG_SET_MSG,
                               RL_DEV_RX_DATA_FORMAT_CONF_SET_SB, (rlUInt8_t*)data,
                               (rlUInt16_t)sizeof(rlDevDataFmtCfg_t));
    }

    RL_LOGV_ARG0("rlDeviceSetDataFmtConfig complete...\n");

    return retVal;
}

/** @fn rlReturnVal_t rlDeviceGetDataFmtConfig(rlUInt8_t deviceMap,
*                                        rlDevDataFmtCfg_t* data)
*
*   @brief Gets LVDS/CSI2 Data output format
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data  - Container for LVDS/CSI2 Data output format
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Gets LVDS/CSI2 Data output format. This API is valid only for AWR1243
*   mmWave device when mmWaveLink instance is running on External Host Processor
*/
/* DesignId : MMWL_DesignId_112 */
/* Requirements : AUTORADAR_REQ-762 */
rlReturnVal_t rlDeviceGetDataFmtConfig(rlUInt8_t deviceMap, rlDevDataFmtCfg_t* data)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDeviceGetDataFmtConfig starts...\n");

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
        (RL_NULL_PTR == data))
    {
        /* set error code if DeviceMAP is invalid or data pointer is null */
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("rlDeviceGetDataFmtConfig, Invalid device map\n");
    }
    /* This API is valid only when mmWaveLink instance is running on
       External Host Processor */
    else if (RL_PLATFORM_HOST != rlDriverGetPlatformId())
    {
        /* set error code of Platform is not set to HOST */
        retVal = RL_RET_CODE_API_NOT_SUPPORTED;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteGetApi(deviceMap, RL_DEV_CONFIG_GET_MSG,
                                        RL_DEV_RX_DATA_FORMAT_CONF_SET_SB,
                                        (rlUInt8_t*)data, 0U);
    }

    RL_LOGV_ARG0("rlDeviceGetDataFmtConfig ends...\n");
    return retVal;
}

/** @fn rlReturnVal_t rlDeviceSetDataPathConfig(rlUInt8_t deviceMap, rlDevDataPathCfg_t* data)
*
*   @brief Sets LVDS/CSI2 Path Configuration
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data  - Container for Path Configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Sets LVDS/CSI2 Data path configuration. This API is valid only for AWR1243
*   mmWave device when mmWaveLink instance is running on External Host Processor
*/
/* DesignId : MMWL_DesignId_105 */
/* Requirements : AUTORADAR_REQ-763 */
rlReturnVal_t rlDeviceSetDataPathConfig(rlUInt8_t deviceMap, rlDevDataPathCfg_t* data)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDeviceSetDataPathConfig starts...\n");

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
       (RL_NULL_PTR == data))
    {
        /* set error code if DeviceMAP is invalid or data pointer is null */
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("Invalid input");
    }
    /* This API is valid only when mmWaveLink instance is running on
       External Host Processor */
    else if (RL_PLATFORM_HOST != rlDriverGetPlatformId())
    {
        /* set error code of Platform is not set to HOST */
        retVal = RL_RET_CODE_API_NOT_SUPPORTED;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_DEV_CONFIG_SET_MSG,
                               RL_DEV_RX_DATA_PATH_CONF_SET_SB, (rlUInt8_t*)data,
                               (rlUInt16_t)sizeof(rlDevDataPathCfg_t));
    }

    RL_LOGV_ARG0("rlDeviceSetDataPathConfig ends...\n");
    return retVal;
}

/** @fn rlReturnVal_t rlDeviceGetDataPathConfig(rlUInt8_t deviceMap,
*                                        rlDevDataPathCfg_t* data)
*
*   @brief Gets data path Configuration
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data  - Container for Path Configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Gets data path Configuration. This API is valid only for AWR1243 mmWave
*   device when mmWaveLink instance is running on External Host Processor
*/
/* DesignId : MMWL_DesignId_113 */
/* Requirements : AUTORADAR_REQ-763 */
rlReturnVal_t rlDeviceGetDataPathConfig(rlUInt8_t deviceMap,
                                                       rlDevDataPathCfg_t* data)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDeviceGetDataPathConfig starts...\n");

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
        (RL_NULL_PTR == data))
    {
        /* set error code if DeviceMAP is invalid or data pointer is null */
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("rlDeviceGetDataPathConfig, Invalid device map\n");
    }
    /* This API is valid only when mmWaveLink instance is running on
       External Host Processor */
    else if (RL_PLATFORM_HOST != rlDriverGetPlatformId())
    {
        /* set error code of Platform is not set to HOST */
        retVal = RL_RET_CODE_API_NOT_SUPPORTED;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteGetApi(deviceMap, RL_DEV_CONFIG_GET_MSG,
                                        RL_DEV_RX_DATA_PATH_CONF_SET_SB,
                                        (rlUInt8_t*)data, 0U);
    }

    RL_LOGV_ARG0("rlDeviceGetDataPathConfig ends...\n");
    return retVal;
}

/** @fn rlReturnVal_t rlDeviceSetLaneConfig(rlUInt8_t deviceMap, rlDevLaneEnable_t* data)
*
*   @brief Sets Lane enable Configuration
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data  - Container for lane enable Configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Sets Lane enable configuration. This API is valid only for AWR1243 mmWave
*   device when mmWaveLink instance is running on External Host Processor.
*/
/* DesignId : MMWL_DesignId_033 */
/* Requirements : AUTORADAR_REQ-756, AUTORADAR_REQ-764 */
rlReturnVal_t rlDeviceSetLaneConfig(rlUInt8_t deviceMap, rlDevLaneEnable_t* data)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDeviceSetLaneConfig starts...\n");

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
       (RL_NULL_PTR == data))
    {
        /* set error code if DeviceMAP is invalid or data pointer is null */
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("Invalid input\n");
    }
    /* This API is valid only when mmWaveLink instance is running on
       External Host Processor */
    else if (RL_PLATFORM_HOST != rlDriverGetPlatformId())
    {
        /* set error code of Platform is not set to HOST */
        retVal = RL_RET_CODE_API_NOT_SUPPORTED;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_DEV_CONFIG_SET_MSG,
                               RL_DEV_DATA_PATH_LANEEN_SET_SB, (rlUInt8_t*)data,
                               (rlUInt16_t)sizeof(rlDevLaneEnable_t));
    }
    RL_LOGV_ARG0("rlDeviceSetLaneConfig ends...\n");

    return retVal;
}

/** @fn rlReturnVal_t rlDeviceGetLaneConfig(rlUInt8_t deviceMap,
*                                        rlDevLaneEnable_t* data)
*
*   @brief Gets Lane enable Configuration
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[out] data - Container for lane enable Configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Gets Lane enable Configuration. This API is valid only for AWR1243 mmWave
*   device when mmWaveLink instance is running on External Host Processor.
*/
/* DesignId : MMWL_DesignId_115 */
/* Requirements : AUTORADAR_REQ-765, AUTORADAR_REQ-764 */
rlReturnVal_t rlDeviceGetLaneConfig(rlUInt8_t deviceMap, rlDevLaneEnable_t* data)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDeviceGetLaneConfig starts...\n");

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
        (RL_NULL_PTR == data))
    {
        /* set error code if DeviceMAP is invalid or data pointer is null */
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("rlDeviceGetLaneConfig, Invalid device map\n");
    }
    /* This API is valid only when mmWaveLink instance is running on
       External Host Processor */
    else if (RL_PLATFORM_HOST != rlDriverGetPlatformId())
    {
        /* set error code of Platform is not set to HOST */
        retVal = RL_RET_CODE_API_NOT_SUPPORTED;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteGetApi(deviceMap, RL_DEV_CONFIG_GET_MSG,
                                        RL_DEV_DATA_PATH_LANEEN_SET_SB,
                                        (rlUInt8_t*)data, 0U);
    }

    RL_LOGV_ARG0("rlDeviceGetLaneConfig ends...\n");
    return retVal;
}

/** @fn rlReturnVal_t rlDeviceSetDataPathClkConfig(rlUInt8_t deviceMap,
*                                                  rlDevDataPathClkCfg_t* data)
*
*   @brief Sets LVDS Clock Configuration
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data  - Container for LVDS Clock Configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   @brief Sets LVDS Clock Configuration. This API is valid only for AWR1243
*   mmWave device when mmWaveLink instance is running on External Host Processor.
*/
/* DesignId : MMWL_DesignId_106 */
/* Requirements : AUTORADAR_REQ-764 */
rlReturnVal_t rlDeviceSetDataPathClkConfig(rlUInt8_t deviceMap,
                                                  rlDevDataPathClkCfg_t* data )
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDeviceSetDataPathClkConfig starts...\n");

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || (RL_NULL_PTR == data))
    {
        /* set error code if DeviceMAP is invalid or data pointer is null */
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("Invalid input\n");
    }
    /* This API is valid only when mmWaveLink instance is running on
       External Host Processor */
    else if (RL_PLATFORM_HOST != rlDriverGetPlatformId())
    {
        /* set error code of Platform is not set to HOST */
        retVal = RL_RET_CODE_API_NOT_SUPPORTED;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_DEV_CONFIG_SET_MSG,
                               RL_DEV_DATA_PATH_CLOCK_SET_SB, (rlUInt8_t*)data,
                               (rlUInt16_t)sizeof(rlDevDataPathClkCfg_t));
    }
    RL_LOGV_ARG0("rlDeviceSetDataPathClkConfig ends...\n");

    return retVal;
}

/** @fn rlReturnVal_t rlDeviceGetDataPathClkConfig(rlUInt8_t deviceMap,
*                                        rlProfileCfg_t* data)
*
*   @brief Gets LVDS Clock Configuration
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[out] data - Container for LVDS Clock Configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Gets LVDS Clock Configuration. This API is valid only for AWR1243 mmWave
*   device when mmWaveLink instance is running on External Host Processor
*/
/* DesignId : MMWL_DesignId_114 */
/* Requirements : AUTORADAR_REQ-764 */
rlReturnVal_t rlDeviceGetDataPathClkConfig(rlUInt8_t deviceMap, rlDevDataPathClkCfg_t* data)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDeviceGetDataPathClkConfig starts...\n");

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
        (RL_NULL_PTR == data))
    {
        /* set error code if DeviceMAP is invalid or data pointer is null */
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("rlDeviceGetDataPathClkConfig, Invalid device map\n");
    }
    /* This API is valid only when mmWaveLink instance is running on
       External Host Processor */
    else if (RL_PLATFORM_HOST != rlDriverGetPlatformId())
    {
        /* set error code of Platform is not set to HOST */
        retVal = RL_RET_CODE_API_NOT_SUPPORTED;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteGetApi(deviceMap, RL_DEV_CONFIG_GET_MSG,
                                        RL_DEV_DATA_PATH_CLOCK_SET_SB,
                                        (rlUInt8_t*)data, 0U);
    }

    RL_LOGV_ARG0("rlDeviceGetDataPathClkConfig ends...\n");
    return retVal;
}

/** @fn rlReturnVal_t rlDeviceSetLvdsLaneConfig(rlUInt8_t deviceMap, rlDevLvdsDataCfg_t* data)
*
*   @brief Sets LVDS Lane data format Configuration
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data  - Container for LVDS Lane Configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Sets LVDS Lane data format Configuration. This API is valid only for AWR1243
*   mmWave device when mmWaveLink instance is running on External Host Processor
*/
/* DesignId : MMWL_DesignId_030 */
/* Requirements : AUTORADAR_REQ-767 */
rlReturnVal_t rlDeviceSetLvdsLaneConfig(rlUInt8_t deviceMap, rlDevLvdsLaneCfg_t* data)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDeviceSetLvdsLaneConfig starts...\n");

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
       (RL_NULL_PTR == data))
    {
        /* set error code if DeviceMAP is invalid or data pointer is null */
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("Invalid input\n");
    }
    /* This API is valid only when mmWaveLink instance is running on
       External Host Processor */
    else if (RL_PLATFORM_HOST != rlDriverGetPlatformId())
    {
        /* set error code of Platform is not set to HOST */
        retVal = RL_RET_CODE_API_NOT_SUPPORTED;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_DEV_CONFIG_SET_MSG,
                               RL_DEV_DATA_PATH_CFG_SET_SB, (rlUInt8_t*)data,
                               (rlUInt16_t)sizeof(rlDevLvdsLaneCfg_t));
    }

    RL_LOGV_ARG0("rlDeviceSetLvdsLaneConfig ends...\n");

    return retVal;
}

/** @fn rlReturnVal_t rlDeviceGetLvdsLaneConfig(rlUInt8_t deviceMap,
*                                        rlProfileCfg_t* data)
*
*   @brief Gets LVDS Lane Configuration
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[out] data - Container for LVDS Lane Configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Gets LVDS Lane Configuration. This API is valid only for AWR1243 mmWave
*   device when mmWaveLink instance is running on External Host Processor
*/
/* DesignId : MMWL_DesignId_116 */
/* Requirements : AUTORADAR_REQ-767 */
rlReturnVal_t rlDeviceGetLvdsLaneConfig(rlUInt8_t deviceMap, rlDevLvdsLaneCfg_t* data)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDeviceGetLvdsLaneConfig starts...\n");

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
        (RL_NULL_PTR == data))
    {
        /* set error code if DeviceMAP is invalid or data pointer is null */
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("rlDeviceGetLvdsLaneConfig, Invalid device map\n");
    }
    /* This API is valid only when mmWaveLink instance is running on
       External Host Processor */
    else if (RL_PLATFORM_HOST != rlDriverGetPlatformId())
    {
        /* set error code of Platform is not set to HOST */
        retVal = RL_RET_CODE_API_NOT_SUPPORTED;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteGetApi(deviceMap, RL_DEV_CONFIG_GET_MSG,
                                        RL_DEV_DATA_PATH_CFG_SET_SB,
                                        (rlUInt8_t*)data, 0U);
    }

    RL_LOGV_ARG0("rlDeviceGetLvdsLaneConfig ends...\n");
    return retVal;
}

/** @fn rlReturnVal_t rlDeviceSetContStreamingModeConfig(rlUInt8_t deviceMap,
                                                         rlDevContStreamingModeCfg_t* data)
*
*   @brief Sets Continous Streaming Mode Configuration
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data  - Container for Continous Streaming Mode Configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   @brief This function configures the transfer of captured ADC samples continuously without
*          missing any sample to an external host. This API is valid only for AWR1243 mmWave
*          device when mmWaveLink instance is running on External Host Processor.
*   @note : Continuous streaming mode is useful for RF lab characterization and debug. In this 
*           mode, the device is configured to transmit a single continuous wave tone at a specific
*           RF frequency continuously
*/
/* DesignId : MMWL_DesignId_040 */
/* Requirements : AUTORADAR_REQ-829 */
rlReturnVal_t rlDeviceSetContStreamingModeConfig(rlUInt8_t deviceMap,
                                                 rlDevContStreamingModeCfg_t* data)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDeviceSetContStreamingModeConfig ends...\n");

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
        (RL_NULL_PTR == data))
    {
        /* set error code if DeviceMAP is invalid or data pointer is null */
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("Invalid input");
    }
    /* This API is valid only when mmWaveLink instance is running on
       External Host Processor */
    else if (RL_PLATFORM_HOST != rlDriverGetPlatformId())
    {
        /* set error code of Platform is not set to HOST */
        retVal = RL_RET_CODE_API_NOT_SUPPORTED;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_DEV_CONFIG_SET_MSG,
                               RL_DEV_RX_CONTSTREAMING_MODE_CONF_SET_SB,
                               (rlUInt8_t*)data,
                               (rlUInt16_t)sizeof(rlDevContStreamingModeCfg_t));
    }

    RL_LOGV_ARG0("rlDeviceSetContStreamingModeConfig ends...\n");

    return retVal;

}

/** @fn rlReturnVal_t rlDeviceGetContStreamingModeConfig(rlUInt8_t deviceMap,
*                                        rlDevContStreamingModeCfg_t* data)
*
*   @brief Gets continuous Streaming Mode Configuration
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[out] data - Container for continuous Streaming Mode Configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Gets continuous Streaming Mode Configuration. This API is valid only for
*   AWR1243 mmWave device when mmWaveLink instance is running on External
*   Host Processor
*/
/* DesignId : MMWL_DesignId_121 */
/* Requirements : AUTORADAR_REQ-829 */
rlReturnVal_t rlDeviceGetContStreamingModeConfig(
                        rlUInt8_t deviceMap, rlDevContStreamingModeCfg_t* data)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDeviceGetContStreamingModeConfig starts...\n");

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
        (RL_NULL_PTR == data))
    {
        /* set error code if DeviceMAP is invalid or data pointer is null */
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("rlDeviceGetContStreamingModeConfig, Invalid device map\n");
    }
    /* This API is valid only when mmWaveLink instance is running on
       External Host Processor */
    else if (RL_PLATFORM_HOST != rlDriverGetPlatformId())
    {
        /* set error code of Platform is not set to HOST */
        retVal = RL_RET_CODE_API_NOT_SUPPORTED;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteGetApi(deviceMap, RL_DEV_CONFIG_GET_MSG,
                                        RL_DEV_RX_CONTSTREAMING_MODE_CONF_SET_SB,
                                        (rlUInt8_t*)data, 0U);
    }

    RL_LOGV_ARG0("rlDeviceGetContStreamingModeConfig ends...\n");
    return retVal;
}

/** @fn rlReturnVal_t rlDeviceSetCsi2Config(rlUInt8_t deviceMap, rlDevCsi2Cfg_t* data)
*
*   @brief Sets CSI2 data format Configuration
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data  - Container for CSI2 Configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Sets CSI2 data format Configuration. This API is valid only for AWR1243
*   mmWave device when mmWaveLink instance is running on External Host Processor
*/
/* DesignId : MMWL_DesignId_048 */
/* Requirements : AUTORADAR_REQ-756, AUTORADAR_REQ-766 */
rlReturnVal_t rlDeviceSetCsi2Config(rlUInt8_t deviceMap, rlDevCsi2Cfg_t* data)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDeviceSetCsi2Config starts...\n");

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
        (RL_NULL_PTR == data))
    {
        /* set error code if DeviceMAP is invalid or data pointer is null */
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("Invalid input");
    }
    /* This API is valid only when mmWaveLink instance is running on
       External Host Processor */
    else if (RL_PLATFORM_HOST != rlDriverGetPlatformId())
    {
        /* set error code of Platform is not set to HOST */
        retVal = RL_RET_CODE_API_NOT_SUPPORTED;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_DEV_CONFIG_SET_MSG,
                               RL_DEV_CSI2_CFG_SET_SB, (rlUInt8_t*)data,
                               (rlUInt16_t)sizeof(rlDevCsi2Cfg_t));
    }
    RL_LOGV_ARG0("rlDeviceSetCsi2Config ends...\n");

    return retVal;
}

/** @fn rlReturnVal_t rlDeviceGetCsi2Config(rlUInt8_t deviceMap,
*                                            rlDevCsi2Cfg_t* data)
*
*   @brief Gets Csi2 data format Configuration
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[out] data - Container for CSI2 Configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*  Gets CSI2 data format Configuration. This API is valid only for AWR1243
*  mmWave device when mmWaveLink instance is running on External Host Processor.
*/
/* DesignId : MMWL_DesignId_111 */
/* Requirements : AUTORADAR_REQ-756, AUTORADAR_REQ-766 */
rlReturnVal_t rlDeviceGetCsi2Config(rlUInt8_t deviceMap, rlDevCsi2Cfg_t* data)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDeviceGetCsi2Config starts...\n");

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
        (RL_NULL_PTR == data))
    {
        /* set error code if DeviceMAP is invalid or data pointer is null */
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("rlDeviceGetCsi2Config, Invalid device map\n");
    }
    /* This API is valid only when mmWaveLink instance is running on
       External Host Processor */
    else if (RL_PLATFORM_HOST != rlDriverGetPlatformId())
    {
        /* set error code of Platform is not set to HOST */
        retVal = RL_RET_CODE_API_NOT_SUPPORTED;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteGetApi(deviceMap, RL_DEV_CONFIG_GET_MSG,
                                        RL_DEV_CSI2_CFG_SET_SB,
                                        (rlUInt8_t*)data, 0U);
    }

    RL_LOGV_ARG0("rlDeviceGetCsi2Config ends...\n");
    return retVal;
}

/** @fn rlReturnVal_t rlDeviceSetHsiConfig(rlUInt8_t deviceMap,
*                                          rlDevHsiCfg_t* data)
*
*    @brief: This function sets the High Speed Interface(LVDS/CSI2) clock, lane,
*                 data rate and data format
*    @param[in] deviceMap - Connected device Index
*    @param[in] data         - HSI Config data
*
*    @return rlReturnVal_t Success - 0, Failure - Error Code
*
*    @brief: This function sets the High Speed Interface(LVDS/CSI2) clock, lane,
*    data rate and data format. This API is valid only for AWR1243
*   mmWave device when mmWaveLink instance is running on External Host Processor
*/
/* DesignId : MMWL_DesignId_020 */
/* Requirements : AUTORADAR_REQ-756 */
rlReturnVal_t rlDeviceSetHsiConfig(rlUInt8_t deviceMap,
                                              rlDevHsiCfg_t* data )
{
    rlReturnVal_t retVal;
    /* Initialize Command and Response Sub Blocks */
    rlDriverMsg_t inMsg;
    rlDriverMsg_t outMsg = {0};
    rlUInt16_t sbcCnt = 0U;

    /* Initialize Command and Response Sub Blocks */
    rlPayloadSb_t inPayloadSb[3U] = {0};

    RL_LOGV_ARG0("rlDeviceSetHsiConfig starts...\n");

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
       (RL_NULL_PTR == data) )
    {
        /* set error code if DeviceMAP is invalid or data pointer is null */
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("rlDeviceSetHsiConfig, Invalid input \n");
    }
    else
    {
        /* Data Format config SubBlock */
        if (RL_NULL_PTR != data->datafmt)
        {
            /* Fill in-message Payload */
            rlDriverFillPayload(RL_DEV_CONFIG_SET_MSG,
                                RL_DEV_RX_DATA_FORMAT_CONF_SET_SB,
                                &inPayloadSb[sbcCnt],
                                (rlUInt8_t* )data->datafmt,
                                (rlUInt16_t)sizeof(rlDevDataFmtCfg_t));
            /* increament Sub-block count */
            sbcCnt++;
        }

        /* Data Path Config SubBlock */
        if (RL_NULL_PTR != data->dataPath)
        {
            /* Fill in-message Payload */
            rlDriverFillPayload(RL_DEV_CONFIG_SET_MSG,
                        RL_DEV_RX_DATA_PATH_CONF_SET_SB,
                        &inPayloadSb[sbcCnt],
                        (rlUInt8_t* )data->dataPath,
                        (rlUInt16_t)sizeof(rlDevDataPathCfg_t));
            /* increament Sub-block count */
            sbcCnt++;
        }

        /* Data Path clock Config SubBlock */
        if (RL_NULL_PTR != data->dataPathClk)
        {
            /* Fill in-message Payload */
            rlDriverFillPayload(RL_DEV_CONFIG_SET_MSG,
                        RL_DEV_DATA_PATH_CLOCK_SET_SB,
                        &inPayloadSb[sbcCnt],
                        (rlUInt8_t* )data->dataPathClk,
                        (rlUInt16_t)sizeof(rlDevDataPathClkCfg_t));
            /* increament Sub-block count */
            sbcCnt++;
        }

        /* Construct command packet */
        rlDriverConstructInMsg(RL_DEV_CONFIG_SET_MSG, &inMsg, &inPayloadSb[0U]);

        if (sbcCnt > 0U)
        {
            /* setting num of sub-block to inMsg */
            inMsg.opcode.nsbc = sbcCnt;

            /* Send Command to mmWave Radar Device */
            retVal = rlDriverCmdInvoke(deviceMap, inMsg, &outMsg);
        }
        else
        {
            /* set error code if application doesn't pass any subBlock data */
            retVal = RL_RET_CODE_INVALID_INPUT;
            RL_LOGE_ARG0("sub block is NULL\n");
        }
    }

    RL_LOGV_ARG0("rlDeviceSetHsiConfig ends...\n");

    return retVal;
}

/** @fn rlReturnVal_t rlDeviceSetHsiClk(rlUInt8_t deviceMap, rlDevHsiClk_t* data)
*
*   @brief Sets High Speed Interface Clock
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data  - Container for HSI Clock
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*    @brief Sets High Speed Interface Clock

*/
/* DesignId : MMWL_DesignId_101 */
/* Requirements : AUTORADAR_REQ-765 */
rlReturnVal_t rlDeviceSetHsiClk(rlUInt8_t deviceMap, rlDevHsiClk_t* data)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDeviceSetHsiClk starts...\n");

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
       (RL_NULL_PTR == data))
    {
        /* set error code if DeviceMAP is invalid or data pointer is null */
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("Invalid input\n");
    }
    else
    {
            /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_RF_STATIC_CONF_SET_MSG,
                               RL_RF_HIGHSPEEDINTFCLK_CONF_SET_SB,
                               (rlUInt8_t*)data,
                               (rlUInt16_t)sizeof(rlDevHsiClk_t));
    }
    RL_LOGV_ARG0("rlDeviceSetHsiClk ends...\n");

    return retVal;
}

/** @fn rlReturnVal_t rlDeviceMcuClkConfig(rlUInt8_t deviceMap, rlMcuClkCfg_t * data)
*
*   @brief Sets the configurations to setup the desired frequency of the MCU Clock
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data  - Container for MCU clock Configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Sets the configurations to setup the desired frequency of the MCU Clock. This
*   API is valid only for AWR1243 mmWave device when mmWaveLink instance is running
*   on External Host Processor.
*/
/* DesignId : MMWL_DesignId_069 */
/* Requirements : AUTORADAR_REQ-713 */
rlReturnVal_t rlDeviceMcuClkConfig(rlUInt8_t deviceMap, rlMcuClkCfg_t * data)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDeviceMcuClkConfig starts...\n");

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
        (RL_NULL_PTR == data))
    {
        /* set error code if DeviceMAP is invalid or data pointer is null */
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("Invalid input");
    }
    /* This API is valid only when mmWaveLink instance is running on
    External Host Processor */
    else if (RL_PLATFORM_HOST != rlDriverGetPlatformId())
    {
        /* set error code of Platform is not set to HOST */
        retVal = RL_RET_CODE_API_NOT_SUPPORTED;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_DEV_CONFIG_SET_MSG,
            RL_DEV_MCUCLOCK_CONF_SET_SB, (rlUInt8_t*)data,
            (rlUInt16_t)sizeof(rlMcuClkCfg_t));
    }
    RL_LOGV_ARG0("rlDeviceMcuClkConfig ends...\n");

    return retVal;
}

/** @fn rlReturnVal_t rlDevicePmicClkConfig(rlUInt8_t deviceMap, rlPmicClkCfg_t * data)
*
*   @brief Sets the configurations for PMIC clock
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data  - Container for PMIC clock Configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Sets the configurations for PMIC clock. This API is valid only for AWR1243
*   mmWave device when mmWaveLink instance is running on External Host Processor.
*/
/* DesignId : MMWL_DesignId_070 */
/* Requirements : AUTORADAR_REQ-907 */
rlReturnVal_t rlDevicePmicClkConfig(rlUInt8_t deviceMap, rlPmicClkCfg_t * data)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDevicePmicClkConfig starts...\n");

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
        (RL_NULL_PTR == data))
    {
        /* set error code if DeviceMAP is invalid or data pointer is null */
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("Invalid input");
    }
    /* This API is valid only when mmWaveLink instance is running on
    External Host Processor */
    else if (RL_PLATFORM_HOST != rlDriverGetPlatformId())
    {
        /* set error code of Platform is not set to HOST */
        retVal = RL_RET_CODE_API_NOT_SUPPORTED;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_DEV_CONFIG_SET_MSG,
            RL_DEV_PMICCLOCK_CONF_SET_SB, (rlUInt8_t*)data,
            (rlUInt16_t)sizeof(rlPmicClkCfg_t));
    }
    RL_LOGV_ARG0("rlDevicePmicClkConfig ends...\n");

    return retVal;
}

/** @fn rlReturnVal_t rlDeviceLatentFaultTests(rlUInt8_t deviceMap, rllatentFault_t * data)
*
*   @brief Sets the configurations for latent fault test
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data  - Container for latent fault test Configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Sets the configurations for latent fault test. This API is valid only for AWR1243
*   mmWave device when mmWaveLink instance is running on External Host Processor.
*/
/* DesignId : MMWL_DesignId_071 */
/* Requirements : AUTORADAR_REQ-908 */
rlReturnVal_t rlDeviceLatentFaultTests(rlUInt8_t deviceMap, rllatentFault_t * data)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDeviceLatentFaultTests starts...\n");

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
        (RL_NULL_PTR == data))
    {
        /* set error code if DeviceMAP is invalid or data pointer is null */
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("Invalid input");
    }
    /* This API is valid only when mmWaveLink instance is running on
    External Host Processor */
    else if (RL_PLATFORM_HOST != rlDriverGetPlatformId())
    {
        /* set error code of Platform is not set to HOST */
        retVal = RL_RET_CODE_API_NOT_SUPPORTED;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_DEV_CONFIG_SET_MSG,
            RL_DEV_LATENTFAULT_TEST_CONF_SB, (rlUInt8_t*)data,
            (rlUInt16_t)sizeof(rllatentFault_t));
    }
    RL_LOGV_ARG0("rlDeviceLatentFaultTests ends...\n");

    return retVal;
}

/** @fn rlReturnVal_t rlDeviceEnablePeriodicTests(rlUInt8_t deviceMap, rlperiodicTest_t * data)
*
*   @brief Sets the configurations for periodic test
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data  - Container for periodic test Configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Sets the configurations for periodic test. This API is valid only for AWR1243
*   mmWave device when mmWaveLink instance is running on External Host Processor.
*/
/* DesignId : MMWL_DesignId_072 */
/* Requirements : AUTORADAR_REQ-909 */
rlReturnVal_t rlDeviceEnablePeriodicTests(rlUInt8_t deviceMap, rlperiodicTest_t * data)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDeviceEnablePeriodicTests starts...\n");

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
        (RL_NULL_PTR == data))
    {
        /* set error code if DeviceMAP is invalid or data pointer is null */
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("Invalid input");
    }
    /* This API is valid only when mmWaveLink instance is running on
    External Host Processor */
    else if (RL_PLATFORM_HOST != rlDriverGetPlatformId())
    {
        /* set error code of Platform is not set to HOST */
        retVal = RL_RET_CODE_API_NOT_SUPPORTED;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_DEV_CONFIG_SET_MSG,
            RL_DEV_PERIODICTESTS_CONF_SB, (rlUInt8_t*)data,
            (rlUInt16_t)sizeof(rlperiodicTest_t));
    }
    RL_LOGV_ARG0("rlDeviceEnablePeriodicTests ends...\n");

    return retVal;
}

/** @fn rlReturnVal_t rlDeviceSetTestPatternConfig(rlUInt8_t deviceMap, rltestPattern_t * data)
*
*   @brief Setup for test pattern to be generated
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] data  - Container for periodic test Configuration
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Set the configurations to setup the test pattern to be generated and transferred
*   over the selected high speed interface (LVDS/CSI2). This API is valid only for
*   AWR1243 mmWave device when mmWaveLink instance is running on External Host Processor.
*/
/* DesignId : MMWL_DesignId_103 */
/* Requirements : AUTORADAR_REQ-910 */
rlReturnVal_t rlDeviceSetTestPatternConfig(rlUInt8_t deviceMap, rltestPattern_t * data)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDeviceSetTestPatternConfig starts...\n");

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
        (RL_NULL_PTR == data))
    {
        /* set error code if DeviceMAP is invalid or data pointer is null */
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("Invalid input");
    }
    /* This API is valid only when mmWaveLink instance is running on
    External Host Processor */
    else if (RL_PLATFORM_HOST != rlDriverGetPlatformId())
    {
        /* set error code of Platform is not set to HOST */
        retVal = RL_RET_CODE_API_NOT_SUPPORTED;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_DEV_CONFIG_SET_MSG,
                                        RL_DEV_TESTPATTERN_GEN_SET_SB, (rlUInt8_t*)data,
                                        (rlUInt16_t)sizeof(rltestPattern_t));
    }
    RL_LOGV_ARG0("rlDeviceSetTestPatternConfig ends...\n");

    return retVal;
}

/** @fn rlReturnVal_t rlDeviceSetMiscConfig(rlUInt8_t deviceMap, rlDevMiscCfg_t *data)
*
*   @brief Setup misc. device configurations
*   @param[in] data  - Container for device Configuration
*   @param[in] deviceMap - Bitmap of devices to send the message
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Set misc. device configurations of MasterMSS, where currently it set CRC type for async event
*   message sent by MSS to Host. This API is valid only for AWR1243 mmWave device when mmWaveLink
*   instance is running on External Host Processor.
*/
/* DesignId : MMWL_DesignId_117 */
/* Requirements : AUTORADAR_REQ-886 */
rlReturnVal_t rlDeviceSetMiscConfig(rlUInt8_t deviceMap, rlDevMiscCfg_t *data)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDeviceSetMiscConfig starts...\n");

    /* check if deviceIndex is out of defined value */
    if ((rlDriverIsDeviceMapValid(deviceMap) != RL_RET_CODE_OK) || \
        (RL_NULL_PTR == data))
    {
        /* set error code if DeviceMAP is invalid or data pointer is null */
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("Invalid input");
    }
    /* This API is valid only when mmWaveLink instance is running on
    External Host Processor */
    else if (RL_PLATFORM_HOST != rlDriverGetPlatformId())
    {
        /* set error code of Platform is not set to HOST */
        retVal = RL_RET_CODE_API_NOT_SUPPORTED;
    }
    else
    {
        /* Package the command with given data and send it to device */
        retVal = rlDriverExecuteSetApi(deviceMap, RL_DEV_CONFIG_SET_MSG,
                                       RL_DEV_MISC_CFG_SET_SB, (rlUInt8_t*)data,
                                       (rlUInt16_t)sizeof(rlDevMiscCfg_t));
    }
    RL_LOGV_ARG0("rlDeviceSetMiscConfig ends...\n");

    return retVal;
}

/** @fn rlReturnVal_t rlDeviceConfigureCrc(rlCrcType_t crcType)
*
*   @brief  Configures the CRC Type in mmWaveLink Driver
*   @param[in] crcType - CRC Types, 0 - No CRC
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Configures the CRC Type in mmWaveLink Driver
*/
/* DesignId : MMWL_DesignId_028 */
/* Requirements : AUTORADAR_REQ-710 */
rlReturnVal_t rlDeviceConfigureCrc(rlCrcType_t crcType)
{
    /* set CRC Type passed by application to rlDriver */
    return rlDriverConfigureCrc(crcType);
}

/** @fn rlReturnVal_t rlDeviceConfigureAckTimeout(rlUInt32_t ackTimeout)
*
*   @brief  Configures the Acknowledgement timeout in mmWaveLink Driver
*   @param[in] ackTimeout - ACK timeout, 0 - No ACK
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Configures the Acknowledgement timeout in mmWaveLink Driver, 0 - Disable ACK
*/
/* DesignId : MMWL_DesignId_028 */
/* Requirements : AUTORADAR_REQ_710 */
rlReturnVal_t rlDeviceConfigureAckTimeout(rlUInt32_t ackTimeout)
{
    /* Set Ack Timeout value passed by applicatio to rlDriver */
    return rlDriverConfigureAckTimeout(ackTimeout);
}

/*
 * END OF rl_device.c FILE
 */
