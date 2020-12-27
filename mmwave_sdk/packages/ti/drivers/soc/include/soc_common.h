/**
 *   @file  soc_common.h
 *
 *   @brief
 *      This is the header file for the SOC driver which exposes the
 *      data structures which can be used by the applications to use 
 *      the SOC driver.
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

#ifndef SOC_COMMON_H
#define SOC_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup SOC_DRIVER_ERROR_CODE
 *  Base error code for the SOC module is defined in the
 *  \include ti/common/mmwave_error.h
 @{ */

/**
 * @brief   Error Code: Invalid argument
 */
#define SOC_EINVAL                  (MMWAVE_ERRNO_SOC_BASE-1)

/**
 * @brief   Error Code: Out of memory
 */
#define SOC_ENOMEM                  (MMWAVE_ERRNO_SOC_BASE-2)

/**
 * @brief   Error Code: Internal error
 */
#define SOC_EINTERNAL               (MMWAVE_ERRNO_SOC_BASE-3)

/**
 * @brief   Error Code: Limits have been exceeded.
 */
#define SOC_ENOSPACE                (MMWAVE_ERRNO_SOC_BASE-4)

/**
 * @brief   Error Code: BL error.
 */
#define SOC_EBLERR                  (MMWAVE_ERRNO_SOC_BASE-5)

/**
 * @brief   Error Code: SOC_translateAddress couldn't translate the provided address
 *          This value is chosen since all other values can map to a valid address in
 *          some domain.
 */
#define SOC_TRANSLATEADDR_INVALID    (0xFFFFFFFFU)


/**
@}
*/

/** @addtogroup SOC_DRIVER_EXTERNAL_DATA_STRUCTURE
 @{ */

/**
 * @brief
 *  SOC Driver Handle
 */
typedef void*   SOC_Handle;

/**
 * @brief
 *  SOC System Interrupt Listener Handle
 */
typedef void*   SOC_SysIntListenerHandle;

/**
 * @brief
 *  SOC System Interrupt Listener Function
 */
typedef void (*SOC_SysIntListenerFxn)(uintptr_t arg);

/**
 * @brief
 *  Enumeration for system clocks
 *
 * @details
 *  The enumeration describes the configuration of the system clock.
 *  - On the MSS in the XWR14xx; the System clocks need to be initialized
 *    and setup during SOC Initialization.
 *  - On the XWR16xx/XWR18xx/XWR68xx; either the MSS *OR* DSS can perform clock initialization
 *    Application developers need to determine upfront the system responsible
 *    for doing so.
 */
typedef enum SOC_SysClock_e
{
    /**
     * @brief
     *  System Clocks need to be initialized
     */
    SOC_SysClock_INIT           = 0x1,

    /**
     * @brief
     *  System Clocks initialization is bypassed
     */
    SOC_SysClock_BYPASS_INIT
}SOC_SysClock;

/**
 * @brief
 *  Enumeration for address translation from one domain to the other
 *
 * @details
 *  The enumeration describes the direction of address translation.
 */
typedef enum SOC_TranslateAddr_Dir_e
{
    /**
     * @brief
     *  CPU to EDMA address translation
     */
    SOC_TranslateAddr_Dir_TO_EDMA,

    /**
     * @brief
     *  EDMA to CPU address translation
     */
    SOC_TranslateAddr_Dir_FROM_EDMA,

#if defined(SOC_XWR16XX) || defined(SOC_XWR18XX) || defined(SOC_XWR68XX)
    /**
     * @brief
     *  Cross CPU address translation (XWR16xx/XWR18xx/XWR68xx only - MSS to DSS or DSS to MSS)
     *    From the running core to the other core
     */
    SOC_TranslateAddr_Dir_TO_OTHER_CPU,

    /**
     * @brief
     *  Cross CPU address translation (XWR16xx/XWR18xx/XWR68xx only - MSS to DSS or DSS to MSS)
     *    From the other core to the running core
     */
    SOC_TranslateAddr_Dir_FROM_OTHER_CPU
#endif
}SOC_TranslateAddr_Dir;

/**
 * @brief
 *  SOC RCM Module clock Source
 *
 * @details
 *  The enumeration defines the module clock source supported by XWR1xxx device.
 */
typedef enum SOC_PeripheralClkSource_e
{
    /*! VCLK */
    SOC_CLKSOURCE_VCLK = 0U,

    /*! RCCLK */
    SOC_CLKSOURCE_RCCLK = 1U,

    /*! 600MPLL */
    SOC_CLKSOURCE_600MPLL = 2U,

    /*! 240MPLL */
    SOC_CLKSOURCE_240MPLL = 3U,

    /*! CPUCLK */
    SOC_CLKSOURCE_CPUCLK = 4U,

    /*! REFCLK */
    SOC_CLKSOURCE_REFCLK = 6U
}SOC_PeripheralClkSource;

/**
 * @brief
 *  Enumeration for MPU configuration.
 *
 * @details
 *  The enumeration describes the configuration of the memory protection unit.
 *  MPU can be intialized with default values as determined by the SOC_mpu_config() function.
 *  OR
 *  Application can bypass the default MPU initialization and configure MPU as per the application requirement.
 *  In this case, MPU MUST be configured by the application immediately after calling SOC_init().
 */
