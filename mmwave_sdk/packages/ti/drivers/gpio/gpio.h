/*
 * Copyright (c) 2015-2016, Texas Instruments Incorporated
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
/** @mainpage GPIO Driver
 *
 *  The GPIO header file should be included in an application as follows:
 *  @code
 *  #include <ti/drivers/gpio/gpio.h>
 *  @endcode
 *
 *  The drivers/gpio/include/reg_gio.h has the register layer definitons for the
 *  GPIO Module.
 *
 *  # Operation #
 *
 *  The GPIO module allows you to manage General Purpose I/O pins via
 *  simple and portable APIs.
 *
 *  The application is required to call GPIO_init() before invoking any other
 *  driver API.
 *
 *  Asserts are used to verify that the driver has been initialized and
 *  to validate pin indexes within the various APIs.
 *
 *  Each platform will have its own GPIO Index which are defined in the
 *  corresponding platform header file. For example: Please refer to the
 *  drivers/gpio/include/gpio_xwr14xx.h for the GPIO pins available on the
 *  XWR14xx platform.
 */

/** @defgroup GPIO_DRIVER      GPIO Driver
 */
/**
@defgroup GPIO_DRIVER_EXTERNAL_FUNCTION            GPIO Driver External Functions
@ingroup GPIO_DRIVER
@brief
*   The section has a list of all external API which are exposed to the applications
*/
/**
@defgroup GPIO_DRIVER_EXTERNAL_DATA_STRUCTURE       GPIO Driver External Data structures
@ingroup GPIO_DRIVER
@brief
*   The section has a list of all external data structures which are exposed to the
*   application.
*/
/**
@defgroup GPIO_DRIVER_INTERNAL_DATA_STRUCTURE      GPIO Driver Internal Data Structures
@ingroup GPIO_DRIVER
@brief
*   The section has a list of all internal data structures which are used internally
*   by the GPIO module.
*/
/**
@defgroup GPIO_DRIVER_INTERNAL_FUNCTION            GPIO Driver Internal Functions
@ingroup GPIO_DRIVER
@brief
*   The section has a list of all internal API which are not exposed to the external
*   applications.
*/


#ifndef ti_drivers_GPIO__include
#define ti_drivers_GPIO__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <ti/common/mmwave_error.h>

#ifdef SOC_XWR14XX
#include <ti/drivers/gpio/include/gpio_xwr14xx.h>
#elif SOC_XWR16XX
#include <ti/drivers/gpio/include/gpio_xwr16xx.h>
#elif SOC_XWR18XX
#include <ti/drivers/gpio/include/gpio_xwr18xx.h>
#elif SOC_XWR68XX
#include <ti/drivers/gpio/include/gpio_xwr68xx.h>
#endif

/** @addtogroup GPIO_DRIVER_EXTERNAL_DATA_STRUCTURE
 @{ */


/*!
 *  @brief  GPIO pin configuration settings
 *
 *  The upper 16 bits of the 32 bit PinConfig is reserved
 *  for pin configuration settings.
 *
 *  The lower 16 bits are reserved for device-specific
 *  port/pin identifications
 *
 *  The figures below represent the distribution of the higher
 *  order 32 bits.
 *
 * @verbatim
    Input/Output Field: [Refer to the Input/Output configuration macros]
   ------------------------------------------
   | 23 | 22 | 21 | 20 | 19 | 18 | 17 | 16 |
   ------------------------------------------

   Interrupt Fields: [Refer to the Interrupt configuration macros]
   --------------------------------
   | 29 | 28 | 27 | 26 | 25 | 24 |
   --------------------------------

   Internally used [Reserved fields]
   -------------
    | 31 | 30 |
   -------------
   @endverbatim
 *
 */
typedef uint32_t GPIO_PinConfig;

/*!
 *  @cond NODOC
 *  Internally used configuration bit access macros.
 */
#define GPIO_CFG_IO_MASK           0x00FF0000U
#define GPIO_CFG_IO_LSB            16U

#define GPIO_CFG_OUT_TYPE_MASK     0xC0000000U
#define GPIO_CFG_OUT_TYPE_LSB      30U
#define GPIO_CFG_OUT_HIGH          (((uint32_t)1)<< GPIO_CFG_OUT_TYPE_LSB)

#define GPIO_CFG_INT_MASK          0x3F000000U
#define GPIO_CFG_INT_LSB           24U

/*! @endcond */

/** @name GPIO_PinConfig pin Input/Output configuration macros
 *  @{
 *
 * These macros define the Input/Output properties associated with the GPIO pin.
 */

/**
 * @brief   Pin is an output.
 */
#define GPIO_CFG_OUTPUT            (((uint32_t)1U) << GPIO_CFG_IO_LSB)

/**
 * @brief   Output pin is Open Drain
 */
#define GPIO_CFG_OUT_OD_NOPULL     (((uint32_t)2U) << GPIO_CFG_IO_LSB)

/**
 * @brief   Pin is an input.
 */
#define GPIO_CFG_INPUT             (((uint32_t)16U) << GPIO_CFG_IO_LSB)

/** @} */

/** @name GPIO_PinConfig Interrupt configuration macros
 *  @{
 *
 * These macros define the Interrupt properties associated with the GPIO pin.
 */

/**
 * @brief   No Interrupt
 */
#define GPIO_CFG_IN_INT_NONE       (((uint32_t)1) << GPIO_CFG_INT_LSB)

/**
 * @brief   Interrupt on falling edge
 */
#define GPIO_CFG_IN_INT_FALLING    (((uint32_t)2) << GPIO_CFG_INT_LSB)

/**
 * @brief   Interrupt on rising edge
 */
