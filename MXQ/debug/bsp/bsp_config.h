/*HEADER**********************************************************************
*
* Copyright 2014 Freescale Semiconductor, Inc.
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
*   Freescale FRDM-KL43Z and TWR-KL43Z boards.
*
*
*END************************************************************************/

#ifndef __bsp_config_h__
#define __bsp_config_h__ 1


/* Define the board type */
#define BSP_FRDM_KL43Z                    1   /* Freedom board */
//#define BSP_TWR_KL43Z                     1   /* Tower board */

#if (BSP_FRDM_KL43Z + BSP_TWR_KL43Z > 1)
#error Cannot enable two platforms simultaneously
#endif

#if (PLATFORM_LPUART_ENABLED && BSP_TWR_KL43Z)
#error TWR_KL43Z requires PLATFORM_LPUART_ENABLED disabled in mqx_sdk_config.h
#endif
/*
** PROCESSOR MEMORY BOUNDS
*/

typedef void (*vector_entry)(void);

#ifdef __CC_ARM
/* Keil compiler */
extern unsigned char Image$$USEDFLASH_END$$Base[];
#define __INTERNAL_SRAM_BASE    0x1FFFE000
#define __INTERNAL_SRAM_SIZE    0x00008000
#define __SRAM_POOL             (void *)0x1FFFE000

#define __DEFAULT_PROCESSOR_NUMBER        1
#define __DEFAULT_INTERRUPT_STACK_SIZE    512

extern unsigned char Image$$VECTORS$$Base[];
#define __VECTOR_TABLE_ROM_START ((void *)Image$$VECTORS$$Base)

extern unsigned char Image$$RAM_VECTORS$$Base[];
#define __VECTOR_TABLE_RAM_START ((void *)Image$$RAM_VECTORS$$Base)

#else  /* __CC_ARM */
extern unsigned char __INTERNAL_SRAM_BASE[],        __INTERNAL_SRAM_SIZE[];

extern unsigned long __SRAM_POOL[];

extern unsigned char __INTERNAL_FLEXNVM_BASE[];
extern unsigned char __INTERNAL_FLEXNVM_SIZE[];

extern vector_entry __VECTOR_TABLE_RAM_START[]; /* defined in linker command file */
extern vector_entry __VECTOR_TABLE_ROM_START[]; /* defined in linker command file */

extern unsigned char __DEFAULT_PROCESSOR_NUMBER[];
extern unsigned char __DEFAULT_INTERRUPT_STACK_SIZE[];
#endif /* __CC_ARM */


/* Enable modification of flash configuration bytes during loading for flash targets */
#ifndef BSPCFG_ENABLE_CFMPROTECT
    #define BSPCFG_ENABLE_CFMPROTECT        1
#endif
#if !BSPCFG_ENABLE_CFMPROTECT && defined(__ICCARM__)
    #error Cannot disable CFMPROTECT field on IAR compiler. Please define BSPCFG_ENABLE_CFMPROTECT to 1.
#endif

/* Init startup clock configuration is CPU_CLOCK_CONFIG_0 */

/** MGCT: <category name="BSP Hardware Options"> */

/*
 ** System timer definitions
 */
#define BSP_SYSTIMER_DEV          kSystickDevif
#define BSP_SYSTIMER_ID           0
#define BSP_SYSTIMER_SRC_CLK      kCoreClock
#define BSP_SYSTIMER_ISR_PRIOR    3
#define BSP_SYSTIMER_INTERRUPT_VECTOR SysTick_IRQn

/*
 ** Interrupt handler UART re-definitions
 */
#define UART0_RX_TX_IRQHandler        UART2_FLEXIO_IRQHandler

/*
 * The clock tick rate in miliseconds - be cautious to keep this value such
 * that it divides 1000 well
 *
 * MGCT: <option type="number" min="1" max="1000"/>
 */
#ifndef BSP_ALARM_FREQUENCY
    #define BSP_ALARM_FREQUENCY             (200)
#endif


/** MGCT: </category> */

/*
 * Old clock rate definition in MS per tick, kept for compatibility
 */
#define BSP_ALARM_RESOLUTION                (1000 / BSP_ALARM_FREQUENCY)

/*
 * Define the location of the hardware interrupt vector table
 */
#if MQX_ROM_VECTORS
    #define BSP_INTERRUPT_VECTOR_TABLE              ((uint32_t)__VECTOR_TABLE_ROM_START)
#else
    #define BSP_INTERRUPT_VECTOR_TABLE              ((uint32_t)__VECTOR_TABLE_RAM_START)
#endif


#ifndef BSP_FIRST_INTERRUPT_VECTOR_USED
    #define BSP_FIRST_INTERRUPT_VECTOR_USED     (0)
#endif

#ifndef BSP_LAST_INTERRUPT_VECTOR_USED
    #define BSP_LAST_INTERRUPT_VECTOR_USED      (104)
#endif



/****************************************************************************
 *                  DEFAULT MQX INITIALIZATION DEFINITIONS
 * MGCT: <category name="Default MQX initialization parameters">
 */

/* Defined in link.xxx */
extern unsigned char __KERNEL_DATA_START[];
extern unsigned char __KERNEL_DATA_END[];

#define BSP_DEFAULT_START_OF_KERNEL_MEMORY                    ((void *)__KERNEL_DATA_START)
#define BSP_DEFAULT_END_OF_KERNEL_MEMORY                      ((void *)__KERNEL_DATA_END)
#define BSP_DEFAULT_PROCESSOR_NUMBER                          ((uint32_t)__DEFAULT_PROCESSOR_NUMBER)

/* MGCT: <option type="string" quoted="false" allowempty="false"/> */
#ifndef BSP_DEFAULT_INTERRUPT_STACK_SIZE
    #define BSP_DEFAULT_INTERRUPT_STACK_SIZE                  ((uint32_t)__DEFAULT_INTERRUPT_STACK_SIZE)
#endif

/* MGCT: <option type="list">
 * <item name="1 (all levels disabled)" value="(1L)"/>
 * <item name="2" value="(2L)"/>
 * <item name="3" value="(3L)"/>
 * <item name="4" value="(4L)"/>
 * <item name="5" value="(5L)"/>
 * <item name="6" value="(6L)"/>
 * <item name="7" value="(7L)"/>
 * </option>
 */
#ifndef BSP_DEFAULT_MQX_HARDWARE_INTERRUPT_LEVEL_MAX
   #define BSP_DEFAULT_MQX_HARDWARE_INTERRUPT_LEVEL_MAX        (1L)
#endif

/*
 * MGCT: <option type="number"/>
 */
#ifndef BSP_DEFAULT_MAX_MSGPOOLS
    #define BSP_DEFAULT_MAX_MSGPOOLS                            (2L)
#endif

/*
 * MGCT: <option type="number"/>
 */
#ifndef BSP_DEFAULT_MAX_MSGQS
    #define BSP_DEFAULT_MAX_MSGQS                               (16L)
#endif

/*
 * MGCT: <option type="string" maxsize="1024" quoted="false" allowempty="false"/>
 */
#ifndef BSP_DEFAULT_IO_OPEN_MODE
    #define BSP_DEFAULT_IO_OPEN_MODE                      (void *)(NULL)
#endif

#endif /* __bsp_config_h__*/
