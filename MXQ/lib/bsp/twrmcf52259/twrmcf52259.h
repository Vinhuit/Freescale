/*HEADER**********************************************************************
*
* Copyright 2008 Freescale Semiconductor, Inc.
*
* This software is owned or controlled by Freescale Semiconductor.
* Use of this software is governed by the Freescale MQX RTOS License
* distributed with this Material.
* See the MQX_RTOS_LICENSE file distributed for more details.
*
* Brief License Summary:
* This software is provided in source form for you to use free of charge,
* but it is not open source software. You are allowed to use this software
* but you cannot redistribute it or derivative works of it in source form.
* The software may be used only in connection with a product containing
* a Freescale microprocessor, microcontroller, or digital signal processor.
* See license agreement file for full license terms including other
* restrictions.
*****************************************************************************
*
* Comments:
*
*   This include file is used to provide information needed by
*   an application program using the kernel running on the
*   Freescale TWR - MCF52259 Kit
*
*
*END************************************************************************/
#ifndef _twrmcf52259_h_
#define _twrmcf52259_h_ 1

/*
** Get MQX configuration (also gets user_config.h)
*/
#include "mqx_cnfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------
**                  HARDWARE INITIALIZATION DEFINITIONS
*/

/*
** Define the board type
*/
#define BSP_TWRMCF52259 1

/*
** PROCESSOR MEMORY BOUNDS
*/
extern unsigned char __INTERNAL_SRAM_BASE[],  __INTERNAL_SRAM_SIZE[];
extern unsigned char __INTERNAL_FLASH_BASE[], __INTERNAL_FLASH_SIZE[];
extern unsigned char __EXTERNAL_MRAM_BASE[], __EXTERNAL_MRAM_SIZE[];
extern unsigned char __EXTERNAL_MRAM_ROM_BASE[],  __EXTERNAL_MRAM_ROM_SIZE[];
extern unsigned char __EXTERNAL_MRAM_RAM_BASE[],  __EXTERNAL_MRAM_RAM_SIZE[];
extern unsigned char __IPSBAR[];
extern unsigned char __EXTERNAL_LCD_BASE[], __EXTERNAL_LCD_SIZE[];
extern unsigned char __EXTERNAL_LCD_DC_BASE[];

#define BSP_IPSBAR                  ((uint32_t)__IPSBAR)
#define BSP_INTERNAL_SRAM_BASE      ((void *)__INTERNAL_SRAM_BASE)
#define BSP_INTERNAL_SRAM_SIZE      ((uint32_t)__INTERNAL_SRAM_SIZE)
#define BSP_INTERNAL_FLASH_BASE     ((void *)__INTERNAL_FLASH_BASE)
#define BSP_INTERNAL_FLASH_SIZE     (0x00080000)
#define BSP_INTERNAL_FLASH_SECTOR_SIZE  (0x1000)
#define BSP_EXTERNAL_MRAM_BASE      ((void *)__EXTERNAL_MRAM_BASE)
#define BSP_EXTERNAL_MRAM_SIZE      ((uint32_t)__EXTERNAL_MRAM_SIZE)
#define BSP_EXTERNAL_MRAM_ROM_BASE  ((void *)__EXTERNAL_MRAM_ROM_BASE)
#define BSP_EXTERNAL_MRAM_ROM_SIZE  ((uint32_t)__EXTERNAL_MRAM_ROM_SIZE)
#define BSP_EXTERNAL_MRAM_RAM_BASE  ((void *)__EXTERNAL_MRAM_RAM_BASE)
#define BSP_EXTERNAL_MRAM_RAM_SIZE  ((uint32_t)__EXTERNAL_MRAM_RAM_SIZE)
#define BSP_EXTERNAL_LCD_BASE       ((void *)__EXTERNAL_LCD_BASE)
#define BSP_EXTERNAL_LCD_SIZE       ((uint32_t)__EXTERNAL_LCD_SIZE)
#define BSP_EXTERNAL_LCD_DC         ((void *)__EXTERNAL_LCD_DC_BASE)


/* Compact Flash card base address */
#define BSP_CFCARD_BASE             ((void *)0x30010000)

