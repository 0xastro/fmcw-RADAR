/*
 *   @file  main.c
 *
 *   @brief
 *      Contains the main function for secondary bootloader application.
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

/* Standard Include Files. */
#include <stdint.h>
#include <string.h>

/**************************************************************************
 *************************** External definitions *************************
 **************************************************************************/
extern uint32_t _libRunAddr, _libLoadStart, _libLoadEnd;
extern uint32_t _constRunAddr, _constLoadStart, _constLoadEnd;

extern void SBL_init(void);

/**************************************************************************
 *************************** Function Definitions *************************
 **************************************************************************/
static void copyCode(uint32_t *dest, uint32_t *src, uint32_t length);

/**
 *  @b Description
 *  @n
 *      Code copy routine
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not Applicable
 */
static void copyCode(uint32_t *dest, uint32_t *src, uint32_t length)
{
    uint32_t    index = 0;
    uint8_t*    srcPtr = (uint8_t*)src;
    uint8_t*    destPtr = (uint8_t*)dest;

     for (index = 0; index < length; index++)
         destPtr[index] = srcPtr[index];
    return;
}

/**
 *  @b Description
 *  @n
 *      This function is called on reset. It copies the code deom TCMA to TCMB memory.
 *      All the SBL routines run in TCMB memory. The variables used are defined in
 *      the linker command file.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not Applicable
 */
void _system_pre_init(void)
{
    copyCode(&_libRunAddr, &_libLoadStart, ((uint32_t)&_libLoadEnd - (uint32_t)&_libLoadStart));
    copyCode(&_constRunAddr, &_constLoadStart, ((uint32_t)&_constLoadEnd - (uint32_t)&_constLoadStart));

    return;
}

/**
 *  @b Description
 *  @n
 *      Entry point into the application code.
 *      This is the only code that runs in TCMA memory.
 *      Calls the SBL initialization routine that will run in TCMB memory.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not Applicable.
 */
int main (void)
{
    SBL_init();
    return 0;
}

