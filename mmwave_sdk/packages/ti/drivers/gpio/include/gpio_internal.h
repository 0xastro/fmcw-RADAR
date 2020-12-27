/**
 *   @file  gpio_internal.h
 *
 *   @brief
 *      This is the internal header file used by the GPIO Driver. The
 *      file is NOT exposed to the application developers and should
 *      not be directly included.
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
#ifndef GPIO_DRIVER_INTERNAL_H
#define GPIO_DRIVER_INTERNAL_H

#include <ti/drivers/gpio/gpio.h>
#include <ti/drivers/osal/HwiP.h>
#include <ti/drivers/gpio/include/reg_gio.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup GPIO_DRIVER_INTERNAL_DATA_STRUCTURE
 @{ */

inline void GPIO_DECODE_INDEX(uint8_t index, uint8_t* port, uint8_t* pin);

/**
 *  @b Description
 *  @n
 *      Utility function which is used to decode the index provided
 *      to the GPIO exported API into a GPIO Port & Pin which can be
 *      used to program the GPIO registers.
 *
 *      The implementation of this function and the macro GPIO_CREATE_INDEX
 *      should always be maintained.
 *
 *  @param[in]  index
 *      GPIO Index
 *  @param[out] port
 *      Translated Port
 *  @param[out] pin
 *      Translated Pin
 *
 *  @retval
 *      Not applicable
 */
inline void GPIO_DECODE_INDEX(uint8_t index, uint8_t* port, uint8_t* pin)
{
    *port = (index / 8U);
    *pin  = (index % 8U);
}

/**
 *  @b Description
 *  @n
 *      Helper macro which is used to create a unique index given the GPIO
 *      Port & Pin.
 *
 *  @param[in] PORT
 *      GPIO Port
 *  @param[in] PIN
 *      GPIO Pin
 *
 *  @retval
 *      Unique Index
 */
#define GPIO_CREATE_INDEX(PORT, PIN)       (((PORT)*8U) + (PIN))

/**
 * @brief
 *  GPIO Hardware Attributes
 *
 * @details
 *  The structure contains the hardware atrributes which are used
 *  to specify the platform specific configurations.
 */
typedef struct GPIO_Hw_Attrib_t
{
    /**
     * @brief   Pointer to the GPIO registers
     */
    GIORegs*        ptrGPIORegs;

    /**
     * @brief   Interrupt number associated with the high interrupts
     */
    uint8_t         highInterruptNum;

    /**
     * @brief   Interrupt number associated with the low interrupts
     */
    uint8_t         lowInterruptNum;
}GPIO_Hw_Attrib;

/*****************************************************************************
 * Extern Platform specific definition
 *****************************************************************************/
extern GPIO_Hw_Attrib gGPIOHwAtrrib;

/**
@}
*/

#ifdef __cplusplus
}
#endif


#endif /* GPIO_DRIVER_INTERNAL_H */