/* Enable modification of flash configuration bytes during loading for flash targets */
#ifndef BSPCFG_ENABLE_CFMPROTECT
    #define BSPCFG_ENABLE_CFMPROTECT        1
#endif

/*
** The timer MQX uses
*/
#define BSP_FIRST_TIMER                               (0) /* PIT0 */
#define BSP_LAST_TIMER                                (1) /* PIT1 */
#define BSP_TIMER                                     BSP_FIRST_TIMER
#define BSP_ADC_TIMER                                 BSP_LAST_TIMER


/*
** The clock tick rate in number of ticks per second
*/
#ifndef BSP_ALARM_FREQUENCY
#define BSP_ALARM_FREQUENCY                           (200)
#endif

/*
** Old clock rate definition in milliseconds per tick for compatibility
*/
#define BSP_ALARM_RESOLUTION                          (1000 / BSP_ALARM_FREQUENCY)

/*
** CLKIN speed
*/
#define BSP_SYSTEM_CLOCK                              (80000000L)
#define BSP_BUS_CLOCK                                 (BSP_SYSTEM_CLOCK/2)

/*
** System Bus Clock Info
*/
#define SYSTEM_CLOCK                    (BSP_SYSTEM_CLOCK/1000000)       /* system bus frequency in MHz */
#define SYSTEM_CLK_PERIOD               (1000000000/(BSP_SYSTEM_CLOCK))  /* system bus period in ns */

/*
** Core Watchdog Control Register (CWCR)
*/
#define BSP_WATCHDOG_DISABLED   0
#define BSP_WATCHDOG_RESET      1
#define BSP_WATCHDOG_INTERRUPT  2
#define BSP_WATCHDOG_DELAY      7 /* watchdog timing delay = 2^31 * bus clock frq */
#define BSP_WATCHDOG_INITIALIZATION BSP_WATCHDOG_INTERRUPT

/*
** The timer interrupt level and vector.
*/
#define BSP_PIT0_INT_LEVEL                            (2)
#define BSP_PIT0_INT_SUBLEVEL                         (4)
#define BSP_PIT1_INT_LEVEL                            (2)
#define BSP_PIT1_INT_SUBLEVEL                         (3)
#define BSP_PIT0_INT_VECTOR                           (MCF5225_INT_PIT0)
#define BSP_PIT1_INT_VECTOR                           (MCF5225_INT_PIT1)

#define BSP_TIMER_INTERRUPT_LEVEL                     (BSP_PIT0_INT_LEVEL)
#define BSP_TIMER_INTERRUPT_SUBLEVEL                  (BSP_PIT0_INT_SUBLEVEL)
#define BSP_TIMER_INTERRUPT_VECTOR                    (BSP_PIT0_INT_VECTOR)

#define BSP_ADC_TIMER_INTERRUPT_LEVEL                 (BSP_PIT1_INT_LEVEL)
#define BSP_ADC_TIMER_INTERRUPT_SUBLEVEL              (BSP_PIT1_INT_SUBLEVEL)
#define BSP_ADC_TIMER_INTERRUPT_VECTOR                (BSP_PIT1_INT_VECTOR)

#define BSP_ADC_INTERRUPT_LEVEL                       (BSP_ADC_TIMER_INTERRUPT_LEVEL + 1) /* cannot be interrupted by timer */
#define BSP_ADC_INTERRUPT_SUBLEVEL                    (2)
#define BSP_ADC_INTERRUPT_VECTOR                      (MCF5225_INT_ADC_ADCA)

/*
** The RTC interrupt level and vector.
*/
#define BSP_RTC_INT_LEVEL                          (2)
#define BSP_RTC_INT_SUBLEVEL                       (4)
#define BSP_RTC_PRESCALER_GOCU                     (0x0000)
#define BSP_RTC_PRESCALER_GOCL                     (0x2000)

