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
/*
 *  ======== pinmux.c ========
 */
/**
    @defgroup PINMUX_DRIVER_INTERNAL_FUNCTION            PINMUX Driver Internal Functions
    @ingroup PINMUX_DRIVER
    @brief
 *   The section has a list of all the internal functions used by the driver
 */
/**
    @defgroup PINMUX_DRIVER_INTERNAL_DATA_STRUCTURE      PINMUX Driver Internal Data Structures
    @ingroup PINMUX_DRIVER
    @brief
 *   The section has a list of all the data structures which are used internally by the driver
 */

#include <stdint.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/pinmux/pinmux.h>
#include <ti/drivers/pinmux/include/reg_pinmux.h>
#include <ti/drivers/osal/HwiP.h>

#define PINMUX_PARAM_CHECK


static int32_t Pinmux_Unlock(void);
static int32_t Pinmux_Lock(void);

extern volatile PINMUXRegs *gPinmuxRegs;


/** @addtogroup PINMUX_DRIVER_INTERNAL_FUNCTION
 @{ */

/* INTERNAL FUNCTIONS */

/** @fn int32_t Pinmux_Unlock(void)
*   @brief Configure the key to enable the pinmuxing.
*/
static int32_t Pinmux_Unlock(void)
{
    /* Enable Pin Muxing */    
    gPinmuxRegs->IOCFGKICK0 = 0x83E70B13U;
    gPinmuxRegs->IOCFGKICK1 = 0x95A4F1E0U;
    return 0;
}

/** @fn int32_t Pinmux_Lock(void)
*   @brief Lock to disable the pinmuxing.
*
*/
static int32_t Pinmux_Lock(void)
{
    /* Disable Pin Muxing */    
    gPinmuxRegs->IOCFGKICK1 = 0x0U;
    gPinmuxRegs->IOCFGKICK0 = 0x0U;
    return 0;
}
/** @}*/

/* END INTERNAL FUNCTIONS */


