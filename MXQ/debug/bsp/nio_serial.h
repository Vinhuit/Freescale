/*HEADER**********************************************************************
*
* Copyright 2013 Freescale Semiconductor, Inc.
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
*
*END************************************************************************/

#ifndef __NIO_SERIAL_H__
#define __NIO_SERIAL_H__

#include <stdint.h>
#include <fsl_uart_driver.h>
#if PLATFORM_LPUART_ENABLED
#include <fsl_lpuart_driver.h>
#endif
#include "nio.h"

extern const NIO_DEV_FN_STRUCT nio_serial_dev_fn;
typedef void (*NIO_SERIAL_ISR_HANDLE)(void);

typedef struct
{
    uint32_t                    UART_INSTANCE;                  /*!< Number of used UART peripheral */
    uint32_t                    BAUDRATE;                       /*!< UART baud rate */
    uart_parity_mode_t          PARITY_MODE;                    /*!< parity mode, disabled (default), even, odd */
    uart_stop_bit_count_t       STOPBIT_COUNT;                  /*!< number of stop bits, 1 stop bit (default) or 2 stop bits */
    uart_bit_count_per_char_t   BITCOUNT_PERCHAR;               /*!< number of bits, 8-bit (default) or 9-bit in a word (up to 10-bits in some UART instances) */
    uint32_t                    RXTX_PRIOR;                     /*!< IRQ priority */
    void                        (*handler)(void) ;              /*!< IRQ handler function */
} NIO_SERIAL_INIT_DATA_STRUCT;

#endif //__NIO_SERIAL_H__
