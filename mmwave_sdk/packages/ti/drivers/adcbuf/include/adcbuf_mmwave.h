/**
 *   @file  adcbuf_mmwave.h
 *
 *   @brief
 *      This is the internal Header for mmWave ADCBUF Driver. This header
 *      file should *NOT* be directly included by applications.
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

#ifndef ADCBUF_MMWAVE_H
#define ADCBUF_MMWAVE_H

#include <ti/drivers/osal/HwiP.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/osal/SemaphoreP.h>
#include <ti/drivers/soc/include/reg_dssreg.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief    ADCBUF Driver HW configuration
 *
 * @details
 *  The structure is used to store the hardware specific configuration which is
 *  passed to each driver instance
 *
 *  \ingroup ADCBUF_DRIVER_INTERNAL_DATA_STRUCTURE
 *
 */
typedef const struct ADCBuf_HwCfg_t
{
    /**
     * @brief   Base address of the DSS_REG registers.
     */
    DSSRegs*           ptrDSSRegBase;

    /**
     * @brief   Interrupt Number
     */
    uint32_t           interruptNum;

    /**
     * @brief   ADC Buffer base address
     */
    uint32_t           ptrADCBUFBaseAddr;

    /**
     * @brief   ADC Buffer base address
     */
    uint32_t           ptrCQBUFBaseAddr;
}ADCBuf_HwCfg;

/**
 *  @brief      ADCBufMMWave Object
 *
 *  The application must not access any member variables of this structure!
 *
 *  \ingroup ADCBUF_DRIVER_INTERNAL_DATA_STRUCTURE
 *
 */
typedef struct ADCBufMMWave_Object_t
{
    /**
      * @brief   Status of ADCBUF driver.
     */
    bool                   isOpen;

     /**
      * @brief   Base address of DSS Register Base.
     */
    DSSRegs                *ptrDssRegBase;

} ADCBufMMWave_Object, *ADCBufMMWave_Handle;

#ifdef __cplusplus
}
#endif

#endif /* ADCBUF_MMWAVE_H */
