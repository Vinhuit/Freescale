/*HEADER**********************************************************************
*
* Copyright 2011 Freescale Semiconductor, Inc.
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
*   Freescale TWR-MCF51JF Evaluation board.
*
*
*END************************************************************************/

#ifndef __twrmcf51jf_h__
#define __twrmcf51jf_h__    1

/*
** Get MQX configuration (also gets user_config.h)
*/
#include "mqx_cnfg.h"

/* Define the board type*/
#define BSP_TWRMCF51JF                      1

/*****************************************************************************
**  PROCESSOR MEMORY BOUNDS
*****************************************************************************/

extern unsigned char __INTERNAL_SRAM_BASE[],        __INTERNAL_SRAM_SIZE[];
extern unsigned char __INTERNAL_FLASH_BASE[],       __INTERNAL_FLASH_SIZE[];
extern unsigned char __INTERNAL_FLEXRAM_BASE[],     __INTERNAL_FLEXRAM_SIZE[];
extern unsigned char __EXTERNAL_MRAM_BASE[],        __EXTERNAL_MRAM_SIZE[];
extern unsigned char __EXTERNAL_MRAM_ROM_BASE[],    __EXTERNAL_MRAM_ROM_SIZE[];
extern unsigned char __EXTERNAL_MRAM_RAM_BASE[],    __EXTERNAL_MRAM_RAM_SIZE[];

#define BSP_INTERNAL_SRAM_BASE              ((void *)__INTERNAL_SRAM_BASE)
#define BSP_INTERNAL_SRAM_SIZE              ((uint32_t)__INTERNAL_SRAM_SIZE)
#define BSP_INTERNAL_FLASH_BASE             ((void *)__INTERNAL_FLASH_BASE)
#define BSP_INTERNAL_FLASH_SIZE             (0x00020000)
#define BSP_INTERNAL_FLASH_SECTOR_SIZE      (0x400)
#define BSP_EXTERNAL_MRAM_BASE              ((void *)__EXTERNAL_MRAM_BASE)
#define BSP_EXTERNAL_MRAM_SIZE              ((uint32_t)__EXTERNAL_MRAM_SIZE)
#define BSP_EXTERNAL_MRAM_ROM_BASE          ((void *)__EXTERNAL_MRAM_ROM_BASE)
#define BSP_EXTERNAL_MRAM_ROM_SIZE          ((uint32_t)__EXTERNAL_MRAM_ROM_SIZE)
#define BSP_EXTERNAL_MRAM_RAM_BASE          ((void *)__EXTERNAL_MRAM_RAM_BASE)
#define BSP_EXTERNAL_MRAM_RAM_SIZE          ((uint32_t)__EXTERNAL_MRAM_RAM_SIZE)

#define BSP_INTERNAL_FLEXRAM_BASE           (0x00280000)
#define BSP_INTERNAL_FLEXRAM_SIZE           (0x00000800)

#define BSP_INTERNAL_PA_RAM_BASE            BSP_INTERNAL_FLEXRAM_BASE
#define BSP_INTERNAL_PA_RAM_SIZE            BSP_INTERNAL_FLEXRAM_SIZE
#define FTFL_PROGRAM_SECTION_ALIGNMENT      FTFL_PHRASE_SIZE

#define BSP_CFCARD_BASE                     ((void *)0x30010000)
#define BSP_PCCARD_BASE                     ((void *)0x30010000)

#define INTERNAL_FLEXRAM_BASE               (BSP_INTERNAL_FLEXRAM_BASE)


/*****************************************************************************
**  Default flash protection settings
*****************************************************************************/
/* Enable modification of flash configuration bytes during loading for flash targets */
#ifndef BSPCFG_ENABLE_CFMPROTECT
    #define BSPCFG_ENABLE_CFMPROTECT        1
#endif

/*****************************************************************************
**  Default clock settings
*****************************************************************************/
/*
 * Remove old definitions of "BSP_CLOCKS" in drivers and replace
 * by runtime clock checking. Its assumed that BSP_CLOCK_CONFIGURATION_0
 * sets PLL to full speed to be compatible with old drivers.
 */

#ifndef BSP_CLOCK_CONFIGURATION_STARTUP
    #define BSP_CLOCK_CONFIGURATION_STARTUP (BSP_CLOCK_CONFIGURATION_DEFAULT)
