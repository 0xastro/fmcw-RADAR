/*
 *   @file  spi_xwr14xx.c
 *
 *   @brief
 *      XWR14xx SPI configuration: This file is provided as a sample
 *      file but it can be customized by application developers to
 *      meet their application requirements.
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

#include <stdint.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/spi/SPI.h>
#include <ti/drivers/spi/include/mibspi.h>

/**************************************************************************
 ************************* Extern Declarations ****************************
 **************************************************************************/

/**
 * @brief   This is the SPI Driver registered function table
 */
extern SPI_FxnTable gSpiFxnTable;

/**************************************************************************
 ************************** Global Variables ******************************
 **************************************************************************/
/**
 * @brief   This is XWR14xx SPI specific configuration and should *NOT* be modified
 * by the customer.
 */
MibSpi_HwCfg gSpiHwCfg[1] =
{
    {
        /* SPI Hardware configuration: */
        ((MIBSPIRegs *)SOC_XWR14XX_MSS_MIBSPIA_BASE_ADDRESS),
        ((MIBSPIRam *)SOC_XWR14XX_MSS_MIBSPIA_RAM_BASE_ADDRESS),
        MSS_SYS_VCLK,
        SOC_XWR14XX_MSS_SPIA_LVL0_INT,
        SOC_XWR14XX_MSS_SPIA_LVL1_INT,
        /* XWR14xx MibSPI-A DMA request lines */
        {
            {
                SOC_XWR14XX_MSS_SPIA_CH0_DMA_REQ,
                SOC_XWR14XX_MSS_SPIA_CH1_DMA_REQ
            },
            {
                MIBSPI_INVALID_DMA_REQLINE,
                MIBSPI_INVALID_DMA_REQLINE
            },
            {
                MIBSPI_INVALID_DMA_REQLINE,
                MIBSPI_INVALID_DMA_REQLINE
    }
        }
    }
};

/**
 * @brief   The XWR14xx Platform has 1 MIBSPI. Application
 * developers can modify the configuration definition below as per 
 * requirements.
 */
SPI_Config SPI_config[2] =
{
    /* MibSPIA driver config */
    {
        &gSpiFxnTable,                  /* SPI Driver Function Table            */
        (void *)NULL,                   /* SPI Driver Object */
        (void *)&gSpiHwCfg[0]           /* SPI Hw configuration */
    },

    /* Dummy SPI driver config */  
    {
        NULL,      /* SPI Driver Function Table            */
        NULL,      /* SPI Driver Object */
        NULL       /* SPI Hw configuration */
    }
};

