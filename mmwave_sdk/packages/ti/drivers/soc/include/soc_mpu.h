/*!
 *   @file  soc_mpu.h
 *
 *   @brief
 *      The file has MPU (Memory Protection Unit) APIs for programming the MPU.
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


#ifndef SOC_MPU_H
#define SOC_MPU_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/** @def SOC_MPU_REGION1
*   @brief MPU region 1
*
*   Alias for MPU region 1
*/
#define SOC_MPU_REGION1 0U

/** @def SOC_MPU_REGION2
*   @brief MPU region 2
*
*   Alias for MPU region 1
*/
#define SOC_MPU_REGION2 1U

/** @def SOC_MPU_REGION3
*   @brief MPU region 3
*
*   Alias for MPU region 3
*/
#define SOC_MPU_REGION3 2U

/** @def SOC_MPU_REGION4
*   @brief MPU region 4
*
*   Alias for MPU region 4
*/
#define SOC_MPU_REGION4 3U

/** @def SOC_MPU_REGION5
*   @brief MPU region 5
*
*   Alias for MPU region 5
*/
#define SOC_MPU_REGION5 4U

/** @def SOC_MPU_REGION6
*   @brief MPU region 6
*
*   Alias for MPU region 6
*/
#define SOC_MPU_REGION6 5U

/** @def SOC_MPU_REGION7
*   @brief MPU region 7
*
*   Alias for MPU region 7
*/
#define SOC_MPU_REGION7 6U

/** @def SOC_MPU_REGION8
*   @brief MPU region 8
*
*   Alias for MPU region 8
*/
#define SOC_MPU_REGION8 7U

/** @def SOC_MPU_REGION9
*   @brief MPU region 9
*
*   Alias for MPU region 9
*/
#define SOC_MPU_REGION9 8U

/** @def SOC_MPU_REGION10
*   @brief MPU region 10
*
*   Alias for MPU region 10
*/
#define SOC_MPU_REGION10 9U

/** @def SOC_MPU_REGION11
*   @brief MPU region 11
*
*   Alias for MPU region 11
*/
#define SOC_MPU_REGION11 10U

/** @def SOC_MPU_REGION12
*   @brief MPU region 12
*
*   Alias for MPU region 12
*/
#define SOC_MPU_REGION12 11U

/** @def SOC_MPU_REGION_ENABLE
*   @brief Enable MPU Region
*
*   Alias for MPU region enable.
*
*   @note This should be used as the parameter of the API SOC_MPUSetRegionSizeRegister
*/
#define SOC_MPU_REGION_ENABLE 1U

/** @def SOC_MPU_REGION_DISABLE
*   @brief Disable MPU Region
*
*   Alias for MPU region disable.
*
*   @note This should be used as the parameter of the API SOC_MPUSetRegionSizeRegister
*/
#define SOC_MPU_REGION_DISABLE 0U

/** @def SOC_MPU_SUBREGION0_DISABLE
*   @brief Disable MPU Sub Region0
*
*   Alias for MPU subregion0 disable.
*
*   @note This should be used as the parameter of the API SOC_MPUSetRegionSizeRegister
*/
#define SOC_MPU_SUBREGION0_DISABLE 0x100U

/** @def SOC_MPU_SUBREGION1_DISABLE
*   @brief Disable MPU Sub Region1
*
*   Alias for MPU subregion1 disable.
*
*   @note This should be used as the parameter of the API SOC_MPUSetRegionSizeRegister
*/
#define SOC_MPU_SUBREGION1_DISABLE 0x200U

/** @def SOC_MPU_SUBREGION2_DISABLE
*   @brief Disable MPU Sub Region2
*
*   Alias for MPU subregion2 disable.
*
*   @note This should be used as the parameter of the API SOC_MPUSetRegionSizeRegister
*/
#define SOC_MPU_SUBREGION2_DISABLE 0x400U

/** @def SOC_MPU_SUBREGION3_DISABLE
*   @brief Disable MPU Sub Region3
*
*   Alias for MPU subregion3 disable.
*
*   @note This should be used as the parameter of the API SOC_MPUSetRegionSizeRegister
*/
#define SOC_MPU_SUBREGION3_DISABLE 0x800U

