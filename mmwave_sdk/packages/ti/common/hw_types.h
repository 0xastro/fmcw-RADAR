/**
 * \file  hw_types.h
 *
 * \brief This file contains the in-line functions required to read/write
 *        values from/to the hardware registers. This file also contains field
 *        manipulation macros to get and set field values.
 *
 *  \copyright Copyright (C) 2013 Texas Instruments Incorporated -
 *             http://www.ti.com/
 */

/*
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
 *
 */


#ifndef HW_TYPES_H_
#define HW_TYPES_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 *  \brief   This macro reads a 32-bit value from a hardware register
 *           and returns the value.
 *
 *  \param   addr    Address of the memory mapped hardware register.
 *
 *  \return  Unsigned 32-bit value read from a register.
 */
#define HW_RD_REG32(addr) (HW_RD_REG32_RAW((uint32_t) (addr)))

/**
 *  \brief   This macro writes a 32-bit value to a hardware register.
 *
 *  \param   addr    Address of the memory mapped hardware register.
 *  \param   value   unsigned 32-bit value which has to be written to the
 *                   register.
 */
#define HW_WR_REG32(addr, value)                                               \
    (HW_WR_REG32_RAW((uint32_t) (addr), (uint32_t) (value)))

/**
 *  \brief   This macro reads a 16-bit value from a hardware register
 *           and returns the value.
 *
 *  \param   addr    Address of the memory mapped hardware register.
 *
 *  \return  Unsigned 16-bit value read from a register.
 */
#define HW_RD_REG16(addr) (HW_RD_REG16_RAW((uint32_t) (addr)))

/**
 *  \brief   This macro writes a 16-bit value to a hardware register.
 *
 *  \param   addr    Address of the memory mapped hardware register.
 *  \param   value   unsigned 16-bit value which has to be written to the
 *                   register.
 */
#define HW_WR_REG16(addr, value)                                               \
    (HW_WR_REG16_RAW((uint32_t) (addr), (uint16_t) (value)))

/**
 *  \brief   This macro reads a 8-bit value from a hardware register
 *           and returns the value.
 *
 *  \param   addr    Address of the memory mapped hardware register.
 *
 *  \return  Unsigned 8-bit value read from a register.
 */
#define HW_RD_REG8(addr) (HW_RD_REG8_RAW((uint32_t) (addr)))

/**
 *  \brief   This macro writes a 8-bit value to a hardware
 *           register.
 *
 *  \param   addr    Address of the memory mapped hardware register.
 *  \param   value   unsigned 8-bit value which has to be written to the
 *                   register.
 */
#define HW_WR_REG8(addr, value)                                                \
    (HW_WR_REG8_RAW((uint32_t) (addr), (uint8_t) (value)))

/**
 *  \brief Macro to extract a field value. This macro extracts the field value
 *         from a 32-bit variable (which contains the register value).
 *         This macro does not read from actual register address, and only
 *         extracts the field from a variable.
 *
 *  \param regVal         32-bit variable containing the register value.
 *  \param REG_FIELD      Peripheral register bit field name, whose value has to
 *                        be extracted.
 */
