/**
 *   @file  soc_xwr18xx.h
 *
 *   @brief
 *      This is the header file for the XWR18XX specific definitions needed by
 *    SOC driver.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2017 Texas Instruments, Inc.
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

#ifndef SOC_XWR18XX_DRIVER_H
#define SOC_XWR18XX_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 *  Enumeration for system peripheral module id
 *
 * @details
 *  The enumeration defines the system peripheral module id to be used to
 *  configure clock, memory initialization etc.
 */
typedef enum SOC_ModuleId_e
{
    /**
     * @brief
     *  DCAN module
     */
    SOC_MODULE_DCAN           = 1U,

    /**
     * @brief
     *  DMA module
     */
    SOC_MODULE_DMA,

    /**
     * @brief
     *  DMA2 module
     */
    SOC_MODULE_DMA2,

    /**
     * @brief
     *  MCAN module
     */
    SOC_MODULE_MCAN,

    /**
     * @brief
     *  QSPI module
     */
    SOC_MODULE_QSPI,

    /**
     * @brief
     *  SPIA module
     */
    SOC_MODULE_SPIA,

    /**
     * @brief
     *  SPIB module
     */
    SOC_MODULE_SPIB
}SOC_ModuleId;


/*******************************************************************************************************
 * SOC Driver platform specific Exported APIs:
 *******************************************************************************************************/

extern int32_t SOC_enableHWA(SOC_Handle handle, int32_t* errCode);

/* These functions are exported for use by the mmWave module. */
extern int32_t SOC_isMMWaveMSSOperational(SOC_Handle handle, int32_t* errCode);
extern int32_t SOC_setMMWaveMSSLinkState(SOC_Handle handle, uint8_t state, int32_t* errCode);
extern int32_t SOC_isMMWaveDSSOperational(SOC_Handle handle, int32_t* errCode);
extern int32_t SOC_setMMWaveDSSLinkState(SOC_Handle handle, uint8_t state, int32_t* errCode);
extern int32_t SOC_setMSSLinkState(SOC_Handle handle, uint8_t state, int32_t* errCode);
extern int32_t SOC_isMSSOperational(SOC_Handle handle, int32_t* errCode);
extern uint32_t SOC_getMSSVCLKFrequency(SOC_Handle handle, int32_t* errCode);

extern int32_t SOC_setDSSLinkState(SOC_Handle handle, uint8_t state, int32_t* errCode);
extern int32_t SOC_isDSSOperational(SOC_Handle handle, int32_t* errCode);

#ifdef SUBSYS_DSS
extern int32_t SOC_triggerDSStoMSSsoftwareInterrupt(SOC_Handle handle,
    uint8_t interruptNumber, int32_t* errCode);
extern int32_t SOC_configureDSSESMMask(SOC_Handle handle, uint8_t errorNumber, uint8_t mask, int32_t *errCode);
#endif

#ifdef __cplusplus
}
#endif

#endif /* SOC_XWR18XX_DRIVER_H */