/** @def SOC_MPU_SUBREGION4_DISABLE
*   @brief Disable MPU Sub Region4
*
*   Alias for MPU subregion4 disable.
*
*   @note This should be used as the parameter of the API SOC_MPUSetRegionSizeRegister
*/
#define SOC_MPU_SUBREGION4_DISABLE 0x1000U

/** @def SOC_MPU_SUBREGION5_DISABLE
*   @brief Disable MPU Sub Region5
*
*   Alias for MPU subregion5 disable.
*
*   @note This should be used as the parameter of the API SOC_MPUSetRegionSizeRegister
*/
#define SOC_MPU_SUBREGION5_DISABLE 0x2000U

/** @def SOC_MPU_SUBREGION6_DISABLE
*   @brief Disable MPU Sub Region6
*
*   Alias for MPU subregion6 disable.
*
*   @note This should be used as the parameter of the API SOC_MPUSetRegionSizeRegister_
*/
#define SOC_MPU_SUBREGION6_DISABLE 0x4000U

/** @def SOC_MPU_SUBREGION7_DISABLE
*   @brief Disable MPU Sub Region7
*
*   Alias for MPU subregion7 disable.
*
*   @note This should be used as the parameter of the API SOC_MPUSetRegionSizeRegister
*/
#define SOC_MPU_SUBREGION7_DISABLE 0x8000U

/*! @brief Alignment macro for MPU programming purposes */
#define SOC_MPU_ALIGN(x,y)          ((x) & ~((y) - 1U))



/** @enum SOC_MPURegionAccessPermission_e
*   @brief Alias names for MPU region access permissions
*
*   This enumeration is used to provide alias names for the MPU region access permission:
*     - SOC_MPU_PRIV_NA_USER_NA_EXEC no access in privileged mode, no access in user mode and execute
*     - SOC_MPU_PRIV_RW_USER_NA_EXEC read/write in privileged mode, no access in user mode and execute
*     - SOC_MPU_PRIV_RW_USER_RO_EXEC read/write in privileged mode, read only in user mode and execute
*     - SOC_MPU_PRIV_RW_USER_RW_EXEC read/write in privileged mode, read/write in user mode and execute
*     - SOC_MPU_PRIV_RO_USER_NA_EXEC read only in privileged mode, no access in user mode and execute
*     - SOC_MPU_PRIV_RO_USER_RO_EXEC read only in privileged mode, read only in user mode and execute
*     - SOC_MPU_PRIV_NA_USER_NA_NOEXEC no access in privileged mode, no access in user mode and no execution
*     - SOC_MPU_PRIV_RW_USER_NA_NOEXEC read/write in privileged mode, no access in user mode and no execution
*     - SOC_MPU_PRIV_RW_USER_RO_NOEXEC read/write in privileged mode, read only in user mode and no execution
*     - SOC_MPU_PRIV_RW_USER_RW_NOEXEC read/write in privileged mode, read/write in user mode and no execution
*     - SOC_MPU_PRIV_RO_USER_NA_NOEXEC read only in privileged mode, no access in user mode and no execution
*     - SOC_MPU_PRIV_RO_USER_RO_NOEXEC read only in privileged mode, read only in user mode and no execution
*
*/
typedef enum SOC_MPURegionAccessPermission_e
{
    SOC_MPU_PRIV_NA_USER_NA_EXEC   = 0x0000U, /**< Alias no access in privileged mode, no access in user mode and execute */
    SOC_MPU_PRIV_RW_USER_NA_EXEC   = 0x0100U, /**< Alias read/write in privileged mode, no access in user mode and execute */
    SOC_MPU_PRIV_RW_USER_RO_EXEC   = 0x0200U, /**< Alias read/write in privileged mode, read only in user mode and execute */
    SOC_MPU_PRIV_RW_USER_RW_EXEC   = 0x0300U, /**< Alias read/write in privileged mode, read/write in user mode and execute */
    SOC_MPU_PRIV_RO_USER_NA_EXEC   = 0x0500U, /**< Alias read only in privileged mode, no access in user mode and execute */
    SOC_MPU_PRIV_RO_USER_RO_EXEC   = 0x0600U, /**< Alias read only in privileged mode, read only in user mode and execute */
    SOC_MPU_PRIV_NA_USER_NA_NOEXEC = 0x1000U, /**< Alias no access in privileged mode, no access in user mode and no execution */
    SOC_MPU_PRIV_RW_USER_NA_NOEXEC = 0x1100U, /**< Alias read/write in privileged mode, no access in user mode and no execution */
    SOC_MPU_PRIV_RW_USER_RO_NOEXEC = 0x1200U, /**< Alias read/write in privileged mode, read only in user mode and no execution */
    SOC_MPU_PRIV_RW_USER_RW_NOEXEC = 0x1300U, /**< Alias read/write in privileged mode, read/write in user mode and no execution */
    SOC_MPU_PRIV_RO_USER_NA_NOEXEC = 0x1500U, /**< Alias read only in privileged mode, no access in user mode and no execution */
    SOC_MPU_PRIV_RO_USER_RO_NOEXEC = 0x1600U  /**< Alias read only in privileged mode, read only in user mode and no execution */
}SOC_MPURegionAccessPermission;

