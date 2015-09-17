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
*   This file includes all include files specific to this target system.
*
*END*************************************************************************/

#ifndef __bsp_h__
#define __bsp_h__   1

#include "psp.h"
/* SDK */

#include "fsl_clock_manager.h"
#include "fsl_hwtimer.h"
#include "fsl_hwtimer_systick.h"

#ifndef PEX_MQX_KSDK
  #include "fsl_gpio_driver.h"
  #include "fsl_hwtimer_pit.h"
  #ifdef HW_ENET_INSTANCE_COUNT
    #include "fsl_enet_features.h"
  #endif
#endif

/* SDK board configuration file */
#include "board.h"
/* BSP default configuration file */
#include "bsp_config.h"

/*
 * Board Debug Console Settings
 */

#ifndef BOARD_DEBUG_UART_INSTANCE
  #error BOARD_DEBUG_UART_INSTANCE should be defined in board.h file.
#endif
#ifndef BOARD_DEBUG_UART_BAUD
    #error BOARD_DEBUG_UART_BAUD should be defined in board.h file.
#endif

/* Define BSP_DEFAULT_IO_CHANNEL for MQX_init_struct */
#define BSP_MACRO_TO_STRING_(x) #x
#define BSP_MACRO_TO_STRING(x) BSP_MACRO_TO_STRING_(x)

#ifndef BSP_DEFAULT_IO_CHANNEL
    #define BSP_DEFAULT_IO_CHANNEL                              "nser" BSP_MACRO_TO_STRING(BOARD_DEBUG_UART_INSTANCE) ":"
#endif


#ifdef __cplusplus
extern "C" {
#endif
/* Function declarations */


#ifdef __cplusplus
}
#endif

#endif  /* __bsp_h__ */
/* EOF */
