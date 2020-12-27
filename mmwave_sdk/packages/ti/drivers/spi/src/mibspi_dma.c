/*
 *   @file  mibspi_dma.c
 *
 *   @brief
 *      The file implements MIBSPI Driver with DMA.
 *
 *  The ti/drivers/spi/include/reg_mibspi.h has the register layer definitons for the
 *  MIBSPI Module.
 *
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

/* Include Files */
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/osal/HwiP.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/spi/SPI.h>
#include <ti/drivers/spi/include/mibspi.h>
#include <ti/drivers/dma/dma.h>

/**************************************************************************
 ************************** Local Definitions **********************************
 **************************************************************************/
 /* Flag to enalbe Params check */
#define SPI_PARAMS_CHECK

/* Support for multi icount in one transfer to achieve high throughput , this is only supported in blocking mode */
/* #define SPI_MULT_ICOUNT_SUPPORT */

/**************************************************************************
 ************************* MibSPI Driver Functions Prototype ********************
 **************************************************************************/
/* Registered Driver  Functions: */
static void MIBSPI_init( SPI_Handle handle);
static SPI_Handle MIBSPI_open(SPI_Handle handle, SPI_Params *params);
static void MIBSPI_close(SPI_Handle handle);
static int32_t MIBSPI_control(SPI_Handle handle, uint32_t cmd, void *arg);
static int32_t MIBSPI_getStats(SPI_Handle handle, SPI_Stats *ptrStats);
static bool MIBSPI_transfer(SPI_Handle handle, SPI_Transaction *transaction);
static void MIBSPI_transferCancel(SPI_Handle handle);
static void MIBSPI_serviceISR(SPI_Handle handle);

/* Local Driver Functions */
#ifdef SPI_PARAMS_CHECK
static int32_t MIBSPI_validateParams(SPI_Params *params);
#endif

/* System DMA related function prototype */
static void MIBSPI_sysDmaIntHandler(uint32_t dmaChannel, DMA_IntType dmaIntType, void *arg);
static void MIBSPI_sysDmaSetChanParams
(
    MibSpiDriver_Object *ptrMibSpiDriver,
    uint32_t dmaChan,
    uint32_t sAddr,
    uint32_t dAddr,
    uint16_t elemCnt,
    uint16_t frameCnt,
    MibSpi_transferType txRxConfig
);
static uint32_t MIBSPI_sysDmaChanInit(MibSpiDriver_Object *ptrMibSpiDriver, uint8_t dmaChan, uint8_t dmaReqline, int32_t *errCode);
static int32_t MIBSPI_sysDmaChanDeInit(MibSpiDriver_Object *ptrMibSpiDriver, uint32_t dmaChan, uint32_t dmaReqline);

static void MIBSPI_initSlave(MibSpi_HwCfg* ptrHwCfg, const SPI_Params *params);
static void MIBSPI_initMaster(MibSpi_HwCfg* ptrHwCfg, const SPI_Params *params);
static int32_t MIBSPI_openSlaveMode(MibSpiDriver_Object *ptrMibSpiDriver, MibSpi_HwCfg* ptrHwCfg, SPI_Params *params);
static int32_t MIBSPI_openMasterMode(MibSpiDriver_Object *ptrMibSpiDriver, MibSpi_HwCfg* ptrHwCfg, SPI_Params *params);
static void MIBSPI_enablePinSettings(MIBSPIRegs  *ptrMibSpiReg, SPI_PinMode pinMode, uint8_t chipSelectMask);
static void MIBSPI_setMasterClockRate(MIBSPIRegs *ptrMibSpiReg, uint32_t clockSrcFreq, uint32_t desiredSpiClock);
static void MIBSPI_setResetMode(MIBSPIRegs *ptrMibSpiReg, bool reset);
static void MIBSPI_writeDataRAM(MibSpiDriver_Object *ptrMibSpiDriver, uint8_t group, uint16_t *data, uint16_t dataElem);
static uint32_t MIBSPI_readDataRAM(MibSpiDriver_Object *ptrMibSpiDriver, uint8_t group, uint16_t *data, uint16_t dataElem);
static void MIBSPI_transferGroupEnable(MIBSPIRegs *ptrMibSpiReg, uint8_t group);
static void MIBSPI_transferGroupDisable(MIBSPIRegs *ptrMibSpiReg, uint8_t group);
static void MIBSPI_transferSetPStart(MIBSPIRegs *ptrMibSpiReg, uint8_t group, uint8_t offset);
static uint32_t MIBSPI_checkTGComplete(MIBSPIRegs *ptrMibSpiReg, uint8_t group);
static void MIBSPI_enableGroupInterrupt(MIBSPIRegs *ptrMibSpiReg, uint8_t group, uint32_t intLine);
static void MIBSPI_disableGroupInterrupt(MIBSPIRegs *ptrMibSpiReg, uint8_t group);
static void MIBSPI_enableLoopback(MIBSPIRegs *ptrMibSpiReg, MibSpi_LoopBackType loopbacktype);
static void MIBSPI_disableLoopback(MIBSPIRegs *ptrMibSpiReg);
static void MIBSPI_setClockPhasePolarity(volatile MIBSPIRegs *ptrMibSpiReg, uint8_t clockFmt);
static void MIBSPI_dmaCtrlGroupEnable(MIBSPIRegs *ptrMibSpiReg, uint16_t bufId, uint8_t iCount, uint8_t dmaCtrlGroup);
static void MIBSPI_dmaCtrlGroupDisable(MIBSPIRegs *ptrMibSpiReg, uint8_t dmaCtrlGroup);
static void MIBSPI_SPIEnable(MIBSPIRegs *ptrMibSpiReg);
static void MIBSPI_SPIDisable(MIBSPIRegs *ptrMibSpiReg);
static void MIBSPI_dataTransfer
(
    MibSpiDriver_Object *ptrMibSpiDriver,
    uint8_t *srcData,
    uint8_t *dstData,
    uint16_t dataElemSize,
    uint8_t group
);

static void MIBSPI_ISR(uintptr_t arg);

/**************************************************************************
 ************************** Global Variables **********************************
 **************************************************************************/
/**
 * @brief   mibSPI Driver Function Table
 */
SPI_FxnTable gSpiFxnTable =
{
    &MIBSPI_close,
    &MIBSPI_control,
    &MIBSPI_init,
    &MIBSPI_open,
    &MIBSPI_transfer,
    &MIBSPI_transferCancel,
    &MIBSPI_serviceISR,
    &MIBSPI_getStats
};

/**************************************************************************
 ************************* MibSPI Driver Local Functions ************************
 **************************************************************************/

#ifdef SPI_PARAMS_CHECK
/**
 *  @b Description
 *  @n
 *       SPI driver parameter validation
 *
 *   @param[in] params             SPI driver parameters
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    Successful          =0
 *             Failed              <0
 */
static int32_t MIBSPI_validateParams(SPI_Params *params)
{
    int32_t     retVal = (int32_t)0;
    uint8_t     index;
    uint8_t     ramBufOffset = 0U;

    /* Validate dataSize, only 8bits and 16bits are supported */
    if( (params->dataSize != 8U) && (params->dataSize != 16U))
    {
        retVal = MINUS_ONE;
        goto Exit;
    }


    /* Validate bitRate */
    if(params->mode == SPI_MASTER)
    {
        if(params->u.masterParams.bitRate == 0U)
        {
            retVal = MINUS_ONE;
            goto Exit;

        }
        if( (params->u.masterParams.numSlaves == 0U) || (params->u.masterParams.numSlaves > MIBSPI_SLAVE_MAX) )
        {
            retVal = MINUS_ONE;
            goto Exit;
        }

        /* Validate slave profile configuraiton */
        for(index = 0; index < params->u.masterParams.numSlaves; index++)
        {
            SPI_SlaveProfile    *ptrSlaveProf;

            /* Get the pointer to the slave profile */
            ptrSlaveProf = &params->u.masterParams.slaveProf[index];

            /* Validate CS signal number */
            if(ptrSlaveProf->chipSelect >= MIBSPI_MAX_CS)
            {
                retVal = MINUS_ONE;
                goto Exit;
            }

            if(ptrSlaveProf->ramBufLen > MIBSPI_RAM_MAX_ELEM)
            {
                retVal = MINUS_ONE;
                goto Exit;
            }

            ramBufOffset += ptrSlaveProf->ramBufLen;

        }

        /* Validate total RAM Elements exceed the size of MibSPI RAM */
        if(ramBufOffset > MIBSPI_RAM_MAX_ELEM)
        {
            retVal = MINUS_ONE;
            goto Exit;
        }
    }
    else
    {
        /* Validate CS signal number */
        if(params->u.slaveParams.chipSelect >= MIBSPI_MAX_CS)
        {
            retVal = MINUS_ONE;
            goto Exit;
        }
    }

    /* Validate DMA driver handle */
    if(params->dmaEnable == (uint8_t)1U)
    {
        if(params->dmaHandle == NULL)
        {
            retVal = MINUS_ONE;
        }
    }
Exit:
    return retVal;
}
#endif
/**
 *  @b Description
 *  @n
 *      This function handles the DMA interrupt which is generated due to MibSpi
 *
 *   @param[in] dmaChannel   DMA channel
 *   @param[in] dmaIntType   DMA interrupt type FTC/BTC
 *   @param[in] arg          DMA interrupt argument
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    None.
 */
static void MIBSPI_sysDmaIntHandler(uint32_t dmaChannel, DMA_IntType dmaIntType, void *arg)
{
    MibSpiDriver_Object    *ptrMibSpiDriver;
    uint8_t                slaveIndex;

    /* Get MibSpi Register Base address */
    ptrMibSpiDriver = (MibSpiDriver_Object *)arg;

    switch(dmaIntType)
    {
        case DMA_IntType_FTC:
        case DMA_IntType_BTC:
            if(ptrMibSpiDriver->params.mode == SPI_MASTER)
            {
                slaveIndex = ptrMibSpiDriver->transaction->slaveIndex;
            }
            else
            {
                /* In Slave mode, only 0 is used */
                slaveIndex = (uint8_t)MIBSPI_SLAVEMODE_TRANS_GROUP;
            }

            if(dmaChannel == ptrMibSpiDriver->dmaInfo[slaveIndex].dmaChannelTx)
            {
                ptrMibSpiDriver->dmaInfo[slaveIndex].txDmaIntCnt++;
            }
            else if(dmaChannel == ptrMibSpiDriver->dmaInfo[slaveIndex].dmaChannelRx)
            {
                ptrMibSpiDriver->dmaInfo[slaveIndex].rxDmaIntCnt++;

                if ((ptrMibSpiDriver->params.transferMode == SPI_MODE_BLOCKING) && (ptrMibSpiDriver->transferCompleteSem != NULL) )
                {
                    /* MibSPI-RX DMA complete interrupt */
                    SemaphoreP_post(ptrMibSpiDriver->transferCompleteSem);
                }
                else
                {
                    /* Error: condition should not happen */
                    DebugP_assert(0);
                }
            }
            else
            {
                /* Error: unmatching dma channel */
                DebugP_assert(0);
            }

            break;
        case DMA_IntType_HBC:
        case DMA_IntType_LFS:
        default:
            DebugP_assert(0);
            break;
    }
    return ;
}

/**
 *  @b Description
 *  @n
 *      This function handles the DMA channel initialization for both TX/RX
 *
 *   @param[in] ptrMibSpiDriver  MibSpi driver handle
 *   @param[in] dmaChan          System DMA channel number
 *   @param[in] dmaReqline       System DMA request line
 *   @param[out] errCode         Error code to be populated by the driver
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    Successful        System DMA channel number
 *                 Failed        DMA_CHANNEL_NONE
 */