/** @enum SOC_MPURegionType_e
*   @brief Alias names for MPU region type
*
*   This enumeration is used to provide alias names for the MPU region type:
*     - SOC_MPU_STRONGLYORDERED_SHAREABLE Memory type strongly ordered and sharable
*     - SOC_MPU_DEVICE_SHAREABLE          Memory type device and sharable
*     - SOC_MPU_NORMAL_OIWTNOWA_NONSHARED Memory type normal outer and inner write-through, no write allocate and non shared
*     - SOC_MPU_NORMAL_OIWTNOWA_SHARED    Memory type normal outer and inner write-through, no write allocate and shared
*     - SOC_MPU_NORMAL_OIWBNOWA_NONSHARED Memory type normal outer and inner write-back, no write allocate and non shared
*     - SOC_MPU_NORMAL_OIWBNOWA_SHARED    Memory type normal outer and inner write-back, no write allocate and shared
*     - SOC_MPU_NORMAL_OINC_NONSHARED     Memory type normal outer and inner non-cachable and non shared
*     - SOC_MPU_NORMAL_OINC_SHARED        Memory type normal outer and inner non-cachable and shared
*     - SOC_MPU_NORMAL_OIWBWA_NONSHARED   Memory type normal outer and inner write-back, write allocate and non shared
*     - SOC_MPU_NORMAL_OIWBWA_SHARED      Memory type normal outer and inner write-back, write allocate and shared
*     - SOC_MPU_DEVICE_NONSHAREABLE       Memory type device and non sharable
*/
typedef enum SOC_MPURegionType_e
{
    SOC_MPU_STRONGLYORDERED_SHAREABLE = 0x0000U, /**< Memory type strongly ordered and sharable */
    SOC_MPU_DEVICE_SHAREABLE          = 0x0001U, /**< Memory type device and sharable */
    SOC_MPU_NORMAL_OIWTNOWA_NONSHARED = 0x0002U, /**< Memory type normal outer and inner write-through, no write allocate and non shared */
    SOC_MPU_NORMAL_OIWBNOWA_NONSHARED = 0x0003U, /**< Memory type normal outer and inner write-back, no write allocate and non shared */
    SOC_MPU_NORMAL_OIWTNOWA_SHARED    = 0x0006U, /**< Memory type normal outer and inner write-through, no write allocate and shared */
    SOC_MPU_NORMAL_OIWBNOWA_SHARED    = 0x0007U, /**< Memory type normal outer and inner write-back, no write allocate and shared */
    SOC_MPU_NORMAL_OINC_NONSHARED     = 0x0008U, /**< Memory type normal outer and inner non-cachable and non shared */
    SOC_MPU_NORMAL_OIWBWA_NONSHARED   = 0x000BU, /**< Memory type normal outer and inner write-back, write allocate and non shared */
    SOC_MPU_NORMAL_OINC_SHARED        = 0x000CU, /**< Memory type normal outer and inner non-cachable and shared */
    SOC_MPU_NORMAL_OIWBWA_SHARED      = 0x000FU, /**< Memory type normal outer and inner write-back, write allocate and shared */
    SOC_MPU_DEVICE_NONSHAREABLE       = 0x0010U  /**< Memory type device and non sharable */
}SOC_MPURegionType;