#endif /* BSP_CLOCK_CONFIGURATION_STARTUP */


/* Init startup clock configuration is CPU_CLOCK_CONFIG_0 */
#define BSP_CLOCK_SRC                   (CPU_XTAL_CLK_HZ)
#define BSP_CORE_CLOCK                  (CPU_CORE_CLK_HZ_CONFIG_0)
#define BSP_SYSTEM_CLOCK                (CPU_CORE_CLK_HZ_CONFIG_0)
#define BSP_BUS_CLOCK                   (CPU_BUS_CLK_HZ_CONFIG_0)
#define BSP_FLEXBUS_CLOCK               (CPU_FLEXBUS_CLK_HZ_CONFIG_0)
#define BSP_FLASH_CLOCK                 (CPU_FLASH_CLK_HZ_CONFIG_0)

#define BSP_LPO_CLOCK                   (1000L)

/*****************************************************************************
**  Default settings of MQX tick timer
*****************************************************************************/

#define BSP_TIMER                           (0) /* MTIM16 */
#define BSP_TIMER_INTERRUPT_VECTOR          (Vmtim0)

/** MGCT: <category name="BSP Hardware Options"> */

/*
** MGCT: <option type="number"/>
*/
#ifndef BSPCFG_TIMER_INT_LEVEL
    #define BSPCFG_TIMER_INT_LEVEL          (1)
#endif

/*
** The clock tick rate in miliseconds - be cautious to keep this value such
** that it divides 1000 well
**
** MGCT: <option type="number" min="1" max="1000"/>
*/
#ifndef BSP_ALARM_FREQUENCY
    #define BSP_ALARM_FREQUENCY             (200)
#endif

/** MGCT: </category> */

/* Old clock rate definition in milliseconds per tick for compatibility */
#define BSP_ALARM_RESOLUTION                (1000 / BSP_ALARM_FREQUENCY)

/*
 * BSP_ALARM_RESOLUTION should be integer number without reminder.
 * otherwise performance could be degraded - functions for obtaining exact time
 * has to calculate with 64bit variables.. e.g _psp_get_elapsed_milliseconds()
 */
#if BSP_ALARM_RESOLUTION * BSP_ALARM_FREQUENCY != 1000
#warning Bsp Alarm Frequency in milliseconds is not integer - performance can be degraded
#endif

/*****************************************************************************
**  Default LPM settings
*****************************************************************************/
#define BSP_LLWU_INTERRUPT_VECTOR                   Vllwu
#define BSP_LLWU_INTERRUPT_PRIORITY                 3

#define BSP_LPM_DEPENDENCY_LEVEL_SERIAL_POLLED      (30)
#define BSP_LPM_DEPENDENCY_LEVEL_SERIAL_INT         (31)

/* HWTIMER definitions for user applications */
#define BSP_HWTIMER1_DEV        lpt_devif
#define BSP_HWTIMER1_SOURCE_CLK (CM_CLOCK_SOURCE_LPO)
#define BSP_HWTIMER1_ID         (0)

#define BSP_HWTIMER2_DEV        lpt_devif
#define BSP_HWTIMER2_SOURCE_CLK (CM_CLOCK_SOURCE_LPO)
#define BSP_HWTIMER2_ID         (1)

#define BSP_HWTIMER_LPT0_DEFAULT_PCSCLK     (1)
#define BSP_HWTIMER_LPT1_DEFAULT_PCSCLK     (1)

/*  Test for disable cpu wait mode macro.*/
#if MQX_STOP_ON_IDLE
    #warning "With MQX_STOP_ON_IDLE and no idle_task USB doesn't recover from wait mode"
#endif

/*****************************************************************************
**  Default Watchdog settings
*****************************************************************************/
// TODO rework and put initialization into __pe_initialize_hardware()
#define MCF51JF_WATCHDOG_DISABLED   0
#define MCF51JF_WATCHDOG_RESET      1
#define MCF51JF_WATCHDOG_INTERRUPT  2
#define MCF51JF_WATCHDOG_DELAY      2 /* watchdog timing delay = 2^10 * bus clock 1kHZ */
#define MCF51JF_WATCHDOG_INITIALIZATION MCF51JF_WATCHDOG_INTERRUPT

