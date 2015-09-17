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

#define GPIO_PORT_A     ((0x0000 << 3) | GPIO_PIN_VALID)
#define GPIO_PORT_B     ((0x0001 << 3) | GPIO_PIN_VALID)
#define GPIO_PORT_C     ((0x0002 << 3) | GPIO_PIN_VALID)
#define GPIO_PORT_D     ((0x0003 << 3) | GPIO_PIN_VALID)
#define GPIO_PORT_E     ((0x0004 << 3) | GPIO_PIN_VALID)
#define GPIO_PORT_F     ((0x0005 << 3) | GPIO_PIN_VALID)
#define GPIO_PORT_G     ((0x0006 << 3) | GPIO_PIN_VALID)
#define GPIO_PORT_H     ((0x0007 << 3) | GPIO_PIN_VALID)
#define GPIO_PORT_I     ((0x0008 << 3) | GPIO_PIN_VALID)
#define GPIO_PORT_J     ((0x0009 << 3) | GPIO_PIN_VALID)
#define GPIO_PORT_K     ((0x000A << 3) | GPIO_PIN_VALID)

#ifdef __cplusplus
}
#endif

#endif

/* EOF */