/** @enum SOC_MPURegionSize_e
*   @brief Alias names for MPU region type
*
*   This enumeration is used to provide alias names for the MPU region type:
*     - SOC_MPU_STRONGLYORDERED_SHAREABLE Memory type strongly ordered and sharable
*     - SOC_MPU_32_BYTES Memory size in bytes
*     - SOC_MPU_64_BYTES Memory size in bytes
*     - SOC_MPU_128_BYTES Memory size in bytes
*     - SOC_MPU_256_BYTES Memory size in bytes
*     - SOC_MPU_512_BYTES Memory size in bytes
*     - SOC_MPU_1_KB Memory size in kB
*     - SOC_MPU_2_KB Memory size in kB
*     - SOC_MPU_4_KB Memory size in kB
*     - SOC_MPU_8_KB Memory size in kB
*     - SOC_MPU_16_KB Memory size in kB
*     - SOC_MPU_32_KB Memory size in kB
*     - SOC_MPU_64_KB Memory size in kB
*     - SOC_MPU_128_KB Memory size in kB
*     - SOC_MPU_256_KB  Memory size in kB
*     - SOC_MPU_512_KB Memory size in kB
*     - SOC_MPU_1_MB Memory size in MB
*     - SOC_MPU_2_MB Memory size in MB
*     - SOC_MPU_4_MB Memory size in MB
*     - SOC_MPU_8_MBv Memory size in MB
*     - SOC_MPU_16_MB Memory size in MB
*     - SOC_MPU_32_MB Memory size in MB
*     - SOC_MPU_64_MB Memory size in MB
*     - SOC_MPU_128_MB Memory size in MB
*     - SOC_MPU_256_MB Memory size in MB
*     - SOC_MPU_512_MB Memory size in MB
*     - SOC_MPU_1_GB Memory size in GB
*     - SOC_MPU_2_GB Memory size in GB
*     - SOC_MPU_4_GB Memory size in GB
*/
typedef enum SOC_MPURegionSize_e
{
    SOC_MPU_32_BYTES  = 0x04U << 1U, /**< Memory size in bytes */
    SOC_MPU_64_BYTES  = 0x05U << 1U, /**< Memory size in bytes */
    SOC_MPU_128_BYTES = 0x06U << 1U, /**< Memory size in bytes */
    SOC_MPU_256_BYTES = 0x07U << 1U, /**< Memory size in bytes */
    SOC_MPU_512_BYTES = 0x08U << 1U, /**< Memory size in bytes */
    SOC_MPU_1_KB      = 0x09U << 1U, /**< Memory size in kB */
    SOC_MPU_2_KB      = 0x0AU << 1U, /**< Memory size in kB */
    SOC_MPU_4_KB      = 0x0BU << 1U, /**< Memory size in kB */
    SOC_MPU_8_KB      = 0x0CU << 1U, /**< Memory size in kB */
    SOC_MPU_16_KB     = 0x0DU << 1U, /**< Memory size in kB */
    SOC_MPU_32_KB     = 0x0EU << 1U, /**< Memory size in kB */
    SOC_MPU_64_KB     = 0x0FU << 1U, /**< Memory size in kB */
    SOC_MPU_128_KB    = 0x10U << 1U, /**< Memory size in kB */
    SOC_MPU_256_KB    = 0x11U << 1U, /**< Memory size in kB */
    SOC_MPU_512_KB    = 0x12U << 1U, /**< Memory size in kB */
    SOC_MPU_1_MB      = 0x13U << 1U, /**< Memory size in MB */
    SOC_MPU_2_MB      = 0x14U << 1U, /**< Memory size in MB */
    SOC_MPU_4_MB      = 0x15U << 1U, /**< Memory size in MB */
    SOC_MPU_8_MB      = 0x16U << 1U, /**< Memory size in MB */
    SOC_MPU_16_MB     = 0x17U << 1U, /**< Memory size in MB */
    SOC_MPU_32_MB     = 0x18U << 1U, /**< Memory size in MB */
    SOC_MPU_64_MB     = 0x19U << 1U, /**< Memory size in MB */
    SOC_MPU_128_MB    = 0x1AU << 1U, /**< Memory size in MB */
    SOC_MPU_256_MB    = 0x1BU << 1U, /**< Memory size in MB */
    SOC_MPU_512_MB    = 0x1CU << 1U, /**< Memory size in MB */
    SOC_MPU_1_GB      = 0x1DU << 1U, /**< Memory size in GB */
    SOC_MPU_2_GB      = 0x1EU << 1U, /**< Memory size in GB */
    SOC_MPU_4_GB      = 0x1FU << 1U  /**< Memory size in GB */
}SOC_MPURegionSize;