/*
** The I2C stuff.
*/
#define BSP_I2C_CLOCK                                 (BSP_SYSTEM_CLOCK)
#define BSP_I2C0_ADDRESS                              (0x6E)
#define BSP_I2C1_ADDRESS                              (0x6F)
#define BSP_I2C0_BAUD_RATE                            (100000)
#define BSP_I2C1_BAUD_RATE                            (100000)
#define BSP_I2C0_MODE                                 (I2C_MODE_MASTER)
#define BSP_I2C1_MODE                                 (I2C_MODE_MASTER)
#define BSP_I2C0_INT_LEVEL                            (1)
#define BSP_I2C0_INT_SUBLEVEL                         (0)
#define BSP_I2C1_INT_LEVEL                            (1)
#define BSP_I2C1_INT_SUBLEVEL                         (1)
#define BSP_I2C0_RX_BUFFER_SIZE                       (64)
#define BSP_I2C0_TX_BUFFER_SIZE                       (64)
#define BSP_I2C1_RX_BUFFER_SIZE                       (64)
#define BSP_I2C1_TX_BUFFER_SIZE                       (64)

/*
** The SPI settings.
*/
#define BSP_QSPI_RX_BUFFER_SIZE                       (32)
#define BSP_QSPI_TX_BUFFER_SIZE                       (32)
#define BSP_QSPI_BAUDRATE                             (1000000)
#define BSP_QSPI_INT_LEVEL                            (4)
#define BSP_QSPI_INT_SUBLEVEL                         (0)
#define BSP_SPI_MEMORY_CHANNEL                        0

/*
** The SDCARD settings.
*/
#define BSP_SDCARD_SPI_CHANNEL                        "spi0:"
#define BSP_SDCARD_SPI_CS                             (MCF5XXX_QSPI_QDR_QSPI_CS0)
#define BSP_SDCARD_GPIO_DETECT                        (GPIO_PORT_NQ | GPIO_PIN7)
#define BSP_SDCARD_GPIO_PROTECT                       (GPIO_PORT_DD | GPIO_PIN6)
#define BSP_SDCARD_DETECT_MUX_GPIO                    (LWGPIO_MUX_NQ7_GPIO)
#define BSP_SDCARD_PROTECT_MUX_GPIO                   (LWGPIO_MUX_DD6_GPIO)
#define BSP_SDCARD_CS_MUX_GPIO                        (0)

/*
** The ethernet PHY device number 0..31
*/
#ifndef BSP_ENET0_PHY_ADDR
#define BSP_ENET0_PHY_ADDR                                  0
#endif

/*
** PHY MII Speed (MDC - Management Data Clock)
*/
#define BSP_ENET0_PHY_MII_SPEED                            (2500000L)

/*
** GPIO board specifications
*/
#define BSP_LED1    (GPIO_PORT_TC | GPIO_PIN0)
#define BSP_LED2    (GPIO_PORT_TC | GPIO_PIN1)
#define BSP_LED3    (GPIO_PORT_TC | GPIO_PIN2)
#define BSP_LED4    (GPIO_PORT_TC | GPIO_PIN3)
#define BSP_SW1     (GPIO_PORT_TA | GPIO_PIN0)
#define BSP_SW3     (GPIO_PORT_TA | GPIO_PIN1)
#define BSP_DIPSW2_1  (GPIO_PORT_DD | GPIO_PIN4)
#define BSP_DIPSW2_2  (GPIO_PORT_DD | GPIO_PIN5)
#define BSP_DIPSW2_3  (GPIO_PORT_DD | GPIO_PIN6)
#define BSP_DIPSW2_4  (GPIO_PORT_DD | GPIO_PIN7)

#define BSP_LED1_MUX_GPIO (LWGPIO_MUX_TC0_GPIO)
#define BSP_LED2_MUX_GPIO (LWGPIO_MUX_TC1_GPIO)
#define BSP_LED3_MUX_GPIO (LWGPIO_MUX_TC2_GPIO)
#define BSP_LED4_MUX_GPIO (LWGPIO_MUX_TC3_GPIO)
#define BSP_SW1_MUX_GPIO  (LWGPIO_MUX_TA0_GPIO)
#define BSP_SW2_MUX_GPIO  (LWGPIO_MUX_TA1_GPIO)
#define BSP_DIPSW2_MUX_GPIO  (LWGPIO_MUX_DD4_GPIO)


/* definitions for user applications */
#define BSP_BUTTON1   BSP_SW1
#define BSP_BUTTON2   BSP_SW3
#define BSP_SWITCH2_1 BSP_DIPSW2_1
#define BSP_SWITCH2_2 BSP_DIPSW2_2
#define BSP_SWITCH2_3 BSP_DIPSW2_3
#define BSP_SWITCH2_4 BSP_DIPSW2_4