/** @fn int32_t Pinmux_Set_OverrideCtrl(uint32_t pin, Pinmux_Output_Override_e outputCtrl, Pinmux_Input_Override_e inputCtrl)
*   @brief Configurations for the input and output override control.
*
*    @param[in] pin: Pin number to be configured
*    @param[in] outputCtrl: Output control selection
*               The parameter \e outputCtrl can be one of the following values:
*                - \b PINMUX_OUTPUT_EN
*                - \b PINMUX_OUTPUT_DIS
*                - \b PINMUX_OUTEN_RETAIN_HW_CTRL
*    @param[in] inputCtrl: Input control selection
*               The parameter \e inputCtrl can be one of the following values:
*                - \b PINMUX_INPUT_EN
*                - \b PINMUX_INPUT_DIS
*                - \b PINMUX_INPEN_RETAIN_HW_CTRL
*
*    @return Value < 0 - in case of error
*            Value = 0 - success
*/
int32_t Pinmux_Set_OverrideCtrl(uint32_t pin, Pinmux_Output_Override_e outputCtrl, Pinmux_Input_Override_e inputCtrl)
{
    int32_t retVal;
    uintptr_t           key;

#ifdef PINMUX_PARAM_CHECK
    /* check if pinmux address is valid */
    if (gPinmuxRegs==NULL)
    {
        retVal = PINMUX_EINVALIDADDRESS;
    }
    /* Check the validity of the pin number to be configured */
    else if (pin > PINMUX_NUM_PADS) 
    {
        /* Error in the pin number */
        retVal = PINMUX_EINVAL;
    }
    else
    {
#endif
        /* Disable preemption while unlocking,writing and locking the pinmux register. */
        key = HwiP_disable();
        
        /* enable access for write */
        Pinmux_Unlock();
        
        /* Setup the output control of the pin */
        if(PINMUX_OUTEN_RETAIN_HW_CTRL != outputCtrl)
        {
            gPinmuxRegs->PADxx_CFG_REG[pin] = CSL_FINSR (gPinmuxRegs->PADxx_CFG_REG[pin], 
                                                         PADS_CFG_REG_OE_OVERRIDE_BIT_START, 
                                                         PADS_CFG_REG_OE_OVERRIDE_BIT_END, (uint32_t)outputCtrl);
            gPinmuxRegs->PADxx_CFG_REG[pin] = CSL_FINSR (gPinmuxRegs->PADxx_CFG_REG[pin], 
                                                         PADS_CFG_REG_OE_OVERRIDE_CTRL_BIT_START, 
                                                         PADS_CFG_REG_OE_OVERRIDE_CTRL_BIT_END, 1U);
        }
        else    /* OUTEN_RETAIN_HW_CTRL */
        {
            gPinmuxRegs->PADxx_CFG_REG[pin] = CSL_FINSR (gPinmuxRegs->PADxx_CFG_REG[pin], 
                                                         PADS_CFG_REG_OE_OVERRIDE_CTRL_BIT_START, 
                                                         PADS_CFG_REG_OE_OVERRIDE_CTRL_BIT_END, 0U);
        }

        /* Setup the input control of the pin */
        if(PINMUX_INPEN_RETAIN_HW_CTRL != inputCtrl)
        {
            gPinmuxRegs->PADxx_CFG_REG[pin] = CSL_FINSR (gPinmuxRegs->PADxx_CFG_REG[pin], 
                                                         PADS_CFG_REG_IE_OVERRIDE_BIT_START, 
                                                         PADS_CFG_REG_IE_OVERRIDE_BIT_END, (uint32_t)inputCtrl);
            gPinmuxRegs->PADxx_CFG_REG[pin] = CSL_FINSR (gPinmuxRegs->PADxx_CFG_REG[pin], 
                                                         PADS_CFG_REG_IE_OVERRIDE_CTRL_BIT_START, 
                                                         PADS_CFG_REG_IE_OVERRIDE_CTRL_BIT_END, 1U);
        }
        /* INPEN_RETAIN_HW_CTRL */
        else
        {
            gPinmuxRegs->PADxx_CFG_REG[pin] = CSL_FINSR (gPinmuxRegs->PADxx_CFG_REG[pin], 
                                                         PADS_CFG_REG_IE_OVERRIDE_CTRL_BIT_START, 
                                                         PADS_CFG_REG_IE_OVERRIDE_CTRL_BIT_END, 0U);
        }
        
        /* dsiable write access */
        Pinmux_Lock();
        
        /* Restore the interrupts: */
        HwiP_restore(key);
        
        retVal = 0;
#ifdef PINMUX_PARAM_CHECK
    }
#endif
    return retVal;
}

/** @fn int32_t Pinmux_Set_Pull(uint32_t pin, Pinmux_Pull_e pullVal)
*   @brief Configurations for the pulls on the pins.
*
*    @param[in] pin: Pin number to be configured
*    @param[in] pullVal: Pull selection
*               The parameter \e pullVal can be one of the following values:
*                - \b PINMUX_PULLUP_EN
*                - \b PINMUX_PULLDOWN_EN
*                - \b PINMUX_DISABLE_PULLS
*
*    @return Value < 0 - in case of error
*            Value = 0 - success
*/
int32_t Pinmux_Set_Pull(uint32_t pin, Pinmux_Pull_e pullVal)
{
    int32_t retVal;
    uintptr_t           key;

#ifdef PINMUX_PARAM_CHECK
    /* check if pinmux address is valid */
    if (gPinmuxRegs==NULL)
    {
        retVal = PINMUX_EINVALIDADDRESS;
    }
    /* Check the validity of the pin number to be configured */
    else if (pin > PINMUX_NUM_PADS) 
    {
        /* Error in the pin number */
        retVal = PINMUX_EINVAL;
    }
    else
#endif
    {
        /* Disable preemption while unlocking,writing and locking the pinmux register. */
        key = HwiP_disable();

        /* enable access for write */
        Pinmux_Unlock();

        /* Setup the configured pull control of the pin */
        gPinmuxRegs->PADxx_CFG_REG[pin] = CSL_FINSR (gPinmuxRegs->PADxx_CFG_REG[pin], 
                                                     PADS_CFG_REG_PI_BIT_START, 
                                                     PADS_CFG_REG_PUPDSEL_BIT_END, pullVal);

        /* dsiable write access */
        Pinmux_Lock();
        
        /* Restore the interrupts: */
        HwiP_restore(key);
        
        retVal = 0;
    }
    return retVal;
}

