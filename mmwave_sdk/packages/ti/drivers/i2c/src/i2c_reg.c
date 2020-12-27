/**
 *   @file  i2c_reg.c
 *
 *   @@brief
 *      The file implements the I2C register access APIs.
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ti/drivers/i2c/include/i2c_internal.h>



/**************************************************************************
 ************************* I2C Register access Functions ******************
 **************************************************************************/
void I2CSetPSC(I2CRegs* i2cRegs, uint32_t psc);
void I2CMasterInitClk(I2CRegs* i2cRegs, uint32_t sysClk,
                         uint32_t internalClk, uint32_t outputClk);
void I2CMasterEnable(I2CRegs* i2cRegs);
void I2CSlaveEnable(I2CRegs* i2cRegs);
void I2CReset(I2CRegs* i2cRegs);
uint32_t I2CGetIntStatus(I2CRegs* i2cRegs, uint32_t intFlag);
bool I2CMasterBusBusy(I2CRegs* i2cRegs);
uint32_t I2CMasterErr(I2CRegs* i2cRegs);
void I2CMasterControl(I2CRegs* i2cRegs, uint32_t ctrlMask, uint32_t ctrlCmds);
void I2CMasterStart(I2CRegs* i2cRegs);
void I2CMasterStop(I2CRegs* i2cRegs);
void I2CIntEnable(I2CRegs* i2cRegs, uint32_t intFlag);
void I2CIntDisable(I2CRegs* i2cRegs, uint32_t intFlag);
void I2CIntClear(I2CRegs* i2cRegs, uint32_t intFlag);
void I2CMasterSetSlaveAddr(I2CRegs* i2cRegs, uint32_t slaveAdd);
void I2CSetDataCount(I2CRegs* i2cRegs, uint32_t count);
uint32_t I2CGetDataCount(I2CRegs* i2cRegs);
void I2CPutData(I2CRegs* i2cRegs, uint8_t data);
uint8_t I2CGetData(I2CRegs* i2cRegs);
void I2CSetOwnAddress(I2CRegs* i2cRegs, uint32_t ownAddr);
uint32_t I2CGetIntVector(I2CRegs* i2cRegs);




/**
 * @brief   This function configures the prescaler value.
 *
 * @param[in]   i2cRegs
 *      It is the Memory address of the I2C instance used.
 * @param[in]   psc
 *      Prescaler value to set
 *
 *  \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void I2CSetPSC(I2CRegs* i2cRegs, uint32_t psc)
{
    /* Set the prescalar value */
    i2cRegs->ICPSC = psc;
}

/**
 * @brief   This function determines and configures the module and master clock frequency.
 *
 * @param[in]   i2cRegs
 *      Memory address of the I2C instance used.
 * @param[in]   sysClk
 *      System clock fed to I2C module.
 * @param[in]   internalClk
 *      Internal clock used by I2C module.Which is obtained by scaling System clock fed to I2C module.
 * @param[in]   outputClk
 *      Required I2C bus speed or frequency.
 *
 *  \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void I2CMasterInitClk(I2CRegs* i2cRegs, uint32_t sysClk,
                         uint32_t internalClk, uint32_t outputClk)
{
    uint32_t prescalar;
    uint32_t divider;

    /* Set the prescalar value */
    prescalar = sysClk / internalClk - 1U;

    I2CSetPSC(i2cRegs, prescalar);

    /* Set the CLKL and CLKH values */
    divider = internalClk / outputClk;
    divider = divider / 2U;
    i2cRegs->ICCLKL = divider - 6U;
    i2cRegs->ICCLKH = divider - 6U;
}

/**
 * @brief   Enables the I2C module. This will bring the I2C module out of reset.
 *
 * @param[in]   i2cRegs
 *      Memory address of the I2C instance used.
 *
 *  \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void I2CMasterEnable(I2CRegs* i2cRegs)
{
    /* Enable master mode */
    i2cRegs->ICMDR = CSL_I2C_ICMDR_MST_MASK | CSL_I2C_ICMDR_FREE_MASK;

    /* Bring I2C out of reset */
    i2cRegs->ICMDR |= CSL_I2C_ICMDR_IRS_MASK;
}