static uint32_t MIBSPI_sysDmaChanInit(MibSpiDriver_Object *ptrMibSpiDriver, uint8_t dmaChan, uint8_t dmaReqline, int32_t *errCode)
{
    uint32_t    dmaChanHandle = DMA_CHANNEL_NONE;

    /* Assigning dma request */
    dmaChanHandle = DMA_assignChannel(ptrMibSpiDriver->params.dmaHandle,
                                       (uint32_t)dmaChan,
                                       (uint32_t)dmaReqline,
                                       errCode);

    if(dmaChanHandle != DMA_CHANNEL_NONE)
    {
        /* MibSPI DMA transfer - enable Block Transfer Complete interrupt */
        *errCode = DMA_enableInterrupt(ptrMibSpiDriver->params.dmaHandle,
                                        dmaChanHandle,
                                        DMA_IntType_BTC,
                                        MIBSPI_sysDmaIntHandler,
                                        (void *)ptrMibSpiDriver);
        if(*errCode == 0)
        {
            /* MibSPI DMA transfer - setting the dma channel to trigger on h/w request */
            *errCode = DMA_enableChannel(ptrMibSpiDriver->params.dmaHandle,
                                           dmaChanHandle,
                                           DMA_ChTriggerType_HW);
        }
    }

    if(*errCode != 0)
    {
        dmaChanHandle = DMA_CHANNEL_NONE;
    }

    return dmaChanHandle;
}

/**
 *  @b Description
 *  @n
 *      This function de-initialize the system DMA
 *
 *   @param[in] ptrMibSpiDriver  MibSpi driver handle
 *   @param[in] dmaChan          System DMA channel number
 *   @param[in] dmaReqline       System DMA request line
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    Successful        =0
 *                 Failed        <0 return error code from system DMA module
 */
static int32_t MIBSPI_sysDmaChanDeInit(MibSpiDriver_Object *ptrMibSpiDriver, uint32_t dmaChan, uint32_t dmaReqline)
{
    int32_t  retVal = 0;

    /* Disable MibSPI DMA channel interrupt for BTC */
    retVal = DMA_disableInterrupt(ptrMibSpiDriver->params.dmaHandle, dmaChan, DMA_IntType_BTC);
    if(retVal == 0)
    {
        retVal = DMA_disableChannel(ptrMibSpiDriver->params.dmaHandle, dmaChan, DMA_ChTriggerType_HW);
        if(retVal == 0)
        {
            /* Free DMA channel */
            retVal = DMA_freeChannel(ptrMibSpiDriver->params.dmaHandle, dmaChan, dmaReqline);
        }
        else
        {
            DebugP_assert(0);
        }
    }
    else
    {
        DebugP_assert(0);
    }

    return retVal;
}

/**
 *  @b Description
 *  @n
 *      This function configures system DMA Control packet for data transfer between user buffer and MibSPI RAM
 *
 *   @param[in] ptrMibSpiDriver  MibSpi driver Handle
 *   @param[in] dmaChan          DMA channel number
 *   @param[in] sAddr            Source buffer address
 *   @param[in] dAddr            Destination buffer address
 *   @param[in] elemCnt          Element count
 *   @param[in] frameCnt         Frame count
 *   @param[in] txRxConfig       Tx/Rx Config option
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    None.
 */
static void MIBSPI_sysDmaSetChanParams
(
    MibSpiDriver_Object *ptrMibSpiDriver,
    uint32_t dmaChan,
    uint32_t sAddr,
    uint32_t dAddr,
    uint16_t elemCnt,
    uint16_t frameCnt,
    MibSpi_transferType txRxConfig
)
{
    DMA_CtrlPktParams     ctrlPacket ={0U};

    ctrlPacket.srcAddr         = sAddr;
    ctrlPacket.destAddr        = dAddr;
    ctrlPacket.nextChannel     = DMA_CHANNEL_NONE;
    ctrlPacket.autoInitiation  = 1U;

    /* Always transer as frame */
    ctrlPacket.xferType         = DMA_XferType_Frame;
    ctrlPacket.frameXferCnt     = frameCnt;
    ctrlPacket.elemXferCnt      = elemCnt;

    /* Setup DMA frame params */
    ctrlPacket.destFrameIndexOffset = 0U;
    ctrlPacket.srcFrameIndexOffset = 0U;

    if(ptrMibSpiDriver->params.dataSize == 8U)
    {
        ctrlPacket.srcElemSize      = DMA_ElemSize_8bit;
        ctrlPacket.destElemSize     = DMA_ElemSize_8bit;
    }
    else
    {
        ctrlPacket.srcElemSize      = DMA_ElemSize_16bit;
        ctrlPacket.destElemSize     = DMA_ElemSize_16bit;
    }

    /* Setup DMA address mode and offset */
    switch(txRxConfig)
    {
        case MIBSPI_TRANSFER_TX:
        case MIBSPI_TRANSFER_DUMMY_TX:
            /* Tx RAM elements are offset by 4 bytes */
            ctrlPacket.destElemIndexOffset = 4U;
            ctrlPacket.srcElemIndexOffset = 0U;
            ctrlPacket.srcAddrMode = (txRxConfig == MIBSPI_TRANSFER_TX) ? DMA_AddrMode_PostIncrement : DMA_AddrMode_Constant;

            ctrlPacket.destAddrMode = DMA_AddrMode_Indexed;

            /* Setting dma control packets */
            DMA_setChannelParams(ptrMibSpiDriver->params.dmaHandle, dmaChan, &ctrlPacket);
            break;

        case MIBSPI_TRANSFER_RX:
        case MIBSPI_TRANSFER_DUMMY_RX:
            /* Rx RAM elements are offset by 4 bytes */
            ctrlPacket.destElemIndexOffset  = 0U;
            ctrlPacket.srcElemIndexOffset   = 4U;
            ctrlPacket.srcAddrMode          = DMA_AddrMode_Indexed;
            ctrlPacket.destAddrMode = (txRxConfig == MIBSPI_TRANSFER_RX) ? DMA_AddrMode_PostIncrement : DMA_AddrMode_Constant;

            /* Setting dma control packets */
            DMA_setChannelParams(ptrMibSpiDriver->params.dmaHandle, dmaChan, &ctrlPacket);
            break;

        default:
            DebugP_assert(0);
            break;
    }
}

/**
 *  @b Description
 *  @n
 *       Enable pin settings for SPI driver
 *
 *   @param[in] ptrMibSpiReg             Pointer to the SPI driver Register Base Address
 *   @param[in] pinMode                  Pin mode Configuration
 *   @param[in] chipSelectMask           Bit Mask for enabled Chip select
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    Not Applicable
 */
static void MIBSPI_enablePinSettings(MIBSPIRegs  *ptrMibSpiReg, SPI_PinMode pinMode, uint8_t chipSelectMask)
{
    switch(pinMode)
    {
        /* 3pin setting, enable CLK, SIMO, SOMI */
        case SPI_PINMODE_3PIN:
            /* SPIPC0 Register: Set Port pins to functional */
            ptrMibSpiReg->SPIPC0 =  CSL_FMKR(9U, 9U, 1U) |        /* enable CLK */
                                 CSL_FMKR(10U, 10U, 1U) |      /* enable SIMO */
                                 CSL_FMKR(11U, 11U, 1U);       /* enable SOMI */
            break;

        /* 4pin with CS setting, enable CLK, SIMO, SOMI and CSx */
        case SPI_PINMODE_4PIN_CS:
            /* SPIPC0 Register: Set Port pins to functional */
            ptrMibSpiReg->SPIPC0 = CSL_FMKR(7U, 0U, (uint32_t)chipSelectMask) | /* enable CSx */
                                CSL_FMKR(9U, 9U, 1U) |          /* enable CLK */
                                CSL_FMKR(10U, 10U, 1U) |        /* enable SIMO */
                                CSL_FMKR(11U, 11U, 1U);         /* enable SOMI */
            break;

        default:
            DebugP_assert(0);
            break;
    }
    
    /*  SPIPC7 Register: Set Port Pullup/Pulldown control: 0 to enable, 1 to disable  */
    ptrMibSpiReg->SPIPC7 = 0U;

    /*  SPIPC8 Register: Set Port Pullup/Pulldown value: 0 to pulldown, 1 to pullup  */
    ptrMibSpiReg->SPIPC8 =  CSL_FMKR(0U, 7U, 0xFFU) |
                         CSL_FMKR(8U, 8U, 1U) |
                         CSL_FMKR(9U, 9U, 1U) |
                         CSL_FMKR(10U, 10U, 1U) |
                         CSL_FMKR(11U, 11U, 1U);
    return;
}

/**
 *  @b Description
 *  @n
 *       Change SPI master clock prescaler
 *
 *   @param[in] ptrMibSpiReg            Pointer to the SPI driver Register Base Address
 *   @param[in] clockSrcFreq            SPI Module source clock frequency
 *   @param[in] desiredSpiClock         Desired SPI clock rate on CLOCK pin
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void MIBSPI_setMasterClockRate(MIBSPIRegs *ptrMibSpiReg, uint32_t clockSrcFreq, uint32_t desiredSpiClock)
{
    uint8_t     clockDivisor;

    clockDivisor = (uint8_t)(clockSrcFreq / desiredSpiClock);

    /* Put MibSpi module in reset */
    ptrMibSpiReg->SPIGCR1 = CSL_FINSR(ptrMibSpiReg->SPIGCR1, 24U, 24U, 0U);

    /* Set MibSpi clockDivisor */
    ptrMibSpiReg->SPIFMT0 = CSL_FINSR(ptrMibSpiReg->SPIFMT0, 15U, 8U, (uint32_t)((uint32_t)clockDivisor - 1U));

    /* Finally start MIBSPI1 */
    ptrMibSpiReg->SPIGCR1 = CSL_FINSR(ptrMibSpiReg->SPIGCR1, 24U, 24U, 1U);
}

/**
 *  @b Description
 *  @n
 *       This function brings Mibspi module out of reset.
 *
 *   @param[in] ptrMibSpiReg    Pointer to the SPI driver Register Base Address
 *   @param[in] reset           true - mibspi module will be set in reset mode.
 *                              false - it brings MibSPI out of reset mode.
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void MIBSPI_setResetMode(MIBSPIRegs *ptrMibSpiReg, bool reset)
{
    if(reset == true)
    {
        /* Set MibSpi in Reset mode */
        ptrMibSpiReg->SPIGCR0 = CSL_FINSR(ptrMibSpiReg->SPIGCR0, 0U, 0U, 0U);
    }
    else
    {

        /* Bring MibSpi out of Reset mode */
        ptrMibSpiReg->SPIGCR0 = CSL_FINSR(ptrMibSpiReg->SPIGCR0, 0U, 0U, 1U);
    }
}

/**
 *  @b Description
 *  @n
 *      This function Enabe SPI and set SPI in MibSPI mode.
 *
 *   @param[in] ptrMibSpiReg        SPI module base address
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void MIBSPI_SPIEnable(MIBSPIRegs *ptrMibSpiReg)
{
    /* Acitvate SPI */
    ptrMibSpiReg->SPIGCR1 = CSL_FINSR(ptrMibSpiReg->SPIGCR1, 24U, 24U, 1U);

    /* Enable MibSpi multibuffered mode and enable buffer RAM */
    ptrMibSpiReg->MIBSPIE = CSL_FINSR (ptrMibSpiReg->MIBSPIE, 0U, 0U, 1U);
}

/**
 *  @b Description
 *  @n
 *      This function Disable SPI and set SPI in SPI mode.
 *
 *   @param[in] ptrMibSpiReg        SPI module base address
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void MIBSPI_SPIDisable(MIBSPIRegs *ptrMibSpiReg)
{
    /* Disable MibSpi multibuffered mode and enable buffer RAM */
    ptrMibSpiReg->MIBSPIE = CSL_FINSR (ptrMibSpiReg->MIBSPIE, 0U, 0U, 0U);

    /* De-acitvate SPI */
    ptrMibSpiReg->SPIGCR1 = CSL_FINSR(ptrMibSpiReg->SPIGCR1, 24U, 24U, 0U);
}

