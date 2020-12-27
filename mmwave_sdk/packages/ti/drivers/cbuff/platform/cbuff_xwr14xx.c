/**
 *   @file  cbuff_xwr14xx.c
 *
 *   @brief
 *      The file implements the Platform specific CBUFF Driver Interface
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
#include <string.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/cbuff/include/cbuff_internal.h>

/**************************************************************************
 *************************** Global Variables *****************************
 **************************************************************************/
/**
 * @brief   This is the XWR14xx Hardware attributes which are used by the
 * CBUFF driver
 */
CBUFF_Hw_Attrib gCBUFFHwAttribute =
{
    SOC_XWR14XX_MSS_CBUFF_BASE_ADDRESS,
    EDMA3_DSS_FIFO_BASE,
    {
        EDMA_TPCC0_REQ_CBUFF_0,
        EDMA_TPCC0_REQ_CBUFF_1,
        EDMA_TPCC0_REQ_CBUFF_2,
        EDMA_TPCC0_REQ_CBUFF_3,
        EDMA_TPCC0_REQ_CBUFF_4,
        EDMA_TPCC0_REQ_CBUFF_5,
        EDMA_TPCC0_REQ_CBUFF_6
    },
    EDMA3_DSS_ADCBUFF_BASE,
    /***************************************************************
     * Chirp Parameters:
     * - Single Chirp Mode
     * - In Interleaved mode the Chirp Parameters are stored from
     *   CPREG4 to CPREG7
     * - Each Chirp Parameter is 4 bytes or 2 CBUFF Units
     ***************************************************************/
    {
        (EDMA3_DSS_CP1_BASE + 0*4U),
        (EDMA3_DSS_CP1_BASE + 1*4U),
        (EDMA3_DSS_CP1_BASE + 2*4U),
        (EDMA3_DSS_CP1_BASE + 3*4U)
    },
    /***************************************************************
     * Chirp Parameters:
     * - Single Chirp Mode
     * - In Non-Interleaved mode the Chirp Parameters are stored from
     *   CPREG0 to CPREG3
     * - Each Chirp Parameter is 4 bytes or 2 CBUFF Units
     ***************************************************************/
    {
        (EDMA3_DSS_CP0_BASE + 0*4U),
        (EDMA3_DSS_CP0_BASE + 1*4U),
        (EDMA3_DSS_CP0_BASE + 2*4U),
        (EDMA3_DSS_CP0_BASE + 3*4U)
    },
    /***************************************************************
     * Chirp Parameters:
     * - Multiple Chirp Mode which is NOT supported on XWR14xx
     ***************************************************************/
    {
        0x0U,
        0x0U,
        0x0U,
        0x0U,
        0x0U,
        0x0U,
        0x0U,
        0x0U
    },
    SOC_XWR14XX_MSS_CSI_PROT_ENG_BASE_ADDRESS,
    4U,
    SOC_XWR14XX_DSS_CBUFF_ERR_INTR,
    SOC_XWR14XX_DSS_CBUFF_IRQ,
    SOC_XWR14XX_DSS_FRAME_START_IRQ,
    /***************************************************************
     * Chirp Mode:
     *  XWR14xx supports only single chirp mode.
     ***************************************************************/
    1U,
    1U
};

/**
 * @brief   This is the CBUFF supported high speed interfaces for XWR14xx
 */