/** @fn int32_t Pinmux_Set_SlewRate(uint32_t pin, Pinmux_Slew_e slewVal)
*   @brief Configurations for the slew on the pins.
*
*    @param[in] pin: Pin number to be configured
*    @param[in] slewVal: Slew selection
*               The parameter \e slewVal can be one of the following values:
*                - \b PINMUX_SLEW_LOW
*                - \b PINMUX_SLEW_HIGH
*
*    @return Value < 0 - in case of error
*            Value = 0 - success
*/
int32_t Pinmux_Set_SlewRate(uint32_t pin, Pinmux_SlewRate_e slewVal)

{
    int32_t retVal;
    uintptr_t           key;

#ifdef PINMUX_PARAM_CHECK
    /* check if pinmux address is valid */
    if (gPinmuxRegs==NULL)
    {
        retVal = PINMUX_EINVALIDADDRESS;
    }
    /* Check the validity of the pin number to be configured */
    else if (pin > PINMUX_NUM_PADS) 
    {
        /* Error in the pin number */
        retVal = PINMUX_EINVAL;
    }
    else
#endif
    {
        /* Disable preemption while unlocking,writing and locking the pinmux register. */
        key = HwiP_disable();

        /* enable access for write */
        Pinmux_Unlock();
        
        /* Setup the configured slew of the pin */        
        gPinmuxRegs->PADxx_CFG_REG[pin] = CSL_FINSR (gPinmuxRegs->PADxx_CFG_REG[pin], 
                                                     PADS_CFG_REG_SC1_BIT_START, 
                                                     PADS_CFG_REG_SC1_BIT_END, slewVal);

        /* dsiable write access */
        Pinmux_Lock();
        
        /* Restore the interrupts: */
        HwiP_restore(key);
        
        retVal = 0;
    }
    return retVal;
}

/** @fn int32_t Pinmux_Set_FuncSel(uint32_t pin, uint32_t func)
*   @brief Setup the pin functionality.
*
*   This function sets up the functional personality of the pin.
*   The specified signal will be brought out on the pin.
*   As there is no strict check on the parameters passed, the values passed
*   must be from the macro specified in the header pinmux.h.
*
*    @param[in] pin: Pin number to be configured
*    @param[in] func: Desired signal functionality to be muxed onto the pin
*/
int32_t Pinmux_Set_FuncSel(uint32_t pin, uint32_t func)
{
    int32_t             retVal;
    uintptr_t           key;

#ifdef PINMUX_PARAM_CHECK
    /* check if pinmux address is valid */
    if (gPinmuxRegs==NULL)
    {
        retVal = PINMUX_EINVALIDADDRESS;
    }
    /* Check the validity of the pin number to be configured */
    else if (pin > PINMUX_NUM_PADS) 
    {
        /* Error in the pin number */
        retVal = PINMUX_EINVAL;
    }
    else
#endif
    {
        /* Disable preemption while unlocking,writing and locking the pinmux register. */
        key = HwiP_disable();

        /* enable access for write */
        Pinmux_Unlock();
        
        /* Mask the existing functionality of the pin */
        gPinmuxRegs->PADxx_CFG_REG[pin] = CSL_FINSR (gPinmuxRegs->PADxx_CFG_REG[pin], 
                                                     PADS_CFG_REG_FUNC_SEL_BIT_END, 
                                                     PADS_CFG_REG_FUNC_SEL_BIT_START, 0);

        /* Setup the required fucntionality */
        gPinmuxRegs->PADxx_CFG_REG[pin] = CSL_FINSR (gPinmuxRegs->PADxx_CFG_REG[pin], 
                                                     PADS_CFG_REG_FUNC_SEL_BIT_END, 
                                                     PADS_CFG_REG_FUNC_SEL_BIT_START, func);

        /* dsiable write access */
        Pinmux_Lock();
        
        /* Restore the interrupts: */
        HwiP_restore(key);

        retVal = 0;
        
    }
    return retVal;

}


