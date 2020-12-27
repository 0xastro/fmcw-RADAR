/**
 *   @file  gpio.c
 *
 *   @brief
 *      The file implements the GPIO Driver.
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include <stdint.h>
#include <string.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/gpio/gpio.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/gpio/include/gpio_internal.h>
#include <ti/drivers/gpio/include/reg_gio.h>

/**************************************************************************
 ***************************** Local Structures ***************************
 **************************************************************************/

/**
 * @brief
 *  GPIO Hardware Attributes
 *
 * @details
 *  The structure contains the hardware atrributes which are used
 *  to specify the platform specific configurations.
 */
typedef struct GPIO_MCB_t
{
    /**
     * @brief   GPIO driver has been initialized
     */
    bool                isInitialized;

    /**
     * @brief   HWI Handle to the high priority interrupts
     */
    HwiP_Handle         hwiHandleHigh;

    /**
     * @brief   HWI Handle to the low priority interrupts
     */
    HwiP_Handle         hwiHandleLow;

    /**
     * @brief   Number of high priority interrupts detected
     */
    uint32_t            numHighPriorityInterrupts;

    /**
     * @brief   Number of low priority interrupts detected
     */
    uint32_t            numLowPriorityInterrupts;

    /**
     * @brief   Callback function table for the GPIO Ports and Pins.
     */
    GPIO_CallbackFxn    callbackFxnTable[GPIO_MAX_PORT][GPIO_MAX_PINS_PER_PORT];

    /**
     * @brief   Pin configuration
     */
    uint32_t            pinCfg[GPIO_MAX_PORT][GPIO_MAX_PINS_PER_PORT];
}GPIO_MCB;


/**************************************************************************
 ****************************** Local Functions ***************************
 **************************************************************************/

/* GPIO Register Functions: */
static void GPIO_ignorePolarity (uint8_t port, uint8_t pin);
static void GPIO_honorPolarity (uint8_t port, uint8_t pin);
static void GPIO_configPolarity (uint8_t port, uint8_t pin, uint8_t highLevel);
static void GPIO_enableInterrupt (uint8_t port, uint8_t pin);
static void GPIO_disableInterrupt (uint8_t port, uint8_t pin);
static void GPIO_clearInterrupt (uint8_t port, uint8_t pin);
static void GPIO_markHighLevelInterrupt (uint8_t port, uint8_t pin);
static void GPIO_markLowLevelInterrupt (uint8_t port, uint8_t pin);
static uint8_t GPIO_getHighLevelPendingInterrupt(void);
static uint8_t GPIO_getLowLevelPendingInterrupt(void);
static void GPIO_setOutputDataDirection (uint8_t port, uint8_t pin, uint8_t dataDirection);
static uint8_t GPIO_getData (uint8_t port, uint8_t pin);
static void GPIO_sendData (uint8_t port, uint8_t pin, uint8_t value);
static void GPIO_setOpenDrainStatus (uint8_t port, uint8_t pin, uint8_t openDrainStatus);

/* GPIO ISR: */
static void GPIO_HighPriorityISR (uintptr_t arg);
static void GPIO_LowPriorityISR (uintptr_t arg);
static void GPIO_processInterrupt (uint8_t pendingInterrupt);

/**************************************************************************
 **************************** Global Variables ****************************
 **************************************************************************/

/**
 * @brief
 *  Global GPIO MCB which tracks the GPIO driver related information.
 */
GPIO_MCB    gGPIOMCB;

