/**
 *   @file  dpedmahwa.h
 *
 *   @brief
 *      EDMA Configuration Utility API definitions for HWA.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2018 Texas Instruments, Inc.
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
#ifndef DPEDMAHWA_H
#define DPEDMAHWA_H

/* Include Files */
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/hwa/hwa.h>

#include <ti/datapath/dpedma/dpedma.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t DPEDMAHWA_configOneHotSignature
(
    EDMA_Handle         edmaHandle,
    DPEDMA_ChanCfg      *chanCfg,
    HWA_Handle          hwaHandle,
    uint8_t             dmaTriggerSource,
    bool                isEventTriggered
);

int32_t DPEDMAHWA_configTwoHotSignature
(
    EDMA_Handle         edmaHandle,
    DPEDMA_ChanCfg      *chanCfg,
    HWA_Handle          hwaHandle,
    uint8_t             dmaTriggerSource1,
    uint8_t             dmaTriggerSource2,
    bool                isEventTriggered
);

#ifdef __cplusplus
}
#endif

#endif