/** @fn int32_t Pinmux_Get_OverrideCtrl(uint32_t pin,Pinmux_Output_Override_e *outputCtrl, Pinmux_Input_Override_e *inputCtrl )
*   @brief Get the pin override control for input and output.
*
*   This function gets the configured override of the pin.
*
*    @param[in]  pin: Pin number to be queried
*    @param[out] outputCtrl: value read from the OE_OVERRIDE and 
*                     OE_OVERRIDE_CTRL bits of a given valid pin (Pinmux_Output_Override_e) 
*    @param[out] inputCtrl: value read from the IE_OVERRIDE and 
*                     IE_OVERRIDE_CTRL bits of a given valid pin (Pinmux_Input_Override_e) 
*
*    @return    Value < 0 - in case of error; outputCtrl and inputCtrl have invalid values
*               Value = 0 - success; outputCtrl and inputCtrl have valid values
*
*/
int32_t Pinmux_Get_OverrideCtrl(uint32_t pin,Pinmux_Output_Override_e *outputCtrl, Pinmux_Input_Override_e *inputCtrl)
{
    
    int32_t retVal;
    
#ifdef PINMUX_PARAM_CHECK
    /* check if pinmux address is valid */
    if (gPinmuxRegs==NULL)
    {
        retVal = PINMUX_EINVALIDADDRESS;
    }
    /* Check the validity of the pin number to be configured */
    else if ((pin > PINMUX_NUM_PADS) || (outputCtrl == NULL) || (inputCtrl == NULL))
    {
        /* Error in the pin number */
        retVal = PINMUX_EINVAL;
    }
    else
#endif
    {
        /* read ctrl bits first */
        uint32_t oe_ctrl = CSL_FEXTR (gPinmuxRegs->PADxx_CFG_REG[pin], 
                                      PADS_CFG_REG_OE_OVERRIDE_CTRL_BIT_START, 
                                      PADS_CFG_REG_OE_OVERRIDE_CTRL_BIT_END);
        uint32_t ie_ctrl = CSL_FEXTR (gPinmuxRegs->PADxx_CFG_REG[pin], 
                                      PADS_CFG_REG_IE_OVERRIDE_CTRL_BIT_START, 
                                      PADS_CFG_REG_IE_OVERRIDE_CTRL_BIT_END);

        if (oe_ctrl == 1U)
        {
            /* if ctrl enabled, then read override bits */
            *outputCtrl = (Pinmux_Output_Override_e)CSL_FEXTR (gPinmuxRegs->PADxx_CFG_REG[pin], 
                                                               PADS_CFG_REG_OE_OVERRIDE_BIT_START, 
                                                               PADS_CFG_REG_OE_OVERRIDE_BIT_END);
        }
        else
        {
            *outputCtrl = PINMUX_OUTEN_RETAIN_HW_CTRL;
        }
        
        if (ie_ctrl == 1U)
        {
            /* if ctrl enabled, then read override bits */
            *inputCtrl = (Pinmux_Input_Override_e)CSL_FEXTR (gPinmuxRegs->PADxx_CFG_REG[pin], 
                                                             PADS_CFG_REG_IE_OVERRIDE_BIT_START, 
                                                             PADS_CFG_REG_IE_OVERRIDE_BIT_END);
        }
        else
        {
            *inputCtrl = PINMUX_INPEN_RETAIN_HW_CTRL;
        }
        
        retVal = 0;
        
    }
    return retVal;
}


