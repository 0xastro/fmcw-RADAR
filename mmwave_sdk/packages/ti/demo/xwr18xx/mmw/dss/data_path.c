/**
 *   @file  data_path.c
 *
 *   @brief
 *      Implements Data path processing functionality.
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

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include <ti/drivers/soc/soc.h>
#include <ti/drivers/hwa/hwa.h>
#include <ti/drivers/edma/edma.h>
#include <ti/demo/xwr18xx/mmw/dss/mmw_dss.h>


/**************************************************************************
 *************************** Global Definitions ********************************
 **************************************************************************/

/**
 * @brief
 *  Global Variable for tracking information required by the mmw Demo
 */
extern MmwDemo_DSS_MCB    gMmwDssMCB;

/**************************************************************************
 *************************** Static Function Prototype***************************
 **************************************************************************/
static void MmwDemo_hwaInit(MmwDemo_DataPathObj *obj);
static void MmwDemo_edmaInit(MmwDemo_DataPathObj *obj);
static void MmwDemo_hwaOpen(MmwDemo_DataPathObj *obj, SOC_Handle socHandle);
static void MmwDemo_edmaOpen(MmwDemo_DataPathObj *obj);
static void MmwDemo_edmaClose(MmwDemo_DataPathObj *obj);
static void MmwDemo_hwaClose(MmwDemo_DataPathObj *obj);
static void MmwDemo_EDMA_errorCallbackFxn(EDMA_Handle handle, EDMA_errorInfo_t *errorInfo);
static void MmwDemo_EDMA_transferControllerErrorCallbackFxn(EDMA_Handle handle,
                EDMA_transferControllerErrorInfo_t *errorInfo);

/**
 *  @b Description
 *  @n
 *      HWA driver init
 *
 *  @param[in] obj      Pointer to data path object
 *
 *  @retval
 *      Not Applicable.
 */
static void MmwDemo_hwaInit(MmwDemo_DataPathObj *obj)
{
    /* Initialize the HWA */
    HWA_init();
}

/**
 *  @b Description
 *  @n
 *      EDMA driver init
 *
 *  @param[in] obj      Pointer to data path object
 *
 *  @retval
 *      Not Applicable.
 */
void MmwDemo_edmaInit(MmwDemo_DataPathObj *obj)
{
    uint8_t edmaNumInstances, inst;
    int32_t errorCode;

    edmaNumInstances = EDMA_getNumInstances();
    for (inst = 0; inst < edmaNumInstances; inst++)
    {
        errorCode = EDMA_init(inst);
        if (errorCode != EDMA_NO_ERROR)
        {
            //System_printf ("Debug: EDMA instance %d initialization returned error %d\n", errorCode);
            MmwDemo_debugAssert (0);
            return;
        }
    }

    memset(&obj->EDMA_errorInfo, 0, sizeof(obj->EDMA_errorInfo));
    memset(&obj->EDMA_transferControllerErrorInfo, 0, sizeof(obj->EDMA_transferControllerErrorInfo));
}

/**
 *  @b Description
 *  @n
 *      Call back function for EDMA CC (Channel controller) error as per EDMA API.
 *      Declare fatal error if happens, the output errorInfo can be examined if code
 *      gets trapped here.
 */
void MmwDemo_EDMA_errorCallbackFxn(EDMA_Handle handle, EDMA_errorInfo_t *errorInfo)
{
    gMmwDssMCB.dataPathObj.EDMA_errorInfo = *errorInfo;
    MmwDemo_debugAssert(0);
}

/**
 *  @b Description
 *  @n
 *      Call back function for EDMA transfer controller error as per EDMA API.
 *      Declare fatal error if happens, the output errorInfo can be examined if code
 *      gets trapped here.
 */
void MmwDemo_EDMA_transferControllerErrorCallbackFxn(EDMA_Handle handle,
                EDMA_transferControllerErrorInfo_t *errorInfo)
{
    gMmwDssMCB.dataPathObj.EDMA_transferControllerErrorInfo = *errorInfo;
    MmwDemo_debugAssert(0);
}

/**
 *  @b Description
 *  @n
 *      Open HWA driver instance
 *
 *  @param[in] obj      Pointer to data path object
 *  @param[in] socHandle SOC driver handle
 *
 *  @retval
 *      Not Applicable.
 */
static void MmwDemo_hwaOpen(MmwDemo_DataPathObj *obj, SOC_Handle socHandle)
{
    int32_t             errCode;

    /* Open the HWA Instance */
    obj->hwaHandle = HWA_open(0, socHandle, &errCode);
    if (obj->hwaHandle == NULL)
    {
        //System_printf("Error: Unable to open the HWA Instance err:%d\n",errCode);
        MmwDemo_debugAssert (0);
        return;
    }
}

