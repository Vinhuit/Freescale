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
*   This file contains the definition for the baud rate for the serial
*   channel
*
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"

const MCF52XX_UART_SERIAL_INIT_STRUCT _bsp_uart0_init = {
   /* queue size         */ BSPCFG_UART0_QUEUE_SIZE,
   /* Channel            */ MCF52XX_IO_UART0,
   /* Clock Speed        */ BSP_SYSTEM_CLOCK,
   /* Interrupt Vector   */ BSP_UART0_INT_VECTOR,
   /* Interrupt Level    */ BSP_UART0_INT_LEVEL,
   /* Interrupt Sublevel */ BSP_UART0_INT_SUBLEVEL,
   /* UMR1 Value         */ MCF52XX_UART_UMR1_NO_PARITY |
                            MCF52XX_UART_UMR1_8_BITS,
   /* UMR2 Value         */ MCF52XX_UART_UMR2_1_STOP_BIT,
   /* Baud rate          */ BSPCFG_UART0_BAUD_RATE,
   /* Address            */ BSPCFG_UART0_STATION_ADDR
};

/* EOF */