/**
 * @brief   Enables the I2C module in slave mode.
 *          This will bring the I2C module out of reset.
 *
 * @param[in]   i2cRegs
 *      Memory address of the I2C instance used.
 *
 *  \ingroup I2CSLAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void I2CSlaveEnable(I2CRegs* i2cRegs)
{
    /* Enable free mode */
    i2cRegs->ICMDR = CSL_I2C_ICMDR_FREE_MASK;

    /* Bring I2C out of reset */
    i2cRegs->ICMDR |= CSL_I2C_ICMDR_IRS_MASK;
}

/**
 * @brief   Disables the I2C Module. This will put the I2C module in reset.
 *          Only Tx and Rx are cleared, status bits are set their default
 *          values and all configuration registers are not reset, they keep
 *          their initial values.
 *
 * @param[in]   i2cRegs
 *      Memory address of the I2C instance used.
 *
 * \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void I2CReset(I2CRegs* i2cRegs)
{
    i2cRegs->ICMDR &= ~CSL_I2C_ICMDR_IRS_MASK;
}

/**
 * @brief  This function returns the status of specified interrupts in
 *         master mode.
 *
 * @param[in]  i2cRegs
 *      Memory address of the I2C instance used.
 * @param[in]  intFlag
 *      It specifies the interrupts whose status needs to be
 *                  returned.
 *
 *         intFlag can take following macros.
 *
 *         I2C_INT_ARBITRATION_LOST     - Arbitration-lost interrupt.
 *         I2C_INT_NO_ACK               - No-acknowledgement interrupt.
 *         I2C_INT_ADRR_READY_ACESS     - I2C registers are ready to access.
 *         I2C_INT_RECV_READY           - Receive-data-ready interrupt.
 *         I2C_INT_TRANSMIT_READY       - Transmit-data-ready interrupt.
 *         I2C_INT_STOP_CONDITION       - Stop condition interrupt.
 *         I2C_INT_ADRR_ZERO            - Address of all zeros interrupt.
 *         I2C_INT_ADRR_SLAVE           - Address-as-slave interrupt.
 *         I2C_INT_TRANSMIT_UNDER_FLOW  - Transmit underflow interrupt.
 *         I2C_INT_RECV_OVER_RUN        - Receive overrun interrupt.
 *         I2C_INT_BUS_BUSY             - Bus busy Interrupt.
 *         I2C_INT_NO_ACK_SENT          - No acknowlegement sent interrupt.
 *         I2C_INT_SLAVE_DIRECTION      - Slave direction interrupt.
 *
 *  \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Status of specified interrupts.
 */
uint32_t I2CGetIntStatus(I2CRegs* i2cRegs, uint32_t intFlag)
{
    return (i2cRegs->ICSTR & intFlag);
}

/**
 * @brief   This function determines whether bus is busy or not.
 *
 * @param[in]   i2cRegs
 *      It is the Memory address of the I2C instance used.
 *
 * \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      true  - bus is busy.
 *  @retval
 *      false -  bus is free.
 */
bool I2CMasterBusBusy(I2CRegs* i2cRegs)
{
    bool ret_val = false;
    if(I2CGetIntStatus(i2cRegs, I2C_INT_BUS_BUSY))
    {
        ret_val = (bool)true;
    }
    else
    {
        ret_val = (bool)false;
    }
    return ret_val;
}


/**
 * @brief   This function determines whether error occurred or not during
 *          I2C operation.
 *
 * @param[in]   i2cRegs
 *      Memory address of the I2C instance used.
 *
 * \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      error -  non zero value.
 *  @retval
 *      No error - 0.
 */
uint32_t I2CMasterErr(I2CRegs* i2cRegs)
{
    uint32_t errMask;

    errMask = I2C_INT_ARBITRATION_LOST    |
              I2C_INT_NO_ACK              |
              I2C_INT_STOP_CONDITION      |
              I2C_INT_ADRR_ZERO           |
              I2C_INT_RECV_OVER_RUN;

    return (I2CGetIntStatus(i2cRegs, errMask));
}

