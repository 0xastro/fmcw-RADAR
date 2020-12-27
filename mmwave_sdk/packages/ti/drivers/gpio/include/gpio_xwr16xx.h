/**
 *   @file  gpio_xwr16xx.h
 *
 *   @brief
 *      GPIO Platform specific definitions for XWR16xx
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
#ifndef GPIO_XWR16XX_H
#define GPIO_XWR16XX_H

#include <ti/drivers/gpio/include/gpio_internal.h>

/***************************************************************************
 * GPIO Index for XWR16xx:
 * - This is the GPIO Index which needs to be passed to the GPIO Driver API
 ***************************************************************************/

/* Port 0 (A) */
#define SOC_XWR16XX_GPIO_0        GPIO_CREATE_INDEX (0, 0)
#define SOC_XWR16XX_GPIO_1        GPIO_CREATE_INDEX (0, 1)
#define SOC_XWR16XX_GPIO_2        GPIO_CREATE_INDEX (0, 2)
#define SOC_XWR16XX_GPIO_3        GPIO_CREATE_INDEX (0, 3)
#define SOC_XWR16XX_GPIO_4        GPIO_CREATE_INDEX (0, 4)
#define SOC_XWR16XX_GPIO_5        GPIO_CREATE_INDEX (0, 5)
#define SOC_XWR16XX_GPIO_6        GPIO_CREATE_INDEX (0, 6)
#define SOC_XWR16XX_GPIO_7        GPIO_CREATE_INDEX (0, 7)

/* Port 1 (B) */
#define SOC_XWR16XX_GPIO_8        GPIO_CREATE_INDEX (1, 0)
#define SOC_XWR16XX_GPIO_9        GPIO_CREATE_INDEX (1, 1)
#define SOC_XWR16XX_GPIO_10       GPIO_CREATE_INDEX (1, 2)
#define SOC_XWR16XX_GPIO_11       GPIO_CREATE_INDEX (1, 3)
#define SOC_XWR16XX_GPIO_12       GPIO_CREATE_INDEX (1, 4)
#define SOC_XWR16XX_GPIO_13       GPIO_CREATE_INDEX (1, 5)
#define SOC_XWR16XX_GPIO_14       GPIO_CREATE_INDEX (1, 6)
#define SOC_XWR16XX_GPIO_15       GPIO_CREATE_INDEX (1, 7)

/* Port 2 (C) */
#define SOC_XWR16XX_GPIO_16       GPIO_CREATE_INDEX (2, 0)
#define SOC_XWR16XX_GPIO_17       GPIO_CREATE_INDEX (2, 1)
#define SOC_XWR16XX_GPIO_18       GPIO_CREATE_INDEX (2, 2)
#define SOC_XWR16XX_GPIO_19       GPIO_CREATE_INDEX (2, 3)
#define SOC_XWR16XX_GPIO_20       GPIO_CREATE_INDEX (2, 4)
#define SOC_XWR16XX_GPIO_21       GPIO_CREATE_INDEX (2, 5)
#define SOC_XWR16XX_GPIO_22       GPIO_CREATE_INDEX (2, 6)
#define SOC_XWR16XX_GPIO_23       GPIO_CREATE_INDEX (2, 7)

/* Port 3 (D)*/
#define SOC_XWR16XX_GPIO_24       GPIO_CREATE_INDEX (3, 0)
#define SOC_XWR16XX_GPIO_25       GPIO_CREATE_INDEX (3, 1)
#define SOC_XWR16XX_GPIO_26       GPIO_CREATE_INDEX (3, 2)
#define SOC_XWR16XX_GPIO_27       GPIO_CREATE_INDEX (3, 3)
#define SOC_XWR16XX_GPIO_28       GPIO_CREATE_INDEX (3, 4)
#define SOC_XWR16XX_GPIO_29       GPIO_CREATE_INDEX (3, 5)
#define SOC_XWR16XX_GPIO_30       GPIO_CREATE_INDEX (3, 6)
#define SOC_XWR16XX_GPIO_31       GPIO_CREATE_INDEX (3, 7)

/* Port 4 (E)*/
#define SOC_XWR16XX_GPIO_32       GPIO_CREATE_INDEX (4, 0)
#define SOC_XWR16XX_GPIO_33       GPIO_CREATE_INDEX (4, 1)
#define SOC_XWR16XX_GPIO_34       GPIO_CREATE_INDEX (4, 2)
#define SOC_XWR16XX_GPIO_35       GPIO_CREATE_INDEX (4, 3)
#define SOC_XWR16XX_GPIO_36       GPIO_CREATE_INDEX (4, 4)
#define SOC_XWR16XX_GPIO_37       GPIO_CREATE_INDEX (4, 5)
#define SOC_XWR16XX_GPIO_38       GPIO_CREATE_INDEX (4, 6)
#define SOC_XWR16XX_GPIO_39       GPIO_CREATE_INDEX (4, 7)

/* Port 5 (F) */
#define SOC_XWR16XX_GPIO_40       GPIO_CREATE_INDEX (5, 0)
#define SOC_XWR16XX_GPIO_41       GPIO_CREATE_INDEX (5, 1)
#define SOC_XWR16XX_GPIO_42       GPIO_CREATE_INDEX (5, 2)
#define SOC_XWR16XX_GPIO_43       GPIO_CREATE_INDEX (5, 3)
#define SOC_XWR16XX_GPIO_44       GPIO_CREATE_INDEX (5, 4)
#define SOC_XWR16XX_GPIO_45       GPIO_CREATE_INDEX (5, 5)
#define SOC_XWR16XX_GPIO_46       GPIO_CREATE_INDEX (5, 6)
#define SOC_XWR16XX_GPIO_47       GPIO_CREATE_INDEX (5, 7)

#endif /* GPIO_XWR16XX_H */