/*****************************************************************************
**  Define the location of the hardware interrupt vector table
*****************************************************************************/

typedef void (*vector_entry)(void);

extern vector_entry __VECTOR_TABLE_ROM_START[]; // defined in linker command file
extern vector_entry __VECTOR_TABLE_RAM_START[]; // defined in linker command file

#if MQX_ROM_VECTORS
    #define BSP_INTERRUPT_VECTOR_TABLE                    ((uint32_t)__VECTOR_TABLE_ROM_START)
#else
    #define BSP_INTERRUPT_VECTOR_TABLE                    ((uint32_t)__VECTOR_TABLE_RAM_START)
#endif

/*
** Define the range of interrupts for which the application can install
** interrupt routines.
*/
#ifndef BSP_FIRST_INTERRUPT_VECTOR_USED
    #define BSP_FIRST_INTERRUPT_VECTOR_USED         Vaccerr //INITSP
#endif

#ifndef BSP_LAST_INTERRUPT_VECTOR_USED
    #define BSP_LAST_INTERRUPT_VECTOR_USED          Vportfe
#endif


/*****************************************************************************
**  IO DEVICE DRIVERS CONFIGURATION
*****************************************************************************/

/** MGCT: <category name="I/O Subsystem"> */

/*
** Enable I/O subsystem features in MQX. No I/O drivers and file I/O will be
** possible without this feature.
**
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_IO_SUBSYSTEM
    #define BSPCFG_ENABLE_IO_SUBSYSTEM      1
#endif

/** MGCT: </category> */

/*****************************************************************************
**  Default TTY / UART driver settings
*****************************************************************************/
/* common settings */
#define BSP_UART_CLOCK                          (BSP_SYSTEM_CLOCK)

/** MGCT: <category name="Default Driver Installation in BSP startup"> */

/*****************************************************************************
 *  TTYA (UART0) device settings */
/*  */

/** MGCT: <category name="UART0 Settings"> */

/*
** Polled TTYA device
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_TTYA
    #define BSPCFG_ENABLE_TTYA                  1
#endif

/*
** Interrupt-driven TTYA device
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_ITTYA
    #define BSPCFG_ENABLE_ITTYA                 0
#endif

/*
** TTYA and ITTYA baud rate
** MGCT: <option type="number" min="0" max="115200"/>
*/
#ifndef BSPCFG_UART0_BAUD_RATE
    #define BSPCFG_UART0_BAUD_RATE              115200
#endif

/*
** TTYA and ITTYA buffer size
** MGCT: <option type="number" min="0" max="256"/>
*/
#ifndef BSPCFG_UART0_QUEUE_SIZE
    #define BSPCFG_UART0_QUEUE_SIZE             64
#endif

/** MGCT: </category> */

/*****************************************************************************
 *  TTYB (UART1) device settings */

/** MGCT: <category name="UART1 Settings"> */

/*
** Polled TTYB device
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_TTYB
    #define BSPCFG_ENABLE_TTYB                  0
#endif

/*
** Interrupt-driven TTYB device
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_ITTYB
    #define BSPCFG_ENABLE_ITTYB                 0
#endif

/*
** TTYB and ITTYB baud rate
** MGCT: <option type="number" min="0" max="115200"/>
*/
#ifndef BSPCFG_UART1_BAUD_RATE
    #define BSPCFG_UART1_BAUD_RATE              115200
#endif

/*
** TTYB and ITTYB buffer size
** MGCT: <option type="number" min="0" max="256"/>
*/
#ifndef BSPCFG_UART1_QUEUE_SIZE
    #define BSPCFG_UART1_QUEUE_SIZE             64
#endif

/** MGCT: </category> */

/*****************************************************************************
**  Default I2C driver settings
*****************************************************************************/
/* common settings */
#define BSP_I2C_CLOCK                               (BSP_BUS_CLOCK)

/*****************************************************************************
 *  I2C0 device settings */
#ifndef BSP_I2C0_MODE
    #define BSP_I2C0_MODE                           (I2C_MODE_MASTER)
#endif
#ifndef BSP_I2C0_BAUD_RATE
    #define BSP_I2C0_BAUD_RATE                      (100000)