/**************************************************************************
 ************************** GPIO Driver Functions *************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      The function is used to configure the GPIO Port/Pin to ignore the
 *      polarity.
 *
 *  @param[in]  port
 *      GPIO Port Number to be configured
 *  @param[in]  pin
 *      GPIO Pin number associated with the port
 *
 *  \ingroup GPIO_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void GPIO_ignorePolarity (uint8_t port, uint8_t pin)
{
    gGPIOHwAtrrib.ptrGPIORegs->GIOINTDET =
            CSL_FINSR (gGPIOHwAtrrib.ptrGPIORegs->GIOINTDET,
                       (port * 8U) + pin,
                       (port * 8U) + pin,
                       1U);
}

/**
 *  @b Description
 *  @n
 *      The function is used to configure the GPIO Port/Pin to honor the
 *      polarity.
 *
 *  @param[in]  port
 *      GPIO Port Number to be configured
 *  @param[in]  pin
 *      GPIO Pin number associated with the port
 *
 *  \ingroup GPIO_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void GPIO_honorPolarity (uint8_t port, uint8_t pin)
{
    gGPIOHwAtrrib.ptrGPIORegs->GIOINTDET =
            CSL_FINSR (gGPIOHwAtrrib.ptrGPIORegs->GIOINTDET,
                       (port * 8U) + pin,
                       (port * 8U) + pin,
                       0U);
}

/**
 *  @b Description
 *  @n
 *      The function is used to configure the GPIO Port/Pin to control
 *      the interrupt polarity
 *
 *  @param[in]  port
 *      GPIO Port Number to be configured
 *  @param[in]  pin
 *      GPIO Pin number associated with the port
 *  @param[in]  highLevel
 *      Set to 1 to indicate that the interrupt be triggered on rising edge
 *      Set to 0 to indicate that the interrupt be triggered on falling edge
 *
 *  \ingroup GPIO_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void GPIO_configPolarity (uint8_t port, uint8_t pin, uint8_t highLevel)
{
    gGPIOHwAtrrib.ptrGPIORegs->GIOPOL =
            CSL_FINSR (gGPIOHwAtrrib.ptrGPIORegs->GIOPOL,
                       (port * 8U) + pin,
                       (port * 8U) + pin,
                       (uint32_t)highLevel);
}

/**
 *  @b Description
 *  @n
 *      The function is used to enable the interrupt on the GPIO Port/Pin
 *
 *  @param[in]  port
 *      GPIO Port Number to be configured
 *  @param[in]  pin
 *      GPIO Pin number associated with the port
 *
 *  \ingroup GPIO_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void GPIO_enableInterrupt (uint8_t port, uint8_t pin)
{
    gGPIOHwAtrrib.ptrGPIORegs->GIOENASET =
            CSL_FINSR (gGPIOHwAtrrib.ptrGPIORegs->GIOENASET,
                       (port * 8U) + pin,
                       (port * 8U) + pin,
                       1U);
}

/**
 *  @b Description
 *  @n
 *      The function is used to disable the interrupt on the GPIO Port/Pin
 *
 *  @param[in]  port
 *      GPIO Port Number to be configured
 *  @param[in]  pin
 *      GPIO Pin number associated with the port
 *
 *  \ingroup GPIO_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void GPIO_disableInterrupt (uint8_t port, uint8_t pin)
{
    gGPIOHwAtrrib.ptrGPIORegs->GIOENACLR =
            CSL_FINSR (gGPIOHwAtrrib.ptrGPIORegs->GIOENACLR,
                       (port * 8U) + pin,
                       (port * 8U) + pin,
                       1U);
}

/**
 *  @b Description
 *  @n
 *      The function is used to clear the interrupt on the GPIO Port/Pin
 *
 *  @param[in]  port
 *      GPIO Port Number to be configured
 *  @param[in]  pin
 *      GPIO Pin number associated with the port
 *
 *  \ingroup GPIO_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void GPIO_clearInterrupt (uint8_t port, uint8_t pin)
{
    gGPIOHwAtrrib.ptrGPIORegs->GIOFLG =
            CSL_FINSR (gGPIOHwAtrrib.ptrGPIORegs->GIOFLG,
                       (port * 8U) + pin,
                       (port * 8U) + pin,
                       1U);
}

/**
 *  @b Description
 *  @n
 *      The function is used to mark the interrupt as high level for the
 *      specific GPIO Port/Pin
 *
 *  @param[in]  port
 *      GPIO Port Number to be configured
 *  @param[in]  pin
 *      GPIO Pin number associated with the port
 *
 *  \ingroup GPIO_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void GPIO_markHighLevelInterrupt (uint8_t port, uint8_t pin)
{
    gGPIOHwAtrrib.ptrGPIORegs->GIOLVLSET =
            CSL_FINSR (gGPIOHwAtrrib.ptrGPIORegs->GIOLVLSET,
                       (port * 8U) + pin,
                       (port * 8U) + pin,
                       1U);
}

/**
 *  @b Description
 *  @n
 *      The function is used to mark the interrupt as low level for the
 *      specific GPIO Port/Pin
 *
 *  @param[in]  port
 *      GPIO Port Number to be configured
 *  @param[in]  pin
 *      GPIO Pin number associated with the port
 *
 *  \ingroup GPIO_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void GPIO_markLowLevelInterrupt (uint8_t port, uint8_t pin)
{
    gGPIOHwAtrrib.ptrGPIORegs->GIOLVLCLR =
            CSL_FINSR (gGPIOHwAtrrib.ptrGPIORegs->GIOLVLCLR,
                       (port * 8U) + pin,
                       (port * 8U) + pin,
                       1U);
}

/**
 *  @b Description
 *  @n
 *      The function is used to get the high level interrupt pending.
 *      The driver will need to cycle through all the pending interrupts
 *      before the processing of the interrupt is over
 *
 *  \ingroup GPIO_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      0        - No pending interrupt
 *  @retval
 *      Non Zero - Highest Pending Interrupt
 */
