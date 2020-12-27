/*
 * Copyright (c) 2017, Texas Instruments Incorporated
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
/** ============================================================================
 *  @file       qspi.c
 *
 *  @brief      QSPI driver
 *
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include <stdint.h>
#include <string.h>

/* mmwaveSDK include files */
#include <ti/drivers/osal/MemoryP.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/osal/DebugP.h>

/* QSPI include files */
#include <ti/drivers/qspi/include/reg_qspi.h>
#include <ti/drivers/qspi/qspi.h>
#include <ti/drivers/qspi/include/qspi_internal.h>

/* Flag for Params check */
#define QSPI_PARAMS_CHECK

/**************************************************************************
 *************************** Extern Variables *********************************
 **************************************************************************/
/* QSPI hardware configurations */
extern QSPI_HWCFG gQSPIHwCfg;

/**************************************************************************
 *************************** Local Structures *********************************
 **************************************************************************/

/**
 * @brief
 *  QSPI Driver
 *
 * @details
 *  The structure describes the QSPI Driver and tracks all the information
 *  relevant to the driver
 */
typedef struct QSPI_Driver_t
{
    /**
     * @brief   QSPI params which was used to instantiate the driver
     */
    QSPI_Params     params;

    /**
     * @brief   Pointer to QSPI Register base address
     */
    QSPIRegs*       ptrRegBase;
}QSPI_Driver;

/**************************************************************************
 *************************** Global Variables *********************************
 **************************************************************************/

/* Default QSPI parameters */
const QSPI_Params QSPI_defaultParams =
{
    .qspiClk      = 200000000U,
    .bitRate      = 40000000U,
    .clkMode      = QSPI_CLOCK_MODE_0,
    .dataDelay    = (uint8_t)0U,
    .csPolarity   = (uint8_t)0U
};

QSPI_Driver *gQSPIDriverPtr = (QSPI_Driver *)NULL;

/**************************************************************************
 *************************** Internal Function Prototype ************************
 **************************************************************************/
static void QSPIWaitIdle(QSPIRegs* ptrQSPIRegs);
static void QSPISetDevControlReg(QSPIRegs* ptrQSPIRegs, uint8_t chipSelect, QSPI_clockMode clockMode, uint8_t dataDelay, uint8_t csPolarity);
static void QSPIConfigClockDivisor(QSPIRegs* ptrQSPIRegs, uint16_t qspiClkDiv);
static uint32_t QSPIGetStatus(QSPIRegs* ptrQSPIRegs);
static void QSPISetInterfaceType(QSPIRegs* ptrQSPIRegs, QSPI_INTF intfType);
static void QSPIHwInit(volatile QSPIRegs*  ptrQSPIRegs);

/**************************************************************************
 *************************** Internal Functions *******************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *     This function wait in tight loop until QSPI is not busy
 *
 *  @param[in]  ptrQSPIRegs         Pointer to QSPI IP register base
 *
 *  \ingroup QSPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void QSPIWaitIdle(QSPIRegs* ptrQSPIRegs)
{
    while (ptrQSPIRegs->SPI_STATUS & QSPI_SPI_STATUS_REG_BUSY);
}

 /**
 *  @b Description
 *  @n
 *     This function sets the QSPI clock phase and polarity
 *
 *  @param[in]  ptrQSPIRegs         Pointer to QSPI IP register base
 *  @param[in]  chipSelect          Chip select for the clock configuration
 *  @param[in]  clockMode           SPI clock mode
 *  @param[in]  dataDelay           Data delay
 *  @param[in]  csPolarity          CS signal polarity
 *
 *  \ingroup QSPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void QSPISetDevControlReg(QSPIRegs* ptrQSPIRegs, uint8_t chipSelect, QSPI_clockMode clockMode, uint8_t dataDelay, uint8_t csPolarity)
{
    uint32_t  shift;
    uint8_t   configVal;

    /* Find out number of bits need to be shifted */
    shift = chipSelect * 8U;

    /* Setup polarity and phase based on the clock mode */
    switch(clockMode)
    {
        case QSPI_CLOCK_MODE_0:
            /* Clock phase and clock polarity are both 0 */
            configVal = 0U << SPI_DC_CKP0_BIT_START |
                      csPolarity << SPI_DC_CSP0_BIT_START |
                      0U << SPI_DC_CKPH0_BIT_START |
                      dataDelay << SPI_DC_DD0_BIT_START;
            break;
        case QSPI_CLOCK_MODE_3:
            /* Clock phase and clock polarity are both 1 */
            configVal = 1U << SPI_DC_CKP0_BIT_START |
                      csPolarity << SPI_DC_CSP0_BIT_START |
                      1U << SPI_DC_CKPH0_BIT_START |
                      dataDelay <<SPI_DC_DD0_BIT_START;
            break;
        default:
            DebugP_assert(0U);
            break;
    }

    /* Initial Data Control Configuration */
    ptrQSPIRegs->SPI_DC = CSL_FINSR(ptrQSPIRegs->SPI_DC, shift + 5U, shift, configVal);
}

