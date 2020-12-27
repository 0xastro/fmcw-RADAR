/*!
 *   @file  soc_internal.h
 *
 *   @brief
 *      This is an internal header file used by the SOC module and should
 *      not be included directly by the applications.
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
#ifndef SOC_INTERNAL_H
#define SOC_INTERNAL_H

#include <ti/common/sys_common.h>
#include <ti/drivers/osal/HwiP.h>

#include <ti/drivers/soc/include/reg_rcm.h>
#include <ti/drivers/soc/include/reg_toprcm.h>
#include <ti/drivers/soc/include/reg_gpcfg.h>
#include <ti/drivers/soc/include/reg_dssreg.h>

#ifdef SUBSYS_DSS
#include <ti/drivers/soc/include/reg_dspicfg.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 *  The SOC Driver is responsible for the registration and handling
 *  of system interrupts. It then provides applications and drivers to
 *  register their interrupts which are then invoked in the order of
 *  registration. The definition is the maximum number of system
 *  interrupts which are supported by this model.
 *
 *  \ingroup SOC_DRIVER_INTERNAL_DEFINITION
 */
#define SOC_MAX_SYSTEM_INTERRUPT            4U

/**
 * @brief
 *  This definition exposes the maximum number of listeners which
 *  can be attached to a system interrupt.
 *
 *  \ingroup SOC_DRIVER_INTERNAL_DEFINITION
 */
#define SOC_MAX_LISTEN_TABLE                4U

/*! @brief
 *    Defines used to ensure memory programmed as MPU base address is aligned to 1KB
 *    \ingroup SOC_DRIVER_INTERNAL_DEFINITION
 */
#define ONE_KB                              (1024U)

/*! @brief
 *    Defines used to ensure memory programmed as MPU base address is aligned to 1MB
 *    \ingroup SOC_DRIVER_INTERNAL_DEFINITION
 */
#define ONE_MB                              (ONE_KB * ONE_KB)

/**
 * @brief
 *  SOC Hardware Atrributes
 *
 * @details
 *  SOC Driver Hardware Attributes which allow the driver to be ported
 *  to different SOC. This is an internal structure and is not exposed
 *  or configurable by the applications.
 *
 *  \ingroup SOC_DRIVER_INTERNAL_DATA_STRUCTURE
 */
typedef struct SOC_HwAttrib_t
{
    /**
     * @brief   Base address of the MSS RCM Module
     */
    uint32_t    rcmBaseAddress;

    /**
     * @brief   Base address of the MSS TOP RCM Module
     */
    uint32_t    topRcmBaseAddress;

    /**
     * @brief   Base address of the MSS GPCFG Module
     */
    uint32_t    gpcfgRegBaseAddress;

    /**
     * @brief   Base address of the MSS DSS Module
     */
    uint32_t    dssRegBaseAddress;

#if defined(SOC_XWR16XX) || defined(SOC_XWR18XX) || defined(SOC_XWR68XX)
    /**
     * @brief   Base address of the MSS DSS2 Module, supported in XWR16xx devices
     */
    uint32_t    dssReg2BaseAddress;

#ifdef SUBSYS_DSS
    /**
     * @brief   Base address of the DSP (C674) ICFG Module.
     */
    uint32_t    dspIcfgRegBaseAddress;
#endif
#endif
}SOC_HwAttrib;

/**
 * @brief
 *  SOC System Interrupt Listener
 *
 * @details
 *  The structure defines a listener block which is listening and needs
 *  to be notified on the system interrupt.
 *
 *  \ingroup SOC_DRIVER_INTERNAL_DATA_STRUCTURE
 */
typedef struct SOC_SysInterruptListener_t
{
    /**
     * @brief   HWI Listener function to be invoked on the system interrupt
     */
    HwiP_Fxn        hwiListenerFxn;

    /**
     * @brief   Argument with which the listener was registered.
     */
    uintptr_t       arg;
}SOC_SysInterruptListener;

/**
 * @brief
 *  SOC System Interrupt
 *
 * @details
 *  The structure describes the mapping of the system interrupt to
 *  a corresponding ISR.
 *
 *  \ingroup SOC_DRIVER_INTERNAL_DATA_STRUCTURE
 */