static uint8_t GPIO_getHighLevelPendingInterrupt(void)
{
    return gGPIOHwAtrrib.ptrGPIORegs->GIOOFFA;
}

/**
 *  @b Description
 *  @n
 *      The function is used to get the low level interrupt pending.
 *      The driver will need to cycle through all the pending interrupts
 *      before the processing of the interrupt is over
 *
 *  \ingroup GPIO_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      0        - No pending interrupt
 *  @retval
 *      Non Zero - Lowest Pending Interrupt
 */
static uint8_t GPIO_getLowLevelPendingInterrupt(void)
{
    return gGPIOHwAtrrib.ptrGPIORegs->GIOOFFB;
}

/**
 *  @b Description
 *  @n
 *      The function is used to set the output data direction
 *      associated with a GPIO Port/Pin.
 *
 *  @param[in]  port
 *      GPIO Port Number to be configured
 *  @param[in]  pin
 *      GPIO Pin number associated with the port
 *  @param[in]  dataDirection
 *      Set to 1 to enable the output data direction
 *      Set to 0 to disable the output data direction
 *
 *  \ingroup GPIO_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void GPIO_setOutputDataDirection (uint8_t port, uint8_t pin, uint8_t dataDirection)
{
    gGPIOHwAtrrib.ptrGPIORegs->portRegs[port].GIODIR =
            CSL_FINSR (gGPIOHwAtrrib.ptrGPIORegs->portRegs[port].GIODIR,
                       pin,
                       pin,
                       (uint32_t)dataDirection);
}

/**
 *  @b Description
 *  @n
 *      The function is used to get the data input for a specific GPIO
 *      Port/Pin
 *
 *  @param[in]  port
 *      GPIO Port Number
 *  @param[in]  pin
 *      GPIO Pin number associated with the port
 *
 *  \ingroup GPIO_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      0  - Logic Low
 *  @retval
 *      1  - Logic High
 */
static uint8_t GPIO_getData (uint8_t port, uint8_t pin)
{
    return (uint8_t)CSL_FEXTR (gGPIOHwAtrrib.ptrGPIORegs->portRegs[port].GIODIN, pin, pin);
}