#define GPIO_CFG_IN_INT_RISING     (((uint32_t)4) << GPIO_CFG_INT_LSB)

/**
 * @brief   Interrupt on both edges
 */
#define GPIO_CFG_IN_INT_BOTH_EDGES (((uint32_t)8) << GPIO_CFG_INT_LSB)

/**
 * @brief   Low priority interrupt: Set to 1 for Low Priority Pin interrupt
 * else the Pin is automatically configured as a High Priority interrupt
 */
#define GPIO_CFG_IN_INT_LOW        (((uint32_t)16)<< GPIO_CFG_INT_LSB)

/** @} */

/*!
 *  @brief  GPIO callback function type
 *
 *  @param      index       GPIO index.  This is the same index that
 *                          was passed to GPIO_setCallback().  This allows
 *                          you to use the same callback function for multiple
 *                          GPIO interrupts, by using the index to identify
 *                          the GPIO that caused the interrupt.
 *
 *  \ingroup GPIO_DRIVER_EXTERNAL_DATA_STRUCTURE
 */
typedef void (*GPIO_CallbackFxn)(uint32_t index);

/*!
 *  @brief      Clear a GPIO pin interrupt flag
 *
 *  Clears the GPIO interrupt for the specified index.
 *
 *  Note: It is not necessary to call this API within a
 *  callback assigned to a pin.
 *
 *  @param      index       GPIO index
 *
 *  \ingroup GPIO_DRIVER_EXTERNAL_FUNCTION
 */
extern void GPIO_clearInt(uint32_t index);

/*!
 *  @brief      Disable a GPIO pin interrupt
 *
 *  Disables interrupts for the specified GPIO index.
 *
 *  @param      index       GPIO index
 *
 *  \ingroup GPIO_DRIVER_EXTERNAL_FUNCTION
 */
extern void GPIO_disableInt(uint32_t index);

/*!
 *  @brief      Enable a GPIO pin interrupt
 *
 *  Enables GPIO interrupts for the selected index to occur.
 *
 *  Note:  Prior to enabling a GPIO pin interrupt, make sure
 *  that a corresponding callback function has been provided.
 *  Use the GPIO_setCallback() API for this purpose at runtime.
 *  Alternatively, the callback function can be statically
 *  configured in the GPIO_CallbackFxn array provided.
 *
 *  @param      index       GPIO index
 *
 *  \ingroup GPIO_DRIVER_EXTERNAL_FUNCTION
 */
extern void GPIO_enableInt(uint32_t index);

/*!
 *  @brief      Get the current configuration for a gpio pin
 *
 *  @param      index       GPIO index
 *  @param      pinConfig   Location to store device specific pin
 *                          configuration settings
 *
 *  \ingroup GPIO_DRIVER_EXTERNAL_FUNCTION
 */
extern void GPIO_getConfig(uint32_t index, GPIO_PinConfig *pinConfig);

/*!
 *  @brief  Initializes the GPIO module
 *
 *  @pre    This function must also be called before any other GPIO driver APIs.
 *
 *  \ingroup GPIO_DRIVER_EXTERNAL_FUNCTION
 */
extern void GPIO_init(void);

/*!
 *  @brief      Reads the value of a GPIO pin
 *
 *  The value returned will either be zero or one depending on the
 *  state of the pin.
 *
 *  @param      index  GPIO index
 *
 *  @return     0 or 1, depending on the state of the pin.
 *
 *  \ingroup GPIO_DRIVER_EXTERNAL_FUNCTION
 */
extern uint32_t GPIO_read(uint32_t index);

/*!
 *  @brief      Bind a callback function to a GPIO pin interrupt
 *
 *  Associate a callback function with a particular GPIO pin interrupt.
 *
 *  Callbacks can be changed at any time, making it easy to switch between
 *  efficient, state-specific interrupt handlers.
 *
 *  Note: The callback function is called within the context of an interrupt
 *  handler.
 *
 *  Note: This API does not enable the GPIO pin interrupt.
 *  Use GPIO_enableInt() and GPIO_disableInt() to enable
 *  and disable the pin interrupt as necessary.
 *
 *  Note: it is not necessary to call GPIO_clearInt() within a callback.
 *  That operation is performed internally before the callback is invoked.
 *
 *  @param      index       GPIO index
 *  @param      callback    address of the callback function
 *
 *  \ingroup GPIO_DRIVER_EXTERNAL_FUNCTION
 */
extern void GPIO_setCallback(uint32_t index, GPIO_CallbackFxn callback);

/*!
 *  @brief      Configure the gpio pin
 *
 *  Dynamically configure a gpio pin to a device specific setting.
 *
 *  For input pins with interrupt configurations, a corresponding interrupt
 *  object will be created as needed.
 *
 *  @param      index       GPIO index
 *  @param      pinConfig   device specific pin configuration settings
 *
 *  \ingroup GPIO_DRIVER_EXTERNAL_FUNCTION
 */
extern void GPIO_setConfig(uint32_t index, GPIO_PinConfig pinConfig);

/*!
 *  @brief      Toggles the current state of a GPIO
 *
 *  @param      index  GPIO index
 *
 *  \ingroup GPIO_DRIVER_EXTERNAL_FUNCTION
 */
extern void GPIO_toggle(uint32_t index);

/*!
 *  @brief     Writes the value to a GPIO pin
 *
 *  @param      index    GPIO index
 *  @param      value    must be either 0 or 1
 *
 *  \ingroup GPIO_DRIVER_EXTERNAL_FUNCTION
 */
extern void GPIO_write(uint32_t index, uint32_t value);

/**
@}
*/

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_GPIO__include */