/**
 * @brief   This function configure I2C in different modes of operation.
 *
 * @param[in]   i2cRegs
 *      Memory address of the I2C instance used.
 * @param[in]   ctrlMask
 *      Mask of the control bits to configure I2C in different operating modes.
 * @param[in]   ctrlCmds
 *      Control bits which configures I2C in different operating modes.
 *
 * \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void I2CMasterControl(I2CRegs* i2cRegs, uint32_t ctrlMask, uint32_t ctrlCmds)
{
    uint32_t i2cMdr = i2cRegs->ICMDR;

    i2cMdr &= ~ctrlMask;
    i2cMdr |= (ctrlCmds | CSL_I2C_ICMDR_MST_MASK);
    i2cRegs->ICMDR = i2cMdr;
}

/**
 * @brief   This function starts a I2C transaction on the bus. This API must
 *          be called after all the configuration for the i2c module is
 *          done and after bringing I2C out of local reset
 *
 * @param[in]   i2cRegs
 *      Memory address of the I2C instance used.
 *
 * \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void I2CMasterStart(I2CRegs* i2cRegs)
{
    i2cRegs->ICMDR |= (CSL_I2C_ICMDR_MST_MASK | CSL_I2C_ICMDR_STT_MASK);
}

/**
 * @brief  This function stops a I2C transaction on the bus.
 *         This function must be used in case a deliberate STOP needs to be sent
 *         on the bus.
 *
 * @param[in]  i2cRegs
 *      Memory address of the I2C instance used.
 *
 * \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void I2CMasterStop(I2CRegs* i2cRegs)
{
    i2cRegs->ICMDR |= (CSL_I2C_ICMDR_MST_MASK | CSL_I2C_ICMDR_STP_MASK);
}

/**
 * @brief  This function enables only specified I2C interrupts in master mode.
 *
 * @param[in]  i2cRegs
 *      Memory address of the I2C instance used.
 * @param[in]  intFlag
 *      Specifies the interrupts that are to be enabled.
 *
 *         intFlag can take following values.
 *
 *         I2C_INT_ARBITRATION_LOST     - Arbitration-lost interrupt.
 *         I2C_INT_NO_ACK               - No-acknowledgement interrupt.
 *         I2C_INT_ADRR_READY_ACESS     - I2C registers are ready to access.
 *         I2C_INT_RECV_READY           - Receive-data-ready interrupt.
 *         I2C_INT_TRANSMIT_READY       - Transmit-data-ready interrupt.
 *         I2C_INT_STOP_CONDITION       - Stop condition interrupt.
 *         I2C_INT_ADRR_SLAVE           - Address-as-slave interrupt.
 *
 * \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void I2CIntEnable(I2CRegs* i2cRegs, uint32_t intFlag)
{
    i2cRegs->ICIMR |= intFlag;
}


/**
 * @brief  This function disables the specified I2C interrupts.
 *
 * @param[in]  i2cRegs
 *      Memory address of the I2C instance used.
 * @param[in]  intFlag
 *      Specifies the interrupts that are to be disabled.
 *
 *         intFlag can take following values.
 *
 *         I2C_INT_ARBITRATION_LOST     - Arbitration-lost interrupt.
 *         I2C_INT_NO_ACK               - No-acknowledgement interrupt.
 *         I2C_INT_ADRR_READY_ACESS     - I2C registers are ready to access.
 *         I2C_INT_RECV_READY           - Receive-data-ready interrupt.
 *         I2C_INT_TRANSMIT_READY       - Transmit-data-ready interrupt.
 *         I2C_INT_STOP_CONDITION       - Stop condition interrupt.
 *         I2C_INT_ADRR_SLAVE           - Address-as-slave interrupt.
 *
 *  \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void I2CIntDisable(I2CRegs* i2cRegs, uint32_t intFlag)
{
    i2cRegs->ICIMR &= ~intFlag;
}

/**
 * @brief  This function clears the status of specified interrupts.
 *
 * @param[in]  i2cRegs
 *      Memory address of the I2C instance used.
 * @param[in]  intFlag
 *      Specifies the interrupts whose status needs to be cleared
 *
 *         intFlag can take following macros.
 *
 *         I2C_INT_ARBITRATION_LOST     - Arbitration-lost interrupt.
 *         I2C_INT_NO_ACK               - No-acknowledgement interrupt.
 *         I2C_INT_ADRR_READY_ACESS     - I2C registers are ready to access.
 *         I2C_INT_RECV_READY           - Receive-data-ready interrupt.
 *         I2C_INT_TRANSMIT_READY       - Transmit-data-ready interrupt.
 *         I2C_INT_STOP_CONDITION       - Stop condition interrupt.
 *         I2C_INT_ADRR_ZERO            - Address of all zeros interrupt.
 *         I2C_INT_ADRR_SLAVE           - Address-as-slave interrupt.
 *         I2C_INT_TRANSMIT_UNDER_FLOW  - Transmit underflow interrupt.
 *         I2C_INT_RECV_OVER_RUN        - Receive overrun interrupt.
 *         I2C_INT_BUS_BUSY             - Bus busy Interrupt.
 *         I2C_INT_NO_ACK_SENT          - No acknowlegement sent interrupt.
 *         I2C_INT_SLAVE_DIRECTION      - Slave direction interrupt.
 *
 *  \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void I2CIntClear(I2CRegs* i2cRegs, uint32_t intFlag)
{
    i2cRegs->ICSTR = intFlag;
}


/**
 * @brief   This function sets the address of the slave device with which I2C wants
 *          to communicate.
 *
 * @param[in]   i2cRegs
 *      Memory address of the I2C instance used.
 * @param[in]   slaveAdd
 *      Slave address.
 *
 *  \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void I2CMasterSetSlaveAddr(I2CRegs* i2cRegs, uint32_t slaveAdd)
{
    i2cRegs->ICSAR = slaveAdd;
}


/**
 * @brief   This function configure I2C data count register with a value.
 *          The value in the I2C data count register indicate how many data
 *          words to transfer when the I2C is configured as a master-transmitter
 *          and repeat mode is off.
 *
 * @param[in]   i2cRegs
 *      Memory address of the I2C instance used.
 * @param[in]   count
 *      Value which is set to I2C data count register.
 *
 *  \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void I2CSetDataCount(I2CRegs* i2cRegs, uint32_t count)
{
    i2cRegs->ICCNT = count;
}

/**
 * @brief   This function gets the current value from the I2C data count register with a value.
 *          The value in the I2C data count register indicate how many data
 *          words to transfer when the I2C is configured as a master-transmitter
 *          and repeat mode is off.
 *
 * @param[in]   i2cRegs
 *      Memory address of the I2C instance used.
 *
 *  \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Returns the number of bytes transferred over the I2C bus.
 */