#define BSP_BUTTON1_MUX_GPIO  BSP_SW1_MUX_GPIO
#define BSP_BUTTON2_MUX_GPIO  BSP_SW2_MUX_GPIO
#define BSP_SWITCH1_1_MUX_GPOIO  BSP_DIPSW2_MUX_GPIO

/* LCD board settings */
/* GPIO settings for navigation buttons on LCD board */
#define BSP_LCD_NAVSW_N    (GPIO_PORT_UB | GPIO_PIN2)
#define BSP_LCD_NAVSW_W    (GPIO_PORT_TA | GPIO_PIN2)
#define BSP_LCD_NAVSW_E    (GPIO_PORT_DD | GPIO_PIN5)
#define BSP_LCD_NAVSW_S    (GPIO_PORT_UB | GPIO_PIN3)
#define BSP_LCD_NAVSW_CTR  (GPIO_PORT_DD | GPIO_PIN6)

#define BSP_LCD_DC         (GPIO_PORT_TG | GPIO_PIN0)
#define BSP_LCD_DC_FN      0

/* GPIO settings for resistive touch screen on LCD board */
#define BSP_LCD_TCHRES_X_PLUS    (GPIO_PORT_AN | GPIO_PIN4)
#define BSP_LCD_TCHRES_X_MINUS   (GPIO_PORT_AN | GPIO_PIN6)
#define BSP_LCD_TCHRES_Y_PLUS    (GPIO_PORT_AN | GPIO_PIN7)
#define BSP_LCD_TCHRES_Y_MINUS   (GPIO_PORT_AN | GPIO_PIN5)

#define BSP_LCD_SPI_CHANNEL   "spi0:"

#define BSP_LCD_X_PLUS_ADC_CHANNEL  (ADC_SOURCE_AN4)
#define BSP_LCD_Y_PLUS_ADC_CHANNEL  (ADC_SOURCE_AN7)

/* ADC tchres device init struct */
#define BSP_TCHRES_ADC_DEVICE "adc:"
#define BSP_TCHRES_X_TRIGGER ADC_TRIGGER_1
#define BSP_TCHRES_Y_TRIGGER ADC_TRIGGER_2

/*
** ADC channels
*/
#define BSP_ADC_CH_ACCEL_X          (ADC_SOURCE_AN0)
#define BSP_ADC_CH_ACCEL_Y          (ADC_SOURCE_AN1)
#define BSP_ADC_CH_ACCEL_Z          (ADC_SOURCE_AN2)
#define BSP_ADC_CH_POT              (ADC_SOURCE_AN3)

/*Select Flexcan device support module*/
#define BSP_CAN_DEVICE              (0)

/********************************************************************/

/*
** Define the location of the hardware interrupt vector table
*/
typedef void (*vector_entry)(void);

extern vector_entry __VECTOR_TABLE_ROM_START[]; // defined in linker command file
extern vector_entry __VECTOR_TABLE_RAM_START[]; // defined in linker command file

#if MQX_ROM_VECTORS
    #define BSP_INTERRUPT_VECTOR_TABLE                    ((uint32_t)__VECTOR_TABLE_ROM_START)
#else
    #define BSP_INTERRUPT_VECTOR_TABLE                    ((uint32_t)__VECTOR_TABLE_RAM_START)
#endif

/*
** UART interrupt levels and vectors
*/

#define BSP_UART0_INT_LEVEL                           (5)
#define BSP_UART0_INT_SUBLEVEL                        (3)
#define BSP_UART1_INT_LEVEL                           (5)
#define BSP_UART1_INT_SUBLEVEL                        (2)
#define BSP_UART2_INT_LEVEL                           (5)
#define BSP_UART2_INT_SUBLEVEL                        (1)
#define BSP_UART0_INT_VECTOR                          (MCF5225_INT_UART0)
#define BSP_UART1_INT_VECTOR                          (MCF5225_INT_UART1)
#define BSP_UART2_INT_VECTOR                          (MCF5225_INT_UART2)