typedef struct SOC_SysInterrupt_t
{
    /**
     * @brief   System Interrupt Number
     */
    uint8_t                     intNum;

    /**
     * @brief   Registered HWI Handle
     */
    HwiP_Handle                 hwiHandle;

    /**
     * @brief   Interrupt counter used for debugging.
     */
    uint32_t                    numInterrupts;

    /**
     * @brief   Table of registered listeners
     */
    SOC_SysInterruptListener    listenTable[SOC_MAX_LISTEN_TABLE];
}SOC_SysInterrupt;

/**
 * @brief
 *  SOC Master Control Block
 *
 * @details
 *  The structure describes the SOC Driver and is used to hold the relevant
 *  information with respect to the SOC module.
 *
 *  \ingroup SOC_DRIVER_INTERNAL_DATA_STRUCTURE
 */
typedef struct SOC_MCB_t
{
    /**
     * @brief   Configuration used to initialize the SOC module
     */
    SOC_Cfg             cfg;

    /**
     * @brief   This is a table of all the registered system interrupts.
     */
    SOC_SysInterrupt    sysIntTable[SOC_MAX_SYSTEM_INTERRUPT];

    /**
     * @brief   Pointer to the RCM registers
     */
    RCMRegs*            ptrRCMRegs;

    /**
     * @brief   Pointer to the TOP RCM registers
     */
    TOPRCMRegs*         ptrTopRCMRegs;

    /**
     * @brief   Pointer to the TOP RCM registers
     */
    GPCFGRegs*          ptrGPCFGRegs;

    /**
     * @brief   Pointer to the DSS_REG registers
     */
    DSSRegs*            ptrDSSRegs;

#if defined(SOC_XWR16XX) || defined(SOC_XWR18XX) || defined(SOC_XWR68XX)
    /**
     * @brief   Pointer to the DSS_REG2 registers
     */
    DSS2Regs*           ptrDSS2Regs;

#ifdef SUBSYS_DSS
    /**
     * @brief   Pointer to the DSPICFG registers
     */
    DSPICFGRegs*        ptrDSPICFGRegs;
#endif
#endif
}SOC_DriverMCB;

/**
 * @brief
 *  Enumeration for address translation lookup table
 *
 * @details
 *  The enumeration describes the direction of address translation. 
 *
 *  \ingroup SOC_DRIVER_INTERNAL_DATA_STRUCTURE
 */
typedef enum SOC_TranslateAddr_LUT_Type_e {
    SOC_TranslateAddr_LUT_Type_MSS = 0,         /*!< address as seen by MSS */
    SOC_TranslateAddr_LUT_Type_EDMA,            /*!< address as seen by EDMA */
#if defined(SOC_XWR16XX) || defined(SOC_XWR18XX) || defined(SOC_XWR68XX)
    SOC_TranslateAddr_LUT_Type_DSS,             /*!< address as seen by DSS */
#endif
    SOC_TranslateAddr_LUT_Type_SIZE,            /*!< address range */
    SOC_TranslateAddr_LUT_Type_MAX,             /*!< max - defined for array dimensioning */
    SOC_TranslateAddr_LUT_Type_INVALID          /*!< invalid - defined for invalid translation */
}SOC_TranslateAddr_LUT_Type;


/********************************************************************************
 * Extern Definitions:
 ********************************************************************************/
extern void SOC_deviceInit (SOC_DriverMCB* ptrSOCDriverMCB, int32_t* errCode);
extern void SOC_deviceDeinit(SOC_DriverMCB* ptrSOCDriverMCB, int32_t* errCode);
extern void SOC_SystemISR (uintptr_t arg);
extern SOC_TranslateAddr_LUT_Type SOC_getInAddr_LUTType(SOC_TranslateAddr_Dir direction);
extern SOC_TranslateAddr_LUT_Type SOC_getOutAddr_LUTType(SOC_TranslateAddr_Dir direction);


#ifdef __cplusplus
}
#endif

#endif /* SOC_INTERNAL_H */