/**
 *  @b Description
 *  @n
 *      This function initiates a transfer for the specified transfer group.
 *
*   @param[in] ptrMibSpiReg        SPI module base address
*   @param[in] group               Transfer group (0..7)
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void MIBSPI_transferGroupEnable(MIBSPIRegs *ptrMibSpiReg, uint8_t group)
{
    /* Enable Transfer group */
    ptrMibSpiReg->TGCTRL[group] = CSL_FINSR(ptrMibSpiReg->TGCTRL[group], 31U, 31U, 1U);
}


/**
 *  @b Description
 *  @n
 *      This function initiates a transfer for the specified transfer group.
 *
 *   @param[in] ptrMibSpiReg        SPI module base address
 *   @param[in] group               Transfer group (0..7)
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void MIBSPI_transferGroupDisable(MIBSPIRegs *ptrMibSpiReg, uint8_t group)
{
    /* Disable Transfer group */
    ptrMibSpiReg->TGCTRL[group] = CSL_FINSR(ptrMibSpiReg->TGCTRL[group], 31U, 31U, 0U);

    /* Transfer is completed , disable SPI */
    MIBSPI_SPIDisable(ptrMibSpiReg);
}

/**
 *  @b Description
 *  @n
 *      This function sets the start offset of a transfer for the specified transfer group.
 *
 *   @param[in] ptrMibSpiReg       SPI module base address
 *   @param[in] group              Transfer group
 *   @param[in] offset             RAM offset For the TG group
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void MIBSPI_transferSetPStart(MIBSPIRegs *ptrMibSpiReg, uint8_t group, uint8_t offset)
{
    ptrMibSpiReg->TGCTRL[group] = CSL_FINSR(ptrMibSpiReg->TGCTRL[group], 15U, 8U, (uint32_t)offset);
}

/**
 *  @b Description
 *  @n
 *      This function checks to see if the transfer for the specified transfer group
 *   has finished.
 *
 *   @param[in] ptrMibSpiReg       SPI module base address
 *   @param[in] group              Transfer group
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    =1     Transfer is finished.
 *             =0     Transfer is not finished.
 */
static uint32_t MIBSPI_checkTGComplete(MIBSPIRegs *ptrMibSpiReg, uint8_t group)
{
    uint32_t status = 0U;
    uint32_t groupMask;
    uint32_t intFlagReady = 0U;

    /* Get the bit mask of the group */
    groupMask = (uint32_t)1U << group;

    /* Read TGINT flag */
    intFlagReady = CSL_FEXTR(ptrMibSpiReg->TGINTFLAG, 31U, 16U);
    if ( intFlagReady & groupMask)
    {
        /* Transfer finished, clear the corresponding flag */
        ptrMibSpiReg->TGINTFLAG = CSL_FINSR(ptrMibSpiReg->TGINTFLAG, 31U, 16U, groupMask);
        status = 1U;
    }

    return (status);
}

/**
 *  @b Description
 *  @n
 *      This function enables the Loopback mode for self test. Loopback is SPI master only feature.
 *
 *   @param[in] ptrMibSpiReg            Mibspi module base address
 *   @param[in] loopbacktype            Digital or Analog
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    None.
 */
static void MIBSPI_enableLoopback(MIBSPIRegs *ptrMibSpiReg, MibSpi_LoopBackType loopbacktype)
{
    /* Put MibSpi module in reset */
    ptrMibSpiReg->SPIGCR1 = CSL_FINSR(ptrMibSpiReg->SPIGCR1, 24U, 24U, 0U);

    /* Set Loopback either in Analog or Digital Mode */
    ptrMibSpiReg->IOLPBKTSTCR = CSL_FINSR(ptrMibSpiReg->IOLPBKTSTCR, 1U, 1U, loopbacktype);

    /* Enable Loopback  */
    ptrMibSpiReg->IOLPBKTSTCR = CSL_FINSR(ptrMibSpiReg->IOLPBKTSTCR, 11U, 8U, 0xAU);

    /* Restart MIBSPI1 */
    ptrMibSpiReg->SPIGCR1 = CSL_FINSR(ptrMibSpiReg->SPIGCR1, 24U, 24U, 1U);

    ptrMibSpiReg->SPIGCR1 = CSL_FINSR(ptrMibSpiReg->SPIGCR1, 16U, 16U, 1U);
}

/**
 *  @b Description
 *  @n
 *      This function disable the Loopback mode.
 *
 *   @param[in] ptrMibSpiReg        Mibspi module base address
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    None.
 */
static void MIBSPI_disableLoopback(MIBSPIRegs *ptrMibSpiReg)
{
    /* Put MibSpi module in reset */
    ptrMibSpiReg->SPIGCR1 = CSL_FINSR(ptrMibSpiReg->SPIGCR1, 24U, 24U, 0U);

    /* Disable Loopback either in Analog or Digital Mode */
    ptrMibSpiReg->IOLPBKTSTCR = CSL_FINSR(ptrMibSpiReg->IOLPBKTSTCR, 11U, 8U, 0x5U);

    /* Restart MIBSPI1 */
    ptrMibSpiReg->SPIGCR1 = CSL_FINSR(ptrMibSpiReg->SPIGCR1, 24U, 24U, 1U);
}

/**
 *  @b Description
 *  @n
 *      This function sets the Polarity and phase for MibSpi as requested.
 *
 *   @param[in] ptrMibSpiReg      MibSpi register base address
 *   @param[in] clockFmt          MibSpiSPI functional mode (clock/polarity)
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    None.
 */
static void MIBSPI_setClockPhasePolarity(volatile MIBSPIRegs *ptrMibSpiReg, uint8_t clockFmt)
{
    /* Put MibSpi module in reset */
    ptrMibSpiReg->SPIGCR1 = CSL_FINSR(ptrMibSpiReg->SPIGCR1, 24U, 24U, 0U);

    /* Set MibSpi Slave functional Mode Clock/polarity */
    ptrMibSpiReg->SPIFMT0 = CSL_FINSR(ptrMibSpiReg->SPIFMT0, 17U, 16U, (uint32_t)clockFmt );

    /* Finally start MIBSPI1 */
    ptrMibSpiReg->SPIGCR1 = CSL_FINSR(ptrMibSpiReg->SPIGCR1, 24U, 24U, 1U);
}

/**
 *  @b Description
 *  @n
 *      This function enables the transfer group finished interrupt.
 *
 *   @param[in] ptrMibSpiReg   SPI module base address
 *   @param[in] group          Transfer group
 *   @param[in] intLine        Interrupt Line - 0->INT0, 1->INT1
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    None.
 */
static void MIBSPI_enableGroupInterrupt(MIBSPIRegs *ptrMibSpiReg, uint8_t group, uint32_t intLine)
{
    if(intLine == 0)
    {
        /* INT0 interrupt */
        ptrMibSpiReg->TGITLVCR = CSL_FINSR(ptrMibSpiReg->TGITLVCR, 31U, 16U, (uint32_t)1U << group);
    }
    else
    {
        /* INT1 interrupt */
        ptrMibSpiReg->TGITLVST = CSL_FINSR(ptrMibSpiReg->TGITLVST, 31U, 16U, (uint32_t)1U << group);
    }

    /* Enable interrupt */
    ptrMibSpiReg->TGITENST = CSL_FINSR(ptrMibSpiReg->TGITENST, 31U, 16U, (uint32_t)1U << group);
}

/**
 *  @b Description
 *  @n
 *      This function disables the transfer group finished interrupt.
 *
 *   @param[in] ptrMibSpiReg       SPI module base address
 *   @param[in] group              Transfer group
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    None.
 */
static void MIBSPI_disableGroupInterrupt(MIBSPIRegs *ptrMibSpiReg, uint8_t group)
{
    /* Disable interrupt */
    ptrMibSpiReg->TGITENCR = CSL_FINSR(ptrMibSpiReg->TGITENCR, 31U, 16U, (uint32_t)1U << group);
}

/**
 *  @b Description
 *  @n
 *      This function set the MIBSPI DMA channel Control Register depending on given
 *  input parameters
 *
 *   @param[in] ptrMibSpiReg      MibSpi Register base address
 *   @param[in] bufId             Buffer id
 *   @param[in] iCount            icount configuration
 *   @param[in] dmaCtrlGroup      MibSPI DMA Control group(0..7)
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval   None.
 */
static void MIBSPI_dmaCtrlGroupEnable(MIBSPIRegs *ptrMibSpiReg, uint16_t bufId, uint8_t iCount, uint8_t dmaCtrlGroup)
{
    /* Setting Transmit channel DMA request */
    ptrMibSpiReg->DMACTRL[dmaCtrlGroup] = CSL_FINSR(ptrMibSpiReg->DMACTRL[dmaCtrlGroup], 19U, 16U, (uint32_t)dmaCtrlGroup * 2U) |
                                           CSL_FINSR(ptrMibSpiReg->DMACTRL[dmaCtrlGroup], 14U, 14U, 1U) |
                                           CSL_FINSR(ptrMibSpiReg->DMACTRL[dmaCtrlGroup], 30U, 24U, (uint32_t)bufId) |
                                           CSL_FINSR(ptrMibSpiReg->DMACTRL[dmaCtrlGroup], 12U, 8U, (uint32_t)iCount) |
                                           CSL_FINSR(ptrMibSpiReg->DMACTRL[dmaCtrlGroup], 31U, 31U, 1U);

    /* Setting Receive channel DMA request */
    ptrMibSpiReg->DMACTRL[dmaCtrlGroup] = CSL_FINSR(ptrMibSpiReg->DMACTRL[dmaCtrlGroup], 23U, 20U, (uint32_t)dmaCtrlGroup * 2U + 1U) |
                                           CSL_FINSR(ptrMibSpiReg->DMACTRL[dmaCtrlGroup], 15U, 15U, 1U) |
                                           CSL_FINSR(ptrMibSpiReg->DMACTRL[dmaCtrlGroup], 30U, 24U, (uint32_t)bufId) |
                                           CSL_FINSR(ptrMibSpiReg->DMACTRL[dmaCtrlGroup], 12U, 8U, (uint32_t)iCount) |
                                           CSL_FINSR(ptrMibSpiReg->DMACTRL[dmaCtrlGroup], 31U, 31U, 1U);
    return;
}

/**
 *  @b Description
 *  @n
 *      This function reset the MIBSPI TX and RX DMA channel Control Register
 *
 *   @param[in] ptrMibSpiReg      MibSpi Register base address
 *   @param[in] dmaCtrlGroup      MibSPI DMA Control group(0..7)
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval   None.
 */