#endif
#ifndef BSP_I2C0_ADDRESS
    #define BSP_I2C0_ADDRESS                        (0x6E)
#endif
#ifndef BSP_I2C0_RX_BUFFER_SIZE
    #define BSP_I2C0_RX_BUFFER_SIZE                 (64)
#endif
#ifndef BSP_I2C0_TX_BUFFER_SIZE
    #define BSP_I2C0_TX_BUFFER_SIZE                 (64)
#endif

#define BSP_I2C0_PORT_BASE                          (PCTLC_BASE_PTR)
#define BSP_I2C0_PORT_VECTOR                        (Vportdc)
#define BSP_I2C0_SDA_PIN_NUM                        (7)

/*
** Polled device
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_I2C0
    #define BSPCFG_ENABLE_I2C0                      0
#endif

/*
** Interrupt-driven device
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_II2C0
    #define BSPCFG_ENABLE_II2C0                     0
#endif

/*****************************************************************************
 *  I2C1 device settings */
#ifndef BSP_I2C1_MODE
    #define BSP_I2C1_MODE                           (I2C_MODE_MASTER)
#endif
#ifndef BSP_I2C1_BAUD_RATE
    #define BSP_I2C1_BAUD_RATE                      (100000)
#endif
#ifndef BSP_I2C1_ADDRESS
    #define BSP_I2C1_ADDRESS                        (0x6F)
#endif
#ifndef BSP_I2C1_RX_BUFFER_SIZE
    #define BSP_I2C1_RX_BUFFER_SIZE                 (64)
#endif
#ifndef BSP_I2C1_TX_BUFFER_SIZE
    #define BSP_I2C1_TX_BUFFER_SIZE                 (64)
#endif

#define BSP_I2C1_PORT_BASE                          (PCTLD_BASE_PTR)
#define BSP_I2C1_PORT_VECTOR                        (Vportdc)
#define BSP_I2C1_SDA_PIN_NUM                        (0)

/*
** Polled device
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_I2C1
    #define BSPCFG_ENABLE_I2C1                      0
#endif

/*
** Interrupt-driven device
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_II2C1
    #define BSPCFG_ENABLE_II2C1                     0
#endif

/*****************************************************************************
 * I2C2 device settings */
#ifndef BSP_I2C2_MODE
    #define BSP_I2C2_MODE                           (I2C_MODE_MASTER)
#endif
#ifndef BSP_I2C2_BAUD_RATE
    #define BSP_I2C2_BAUD_RATE                      (100000)
#endif
#ifndef BSP_I2C2_ADDRESS
    #define BSP_I2C2_ADDRESS                        (0x6E)
#endif
#ifndef BSP_I2C2_RX_BUFFER_SIZE
    #define BSP_I2C2_RX_BUFFER_SIZE                 (64)
#endif
#ifndef BSP_I2C2_TX_BUFFER_SIZE
    #define BSP_I2C2_TX_BUFFER_SIZE                 (64)
#endif

#define BSP_I2C2_PORT_BASE                          (PCTLA_BASE_PTR)
#define BSP_I2C2_PORT_VECTOR                        (Vportab)
#define BSP_I2C2_SDA_PIN_NUM                        (1)

/*
** Polled device
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_I2C2
    #define BSPCFG_ENABLE_I2C2                      0
#endif

/*
** Interrupt-driven device
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_II2C2
    #define BSPCFG_ENABLE_II2C2                     0
#endif

/*****************************************************************************
 * I2C3 device settings */
#ifndef BSP_I2C3_MODE
    #define BSP_I2C3_MODE                           (I2C_MODE_MASTER)
#endif
#ifndef BSP_I2C3_BAUD_RATE
    #define BSP_I2C3_BAUD_RATE                      (100000)
#endif
#ifndef BSP_I2C3_ADDRESS
    #define BSP_I2C3_ADDRESS                        (0x6E)
#endif
#ifndef BSP_I2C3_RX_BUFFER_SIZE
    #define BSP_I2C3_RX_BUFFER_SIZE                 (64)
#endif
#ifndef BSP_I2C3_TX_BUFFER_SIZE
    #define BSP_I2C3_TX_BUFFER_SIZE                 (64)
#endif