/**
 *  @b Description
 *  @n
 *      The function is used to send the data to a specific GPIO Port/Pin
 *
 *  @param[in]  port
 *      GPIO Port Number
 *  @param[in]  pin
 *      GPIO Pin number associated with the port
 *  @param[in]  value
 *      Bit Value to write
 *
 *  \ingroup GPIO_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void GPIO_sendData (uint8_t port, uint8_t pin, uint8_t value)
{
    if (value == 1U)
    {
        gGPIOHwAtrrib.ptrGPIORegs->portRegs[port].GIODSET =
                CSL_FMKR (pin, pin, 1U);
    }
    else
    {
        gGPIOHwAtrrib.ptrGPIORegs->portRegs[port].GIODCLR =
                CSL_FMKR (pin, pin, 1U);
    }
}

/**
 *  @b Description
 *  @n
 *      The function is used to enable/disable open drain functionality
 *      for a specific GPIO Port/Pin
 *
 *  @param[in]  port
 *      GPIO Port Number
 *  @param[in]  pin
 *      GPIO Pin number associated with the port
 *  @param[in]  openDrainStatus
 *      Set to 1 to enable open drain functionality
 *      Set to 0 to disable open drain functionality
 *
 *  \ingroup GPIO_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void GPIO_setOpenDrainStatus (uint8_t port, uint8_t pin, uint8_t openDrainStatus)
{
    gGPIOHwAtrrib.ptrGPIORegs->portRegs[port].GIOPDR =
                CSL_FMKR (pin, pin, (uint32_t)openDrainStatus);
}

/**
 *  @b Description
 *  @n
 *      The function process the pending interrupt.
 *
 *  @param[in]  pendingInterrupt
 *      Pending Interupt to be processed.
 *
 *  \ingroup GPIO_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void GPIO_processInterrupt (uint8_t pendingInterrupt)
{
    uint8_t     port;
    uint8_t     pin;

    /* Decode the index:
     *  The Table mapping as per the documentation is as follows:-
     *      Interrupt 0  -> No Interrupt
     *      Interrupt 1  -> Port 0 Pin 0
     *      Interrupt 2  -> Port 0 Pin 1
     *      ...
     * NOTE: We have already verified that there there is a pending interrupt */
    pendingInterrupt = pendingInterrupt - 1U;
    GPIO_DECODE_INDEX (pendingInterrupt, &port, &pin);

    /* Invoke the callback function if one was registered */
    if (gGPIOMCB.callbackFxnTable[port][pin] != NULL)
    {
        gGPIOMCB.callbackFxnTable[port][pin](pendingInterrupt);
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered ISR which handles the high
 *      priority GPIO Interrupts
 *
 *  @param[in]  arg
 *      Not used
 *
 *  \ingroup GPIO_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void GPIO_HighPriorityISR (uintptr_t arg)
{
    uint8_t     pendingInterrupt;

    /* We need to process all the interrupts which are pending */
    while (1)
    {
        /* Get the high priority pending GPIO interrupt: */
        pendingInterrupt = GPIO_getHighLevelPendingInterrupt();

        /* Have we processed all the GPIO interrupts? */
        if (pendingInterrupt == 0U)
        {
            /* YES: There are no more pending interrupts. */
            break;
        }

        /* Increment the statistics: */
        gGPIOMCB.numHighPriorityInterrupts++;

        /* Process the pending interrupt: */
        GPIO_processInterrupt (pendingInterrupt);
    }
}

/**
 *  @b Description
 *  @n
 *      The function is the registered ISR which handles the low
 *      priority GPIO Interrupts
 *
 *  @param[in]  arg
 *      Not used
 *
 *  \ingroup GPIO_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void GPIO_LowPriorityISR (uintptr_t arg)
{
    uint8_t     pendingInterrupt;

    /* We need to process all the interrupts which are pending */
    while (1)
    {
        /* Get the low priority pending GPIO interrupt: */
        pendingInterrupt = GPIO_getLowLevelPendingInterrupt();

        /* Have we processed all the GPIO interrupts? */
        if (pendingInterrupt == 0U)
        {
            /* YES: There are no more pending interrupts. */
            break;
        }

        /* Increment the statistics: */
        gGPIOMCB.numLowPriorityInterrupts++;

        /* Process the pending interrupt: */
        GPIO_processInterrupt (pendingInterrupt);
    }
}

/*
 *  ======== GPIO_init ========
 */