/* SOC MPU configuration APIs */


/** @fn void SOC_MPUInit(void)
*   @brief Initialize MPU
*
*   This function initializes memory protection unit.
*/
void SOC_MPUInit(void);

/** @fn void SOC_MPUEnable(void)
*   @brief Enable MPU
*
*   This function enables memory protection unit.
*/
void SOC_MPUEnable(void);

/** @fn void SOC_MPUDisable(void)
*   @brief Disable MPU
*
*   This function disables memory protection unit.
*/
void SOC_MPUDisable(void);

/** @fn void SOC_MPUEnableBackgroundRegion(void)
*   @brief Enable MPU background region
*
*   This function enables background region of the memory protection unit.
*/
void SOC_MPUEnableBackgroundRegion(void);

/** @fn void SOC_MPUDisableBackgroundRegion(void)
*   @brief Disable MPU background region
*
*   This function disables background region of the memory protection unit.
*/
void SOC_MPUDisableBackgroundRegion(void);

/** @fn uint32_t SOC_MPUGetNumberOfRegions(void)
*   @brief Returns number of implemented MPU regions
*   @return Number of implemented MPU regions
*
*   This function returns the number of implemented MPU regions.
*/
uint32_t SOC_MPUGetNumberOfRegions(void);

/** @fn uint32_t SOC_MPUAreRegionsSeparate(void)
*   @brief Returns the type of the implemented MPU regions
*   @return MPU type of regions
*
*   This function returns 0 when MPU regions are of type unified otherwise regions are of type separate.
*/
uint32_t SOC_MPUAreRegionsSeparate(void);

/** @fn void SOC_MPUSetRegion(uint32_t region)
*   @brief Set MPU region number
*   @param[in] region Region number: SOC_MPU_REGION1..SOC_MPU_REGION12
*
*   This function selects one of the implemented MPU regions.
*/
void SOC_MPUSetRegion(uint32_t region);

/** @fn uint32_t SOC_MPUGetRegion(void)
*   @brief Returns the currently selected MPU region
*   @return MPU region number
*
*   This function returns currently selected MPU region number.
*/
uint32_t SOC_MPUGetRegion(void);

/** @fn void SOC_MPUSetRegionBaseAddress(uint32_t address)
*   @brief Set base address of currently selected MPU region
*   @param[in] address Base address of the MPU region
*   @note The base address must always aligned with region size
*
*   This function sets the base address of currently selected MPU region.
*/
void SOC_MPUSetRegionBaseAddress(uint32_t address);

/** @fn uint32_t SOC_MPUGetRegionBaseAddress(void)
*   @brief Returns base address of currently selected MPU region
*   @return Current base address of selected MPU region
*
*   This function returns the base address of currently selected MPU region.
*/
uint32_t SOC_MPUGetRegionBaseAddress(void);