#define HW_GET_FIELD(regVal, REG_FIELD)                                        \
    (((regVal) & (uint32_t) REG_FIELD##_MASK) >> (uint32_t) REG_FIELD##_SHIFT)

/**
 *  \brief Macro to write a specific field value. This macro first clears the
 *         specified field value and then performs "OR" of the field value which
 *         is shifted and masked. This will set the field value at its
 *         desired position.
 *
 *  \param regVal         32-bit variable containing the register value.
 *  \param REG_FIELD      Peripheral register bit field name, to which specified
 *                        value has to be set.
 *  \param fieldVal       Value of the field which has to be set.
 */
#define HW_SET_FIELD32(regVal, REG_FIELD, fieldVal)                            \
    ((regVal) = ((regVal) & (uint32_t) (~(uint32_t) REG_FIELD##_MASK)) |       \
                    ((((uint32_t) (fieldVal)) << (uint32_t) REG_FIELD##_SHIFT) \
                        & (uint32_t) REG_FIELD##_MASK))

/**
 *  \brief Macro to write a specific field value. This macro first clears the
 *         specified field value and then performs "OR" of the field value which
 *         is shifted and masked. This will set the field value at its
 *         desired position.
 *
 *  \param regVal         16-bit variable containing the register value.
 *  \param REG_FIELD      Peripheral register bit field name, to which specified
 *                        value has to be set.
 *  \param fieldVal       Value of the field which has to be set.
 */
#define HW_SET_FIELD16(regVal, REG_FIELD, fieldVal)                            \
    ((regVal) = ((regVal) & (uint16_t) (~(uint16_t) REG_FIELD##_MASK)) |       \
                    ((((uint16_t) (fieldVal)) << (uint16_t) REG_FIELD##_SHIFT) \
                        & (uint16_t) REG_FIELD##_MASK))

/**
 *  \brief Macro to write a specific field value. This macro first clears the
 *         specified field value and then performs "OR" of the field value which
 *         is shifted and masked. This will set the field value at its
 *         desired position.
 *
 *  \param regVal         8-bit variable containing the register value.
 *  \param REG_FIELD      Peripheral register bit field name, to which specified
 *                        value has to be set.
 *  \param fieldVal       Value of the field which has to be set.
 */
#define HW_SET_FIELD8(regVal, REG_FIELD, fieldVal)                           \
    ((regVal) = ((regVal) & (uint8_t) (~(uint8_t) REG_FIELD##_MASK)) |       \
                    ((((uint8_t) (fieldVal)) << (uint8_t) REG_FIELD##_SHIFT) \
                        & (uint8_t) REG_FIELD##_MASK))

/**
 *  \brief This macro calls read-modify-write API for 32 bit register. It also
 *         frames the mask and shift from register field macro.
 *
 *  \param regAddr        Register Address.
 *  \param REG_FIELD      Peripheral register bit field name, to which specified
 *                        value has to be set.
 *  \param fieldVal       Value of the field which has to be set.
 */
#define HW_WR_FIELD32(regAddr, REG_FIELD, fieldVal)                            \
    (HW_WR_FIELD32_RAW((uint32_t) (regAddr), ((uint32_t)REG_FIELD##_MASK),                   \
                          ((uint32_t)REG_FIELD##_SHIFT), (uint32_t)(fieldVal)))

/**
 *  \brief This macro calls read-modify-write API for 16 bit register. It also
 *         frames the mask and shift from register field macro.
 *
 *  \param regAddr        Register Address.
 *  \param REG_FIELD      Peripheral register bit field name, to which specified
 *                        value has to be set.
 *  \param fieldVal       Value of the field which has to be set.
 */
#define HW_WR_FIELD16(regAddr, REG_FIELD, fieldVal)                            \
    (HW_WR_FIELD16_RAW((uint32_t) (regAddr), ((uint16_t) REG_FIELD##_MASK),                   \
                          ((uint32_t) REG_FIELD##_SHIFT), (uint16_t)(fieldVal)))

/**
 *  \brief This macro calls read-modify-write API for 8 bit register. It also
 *         frames the mask and shift from register field macro.
 *
 *  \param regAddr        Register Address.
 *  \param REG_FIELD      Peripheral register bit field name, to which specified
 *                        value has to be set.
 *  \param fieldVal       Value of the field which has to be set.
 */
#define HW_WR_FIELD8(regAddr, REG_FIELD, fieldVal)                             \
    (HW_WR_FIELD8_RAW((uint32_t) (regAddr), ((uint8_t) REG_FIELD##_MASK),                     \
                         ((uint32_t) REG_FIELD##_SHIFT), (uint8_t)(fieldVal)))

/**
 *  \brief This macro calls read field API for 32 bit register. It also
 *         frames the mask and shift from register field macro.
 *
 *  \param regAddr        Register Address.
 *  \param REG_FIELD      Peripheral register bit field name, from which
 *                        specified bit-field value has to be read.
 *  \return Value of the bit-field
 */
#define HW_RD_FIELD32(regAddr, REG_FIELD)                                      \
    (HW_RD_FIELD32_RAW((uint32_t) (regAddr), ((uint32_t) REG_FIELD##_MASK),                   \
                          ((uint32_t) REG_FIELD##_SHIFT)))

/**
 *  \brief This macro calls read field API for 16 bit register. It also
 *         frames the mask and shift from register field macro.
 *
 *  \param regAddr        Register Address.
 *  \param REG_FIELD      Peripheral register bit field name, from which
 *                        specified bit-field value has to be read.
 *  \return Value of the bit-field
 */
#define HW_RD_FIELD16(regAddr, REG_FIELD)                                      \
    (HW_RD_FIELD16_RAW((uint32_t) (regAddr), ((uint16_t) REG_FIELD##_MASK),                   \
                          ((uint32_t) REG_FIELD##_SHIFT)))

/**
 *  \brief This macro calls read field API for 8 bit register. It also
 *         frames the mask and shift from register field macro.
 *
 *  \param regAddr        Register Address.
 *  \param REG_FIELD      Peripheral register bit field name, from which
 *                        specified bit-field value has to be read.
 *  \return Value of the bit-field
 */
#define HW_RD_FIELD8(regAddr, REG_FIELD)                                       \
    (HW_RD_FIELD8_RAW((uint32_t) (regAddr), ((uint8_t) REG_FIELD##_MASK),                     \
                         ((uint32_t) REG_FIELD##_SHIFT)))

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                       Global Variables Declarations                        */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief   This function reads a 32-bit value from a hardware register
 *           and returns the value.
 *
 *  \param   addr    Address of the memory mapped hardware register.
 *
 *  \return  Unsigned 32-bit value read from a register.
 */
static inline uint32_t HW_RD_REG32_RAW(uint32_t addr);

/**
 *  \brief   This function writes a 32-bit value to a hardware register.
 *
 *  \param   addr    Address of the memory mapped hardware register.
 *  \param   value   unsigned 32-bit value which has to be written to the
 *                   register.
 */
static inline void HW_WR_REG32_RAW(uint32_t addr, uint32_t value);

/**
 *  \brief   This function reads a 16-bit value from a hardware register
 *           and returns the value.
 *
 *  \param   addr    Address of the memory mapped hardware register.
 *
 *  \return  Unsigned 16-bit value read from a register.
 */
static inline uint16_t HW_RD_REG16_RAW(uint32_t addr);

/**
 *  \brief   This function writes a 16-bit value to a hardware register.
 *
 *  \param   addr    Address of the memory mapped hardware register.
 *  \param   value   unsigned 16-bit value which has to be written to the
 *                   register.
 */
static inline void HW_WR_REG16_RAW(uint32_t addr, uint16_t value);

/**
 *  \brief   This function reads a 8-bit value from a hardware register
 *           and returns the value.
 *
 *  \param   addr    Address of the memory mapped hardware register.
 *
 *  \return  Unsigned 8-bit value read from a register.
 */
static inline uint8_t HW_RD_REG8_RAW(uint32_t addr);

/**
 *  \brief   This function writes a 8-bit value to a hardware
 *           register.
 *
 *  \param   addr    Address of the memory mapped hardware register.
 *  \param   value   unsigned 8-bit value which has to be written to the
 *                   register.
 */
static inline void HW_WR_REG8_RAW(uint32_t addr, uint8_t value);

/**
 *  \brief   This function reads a 32 bit register, modifies specific set of
 *           bits and writes back to the register.
 *
 *  \param   addr    Address of the memory mapped hardware register.
 *  \param   mask    Mask for the bit field.
 *  \param   shift   Bit field shift from LSB.
 *  \param   value   Value to be written to bit-field.
 */
static inline void HW_WR_FIELD32_RAW(uint32_t addr,
                                     uint32_t mask,
                                     uint32_t shift,
                                     uint32_t value);

/**
 *  \brief   This function reads a 16 bit register, modifies specific set of
 *           bits and writes back to the register.
 *
 *  \param   addr    Address of the memory mapped hardware register.
 *  \param   mask    Mask for the bit field.
 *  \param   shift   Bit field shift from LSB.
 *  \param   value   Value to be written to bit-field.
 */
static inline void HW_WR_FIELD16_RAW(uint32_t addr,
                                     uint16_t mask,
                                     uint32_t shift,
                                     uint16_t value);

/**
 *  \brief   This function reads a 8 bit register, modifies specific set of
 *           bits and writes back to the register.
 *
 *  \param   addr    Address of the memory mapped hardware register.
 *  \param   mask    Mask for the bit field.
 *  \param   shift   Bit field shift from LSB.
 *  \param   value   Value to be written to bit-field.
 */
static inline void HW_WR_FIELD8_RAW(uint32_t addr,
                                    uint8_t mask,
                                    uint32_t shift,
                                    uint8_t value);

/**
 *  \brief   This function reads a 32 bit register, masks specific set of bits
 *           and the left shifted value.
 *
 *  \param   addr    Address of the memory mapped hardware register.
 *  \param   mask    Mask for the bit field.
 *  \param   shift   Bit field shift from LSB.
 *
 *  \return  Bit-field value (absolute value - shifted to LSB position)
 */
static inline uint32_t HW_RD_FIELD32_RAW(uint32_t addr,
                                         uint32_t mask,
                                         uint32_t shift);

/**
 *  \brief   This function reads a 16 bit register, masks specific set of bits
 *           and the left shifted value.
 *
 *  \param   addr    Address of the memory mapped hardware register.
 *  \param   mask    Mask for the bit field.
 *  \param   shift   Bit field shift from LSB.
 *
 *  \return  Bit-field value (absolute value - shifted to LSB position)
 */
static inline uint16_t HW_RD_FIELD16_RAW(uint32_t addr,
                                         uint16_t mask,
                                         uint32_t shift);

/**
 *  \brief   This function reads a 8 bit register, masks specific set of bits
 *           and the left shifted value.
 *
 *  \param   addr    Address of the memory mapped hardware register.
 *  \param   mask    Mask for the bit field.
 *  \param   shift   Bit field shift from LSB.
 *
 *  \return  Bit-field value (absolute value - shifted to LSB position)
 */
static inline uint8_t HW_RD_FIELD8_RAW(uint32_t addr,
                                       uint8_t mask,
                                       uint32_t shift);


/**
 *  \brief   This function to use memory barrier in case of ARM.
 */
static inline void HW_MEM_BARRIER(void);
/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

static inline void HW_MEM_BARRIER(void)
{
#if (defined(__ARMv7) || defined(__TI_ARM_V7__))
#ifndef MEM_BARRIER_DISABLE
//    asm("    dsb");
#endif
#endif
}
static inline uint32_t HW_RD_REG32_RAW(uint32_t addr)
{
    uint32_t regVal = *(volatile uint32_t *) addr;
    HW_MEM_BARRIER();
    return (regVal);
}

static inline void HW_WR_REG32_RAW(uint32_t addr, uint32_t value)
{
    *(volatile uint32_t *) addr = value;
    HW_MEM_BARRIER();
    return;
}

static inline uint16_t HW_RD_REG16_RAW(uint32_t addr)
{
    uint16_t regVal = *(volatile uint16_t *) addr;
    HW_MEM_BARRIER();
    return (regVal);
}

static inline void HW_WR_REG16_RAW(uint32_t addr, uint16_t value)
{
    *(volatile uint16_t *) addr = value;
    HW_MEM_BARRIER();
    return;
}

static inline uint8_t HW_RD_REG8_RAW(uint32_t addr)
{
    uint8_t regVal = *(volatile uint8_t *) addr;
    HW_MEM_BARRIER();
    return (regVal);
}

static inline void HW_WR_REG8_RAW(uint32_t addr, uint8_t value)
{
    *(volatile uint8_t *) addr = value;
    HW_MEM_BARRIER();
    return;
}

static inline void HW_WR_FIELD32_RAW(uint32_t addr,
                                     uint32_t mask,
                                     uint32_t shift,
                                     uint32_t value)
{
    uint32_t regVal = *(volatile uint32_t *) addr;
    regVal &= (~mask);
    regVal |= (value << shift) & mask;
    *(volatile uint32_t *) addr = regVal;
    HW_MEM_BARRIER();
    return;
}

static inline void HW_WR_FIELD16_RAW(uint32_t addr,
                                     uint16_t mask,
                                     uint32_t shift,
                                     uint16_t value)
{
    uint32_t tempVal;
    uint16_t regVal = *(volatile uint16_t *) addr;
    tempVal = ((uint32_t) regVal);
    tempVal &=  (~((uint32_t) mask));
    tempVal |= (((uint32_t) value) << shift) & ((uint32_t) mask);
    regVal = (uint16_t) tempVal;
    *(volatile uint16_t *) addr = regVal;
    HW_MEM_BARRIER();
    return;
}

static inline void HW_WR_FIELD8_RAW(uint32_t addr,
                                    uint8_t mask,
                                    uint32_t shift,
                                    uint8_t value)
{
    uint32_t tempVal;
    uint8_t regVal = *(volatile uint8_t *) addr;
    tempVal = ((uint32_t) regVal);
    tempVal &=  (~((uint32_t) mask));
    tempVal |= (((uint32_t) value) << shift) & ((uint32_t) mask);
    regVal = (uint8_t) tempVal;
    *(volatile uint8_t *) addr = regVal;
    HW_MEM_BARRIER();
    return;
}

static inline uint32_t HW_RD_FIELD32_RAW(uint32_t addr,
                                         uint32_t mask,
                                         uint32_t shift)
{
    uint32_t regVal = *(volatile uint32_t *) addr;
    regVal = (regVal & mask) >> shift;
    HW_MEM_BARRIER();
    return (regVal);
}

static inline uint16_t HW_RD_FIELD16_RAW(uint32_t addr,
                                         uint16_t mask,
                                         uint32_t shift)
{
    uint32_t tempVal;
    uint16_t regVal = *(volatile uint16_t *) addr;
    tempVal = (((uint32_t) regVal & (uint32_t) mask) >> shift);
    regVal = (uint16_t) tempVal;
    HW_MEM_BARRIER();
    return (regVal);
}

static inline uint8_t HW_RD_FIELD8_RAW(uint32_t addr,
                                       uint8_t mask,
                                       uint32_t shift)
{
    uint32_t tempVal;
    uint8_t regVal = *(volatile uint8_t *) addr;
    tempVal = (((uint32_t) regVal & (uint32_t) mask) >> shift);
    regVal = (uint8_t) tempVal;
    HW_MEM_BARRIER();
    return (regVal);
}

/* ========================================================================== */
/*                         Deprecated Macros                                  */
/* ========================================================================== */

#define HW_SET_FIELD HW_SET_FIELD32
#define HWREG(x)                                                               \
        (*((volatile uint32_t *)(x)))
#define HWREGH(x)                                                              \
        (*((volatile uint16_t *)(x)))
#define HWREGB(x)                                                              \
        (*((volatile uint8_t *)(x)))
#define HWREGBITW(x, b)                                                        \
        (HWREG(((uint32_t)(x) & 0xF0000000) | 0x02000000 |                      \
              (((uint32_t)(x) & 0x000FFFFF) << 5) | ((b) << 2)))
#define HWREGBITH(x, b)                                                        \
        (HWREGH(((uint32_t)(x) & 0xF0000000) | 0x02000000 |                     \
               (((uint32_t)(x) & 0x000FFFFF) << 5) | ((b) << 2)))
#define HWREGBITB(x, b)                                                        \
        (HWREGB(((uint32_t)(x) & 0xF0000000) | 0x02000000 |                     \
               (((uint32_t)(x) & 0x000FFFFF) << 5) | ((b) << 2)))

#ifdef __cplusplus
}
#endif

#endif /* #ifndef HW_TYPES_H_ */