/**
 *  @b Description
 *  @n
 *     This function configure QSPI clock
 *
 *  @param[in]  ptrQSPIRegs         Pointer to QSPI IP register base
 *  @param[in]  qspiClkDiv          Clock divisor
 *
 *  \ingroup QSPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void QSPIConfigClockDivisor(QSPIRegs* ptrQSPIRegs, uint16_t qspiClkDiv)
{
    /* Disable QSPI data clock */
    ptrQSPIRegs->SPI_CLOCK_CNTRL = CSL_FINSR(ptrQSPIRegs->SPI_CLOCK_CNTRL,
                                              SPI_CLOCK_CNTRL_CLKEN_BIT_END,
                                              SPI_CLOCK_CNTRL_CLKEN_BIT_START,
                                              QSPI_CLK_CTRL_CLKEN_DCLOCK_OFF);

    /* Change clock divisor */
    ptrQSPIRegs->SPI_CLOCK_CNTRL = CSL_FINSR(ptrQSPIRegs->SPI_CLOCK_CNTRL,
                                              SPI_CLOCK_CNTRL_DCLK_DIV_BIT_END,
                                              SPI_CLOCK_CNTRL_DCLK_DIV_BIT_START,
                                              qspiClkDiv);

    /* Enable QSPI data clock */
    ptrQSPIRegs->SPI_CLOCK_CNTRL= CSL_FINSR(ptrQSPIRegs->SPI_CLOCK_CNTRL,
                                              SPI_CLOCK_CNTRL_CLKEN_BIT_END,
                                              SPI_CLOCK_CNTRL_CLKEN_BIT_START,
                                              QSPI_CLK_CTRL_CLKEN_DCLOCK_ON);
}

#if 0
/**
 *  @b Description
 *  @n
 *     This function read from QSPI data registers
 *
 *  @param[in]  ptrQSPIRegs         Pointer to QSPI IP register base
 *  @param[in]  nthWord             Word selection
 *
 *  \ingroup QSPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval     Data read from data register
 */
static uint32_t QSPIReadWord(QSPIRegs* ptrQSPIRegs, uint8_t nthWord)
{
    uint32_t data;

    /* Wait for the QSPI to be idle */
    QSPIWaitIdle(ptrQSPIRegs);

    /* Reading data from QSPI peripheral */
    switch(nthWord)
    {
        case 0:
            data = ptrQSPIRegs->SPI_DATA;
            break;
        case 1:
            data = ptrQSPIRegs->SPI_DATA1;
            break;
        case 2:
            data = ptrQSPIRegs->SPI_DATA2;
            break;
        case 3:
            data = ptrQSPIRegs->SPI_DATA3;
            break;
        default:
            DebugP_assert(0);
            break;
    }
    return data;
}

/**
 *  @b Description
 *  @n
 *     This function write a word to QSPI
 *
 *  @param[in]  ptrQSPIRegs         Pointer to QSPI IP register base
 *  @param[in]  data                Data to be written
 *  @param[in]  nthWord             Word selection
 *
 *  \ingroup QSPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void QSPIWriteWord(QSPIRegs* ptrQSPIRegs, uint32_t data, uint8_t nthWord)
{
    /* wait for the QSPI to be idle */
    QSPIWaitIdle(ptrQSPIRegs);

    /* Reading data from QSPI peripheral */
    switch(nthWord)
    {
        case 0:
            ptrQSPIRegs->SPI_DATA = data;
            break;
        case 1:
            ptrQSPIRegs->SPI_DATA1 = data;
            break;
        case 2:
            ptrQSPIRegs->SPI_DATA2 = data;
            break;
        case 3:
            ptrQSPIRegs->SPI_DATA3 = data;
            break;
        default:
            DebugP_assert(0);
            break;
    }
}
#endif

