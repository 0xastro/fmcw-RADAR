/**
 *   @file  sys_common.h
 *
 *   @brief
 *      This is the common header file used by the various mmWave SDK
 *      modules.
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

#ifndef SYS_COMMON_H
#define SYS_COMMON_H

#include <ti/common/sys_types.h>
#include <ti/common/sys_defs.h>

#ifdef SOC_XWR14XX
#include <ti/common/sys_common_xwr14xx.h>
#endif

#ifdef SOC_XWR16XX
#include <ti/common/sys_common_xwr16xx.h>
#endif

#ifdef SOC_XWR18XX
#include <ti/common/sys_common_xwr18xx.h>
#endif

#ifdef SOC_XWR68XX
#include <ti/common/sys_common_xwr68xx.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************
 * MMWAVE System level defines
 *************************************************************/
#define SYS_COMMON_NUM_RX_CHANNEL                   4U
#define SYS_COMMON_CQ_MAX_CHIRP_THRESHOLD           8U

/* This is the size of the Chirp Parameters (CP) in CBUFF Units */
#define SYS_COMMON_CP_SIZE_CBUFF_UNITS              2U

#ifdef __cplusplus
}
#endif

#endif /* SYS_COMMON_H */