CBUFF_InterfaceFxn gCBUFFInterfaceFxn[] =
{
    {
        CBUFF_Interface_CSI,                /* CSI Interface                        */
        &CBUFF_initCSI,                     /* Initialize the CSI Interface         */
        &CBUFF_deinitCSI,                   /* Deinitialize the CSI Interface       */
        &CBUFF_openCSI,                     /* Open the CSI Interface               */
        &CBUFF_closeCSI,                    /* Close the CSI Interface              */
        &CBUFF_initCSILinkListParams,       /* CSI Init Linked List Parameters      */
        &CBUFF_finalizeCSILinkListParams,   /* CSI Finalize Linked List Parameters  */
    },
    {
        CBUFF_Interface_LVDS,               /* LVDS Interface                       */
        &CBUFF_initLVDS,                    /* Initialize the LVDS Interface        */
        &CBUFF_deinitLVDS,                  /* Deinitialize the LVDS Interface      */
        &CBUFF_openLVDS,                    /* Open the LVDS Interface              */
        &CBUFF_closeLVDS,                   /* Close the LVDS Interface             */
        &CBUFF_initLVDSLinkListParams,      /* LVDS Init Linked List Parameters     */
        &CBUFF_finalizeLVDSLinkListParams   /* LVDS Finalize Linked List Parameters */
    },
    {
        CBUFF_Interface_CSI,            /* Dummy not used                   */
        NULL,                           /* NULL: Indicates end of the table */
        NULL,                           /* NULL: Indicates end of the table */
        NULL,                           /* NULL: Indicates end of the table */
        NULL                            /* NULL: Indicates end of the table */
    }
};

/**
 * @brief   This is the function table which supports all the interleaved transfer functions.
 * The default driver library supports all data formats in the interleaved mode. Modify this
 * table and set to NULL any data format that is not required. Rebuild the driver and this
 * will help reduce the application footprint.
 */
CBUFF_setupTransferFxn gInterleavedTransferFxn[] =
{
    CBUFF_setupInterleaved_ADC,                 /* Data Format: CBUFF_DataFmt_ADC_DATA          */
    CBUFF_setupInterleaved_CP_ADC,              /* Data Format: CBUFF_DataFmt_CP_ADC            */
    CBUFF_setupInterleaved_ADC_CP,              /* Data Format: CBUFF_DataFmt_ADC_CP            */
    CBUFF_setupInterleaved_CP_ADC_CQ,           /* Data Format: CBUFF_DataFmt_CP_ADC_CQ         */
    CBUFF_setupInterleaved_ADC_USER,            /* Data Format: CBUFF_DataFmt_ADC_USER          */
    CBUFF_setupInterleaved_CP_ADC_CQ_USER       /* Data Format: CBUFF_DataFmt_CP_ADC_CQ_USER    */
};

/**
 * @brief   This is the function table which supports all the non-interleaved transfer functions.
 * The default driver library supports all data formats in the non-interleaved mode. Modify this
 * table and set to NULL any data format that is not required. Rebuild the driver and this
 * will help reduce the application footprint.
 */
CBUFF_setupTransferFxn gNonInterleavedTransferFxn[] =
{
    CBUFF_setupNonInterleaved_ADC,                 /* Data Format: CBUFF_DataFmt_ADC_DATA          */
    CBUFF_setupNonInterleaved_CP_ADC,              /* Data Format: CBUFF_DataFmt_CP_ADC            */
    CBUFF_setupNonInterleaved_ADC_CP,              /* Data Format: CBUFF_DataFmt_ADC_CP            */
    CBUFF_setupNonInterleaved_CP_ADC_CQ,           /* Data Format: CBUFF_DataFmt_CP_ADC_CQ         */
    CBUFF_setupNonInterleaved_ADC_USER,            /* Data Format: CBUFF_DataFmt_ADC_USER          */
    CBUFF_setupNonInterleaved_CP_ADC_CQ_USER       /* Data Format: CBUFF_DataFmt_CP_ADC_CQ_USER    */
};

/**
 * @brief   This is the function table which supports all the non-interleaved transfer functions
 * in Multiple Chirp Mode. This functionality is NOT available on the XWR14xx
 */
CBUFF_setupTransferFxn gMultipleChirpNonInterleavedTransferFxn[] =
{
    NULL,                                           /* Data Format: CBUFF_DataFmt_ADC_DATA          */
    NULL,                                           /* Data Format: CBUFF_DataFmt_CP_ADC            */
    NULL,                                           /* Data Format: CBUFF_DataFmt_ADC_CP            */
    NULL,                                           /* Data Format: CBUFF_DataFmt_CP_ADC_CQ         */
    NULL,                                           /* Data Format: CBUFF_DataFmt_ADC_USER          */
    NULL                                            /* Data Format: CBUFF_DataFmt_CP_ADC_CQ_USER    */
};
