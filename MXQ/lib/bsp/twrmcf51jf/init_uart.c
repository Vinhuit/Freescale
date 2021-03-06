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



#if MQX_ENABLE_LOW_POWER
const KUART_OPERATION_MODE_STRUCT _bsp_uart0_operation_modes[LPM_OPERATION_MODES] =
{
    /* LPM_OPERATION_MODE_RUN */
    {
        IO_PERIPHERAL_PIN_MUX_ENABLE | IO_PERIPHERAL_CLOCK_ENABLE | IO_PERIPHERAL_MODULE_ENABLE,
        0,
        0,
        0
    },

    /* LPM_OPERATION_MODE_WAIT */
    {
        IO_PERIPHERAL_PIN_MUX_ENABLE | IO_PERIPHERAL_CLOCK_ENABLE | IO_PERIPHERAL_MODULE_ENABLE,
        0,
        0,
        0
    },

    /* LPM_OPERATION_MODE_SLEEP */
    {
        IO_PERIPHERAL_PIN_MUX_ENABLE | IO_PERIPHERAL_CLOCK_ENABLE | IO_PERIPHERAL_MODULE_ENABLE | IO_PERIPHERAL_WAKEUP_ENABLE | IO_PERIPHERAL_WAKEUP_SLEEPONEXIT_DISABLE,
        0,
        0,
        0
    },

    /* LPM_OPERATION_MODE_STOP */
    {
        IO_PERIPHERAL_PIN_MUX_DISABLE | IO_PERIPHERAL_CLOCK_DISABLE,
        0,
        0,
        0
    }
};

const KUART_OPERATION_MODE_STRUCT _bsp_uart1_operation_modes[LPM_OPERATION_MODES] =
{
    /* LPM_OPERATION_MODE_RUN */
    {
        IO_PERIPHERAL_PIN_MUX_ENABLE | IO_PERIPHERAL_CLOCK_ENABLE | IO_PERIPHERAL_MODULE_ENABLE,
        0,
        0,
        0
    },

    /* LPM_OPERATION_MODE_WAIT */
    {
        IO_PERIPHERAL_PIN_MUX_ENABLE | IO_PERIPHERAL_CLOCK_ENABLE | IO_PERIPHERAL_MODULE_ENABLE,
        0,
        0,
        0
    },

    /* LPM_OPERATION_MODE_SLEEP */
    {
        IO_PERIPHERAL_PIN_MUX_ENABLE | IO_PERIPHERAL_CLOCK_ENABLE | IO_PERIPHERAL_MODULE_ENABLE | IO_PERIPHERAL_WAKEUP_ENABLE | IO_PERIPHERAL_WAKEUP_SLEEPONEXIT_DISABLE,
        0,
        0,
        0
    },

    /* LPM_OPERATION_MODE_STOP */
    {
        IO_PERIPHERAL_PIN_MUX_DISABLE | IO_PERIPHERAL_CLOCK_DISABLE,
        0,
        0,
        0
    }
};


#endif /* MQX_ENABLE_LOW_POWER */


const KUART_INIT_STRUCT _bsp_uart0_init = {
/* Queue Size           */  BSPCFG_UART0_QUEUE_SIZE,
/* Channel              */  0,
/* Input Clock [Hz]     */  BSP_SYSTEM_CLOCK,
/* Baud Rate            */  BSPCFG_UART0_BAUD_RATE,
/* RX/TX Int vect       */  Vsci0,
/* ERR   Int vect       */  Vsci0,
/* RX/TX priority       */  3,
/* ERR   priority       */  3
#if MQX_ENABLE_LOW_POWER
                            ,
/* Clock source         */  CM_CLOCK_SOURCE_CORE,
/* LPM operation info   */  _bsp_uart0_operation_modes
#endif
};

const KUART_INIT_STRUCT _bsp_uart1_init = {
/* Queue Size           */  BSPCFG_UART1_QUEUE_SIZE,
/* Channel              */  1,
/* Input Clock [Hz]     */  BSP_SYSTEM_CLOCK,
/* Baud Rate            */  BSPCFG_UART1_BAUD_RATE,
/* RX/TX Int vect       */  Vsci1,
/* ERR   Int vect       */  Vsci1,
/* RX/TX priority       */  3,
/* ERR   priority       */  3
#if MQX_ENABLE_LOW_POWER
                            ,
/* Clock source         */  CM_CLOCK_SOURCE_CORE,
/* LPM operation info   */  _bsp_uart1_operation_modes
#endif
};

/* EOF */