/*
** Define the range of interrupts for which the application can install
** isrs.
*/
#ifndef BSP_FIRST_INTERRUPT_VECTOR_USED
    #define BSP_FIRST_INTERRUPT_VECTOR_USED           MCF5225_INT_FIRST_EXTERNAL
#endif

#ifndef BSP_LAST_INTERRUPT_VECTOR_USED
    #define BSP_LAST_INTERRUPT_VECTOR_USED            MCF5225_INT_FLEXCAN_BOFF_INT
#endif

/*------------------------------------------------------------------------
** Gainspan Wi-Fi Device configuration macro.
*/
#if BSPCFG_ENABLE_GAINSPAN_WIFI
    /*
    ** GainSpan SPI channel and GPIO pins configuration.
    */
    #define BSP_GS_SPI_DEVICE                   "spi0:"
    /*
    ** GS1011 GPIO pins configuration.
    ** This GPIO is used by Slave GS1011 node, to indicate the data ready condition.
    ** Its ACTIVE HIGH signal . Whenever Slave wants to transmit the data, this GPIO is made HIGH.
    ** Host(MASTER) can detect interrupt on LOW_to_HIGH transition
    */
    #define BSP_GS_WAKEUP_GPIO_PIN              (LWGPIO_PORT_NQ | LWGPIO_PIN1)
    #define BSP_GS_WAKEUP_MUX_IRQ               (1)
    #define BSP_GS_WAKEUP_INT_LEVEL             (3)
    #define BSP_GS_WAKEUP_INT_SUBLEVEL          (3)

    /*
    ** GainSpan IO DEVICE DRIVERS ENABLED
    */
    #undef BSPCFG_ENABLE_SPI0
    #define BSPCFG_ENABLE_SPI0                  1

    #undef BSP_ENET_WIFI_ENABLED
    #define BSP_ENET_WIFI_ENABLED               1

    #ifndef BSP_ENET_WIFI_RX_PCB
        #define BSP_ENET_WIFI_RX_PCB            16
    #endif

    #define BSP_ENET_WIFI_WEB_PROV_ENABLED      1

    #if BSP_ENET_WIFI_WEB_PROV_ENABLED
        #undef BSPCFG_ENABLE_FLASHX
        #define BSPCFG_ENABLE_FLASHX            1
    #endif
#endif

/*-------------------------------------------------
** The Atheros Wifi settings.
*/
#if BSPCFG_ENABLE_ATHEROS_WIFI
    #define BSP_ATHEROS_WIFI_SPI_DEVICE         "spi0:"
    /* Corresponds to B62 on elevator */
    #define BSP_ATHEROS_WIFI_GPIO_INT_PIN       (LWGPIO_PORT_NQ | LWGPIO_PIN1)
    #define BSP_INT_PIN_MUX_GPIO                (1)

    #define ATHEROS_GPIO_INT_LEVEL              (3)
    #define ATHEROS_GPIO_INT_SUBLEVEL           (0)

    #undef  BSPCFG_ENABLE_SPI0
    #define BSPCFG_ENABLE_SPI0                  1

    #undef  BSP_ENET_WIFI_ENABLED
    #define BSP_ENET_WIFI_ENABLED               1

    #ifndef BSP_ENET_WIFI_RX_PCB
        #define BSP_ENET_WIFI_RX_PCB            BSP_CONFIG_ATHEROS_PCB
    #endif
#endif

/*------------------------------------------------------------------------
**                      REDPINE WIFI
*/
#if BSPCFG_ENABLE_REDPINE_WIFI
    /*
    ** Redpine SPI channel and GPIO pins configuration.
    */
    #define BSP_RSI_SPI_DEVICE                   "spi0:"

    #define BSP_RSI_WAKEUP_GPIO_PIN              (LWGPIO_PORT_NQ | LWGPIO_PIN1)
    #define BSP_RSI_WAKEUP_MUX_IRQ               (1)
    #define BSP_RSI_WAKEUP_INT_LEVEL             (3)
    #define BSP_RSI_WAKEUP_INT_SUBLEVEL          (3)

    #define BSP_RSI_POWER_GPIO_PIN               (LWGPIO_PORT_UB | LWGPIO_PIN3)
    #define BSP_RSI_POWER_MUX_GPIO               (LWGPIO_MUX_UB3_GPIO)

    #undef BSPCFG_ENABLE_SPI0
    #define BSPCFG_ENABLE_SPI0                   1

    #undef BSP_ENET_WIFI_ENABLED
    #define BSP_ENET_WIFI_ENABLED                1

    #ifndef BSP_ENET_WIFI_RX_PCB
        #define BSP_ENET_WIFI_RX_PCB            16
    #endif
