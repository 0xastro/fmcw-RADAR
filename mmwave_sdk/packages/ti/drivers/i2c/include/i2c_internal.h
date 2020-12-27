/**
 *   @file  i2c_internal.h
 *
 *   @brief
 *      This is the internal Header for the I2C Driver. This header
 *      file should *NOT* be directly included by applications.
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

#ifndef I2C_INTERNAL_H
#define I2C_INTERNAL_H

#include <ti/drivers/osal/HwiP.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/osal/SemaphoreP.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/i2c/include/reg_i2c.h>

#define CSL_I2C_ICMDR_TRX_MASK          (0x00000200U)
#define CSL_I2C_ICMDR_MST_MASK          (0x00000400u)
#define CSL_I2C_ICMDR_STP_MASK          (0x00000800U)
#define CSL_I2C_ICMDR_STT_MASK          (0x00002000U)
#define CSL_I2C_ICMDR_FREE_MASK         (0x00004000U)
#define CSL_I2C_ICMDR_RM_MASK           (0x00000080U)
#define CSL_I2C_ICMDR_DLB_MASK          (0x00000040U)
#define CSL_I2C_ICMDR_IRS_MASK          (0x00000020u)
#define I2C_CFG_MASK_TX                 (CSL_I2C_ICMDR_TRX_MASK)
#define I2C_CFG_MASK_RX                 (CSL_I2C_ICMDR_TRX_MASK)
#define I2C_CFG_MASK_STOP               (CSL_I2C_ICMDR_STP_MASK)
#define I2C_CFG_MASK_START              (CSL_I2C_ICMDR_STT_MASK)
#define I2C_CFG_MASK_RUN_FREE           (CSL_I2C_ICMDR_FREE_MASK)
#define I2C_CFG_MASK_REPEAT_MODE        (CSL_I2C_ICMDR_RM_MASK)
#define I2C_CFG_MASK_LOOP_BACK          (CSL_I2C_ICMDR_DLB_MASK)

#define I2C_CFG_CMD_TX                  (CSL_I2C_ICMDR_TRX_MASK)
#define I2C_CFG_CMD_RX                  (0U)
#define I2C_CFG_CMD_STOP                (CSL_I2C_ICMDR_STP_MASK)
#define I2C_CFG_CMD_START               (CSL_I2C_ICMDR_STT_MASK)
#define I2C_CFG_CMD_RUN_FREE_ON         (CSL_I2C_ICMDR_FREE_MASK)
#define I2C_CFG_CMD_RUN_FREE_OFF        (0U)
#define I2C_CFG_CMD_REPEAT_MODE_ON      (CSL_I2C_ICMDR_RM_MASK)
#define I2C_CFG_CMD_REPEAT_MODE_OFF     (0U)
#define I2C_CFG_CMD_LOOP_BACK_ON        (CSL_I2C_ICMDR_DLB_MASK)
#define I2C_CFG_CMD_LOOP_BACK_OFF       (0U)


/*
** Values that can be passed to I2CMasterIntStatusEx and I2CMasterIntClearEx
** APIs as int status flag to check and clear interrupt status
*/
#define I2C_INT_ARBITRATION_LOST        (0x00000001U)
#define I2C_INT_NO_ACK                  (0x00000002U)
#define I2C_INT_ADRR_READY_ACESS        (0x00000004U)
#define I2C_INT_RECV_READY              (0x00000008U)
#define I2C_INT_TRANSMIT_READY          (0x00000010U)
#define I2C_INT_STOP_CONDITION          (0x00000020U)
#define I2C_INT_ADRR_ZERO               (0x00000100U)
#define I2C_INT_ADRR_SLAVE              (0x00000200U)
#define I2C_INT_TRANSMIT_UNDER_FLOW     (0x00000400U)
#define I2C_INT_RECV_OVER_RUN           (0x00000800U)
#define I2C_INT_BUS_BUSY                (0x00001000U)
#define I2C_INT_NO_ACK_SENT             (0x00002000U)
#define I2C_INT_SLAVE_DIRECTION         (0x00004000U)

#define I2C_ALL_INTS                    (I2C_INT_ARBITRATION_LOST     | \
                                        I2C_INT_NO_ACK               | \
                                        I2C_INT_ADRR_READY_ACESS     | \
                                        I2C_INT_RECV_READY           | \
                                        I2C_INT_TRANSMIT_READY       | \
                                        I2C_INT_STOP_CONDITION       | \
                                        I2C_INT_ADRR_ZERO            | \
                                        I2C_INT_ADRR_SLAVE           | \
                                        I2C_INT_TRANSMIT_UNDER_FLOW  | \
                                        I2C_INT_RECV_OVER_RUN        | \
                                        I2C_INT_BUS_BUSY             | \
                                        I2C_INT_NO_ACK_SENT          | \
                                        I2C_INT_SLAVE_DIRECTION)