#define BSP_I2C3_PORT_BASE                          (PCTLE_BASE_PTR)
#define BSP_I2C3_PORT_VECTOR                        (Vportfe)
#define BSP_I2C3_SDA_PIN_NUM                        (0)

/*
** Polled device
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_I2C3
    #define BSPCFG_ENABLE_I2C3                      0
#endif

/*
** Interrupt-driven device
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_II2C3
    #define BSPCFG_ENABLE_II2C3                     0
#endif

/*
** Enable legacy II2C slave
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_LEGACY_II2C_SLAVE
    #define BSPCFG_ENABLE_LEGACY_II2C_SLAVE         1
#endif

/*****************************************************************************
**     Default SPI driver settings
*****************************************************************************/
/* common settings */
#define BSP_SPI_CLOCK                               (BSP_BUS_CLOCK)

/*****************************************************************************
 * SPI0 device settings */

 /*
** Polled device
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_SPI0
    #define BSPCFG_ENABLE_SPI0                      0
#endif

/*
** Interrupt-driven device
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_ISPI0
    #define BSPCFG_ENABLE_ISPI0                     0
#endif
#ifndef BSP_SPI0_BAUDRATE
    #define BSP_SPI0_BAUDRATE                       1000000
#endif
#ifndef BSP_SPI0_GPIO_CHIP_SELECT
    #define BSP_SPI0_GPIO_CHIP_SELECT               (GPIO_PORT_TF | GPIO_PIN0)
#endif
#ifndef BSP_SPI0_RX_BUFFER_SIZE
    #define BSP_SPI0_RX_BUFFER_SIZE                 (32)
#endif
#ifndef BSP_SPI0_TX_BUFFER_SIZE
    #define BSP_SPI0_TX_BUFFER_SIZE                 (32)
#endif


/*****************************************************************************
 * SPI1 device settings */

/*
** Polled device
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_SPI1
    #define BSPCFG_ENABLE_SPI1                      0
#endif

/*
** Interrupt-driven device
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_ISPI1
    #define BSPCFG_ENABLE_ISPI1                     0
#endif
#ifndef BSP_SPI1_BAUDRATE
    #define BSP_SPI1_BAUDRATE                       1000000
#endif
#ifndef BSP_SPI1_GPIO_CHIP_SELECT
    #define BSP_SPI1_GPIO_CHIP_SELECT               (GPIO_PORT_TA | GPIO_PIN2)
#endif
#ifndef BSP_SPI1_RX_BUFFER_SIZE
    #define BSP_SPI1_RX_BUFFER_SIZE                 (32)
#endif
#ifndef BSP_SPI1_TX_BUFFER_SIZE
    #define BSP_SPI1_TX_BUFFER_SIZE                 (32)
#endif

/*****************************************************************************
**      Default SPI Memory settings */

#define BSP_SPI_MEMORY_SPI_CS                       (SPI16_CS0)
#define BSP_SPI_MEMORY_GPIO_CS                      (BSP_SPI1_GPIO_CHIP_SELECT)
#define BSP_SPI_MEMORY_CHANNEL                      (1)
#define BSP_SPI_MUX_GPIO                            (0)

/*****************************************************************************
**      Default BSP SDCARD settings */

#define BSP_SDCARD_SPI_CHANNEL                      "spi0:"
#define BSP_SDCARD_SPI_CS                           (SPI16_CS0)
#define BSP_SDCARD_GPIO_CS                          (BSP_SPI0_GPIO_CHIP_SELECT)
#define BSP_SDCARD_GPIO_DETECT                      (GPIO_PORT_TC | GPIO_PIN4)
#define BSP_SDCARD_DETECT_MUX_GPIO                  (0)
#define BSP_SDCARD_CS_MUX_GPIO                      (0)


/*-----------------------------------------------------------------------------
**                      FlexNVM
*/
#define BSP_EE_DATA_SIZE_CODE               (FLEXNVM_EE_SPLIT | FLEXNVM_EE_SIZE_2048)
#define BSP_FLEXNVM_PART_CODE               (FLEXNVM_PART_CODE_DATA16_EE16)

/*----------------------------------------------------------------------
**                      Ethernet Info
*/
#define BSP_ENET_DEVICE_COUNT                   0

/*****************************************************************************
**      Default GPIO / LWGPIO driver settings
*****************************************************************************/