void GPIO_init(void)
{
    uint8_t         portIndex;
    HwiP_Params     hwiParams;

    /* Initialize the GPIO Driver: */
    memset ((void *)&gGPIOMCB, 0, sizeof(GPIO_MCB));

    /* Register the High Prirority interrupts: */
    HwiP_Params_init(&hwiParams);
    hwiParams.name = "GPIO_High";
    gGPIOMCB.hwiHandleHigh = HwiP_create(gGPIOHwAtrrib.highInterruptNum, GPIO_HighPriorityISR, &hwiParams);

    /* Register the Low Priority Interrupt: */
    HwiP_Params_init(&hwiParams);
    hwiParams.name = "GPIO_Low";
    gGPIOMCB.hwiHandleLow  = HwiP_create(gGPIOHwAtrrib.lowInterruptNum, GPIO_LowPriorityISR, &hwiParams);

    /* Ensure that the GPIO is operating normally */
    gGPIOHwAtrrib.ptrGPIORegs->GIOGCR = CSL_FMKR (GIOGCR_RESET_BIT_END, GIOGCR_RESET_BIT_START, 1U);

    /* Disable the interrupts for all pins */
    gGPIOHwAtrrib.ptrGPIORegs->GIOENACLR = 0xFFFFFFFFU;

    /* Cycle through all the GPIO Ports: */
    for (portIndex = 0U; portIndex < GPIO_MAX_PORT; portIndex++)
    {
        /* Disable the output buffer for all the pins: */
        gGPIOHwAtrrib.ptrGPIORegs->portRegs[portIndex].GIODIR =
                CSL_FMKR (GIODIRA_GIODIRA_BIT_END, GIODIRA_GIODIRA_BIT_START, 0U);

        /* No data is to be sent out for all the pins: */
        gGPIOHwAtrrib.ptrGPIORegs->portRegs[portIndex].GIODOUT =
                CSL_FMKR (GIODOUTA_GIODOUTA_BIT_END, GIODOUTA_GIODOUTA_BIT_START, 0U);

        /* Disable open drain capability for all the pins:  */
        gGPIOHwAtrrib.ptrGPIORegs->portRegs[portIndex].GIOPDR =
                CSL_FMKR (GIOPDRA_GIOPDRA_BIT_END, GIOPDRA_GIOPDRA_BIT_START, 0U);
    }

    /* Driver has been initialized */
    gGPIOMCB.isInitialized = true;
    return;
}

/*
 *  ======== GPIO_clearInt ========
 */
void GPIO_clearInt(uint32_t index)
{
    uint8_t     port;
    uint8_t     pin;

    /* Assert if the driver is still uninitialized. */
    DebugP_assert (gGPIOMCB.isInitialized);

    /* Decode the index into the GPIO Port & Pin */
    GPIO_DECODE_INDEX ((uint8_t)index, &port, &pin);

    /* System Limits: Assert if the configuration is invalid. */
    DebugP_assert (port < GPIO_MAX_INTERRUPT_PORT);
    DebugP_assert (pin  < GPIO_MAX_PINS_PER_PORT);

    /* Clear the interrupt: */
    GPIO_clearInterrupt (port, pin);
    return;
}

/*
 *  ======== GPIO_disableInt ========
 */
void GPIO_disableInt(uint32_t index)
{
    uint8_t     port;
    uint8_t     pin;

    /* Assert if the driver is still uninitialized. */
    DebugP_assert (gGPIOMCB.isInitialized);

    /* Decode the index into the GPIO Port & Pin */
    GPIO_DECODE_INDEX ((uint8_t)index, &port, &pin);

    /* System Limits: Assert if the configuration is invalid. */
    DebugP_assert (port < GPIO_MAX_INTERRUPT_PORT);
    DebugP_assert (pin  < GPIO_MAX_PINS_PER_PORT);

    /* Disable the interrupt: */
    GPIO_disableInterrupt (port, pin);
    return;
}

/*
 *  ======== GPIO_enableInt ========
 */