/*
** Values that can be passed to I2CMasterIntEnableEx and I2CMasterIntDisableEx
** APIs as intFlag to enable or disable interrupts.
*/
#define I2C_INT_MASK_ARBITRATION_LOST   (0x00000001U)
#define I2C_INT_MASK_NO_ACK             (0x00000002U)
#define I2C_INT_MASK_ADRR_READY_ACESS   (0x00000004U)
#define I2C_INT_MASK_RECV_READY         (0x00000008U)
#define I2C_INT_MASK_TRANSMIT_READY     (0x00000010U)
#define I2C_INT_MASK_STOP_CONDITION     (0x00000020U)
#define I2C_INT_MASK_ADRR_SLAVE         (0x00000040U)

#define I2C_ALL_INTS_MASK               (I2C_INT_MASK_ARBITRATION_LOST    | \
                                        I2C_INT_MASK_NO_ACK               | \
                                        I2C_INT_MASK_ADRR_READY_ACESS     | \
                                        I2C_INT_MASK_RECV_READY           | \
                                        I2C_INT_MASK_TRANSMIT_READY       | \
                                        I2C_INT_MASK_STOP_CONDITION       | \
                                        I2C_INT_MASK_ADRR_SLAVE)

/*
** I2C interrupt vector Code.
*/
#define I2C_IVR_INTCODE_MASK             (0x00000007U)
#define I2C_IVR_INTCODE_AL               (0x00000001U)
#define I2C_IVR_INTCODE_NACK             (0x00000002U)
#define I2C_IVR_INTCODE_ARDY             (0x00000003U)
#define I2C_IVR_INTCODE_RRDY             (0x00000004U)
#define I2C_IVR_INTCODE_XRDY             (0x00000005U)
#define I2C_IVR_INTCODE_SCD              (0x00000006U)
#define I2C_IVR_INTCODE_AAS              (0x00000007U)


/**
@defgroup I2C_INTERNAL_FUNCTION             I2C Driver Internal Functions
@ingroup I2C_DRIVER
*/
/**
@defgroup I2C_INTERNAL_DATA_STRUCTURE       I2C Driver Internal Data Structures
@ingroup I2C_DRIVER
*/

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup I2C_INTERNAL_DATA_STRUCTURE
 @{ */

/**
 * @brief
 *  I2C Driver Status
 *
 * @details
 *  The enumeration describes the status of the I2C Driver Instance
 */
typedef enum I2C_DriverState_t
{
    /**
     * @brief   Driver is uninitialized.
     */
    I2C_DriverState_UNINIT,

    /**
     * @brief   Driver is operational.
     */
    I2C_DriverState_OPERATIONAL
}I2C_DriverState;

/**
 * @brief
 *  I2C Driver HW configuration
 *
 * @details
 *  The structure is used to store the hardware specific configuration which is
 *  passed to each driver instance
 */
typedef struct I2C_HwCfg_t
{
    /**
     * @brief   Base address of the I2C address space to be used.
     */
    I2CRegs*            ptrI2cBase;

    /**
     * @brief   Interrupt Number
     */
    uint32_t            interruptNum;
}I2C_HwCfg;

extern void I2CSetPSC(I2CRegs* i2cRegs, uint32_t psc);
extern void I2CReset(I2CRegs* i2cRegs);
extern void I2CSetOwnAddress(I2CRegs* i2cRegs, uint32_t ownAddr);
extern void I2CIntEnable(I2CRegs* i2cRegs, uint32_t intFlag);
extern void I2CIntDisable(I2CRegs* i2cRegs, uint32_t intFlag);
extern uint32_t I2CGetIntStatus(I2CRegs* i2cRegs, uint32_t intFlag);
extern void I2CIntClear(I2CRegs* i2cRegs, uint32_t intFlag);
extern void I2CSetDataCount(I2CRegs* i2cRegs, uint32_t count);
extern uint32_t I2CGetDataCount(I2CRegs* i2cRegs);
extern void I2CPutData(I2CRegs* i2cRegs, uint8_t data);
extern uint8_t I2CGetData(I2CRegs* i2cRegs);
extern uint32_t I2CGetIntVector(I2CRegs* i2cRegs);
/**
@}
*/

#ifdef __cplusplus
}
#endif

#endif /* I2C_INTERNAL_H */
