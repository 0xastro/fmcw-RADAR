/*
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 /* ============================================================================
  */
 /** 
 *   @file  main.c
 *
 *   @brief
 *      CCS Debug Utility application code
 */

 /** @mainpage CCS Debug Utility
 *
 *      CCS Debug Utility: This utility can be flashed on the QSPI
 *      The application is written over SYSBIOS and will loop forever.
 *      In the meantime CCS can be attached and the developers can then download the real
 *      application which needs to be debugged.
 *
 *      This utility provides while loop application for MSS in XWR14xx
 *      and for MSS and DSS in XWR16xx/XWR18xx/XWR68xx.
 *
 *      On MSS, it calls ESM and SOC init functions to initialize the device in 
 *      known state. ESM init is needed to install the FIQ handler in case
 *      there are ESM errors on bootup. SOC init is needed to unhalt DSP in
 *      XWR16xx/XWR18xx/XWR68xx. As a byproduct, SOC init also unhalts the BSS and performs 
 *      the APLL close loop procedure. Hence when this debug utility is
 *      executed from flash, the system is running using the APLL clocks.
 *
 */
/* ============================================================================
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

#include <ti/sysbios/BIOS.h>
#include <ti/common/sys_common.h>

#ifdef SUBSYS_MSS
#include <xdc/runtime/System.h>

#include <ti/drivers/esm/esm.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/osal/DebugP.h>
#endif

#ifdef SUBSYS_DSS
#include <ti/sysbios/family/c64p/Cache.h>
#endif

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/

/* Global Variable which can be used to halt the debugger on startup*/
volatile int32_t gDebug = 0;

#ifdef SUBSYS_MSS
/* Enforce that when we are looping around waiting to connect CCS the CPU is in ARM mode.
   This is necessary because c_int00 is always in ARM mode.
   Ccsdebug can be built in ARM mode or thumb mode. 
   If ccsdebug is built in thumb mode, the CPU must switch to ARM mode before reaching c_int00,
   otherwise CPU will throw a CPU fault in the first instruction of c_int00.
   Placing the while loop inside a function that is forced to be compiled in ARM mode
   ensures that this condition is satisfied.
 */
#pragma CODE_STATE(enforceARMModeAndLoop, 32)
void enforceARMModeAndLoop (void)
{
    /* Loop around forever: */
    while (gDebug == 0);
}
#endif

/**
 *  @b Description
 *  @n
 *      Entry point into the CCS Debug Utility. The function does some system
 *      initialization and then simply causes the execution to be blocked forever.
 *
 *  @retval
 *      Not Applicable.
 */
int main (void)
{

#ifdef SUBSYS_MSS

    SOC_Cfg             socCfg;
    int32_t             errCode;
    SOC_Handle          socHandle;

    ESM_init(0U); //dont clear errors as TI RTOS does it

    /* Initialize the SOC confiugration: */
    memset ((void *)&socCfg, 0, sizeof(SOC_Cfg));
    /* Populate the SOC configuration: */
    socCfg.clockCfg = SOC_SysClock_INIT;
    /* Initialize the SOC Module: 
     *  - this is called here for bringing BSS and DSS out of reset. */
    socHandle = SOC_init (&socCfg, &errCode);
    if (socHandle == NULL)
    {
        System_printf ("Error: SOC Module Initialization failed [Error code %d]\n", errCode);
        /* Error: This should NOT fail */
        DebugP_assert(0);
    }
    
    /* Check if the SOC is a secure device */
    if (SOC_isSecureDevice(socHandle, &errCode))
    {
        /* Disable firewall for JTAG and LOGGER (UART) which is needed by all unit tests */
        SOC_controlSecureFirewall(socHandle, 
                                  (uint32_t)(SOC_SECURE_FIREWALL_JTAG | SOC_SECURE_FIREWALL_LOGGER),
                                  SOC_SECURE_FIREWALL_DISABLE,
                                  &errCode);
    }
    
    enforceARMModeAndLoop();
#endif

#ifdef SUBSYS_DSS
    Cache_Size     cacheSize;

    /* Disable all caches, this is to prevent any application from corrupting its
       SRAM load contents for caches that are smaller than the default coming from 
       common platform linker file in ti/platform/../c674x_linker.cmd. 
       This corruption can happen when application code
       runs from _c_int00 to the point where cache size is changed to the application's
       cache size (this will be done by BIOS code that runs between _c_int00 and main)
    */
    cacheSize.l1pSize = Cache_L1Size_0K;
    cacheSize.l1dSize = Cache_L1Size_0K;
    cacheSize.l2Size = Cache_L2Size_0K;
    Cache_setSize(&cacheSize);
    
    /* Loop around forever: */
    while (gDebug == 0);
#endif

    return 0;

}