/**
 *  @b Description
 *  @n
 *     This function read and return the status register value
 *
 *  @param[in]  ptrQSPIRegs         Pointer to QSPI IP register base
 *
 *  \ingroup QSPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval QSPI status
 */
static uint32_t QSPIGetStatus(QSPIRegs* ptrQSPIRegs)
{
    /* return QSPI Status register value */
    return (ptrQSPIRegs->SPI_STATUS);
}

/**
 *  @b Description
 *  @n
 *     This function selects the memory mapped mode or configure port as interface.
 *
 *  @param[in]  ptrQSPIRegs         Pointer to QSPI IP register base
 *  @param[in]  intfType            Interface type to be configured
 *
 *  \ingroup QSPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void QSPISetInterfaceType(QSPIRegs* ptrQSPIRegs, QSPI_INTF intfType)
{
    switch(intfType)
    {
        case QSPI_INTF_CFG:
        case QSPI_INTF_MM:
            ptrQSPIRegs->SPI_SWITCH = CSL_FINSR(ptrQSPIRegs->SPI_SWITCH,
                                                 SPI_SWITCH_MMPT_S_BIT_END,
                                                 SPI_SWITCH_MMPT_S_BIT_START,
                                                 (uint32_t)intfType);
            break;
        default:
            /* Not supported */
            DebugP_assert(0U);
            break;
    }
}

/**
 *  @b Description
 *  @n
 *      The function performs hardware initialzation of QSPI.
 *
 *  @param[in]  ptrQSPIRegs         Pointer to QSPI IP register base
 *
 *  \ingroup QSPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void QSPIHwInit(volatile QSPIRegs*  ptrQSPIRegs)
{
    /* Disable interrupts */
    ptrQSPIRegs->INTR_ENABLE_CLEAR = 0x0U;

    /* Disable clcok */
    ptrQSPIRegs->SPI_CLOCK_CNTRL = 0x0U;

    /* Initial Data Control Configuraiton */
    ptrQSPIRegs->SPI_DC = 0U;

    /* Initial SWITCH Configuration */
    ptrQSPIRegs->SPI_SWITCH = 0U;
}

/**************************************************************************
 ************************ Exported QSPI APIs*********************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      This function must be called once per system and before
 *          any other QSPI driver APIs.
 *
 *  \ingroup QSPI_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void QSPI_init(void)
{
}
/**
 *  @b Description
 *  @n
 *      This function must be called once per system and before
 *          any other QSPI driver APIs.
 *
 *  @param[in]  params      QSPI params for creating a instance for the first time
 *
 *  \ingroup QSPI_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void QSPI_Params_init(QSPI_Params *params)
{
    *params = QSPI_defaultParams;
}

/*!
 *  @brief  Function to initialize a given QSPI peripheral
 *
 *  @pre    QSPI_init() has been called
 *
 *  @param[in]  params          QSPI params for creating QSPI driver instance. This
 *                              structure can be pre-populated with default values using
 *                              QSPI_Params_init() API.
 *
 *  @param[out]  errCode        Error Code populated by QSPI driver in case of error.
 *
 *  @return  A QSPI_Handle upon success. NULL if an error occurs.
 *
 *  @sa     QSPI_init()
 *  @sa     QSPI_close()
 */
