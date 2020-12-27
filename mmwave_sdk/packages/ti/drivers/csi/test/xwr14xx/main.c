/*
 *   @file  main.c
 *
 *   @brief
 *      Unit Test code for the CSI Driver
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

/* BIOS/XDC Include Files. */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/heaps/HeapBuf.h>
#include <ti/sysbios/heaps/HeapMem.h>
#include <ti/sysbios/knl/Event.h>

/* mmWave SK Include Files: */
#include <ti/common/sys_common.h>
#include <ti/drivers/osal/HwiP.h>
#include <ti/drivers/esm/esm.h>
#include <ti/drivers/csi/csi.h>

/* test logger include files: */
#include <ti/utils/testlogger/logger.h>

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

/**************************************************************************
 *************************** Local Definitions ****************************
 **************************************************************************/

/* MAX Data Pattern Test Size for the Data Tests: */
#define MAX_TEST_BUFFER_SIZE        128

/**************************************************************************
 *************************** CSI Test Functions ***************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      The function is a very simple memory dump verification which matches
 *      the CSI register memory configuration for a specific configuration
 *      with the output generated from a working scenario.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t Test_csiMemoryMap (SOC_Handle socHandle)
{
    CSI_Cfg     csiCfg;
    CSI_Handle  csiHandle;
    int32_t     errCode;
    uint32_t    index;
    uint32_t*   ptrMemory;
    uint32_t    csiMemMap1[] =
    {
        0x00000030U,    /* DSI_REVISION             */
        0x00000000U,
        0x00000000U,
        0x00000000U,
        0x00000004U,    /* DSI_SYSCONFIG            */
        0x00000001U,    /* DSI_SYSSTATUS            */
        0x00000080U,    /* DSI_IRQSTATUS            */
        0x00000000U,    /* DSI_IRQENABLE            */
        0x00000000U,
        0x00000000U,
        0x00000000U,
        0x00000000U,
        0x00000000U,
        0x00000000U,
        0x00000000U,
        0x00000000U,
        0x00004101U,    /* DSI_CTRL                 */
        0x01926936U,    /* DSI_GNQ                  */
        //0xEA054213U     /* DSI_COMPLEXIO_CFG1       */
    };
    uint32_t    csiMemMap2[] =
    {
        0xA030600AU,    /* DSI_CLK_CTRL             */
        0x000000C8U,    /* DSI_TIMING1              */
        0x7FFF7FFFU,    /* DSI_TIMING2              */
        0x00000000U,    /* DSI_VM_TIMING1           */
        0x00000000U,    /* DSI_VM_TIMING2           */
        0x00000000U,    /* DSI_VM_TIMING3           */
        0x00004020U     /* DSI_CLK_TIMING           */
    };
    uint32_t    csiMemMap3[] =
    {
        0x20089F81U,    /* DSI_VC_CTRL0             */
        0x00000000U,    /* DSI_VC_TE0               */
        0x00000000U,    /* DSI_VC_LONG_PKT_HEADER   */
        0x00000000U,    /* DSI_VC_LONG_PKT_PAYLOAD  */
        0x00000000U,    /* DSI_VC_SHORT_PKT_HEADER  */
        0x00000000U,
        0x00000000U,    /* DSI_VC_CTRL0_IRQ_STATUS  */
        0x00000000U     /* DSI_VC_CTRL0_IRQ_ENABLE  */
    };
    uint32_t    csiPhyMemMap[] =
    {
        0x11241220U,    /* REGISTER0                */
        0x0006123BU,    /* REGISTER1                */
        0xB880000EU,    /* REGISTER2                */
        0x625D21A0U,    /* REGISTER3                */
        0x625D21A0U,    /* REGISTER4                */
        0x7F000000U,    /* REGISTER5                */
        0x00010000U,    /* REGISTER6                */
        0x00000000U,    /* REGISTER7                */
        0x00000410U,    /* REGISTER8                */
        0x00020001U,    /* REGISTER9                */
        0x00000000U,    /* REGISTER10               */
        0x7D20A59AU,    /* REGISTER11               */
        0x04320000U,    /* REGISTER12               */
        0x00000000U,    /* REGISTER13               */
        0x3E080000U,    /* REGISTER14               */
        0x01400003U     /* REGISTER15               */
    };

    /* Initialize the CSI Configuration */
    memset ((void *)&csiCfg, 0, sizeof(CSI_Cfg));

    /* Populate the initialization configuration: */
    csiCfg.socHandle               = socHandle;
    csiCfg.fifoArbiteration        = CSI_FIFO_Arbiteration_RR;
    csiCfg.dataRate                = CSI_DataRate_450Mbps;
    CSI_phyParamsInit (csiCfg.dataRate, &csiCfg.phyParams);

    /* Setup the Virtual Channel 1 */
    csiCfg.virtualChannelCfg[0].channelId  = CSI_VirtualCh_CH0;
    csiCfg.virtualChannelCfg[0].rxFIFOSize = CSI_VC_FIFO_Size_32;
    csiCfg.virtualChannelCfg[0].txFIFOSize = CSI_VC_FIFO_Size_32;

    /* No other virtual channel is configured */
    csiCfg.virtualChannelCfg[1].channelId  = CSI_VirtualCh_INVALID;
    csiCfg.virtualChannelCfg[2].channelId  = CSI_VirtualCh_INVALID;
    csiCfg.virtualChannelCfg[3].channelId  = CSI_VirtualCh_INVALID;

    /* Initialize the CSI Driver: */
    csiHandle = CSI_open (&csiCfg, &errCode);
    if (csiHandle == NULL)
    {
        System_printf ("Error: CSI open failed [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult("CSI_open", MCPI_TestResult_FAIL);
        return -1;
    }
    MCPI_setFeatureTestResult("CSI_open", MCPI_TestResult_PASS);

    /********************************************************************************
     * Validations: Memory Map1
     ********************************************************************************/
    ptrMemory = (uint32_t*)SOC_XWR14XX_MSS_CSI_PROT_ENG_BASE_ADDRESS;
    for (index = 0U; index < (sizeof(csiMemMap1)/4); index++)
    {
        if (*(ptrMemory + index) != csiMemMap1[index])
        {
            System_printf ("Error: CSI Memory Map1 Validation Failed [Expected 0x%x Got 0x%x]\n",
                            csiMemMap1[index], *(ptrMemory + index));
            MCPI_setFeatureTestResult("CSI Memory Map1 Validation", MCPI_TestResult_FAIL);
            return -1;
        }
    }
    System_printf ("Debug: Memory Map1 validated\n");
    MCPI_setFeatureTestResult("CSI Memory Map1 Validation", MCPI_TestResult_PASS);

    /********************************************************************************
     * Validations: Memory Map3 [This is the VC0 Memory Map]
     ********************************************************************************/
    ptrMemory = (uint32_t*)(SOC_XWR14XX_MSS_CSI_PROT_ENG_BASE_ADDRESS + 0x100);
    for (index = 0U; index < (sizeof(csiMemMap3)/4); index++)
    {
        if (*(ptrMemory + index) != csiMemMap3[index])
        {
            System_printf ("Error: CSI Memory Map3 Validation Failed [Expected 0x%x Got 0x%x]\n",
                            csiMemMap3[index], *(ptrMemory + index));
            MCPI_setFeatureTestResult("CSI Memory Map3 Validation", MCPI_TestResult_FAIL);
            return -1;
        }
    }
    System_printf ("Debug: Memory Map3 [Virtual Channel0] validated\n");
    MCPI_setFeatureTestResult("CSI Memory Map3 [Virtual Channel0] validation", MCPI_TestResult_PASS);

    /********************************************************************************
     * Validations: Memory Map4 [This is the PHY Memory Map]
     ********************************************************************************/
    ptrMemory = (uint32_t*)(SOC_XWR14XX_MSS_CSI_PHY_BASE_ADDRESS);
    for (index = 0U; index < (sizeof(csiPhyMemMap)/4); index++)
    {
        if (*(ptrMemory + index) != csiPhyMemMap[index])
        {
            System_printf ("Error: CSI PHY Memory Map Validation Failed [Expected 0x%x Got 0x%x]\n",
                            csiPhyMemMap[index], *(ptrMemory + index));
            MCPI_setFeatureTestResult("CSI PHY Memory Map Validation", MCPI_TestResult_FAIL);
            return -1;
        }
    }
    System_printf ("Debug: CSI PHY Memory Map validated\n");
    MCPI_setFeatureTestResult("CSI PHY Memory Map validation", MCPI_TestResult_PASS);

    /********************************************************************************
     * Validations: Memory Map2
     ********************************************************************************/
    ptrMemory = (uint32_t*)(SOC_XWR14XX_MSS_CSI_PROT_ENG_BASE_ADDRESS + 0x54);
    for (index = 0U; index < (sizeof(csiMemMap2)/4); index++)
    {
        if (*(ptrMemory + index) != csiMemMap2[index])
        {
            System_printf ("Error: CSI Memory Map2 Validation Failed [Expected 0x%x Got 0x%x]\n",
                            csiMemMap2[index], *(ptrMemory + index));
            MCPI_setFeatureTestResult("CSI Memory Map2 Validation", MCPI_TestResult_FAIL);
            return -1;
        }
    }
    System_printf ("Debug: Memory Map2 validated\n");
    MCPI_setFeatureTestResult("CSI Memory Map2 Validation", MCPI_TestResult_PASS);

    /* Close the driver */
    if (CSI_close (csiHandle, &errCode) < 0)
    {
        System_printf ("Error: CSI Driver close failed [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult("CSI_close", MCPI_TestResult_FAIL);
        return -1;
    }
    System_printf ("Debug: CSI Memory Map verification passed\n");
    MCPI_setFeatureTestResult("CSI_close", MCPI_TestResult_PASS);
    return 0;
}

/**
 *  @b Description
 *  @n
 *      System Initialization Task which initializes the various
 *      components in the system.
 *
 *  @retval
 *      Not Applicable.
 */
static void Test_initTask(UArg arg0, UArg arg1)
{
    CSI_Cfg         csiCfg;
    CSI_Handle      csiHandle;
    SOC_Handle      socHandle;
    int32_t         errCode;
    SOC_Cfg         socCfg;

    /* Initialize the SOC confiugration: */
    memset ((void *)&socCfg, 0, sizeof(SOC_Cfg));

    /* Populate the SOC configuration: */
    socCfg.clockCfg = SOC_SysClock_INIT;

    /* Initialize the SOC Module: This is done as soon as the application is started
     * to ensure that the MPU is correctly configured. */
    socHandle = SOC_init (&socCfg, &errCode);
    if (socHandle == NULL)
    {
        System_printf ("Error: SOC Module Initialization failed [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult("SOC Module Initialization", MCPI_TestResult_FAIL);
        return;
    }

    /*********************************************************************************
     * Test: CSI Driver Initialization and memory map verification
     *********************************************************************************/
    if (Test_csiMemoryMap(socHandle) < 0)
    {
        System_printf ("Error: CSI Driver Memory Map failed\n");
        MCPI_setFeatureTestResult("CSI Driver Initialization and memory map verification", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult("CSI Driver Initialization and memory map verification", MCPI_TestResult_PASS);

    /*********************************************************************************
     * Test: CSI Driver Initialization with no virtual channels
     *********************************************************************************/

    /* Initialize the CSI Configuration */
    memset ((void *)&csiCfg, 0, sizeof(CSI_Cfg));

    /* Populate the initialization configuration: */
    csiCfg.socHandle               = socHandle;
    csiCfg.fifoArbiteration        = CSI_FIFO_Arbiteration_RR;
    csiCfg.dataRate                = CSI_DataRate_450Mbps;
    CSI_phyParamsInit (csiCfg.dataRate, &csiCfg.phyParams);

    /* No other virtual channel is configured */
    csiCfg.virtualChannelCfg[0].channelId  = CSI_VirtualCh_INVALID;
    csiCfg.virtualChannelCfg[1].channelId  = CSI_VirtualCh_INVALID;
    csiCfg.virtualChannelCfg[2].channelId  = CSI_VirtualCh_INVALID;
    csiCfg.virtualChannelCfg[3].channelId  = CSI_VirtualCh_INVALID;

    /* Initialize the CSI Driver: */
    csiHandle = CSI_open (&csiCfg, &errCode);
    if (csiHandle != NULL)
    {
        System_printf ("Error: CSI Driver Initialized successfully with no virtual channels\n");
        MCPI_setFeatureTestResult("CSI Driver Initialization with no virtual channels", MCPI_TestResult_FAIL);
        return;
    }
    if (errCode != CSI_EINVAL)
    {
        System_printf ("Error: CSI Driver Initialization failed with invalid error code %d\n", errCode);
        MCPI_setFeatureTestResult("CSI Driver Initialization with no virtual channels", MCPI_TestResult_FAIL);
        return;
    }
    System_printf ("Debug: CSI Driver Initialization with no virtual channels test passed\n");
    MCPI_setFeatureTestResult("CSI Driver Initialization with no virtual channels", MCPI_TestResult_PASS);

    /*********************************************************************************
     * Test: CSI Driver Initialization with virtual channels FIFO exceeding limits
     *********************************************************************************/

    /* Initialize the CSI Configuration */
    memset ((void *)&csiCfg, 0, sizeof(CSI_Cfg));

    /* Populate the initialization configuration: */
    csiCfg.socHandle               = socHandle;
    csiCfg.fifoArbiteration        = CSI_FIFO_Arbiteration_RR;
    csiCfg.dataRate                = CSI_DataRate_450Mbps;
    CSI_phyParamsInit (csiCfg.dataRate, &csiCfg.phyParams);

    /* Setup the Virtual Channel 0 */
    csiCfg.virtualChannelCfg[0].channelId  = CSI_VirtualCh_CH0;
    csiCfg.virtualChannelCfg[0].rxFIFOSize = CSI_VC_FIFO_Size_128;
    csiCfg.virtualChannelCfg[0].txFIFOSize = CSI_VC_FIFO_Size_128;

    /* Setup the Virtual Channel 1 */
    csiCfg.virtualChannelCfg[1].channelId  = CSI_VirtualCh_CH1;
    csiCfg.virtualChannelCfg[1].rxFIFOSize = CSI_VC_FIFO_Size_32;
    csiCfg.virtualChannelCfg[1].txFIFOSize = CSI_VC_FIFO_Size_64;

    /* No other virtual channel is configured */
    csiCfg.virtualChannelCfg[2].channelId  = CSI_VirtualCh_INVALID;
    csiCfg.virtualChannelCfg[3].channelId  = CSI_VirtualCh_INVALID;

    /* Initialize the CSI Driver: */
    csiHandle = CSI_open (&csiCfg, &errCode);
    if (csiHandle != NULL)
    {
        System_printf ("Error: CSI Driver Initialized successfully with invalid FIFO size\n");
        MCPI_setFeatureTestResult("CSI Driver Initialization with virtual channels FIFO exceeding limits", MCPI_TestResult_FAIL);
        return;
    }
    if (errCode != CSI_EINVAL)
    {
        System_printf ("Error: CSI Driver Initialization failed with invalid error code %d\n", errCode);
        MCPI_setFeatureTestResult("CSI Driver Initialization with virtual channels FIFO exceeding limits", MCPI_TestResult_FAIL);
        return;
    }
    System_printf ("Debug: CSI Driver Initialization with invalid FIFO size test passed\n");
    MCPI_setFeatureTestResult("CSI Driver Initialization with virtual channels FIFO exceeding limits", MCPI_TestResult_PASS);

    /*********************************************************************************
     * Test: CSI Driver Initialization with correct virtual channel configuration
     *********************************************************************************/

    /* Initialize the CSI Configuration */
    memset ((void *)&csiCfg, 0, sizeof(CSI_Cfg));

    /* Populate the initialization configuration: */
    csiCfg.socHandle               = socHandle;
    csiCfg.fifoArbiteration        = CSI_FIFO_Arbiteration_RR;
    csiCfg.dataRate                = CSI_DataRate_450Mbps;
    CSI_phyParamsInit (csiCfg.dataRate, &csiCfg.phyParams);

    /* Setup the Virtual Channel 1 */
    csiCfg.virtualChannelCfg[0].channelId  = CSI_VirtualCh_CH0;
    csiCfg.virtualChannelCfg[0].rxFIFOSize = CSI_VC_FIFO_Size_32;
    csiCfg.virtualChannelCfg[0].txFIFOSize = CSI_VC_FIFO_Size_32;

    /* Setup the Virtual Channel 2 */
    csiCfg.virtualChannelCfg[1].channelId  = CSI_VirtualCh_CH1;
    csiCfg.virtualChannelCfg[1].rxFIFOSize = CSI_VC_FIFO_Size_64;
    csiCfg.virtualChannelCfg[1].txFIFOSize = CSI_VC_FIFO_Size_64;

    /* Setup the Virtual Channel 3 */
    csiCfg.virtualChannelCfg[3].channelId  = CSI_VirtualCh_CH3;
    csiCfg.virtualChannelCfg[3].rxFIFOSize = CSI_VC_FIFO_Size_32;
    csiCfg.virtualChannelCfg[3].txFIFOSize = CSI_VC_FIFO_Size_32;

    /* Virtual Channel 2 is not configured */
    csiCfg.virtualChannelCfg[2].channelId  = CSI_VirtualCh_INVALID;

    /* Initialize the CSI Driver: */
    csiHandle = CSI_open (&csiCfg, &errCode);
    if (csiHandle == NULL)
    {
        System_printf ("Error: CSI Driver initialization failed [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult("CSI Driver Initialization with correct virtual channel configuration", MCPI_TestResult_FAIL);
        return;
    }
    System_printf ("Debug: CSI Driver Initialization with valid virtual channel configuration passed\n", csiHandle);
    MCPI_setFeatureTestResult("CSI Driver Initialization with correct virtual channel configuration", MCPI_TestResult_PASS);

    /* Close the driver */
    if (CSI_close (csiHandle, &errCode) < 0)
    {
        System_printf ("Error: CSI Driver close failed [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult("CSI_close", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult("CSI_close", MCPI_TestResult_PASS);

    MCPI_setTestResult ();

    /* Exit BIOS */
    BIOS_exit(0);

    return;
}

/**
 *  @b Description
 *  @n
 *      Entry point into the test code.
 *
 *  @retval
 *      Not Applicable.
 */
int main (void)
{
	Task_Params     taskParams;

    /* Initialize the ESM: */
    ESM_init(0U); //dont clear errors as TI RTOS does it

    /* Initialize test logger */
    MCPI_Initialize();

    /* Debug Message: */
    System_printf ("**********************************************\n");
    System_printf ("Debug: Launching the CSI Unit Test\n");
    System_printf ("**********************************************\n");

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 4*1024;
    Task_create(Test_initTask, &taskParams, NULL);

    /* Start BIOS */
	BIOS_start();
    return 0;
}