typedef enum SOC_MPUCfg_e
{
    /**
     * @brief
     *  Memory protection unit to be initialized
     */
    SOC_MPUCfg_CONFIG           = 0x0,

    /**
     * @brief
     *  Memory protection unit initialization is bypassed
     */
    SOC_MPUCfg_BYPASS_CONFIG
}SOC_MPUCfg;

/**
 * @brief
 *  Enumeration for DSS configuration.
 *
 * @details
 *  The enumeration describes the desired state of DSP subsystem. 
 *  It can be configured to unhalt/power up the DSS or leave in its original state - un-halted state (post ROM BL) 
 *  or spinning in while loop (post ccsdebug) .
 *  Note: If the DSS is left in the halted state, it is the application's responsibility to unhalt and power up DSS when required.
 */
typedef enum SOC_DSSCfg_e
{
    /**
     * @brief
     *  Unhalt and powerup DSS.
     */
    SOC_DSSCfg_UNHALT             = 0x0,

    /**
     * @brief
     *  No action is done when this option is provided
     *  So DSS is left in the original state - either halted or spinning.
     */
    SOC_DSSCfg_HALT
}SOC_DSSCfg;

/**
 * @brief
 *  SOC Configuration
 *
 * @details
 *  The structure describes the configuration information which is needed
 *  to open and initialize the SOC Driver.
 */
typedef struct SOC_Cfg_t
{
    /**
     * @brief
     *  System clock configuration
     */
    SOC_SysClock        clockCfg;

#if defined(SUBSYS_MSS)
    /**
     * @brief
     *  Memory protection unit configuration
     */
    SOC_MPUCfg          mpuCfg;

    /**
     * @brief
     *  DSP sub system configuration
     */
    SOC_DSSCfg          dssCfg;
#endif
}SOC_Cfg;

/**
 * @brief
 *  SOC Interrupt Listener Configuration
 *
 * @details
 *  The structure describes the configuration information which is need to initialize
 *  the system interrupt listener
 */
typedef struct SOC_SysIntListenerCfg_t
{
    /**
     * @brief
     *  System interrupt on which the listener is to be registered
     */
    uint32_t                systemInterrupt;

    /**
     * @brief
     *  Listener Function to be registered
     */
    SOC_SysIntListenerFxn   listenerFxn;

    /**
     * @brief
     *  Optional argument with which the listener function is to be invoked on
     *  an ISR
     */
    uintptr_t               arg;
}SOC_SysIntListenerCfg;

/**
 * @brief
 *  SOC Secure device firewall capable modules
 *
 * @details
 *  The bitmap enum defines the modules that can have firewall enabled/disabled on a secure device
 *  Please note that secure variant may not be available for all device types
 */
typedef enum SOC_SecureFirewallModule_e
{
    /*! JTAG */
    SOC_SECURE_FIREWALL_JTAG = 0x1U,

    /*! Secure RAM */
    SOC_SECURE_FIREWALL_SECURERAM = 0x2U,

    /*! Logger */
    SOC_SECURE_FIREWALL_LOGGER = 0x4U,

    /*! Trace */
    SOC_SECURE_FIREWALL_TRACE = 0x8U,

    /*! CRYPTO */
    SOC_SECURE_FIREWALL_CRYPTO = 0x10U,

    /*! CEK1, CEK2 firewall */
    SOC_SECURE_FIREWALL_CUSTCEK = 0x20U,

    /*! DMM */
    SOC_SECURE_FIREWALL_DMM = 0x40U

}SOC_SecureFirewallModule;

/**
 * @brief
 *  SOC device part number definition
 *
 * @details
 *  The enum defines the supported radar devices' part number reading from TOPRCM efuse register
 */
typedef enum SOC_PartNumber_e
{
    /*! AWR14xx devices */
    SOC_AWR14XX_nonSecure_PartNumber = 0x40U,

    /*! IWR14xx devices */
    SOC_IWR14XX_nonSecure_PartNumber = 0xA0U,

    /*! AWR16xx non secure device */
    SOC_AWR16XX_nonSecure_PartNumber = 0x60U,  

    /*! IWR16xx non secure device */
    SOC_IWR16XX_nonSecure_PartNumber = 0xC0U,

    /*! AWR16xx secure devices */
    SOC_AWR16XX_Secure_PartNumber       = 0x61U,

    /*! IWR16xx secure devices */
    SOC_IWR16XX_Secure_PartNumber       = 0xC1U,

    /*! AWR18xx non-secure devices */
    SOC_AWR18XX_nonSecure_PartNumber       = 0x70U,

    /*! AWR18xx non-secure devices */
    SOC_AWR18XX_Secure_PartNumber       = 0x71U,

    /*! IWR18xx non-secure devices */
    SOC_IWR18XX_nonSecure_PartNumber       = 0xD0U,

    /*! IWR68xx non-secure devices */
    SOC_IWR68XX_nonSecure_PartNumber       = 0xE0U
}SOC_PartNumber;

#define SOC_SECURE_FIREWALL_DISABLE 1 /* firewall disable (enable access to module) */
#define SOC_SECURE_FIREWALL_ENABLE  0 /* firewall enable (disable access to module) */

/**
@}
*/

#ifdef __cplusplus
}
#endif

#endif /* SOC_COMMON_H */