QSPI_Handle QSPI_open(QSPI_Params* params, int32_t* errCode)
{
    volatile QSPIRegs*  ptrQSPIReg;
    QSPI_Handle         retHandle = NULL;
    uintptr_t           key;
    int32_t             retVal = 0;

    /* Disable preemption while opening the driver */
    key = HwiP_disable();

    /* Check if driver is already opened */
    if (gQSPIDriverPtr == NULL)
    {

#ifdef QSPI_PARAMS_CHECK
        if ((params->qspiClk == 0U) ||
           (params->bitRate == 0U)   ||
           (params->csPolarity > (uint8_t)0x1U) ||
           (params->dataDelay > (uint8_t) QSPI_MAX_DATA_DELAY) )
        {
            /* Error: Invalid input params */
            retVal = QSPI_EINVAL;
            goto Exit;
        }

        /* QSPI clock has be bigger than SPI bit Rate */
        if(params->qspiClk < params->bitRate)
        {
            /* Error: Invalid input params */
            retVal = QSPI_EINVAL;
            goto Exit;
        }

        /* QSPI bit clock Rate has a limitation of 60MHz in hardware */
        if(params->bitRate > 60 * 1000000U)
        {
            /* Error: Invalid input params */
            retVal = QSPI_EINVAL;
            goto Exit;
        }
#endif
        /* Allocate memory for the driver */
        gQSPIDriverPtr = MemoryP_ctrlAlloc ((uint32_t)sizeof(QSPI_Driver), 0U);
        if (gQSPIDriverPtr == NULL)
        {
            /* Error: Out of memory */
            DebugP_log1 ("Debug: QSPI_open: Out of memory ( size: %d)\n",(uint32_t)sizeof(QSPI_Driver));
            retVal = QSPI_ENOMEM;
        }
        else
        {
            /* Initialize QSPI driver object */
            memset ((void *)gQSPIDriverPtr, 0U, sizeof(QSPI_Driver));
            gQSPIDriverPtr->ptrRegBase= (volatile QSPIRegs*)gQSPIHwCfg.ptrQSPIRegBase;
        }

        /* QSPI hardware initialization */
        if (gQSPIDriverPtr != NULL)
        {
            uint16_t    clkDiv;

            ptrQSPIReg = (volatile QSPIRegs*)gQSPIHwCfg.ptrQSPIRegBase;

            DebugP_assert(ptrQSPIReg != (volatile QSPIRegs*)NULL);

            /* Hardware init */
            QSPIHwInit(ptrQSPIReg);

            QSPISetInterfaceType(ptrQSPIReg, QSPI_INTF_CFG);

            /*
             *  Configre QSPI clock Control Register
             *  QSPI input clock is set by application and passed to QSPI driver
             */
            clkDiv = params->qspiClk / params->bitRate - 1U;

            QSPIConfigClockDivisor(ptrQSPIReg, clkDiv);

            /* Setup clock polarity and phase */
            QSPISetDevControlReg(ptrQSPIReg, QSPI_CS0, params->clkMode, params->dataDelay, params->csPolarity);

            /* Save params in driver object */
            memcpy((void *)&gQSPIDriverPtr->params, (void *)params, sizeof(QSPI_Params));
        }
    }
    else
    {
        retVal = QSPI_EINUSE;
    }

Exit:
    /* Restore the interrupts: */
    HwiP_restore(key);

    /* Check retVal and return handle */
    if(retVal == 0)
    {
        retHandle = gQSPIDriverPtr;
    }
    else if (gQSPIDriverPtr != (QSPI_Driver *)NULL)
    {
        /* Free memory */
        MemoryP_ctrlFree (gQSPIDriverPtr, (uint32_t)sizeof(QSPI_Driver));
        gQSPIDriverPtr = (QSPI_Driver *)NULL;
    }
    else
    {
        /* Do nothing, just for Misra C compliant */
    }

    /* return errCode */
    if (errCode != NULL)
    {
        *errCode = retVal;
    }
    return retHandle;
}

/*!
 *  @brief  Function to close a QSPI driver instance specified by the QSPI handle
 *
 *  @pre    QSPI_open() has been called.
 *
 *  @param  handle      A QSPI_Handle returned from QSPI_open()
 *
 *  @return 0 upon success. Error code if an error occurs.
 *
 *  @sa     QSPI_open()
 */
int32_t QSPI_close(QSPI_Handle handle)
{
    QSPI_Driver*        ptrQSPIriver = NULL;
    volatile QSPIRegs*  ptrQSPIRegs;
    int32_t             retVal = 0;
    uintptr_t           key;

    ptrQSPIriver = (QSPI_Driver *) handle;

    DebugP_assert(ptrQSPIriver == gQSPIDriverPtr);

    /* Disable preemption while opening the driver */
    key = HwiP_disable();

    if (ptrQSPIriver != (QSPI_Driver *)NULL)
    {
        ptrQSPIRegs = ((QSPI_Driver *)handle)->ptrRegBase;

        /* Put QSPI in force-idle state */
        ptrQSPIRegs->SYSCONFIG = CSL_FINSR(ptrQSPIRegs->SYSCONFIG,
                                            SYSCONFIG_IDLEMODE_BIT_END,
                                            SYSCONFIG_IDLEMODE_BIT_START,
                                            QSPI_IDLE_MODE_FORCE_IDLE);


        
        /* Free memory */
        MemoryP_ctrlFree (ptrQSPIriver, (uint32_t)sizeof(QSPI_Driver));

        /* Reset Handle */
        gQSPIDriverPtr = (QSPI_Driver *)NULL;
    }
    else
    {
        /* return invalid code */
        retVal = QSPI_EINVAL;
    }

    /* Restore the interrupts */
    HwiP_restore(key);

    return retVal;
}