#endif

/*----------------------------------------------------------------------
**                  GENERAL WIFI CONFIGURATION
*/
#ifndef BSPCFG_ENABLE_GAINSPAN_WIFI
    #define BSPCFG_ENABLE_GAINSPAN_WIFI         0
#endif

#ifndef BSPCFG_ENABLE_ATHEROS_WIFI
    #define BSPCFG_ENABLE_ATHEROS_WIFI          0
#endif

#ifndef BSPCFG_ENABLE_REDPINE_WIFI
    #define BSPCFG_ENABLE_REDPINE_WIFI          0
#endif

#ifndef BSP_ENET_WIFI_WEB_PROV_ENABLED
    #define BSP_ENET_WIFI_WEB_PROV_ENABLED     (0)
#endif

/* All device vendors have the same wifi device 
   Just one wifi vendor could be enabled at a time
*/
#ifndef BSP_ENET_WIFI_DEVICE
    #define BSP_ENET_WIFI_DEVICE                0 /* MCF5XXX_FEC_DEVICE_COUNT */
#endif

#ifndef BSP_ENET_WIFI_ENABLED
    #define BSP_ENET_WIFI_ENABLED              (0)
#endif

/*
** FEC interrupt levels and vectors
*/

#define BSP_ENET_DEVICE_COUNT                     MCF5XXX_FEC_DEVICE_COUNT
#define BSP_FEC_INT_TX_LEVEL                          (4)
#define BSP_FEC_INT_TX_SUBLEVEL                       (1)
#define BSP_FEC_INT_RX_LEVEL                          (4)
#define BSP_FEC_INT_RX_SUBLEVEL                       (0)


#define BSP_DEFAULT_ENET_DEVICE  0
#define BSP_DEFAULT_ENET_OUI     { 0x00, 0x00, 0x5E, 0, 0, 0 }

/* EPORT interrupts levels
** Note:
** EPORT interrupt sublevel (or priority within level) is hardware-wired to midpoint,
** EPORT interrupt level is hardware-wired to the values below.
** DON'T CHANGE THESE CONSTANTS AS IT'S POINTLESS.
*/
#define BSP_EPORT_EPFX_INT_SUBLEVEL_MIDPOINT          (3) /* in fact, it is value between 3 and 4 */
#define BSP_EPORT0_EPF1_INT_LEVEL                     (1)
#define BSP_EPORT0_EPF1_INT_SUBLEVEL                  (BSP_EPORT_EPFX_INT_SUBLEVEL_MIDPOINT)
#define BSP_EPORT0_EPF3_INT_LEVEL                     (3)
#define BSP_EPORT0_EPF3_INT_SUBLEVEL                  (BSP_EPORT_EPFX_INT_SUBLEVEL_MIDPOINT)
#define BSP_EPORT0_EPF5_INT_LEVEL                     (5)
#define BSP_EPORT0_EPF5_INT_SUBLEVEL                  (BSP_EPORT_EPFX_INT_SUBLEVEL_MIDPOINT)
#define BSP_EPORT0_EPF7_INT_LEVEL                     (7)
#define BSP_EPORT0_EPF7_INT_SUBLEVEL                  (BSP_EPORT_EPFX_INT_SUBLEVEL_MIDPOINT)

/* USB host controler for MCF5225x or MCF5222x */
#define USBCFG_MAX_DRIVERS               (2)

#define USBCFG_KHCI         1

#define USBCFG_DEFAULT_HOST_CONTROLLER    (&_bsp_usb_host_khci0_if)
#define USBCFG_DEFAULT_DEVICE_CONTROLLER  (&_bsp_usb_dev_khci0_if)


#ifndef BSPCFG_RX_RING_LEN
  #define BSPCFG_RX_RING_LEN             3 // Number of receive BD's
#endif

