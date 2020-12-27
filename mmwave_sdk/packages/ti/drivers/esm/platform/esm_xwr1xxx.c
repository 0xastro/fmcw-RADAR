/*
 * Copyright (c) 2015, Texas Instruments Incorporated
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
  *  ======== esm_xwr1xxx.c ========
  */

#include <stdint.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/esm/include/esm_internal.h>

/***********************************************************
  **********************   XWR14XX  ************************
  **********************************************************/
#ifdef SOC_XWR14XX
ESM_HWAttrs gESMHwCfgAttrs =
{
    SOC_XWR14XX_MSS_ESM_BASE_ADDRESS,
    SOC_XWR14XX_MSS_ESM_HIGH_PRIORITY_INT,
    SOC_XWR14XX_MSS_ESM_LOW_PRIORITY_INT
};
#endif

/***********************************************************
  **********************   XWR16XX  ************************
  **********************************************************/
#ifdef SOC_XWR16XX
#ifdef SUBSYS_MSS
ESM_HWAttrs gESMHwCfgAttrs =
{
    SOC_XWR16XX_MSS_ESM_BASE_ADDRESS,
    SOC_XWR16XX_MSS_ESM_HIGH_PRIORITY_INT,
    SOC_XWR16XX_MSS_ESM_LOW_PRIORITY_INT
};
#elif defined (SUBSYS_DSS)
ESM_HWAttrs gESMHwCfgAttrs =
{
    SOC_XWR16XX_DSS_ESM_BASE_ADDRESS,
    0,                                  /* Not used */
    0                                   /* Not used */
};
#else
#error "Error: Please check the compiler flags since SUBSYS_XXX is not defined for the XWR16xx device"
#endif
#endif

/***********************************************************
  **********************   XWR18XX  ************************
  **********************************************************/
#ifdef SOC_XWR18XX
#ifdef SUBSYS_MSS
ESM_HWAttrs gESMHwCfgAttrs =
{
    SOC_XWR18XX_MSS_ESM_BASE_ADDRESS,
    SOC_XWR18XX_MSS_ESM_HIGH_PRIORITY_INT,
    SOC_XWR18XX_MSS_ESM_LOW_PRIORITY_INT
};
#elif defined (SUBSYS_DSS)
ESM_HWAttrs gESMHwCfgAttrs =
{
    SOC_XWR18XX_DSS_ESM_BASE_ADDRESS,
    0,                                  /* Not used */
    0                                   /* Not used */
};
#else
#error "Error: Please check the compiler flags since SUBSYS_XXX is not defined for the XWR18xx device"
#endif
#endif

/***********************************************************
  **********************   XWR68XX  ************************
  **********************************************************/
#ifdef SOC_XWR68XX
#ifdef SUBSYS_MSS
ESM_HWAttrs gESMHwCfgAttrs =
{
    SOC_XWR68XX_MSS_ESM_BASE_ADDRESS,
    SOC_XWR68XX_MSS_ESM_HIGH_PRIORITY_INT,
    SOC_XWR68XX_MSS_ESM_LOW_PRIORITY_INT
};
#elif defined (SUBSYS_DSS)
ESM_HWAttrs gESMHwCfgAttrs =
{
    SOC_XWR68XX_DSS_ESM_BASE_ADDRESS,
    0,                                  /* Not used */
    0                                   /* Not used */
};
#else
#error "Error: Please check the compiler flags since SUBSYS_XXX is not defined for the XWR68xx device"
#endif
#endif