/** @fn void SOC_MPUSetRegionTypeAndPermission(uint32_t type, uint32_t permission)
*   @brief Set type of currently selected MPU region
*   @param[in] type Region Type
*                     - SOC_MPU_STRONGLYORDERED_SHAREABLE : Memory type strongly ordered and sharable
*                     - SOC_MPU_DEVICE_SHAREABLE          : Memory type device and sharable
*                     - SOC_MPU_NORMAL_OIWTNOWA_NONSHARED : Memory type normal outer and inner write-through, no write allocate and non shared
*                     - SOC_MPU_NORMAL_OIWBNOWA_NONSHARED : Memory type normal outer and inner write-back, no write allocate and non shared
*                     - SOC_MPU_NORMAL_OIWTNOWA_SHARED    : Memory type normal outer and inner write-through, no write allocate and shared
*                     - SOC_MPU_NORMAL_OIWBNOWA_SHARED    : Memory type normal outer and inner write-back, no write allocate and shared
*                     - SOC_MPU_NORMAL_OINC_NONSHARED     : Memory type normal outer and inner non-cachable and non shared
*                     - SOC_MPU_NORMAL_OIWBWA_NONSHARED   : Memory type normal outer and inner write-back, write allocate and non shared
*                     - SOC_MPU_NORMAL_OINC_SHARED        : Memory type normal outer and inner non-cachable and shared
*                     - SOC_MPU_NORMAL_OIWBWA_SHARED      : Memory type normal outer and inner write-back, write allocate and shared
*                     - SOC_MPU_DEVICE_NONSHAREABLE       : Memory type device and non sharable
*
*   @param[in] permission Region Access permission
*                           - SOC_MPU_PRIV_NA_USER_NA_EXEC   : Alias no access in privileged mode, no access in user mode and execute
*                           - SOC_MPU_PRIV_RW_USER_NA_EXEC   : Alias read/write in privileged mode, no access in user mode and execute
*                           - SOC_MPU_PRIV_RW_USER_RO_EXEC   : Alias read/write in privileged mode, read only in user mode and execute
*                           - SOC_MPU_PRIV_RW_USER_RW_EXEC   : Alias read/write in privileged mode, read/write in user mode and execute
*                           - SOC_MPU_PRIV_RO_USER_NA_EXEC   : Alias read only in privileged mode, no access in user mode and execute
*                           - SOC_MPU_PRIV_RO_USER_RO_EXEC   : Alias read only in privileged mode, read only in user mode and execute
*                           - SOC_MPU_PRIV_NA_USER_NA_NOEXEC : Alias no access in privileged mode, no access in user mode and no execution
*                           - SOC_MPU_PRIV_RW_USER_NA_NOEXEC : Alias read/write in privileged mode, no access in user mode and no execution
*                           - SOC_MPU_PRIV_RW_USER_RO_NOEXEC : Alias read/write in privileged mode, read only in user mode and no execution
*                           - SOC_MPU_PRIV_RW_USER_RW_NOEXEC : Alias read/write in privileged mode, read/write in user mode and no execution
*                           - SOC_MPU_PRIV_RO_USER_NA_NOEXEC : Alias read only in privileged mode, no access in user mode and no execution
*                           - SOC_MPU_PRIV_RO_USER_RO_NOEXEC : Alias read only in privileged mode, read only in user mode and no execution
*
*   This function sets the type of currently selected MPU region.
*/
void SOC_MPUSetRegionTypeAndPermission(uint32_t type, uint32_t permission);

/** @fn uint32_t SOC_MPUGetRegionType(void)
*   @brief Returns the type of currently selected MPU region
*   @return Current type of selected MPU region
*
*   This function returns the type of currently selected MPU region.
*/
uint32_t SOC_MPUGetRegionType(void);

/** @fn uint32_t SOC_MPUGetRegionPermission(void)
*   @brief Returns permission of currently selected MPU region
*   @return Current type of selected MPU region
*
*   This function returns permission of currently selected MPU region.
*/
uint32_t SOC_MPUGetRegionPermission(void);

/** @fn void SOC_MPUSetRegionSizeRegister(uint32_t value)
*   @brief Set MPU region size register value
*   @param[in] value Value to be written in the MPU Region Size and Enable register
*
*   This function sets MPU region size register value.
*
*   Sample usuage:
*     SOC_MPUSetRegion(SOC_MPUREGION5);
*     SOC_MPUSetRegionSizeRegister(SOC_MPU_REGION_ENABLE | SOC_MPU_16_KB | SOC_MPU_SUBREGION3_DISABLE | SOC_MPU_SUBREGION4_DISABLE);
*/
void SOC_MPUSetRegionSizeRegister(uint32_t value);

#ifdef __cplusplus
}
#endif

#endif