static void MIBSPI_dmaCtrlGroupDisable(MIBSPIRegs *ptrMibSpiReg, uint8_t dmaCtrlGroup)
{
    /* Get MibSpi Register & Ram Base address */
    ptrMibSpiReg->DMACTRL[dmaCtrlGroup] = CSL_FINSR(ptrMibSpiReg->DMACTRL[dmaCtrlGroup], 31U, 7U, 0U);
}

 /**
 *  @b Description
 *  @n
 *       Initializes the MIBSPI as a SPI master
 *
 *   @param[in] ptrHwCfg           Pointer to the SPI driver hardward Configuration
 *   @param[in] params             SPI device configuration parameters
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void MIBSPI_initMaster(MibSpi_HwCfg* ptrHwCfg, const SPI_Params *params)
{
    MIBSPIRam           *ptrMibSpiRam;
    MIBSPIRegs          *ptrMibSpiReg;
    uint32_t            flag;
    uint32_t            index;
    uint8_t             csnr = 0;
    uint8_t             chipSelectMask;
    uint8_t             ramBufOffset = 0;

    /* Get Register and RAM base */
    ptrMibSpiRam  = ptrHwCfg->ptrMibSpiRam;
    ptrMibSpiReg  = ptrHwCfg->ptrSpiRegBase;

    /* Bring MIBSPI out of reset */
    ptrMibSpiReg->SPIGCR0 = 0U;
    ptrMibSpiReg->SPIGCR0 = 1U;

    /* Enable MibSpi multibuffered mode and enable buffer RAM */
    ptrMibSpiReg->MIBSPIE = CSL_FINSR (ptrMibSpiReg->MIBSPIE, 0U, 0U, 1U);

    /* Set MibSpi master mode and clock configuration */
    ptrMibSpiReg->SPIGCR1 = CSL_FINSR (ptrMibSpiReg->SPIGCR1, 1U, 0U, 3U);

    /* SPIENV pin pulled high when not active */
    ptrMibSpiReg->SPIINT0 = CSL_FINSR (ptrMibSpiReg->SPIINT0, 24U, 24U, 0U);

    /* Delays Configuration: this master only configuraion, hence set it to all zeros */
    if( (params->u.masterParams.c2tDelay != 0U) ||
       (params->u.masterParams.t2cDelay != 0U) )
    {
        ptrMibSpiReg->SPIDELAY = CSL_FMKR(31U, 24U, params->u.masterParams.c2tDelay) |
                             CSL_FMKR(23U, 16U, params->u.masterParams.t2cDelay);
    }
    else
    {
        ptrMibSpiReg->SPIDELAY = 0x0U;
    }

    /* Set Data Format 0 */
    ptrMibSpiReg->SPIFMT0 = CSL_FMKR(17U, 16U, params->frameFormat) |   /* PHASE & POLARITY */
                          CSL_FMKR(31U, 24U, params->u.masterParams.wDelay) |   /* WDELAY */
                          CSL_FMKR(15U, 8U, 0U)  |                   /* PRESCALE */
                          CSL_FMKR(20U, 20U, params->shiftFormat) |  /* SHIFTDIR */
                          CSL_FMKR(4U, 0U, params->dataSize);        /* CHARlEN */

    /* Set Data Format 1 */
    ptrMibSpiReg->SPIFMT1 = CSL_FMKR(15U, 8U, 18U)  |                 /* PRESCALE */
                          CSL_FMKR(4U, 0U, params->dataSize);        /* CHARlEN */

    /* Set Data Format 2 */
    ptrMibSpiReg->SPIFMT2 = CSL_FMKR(15U, 8U, 18U)  |                 /* PRESCALE */
                          CSL_FMKR(4U, 0U, params->dataSize);        /* CHARlEN */

    /* Set Data Format 3 */
    ptrMibSpiReg->SPIFMT3 = CSL_FMKR(15U, 8U, 18U)  |                 /* PRESCALE */
                          CSL_FMKR(4U, 0U, params->dataSize);        /* CHARlEN */

    /* Set Default Chip Select pattern: 1- chip select is set to "1" when SPI is IDLE
       Debug Note: Only CS0 can be set to 1 */
    ptrMibSpiReg->SPIDEF = CSL_FINSR (ptrMibSpiReg->SPIDEF , 7U, 0U, 0xFFU);

    /* Wait for buffer initialization complete before accessing MibSPI Ram */
    while(1)
    {
        flag = CSL_FEXTR(ptrMibSpiReg->SPIFLG, 24U, 24U);
        if(flag == 0)
        {
            break;
        }
    }

    /* Enable ECC if enabled */
    if(params->eccEnable)
    {
        /* Enable ECC detection and signal bit Error correction */
        ptrMibSpiReg->PAR_ECC_CTRL = CSL_FMKR( 3U, 0U, 0xAU) |
                                    CSL_FMKR( 19U, 16U, 0xAU) |
                                    CSL_FMKR( 27U, 24U, 0xAU);
    }

    for (index = 0; index < params->u.masterParams.numSlaves; index++)
    {
        /* Initialize transfer groups for number of slaves connected to SPI master */
        ptrMibSpiReg->TGCTRL[index] = CSL_FMKR(30U, 30U, 1U) |    /* Oneshot trigger */
                                    CSL_FMKR(23U, 20U, 7U) |      /* Trigger event : Always */
                                    CSL_FMKR(19U, 16U, 0U) |      /* Trigger source : disabled */
                                    CSL_FMKR(15U, 8U, 0U);        /* TG start address : 0 */
    }

    /* Initialize transfer groups end pointer */
    ptrMibSpiReg->LTGPEND = CSL_FINSR(ptrMibSpiReg->LTGPEND, 15U, 8U, 0xFFU);

    /* Initialize TX Buffer Ram */
    for (index = 0; index < params->u.masterParams.numSlaves; index++)
    {
        uint8_t    ramBufIndex = 0;
        uint8_t    wDelayEnable = 0;

        /* Multibuffer RAM control:
         * buffer mode : 0x6
         * CSHOLD: 0x0
         */

        wDelayEnable = params->u.masterParams.wDelay? (uint8_t)1U : (uint8_t)0U;
        if(params->pinMode == SPI_PINMODE_4PIN_CS)
        {
            chipSelectMask = (uint8_t)(0x1U << params->u.masterParams.slaveProf[index].chipSelect);
            csnr = ~chipSelectMask;
        }
        else
        {
            /* 3-pin mode, set CSNR to 0xFF */
            csnr = (uint8_t)CS_NONE;
        }

        for (ramBufIndex = ramBufOffset; ramBufIndex < params->u.masterParams.slaveProf[index].ramBufLen; ramBufIndex++)
        {
            ptrMibSpiRam->tx[ramBufIndex].control = CSL_FMKR(15U, 13U, MIBSPI_RAM_BUFFER_MODE) |
                                                CSL_FMKR(12U, 12U, (uint32_t)params->csHold) |
                                                CSL_FMKR(10U, 10U, (uint32_t)wDelayEnable) |
                                                CSL_FMKR(7U, 0U, (uint32_t)csnr);
            ramBufOffset++;
        }

        if(ramBufOffset > MIBSPI_RAM_MAX_ELEM)
        {
            DebugP_assert(0);
        }
    }

    /* Clear pending interrupts */
    ptrMibSpiReg->SPIFLG |= 0xFFFFU;

    /* Clear pending TG interrupts */
    ptrMibSpiReg->TGINTFLAG |= 0xFFFFFFFFU;

    /* Enable Error interrupts: Lower 8bits  */
    ptrMibSpiReg->SPIINT0 = CSL_FINSR(ptrMibSpiReg->SPIINT0, 7U, 0U, 0x5FU);

    /* Set Interrupt Levels - Interrupts are mapped to INT1 */
    ptrMibSpiReg->SPILVL = CSL_FINSR(ptrMibSpiReg->SPILVL, 7U, 0U, 0x5FU);

    /* Enable TG Interrupts to INT1 */
    ptrMibSpiReg->TGITENST|= 0xFFFFFFFFU;

    /* Pin settings for SPI signal */
    if(params->pinMode == SPI_PINMODE_4PIN_CS)
    {
        chipSelectMask = (uint8_t)0U;
        for (index = 0; index < params->u.masterParams.numSlaves; index++)
        {
            chipSelectMask |= (uint8_t)(0x1U << params->u.masterParams.slaveProf[index].chipSelect);
        }
    }
    else
    {
        chipSelectMask = (uint8_t)0x0U;
    }
    MIBSPI_enablePinSettings(ptrMibSpiReg, params->pinMode, chipSelectMask);

    /* Finally start MIBSPI1 */
    ptrMibSpiReg->SPIGCR1 = CSL_FINSR(ptrMibSpiReg->SPIGCR1, 24U, 24U, 1U);
}

/**
 *  @b Description
 *  @n
 *       Initializes the MIBSPI as a SPI slave
 *
 *   @param[in] ptrHwCfg             Pointer to the SPI driver hardward Configuration
 *   @param[in] params               SPI device configuration parameters
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void MIBSPI_initSlave(MibSpi_HwCfg* ptrHwCfg, const SPI_Params *params)
{
    MIBSPIRam       *ptrMibSpiRam;
    MIBSPIRegs      *ptrMibSpiReg;
    uint32_t        flag;
    uint32_t        index;

    /* Get Register and RAM base */
    ptrMibSpiRam = ptrHwCfg->ptrMibSpiRam;
    ptrMibSpiReg = ptrHwCfg->ptrSpiRegBase;

    /* Bring MIBSPI out of reset */
    ptrMibSpiReg->SPIGCR0 = 0U;
    ptrMibSpiReg->SPIGCR0 = 1U;

    /* Enable MIBSPI1 multibuffered mode and enable buffer RAM */
    ptrMibSpiReg->MIBSPIE = CSL_FINSR (ptrMibSpiReg->MIBSPIE, 0U, 0U, 1U);

    /* MIBSPI1 slave mode and clock configuration */
    ptrMibSpiReg->SPIGCR1 = CSL_FINSR (ptrMibSpiReg->SPIGCR1, 1U, 0U, 0U);

    /* SPIENA pin pulled high when not active */
    ptrMibSpiReg->SPIINT0 = CSL_FINSR (ptrMibSpiReg->SPIINT0, 24U, 24U, 0U);

    /* Delays Configuration: this is only used by master, hence set it to all zeros */
    ptrMibSpiReg->SPIDELAY = 0x0U;

    /* Set Data Format 0 */
    ptrMibSpiReg->SPIFMT0 = CSL_FMKR(17U, 16U, (uint32_t)(params->frameFormat)) |         /* PHASE & POLARITY */
                          CSL_FMKR(15U, 8U, 4U)  |                            /* PRESCALE */
                          CSL_FMKR(20U, 20U, (uint32_t)(params->shiftFormat)) | /* SHIFTDIR */
                          CSL_FMKR(4U, 0U, params->dataSize);               /* CHARlEN */

    /* Set Data Format 1,2,3. Used mulitple TG group transfer */
    ptrMibSpiReg->SPIFMT1 = CSL_FMKR(15U, 8U, 18U)  |                         /* PRESCALE */
                          CSL_FMKR(4U, 0U, params->dataSize);               /* CHARlEN */

    ptrMibSpiReg->SPIFMT2 = CSL_FMKR(15U, 8U, 18U)  |                         /* PRESCALE */
                          CSL_FMKR(4U, 0U, params->dataSize);               /* CHARlEN */

    ptrMibSpiReg->SPIFMT3 = CSL_FMKR(15U, 8U, 18U)  |                         /* PRESCALE */
                          CSL_FMKR(4U, 0U, params->dataSize);               /* CHARlEN */

    /* Wait for buffer initialization complete before accessing MibSPI registers */
    while(1)
    {
        flag = CSL_FEXTR(ptrMibSpiReg->SPIFLG, 24U, 24U);
        if(flag == 0)
        {
            break;
        }
    }

    /* Enable ECC if enabled */
    if(params->eccEnable)
    {
        /* Enable ECC detection and signal bit Error correction */
        ptrMibSpiReg->PAR_ECC_CTRL =  CSL_FMKR( 3U, 0U, 0xAU) |
                                      CSL_FMKR( 19U, 16U, 0xAU) |
                                      CSL_FMKR( 27U, 24U, 0xAU);
    }

    for (index = 0; index < MIBSPI_NUM_TRANS_GROUP; index++)
    {
        /* Initialize transfer groups */
        ptrMibSpiReg->TGCTRL[index] = CSL_FMKR(30U, 30U, 1U) |      /* Oneshot trigger */
                                    CSL_FMKR(23U, 20U, 7U) |        /* Trigger event : Always */
                                    CSL_FMKR(19U, 16U, 0U) |        /* Trigger source : disabled */
                                    CSL_FMKR(15U, 8U, 0U);          /* TG start address : 0 */
    }

    /* Initialize transfer groups end pointer */
    ptrMibSpiReg->LTGPEND = CSL_FINSR(ptrMibSpiReg->LTGPEND, 15U, 8U, 0x0U);

    /* Initialize TX Buffer Ram, every element contains 16bits of data */
    for (index = 0; index < MIBSPI_RAM_MAX_ELEM; index++)
    {
        ptrMibSpiRam->tx[index].control = CSL_FMKR(15U, 13U, MIBSPI_RAM_BUFFER_MODE) |
                                      CSL_FMKR(12U, 12U, params->csHold) |
                                      CS_NONE;   /* CSNR in case of slave mode */
    }

    /* Clear pending interrupts */
    ptrMibSpiReg->SPIFLG |= 0xFFFFU;

    /* Clear pending TG interrupts */
    ptrMibSpiReg->TGINTFLAG |= 0xFFFFFFFFU;

    /* Enable Error interrupts: Lower 8bits  */
    ptrMibSpiReg->SPIINT0 = CSL_FINSR(ptrMibSpiReg->SPIINT0, 7U, 0U, 0x5FU);

    /* Set Interrupt Levels - Interrupts are mapped to INT0 */
    ptrMibSpiReg->SPILVL = CSL_FINSR(ptrMibSpiReg->SPILVL, 7U, 0U, 0x5FU);

    /* Enable TG Interrupts to INT1 */
    ptrMibSpiReg->TGITENST|= 0xFFFFFFFFU;

    /* Pin settings for SPI signal */
    MIBSPI_enablePinSettings(ptrMibSpiReg, params->pinMode, (uint8_t)(0x1U <<params->u.slaveParams.chipSelect));

    /* Finally start MIBSPI1 */
    ptrMibSpiReg->SPIGCR1 = CSL_FINSR(ptrMibSpiReg->SPIGCR1, 24U, 24U, 1U);
}

