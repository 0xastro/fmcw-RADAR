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
*  @file       pinmux_xwr68xx.h
*
*  @brief      PINMUX pad settings for 68xx device
*
*  ============================================================================
*/

#ifndef ti_drivers_PINMUX_xwr68xx__include
#define ti_drivers_PINMUX_xwr68xx__include
    
#ifdef __cplusplus
    extern "C" {
#endif




/** @addtogroup PINMUX_DRIVER_PAD_FUNCTIONS_68XX
 *
 * @brief
 *  PAD Functionality
 *
 @{ */

/** @name PINP13_PADAA
*  PINP13_PADAA functionality
*/
/**@{*/
#define SOC_XWR68XX_PINP13_PADAA                     0U
#define SOC_XWR68XX_PINP13_PADAA_GPIO_12             0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINP13_PADAA_SPI_HOST_INTR       1U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINP13_PADAA_SPIB_CSN1           6U    /*< SPI Channel B Chip Select*/
/**@}*/ /*PINP13_PADAA*/

/** @name PINH13_PADAB 
*   PINH13_PADAB functionality
*/
/**@{*/
#define SOC_XWR68XX_PINH13_PADAB                     1U
#define SOC_XWR68XX_PINH13_PADAB_GPIO_13             0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINH13_PADAB_GPIO_0              1U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINH13_PADAB_PMIC_CLKOUT         2U    /*< Output Clock from XWR68xx device for PMIC*/
#define SOC_XWR68XX_PINH13_PADAB_EPWM1_B             10U    /*< PWM Module 1 - OutPut B*/
#define SOC_XWR68XX_PINH13_PADAB_EPWM2_A             11U    /*< PWM Module 2 - OutPut A*/
/**@}*/ /*PINH13_PADAB*/

/** @name PINJ13_PADAC
 *  PINJ13_PADAC functionality
 */
/**@{*/
#define SOC_XWR68XX_PINJ13_PADAC                     2U
#define SOC_XWR68XX_PINJ13_PADAC_GPIO_16             0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINJ13_PADAC_GPIO_1              1U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINJ13_PADAC_SYNC_OUT            2U    /*< Low Frequency Synchronization Signal output*/
#define SOC_XWR68XX_PINJ13_PADAC_DMM_MUXIN          12U    /*< Debug Interface (Hardware In Loop) Mux Select between 
                                                              DMM1 and DMM2 (Two Instances)*/
#define SOC_XWR68XX_PINJ13_PADAC_SPIB_CSN1           13U    /*< SPI Channel B Chip Select (Instance ID 1)*/
#define SOC_XWR68XX_PINJ13_PADAC_SPIB_CSN2           14U    /*< SPI Channel B Chip Select (Instance ID 2)*/
#define SOC_XWR68XX_PINJ13_PADAC_EPWM1_SYNCI         15U    /*< PWM Module 1 - Input Synchronization Signal*/
/**@}*/ /*PINJ13_PADAC*/

/** @name PIND13_PADAD
 *  PIND13_PADAD functionality
 */
/**@{*/
#define SOC_XWR68XX_PIND13_PADAD                     3U
#define SOC_XWR68XX_PIND13_PADAD_GPIO_19             0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PIND13_PADAD_SPIA_MOSI           1U    /*< SPI Channel A - Master Out Slave In*/
#define SOC_XWR68XX_PIND13_PADAD_CANFD_RX            2U    /*< CAN FD (MCAN) Receive Signal*/
#define SOC_XWR68XX_PIND13_PADAD_DSS_UART_TX         8U    /*< Debug UART Transmit [DSP]*/
/**@}*/ /*PIND13_PADAD*/

/** @name PINE14_PADAE
 *  PINE14_PADAE functionality
 */
/**@{*/
#define SOC_XWR68XX_PINE14_PADAE                     4U
#define SOC_XWR68XX_PINE14_PADAE_GPIO_20             0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINE14_PADAE_SPIA_MISO           1U    /*< SPI Channel A - Master In Slave Out*/
#define SOC_XWR68XX_PINE14_PADAE_CANFD_TX            2U    /*< CAN FD (MCAN) Transmit Signal*/
/**@}*/ /*PINE14_PADAE*/

/** @name PINE13_PADAF
 *  PINE13_PADAF functionality
 */
/**@{*/
#define SOC_XWR68XX_PINE13_PADAF                     5U
#define SOC_XWR68XX_PINE13_PADAF_GPIO_3              0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINE13_PADAF_SPIA_CLK            1U    /*< SPI Channel A - Clock*/
#define SOC_XWR68XX_PINE13_PADAF_DSS_UART_TX         7U    /*< Debug UART Transmit [DSP]*/
/**@}*/ /*PINE13_PADAF*/

/** @name PINE15_PADAG
 *  PINE15_PADAG functionality
 */
/**@{*/
#define SOC_XWR68XX_PINE15_PADAG                     6U
#define SOC_XWR68XX_PINE15_PADAG_GPIO_30             0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINE15_PADAG_SPIA_CSN            1U    /*< SPI Channel A - Chip Select*/
/**@}*/ /*PINE15_PADAG*/

/** @name PINF13_PADAH
 *  PINF13_PADAH functionality
 */
/**@{*/
#define SOC_XWR68XX_PINF13_PADAH                     7U
#define SOC_XWR68XX_PINF13_PADAH_GPIO_21             0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINF13_PADAH_SPIB_MOSI           1U    /*< SPI Channel B - Master Out Slave In*/
#define SOC_XWR68XX_PINF13_PADAH_I2C_SDA             2U    /*< I2C Data */
/**@}*/ /*PINF13_PADAH*/

/** @name PING14_PADAI
 *  PING14_PADAI functionality
 */
/**@{*/
#define SOC_XWR68XX_PING14_PADAI                     8U
#define SOC_XWR68XX_PING14_PADAI_GPIO_22             0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PING14_PADAI_SPIB_MISO           1U    /*< SPI Channel B - Master In Slave Out*/
#define SOC_XWR68XX_PING14_PADAI_I2C_SCL             2U    /*< I2C Clock*/
#define SOC_XWR68XX_PING14_PADAI_DSS_UART_TX         6U    /*< Debug UART Transmit [DSP]*/
/**@}*/ /*PING14_PADAI*/

/** @name PINF14_PADAJ
 *  PINF14_PADAJ functionality
 */
/**@{*/
#define SOC_XWR68XX_PINF14_PADAJ                     9U
#define SOC_XWR68XX_PINF14_PADAJ_GPIO_5              0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINF14_PADAJ_SPIB_CLK            1U    /*< SPI Channel B - Clock*/
#define SOC_XWR68XX_PINF14_PADAJ_MSS_UARTA_RX        2U    /*< Master Subsystem  - UART A Receive*/
#define SOC_XWR68XX_PINF14_PADAJ_MSS_UARTB_TX        6U    /*< Master Subsystem  - UART B Transmit*/
#define SOC_XWR68XX_PINF14_PADAJ_BSS_UART_TX         7U    /*< Debug UART Transmit [Radar Block]*/
#define SOC_XWR68XX_PINF14_PADAJ_CANFD_RX            8U    /*< CAN FD (MCAN) Receive Signal*/
/**@}*/ /*PINF14_PADAJ*/

/** @name PINH14_PADAK
 *  PINH14_PADAK functionality
 */
/**@{*/
#define SOC_XWR68XX_PINH14_PADAK                     10U
#define SOC_XWR68XX_PINH14_PADAK_GPIO_4              0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINH14_PADAK_SPIB_CSN            1U    /*< SPI Channel B Chip Select (Instance ID 0)*/
#define SOC_XWR68XX_PINH14_PADAK_MSS_UARTA_TX        2U    /*< Master Subsystem  - UART A Transmit*/
#define SOC_XWR68XX_PINH14_PADAK_MSS_UARTB_TX        6U    /*< Master Subsystem  - UART B Transmit*/
#define SOC_XWR68XX_PINH14_PADAK_BSS_UART_TX         7U    /*< Debug UART Transmit [Radar Block]*/
#define SOC_XWR68XX_PINH14_PADAK_QSPI_CLKEXT         8U    /*< QSPI Clock (Used with Serial Data Flash)*/
#define SOC_XWR68XX_PINH14_PADAK_CANFD_TX            9U    /*< CAN FD (MCAN) Transmit Signal*/
/**@}*/ /*PINH14_PADAK*/

/** @name PINR13_PADAL
 *  PINR13_PADAL functionality
 */
/**@{*/
#define SOC_XWR68XX_PINR13_PADAL                     11U
#define SOC_XWR68XX_PINR13_PADAL_GPIO_8              0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINR13_PADAL_QSPI_D0              1U    /*< QSPI Data Line #0 (Used with Serial Data Flash)*/
#define SOC_XWR68XX_PINR13_PADAL_SPIB_MISO           2U    /*< SPI Channel B - Master In Slave Out*/
/**@}*/ /*PINR13_PADAL*/

/** @name PINN12_PADAM
 *  PINN12_PADAM functionality
 */
/**@{*/
#define SOC_XWR68XX_PINN12_PADAM                     12U
#define SOC_XWR68XX_PINN12_PADAM_GPIO_9              0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINN12_PADAM_QSPI_D1              1U    /*< QSPI Data Line #1 (Used with Serial Data Flash)*/
#define SOC_XWR68XX_PINN12_PADAM_SPIB_MOSI           2U    /*< SPI Channel B - Master Out Slave In*/
#define SOC_XWR68XX_PINN12_PADAM_SPIB_CSN2           8U    /*< SPI Channel B Chip Select (Instance ID 2)*/
/**@}*/ /*PINN12_PADAM*/

/** @name PINR14_PADAN
 *  PINR14_PADAN functionality
 */
/**@{*/
#define SOC_XWR68XX_PINR14_PADAN                     13U
#define SOC_XWR68XX_PINR14_PADAN_GPIO_10             0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINR14_PADAN_QSPI_D2              1U    /*< QSPI Data Line #2 (Used with Serial Data Flash)*/
#define SOC_XWR68XX_PINR14_PADAN_CANFD_TX            8U    /*< CAN FD (MCAN) Transmit Signal*/
/**@}*/ /*PINR14_PADAN*/

/** @name PINP12_PADAO
 *  PINP12_PADAO functionality
 */
/**@{*/
#define SOC_XWR68XX_PINP12_PADAO                     14U
#define SOC_XWR68XX_PINP12_PADAO_GPIO_11             0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINP12_PADAO_QSPI_D3              1U    /*< QSPI Data Line #3 (Used with Serial Data Flash)*/
#define SOC_XWR68XX_PINP12_PADAO_CANFD_RX            8U    /*< CAN FD (MCAN) Receive Signal*/
/**@}*/ /*PINP12_PADAO*/

/** @name PINR12_PADAP
 *  PINR12_PADAP functionality
 */
/**@{*/
#define SOC_XWR68XX_PINR12_PADAP                     15U
#define SOC_XWR68XX_PINR12_PADAP_GPIO_7              0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINR12_PADAP_QSPI_CLK            1U    /*< QSPI Clock (Used with Serial Data Flash)*/
#define SOC_XWR68XX_PINR12_PADAP_SPIB_CLK            2U    /*< SPI Channel B - Clock*/
#define SOC_XWR68XX_PINR12_PADAP_DSS_UART_TX         6U    /*< Debug UART Transmit [DSP]*/
/**@}*/ /*PINR12_PADAP*/

/** @name PINP11_PADAQ
 *  PINP11_PADAQ functionality
 */
/**@{*/
#define SOC_XWR68XX_PINP11_PADAQ                     16U
#define SOC_XWR68XX_PINP11_PADAQ_GPIO_6              0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINP11_PADAQ_QSPI_CSN            1U    /*< QSPI Chip Select (Used with Serial Data Flash)*/
#define SOC_XWR68XX_PINP11_PADAQ_SPIB_CSN            2U    /*< SPI Channel B Chip Select (Instance ID 0)*/
/**@}*/ /*PINP11_PADAQ*/

/** @name PINN7_PADAR 
 *  PINN7_PADAR  functionality
 */
/**@{*/
#define SOC_XWR68XX_PINN7_PADAR                      17U
#define SOC_XWR68XX_PINN7_PADAR_NERROR_IN            0U    /*< Failsafe input to the device. Nerror output from any other device 
                                                                can be concentrated in the error signaling monitor module inside the 
                                                                device and appropriate action can be taken by Firmware*/
/**@}*/ /*PINN7_PADAR_*/

/** @name PINN9_PADAS 
 *  PINN9_PADAS  functionality
 */
/**@{*/
#define SOC_XWR68XX_PINN9_PADAS                      18U
#define SOC_XWR68XX_PINN9_PADAS_WARM_RESET           0U    /*< Open drain fail safe warm reset signal. Can be driven from PMIC for 
                                                                diagnostic or can be used as status signal that the device is going 
                                                                through reset.*/
/**@}*/ /*PINN9_PADAS_*/

/** @name PINN6_PADAT 
 *  PINN6_PADAT  functionality
 */
/**@{*/
#define SOC_XWR68XX_PINN6_PADAT                      19U
#define SOC_XWR68XX_PINN6_PADAT_NERROR_OUT           0U    /*< Open drain fail safe output signal. Connected to PMIC/Processor/MCU 
                                                              to indicate that some severe criticality fault has happened. Recovery 
                                                              would be through reset.*/
/**@}*/ /*PINN6_PADAT_*/

/** @name PINP10_PADAU
 *  PINP10_PADAU functionality
 */
/**@{*/
#define SOC_XWR68XX_PINP10_PADAU                     20U
#define SOC_XWR68XX_PINP10_PADAU_GPIO_17             0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINP10_PADAU_TCK                 1U    /*< JTAG Test Clock*/
#define SOC_XWR68XX_PINP10_PADAU_MSS_UARTB_TX        2U    /*< Master Subsystem  - UART B Transmit*/
#define SOC_XWR68XX_PINP10_PADAU_CANFD_TX            8U    /*< CAN FD (MCAN) Transmit Signal*/
/**@}*/ /*PINP10_PADAU*/

/** @name PINN10_PADAV
 *  PINN10_PADAV functionality
 */
/**@{*/
#define SOC_XWR68XX_PINN10_PADAV                     21U
#define SOC_XWR68XX_PINN10_PADAV_GPIO_18             0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINN10_PADAV_TMS                 1U    /*< JTAG Test Mode Signal*/
#define SOC_XWR68XX_PINN10_PADAV_BSS_UART_TX         2U    /*< Debug UART Transmit [Radar Block]*/
#define SOC_XWR68XX_PINN10_PADAV_CANFD_RX            6U    /*< CAN FD (MCAN) Receive Signal*/
/**@}*/ /*PINN10_PADAV*/

/** @name PINR11_PADAW
 *  PINR11_PADAW functionality
 */
/**@{*/
#define SOC_XWR68XX_PINR11_PADAW                     22U
#define SOC_XWR68XX_PINR11_PADAW_GPIO_23             0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINR11_PADAW_TDI                 1U    /*< JTAG Test Data Input */
#define SOC_XWR68XX_PINR11_PADAW_MSS_UARTA_RX        2U    /*< Master Subsystem  - UART A Receive*/
/**@}*/ //Subsystem  -

/** @name PINN13_PADAX
 *  PINN13_PADAX functionality
 */
/**@{*/
#define SOC_XWR68XX_PINN13_PADAX                     23U
#define SOC_XWR68XX_PINN13_PADAX_GPIO_24             0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINN13_PADAX_TDO                 1U    /*< JTAG Test Data Output*/
#define SOC_XWR68XX_PINN13_PADAX_MSS_UARTA_TX        2U    /*< Master Subsystem  - UART A Transmit*/
#define SOC_XWR68XX_PINN13_PADAX_MSS_UARTB_TX        6U    /*< Master Subsystem  - UART B Transmit*/
#define SOC_XWR68XX_PINN13_PADAX_BSS_UART_TX         7U    /*< Debug UART Transmit [Radar Block]*/
#define SOC_XWR68XX_PINN13_PADAX_NDMM_EN             9U    /*< Debug Interface (Hardware In Loop) Enable - Active Low Signal*/
/**@}*/ /*PINN13_PADAX*/

/** @name PINN8_PADAY 
 *  PINN8_PADAY  functionality
 */
/**@{*/
#define SOC_XWR68XX_PINN8_PADAY                      24U
#define SOC_XWR68XX_PINN8_PADAY_GPIO_25              0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINN8_PADAY_MCU_CLKOUT           1U    /*< Programmable clock given out to external MCU or the processor*/
#define SOC_XWR68XX_PINN8_PADAY_EPWM1_A              12U    /*< PWM Module 1 - OutPut A*/
/**@}*/ /*PINN8_PADAY_*/

/** @name PINK13_PADAZ
 *  PINK13_PADAZ functionality
 */
/**@{*/
#define SOC_XWR68XX_PINK13_PADAZ                     25U
#define SOC_XWR68XX_PINK13_PADAZ_GPIO_26             0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINK13_PADAZ_GPIO_2              1U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINK13_PADAZ_OSC_CLKOUT          2U    /*< Reference clock output from clocking sub system after cleanup PLL*/
#define SOC_XWR68XX_PINK13_PADAZ_MSS_UARTB_TX        7U    /*< Master Subsystem  - UART B Transmit*/
#define SOC_XWR68XX_PINK13_PADAZ_BSS_UART_TX         8U    /*< Debug UART Transmit [Radar Block]*/
#define SOC_XWR68XX_PINK13_PADAZ_SYNC_OUT            9U    /*< Low Frequency Synchronization Signal output*/
#define SOC_XWR68XX_PINK13_PADAZ_PMIC_CLKOUT         10U    /*< Output Clock from XWR68xx device for PMIC*/
/**@}*/ /*PINK13_PADAZ*/

/** @name PINP9_PADBA 
 *  PINP9_PADBA  functionality
 */
/**@{*/
#define SOC_XWR68XX_PINP9_PADBA                      26U
#define SOC_XWR68XX_PINP9_PADBA_GPIO_27              0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINP9_PADBA_PMIC_CLKOUT          1U    /*< Output Clock from XWR68xx device for PMIC*/
#define SOC_XWR68XX_PINP9_PADBA_EPWM1_B              11U    /*< PWM Module 1 - OutPut B*/
#define SOC_XWR68XX_PINP9_PADBA_EPWM2_A              12U    /*< PWM Module 2- OutPut A*/
/**@}*/ /*PINP9_PADBA_*/

/** @name PINP4_PADBB 
 *  PINP4_PADBB  functionality
 */
/**@{*/
#define SOC_XWR68XX_PINP4_PADBB                      27U
#define SOC_XWR68XX_PINP4_PADBB_GPIO_28              0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINP4_PADBB_SYNC_IN              1U    /*< Low frequency Synchronization signal input */
#define SOC_XWR68XX_PINP4_PADBB_MSS_UARTB_RX         6U    /*< Master Subsystem  - UART B Receiver*/
#define SOC_XWR68XX_PINP4_PADBB_DMM_MUXIN            7U    /*< Debug Interface (Hardware In Loop) Mux Select between DMM1 and DMM2 (Two Instances)*/
#define SOC_XWR68XX_PINP4_PADBB_SYNC_OUT             9U    /*< Low Frequency Synchronization Signal output*/
/**@}*/ /*PINP4_PADBB_*/

/** @name PING13_PADBC
 *  PING13_PADBC functionality
 */
/**@{*/
#define SOC_XWR68XX_PING13_PADBC                     28U
#define SOC_XWR68XX_PING13_PADBC_GPIO_29             0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PING13_PADBC_SYNC_OUT            1U    /*< Low Frequency Synchronization Signal output*/
#define SOC_XWR68XX_PING13_PADBC_DMM_MUXIN           9U    /*< Debug Interface (Hardware In Loop) Mux Select between DMM1 and DMM2 (Two Instances)*/
#define SOC_XWR68XX_PING13_PADBC_SPIB_CSN1           10U    /*< SPI Channel B Chip Select (Instance ID 1)*/
#define SOC_XWR68XX_PING13_PADBC_SPIB_CSN2           11U    /*< SPI Channel B Chip Select (Instance ID 2)*/
/**@}*/ /*PING13_PADBC*/

/** @name PINN4_PADBD 
 *  PINN4_PADBD  functionality
 */
/**@{*/
#define SOC_XWR68XX_PINN4_PADBD                      29U
#define SOC_XWR68XX_PINN4_PADBD_GPIO_15              0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINN4_PADBD_RS232_RX             1U    /*< Debug UART (Operates as Bus Master) - Receive Signal*/
#define SOC_XWR68XX_PINN4_PADBD_MSS_UARTA_RX         2U    /*< Master Subsystem  - UART A Receive*/
#define SOC_XWR68XX_PINN4_PADBD_BSS_UART_TX          6U    /*< Debug UART Transmit [Radar Block]*/
#define SOC_XWR68XX_PINN4_PADBD_MSS_UARTB_RX         7U    /*< Master Subsystem  - UART B Receive*/
#define SOC_XWR68XX_PINN4_PADBD_CANFD_RX             8U    /*< CAN FD (MCAN) Receive Signal*/
#define SOC_XWR68XX_PINN4_PADBD_I2C_SCL              9U    /*< I2C Clock*/
#define SOC_XWR68XX_PINN4_PADBD_EPWM2_A              10U    /*< PWM Module 2 - OutPut A*/
#define SOC_XWR68XX_PINN4_PADBD_EPWM2_B              11U    /*< PWM Module 2- OutPut B*/
#define SOC_XWR68XX_PINN4_PADBD_EPWM3_A              12U    /*< PWM Module 3 - OutPut A*/
/**@}*/ /*PINN4_PADBD_*/

/** @name PINN5_PADBE 
 *  PINN5_PADBE  functionality
 */
/**@{*/
#define SOC_XWR68XX_PINN5_PADBE                      30U
#define SOC_XWR68XX_PINN5_PADBE_GPIO_14              0U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINN5_PADBE_RS232_TX             1U    /*< Debug UART (Operates as Bus Master) - Receive Signal*/
#define SOC_XWR68XX_PINN5_PADBE_MSS_UARTA_TX         5U    /*< Master Subsystem  - UART A Transmit*/
#define SOC_XWR68XX_PINN5_PADBE_MSS_UARTB_TX         6U    /*< Master Subsystem  - UART B Transmit*/
#define SOC_XWR68XX_PINN5_PADBE_BSS_UART_TX          7U    /*< Debug UART Transmit [Radar Block]*/
#define SOC_XWR68XX_PINN5_PADBE_CANFD_TX             10U    /*< CAN FD (MCAN) Transmit Signal*/
#define SOC_XWR68XX_PINN5_PADBE_I2C_SDA              11U    /*< I2C Data */
#define SOC_XWR68XX_PINN5_PADBE_EPWM1_A              12U    /*< PWM Module 1 - OutPut A*/
#define SOC_XWR68XX_PINN5_PADBE_EPWM1_B              13U    /*< PWM Module 1 - OutPut B*/
#define SOC_XWR68XX_PINN5_PADBE_NDMM_EN              14U    /*< Debug Interface (Hardware In Loop) Enable - Active Low Signal*/
#define SOC_XWR68XX_PINN5_PADBE_EPWM2_A              15U    /*< PWM Module 2 - OutPut A*/
/**@}*/ /*PINN5_PADBE_*/

/** @name PINR4_PADBF 
 *  PINR4_PADBF  functionality
 */
/**@{*/
#define SOC_XWR68XX_PINR4_PADBF                      31U
#define SOC_XWR68XX_PINR4_PADBF_TRACE_DATA0         0U    /*< Debug Trace Output - Data Line*/
#define SOC_XWR68XX_PINR4_PADBF_GPIO_31              1U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINR4_PADBF_DMM0                 2U    /*< Debug Interface (Hardware In Loop)  - Data Line*/
#define SOC_XWR68XX_PINR4_PADBF_MSS_UARTA_TX         4U    /*< Master Subsystem  - UART A Transmit*/
/**@}*/ /*PINR4_PADBF_*/

/** @name PINP5_PADBG 
 *  PINP5_PADBG  functionality
 */
/**@{*/
#define SOC_XWR68XX_PINP5_PADBG                      32U
#define SOC_XWR68XX_PINP5_PADBG_TRACE_DATA1         0U    /*< Debug Trace Output - Data Line*/
#define SOC_XWR68XX_PINP5_PADBG_GPIO_32              1U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINP5_PADBG_DMM1                 2U    /*< Debug Interface (Hardware In Loop)  - Data Line*/
/**@}*/ /*PINP5_PADBG_*/

/** @name PINR5_PADBH 
 *  PINR5_PADBH  functionality
 */
/**@{*/
#define SOC_XWR68XX_PINR5_PADBH                      33U
#define SOC_XWR68XX_PINR5_PADBH_TRACE_DATA2         0U    /*< Debug Trace Output - Data Line*/
#define SOC_XWR68XX_PINR5_PADBH_GPIO_33              1U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINR5_PADBH_DMM2                 2U    /*< Debug Interface (Hardware In Loop)  - Data Line*/
/**@}*/ /*PINR5_PADBH_*/

/** @name PINP6_PADBI 
 *  PINP6_PADBI  functionality
 */
/**@{*/
#define SOC_XWR68XX_PINP6_PADBI                      34U
#define SOC_XWR68XX_PINP6_PADBI_TRACE_DATA3         0U    /*< Debug Trace Output - Data Line*/
#define SOC_XWR68XX_PINP6_PADBI_GPIO_34              1U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINP6_PADBI_DMM3                 2U    /*< Debug Interface (Hardware In Loop)  - Data Line*/
#define SOC_XWR68XX_PINP6_PADBI_EPWM3_SYNCO          4U    /*< */
/**@}*/ /*PINP6_PADBI_*/

/** @name PINR7_PADBJ 
 *  PINR7_PADBJ  functionality
 */
/**@{*/
#define SOC_XWR68XX_PINR7_PADBJ                      35U
#define SOC_XWR68XX_PINR7_PADBJ_TRACE_DATA4         0U    /*< Debug Trace Output - Data Line*/
#define SOC_XWR68XX_PINR7_PADBJ_GPIO_35              1U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINR7_PADBJ_DMM4                 2U    /*< Debug Interface (Hardware In Loop)  - Data Line*/
#define SOC_XWR68XX_PINR7_PADBJ_EPWM2_SYNCO          4U    /*< */
/**@}*/ /*PINR7_PADBJ_*/

/** @name PINP7_PADBK 
 *  PINP7_PADBK  functionality
 */
/**@{*/
#define SOC_XWR68XX_PINP7_PADBK                      36U
#define SOC_XWR68XX_PINP7_PADBK_TRACE_DATA5         0U    /*< Debug Trace Output - Data Line*/
#define SOC_XWR68XX_PINP7_PADBK_GPIO_36              1U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINP7_PADBK_DMM5                 2U    /*< Debug Interface (Hardware In Loop)  - Data Line*/
#define SOC_XWR68XX_PINP7_PADBK_MSS_UARTB_TX         5U    /*< Master Subsystem  - UART B Transmit*/
/**@}*/ /*PINP7_PADBK_*/

/** @name PINR8_PADBL 
 *  PINR8_PADBL  functionality
 */
/**@{*/
#define SOC_XWR68XX_PINR8_PADBL                      37U
#define SOC_XWR68XX_PINR8_PADBL_TRACE_DATA6         0U    /*< Debug Trace Output - Data Line*/
#define SOC_XWR68XX_PINR8_PADBL_GPIO_37              1U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINR8_PADBL_DMM6                 2U    /*< Debug Interface (Hardware In Loop)  - Data Line*/
#define SOC_XWR68XX_PINR8_PADBL_BSS_UART_TX          5U    /*< Debug UART Transmit [Radar Block]*/
/**@}*/ /*PINR8_PADBL_*/

/** @name PINP8_PADBM 
 *  PINP8_PADBM  functionality
 */
/**@{*/
#define SOC_XWR68XX_PINP8_PADBM                      38U
#define SOC_XWR68XX_PINP8_PADBM_TRACE_DATA7         0U    /*< Debug Trace Output - Data Line*/
#define SOC_XWR68XX_PINP8_PADBM_GPIO_38              1U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINP8_PADBM_DMM7                 2U    /*< Debug Interface (Hardware In Loop)  - Data Line*/
#define SOC_XWR68XX_PINP8_PADBM_DSS_UART_TX          5U    /*< Debug UART Transmit [DSP]*/
/**@}*/ /*PINP8_PADBM_*/

/** @name PINN15_PADBV
 *  PINN15_PADBV functionality
 */
/**@{*/
#define SOC_XWR68XX_PINN15_PADBV                     47U
#define SOC_XWR68XX_PINN15_PADBV_TRACE_CLK           0U    /*< Debug Trace Output - Clock*/
#define SOC_XWR68XX_PINN15_PADBV_GPIO_47             1U    /*< General Purpose IO*/
#define SOC_XWR68XX_PINN15_PADBV_DMM_CLK             2U    /*< Debug Interface (Hardware In Loop)  - Clock*/
/**@}*/ /*PINN15_PADBV*/

/** @name PINN14_PADBW
 *  PINN14_PADBW functionality
 */
/**@{*/
#define SOC_XWR68XX_PINN14_PADBW                     48U
#define SOC_XWR68XX_PINN14_PADBW_TRACE_CTL           0U    /*< Debug Trace Output - Control*/
#define SOC_XWR68XX_PINN14_PADBW_DMM_SYNC            2U    /*< Debug Interface (Hardware In Loop)  - Sync*/
/**@}*/ /*PINN14_PADBW*/



/** @}*/

#define PINMUX_NUM_PADS                                 SOC_XWR68XX_PINN14_PADBW


#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_PINMUX_xwr68xx__include */


