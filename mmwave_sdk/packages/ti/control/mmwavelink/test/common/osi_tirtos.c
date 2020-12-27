/*
 *   @file  osi_tirtos.c
 *
 *   @brief
 *      OSAL Porting Interface which is required by the mmWave Link
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* BIOS Include Files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Mailbox.h>

/* mmWave Link Include File */
#include <ti/control/mmwavelink/mmwavelink.h>

/**
 *  @b Description
 *  @n
 *      Registered OSAL Function with the Radar Link to create a Mutex
 *
 *  @retval
 *      Not Applicable.
 */
rlInt32_t Osal_mutexCreate(rlOsiMutexHdl_t* mutexHdl, rlInt8_t* name)
{
    Semaphore_Handle    semHandle;
    Semaphore_Params    params;
    int32_t             errCode = 0;

    /* Initialize and create the semaphore: */
    Semaphore_Params_init(&params);
    params.mode = Semaphore_Mode_BINARY;
    semHandle = Semaphore_create(1, &params, NULL);
    if (semHandle == NULL)
        errCode = -1;

    /* Populate the Mutex Handle */
    *mutexHdl = (rlOsiMutexHdl_t*)semHandle;
    return errCode;
}

/**
 *  @b Description
 *  @n
 *      Registered OSAL Function with the Radar Link to lock the Mutex
 *
 *  @retval
 *      Not Applicable.
 */
rlInt32_t Osal_mutexLock(rlOsiMutexHdl_t* mutexHdl, rlOsiTime_t timeout)
{
    Semaphore_pend((Semaphore_Handle)(*mutexHdl), timeout);
    return 0;
}

/**
 *  @b Description
 *  @n
 *      Registered OSAL Function with the Radar Link to unlock the Mutex
 *
 *  @retval
 *      Not Applicable.
 */
rlInt32_t Osal_mutexUnlock(rlOsiMutexHdl_t* mutexHdl)
{
    Semaphore_post((Semaphore_Handle)(*mutexHdl));
    return 0;
}

/**
 *  @b Description
 *  @n
 *      Registered OSAL Function with the Radar Link to delete the Mutex
 *
 *  @retval
 *      Not Applicable.
 */
rlInt32_t Osal_mutexDelete(rlOsiMutexHdl_t* mutexHdl)
{
    Semaphore_delete((Semaphore_Handle*)mutexHdl);
    return 0;
}

/**
 *  @b Description
 *  @n
 *      Registered OSAL Function with the Radar Link to create a semaphore
 *
 *  @retval
 *      Not Applicable.
 */
rlInt32_t Osal_semCreate(rlOsiSemHdl_t* semHdl, rlInt8_t* name)
{
    Semaphore_Handle    semHandle;
    Semaphore_Params    params;
    int32_t             errCode = 0;

    /* Initialize and create the semaphore: */
    Semaphore_Params_init(&params);
    params.mode = Semaphore_Mode_BINARY;
    semHandle = Semaphore_create(1, &params, NULL);
    if (semHandle == NULL)
        errCode = -1;

    /* Populate the Mutex Handle */
    *semHdl = (rlOsiSemHdl_t*)semHandle;
    return errCode;
}

/**
 *  @b Description
 *  @n
 *      Registered OSAL Function with the Radar Link to wait on a semaphore
 *
 *  @retval
 *      Not Applicable.
 */
rlInt32_t Osal_semWait(rlOsiSemHdl_t* semHdl, rlOsiTime_t timeout)
{
    Semaphore_pend((Semaphore_Handle)(*semHdl), timeout);
    return 0;
}

/**
 *  @b Description
 *  @n
 *      Registered OSAL Function with the Radar Link to release the semaphore
 *
 *  @retval
 *      Not Applicable.
 */
rlInt32_t Osal_semSignal(rlOsiSemHdl_t* semHdl)
{
    Semaphore_post((Semaphore_Handle)(*semHdl));
    return 0;
}

/**
 *  @b Description
 *  @n
 *      Registered OSAL Function with the Radar Link to delete the semaphore
 *
 *  @retval
 *      Not Applicable.
 */
rlInt32_t Osal_semDelete(rlOsiSemHdl_t* semHdl)
{
    Semaphore_delete((Semaphore_Handle*)semHdl);
    return 0;
}

/**
 *  @b Description
 *  @n
 *      Registered OSAL Function with the Radar Link to suspend the task for
 *      a specific delay period
 *
 *  @retval
 *      Not Applicable.
 */
rlInt32_t Osal_delay(rlUInt32_t delay)
{
    Task_sleep(delay);
    return 0;
}