/**
 *  @b Description
 *  @n
 *     This function updates the data for the specified transfer group,
 *   the length of the data must match the length of the transfer group.
 *
 *   @param[in] ptrMibSpiDriver  MibSpi driver Handle
 *   @param[in] group            Transfer group (0..7)
 *   @param[in] data             Data buffer address
 *   @param[in] dataElem         Data element size
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void MIBSPI_writeDataRAM(MibSpiDriver_Object *ptrMibSpiDriver, uint8_t group, uint16_t *data, uint16_t dataElem)
{
    MIBSPIRegs      *ptrMibSpiReg;
    MIBSPIRam       *ptrMibSpiRam;
    uint32_t        start;
    uint32_t        index;

    /* Get MibSpi Register & RAM Base address */
    ptrMibSpiReg  = ptrMibSpiDriver->ptrHwCfg->ptrSpiRegBase;
    ptrMibSpiRam  = ptrMibSpiDriver->ptrHwCfg->ptrMibSpiRam;

    /* Fetch the start address from Register */
    start = CSL_FEXTR(ptrMibSpiReg->TGCTRL[group], 15U, 8U);

    /* Write data in TX RAM */
    if(data == NULL)
    {
        for(index=start; index < (start + dataElem); index++)
        {
            ptrMibSpiRam->tx[index].data = ptrMibSpiDriver->txScratchBuffer;
        }
    }
    else
    {
        if(ptrMibSpiDriver->params.dataSize == 16U)
        {
            for(index=start; index < (start + dataElem); index++)
            {
                ptrMibSpiRam->tx[index].data = data[index-start];
            }
        }
        else
        {
            uint8_t *pBuffer = (uint8_t *)data;

            for(index=start; index < (start + dataElem); index++)
            {
                ptrMibSpiRam->tx[index].data = pBuffer[index-start];
            }
        }
    }
}

/**
 *  @b Description
 *  @n
 *      This function transfers the data from the specified transfer group receive
 *   buffers to the data array,  the length of the data must match the length
 *   of the transfer group.
 *
 *   @param[in]   ptrMibSpiDriver  MibSpi driver Handle
 *   @param[in]   group            Transfer group (0..7)
 *   @param[out]  data             Pointer to data array
 *   @param[in]   dataElem         Data element size
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval return error flags from data buffer, if there was a receive error on
 *         one of the buffers this will be reflected in the return value.
 */