/*
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_GPIODEV
    #define BSPCFG_ENABLE_GPIODEV                   0
#endif

/* GPIO Port Interrupt Mode */

#define PORTA_GPIO_INTERRUPT_MODE   0 /* 0 means edge only, 1 means edge/level interrupt */
#define PORTB_GPIO_INTERRUPT_MODE   0 /* 0 means edge only, 1 means edge/level interrupt */
#define PORTC_GPIO_INTERRUPT_MODE   0 /* 0 means edge only, 1 means edge/level interrupt */
#define PORTD_GPIO_INTERRUPT_MODE   0 /* 0 means edge only, 1 means edge/level interrupt */
#define PORTE_GPIO_INTERRUPT_MODE   0 /* 0 means edge only, 1 means edge/level interrupt */
#define PORTF_GPIO_INTERRUPT_MODE   0 /* 0 means edge only, 1 means edge/level interrupt */

/*  BSP GPIO definitions for leds, buttons, switches, etc. */

#define BSP_LED1                                    (GPIO_PORT_TA | GPIO_PIN0)
#define BSP_LED1_MUX_GPIO                           (0)

#define BSP_LED2                                    (GPIO_PORT_TC | GPIO_PIN5)
#define BSP_LED2_MUX_GPIO                           (0)
// TODO check
#define BSP_SW1                                     (GPIO_PORT_TB | GPIO_PIN0)
#define BSP_SW1_MUX_GPIO                            (1)
#define BSP_SW1_MUX_IRQ                             (1)


/* Aliases for freq_change and lowpower example applications compatibility */
#define BSP_BUTTON1                                 BSP_SW1
#define BSP_BUTTON1_MUX_GPIO                        BSP_SW1_MUX_GPIO
#define BSP_BUTTON1_MUX_IRQ                         BSP_SW1_MUX_IRQ
#define BSP_SW2                                     BSP_SW1
#define BSP_SW2_MUX_GPIO                            BSP_SW1_MUX_GPIO
#define BSP_SW2_MUX_IRQ                             BSP_SW1_MUX_IRQ
#define BSP_BUTTON2                                 BSP_SW2
#define BSP_BUTTON2_MUX_GPIO                        BSP_SW2_MUX_GPIO
#define BSP_BUTTON2_MUX_IRQ                         BSP_SW2_MUX_IRQ

/* HMI Touch TWRPI daughter cards */
#define BSP_TWRPI_VOID   0
#define BSP_TWRPI_ROTARY 1
#define BSP_TWRPI_KEYPAD 2

/*****************************************************************************
**  Default ADC settings
*****************************************************************************/

#define BSP_ADC0_VECTOR_PRIORITY                    (3)
#define BSP_PDB_VECTOR_PRIORITY                     (3)
#define ADC_MAX_MODULES                             (1)

/*****************************************************************************
**      Default RTC driver settings
*****************************************************************************/

/*
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_RTCDEV
    #define BSPCFG_ENABLE_RTCDEV                    0
#endif

/*****************************************************************************
 *      Default FLASHX driver settings
 *****************************************************************************/
#define BSP_FLASHX_USE_FTFL                     1 /* Use FTFL or FTFE? */

/*
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_FLASHX
    #define BSPCFG_ENABLE_FLASHX                0
#endif

/*
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_PCFLASH
    #define BSPCFG_ENABLE_PCFLASH               0
#endif

/*****************************************************************************
 *      Default USB driver settings
 *****************************************************************************/

#define USBCFG_KHCI                             1

/*
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_USB
    #define BSPCFG_ENABLE_USB                   1
#endif

#define USBCFG_MAX_DRIVERS               (2)

#define USBCFG_DEFAULT_HOST_CONTROLLER    (&_bsp_usb_host_khci0_if)
#define USBCFG_DEFAULT_DEVICE_CONTROLLER  (&_bsp_usb_dev_khci0_if)

/*
** USB DCD device
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_USBDCD
    #define BSPCFG_ENABLE_USBDCD                0
#endif

/*
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_IUSBDCD
    #define BSPCFG_ENABLE_IUSBDCD               0
#endif


/*
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_ADC
    #define BSPCFG_ENABLE_ADC                   0
#endif

/*
** MGCT: <option type="bool"/>
*/
#ifndef BSPCFG_ENABLE_DAC
    #define BSPCFG_ENABLE_DAC                   0
