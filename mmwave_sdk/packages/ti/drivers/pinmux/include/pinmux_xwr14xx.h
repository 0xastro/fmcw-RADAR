/*
 * Copyright (c) 2015, Texas Instruments Incorporated
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
*  @file       pinmux_xwr14xx.h
*
*  @brief      PINMUX pad settings for 14xx device
*
*  ============================================================================
*/

#ifndef ti_drivers_PINMUX_xwr14xx__include
#define ti_drivers_PINMUX_xwr14xx__include
    
#ifdef __cplusplus
    extern "C" {
#endif


/** @addtogroup PINMUX_DRIVER_PAD_FUNCTIONS_14XX
 *
 * @brief
 *  PAD Functionality
 *
 @{ */
     
/** @name PINP6_PADAA
*  PINP6  PADAA functionality
*/
/**@{*/
#define SOC_XWR14XX_PINP6_PADAA                      0U
#define SOC_XWR14XX_PINP6_PADAA_GPIO_12              0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINP6_PADAA_SPI_HOST1_INTR       1U    /*< General Purpose IO [XWR14XX]*/
/**@}*/ /*PINP6_PADAA*/

/** @name PINN4_PADAB 
*   PINN4_PADAB functionality
*/
/**@{*/
#define SOC_XWR14XX_PINN4_PADAB                      1U
#define SOC_XWR14XX_PINN4_PADAB_GPIO_13              0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINN4_PADAB_GPIO_0               1U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINN4_PADAB_PMIC_CLKOUT          2U    /*< Dithered Clock Output for PMIC*/
/**@}*/ /*PINN4_PADAB*/

/** @name PINN7_PADAC 
*   PINN7_PADAC functionality
*/
/**@{*/
#define SOC_XWR14XX_PINN7_PADAC                      2U
#define SOC_XWR14XX_PINN7_PADAC_GPIO_16              0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINN7_PADAC_GPIO_1               1U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINN7_PADAC_SYNC_OUT             2U    /*< Low Frequency Synchronization Signal output*/
/**@}*/ /*PINN7_PADAC*/

/** @name PINR8_PADAD 
*   PINR8_PADAD functionality
*/
/**@{*/
#define SOC_XWR14XX_PINR8_PADAD                      3U
#define SOC_XWR14XX_PINR8_PADAD_GPIO_19              0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINR8_PADAD_SPIA_MOSI            1U    /*< SPI Channel A - Master Out Slave In*/
#define SOC_XWR14XX_PINR8_PADAD_CAN_RX               2U    /*< CAN Interface*/
/**@}*/ /*PINR8_PADAD*/

/** @name PINP5_PADAE 
*   PINP5_PADAE functionality
*/
/**@{*/
#define SOC_XWR14XX_PINP5_PADAE                      4U
#define SOC_XWR14XX_PINP5_PADAE_GPIO_20              0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINP5_PADAE_SPIA_MISO            1U    /*< SPI Channel A - Master In Slave Out*/
#define SOC_XWR14XX_PINP5_PADAE_CAN_TX               2U    /*< CAN Interface*/
/**@}*/ /*PINP5_PADAE*/

/** @name PINR9_PADAF 
*   PINR9_PADAF functionality
*/
/**@{*/
#define SOC_XWR14XX_PINR9_PADAF                      5U
#define SOC_XWR14XX_PINR9_PADAF_GPIO_3               0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINR9_PADAF_SPIA_CLK             1U    /*< SPI Channel A - Clock*/
/**@}*/ /*PINR9_PADAF*/

/** @name PINR7_PADAG 
*   PINR7_PADAG functionality
*/
/**@{*/
#define SOC_XWR14XX_PINR7_PADAG                      6U
#define SOC_XWR14XX_PINR7_PADAG_GPIO_30              0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINR7_PADAG_SPIA_CS              1U    /*< SPI Channel A - Chip Select*/
/**@}*/ /*PINR7_PADAG*/

/** @name PINR3_PADAH 
*   PINR3_PADAH functionality
*/
/**@{*/
#define SOC_XWR14XX_PINR3_PADAH                      7U
#define SOC_XWR14XX_PINR3_PADAH_GPIO_21              0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINR3_PADAH_SPIB_MOSI            1U    /*< SPI Channel B - Master Out Slave In*/
#define SOC_XWR14XX_PINR3_PADAH_I2C_SDA              2U    /*< I2C Data*/
/**@}*/ /*PINR3_PADAH*/

/** @name PINP4_PADAI 
*   PINP4_PADAI functionality
*/
/**@{*/
#define SOC_XWR14XX_PINP4_PADAI                      8U
#define SOC_XWR14XX_PINP4_PADAI_GPIO_22              0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINP4_PADAI_SPIB_MISO            1U    /*< SPI Channel B - Master In Slave Out*/
#define SOC_XWR14XX_PINP4_PADAI_I2C_SCL              2U    /*< I2C Clock*/
/**@}*/ /*PINP4_PADAI*/

/** @name PINR5_PADAJ 
*   PINR5_PADAJ functionality
*/
/**@{*/
#define SOC_XWR14XX_PINR5_PADAJ                      9U
#define SOC_XWR14XX_PINR5_PADAJ_GPIO_5               0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINR5_PADAJ_SPIB_CLK             1U    /*< SPI Channel B - Clock*/
#define SOC_XWR14XX_PINR5_PADAJ_MSS_UARTA_RX         2U    /*< Master Subsystem  - UART A Receive*/
#define SOC_XWR14XX_PINR5_PADAJ_MSS_UARTB_TX         6U    /*< Master Subsystem  - UART B Transmit*/
#define SOC_XWR14XX_PINR5_PADAJ_BSS_UART_TX          7U    /*< Debug UART Transmit [Radar Block]*/
/**@}*/ /*PINR5_PADAJ*/

/** @name PINR4_PADAK 
*   PINR4_PADAK functionality
*/
/**@{*/
#define SOC_XWR14XX_PINR4_PADAK                      10U
#define SOC_XWR14XX_PINR4_PADAK_GPIO_4               0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINR4_PADAK_SPIB_CS              1U    /*< SPI Channel B Chip Select (Instance ID 0)*/
#define SOC_XWR14XX_PINR4_PADAK_MSS_UARTA_TX         2U    /*< Master Subsystem  - UART A Transmit*/
#define SOC_XWR14XX_PINR4_PADAK_MSS_UARTB_TX         6U    /*< Master Subsystem  - UART B Transmit*/
#define SOC_XWR14XX_PINR4_PADAK_BSS_UART_TX          7U    /*< Debug UART Transmit [Radar Block]*/
/**@}*/ /*PINR4_PADAK*/

/** @name PINR11_PADAL 
*   PINR11_PADAL functionality
*/
/**@{*/
#define SOC_XWR14XX_PINR11_PADAL                     11U
#define SOC_XWR14XX_PINR11_PADAL_GPIO_8              0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINR11_PADAL_QSPI_D0              1U    /*< QSPI Data IN/OUT*/
#define SOC_XWR14XX_PINR11_PADAL_SPIB_MISO           2U    /*< SPI Channel B - Master In Slave Out*/
/**@}*/ /*PINR11_PADAL*/

/** @name PINP9_PADAM 
*   PINP9_PADAM functionality
*/
/**@{*/
#define SOC_XWR14XX_PINP9_PADAM                      12U
#define SOC_XWR14XX_PINP9_PADAM_GPIO_9               0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINP9_PADAM_QSPI_D1               1U    /*< QSPI Data IN/OUT*/
#define SOC_XWR14XX_PINP9_PADAM_SPIB_MOSI            2U    /*< SPI Channel B - Master Out Slave In*/
/**@}*/ /*PINP9_PADAM*/

/** @name PINR12_PADAN 
*   PINR12_PADAN functionality
*/
/**@{*/
#define SOC_XWR14XX_PINR12_PADAN                     13U
#define SOC_XWR14XX_PINR12_PADAN_GPIO_10             0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINR12_PADAN_QSPI_D2              1U    /*< QSPI Data IN/OUT*/
/**@}*/ /*PINR12_PADAN*/

/** @name PINP10_PADAO 
*   PINP10_PADAO functionality
*/
/**@{*/
#define SOC_XWR14XX_PINP10_PADAO                     14U
#define SOC_XWR14XX_PINP10_PADAO_GPIO_11             0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINP10_PADAO_QSPI_D3              1U    /*< QSPI Data IN/OUT*/
/**@}*/ /*PINP10_PADA*/

/** @name PINR10_PADAP 
*   PINR10_PADAP functionality
*/
/**@{*/
#define SOC_XWR14XX_PINR10_PADAP                     15U
#define SOC_XWR14XX_PINR10_PADAP_GPIO_7              0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINR10_PADAP_QSPI_CLK            1U    /*< QSPI Clock output from the device.Device operates as a master with the serial flash being a slave*/
#define SOC_XWR14XX_PINR10_PADAP_SPIB_CLK            2U    /*< SPI Channel B - Clock*/
/**@}*/ /*PINR10_PADAP*/

/** @name PINP8_PADAQ 
*   PINP8_PADAQ functionality
*/
/**@{*/
#define SOC_XWR14XX_PINP8_PADAQ                      16U
#define SOC_XWR14XX_PINP8_PADAQ_GPIO_6               0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINP8_PADAQ_QSPI_CS              1U    /*< QSPI Chip Select output from the device.Device operates as a master with the serial flash being a slave*/
#define SOC_XWR14XX_PINP8_PADAQ_SPIB_CS              2U    /*< SPI Channel B Chip Select (Instance ID 0)*/
/**@}*/ /*PINP8_PADAQ*/

/** @name PINP7_PADAR 
*   PINP7_PADAR functionality
*/
/**@{*/
#define SOC_XWR14XX_PINP7_PADAR                      17U
#define SOC_XWR14XX_PINP7_PADAR_NERROR_IN            0U    /*< Failsafe input to the device. Nerror output from any other device can be concentrated in the error signaling monitor module inside the device and appropriate action can be taken by Firmware*/
/**@}*/ /*PINP7_PADAR*/

/** @name PINN12_PADAS 
*   PINN12_PADAS functionality
*/
/**@{*/
#define SOC_XWR14XX_PINN12_PADAS                     18U
#define SOC_XWR14XX_PINN12_PADAS_WARM_RESET          0U    /*< Open drain fail safe warm reset signal. Can be driven from PMIC for diagnostic or can be used as status signal that the device is going through reset.*/
/**@}*/ /*PINN12_PADAS*/

/** @name PINN8_PADAT 
*   PINN8_PADAT functionality
*/
/**@{*/
#define SOC_XWR14XX_PINN8_PADAT                      19U
#define SOC_XWR14XX_PINN8_PADAT_NERROR_OUT           0U    /*< Open drain fail safe output signal. Connected to PMIC/Processor/MCU to indicate that some severe criticality fault has happened. Recovery would be through reset.*/
/**@}*/ /*PINN8_PADAT*/

/** @name PINM13_PADAU 
*   PINM13_PADAU functionality
*/
/**@{*/
#define SOC_XWR14XX_PINM13_PADAU                     20U
#define SOC_XWR14XX_PINM13_PADAU_GPIO_17             0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINM13_PADAU_TCK                 1U    /*< JTAG Clock*/
#define SOC_XWR14XX_PINM13_PADAU_MSS_UARTB_TX        2U    /*< Master Subsystem  - UART B Transmit*/
#define SOC_XWR14XX_PINM13_PADAU_BSS_UART_RX         6U    /*< Debug UART Receive [Radar Block]*/
/**@}*/ /*PINM13_PADAU*/

/** @name PINL13_PADAV 
*   PINL13_PADAV functionality
*/
/**@{*/
#define SOC_XWR14XX_PINL13_PADAV                     21U
#define SOC_XWR14XX_PINL13_PADAV_GPIO_18             0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINL13_PADAV_TMS                 1U    /*< JTAG Test Mode Select*/
#define SOC_XWR14XX_PINL13_PADAV_BSS_UART_TX         2U    /*< Debug UART Transmit [Radar Block]*/
/**@}*/ /*PINL13_PADAV*/

/** @name PINH13_PADAW 
*   PINH13_PADAW functionality
*/
/**@{*/
#define SOC_XWR14XX_PINH13_PADAW                         22U
#define SOC_XWR14XX_PINH13_PADAW_GPIO_23                 0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINH13_PADAW_TDI                     1U    /*< JTAG Test Data In*/
#define SOC_XWR14XX_PINH13_PADAW_MSS_UARTA_RX            2U    /*< Master Subsystem  - UART A Receive*/
/**@}*/ /*PINH13_PADAW*/

/** @name PINJ13_PADAX
*   PINJ13_PADAX functionality 
*/
/**@{*/
#define SOC_XWR14XX_PINJ13_PADAX                         23U
#define SOC_XWR14XX_PINJ13_PADAX_GPIO_24                 0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINJ13_PADAX_TDO                     1U    /*< JTAG Test Data Out*/
#define SOC_XWR14XX_PINJ13_PADAX_MSS_UARTA_TX            2U    /*< Master Subsystem  - UART A Transmit*/
#define SOC_XWR14XX_PINJ13_PADAX_MSS_UARTB_TX            6U    /*< Master Subsystem  - UART B Transmit*/
#define SOC_XWR14XX_PINJ13_PADAX_BSS_UART_TX             7U    /*< Debug UART Transmit [Radar Block]*/
/**@}*/ /*PINJ13_PADAX*/

/** @name PINN9_PADAY 
*   PINN9_PADAY functionality
*/
/**@{*/
#define SOC_XWR14XX_PINN9_PADAY                      24U
#define SOC_XWR14XX_PINN9_PADAY_GPIO_25              0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINN9_PADAY_MCU_CLKOUT           1U    /*< Programmable clock given out to external MCU or the processor*/
#define SOC_XWR14XX_PINN9_PADAY_BSS_UART_RX          10U    /*< Debug UART Receive [Radar Block]*/
/**@}*/ /*PINN9_PADAY*/

/** @name PINN13_PADAZ
*   PINN13_PADAZ functionality
*/
/**@{*/
#define SOC_XWR14XX_PINN13_PADAZ                         25U
#define SOC_XWR14XX_PINN13_PADAZ_GPIO_26                 0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINN13_PADAZ_GPIO_2                  1U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINN13_PADAZ_MSS_UARTB_TX            7U    /*< Master Subsystem  - UART B Transmit*/
#define SOC_XWR14XX_PINN13_PADAZ_BSS_UART_TX             8U    /*< Debug UART Transmit [Radar Block]*/
#define SOC_XWR14XX_PINN13_PADAZ_SYNC_OUT                9U    /*< Low frequency Synchronization signal output*/
#define SOC_XWR14XX_PINN13_PADAZ_PMIC_CLKOUT             10U    /*< Dithered clock input to PMIC*/
/**@}*/ /*PINN13_PADAZ*/

/** @name PINP13_PADBA 
*   PINP13_PADBA functionality
*/
/**@{*/
#define SOC_XWR14XX_PINP13_PADBA                         26U
#define SOC_XWR14XX_PINP13_PADBA_GPIO_27                 0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINP13_PADBA_PMIC_CLKOUT             1U    /*< Dithered Clock Output for PMIC*/
/**@}*/ /*PINP13_PADBA*/

/** @name PINN10_PADBB 
*   PINN10_PADBB functionality
*/
/**@{*/
#define SOC_XWR14XX_PINN10_PADBB                         27U
#define SOC_XWR14XX_PINN10_PADBB_GPIO_28                 0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINN10_PADBB_SYNC_IN                 1U    /*< Low frequency Synchronization signal input */
#define SOC_XWR14XX_PINN10_PADBB_MSS_UARTB_RX            6U    /*< Master Subsystem  - UART B Receiver*/
/**@}*/ /*PINN10_PADBB*/

/** @name PINP11_PADBC 
*   PINP11_PADBC functionality
*/
/**@{*/
#define SOC_XWR14XX_PINP11_PADBC                     28U
#define SOC_XWR14XX_PINP11_PADBC_GPIO_29             0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINP11_PADBC_SYNC_OUT            1U    /*< Low frequency Synchronization signal output*/
/**@}*/ /*PINP11_PADBC*/

/** @name PINN5_PADBD 
*   PINN5_PADBD functionality
*/
/**@{*/
#define SOC_XWR14XX_PINN5_PADBD                      29U
#define SOC_XWR14XX_PINN5_PADBD_GPIO_15              0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINN5_PADBD_RS232_RX             1U    /*< Debug UART (Operates as Bus Master) - Receive Signal*/
#define SOC_XWR14XX_PINN5_PADBD_MSS_UARTA_RX         2U    /*< Master Subsystem  - UART A Receive*/
#define SOC_XWR14XX_PINN5_PADBD_BSS_UART_TX          6U    /*< Debug UART Transmit [Radar Block]*/
#define SOC_XWR14XX_PINN5_PADBD_MSS_UARTB_RX         7U    /*< Master Subsystem  - UART B Receive*/
/**@}*/ /*PINN5_PADBD*/

/** @name PINN6_PADBE 
*   PINN6_PADBE functionality
*/
/**@{*/
#define SOC_XWR14XX_PINN6_PADBE                      30U
#define SOC_XWR14XX_PINN6_PADBE_GPIO_14              0U    /*< General Purpose IO*/
#define SOC_XWR14XX_PINN6_PADBE_RS232_TX             1U    /*< Debug UART (Operates as Bus Master) - Receive Signal*/
#define SOC_XWR14XX_PINN6_PADBE_MSS_UARTA_TX         5U    /*< Master Subsystem  - UART A Transmit*/
#define SOC_XWR14XX_PINN6_PADBE_MSS_UARTB_TX         6U    /*< Master Subsystem  - UART B Transmit*/
#define SOC_XWR14XX_PINN6_PADBE_BSS_UART_TX          7U    /*< Debug UART Transmit [Radar Block]*/
/**@}*/ /*PINN6_PADBE*/


/** @}*/

#define PINMUX_NUM_PADS                                 SOC_XWR14XX_PINN6_PADBE

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_PINMUX_xwr14xx__include */