#ifndef BSPCFG_TX_RING_LEN
  #define BSPCFG_TX_RING_LEN             3 // Number of transmit BD's
#endif

#ifndef BSPCFG_ENABLE_ENET_STATS
   #define BSPCFG_ENABLE_ENET_STATS  1
#endif


#ifndef BSPCFG_ENET_RESTORE
   #define BSPCFG_ENET_RESTORE  1
#endif


#ifndef BSPCFG_ENABLE_CPP
#define BSPCFG_ENABLE_CPP   0
#endif

/*----------------------------------------------------------------------
**                  IO DEVICE DRIVERS CONFIGURATION
*/

#ifndef BSPCFG_ENABLE_IO_SUBSYSTEM
    #define BSPCFG_ENABLE_IO_SUBSYSTEM  1
#endif

/* polled TTY device (UART0) */
#ifndef BSPCFG_ENABLE_TTYA
#define BSPCFG_ENABLE_TTYA  1
#endif

/* polled TTY device (UART1) */
#ifndef BSPCFG_ENABLE_TTYB
#define BSPCFG_ENABLE_TTYB  1
#endif

/* polled TTY device (UART2) */
#ifndef BSPCFG_ENABLE_TTYC
#define BSPCFG_ENABLE_TTYC  0
#endif

/* interrupt-driven TTY device (UART0) */
#ifndef BSPCFG_ENABLE_ITTYA
#define BSPCFG_ENABLE_ITTYA 0
#endif

/* interrupt-driven TTY device (UART1) */
#ifndef BSPCFG_ENABLE_ITTYB
#define BSPCFG_ENABLE_ITTYB 0
#endif

/* interrupt-driven TTY device (UART2) */
#ifndef BSPCFG_ENABLE_ITTYC
#define BSPCFG_ENABLE_ITTYC 0
#endif

/* TTY device HW signals (UART0) */
#ifndef BSPCFG_ENABLE_TTYA_HW_SIGNALS
#define BSPCFG_ENABLE_TTYA_HW_SIGNALS 0
#endif

/* TTY device HW signals (UART1) */
#ifndef BSPCFG_ENABLE_TTYB_HW_SIGNALS
#define BSPCFG_ENABLE_TTYB_HW_SIGNALS 0
#endif

/* TTY device HW signals (UART2) */
#ifndef BSPCFG_ENABLE_TTYC_HW_SIGNALS
#define BSPCFG_ENABLE_TTYC_HW_SIGNALS 0
#endif

/* TTYA and ITTYA baud rate */
#ifndef BSPCFG_UART0_BAUD_RATE
#define BSPCFG_UART0_BAUD_RATE 115200
#endif

/* TTYB and ITTYB baud rate */
#ifndef BSPCFG_UART1_BAUD_RATE
#define BSPCFG_UART1_BAUD_RATE 115200
#endif

/* TTYC and ITTYC baud rate */
#ifndef BSPCFG_UART2_BAUD_RATE
#define BSPCFG_UART2_BAUD_RATE 115200
#endif

/* TTYA and ITTYA buffer size */
#ifndef BSPCFG_UART0_QUEUE_SIZE
#define BSPCFG_UART0_QUEUE_SIZE 64
#endif

/* TTYB and ITTYB buffer size */
#ifndef BSPCFG_UART1_QUEUE_SIZE
#define BSPCFG_UART1_QUEUE_SIZE 64
#endif

/* TTYC and ITTYC buffer size */
#ifndef BSPCFG_UART2_QUEUE_SIZE
#define BSPCFG_UART2_QUEUE_SIZE 64
#endif

/* TTYA and ITTYA address */
#ifndef BSPCFG_UART0_STATION_ADDR
#define BSPCFG_UART0_STATION_ADDR      (0)
#endif

/* TTYB and ITTYB address */
#ifndef BSPCFG_UART1_STATION_ADDR
#define BSPCFG_UART1_STATION_ADDR      (1)
#endif

/* TTYC and ITTYC address */
#ifndef BSPCFG_UART2_STATION_ADDR
#define BSPCFG_UART2_STATION_ADDR      (2)
#endif

/* polled I2C0 device */
#ifndef BSPCFG_ENABLE_I2C0
#define BSPCFG_ENABLE_I2C0 1
#endif