void GPIO_enableInt(uint32_t index)
{
    uint8_t     port;
    uint8_t     pin;

    /* Assert if the driver is still uninitialized. */
    DebugP_assert (gGPIOMCB.isInitialized);

    /* Decode the index into the GPIO Port & Pin */
    GPIO_DECODE_INDEX ((uint8_t)index, &port, &pin);

    /* System Limits: Assert if the configuration is invalid. */
    DebugP_assert (port < GPIO_MAX_INTERRUPT_PORT);
    DebugP_assert (pin  < GPIO_MAX_PINS_PER_PORT);

    /* Enable the interrupt: */
    GPIO_enableInterrupt (port, pin);
    return;
}

/*
 *  ======== GPIO_getConfig ========
 */
void GPIO_getConfig(uint32_t index, GPIO_PinConfig *pinConfig)
{
    uint8_t     port;
    uint8_t     pin;

    /* Assert if the driver is still uninitialized. */
    DebugP_assert (gGPIOMCB.isInitialized);

    /* Decode the index into the GPIO Port & Pin */
    GPIO_DECODE_INDEX ((uint8_t)index, &port, &pin);

    /* System Limits: Assert if the configuration is invalid. */
    DebugP_assert (port < GPIO_MAX_PORT);
    DebugP_assert (pin  < GPIO_MAX_PINS_PER_PORT);

    /* Copy over the pin configuration */
    *pinConfig = gGPIOMCB.pinCfg [port][pin];
    return;
}

/*
 *  ======== GPIO_read ========
 */
uint32_t GPIO_read(uint32_t index)
{
    uint8_t     port;
    uint8_t     pin;

    /* Assert if the driver is still uninitialized. */
    DebugP_assert (gGPIOMCB.isInitialized);

    /* Decode the index into the GPIO Port & Pin */
    GPIO_DECODE_INDEX ((uint8_t)index, &port, &pin);

    /* System Limits: Assert if the configuration is invalid. */
    DebugP_assert (port < GPIO_MAX_PORT);
    DebugP_assert (pin  < GPIO_MAX_PINS_PER_PORT);

    return (uint32_t)GPIO_getData(port, pin);
}

/*
 *  ======== GPIO_setCallback ========
 */
void GPIO_setCallback(uint32_t index, GPIO_CallbackFxn callback)
{
    uint8_t     port;
    uint8_t     pin;

    /* Assert if the driver is still uninitialized. */
    DebugP_assert (gGPIOMCB.isInitialized);

    /* Decode the index into the GPIO Port & Pin */
    GPIO_DECODE_INDEX ((uint8_t)index, &port, &pin);

    /* System Limits: Assert if the configuration is invalid. */
    DebugP_assert (port < GPIO_MAX_PORT);
    DebugP_assert (pin  < GPIO_MAX_PINS_PER_PORT);

    /* Update the callback function table */
    gGPIOMCB.callbackFxnTable[port][pin] = callback;
    return;
}

/*
 *  ======== GPIO_setConfig ========
 */