#endif
/** MGCT: </category> */

/*****************************************************************************
 *      DEFAULT MQX INITIALIZATION DEFINITIONS
 *****************************************************************************/

 /** MGCT: <category name="Default MQX initialization parameters">  */

/* Defined in linker command file */
extern unsigned char __KERNEL_DATA_START[];
extern unsigned char __KERNEL_DATA_END[];
extern unsigned char __DEFAULT_PROCESSOR_NUMBER[];
extern unsigned char __DEFAULT_INTERRUPT_STACK_SIZE[];

#define BSP_DEFAULT_START_OF_KERNEL_MEMORY      ((void *)__KERNEL_DATA_START)
#define BSP_DEFAULT_END_OF_KERNEL_MEMORY        ((void *)__KERNEL_DATA_END)
#define BSP_DEFAULT_PROCESSOR_NUMBER            ((uint32_t)__DEFAULT_PROCESSOR_NUMBER)

#ifndef BSP_DEFAULT_INTERRUPT_STACK_SIZE
    #define BSP_DEFAULT_INTERRUPT_STACK_SIZE    ((uint32_t)__DEFAULT_INTERRUPT_STACK_SIZE)
#endif

/* MGCT: <option type="list">
** <item name="1 (all levels disabled)" value="(1L)"/>
** <item name="2" value="(2L)"/>
** <item name="3" value="(3L)"/>
** <item name="4" value="(4L)"/>
** <item name="5" value="(5L)"/>
** <item name="6" value="(6L)"/>
** <item name="7" value="(7L)"/>
** </option>
*/
#ifndef BSP_DEFAULT_MQX_HARDWARE_INTERRUPT_LEVEL_MAX
    #define BSP_DEFAULT_MQX_HARDWARE_INTERRUPT_LEVEL_MAX        (6L)
#endif

/*
** MGCT: <option type="number"/>
*/
#ifndef BSP_DEFAULT_MAX_MSGPOOLS
    #define BSP_DEFAULT_MAX_MSGPOOLS                            (2L)
#endif

/*
** MGCT: <option type="number"/>
*/
#ifndef BSP_DEFAULT_MAX_MSGQS
    #define BSP_DEFAULT_MAX_MSGQS                               (16L)
#endif


/*****************************************************************************
** Default console settings
** Other Serial console options:
** (do not forget to enable BSPCFG_ENABLE_TTY define if changed)
**     "ittya:"     OSJTAG-COM / TWR-SER interrupt mode
** MGCT: <option type="string" maxsize="256" quoted="false" allowempty="false"/>
*/
#ifndef BSP_DEFAULT_IO_CHANNEL
    #if BSPCFG_ENABLE_TTYA
        /* OSJTAG-COM / TWR-SER   polled mode   */
        #define BSP_DEFAULT_IO_CHANNEL                  "ttya:"
        #define BSP_DEFAULT_IO_CHANNEL_DEFINED
    #else
        #define BSP_DEFAULT_IO_CHANNEL                  NULL
    #endif
#else
    /* undef is for backward compatibility with user_configh.h files which have already had it defined */
    #undef  BSP_DEFAULT_IO_CHANNEL_DEFINED
    #define BSP_DEFAULT_IO_CHANNEL_DEFINED
#endif

#ifndef BSP_DEFAULT_IO_OPEN_MODE
    #define BSP_DEFAULT_IO_OPEN_MODE    (void *)(IO_SERIAL_XON_XOFF    | \
                                                  IO_SERIAL_TRANSLATION | \
                                                  IO_SERIAL_ECHO)
#endif

/*
** FLASHX flash use Programming Acceleration RAM
** MGCT: <option type="bool"/>
*/
#if BSPCFG_ENABLE_FLASHX
    #ifndef BSPCFG_FLASHX_USE_PA_RAM
        #define BSPCFG_FLASHX_USE_PA_RAM 0
    #endif
#else
    #undef  BSPCFG_FLASHX_USE_PA_RAM
    #define BSPCFG_FLASHX_USE_PA_RAM 0
#endif

/** MGCT: </category> */

#endif  /* __twrmcf51jf_h__ */
/* EOF */