/* polled I2C1 device */
#ifndef BSPCFG_ENABLE_I2C1
#define BSPCFG_ENABLE_I2C1 0
#endif

/* polled SPI */
#ifndef BSPCFG_ENABLE_SPI0
#define BSPCFG_ENABLE_SPI0      0
#endif

/* int SPI */
#ifndef BSPCFG_ENABLE_ISPI0
#define BSPCFG_ENABLE_ISPI0     0
#endif

/* GPIO device */
#ifndef BSPCFG_ENABLE_GPIODEV
#define BSPCFG_ENABLE_GPIODEV  0
#endif

/* RTC device */
#ifndef BSPCFG_ENABLE_RTCDEV
#define BSPCFG_ENABLE_RTCDEV  1
#endif

/* CFM */
#ifndef BSPCFG_ENABLE_FLASHX
#define BSPCFG_ENABLE_FLASHX        1
#endif

/* TCHSCR_RESISTIVE device */
#ifndef BSPCFG_ENABLE_TCHSRES
#define BSPCFG_ENABLE_TCHSRES 0
#endif
/*----------------------------------------------------------------------
**                  DEFAULT MQX INITIALIZATION DEFINITIONS
*/

/* Defined in link.xxx */
extern unsigned char __KERNEL_DATA_START[];
extern unsigned char __KERNEL_DATA_END[];
extern unsigned char __DEFAULT_PROCESSOR_NUMBER[];
extern unsigned char __DEFAULT_INTERRUPT_STACK_SIZE[];

#define BSP_DEFAULT_START_OF_KERNEL_MEMORY                  ((void *)__KERNEL_DATA_START)
#define BSP_DEFAULT_END_OF_KERNEL_MEMORY                    ((void *)__KERNEL_DATA_END)
#define BSP_DEFAULT_PROCESSOR_NUMBER                        ((uint32_t)__DEFAULT_PROCESSOR_NUMBER)

#ifndef BSP_DEFAULT_INTERRUPT_STACK_SIZE
    #define BSP_DEFAULT_INTERRUPT_STACK_SIZE                ((uint32_t)__DEFAULT_INTERRUPT_STACK_SIZE)
#endif

#ifndef BSP_DEFAULT_MQX_HARDWARE_INTERRUPT_LEVEL_MAX
    #define BSP_DEFAULT_MQX_HARDWARE_INTERRUPT_LEVEL_MAX    (6L)
#endif

#ifndef BSP_DEFAULT_MAX_MSGPOOLS
    #define BSP_DEFAULT_MAX_MSGPOOLS                        (16L)
#endif

#ifndef BSP_DEFAULT_MAX_MSGQS
    #define BSP_DEFAULT_MAX_MSGQS                           (16L)
#endif

#ifndef BSP_DEFAULT_IO_CHANNEL
    #if BSPCFG_ENABLE_TTYB
        #define BSP_DEFAULT_IO_CHANNEL                      "ttyb:"
    #define BSP_DEFAULT_IO_CHANNEL_DEFINED
    #else
        #define BSP_DEFAULT_IO_CHANNEL                      NULL
    #endif
#else
    /* undef is for backward compatibility with user_configh.h files which have already had it defined */
    #undef  BSP_DEFAULT_IO_CHANNEL_DEFINED
    #define BSP_DEFAULT_IO_CHANNEL_DEFINED
#endif

#ifndef BSP_DEFAULT_IO_OPEN_MODE
    #define BSP_DEFAULT_IO_OPEN_MODE                        (void *) (IO_SERIAL_XON_XOFF | IO_SERIAL_TRANSLATION | IO_SERIAL_ECHO)
#endif

#if BSPCFG_ENABLE_TTYC
    #define BSP_SERIAL_MULTIDROP_DEVICE                     "ttyc:"
#endif

/*
** CFM internal flash
*/
#if BSPCFG_ENABLE_FLASHX
    #ifndef BSPCFG_FLASHX_SIZE
    #define BSPCFG_FLASHX_SIZE      0x4000
    #endif
#else
    #undef  BSPCFG_FLASHX_SIZE
    #define BSPCFG_FLASHX_SIZE      0x0
#endif

#ifdef __cplusplus
}
#endif

#endif
/* EOF */