void GPIO_setConfig(uint32_t index, GPIO_PinConfig pinConfig)
{
    uint8_t     port;
    uint8_t     pin;
    uintptr_t   key;

    /* Assert if the driver is still uninitialized. */
    DebugP_assert (gGPIOMCB.isInitialized);

    /* Decode the index into the GPIO Port & Pin */
    GPIO_DECODE_INDEX ((uint8_t)index, &port, &pin);

    /* System Limits: Assert if the configuration is invalid. */
    DebugP_assert (port < GPIO_MAX_PORT);
    DebugP_assert (pin  < GPIO_MAX_PINS_PER_PORT);

    /* Disable preemption while configuring the GPIO */
    key = HwiP_disable();

    /* Input or output? */
    if (pinConfig & GPIO_CFG_INPUT)
    {
        /* Input: Disable the output data direction */
        GPIO_setOutputDataDirection (port, pin, 0U);
    }
    else
    {
        /* Output: Set the data direction */
        GPIO_setOutputDataDirection (port, pin, 1U);

        /* Is the GPIO Port/Pin open open drain? */
        if (pinConfig & GPIO_CFG_OUT_OD_NOPULL)
        {
            /* YES: Open Drain */
            GPIO_setOpenDrainStatus (port, pin, 1U);
        }
    }

    /* Do we need to enable interrupts for this? */
    if (pinConfig & GPIO_CFG_INT_MASK)
    {
        /* YES: As per the documentation of the IP only the first 4 ports are capable of handling
         * interrupts. */
        DebugP_assert (port < GPIO_MAX_INTERRUPT_PORT);

        /* YES: Do we need to configure the interrupt on the falling edge? */
        if (pinConfig & GPIO_CFG_IN_INT_FALLING)
        {
            /* YES: Honor the polarity for the falling edge */
            GPIO_honorPolarity (port, pin);
            GPIO_configPolarity (port, pin, 0U);
        }

        /* Do we need to configure the interrupt on the rising edge? */
        if (pinConfig & GPIO_CFG_IN_INT_RISING)
        {
            /* YES: Honor the polarity for the rising edge */
            GPIO_honorPolarity (port, pin);
            GPIO_configPolarity (port, pin, 1U);
        }

        /* Do we need to handle interrupts on both edges? */
        if (pinConfig & GPIO_CFG_IN_INT_BOTH_EDGES)
        {
            /* YES: Ignore the polarity since interrupts can be triggered on either edge */
            GPIO_ignorePolarity (port, pin);
        }

        /* Is the interrupt a high or low level? */
        if (pinConfig & GPIO_CFG_IN_INT_LOW)
        {
            /* YES: Mark this as a low level interrupt */
            GPIO_markLowLevelInterrupt (port, pin);
        }
        else
        {
            /* YES: Mark this as a high level interrupt */
            GPIO_markHighLevelInterrupt (port, pin);
        }
    }

    /* Store the PORT/PIN configuration: */
    gGPIOMCB.pinCfg [port][pin] = pinConfig;

    /* Restore the interrupts */
    HwiP_restore(key);
    return;
}

/*
 *  ======== GPIO_toggle ========
 */
void GPIO_toggle(uint32_t index)
{
    uint8_t     port;
    uint8_t     pin;

    /* Assert if the driver is still uninitialized. */
    DebugP_assert (gGPIOMCB.isInitialized);

    /* Decode the index into the GPIO Port & Pin */
    GPIO_DECODE_INDEX ((uint8_t)index, &port, &pin);

    /* System Limits: Assert if the configuration is invalid. */
    DebugP_assert (port < GPIO_MAX_PORT);
    DebugP_assert (pin  < GPIO_MAX_PINS_PER_PORT);

    /* Is the value on the pin currently 1 */
    if (gGPIOMCB.pinCfg [port][pin] & GPIO_CFG_OUT_HIGH)
    {
        /* YES: Write a 0 */
        GPIO_write (index, 0U);
    }
    else
    {
        /* NO: Write a 1 */
        GPIO_write (index, 1U);
    }
    return;
}

/*
 *  ======== GPIO_write ========
 */
void GPIO_write(uint32_t index, uint32_t value)
{
    uint8_t     port;
    uint8_t     pin;

    /* Assert if the driver is still uninitialized. */
    DebugP_assert (gGPIOMCB.isInitialized);

    /* Decode the index into the GPIO Port & Pin */
    GPIO_DECODE_INDEX ((uint8_t)index, &port, &pin);

    /* System Limits: Assert if the configuration is invalid. */
    DebugP_assert (port < GPIO_MAX_PORT);
    DebugP_assert (pin  < GPIO_MAX_PINS_PER_PORT);

    /* We can only send data if the Port/Pin was configured correctly */
    DebugP_assert ((gGPIOMCB.pinCfg [port][pin] & GPIO_CFG_OUTPUT) == GPIO_CFG_OUTPUT);

    /* Setup the pin configuration to track the last value written */
    if (value == 1U)
    {
        gGPIOMCB.pinCfg [port][pin] = (gGPIOMCB.pinCfg [port][pin] | GPIO_CFG_OUT_HIGH);
    }
    else
    {
        gGPIOMCB.pinCfg [port][pin] = (gGPIOMCB.pinCfg [port][pin] & ~GPIO_CFG_OUT_HIGH);
    }

    /* Send out the data */
    GPIO_sendData (port, pin, (uint8_t)value);
}