uint32_t I2CGetDataCount(I2CRegs* i2cRegs)
{
    return(i2cRegs->ICCNT);
}

/**
 * @brief  This function transmits a byte from the I2C.
 *
 * @param[in]  i2cRegs
 *      Memory address of the I2C instance used.
 * @param[in]  data
 *      Data to be transmitted from the I2C Master.
 *
 *  \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void I2CPutData(I2CRegs* i2cRegs, uint8_t data)
{
    i2cRegs->ICDXR = data;
}

/**
 *  @brief  This fuunction receives a byte.
 *
 *  @param[in]  i2cRegs
 *      Memory address of the I2C instance used.
 *
 *  \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Returns the byte received.
 */
uint8_t I2CGetData(I2CRegs* i2cRegs)
{
    return (uint8_t)(i2cRegs->ICDRR);
}

/**
 *  @brief  Set the own slave address
 *
 *  @param[in]  i2cRegs
 *          Memory address of the I2C instance used.
 *  @param[in]  ownAddr
 *          Slave address of the I2C instance.
 *
 *  \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 *
 */
void I2CSetOwnAddress(I2CRegs* i2cRegs, uint32_t ownAddr)
{
    i2cRegs->ICOAR = ownAddr;
}

/**
 *  @brief  Get the interrupt vector code.
 *
 *  @param[in]  i2cRegs
 *      Memory address of the I2C instance used.
 *
 *  \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Returns the interrupt vector register value.
 */
uint32_t I2CGetIntVector(I2CRegs* i2cRegs)
{
    return ((i2cRegs->ICIVR) & I2C_IVR_INTCODE_MASK);
}

