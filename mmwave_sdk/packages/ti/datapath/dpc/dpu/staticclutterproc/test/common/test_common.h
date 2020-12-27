/**
 *   @file  test_common.h
 *
 *   @brief
 *      Common APIs used on unit uest code for static clutter removal.
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
#ifndef STATICCLUTTERPROC_TESTCOMMON_H
#define STATICCLUTTERPROC_TESTCOMMON_H

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* SDK Include files */
#include <ti/common/sys_common.h>

/* Data Path include files */
//#include <ti/datapath/dpif/dpif.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct testConfiguration_t
{
    uint8_t  numTxAnt;
    uint8_t  numRxAnt;
    uint16_t numDopChirps;
    uint16_t numRanBin;
}testConfiguration;

void Test_main
(
    uint8_t                        numOfScenarios, 
    testConfiguration              *testConfigArray,
    DPU_StaticClutterProc_Config   *initialCfg,
    uint8_t                        EdmaInstanceId,
    uint8_t                        *radarCubeRefPtr,
    uint8_t                        *radarCubeTestPtr,
    uint32_t                       radarCubeSizeInBytes
);


#ifdef __cplusplus
}
#endif

#endif