static uint32_t MIBSPI_readDataRAM(MibSpiDriver_Object *ptrMibSpiDriver, uint8_t group, uint16_t *data, uint16_t dataElem)
{
    MIBSPIRegs      *ptrMibSpiReg;
    MIBSPIRam       *ptrMibSpiRam;
    uint16_t        mibspiFlags  = 0U;
    uint32_t        ret;
    uint32_t        start;
    uint32_t        index;

    /* Get MibSpi Register & Ram Base address */
    ptrMibSpiReg  = ptrMibSpiDriver->ptrHwCfg->ptrSpiRegBase;
    ptrMibSpiRam  = ptrMibSpiDriver->ptrHwCfg->ptrMibSpiRam;

    /* Fetch the start address from Register */
    start = CSL_FEXTR(ptrMibSpiReg->TGCTRL[group], 15U, 8U);

    if(data == NULL)
    {
         /* Save data from RAM into scrach buffer */
         for(index=start; index < (start + dataElem); index++)
         {
             /* Wait until data is available */
             while((ptrMibSpiRam->rx[index].flags & 0x8000U) != 0U)
             {
             }

             mibspiFlags |= ptrMibSpiRam->rx[index].flags;

             ptrMibSpiDriver->rxScratchBuffer = ptrMibSpiRam->rx[index].data;
         }
    }
    else
    {

         /* Save data from RAM */
         for(index=start; index < (start + dataElem); index++)
         {

             /* Wait until data is available */
             while((ptrMibSpiRam->rx[index].flags & 0x8000U) != 0U)
             {
             }

             mibspiFlags |= ptrMibSpiRam->rx[index].flags;

             if(ptrMibSpiDriver->params.dataSize == 16U)
             {
                 data[index-start] = ptrMibSpiRam->rx[index].data;
             }
             else
             {
                 uint8_t *pBuffer = (uint8_t *)data;
             
                 pBuffer[index-start] = (uint8_t)(ptrMibSpiRam->rx[index].data & 0xFFU);
             }
         }
    }

    /* Rx Flags */
    ret = ((uint32_t)mibspiFlags >> 8U) & 0x5FU;

    return ret;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered ISR for the MIBSPI interrupt.
 *
 *  @param[in]  arg
 *      Argument which is registered with the OS while registering
 *      the ISR
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void MIBSPI_ISR (uintptr_t arg)
{
    MibSpiDriver_Object    *ptrMibSpiDriver;
    MIBSPIRegs             *ptrMibSpiReg;
    volatile uint32_t      intVector;
    uint8_t                group = 0U;

    /* Get the MibSpi driver handle */
    ptrMibSpiDriver = (MibSpiDriver_Object *)arg;

    if(ptrMibSpiDriver != (MibSpiDriver_Object *)NULL)
    {
        ptrMibSpiReg    = ptrMibSpiDriver->ptrHwCfg->ptrSpiRegBase;

        /* Handles the TG Complete Interrupt */
        intVector = ptrMibSpiReg->TGINTFLAG;
        intVector = intVector >> 16U;
        for(group = 0U ; group < MIBSPI_SLAVE_MAX; group++)
        {
            if(intVector & (0x1U << group))
            {
                ptrMibSpiDriver->hwStats.TGComplete[group]++;
                ptrMibSpiReg->TGINTFLAG = (uint32_t)0x10000U << group;

#ifdef SPI_MULT_ICOUNT_SUPPORT
                /* All transfer done ? - check icount status */
                if((ptrMibSpiReg->DMACTRL[group] & 0x3FU) != 0)
                {
                    MIBSPI_transferGroupEnable(ptrMibSpiReg, group);
              
 }
#endif
            }
        }

        /* Found out the interrupt source and increment the stats count */
        intVector = ptrMibSpiReg->SPIFLG;
        if (intVector & 0x1U)
        {
            ptrMibSpiDriver->hwStats.dlenErr++;
        }
        if (intVector & 0x2U)
        {
            ptrMibSpiDriver->hwStats.timeout++;
        }
        if (intVector & 0x4U)
        {
            ptrMibSpiDriver->hwStats.parErr++;
        }
        if (intVector & 0x8U)
        {
            /* Master only interrupt */
            ptrMibSpiDriver->hwStats.desync++;
        }
        if (intVector & 0x10U)
        {
            /* Master only interrupt */
            ptrMibSpiDriver->hwStats.bitErr++;
        }
        if (intVector & 0x40U)
        {
            /* Master only interrupt */
            ptrMibSpiDriver->hwStats.rxOvrnErr++;
        }
        if (intVector & 0x100U)
        {
            /* Master only interrupt */
            ptrMibSpiDriver->hwStats.rxFull++;
        }
        if (intVector & 0x200U)
        {
            /* Master only interrupt */
            ptrMibSpiDriver->hwStats.txEmpty++;
        }

        ptrMibSpiReg->SPIFLG = intVector;
    }

    return;
}

/**
 *  @b Description
 *  @n
 *      This function takes source and destination address and invoke MibSPI to transfer it over DMA/CPU
 *
 *   @param[in] ptrMibSpiDriver   MibSpi driver handle
 *   @param[in] srcData           Input buffer pointer to send the Data from
 *   @param[in] dstData           Output buffer pointer to get the Data
 *   @param[in] dataElemSize      Data Elements size
 *   @param[in] group             MibSPI transfer group used for this transfer
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    Successful         =0
 *                 Failed         <0
 */
static void MIBSPI_dataTransfer
(
    MibSpiDriver_Object *ptrMibSpiDriver,
    uint8_t *srcData,
    uint8_t *dstData,
    uint16_t dataElemSize,
    uint8_t group
)
{
    MibSpi_HwCfg    *ptrHwCfg;
    MIBSPIRegs      *ptrMibSpiReg;
    MIBSPIRam       *ptrMibSpiRam;
    uint8_t         ramOffset = 0U;
    uint16_t        bufId = 0U;
    uint8_t         iCount = 0U;

    /* Get MibSpi driver hardware config */
    ptrHwCfg = ptrMibSpiDriver->ptrHwCfg;

    /* Get MibSpi Register & Ram Base address */
    ptrMibSpiReg  = ptrHwCfg->ptrSpiRegBase;
    ptrMibSpiRam  = ptrHwCfg->ptrMibSpiRam;

    /* Put SPI in active mode */
    MIBSPI_SPIEnable(ptrMibSpiReg);

    /* Find out bufId and RAM offset */
    if(ptrMibSpiDriver->params.mode == SPI_SLAVE)
    {
        ramOffset = 0U;

        /* Find out iCount and bufid */
        if (dataElemSize > MIBSPI_RAM_MAX_ELEM)
        {
            /* (iCount + 1) transfer of size MIBSPI_RAM_MAX_ELEM */
            iCount = (uint8_t)(dataElemSize / MIBSPI_RAM_MAX_ELEM - 1U);
            bufId = (uint16_t)(MIBSPI_RAM_MAX_ELEM -1U);
        }
        else
        {
            /* One transfer of dataElemSize */
            iCount = (uint8_t)0U;
            bufId = (uint16_t)(dataElemSize -1U);
        }
    }
    else
    {
        uint8_t    ramLen = 0;

        ramOffset = ptrMibSpiDriver->rambufStart[group];
        ramLen = ptrMibSpiDriver->params.u.masterParams.slaveProf[group].ramBufLen;

        /* Find out iCound and bufid */
        if (dataElemSize > ramLen)
        {
            /* (iCount + 1) transfer of size ramLen */
            iCount = (uint8_t)(dataElemSize / ramLen - 1U);
            bufId = (uint16_t)ramOffset + (uint16_t)ramLen -(uint16_t)1U;
        }
        else
        {
            /* One transfer of dataElemSize */
            iCount = 0;
            bufId = (uint16_t)ramOffset + dataElemSize - (uint16_t)1U;
        }
    }

    /* Initialize transfer group start offset */
    MIBSPI_transferSetPStart(ptrMibSpiReg, group, ramOffset);

    /* This is only needed for XWR16xx/XWR18xx/XWR68xx device */
    MIBSPI_transferSetPStart(ptrMibSpiReg, (uint8_t)(group + 1U), (uint8_t)(bufId + 1U));

    /* Initialize transfer groups end pointer */
    ptrMibSpiReg->LTGPEND = CSL_FINSR(ptrMibSpiReg->LTGPEND, 15U, 8U, (uint32_t)bufId);

     /* Configure DMA in the following 3 cases
         Case 1: SrcData=NULL, DstData!=NULL   => Read data from SPI with dummy write
         Case 2: SrcData!=NULL, DstData=NULL   => Write data to SPI with dummy read
         Case 3: SrcData!=NULL, DstData!=NULL  => duplex Read/Write
     */
    if(ptrMibSpiDriver->params.dmaEnable == (uint8_t)1U)
    {
        uint32_t   dmaChanTx;
        uint32_t   dmaChanRx;
        uint32_t   txRAMAddr;
        uint32_t   rxRAMAddr;

        /* Get DMA channel number */
        dmaChanTx = ptrMibSpiDriver->dmaInfo[group].dmaChannelTx;
        dmaChanRx = ptrMibSpiDriver->dmaInfo[group].dmaChannelRx;

        /* Get MibSPI RAM address */
        txRAMAddr = (uint32_t)(&(ptrMibSpiRam->tx[ramOffset].data));
        rxRAMAddr = (uint32_t)(&(ptrMibSpiRam->rx[ramOffset].data));

        /* Disable TG complete interrupt */
        MIBSPI_disableGroupInterrupt(ptrMibSpiReg, group);

         /* Case 1: SrcData=NULL, DstData!=NULL  => Read data from SPI with dummy write */
        if ((srcData == NULL) && (dstData != NULL))
        {
            /* Configuring dma control packets for writing to TX RAM */
            MIBSPI_sysDmaSetChanParams(ptrMibSpiDriver,
                                          dmaChanTx,
                                          (uint32_t)&ptrMibSpiDriver->txScratchBuffer,
                                          txRAMAddr,
                                          (uint16_t)(bufId + 1U),
                                          (uint16_t)iCount + 1U,
                                          MIBSPI_TRANSFER_DUMMY_TX);

            /* Configuring dma control packets for reading from RX RAM */
            MIBSPI_sysDmaSetChanParams(ptrMibSpiDriver,
                                          dmaChanRx,
                                          rxRAMAddr,
                                          (uint32_t)(dstData),
                                          (uint16_t)(bufId + 1U),
                                          (uint16_t)iCount + 1U,
                                          MIBSPI_TRANSFER_RX);
        }
        /*  Case 2: SrcData!=NULL, DstData=NULL  => Write data to SPI with dummy read  */
        else if ((dstData == NULL) && (srcData != NULL))
        {
            /* Configuring dma control packets for writing to TX RAM */
            MIBSPI_sysDmaSetChanParams(ptrMibSpiDriver,
                                           dmaChanTx,
                                           (uint32_t)srcData,
                                           txRAMAddr,
                                           (uint16_t)(bufId + 1U),
                                           (uint16_t)iCount + 1U,
                                           MIBSPI_TRANSFER_TX);

            /* Configuring dma control packets for reading from RX RAM */
            ptrMibSpiDriver->rxScratchBuffer = 0x0;
            MIBSPI_sysDmaSetChanParams(ptrMibSpiDriver,
                                          dmaChanRx,
                                          rxRAMAddr,
                                          (uint32_t)&ptrMibSpiDriver->rxScratchBuffer,
                                          (uint16_t)(bufId + 1U),
                                          (uint16_t)iCount + 1U,
                                          MIBSPI_TRANSFER_DUMMY_RX);
        }
        /*  Case 3: SrcData!=NULL, DstData!=NULL => duplex Read/Write */
        else
        {
            /* Configuring dma control packets for writing to TX RAM */
            MIBSPI_sysDmaSetChanParams(ptrMibSpiDriver,
                                          dmaChanTx,
                                          (uint32_t)srcData,
                                          txRAMAddr,
                                          (uint16_t)(bufId + 1U),
                                          (uint16_t)iCount + 1U,
                                          MIBSPI_TRANSFER_TX);

            /* Configuring dma control packets for reading from RX RAM */
            MIBSPI_sysDmaSetChanParams(ptrMibSpiDriver,
                                          dmaChanRx,
                                          rxRAMAddr,
                                          (uint32_t)(dstData),
                                          (uint16_t)(bufId + 1U),
                                          (uint16_t)iCount + 1U,
                                          MIBSPI_TRANSFER_RX);
        }

        /* Disable SPI DMA */
        MIBSPI_dmaCtrlGroupDisable(ptrMibSpiReg, group);

        /* Configuring the mibspi dmaCtrl for the channel */
        MIBSPI_dmaCtrlGroupEnable(ptrMibSpiReg, bufId, iCount, group);

        /* Setup TG interrupt */
        MIBSPI_enableGroupInterrupt(ptrMibSpiReg, group, MIBSPI_INT_LEVEL);

        /* Start TG group transfer */
        MIBSPI_transferGroupEnable(ptrMibSpiReg, group);
    }
    else
    {
        uint32_t  index;
        uint16_t  size;

        size = (uint32_t)bufId + 1U - (uint32_t)ramOffset;

        for(index = 0; index <= iCount; index++)
        {
            /* Read data with dummy write through CPU mode */
            if ((srcData == (uint8_t *)NULL) && (dstData != (uint8_t *)NULL))
            {
                MIBSPI_transferGroupEnable(ptrHwCfg->ptrSpiRegBase, group);

                while(MIBSPI_checkTGComplete(ptrMibSpiReg, group) == 0U)
                {
                }

                /* Read data from local buffer to MibSPI Rx RAM */
                MIBSPI_readDataRAM(ptrMibSpiDriver, group, (uint16_t *)dstData, size);

            }
            else if( (srcData != (uint8_t *)NULL) && (dstData == (uint8_t *)NULL))
            {
                /* Write data from MibSPI Rx RAM to local buffer */
                MIBSPI_writeDataRAM(ptrMibSpiDriver, group, (uint16_t *)srcData, size);

                MIBSPI_transferGroupEnable(ptrHwCfg->ptrSpiRegBase, group);

                while(MIBSPI_checkTGComplete(ptrMibSpiReg, group) == 0U)
                {
                }
                /* Read data from local buffer to MibSPI Rx RAM */
                MIBSPI_readDataRAM(ptrMibSpiDriver, group, (uint16_t *)NULL, size);

            }
            else
            {
                /* Write data from local buffer to MibSPI Tx RAM */
                MIBSPI_writeDataRAM(ptrMibSpiDriver, group, (uint16_t *)srcData, size);

                MIBSPI_transferGroupEnable(ptrHwCfg->ptrSpiRegBase, group);

                /* Wait for the transfer to complete */
                while(MIBSPI_checkTGComplete(ptrMibSpiReg, group) == 0U)
                {
                }

                /* Read the data from RX RAM */
                MIBSPI_readDataRAM(ptrMibSpiDriver, group, (uint16_t *)dstData, size);

            }

            srcData = (srcData == (uint8_t *)NULL)? (uint8_t *)NULL : (uint8_t *)(srcData + size * ptrMibSpiDriver->params.dataSize / 8U);
            dstData = (dstData == (uint8_t *)NULL)? (uint8_t *)NULL : (uint8_t *)(dstData + size * ptrMibSpiDriver->params.dataSize / 8U);

            /* Transfer finished, unblock the thread */
            SemaphoreP_post(ptrMibSpiDriver->transferCompleteSem);
        }
    }

    return;
}

/**
 *  @b Description
 *  @n
 *      SPI interface funtion to open SPI device.
 *
 *   @param[in] ptrMibSpiDriver   MibSpi driver handle
 *   @param[in] ptrHwCfg          SPI device Hardware configuration
 *   @param[in] params            SPI device configuration parameters
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    Successful         =0
 *                 Failed         <0
 */
static int32_t MIBSPI_openSlaveMode(MibSpiDriver_Object *ptrMibSpiDriver, MibSpi_HwCfg* ptrHwCfg, SPI_Params *params)
{
    int32_t                 retVal = 0;
    SPI_DMAChanCfg          *ptrDmaCfg = NULL;
    MibSpi_driverDmaInfo    *ptrDmaInfo = NULL;

    /* Initializing mibspi as a SPI slave */
    MIBSPI_initSlave(ptrHwCfg, params);

    if(params->dmaEnable)
    {
        /* Get DMA config params handle */
        ptrDmaCfg = &params->u.slaveParams.dmaCfg;
        ptrDmaInfo = &ptrMibSpiDriver->dmaInfo[MIBSPI_SLAVEMODE_TRANS_GROUP];

        /* DMA Channel Initialization for TX */
        ptrDmaInfo->dmaChannelTx = MIBSPI_sysDmaChanInit(ptrMibSpiDriver,
                                                          ptrDmaCfg->txDmaChanNum,
                                                          ptrHwCfg->dmaReqlineCfg[MIBSPI_SLAVEMODE_TRANS_GROUP].txDmaReqLine,
                                                          &retVal);
        if(retVal < 0)
        {
            retVal = SPI_EDMACONF;
            goto exit;
        }
        ptrDmaInfo->dmaReqlineTx = ptrHwCfg->dmaReqlineCfg[MIBSPI_SLAVEMODE_TRANS_GROUP].txDmaReqLine;

        /* DMA Channel Initialization for RX */
        ptrDmaInfo->dmaChannelRx  = MIBSPI_sysDmaChanInit(ptrMibSpiDriver,
                                                            ptrDmaCfg->rxDmaChanNum,
                                                            ptrHwCfg->dmaReqlineCfg[MIBSPI_SLAVEMODE_TRANS_GROUP].rxDmaReqLine,
                                                            &retVal);
        if(retVal < 0)
        {
            /* Tx channel deinit */
            MIBSPI_sysDmaChanDeInit(ptrMibSpiDriver,
                                      ptrDmaCfg->txDmaChanNum,
                                      ptrHwCfg->dmaReqlineCfg[MIBSPI_SLAVEMODE_TRANS_GROUP].txDmaReqLine);

            retVal = SPI_EDMACONF;
            goto exit;
        }
        ptrDmaInfo->dmaReqlineRx = ptrHwCfg->dmaReqlineCfg[MIBSPI_SLAVEMODE_TRANS_GROUP].rxDmaReqLine;
    }

    /* Save driver info for Slave mode */
    ptrMibSpiDriver->rambufStart [MIBSPI_SLAVEMODE_TRANS_GROUP]  = 0U;
    ptrMibSpiDriver->rambufEnd [MIBSPI_SLAVEMODE_TRANS_GROUP]    = MIBSPI_RAM_MAX_ELEM;

exit:
    return (retVal);
}

/**
 *  @b Description
 *  @n
 *      SPI interface funtion to open SPI device.
 *
 *   @param[in] ptrMibSpiDriver     MibSpi driver handle
 *   @param[in] ptrHwCfg            SPI device Hardware configuration
 *   @param[in] params              SPI device configuration parameters
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    Successful           =0
 *                 Failed           <0
 */
static int32_t MIBSPI_openMasterMode(MibSpiDriver_Object *ptrMibSpiDriver, MibSpi_HwCfg* ptrHwCfg, SPI_Params *params)
{
    int32_t                 retVal = 0;
    SPI_DMAChanCfg          *ptrDmaCfg = NULL;
    MibSpi_driverDmaInfo    *ptrDmaInfo = NULL;
    uint8_t                 index;
    uint8_t                 ramBufOffset = 0;

    /* Initializing mibspi as a SPI master */
    MIBSPI_initMaster(ptrHwCfg, params);

    /* Configure the Master clock prescaler */
    MIBSPI_setMasterClockRate(ptrHwCfg->ptrSpiRegBase, ptrHwCfg->clockSrcFreq, params->u.masterParams.bitRate);

    for(index = 0; index < params->u.masterParams.numSlaves; index++)
    {
        /***************************************
         ****** DMA Configuration if enabled *******
         **************************************/
        if(params->dmaEnable)
        {
            /* Get DMA config params handle */
            ptrDmaCfg = &params->u.masterParams.slaveProf[index].dmaCfg;
            ptrDmaInfo = &ptrMibSpiDriver->dmaInfo[index];

            /* DMA Channel Initialization for TX */
            ptrDmaInfo->dmaChannelTx = MIBSPI_sysDmaChanInit(ptrMibSpiDriver,
                                                              ptrDmaCfg->txDmaChanNum,
                                                              ptrHwCfg->dmaReqlineCfg[index].txDmaReqLine,
                                                              &retVal);
            if(retVal < 0)
            {
                break;
            }
            ptrDmaInfo->dmaReqlineTx = ptrHwCfg->dmaReqlineCfg[index].txDmaReqLine;

            /* DMA Channel Initialization for RX */
            ptrDmaInfo->dmaChannelRx = MIBSPI_sysDmaChanInit(ptrMibSpiDriver,
                                                              ptrDmaCfg->rxDmaChanNum,
                                                              ptrHwCfg->dmaReqlineCfg[index].rxDmaReqLine,
                                                              &retVal);
            if(retVal < 0)
            {
                /* Tx channel deinit */
                MIBSPI_sysDmaChanDeInit(ptrMibSpiDriver,
                                           ptrDmaInfo->dmaChannelTx,
                                           ptrDmaInfo->dmaReqlineTx);

                break;
            }

            ptrDmaInfo->dmaReqlineRx = ptrHwCfg->dmaReqlineCfg[index].rxDmaReqLine;
        }

        /***************************************
         ******** Save RAM offset information *******
         **************************************/
        ptrMibSpiDriver->rambufStart[index] = ramBufOffset;
        ptrMibSpiDriver->rambufEnd[index] = ramBufOffset + params->u.masterParams.slaveProf[index].ramBufLen;
    }

    if(retVal < 0)
    {
        /* Free memory */
        MemoryP_ctrlFree (ptrMibSpiDriver, (uint32_t)sizeof(MibSpiDriver_Object));
    }
    return (retVal);
}

/**************************************************************************
 ************************* MibSPI Exported Driver Functions *********************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      SPI interface funtion to init SPI device.
 *
 *   @param[in] handle      SPI device handle
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    None.
 */
static void MIBSPI_init( SPI_Handle handle)
{
    return;
}

/**
 *  @b Description
 *  @n
 *      SPI interface funtion to open SPI device.
 *
 *   @param[in] handle     SPI device handle
 *   @param[in] params     SPI device configuration parameters
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    SPI device handle.
 */
static SPI_Handle MIBSPI_open(SPI_Handle handle, SPI_Params *params)
{
    SPI_Config*             ptrSPIConfig;
    MibSpi_HwCfg*           ptrHwCfg;
    SemaphoreP_Params       semParams;
    SPI_Handle              retHandle = (SPI_Handle)NULL;
    MibSpiDriver_Object*    ptrMibSpiDriver = NULL;
    HwiP_Params             hwiParams;
    int32_t                 retVal = 0;

    /* Get the SPI driver Configuration: */
    ptrSPIConfig = (SPI_Config*)handle;

    /* Get the hardware configuration: */
    ptrHwCfg = (MibSpi_HwCfg*)ptrSPIConfig->hwAttrs;

    /* Validate hardware configuration */
    DebugP_assert(ptrHwCfg != NULL);
    DebugP_assert(ptrHwCfg->ptrSpiRegBase != NULL);
    DebugP_assert(ptrHwCfg->ptrMibSpiRam != NULL);

#ifdef SPI_PARAMS_CHECK
    /* Validate params for SPI driver */
    retVal = MIBSPI_validateParams(params);
    if(retVal < 0)
    {
        /* Error: SPI Driver found incorrect parameters. */
        DebugP_log1 ("Debug: SPI Driver (%p) failed params validation\n", ptrHwCfg->ptrSpiBase);
        goto exit;
    }
    else
#endif
    {
        /* Ensure that the driver is NOT being used: */
        if (ptrSPIConfig->object != NULL)
        {
            /* Error: SPI Driver is already being used. */
            DebugP_log1 ("Debug: SPI Driver (%p) has already been opened\n", ptrHwCfg->ptrSpiBase);
        }
        else
        {
            /* Allocate memory for the driver: */
            ptrMibSpiDriver = (MibSpiDriver_Object *)MemoryP_ctrlAlloc ((uint32_t)sizeof(MibSpiDriver_Object), 0);
            if (ptrMibSpiDriver == NULL)
            {
                /* Error: Out of memory */
                DebugP_log1 ("Debug: MibSPI Driver (%p) Out of memory\n", ptrHwCfg->ptrSpiBase);

                goto exit;
            }
            else
            {
                /* Initialize the memory: */
                memset ((void *)ptrMibSpiDriver, 0, sizeof(MibSpiDriver_Object));

                /* Save parameters and hardware configurations */
                memcpy((void *)&ptrMibSpiDriver->params, (void *)params, sizeof(SPI_Params));

                ptrMibSpiDriver->ptrHwCfg = ptrHwCfg;

                /* Call open function for the MibSPI operating mode */
                if(params->mode == SPI_SLAVE)
                {
                    retVal = MIBSPI_openSlaveMode(ptrMibSpiDriver, ptrHwCfg, params);
                }
                else
                {
                    retVal = MIBSPI_openMasterMode(ptrMibSpiDriver, ptrHwCfg, params);
                }
                if(retVal < 0)
                {
                    goto exit;
                }

                /* Create a binary semaphore which is used to handle the Blocking operation. */
                SemaphoreP_Params_init(&semParams);
                semParams.mode            = SemaphoreP_Mode_BINARY;
                ptrMibSpiDriver->transferCompleteSem = SemaphoreP_create(0U, &semParams);

		/* Register SPI Interrupt handling ISR */
                HwiP_Params_init(&hwiParams);
                hwiParams.name = "MibSpiInt";
                hwiParams.arg  = (uintptr_t)ptrMibSpiDriver;
                ptrMibSpiDriver->hwiHandle = HwiP_create((int32_t)(ptrHwCfg->interrupt1Num), MIBSPI_ISR, &hwiParams);

                /* Save the Driver handle and Hw config */
                ptrMibSpiDriver->ptrHwCfg = ptrHwCfg;
                ptrSPIConfig->object = (void *)ptrMibSpiDriver;
                ptrMibSpiDriver->spiHandle = handle;
                ptrMibSpiDriver->txScratchBuffer = params->txDummyValue;

                /* Setup the return handle: */
                retHandle = handle;
            }
        }
    }
exit:
    if((retVal < 0) && (ptrMibSpiDriver != NULL) )
    {
        /* Cleanup in case of error */
        if(ptrMibSpiDriver->hwiHandle)
        {
        

HwiP_delete(ptrMibSpiDriver->hwiHandle);
        }

        if(ptrMibSpiDriver != NULL)
        {
            /* Free driver object if memory is allocated */
            MemoryP_ctrlFree(ptrMibSpiDriver, sizeof(ptrMibSpiDriver));
        }
    }

    return (retHandle);
}

/**
 *  @b Description
 *  @n
 *      SPI interface funtion to close SPI device.
 *
 *   @param[in] handle      SPI device handle
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    None.
 */
static void MIBSPI_close(SPI_Handle handle)
{
    MibSpiDriver_Object*    ptrMibSpiDriver;
    MIBSPIRegs              *ptrMibSpiReg;
    MibSpi_driverDmaInfo    *ptrDmaInfo = NULL;
    uint8_t                 index;

    /* Sanity check handle */
    DebugP_assert(handle != NULL);

    /* Get the Object from SPI Handle */
    ptrMibSpiDriver = (MibSpiDriver_Object *)handle->object;

    /* Sanity check driver handle */
    if(ptrMibSpiDriver != (MibSpiDriver_Object *)NULL)
    {
        /* Get the Register base address from SPI Handle */
        ptrMibSpiReg = ptrMibSpiDriver->ptrHwCfg->ptrSpiRegBase;

        if(ptrMibSpiDriver->params.mode == SPI_SLAVE)
        {

            ptrDmaInfo = &ptrMibSpiDriver->dmaInfo[MIBSPI_SLAVEMODE_TRANS_GROUP];

            if(ptrMibSpiDriver->params.dmaEnable)
            {
                /* Deinit DMA channels */
                MIBSPI_sysDmaChanDeInit(ptrMibSpiDriver, ptrDmaInfo->dmaChannelTx, ptrDmaInfo->dmaReqlineTx);
                MIBSPI_sysDmaChanDeInit(ptrMibSpiDriver, ptrDmaInfo->dmaChannelRx, ptrDmaInfo->dmaReqlineRx);
            }
        }
        else
        {
            for(index = 0; index < ptrMibSpiDriver->params.u.masterParams.numSlaves; index ++)
            {
                if(ptrMibSpiDriver->params.dmaEnable)
                {
                    ptrDmaInfo = &ptrMibSpiDriver->dmaInfo[index];

                    /* Deinit DMA channels */
                    MIBSPI_sysDmaChanDeInit(ptrMibSpiDriver, ptrDmaInfo->dmaChannelTx, ptrDmaInfo->dmaReqlineTx);
                    MIBSPI_sysDmaChanDeInit(ptrMibSpiDriver, ptrDmaInfo->dmaChannelRx, ptrDmaInfo->dmaReqlineRx);
                }
            }
        }

        /* Does the SPI Driver in Blocking mode? */
        if (ptrMibSpiDriver->transferCompleteSem)
        {
            /* Delete the semaphore */
            SemaphoreP_delete (ptrMibSpiDriver->transferCompleteSem);
        }

        /* Disable MibSPI */
        MIBSPI_setResetMode(ptrMibSpiReg, true);

        /* Delete Hwi */
        if(ptrMibSpiDriver->hwiHandle)
        {
        

HwiP_delete(ptrMibSpiDriver->hwiHandle);
        }

        /* Free memory */
        MemoryP_ctrlFree (ptrMibSpiDriver, (uint32_t)sizeof(MibSpiDriver_Object));
    }

    /* Reset the object handle : */
    handle->object = NULL;
}

/**
 *  @b Description
 *  @n
 *      SPI interface funtion to send command to SPI driver.
 *
 *   @param[in] handle      SPI device handle
 *   @param[in] cmd          Command to SPI device
 *   @param[in] arg           Command Argument to SPI device
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    Status of SPI device command.
 */
static int32_t MIBSPI_control(SPI_Handle handle, uint32_t cmd, void *arg)
{
    MibSpiDriver_Object     *ptrMibSpiDriver;
    MIBSPIRegs              *ptrMibSpiReg;
    int32_t                 status = SPI_STATUS_SUCCESS;
    MibSpi_LoopBackType     loopbacktype;

    /* Sanity check params */
    if( (handle == NULL) || (arg == NULL))
    {
        status = SPI_EINVAL;
    }
    else
    {
        /* Get the Object from SPI Handle */
        ptrMibSpiDriver = handle->object;

        /* Get the Register base address from SPI Handle */
        ptrMibSpiReg = ptrMibSpiDriver->ptrHwCfg->ptrSpiRegBase;

        switch (cmd)
        {
            case SPI_CMD_LOOPBACK_ENABLE:
                {
                    loopbacktype = *(MibSpi_LoopBackType *)arg;

                    /* Sanity check the input parameters */
                    if (ptrMibSpiDriver->params.mode != SPI_MASTER)
                    {
                        /* Loopback is not supported in Slave mode */
                        status = SPI_EINVAL;
                    }
                    else if ( (loopbacktype == SPI_LOOPBK_DIGITAL) || (loopbacktype == SPI_LOOPBK_ANALOG) )
                    {
                        /* Enable Loopback here in case Deice act as MibSPI Slave */
                        MIBSPI_enableLoopback(ptrMibSpiReg, loopbacktype);
                        status = SPI_STATUS_SUCCESS;
                    }
                    else
                    {
                        /* Disable MibSPI LoopBack */
                        MIBSPI_disableLoopback(ptrMibSpiReg);

                        status = SPI_STATUS_SUCCESS;
                    }
                }

                break;

            case SPI_CMD_SET_CLOCK_PHASEPOLARITY:
                {
                    uint32_t frameFmt;
                    frameFmt = *(uint32_t *)arg;

                    /* 2 bit setting */
                    frameFmt = frameFmt & 0x3U;

                    /* Sanity check the input parameters */
                    if (ptrMibSpiDriver->params.mode != SPI_SLAVE)
                    {
                        /* Loopback is not supported in Master mode */
                        status = SPI_EINVAL;
                    }
                    else
                    {
                        MIBSPI_setClockPhasePolarity(ptrMibSpiReg, (uint8_t)frameFmt);
                        status = SPI_STATUS_SUCCESS;
                    }
                }
                break;

            default:
                /* Command is not supported */
                status = SPI_EINVALCMD;
                break;
        }
    }
    return (status);
}

/**
 *  @b Description
 *  @n
 *      SPI interface funtion to initiate SPI transaction based on the transaction configuration.
 *
 *   @param[in] handle         SPI device handle
 *   @param[in] transaction    Transaction configuration
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    true          if transaction is successful
 *             false         if transaction is failed
 */
static bool MIBSPI_transfer(SPI_Handle handle, SPI_Transaction *transaction)
{
    uintptr_t               key;
    MibSpiDriver_Object     *ptrMibSpiDriver;
    SemaphoreP_Status       semaStatus;
    bool                    ret = false;
    uint16_t                dataLength;

    /* Get the MibSpi driver handle */
    ptrMibSpiDriver = (MibSpiDriver_Object *)handle->object;

#ifdef SPI_PARAMS_CHECK
    /* Initialiaze transaction as invalid transer */
    transaction->status = SPI_TRANSFER_INVAL;

    /* Check the transaction arguments */
    if (transaction->count == 0U)
    {
        goto Exit;
    }

    /* Sanity check of parameters */
    if((transaction->txBuf == NULL) && (transaction->rxBuf == NULL))
    {
        goto Exit;
    }

    if(transaction->slaveIndex >= MIBSPI_SLAVE_MAX)
    {
        goto Exit;
    }
#endif
    if(ptrMibSpiDriver->params.dataSize == 16U)
    {
        if(transaction->count % 2U != 0)
        {
            goto Exit;
        }

        /* MIBSPI reads in 2 bytes format */
        dataLength = (uint16_t)transaction->count >> 1U;
    }
    else
    {
        dataLength = transaction->count;
    }

#ifndef SPI_MULT_ICOUNT_SUPPORT

    /* Check data elements */
    if(dataLength > MIBSPI_RAM_MAX_ELEM)
    {
        goto Exit;
    }
#endif
    /* Check if a transfer is in progress */
    key = HwiP_disable();

    if ( ptrMibSpiDriver->transaction != NULL)
    {
        /* Transfer is in progress */
        HwiP_restore(key);
        transaction->status = SPI_TRANSFER_INPROG;
        goto Exit;
    }
    else
    {
        ptrMibSpiDriver->transaction = transaction;
    }

    HwiP_restore(key);

#ifndef SPI_MULT_ICOUNT_SUPPORT
    /* Update transaction status as started */
    transaction->status = SPI_TRANSFER_STARTED;
    if(ptrMibSpiDriver->params.mode == SPI_SLAVE)
    {
        MIBSPI_dataTransfer(ptrMibSpiDriver, transaction->txBuf, transaction->rxBuf, dataLength, MIBSPI_SLAVEMODE_TRANS_GROUP);
    }
    else
    {
        MIBSPI_dataTransfer(ptrMibSpiDriver, transaction->txBuf, transaction->rxBuf, dataLength, transaction->slaveIndex);
    }

    if (ptrMibSpiDriver->params.transferMode == SPI_MODE_BLOCKING)
    {
        semaStatus = SemaphoreP_pend(ptrMibSpiDriver->transferCompleteSem, ptrMibSpiDriver->params.transferTimeout);
        if(semaStatus != SemaphoreP_OK)
        {
            /* Populate status code */
            transaction->status = SPI_TRANSFER_TIMEOUT;
            ret = false;
        }
        else
        {
            /* Populate status code */
            transaction->status = SPI_TRANSFER_COMPLETED;
            ret = true;
        }
    }
    else
    {
        /* Execution should not reach here */
        transaction->status = SPI_TRANSFER_INVAL;
        ret = false;
    }
#else
    /* Update transaction status as started */
    transaction->status = SPI_TRANSFER_STARTED;

    do {
        uint32_t    ramSize;
        uint16_t    remainSize;
        uint16_t    dataSizeInBytes = 2U;

        if(ptrMibSpiDriver->params.dataSize == 16U)
        {
            dataSizeInBytes = 2U;
        }
        else
        {
            dataSizeInBytes = 1U;
        }

        /* Find out rambuf size */
        if(ptrMibSpiDriver->params.mode == SPI_SLAVE)
        {
            ramSize = MIBSPI_RAM_MAX_ELEM;
        }
        else
        {
            ramSize = ptrMibSpiDriver->params.u.masterParams.slaveProf[transaction->slaveIndex].ramBufLen;
        }

        /*
         *  If dataLength is bigger than ramSize, there will be two data transfers:
         *      1. multiple of ramSize
         *      2. remaining data after multiple of ramsize
         *  If dataLength is smaller or equal to ramSize, there will be one data transfer
         */
        remainSize = (dataLength > ramSize) ? dataLength%ramSize : 0U;

        if(remainSize > 0U)
        {
            /* Size of the first transfer */
            dataLength -= remainSize;
        }

        if(ptrMibSpiDriver->params.mode == SPI_SLAVE)
        {
            MIBSPI_dataTransfer(ptrMibSpiDriver, transaction->txBuf, transaction->rxBuf, dataLength, MIBSPI_SLAVEMODE_TRANS_GROUP);
        }
        else
        {
            MIBSPI_dataTransfer(ptrMibSpiDriver, transaction->txBuf, transaction->rxBuf, dataLength, transaction->slaveIndex);
        }

        if (ptrMibSpiDriver->params.transferMode == SPI_MODE_BLOCKING)
        {
            semaStatus = SemaphoreP_pend(ptrMibSpiDriver->transferCompleteSem, ptrMibSpiDriver->params.transferTimeout);
            if(semaStatus != SemaphoreP_OK)
            {
                /* Populate status code */
                transaction->status = SPI_TRANSFER_TIMEOUT;
                ret = false;
                goto Exit;
            }
            else
            {
                /* Populate status code */
                transaction->status = SPI_TRANSFER_COMPLETED;
                ret = true;
            }
        }
        else
        {
            /* Execution should not reach here */
            transaction->status = SPI_TRANSFER_INVAL;
            ret = false;
        }

        /* Check if transfer finished */
        if(remainSize == 0)
        {
            break;
        }

        /* Change buffer pointer and data size for the second transfer */
        transaction->txBuf = (void *)((transaction->txBuf == NULL)? NULL: (uint8_t *)transaction->txBuf + dataLength * dataSizeInBytes);
        transaction->rxBuf = (void *)((transaction->rxBuf == NULL)? NULL: (uint8_t *)transaction->rxBuf + dataLength * dataSizeInBytes);
        dataLength = remainSize;

        /* Transfer the remaining size */
        if(ptrMibSpiDriver->params.mode == SPI_SLAVE)
        {
            MIBSPI_dataTransfer(ptrMibSpiDriver, transaction->txBuf, transaction->rxBuf, dataLength, MIBSPI_SLAVEMODE_TRANS_GROUP);
        }
        else
        {
            MIBSPI_dataTransfer(ptrMibSpiDriver, transaction->txBuf, transaction->rxBuf, dataLength, transaction->slaveIndex);
        }

        if (ptrMibSpiDriver->params.transferMode == SPI_MODE_BLOCKING)
        {
            semaStatus = SemaphoreP_pend(ptrMibSpiDriver->transferCompleteSem, ptrMibSpiDriver->params.transferTimeout);
            if(semaStatus != SemaphoreP_OK)
            {
                /* Populate status code */
                transaction->status = SPI_TRANSFER_TIMEOUT;
                ret = false;
            }
            else
            {
                /* Populate status code */
                transaction->status = SPI_TRANSFER_COMPLETED;
                ret = true;
            }
        }
        else
        {
            /* Execution should not reach here */
            transaction->status = SPI_TRANSFER_INVAL;
            ret = false;
        }
    }while(0);

#endif

Exit:
    /* Disable transfer group */
    MIBSPI_transferGroupDisable(ptrMibSpiDriver->ptrHwCfg->ptrSpiRegBase, transaction->slaveIndex);
    
    /* Clear transaction handle */
    key = HwiP_disable();

    ptrMibSpiDriver->transaction = NULL;

    HwiP_restore(key);
    return ret;
}

/**
 *  @b Description
 *  @n
 *      SPI interface funtion to cancell an ongoing transaction.
 *
 *   @param[in] handle          SPI device handle
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    None
 */
static void MIBSPI_transferCancel(SPI_Handle handle)
{
    /* Stop Transfer is in progress */
    DebugP_log1("SPI:(%p) Error! Stop Transaction is  not supported\n", handle);

    /* This API is not supported, hence throw an assertion */
    DebugP_assert(0);

    return;
}

/**
 *  @b Description
 *  @n
 *      ISR service routine for SPI driver .
 *
 *   @param[in] handle          SPI device handle
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    None
 */
static void  MIBSPI_serviceISR(SPI_Handle handle)
{
    return;
}

/**
 *  @b Description
 *  @n
 *      Get SPI driver internal stats
 *
 *   @param[in] handle           SPI device handle
 *   @param[in] ptrStats         pointer to SPI driver stats structure
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    None
 */
static int32_t MIBSPI_getStats(SPI_Handle handle, SPI_Stats *ptrStats)
{
    MibSpiDriver_Object     *ptrMibSpiDriver;
    int32_t                 retCode = 0;

    /* Validate input parameters */
    if((handle == NULL) || (ptrStats == NULL))
    {
        retCode = SPI_EINVAL;
    }
    else
    {
        /* Get the MibSpi driver handle */
        ptrMibSpiDriver = (MibSpiDriver_Object *)handle->object;

        ptrStats->dlenErr = ptrMibSpiDriver->hwStats.dlenErr;
        ptrStats->timeout = ptrMibSpiDriver->hwStats.timeout;
        ptrStats->parErr = ptrMibSpiDriver->hwStats.parErr;
        ptrStats->desync = ptrMibSpiDriver->hwStats.desync;
        ptrStats->bitErr = ptrMibSpiDriver->hwStats.bitErr;
        ptrStats->rxOvrnErr =  ptrMibSpiDriver->hwStats.rxOvrnErr;
    }
    
    return retCode;
}
