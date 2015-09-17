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
*   SDK RTOS adapter configuration.
*
*END************************************************************************/

#ifndef __mqx_sdk_config_h__
#define __mqx_sdk_config_h__

#define CMSIS_ENABLED                       1
#define PLATFORM_SDK_ENABLED                1

/* When use LPUART mode */
#define PLATFORM_LPUART0_ENABLED            1
#define PLATFORM_LPUART1_ENABLED            1
#define PLATFORM_LPUART_ENABLED             (PLATFORM_LPUART0_ENABLED | PLATFORM_LPUART1_ENABLED)
/* mandatory CPU identification */
#ifndef MQX_CPU
#define MQX_CPU              PSP_CPU_MKL43Z256VLH4
#endif

#undef MQX_DISABLE_CONFIG_CHECK


/*
** When MQX_USE_LWEVENT is defined as 1,
** then mqx will compile in the support code for light weight events.
*/
#ifndef MQX_USE_LWEVENT
#define MQX_USE_LWEVENT         1
#endif

/*
** When MQX_USE_LWMSGQ is defined as 1,
** then mqx will compile in the support code for light weight message queues.
*/
#ifndef MQX_USE_LWMSGQ
#define MQX_USE_LWMSGQ          1
#endif

/*
** When MQX_USE_MESSAGES is defined as 1,
** then mqx will compile in the support code for messages.
*/
#ifndef MQX_USE_MESSAGES
#define MQX_USE_MESSAGES        1
#endif


/*
** When MQX_USE_MUTEXES is defined as 1,
** then mqx will compile in the support code for mutex component.
*/
#ifndef MQX_USE_MUTEXES
#define MQX_USE_MUTEXES         1
#endif

/*
** When MQX_USE_TIMER is defined as 1,
** then mqx will compile in the support code for timer component.
*/
#ifndef MQX_USE_TIMER
#define MQX_USE_TIMER           1
#endif

#ifndef MQX_USE_LWMEM
#define MQX_USE_LWMEM           1
#define MQX_USE_LWMEM_ALLOCATOR 1
#endif

#endif /* __mqx_sdk_config_h__ */
