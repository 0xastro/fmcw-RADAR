/**
 *   @file  sys_defs.h
 *
 *   @brief
 *      This is the common header file that contains definitions that are usable
 *      across mmwave SDK.
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

#ifndef SYS_DEFS_H
#define SYS_DEFS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Inline Functions:
 ******************************************************************************/
static inline uint32_t CSL_FMKR(uint8_t msb, uint8_t lsb, uint32_t val);
static inline uint32_t CSL_FEXTR(uint32_t reg, uint8_t msb, uint8_t lsb);
static inline uint32_t CSL_FINSR(uint32_t reg, uint8_t msb, uint8_t lsb, uint32_t val);

/* the Field MaKe (Raw) macro */
static inline uint32_t CSL_FMKR(uint8_t msb, uint8_t lsb, uint32_t val)
{
    uint32_t    mask;
    uint8_t     bits;
    uint32_t    newVal;

    bits = (msb - lsb + 1U);
    mask = (1U << bits);
    mask = mask - 1U;

    newVal = val & mask;

    return (newVal << lsb);
}

/* the Field EXTract (Raw) macro */
static inline uint32_t CSL_FEXTR(volatile uint32_t reg, uint8_t msb, uint8_t lsb)
{
    uint32_t    mask;
    uint8_t     bits;
    uint32_t    value;

    bits = (msb - lsb + 1U);
    mask = (1U << bits);
    mask = mask - 1U;
    value = (reg >> lsb) & mask;
    return value;
}

/* the Field INSert (Raw) macro */
static inline uint32_t CSL_FINSR(volatile uint32_t reg, uint8_t msb, uint8_t lsb, uint32_t val)
{
    uint32_t    mask;
    uint8_t     bits;
    uint32_t    value;
    uint32_t    tmp;

    bits = (msb - lsb + 1U);
    mask = (1U << bits);
    mask = mask - 1U;
    value = (mask << lsb);
    tmp   = (reg & ~value);
    reg   = tmp | CSL_FMKR(msb, lsb, val);
    return reg;
}

#define     MINUS_ONE           -((int32_t)1)

/*************************************************************
 * Common MACROs
 *************************************************************/
/*! Max macro */
#define MAX(x,y) ((x) > (y) ? (x) : (y))

/*! Min macro */
#define MIN(x,y) ((x) < (y) ? (x) : (y))

/*! Memory alignment */
#define MEM_ALIGN(addr, byteAlignment)  ((((uintptr_t)addr) + ((byteAlignment)-1)) & ~((byteAlignment)-1))

/*! Check for memory non-alignment, returns 1 if not aligned, 0 otherwise */
#define MEM_IS_NOT_ALIGN(addr, byteAlignment) ((uintptr_t)addr & (byteAlignment - 1))

/*! Check for memory alignment, returns 1 if aligned, 0 otherwise */
#define MEM_IS_ALIGN(addr, byteAlignment) (! MEM_IS_NOT_ALIGN(addr, byteAlignment))

/*! Macro for memory alignment to double word (= 2x32-bit = 64-bit). Typically in 
    signal processing the datum sizes are 16 or 32-bits, but optimized code tends 
    to manipulate this data in chunks of 64-bits which are more optimally implemented
    if the data is aligned to 64-bits even though the underlying datum sizes are 
    less than 64-bits. Such requirements are present for example in several 
    mmwavelib functions. */
#define SYS_MEMORY_ALLOC_DOUBLE_WORD_ALIGN_DSP  8

/*! Maximum structure alignment on DSP. Assumes the largest sized field is
    present in the structure, which is 64-bit. Note actual structure may need less
    alignment because the largest size of a field in the structure may be less than 64-bit. */
#define SYS_MEMORY_ALLOC_MAX_STRUCT_BYTE_ALIGNMENT_DSP  sizeof(uint64_t)

/*! Maximum structure alignment on R4F. Assumes the largest sized field is
    present in the structure, which is 64-bit. Note actual structure may need less
    alignment because the largest size of a field in the structure may be less than 64-bit. */
#define SYS_MEMORY_ALLOC_MAX_STRUCT_BYTE_ALIGNMENT_R4F  sizeof(uint64_t)

/*! Find the next multiple of y for x */
#define NEXT_MULTIPLE_OF(x, y)  MEM_ALIGN(x, y)


#ifdef __cplusplus
}
#endif

#endif /* SYS_DEFS_H */

