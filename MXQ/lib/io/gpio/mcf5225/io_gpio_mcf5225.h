#ifndef __io_gpio_cpu_h__
#define __io_gpio_cpu_h__
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
*   The file contains definitions used in user program and/or in other
*   kernel modules to access GPIO pins
*
*
*END************************************************************************/

/*----------------------------------------------------------------------*/
/*
**                          CONSTANT DEFINITIONS
*/

#ifdef __cplusplus
extern "C" {
#endif

typedef struct irq_map_struct {
   uint8_t  portnq;
} GPIO_IRQ_MAP, * GPIO_IRQ_MAP_PTR;

#define GPIO_PORT_TE ((0x0000 << 3) | GPIO_PIN_VALID)
#define GPIO_PORT_TF ((0x0001 << 3) | GPIO_PIN_VALID)
#define GPIO_PORT_TG ((0x0002 << 3) | GPIO_PIN_VALID)
#define GPIO_PORT_TH ((0x0003 << 3) | GPIO_PIN_VALID)
#define GPIO_PORT_TI ((0x0004 << 3) | GPIO_PIN_VALID)
#define GPIO_PORT_TJ ((0x0006 << 3) | GPIO_PIN_VALID)
#define GPIO_PORT_NQ ((0x0008 << 3) | GPIO_PIN_VALID)
#define GPIO_PORT_AN ((0x000A << 3) | GPIO_PIN_VALID)
#define GPIO_PORT_AS ((0x000B << 3) | GPIO_PIN_VALID)
#define GPIO_PORT_QS ((0x000C << 3) | GPIO_PIN_VALID)
#define GPIO_PORT_TA ((0x000E << 3) | GPIO_PIN_VALID)
#define GPIO_PORT_TC ((0x000F << 3) | GPIO_PIN_VALID)
#define GPIO_PORT_UA ((0x0011 << 3) | GPIO_PIN_VALID)
#define GPIO_PORT_UB ((0x0012 << 3) | GPIO_PIN_VALID)
#define GPIO_PORT_UC ((0x0013 << 3) | GPIO_PIN_VALID)
#define GPIO_PORT_DD ((0x0014 << 3) | GPIO_PIN_VALID)

#ifdef __cplusplus
}
#endif

#endif

/* EOF */
