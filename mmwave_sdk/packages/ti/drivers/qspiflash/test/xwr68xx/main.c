/*
 *   @file  main.c
 *
 *   @brief
 *      Unit Test code for the MibSpi Driver
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
#include <ti/sysbios/family/arm/v7a/Pmu.h>

/* mmWave SK Include Files: */
#include <ti/common/sys_common.h>
#include <ti/drivers/pinmux/pinmux.h>
#include <ti/drivers/osal/HwiP.h>
#include <ti/drivers/esm/esm.h>

/* SOC register file */
#include <ti/drivers/soc/soc.h>

extern void Test_initTask(UArg arg0, UArg arg1);

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
    SOC_Handle      socHandle;
    int32_t         errCode;
    SOC_Cfg         socCfg;

    /* Initialize the ESM: Dont clear errors as TI RTOS does it */
    ESM_init(0U);

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
        return -1;
    }

    /* Wait for BSS powerup */
    if (SOC_waitBSSPowerUp(socHandle, &errCode) < 0)
    {
        /* Debug Message: */
        System_printf ("Debug: SOC_waitBSSPowerUp failed with Error [%d]\n", errCode);
        goto Exit;
    }

    /* Setup the PINMUX to bring out the QSPI */
    Pinmux_Set_OverrideCtrl(SOC_XWR68XX_PINR12_PADAP, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR68XX_PINR12_PADAP, SOC_XWR68XX_PINR12_PADAP_QSPI_CLK);

    Pinmux_Set_OverrideCtrl(SOC_XWR68XX_PINP11_PADAQ, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR68XX_PINP11_PADAQ, SOC_XWR68XX_PINP11_PADAQ_QSPI_CSN);

    Pinmux_Set_OverrideCtrl(SOC_XWR68XX_PINR13_PADAL, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR68XX_PINR13_PADAL, SOC_XWR68XX_PINR13_PADAL_QSPI_D0);

    Pinmux_Set_OverrideCtrl(SOC_XWR68XX_PINN12_PADAM, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR68XX_PINN12_PADAM, SOC_XWR68XX_PINN12_PADAM_QSPI_D1);

    Pinmux_Set_OverrideCtrl(SOC_XWR68XX_PINR14_PADAN, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR68XX_PINR14_PADAN, SOC_XWR68XX_PINR14_PADAN_QSPI_D2);

    Pinmux_Set_OverrideCtrl(SOC_XWR68XX_PINP12_PADAO, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR68XX_PINP12_PADAO, SOC_XWR68XX_PINP12_PADAO_QSPI_D3);

    /* Debug Message: */
    System_printf ("******************************************\n");
    System_printf ("Debug: QSPI Driver Test Application Start \n");
    System_printf ("******************************************\n");

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 6*1024;
    Task_create(Test_initTask, &taskParams, NULL);

    /* Start BIOS */
    BIOS_start();
Exit:
    return 0;
}