/**
 *  @b Description
 *  @n
 *      This function Performs a QSPI read through configuration port.
 *
 *   @param[in] handle      QSPI handle
 *   @param[in] cmd         QSPI read command
 *   @param[in] wordLen     QSPI data word length in number of bits
 *   @param[in] data        Pointer to a data buffer to save the read data
 *   @param[in] numWords    Number of Words to be read in a frame
 *
 *  \ingroup QSPI_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success             =0;
 *      Faliure             <0 value indicates the error code
 */

int32_t QSPI_cfgRead(QSPI_Handle handle, QSPI_CMD cmd, QSPI_WORDLEN wordLen, uint32_t *data, uint32_t numWords)
{
    volatile QSPIRegs*  ptrQSPIRegs;
    int32_t             retVal = 0;

    DebugP_assert(handle != (QSPI_Handle)NULL);

    ptrQSPIRegs = ((QSPI_Driver *)handle)->ptrRegBase;

#ifdef QSPI_PARAMS_CHECK
    if ((data == (uint32_t *)NULL) ||
        (numWords > QSPI_MAX_NUM_WORDS))
    {
        retVal = QSPI_EINVAL;
    }
    else
#endif
    {
        QSPIWaitIdle(ptrQSPIRegs);

        /* write QSPI read command */
        ptrQSPIRegs->SPI_CMD = CSL_FMKR(SPI_CMD_CSNUM_BIT_END,      SPI_CMD_CSNUM_BIT_START,    QSPI_CS0) |
                               CSL_FMKR(SPI_CMD_WLEN_BIT_END,       SPI_CMD_WLEN_BIT_START,     wordLen - 1U) |
                               CSL_FMKR(SPI_CMD_CMD_BIT_END,        SPI_CMD_CMD_BIT_START,      cmd) |
                               CSL_FMKR(SPI_CMD_FIRQ_BIT_END,       SPI_CMD_FIRQ_BIT_START,     1U) |
                               CSL_FMKR(SPI_CMD_WIRQ_BIT_END,       SPI_CMD_WIRQ_BIT_START,     0U) |
                               CSL_FMKR(SPI_CMD_FLEN_BIT_END,       SPI_CMD_FLEN_BIT_START,     numWords - 1U);

        QSPIWaitIdle(ptrQSPIRegs);

        /* Read data register */
        *data = ptrQSPIRegs->SPI_DATA;
    }
    return (retVal);
}

/**
 *  @b Description
 *  @n
 *      This function Performs a QSPI write through configuration port.
 *
 *   @param[in] handle      QSPI handle
 *   @param[in] cmd         QSPI write command
 *   @param[in] wordLen     QSPI data word length in number of bits
 *   @param[in] data        Data to be written
 *   @param[in] numWords    Number of Words to be written in a frame
 *
 *  \ingroup QSPI_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success             =0;
 *      Faliure             <0 value indicates the error code
 */
int32_t QSPI_cfgWrite(QSPI_Handle handle, QSPI_CMD cmd, QSPI_WORDLEN wordLen, uint32_t data, uint32_t numWords)
{
    volatile QSPIRegs*  ptrQSPIRegs;
    int32_t             retVal = 0;

    DebugP_assert(handle != (QSPI_Handle)NULL);

#ifdef QSPI_PARAMS_CHECK
    if (numWords > QSPI_MAX_NUM_WORDS)
    {
        retVal = QSPI_EINVAL;
    }
    else
#endif
    {
        ptrQSPIRegs = ((QSPI_Driver *)handle)->ptrRegBase;

        QSPIWaitIdle(ptrQSPIRegs);

        /* Write data register */
        ptrQSPIRegs->SPI_DATA = data;

        /* Write command */
        ptrQSPIRegs->SPI_CMD = CSL_FMKR(SPI_CMD_CSNUM_BIT_END,      SPI_CMD_CSNUM_BIT_START,    QSPI_CS0) |
                               CSL_FMKR(SPI_CMD_WLEN_BIT_END,       SPI_CMD_WLEN_BIT_START,     wordLen - 1U) |
                               CSL_FMKR(SPI_CMD_CMD_BIT_END,        SPI_CMD_CMD_BIT_START,      cmd) |
                               CSL_FMKR(SPI_CMD_FIRQ_BIT_END,       SPI_CMD_FIRQ_BIT_START,     1U) |
                               CSL_FMKR(SPI_CMD_WIRQ_BIT_END,       SPI_CMD_WIRQ_BIT_START,     0U) |
                               CSL_FMKR(SPI_CMD_FLEN_BIT_END,       SPI_CMD_FLEN_BIT_START,     numWords - 1U);
    }
    return (retVal);
}