/**
 *  @b Description
 *  @n
 *      Close HWA driver instance
 *
 *  @param[in] obj      Pointer to data path object
 *
 *  @retval
 *      Not Applicable.
 */
static void MmwDemo_hwaClose(MmwDemo_DataPathObj *obj)
{
    int32_t             errCode;

    /* Close the HWA Instance */
    errCode = HWA_close(obj->hwaHandle);
    if (errCode != 0)
    {
        MmwDemo_debugAssert (0);
        return;
    }
}

/**
 *  @b Description
 *  @n
 *      Open EDMA driver instance
 *
 *  @param[in] obj      Pointer to data path object
 *
 *  @retval
 *      Not Applicable.
 */
static void MmwDemo_edmaOpen(MmwDemo_DataPathObj *obj)
{
    int32_t             errCode;
    EDMA_instanceInfo_t edmaInstanceInfo;
    EDMA_errorConfig_t  errorConfig;
    int32_t edmaCCIdx;

    for (edmaCCIdx = 0; edmaCCIdx < EDMA_NUM_CC; edmaCCIdx++)
    {
        obj->edmaHandle[edmaCCIdx] = EDMA_open(edmaCCIdx, &errCode, &edmaInstanceInfo);

        if (obj->edmaHandle[edmaCCIdx] == NULL)
        {
            //System_printf("Error: Unable to open the EDMA Instance err:%d\n",errCode);
            MmwDemo_debugAssert (0);
            return;
        }

        errorConfig.isConfigAllEventQueues = true;
        errorConfig.isConfigAllTransferControllers = true;
        errorConfig.isEventQueueThresholdingEnabled = true;
        errorConfig.eventQueueThreshold = EDMA_EVENT_QUEUE_THRESHOLD_MAX;
        errorConfig.isEnableAllTransferControllerErrors = true;
        errorConfig.callbackFxn = MmwDemo_EDMA_errorCallbackFxn;
        errorConfig.transferControllerCallbackFxn = MmwDemo_EDMA_transferControllerErrorCallbackFxn;
        if ((errCode = EDMA_configErrorMonitoring(obj->edmaHandle[edmaCCIdx], &errorConfig)) != EDMA_NO_ERROR)
        {
            //System_printf("Error: EDMA_configErrorMonitoring() failed with errorCode = %d\n", errCode);
            MmwDemo_debugAssert (0);
            return;
        }
    }
}

/**
 *  @b Description
 *  @n
 *      Close EDMA driver instance
 *
 *  @param[in] obj      Pointer to data path object
 *
 *  @retval
 *      Not Applicable.
 */
void MmwDemo_edmaClose(MmwDemo_DataPathObj *obj)
{
    EDMA_close(obj->edmaHandle);
}

/**
 *  @b Description
 *  @n
 *  This function is called at the init time to initialze data path driver.
 *
 *  @param[in] obj      Pointer to data path object
 *
 *  @retval
 *      Not Applicable.
 */
void MmwDemo_dataPathInit(MmwDemo_DataPathObj *obj)
{
    memset(obj, 0, sizeof(MmwDemo_DataPathObj));

    /* Initialize HWA */
    MmwDemo_hwaInit(obj);

    /* Initialize EDMA */
    MmwDemo_edmaInit(obj);
}

/**
 *  @b Description
 *  @n
 *      This function is called at the init time to open data path driver instances.
 *
 *  @param[in] obj      Pointer to data path object
 *
 *  @retval
 *      Not Applicable.
 */
void MmwDemo_dataPathOpen(MmwDemo_DataPathObj *obj)
{
    /*****************************************************************************
     * Open HWA, EDMA drivers instances
     *****************************************************************************/
    MmwDemo_hwaOpen(obj, gMmwDssMCB.socHandle);
    MmwDemo_edmaOpen(obj);
}

/**
 *  @b Description
 *  @n
 *  This function is called to close data path driver instances.
 *
 *  @param[in] obj      Pointer to data path object
 *
 *  @retval
 *      Not Applicable.
 */
void MmwDemo_dataPathClose(MmwDemo_DataPathObj *obj)
{
    /* DPC close */
    DPM_deinit(obj->objDetDpmHandle);
    
    /* Close HWA driver */
    MmwDemo_hwaClose(obj);

    /* Close EDMA driver */
    MmwDemo_edmaClose(obj);
}