/** @fn int32_t Pinmux_Get_Pull(uint32_t pin)
*   @brief Get the pin pull up/down setting.
*
*   This function gets the configured Pull up/down of the pin.
*
*    @param[in] pin: Pin number to be queried
*
*    @return    Value < 0 - in case of error 
*               Value > 0 - value read from the Pi and pupdsel bits of a given valid pin (Pinmux_Pull_e)
*
*/
int32_t Pinmux_Get_Pull(uint32_t pin)
{
    
    int32_t retVal;

#ifdef PINMUX_PARAM_CHECK
    /* check if pinmux address is valid */
    if (gPinmuxRegs==NULL)
    {
        retVal = PINMUX_EINVALIDADDRESS;
    }
    /* Check the validity of the pin number to be configured */
    else if (pin > PINMUX_NUM_PADS) 
    {
        /* Error in the pin number */
        retVal = PINMUX_EINVAL;
    }
    else
#endif
    {
        retVal = (int32_t)CSL_FEXTR (gPinmuxRegs->PADxx_CFG_REG[pin], 
                                     PADS_CFG_REG_PI_BIT_START, 
                                     PADS_CFG_REG_PUPDSEL_BIT_END);
    }
    return retVal;
}


/** @fn int32_t Pinmux_Get_SlewRate(uint32_t pin)
*   @brief Get the pin slew rate.
*
*   This function gets the configured functionality of the pin.
*
*    @param[in] pin: Pin number to be queried
*
*    @return    Value < 0 - in case of error 
*               Value > 0 - value read from the SC1 bits of a given valid pin (Pinmux_SlewRate_e)
*
*/
int32_t Pinmux_Get_SlewRate(uint32_t pin)
{
    
    int32_t retVal;
    
#ifdef PINMUX_PARAM_CHECK
    /* check if pinmux address is valid */
    if (gPinmuxRegs==NULL)
    {
        retVal = PINMUX_EINVALIDADDRESS;
    }
    /* Check the validity of the pin number to be configured */
    else if (pin > PINMUX_NUM_PADS) 
    {
        /* Error in the pin number */
        retVal = PINMUX_EINVAL;
    }
    else
#endif
    {
        retVal = (int32_t)CSL_FEXTR (gPinmuxRegs->PADxx_CFG_REG[pin], 
                                     PADS_CFG_REG_SC1_BIT_START, PADS_CFG_REG_SC1_BIT_END);
    }
    return retVal;
}


/** @fn int32_t Pinmux_Get_FuncSel(uint32_t pin)
*   @brief Get the pin functionality.
*
*   This function gets the configured functionality of the pin.
*
*    @param[in] pin: Pin number to be configured
*
*    @return    Value < 0 - in case of error 
*               Value > 0 - value read from the Func_Sel bits of a given valid pin
*
*/
int32_t Pinmux_Get_FuncSel(uint32_t pin)
{
    
    int32_t retVal;
    
#ifdef PINMUX_PARAM_CHECK
    /* check if pinmux address is valid */
    if (gPinmuxRegs==NULL)
    {
        retVal = PINMUX_EINVALIDADDRESS;
    }
    /* Check the validity of the pin number to be configured */
    else if (pin > PINMUX_NUM_PADS) 
    {
        /* Error in the pin number */
        retVal = PINMUX_EINVAL;
    }
    else
#endif
    {
        retVal = (int32_t)CSL_FEXTR (gPinmuxRegs->PADxx_CFG_REG[pin], 
                                     PADS_CFG_REG_FUNC_SEL_BIT_END, PADS_CFG_REG_FUNC_SEL_BIT_START);
    }
    return retVal;
}