/**
 *  @b Description
 *  @n
 *      This function configurates memory map setup register
 *
 *   @param[in] handle      QSPI handle
 *   @param[in] memConfig   memory map configuration
 *
 *  \ingroup QSPI_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success             =0;
 *      Faliure             <0 value indicates the error code
 */

void QSPI_memAccessCfg(QSPI_Handle handle, QSPI_memConfig* memConfig)
{
    volatile QSPIRegs*  ptrQSPIRegs;
    uint32_t            setupConfig;

    DebugP_assert(handle != (QSPI_Handle)NULL);

    ptrQSPIRegs = ((QSPI_Driver *)handle)->ptrRegBase;

    setupConfig = CSL_FMKR(SPI_SETUP0_NUM_D_BITS_BIT_END,  SPI_SETUP0_NUM_D_BITS_BIT_START,    memConfig->numDBits) |
                CSL_FMKR(SPI_SETUP0_WCMD_BIT_END,          SPI_SETUP0_WCMD_BIT_START,          memConfig->writeCmd) |
                CSL_FMKR(SPI_SETUP0_READ_TYPE_BIT_END,     SPI_SETUP0_READ_TYPE_BIT_START,     memConfig->readType) |
                CSL_FMKR(SPI_SETUP0_NUM_D_BYTES_BIT_END,   SPI_SETUP0_NUM_D_BYTES_BIT_START,   memConfig->numDBytes) |
                CSL_FMKR(SPI_SETUP0_NUM_A_BYTES_BIT_END,   SPI_SETUP0_NUM_A_BYTES_BIT_START,   memConfig->numABytes) |
                CSL_FMKR(SPI_SETUP0_RCMD_BIT_END,          SPI_SETUP0_RCMD_BIT_START,          memConfig->readCmd);

    ptrQSPIRegs->SPI_SETUP[QSPI_CS0] = setupConfig;

    /* Enable Memory map transfer complete interrupt */
    ptrQSPIRegs->SPI_SWITCH = CSL_FINSR(ptrQSPIRegs->SPI_SWITCH,
                                     SPI_SWITCH_MM_INT_EN_BIT_END,
                                     SPI_SWITCH_MM_INT_EN_BIT_START,
                                     1U);

    /* Set SWITCH Register to read/write in memory map interface */
    QSPISetInterfaceType(ptrQSPIRegs, QSPI_INTF_MM);
}

/**
 *  @b Description
 *  @n
 *      This function polls the status of frame transfer complete
 *
 *   @param[in] handle      QSPI handle
 *
 *  \ingroup QSPI_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      true                Frame transfer completed
 *      false               Frame transfer is not completed
 */

bool QSPI_isFrameCompleted(QSPI_Handle handle)
{
    volatile QSPIRegs*  ptrQSPIRegs;

    DebugP_assert(handle != (QSPI_Handle)NULL);

    ptrQSPIRegs = ((QSPI_Driver *)handle)->ptrRegBase;

    return ((QSPIGetStatus(ptrQSPIRegs) & (0x1U <<SPI_STATUS_FC_BIT_START)) != 0U);
}

/**
 *  @b Description
 *  @n
 *      This function polls the status of QSPI busy bit
 *
 *   @param[in] handle      QSPI handle
 *
 *  \ingroup QSPI_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      true                QSPI is in busy state
 *      false               QSPI is in free state
 */

bool QSPI_isBusy(QSPI_Handle handle)
{
    volatile QSPIRegs*  ptrQSPIRegs;

    DebugP_assert(handle != (QSPI_Handle)NULL);

    ptrQSPIRegs = ((QSPI_Driver *)handle)->ptrRegBase;

    return ((QSPIGetStatus(ptrQSPIRegs) & (0x1U << SPI_STATUS_BUSY_BIT_START ))!= 0U);
}
