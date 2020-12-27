/**
 *   @file  csi_internal.h
 *
 *   @brief
 *      This is the internal header file used by the CSI Driver. The
 *      file is NOT exposed to the application developers and should
 *      not be directly included.
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

#ifndef CSI_DRIVER_INTERNAL_H
#define CSI_DRIVER_INTERNAL_H

#include <ti/drivers/csi/csi.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CSI_DRIVER_INTERNAL_DATA_STRUCTURE
 @{ */

/**
 * @brief
 *  CSI Virtual Channel MCB
 *
 * @details
 *  The structure is used to track the relevant configuration and run time
 *  information for the CSI Virtual Channel
 */
typedef struct CSI_VirtualChannelMCB_t
{
    /**
     * @brief   Flag which indicates if the virtual channel is active and has
     * been configured
     */
    uint8_t         isActive;

    /**
     * @brief   Flag which indicates that the virtual channel has a transmit
     * transaction in progress
     */
    uint8_t         isTxActive;
}CSI_VirtualChannelMCB;

/**
 * @brief
 *  CSI Driver
 *
 * @details
 *  The structure is used to track the relevant configuration and run time
 *  information for the CSI Driver.
 */
typedef struct CSI_DriverMCB_t
{
    /**
     * @brief   Configuration used to setup the driver.
     */
    CSI_Cfg                     cfg;

    /**
     * @brief   CSI PHY register space
     */
    CSI2PHYRegs*                ptrPhyReg;

    /**
     * @brief   CSI protocol engine register space
     */
    CSI2PROTOCOLENGINERegs*     ptrProtReg;

    /**
     * @brief   HSI Clock Rate
     */
    uint32_t                    hsiClock;

    /**
     * @brief   Database of virtual channels.
     */
    CSI_VirtualChannelMCB       virtualChannel[CSI_VirtualCh_MAX_CHANNEL];
}CSI_DriverMCB;

/***************************************************************************************
 * Internal Exported Defintions:
 ***************************************************************************************/
extern CSI2PHYRegs*              gCSIPhyRegister;
extern CSI2PROTOCOLENGINERegs*   gCSIProtocolEngineRegister;

/**
@}
*/

#ifdef __cplusplus
}
#endif

#endif /* CSI_DRIVER_INTERNAL_H */


