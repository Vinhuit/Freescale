/*HEADER**********************************************************************
*
* Copyright 2008 Freescale Semiconductor, Inc.
* Copyright 2004-2008 Embedded Access Inc.
* Copyright 1989-2008 ARC International
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
*   This file contains the definitions of constants and structures
*   required for the serial drivers for the MCF5223
*
*
*END************************************************************************/
#ifndef _serial_mcf52xx_h
#define _serial_mcf52xx_h 1

#include "mcf52xx_uart.h"

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------------*/
/*
**                    CONSTANT DEFINITIONS
*/

/*
** I/O Initialization error codes
*/
#define MCF52XX_IO_INVALID_CHANNEL        (0x10000)

/* Initialization values for the DEVICE field of the IO_INIT_STRUCT */
/* Channel number for Serial port 1 */
#define MCF52XX_IO_UART0                  (0)
/* Channel number for Serial port 2 */
#define MCF52XX_IO_UART1                  (1)
/* Channel number for Serial port 3 */
#define MCF52XX_IO_UART2                  (2)



/*--------------------------------------------------------------------------*/
/*
**                    DATATYPE DECLARATIONS
*/

/*
** MCF52XX_UART_SERIAL_INIT_STRUCT
**
** This structure defines the initialization parameters to be used
** when a serial port is initialized.
*/
typedef struct mcf52XX_uart_serial_init_struct
{

   /* The size of the queues to buffer incoming/outgoing data */
   uint32_t QUEUE_SIZE;

   /* The device to initialize */
   uint32_t DEVICE;

   /* The clock speed of cpu */
   uint32_t CLOCK_SPEED;

   /* The interrupt vector to use if interrupt driven */
   uint32_t VECTOR;

   /* The interrupt level to use if interrupt driven */
   _int_level LEVEL;

   /* The sub-level within the interrupt level to use if interrupt driven */
   _int_priority SUBLEVEL;

   /* The value for the UMR 1 register */
   uint32_t UMR1_VALUE;

   /* The value for the UMR 2 register */
   uint32_t UMR2_VALUE;

   /* The baud rate for the channel */
   uint32_t BAUD_RATE;
   
   /* The address for the channel */
   uint32_t STATION_ADDR;
} MCF52XX_UART_SERIAL_INIT_STRUCT, * MCF52XX_UART_SERIAL_INIT_STRUCT_PTR;

typedef const MCF52XX_UART_SERIAL_INIT_STRUCT * MCF52XX_UART_SERIAL_INIT_STRUCT_CPTR;

/*
** MCF52XX_UART_SERIAL_INFO_STRUCT
** Run time state information for each serial channel
*/
typedef struct mcf52XX_uart_serial_info_struct
{
   /* The current init values for this port */
   MCF52XX_UART_SERIAL_INIT_STRUCT   INIT;

   /* The serial device register */
   MCF52XX_UART_STRUCT_PTR           UART_PTR;

   uint32_t                           INT_ENABLE_BITS;

   uint32_t                           INT_BITS;
   
   uint32_t                           CONTROL_BITS;

   /* The previous interrupt handler and data */
   void       (_CODE_PTR_ OLD_ISR)(void *);
   void       (_CODE_PTR_ OLD_ISR_EXCEPTION_HANDLER)(uint32_t, uint32_t, void *,
               void *);
   void                             *OLD_ISR_DATA;

   /* Statistical information */
   uint32_t                           INTERRUPTS;
   uint32_t                           RX_CHARS;
   uint32_t                           TX_CHARS;
   uint32_t                           RX_BREAKS;
   uint32_t                           RX_PARITY_ERRORS;
   uint32_t                           RX_FRAMING_ERRORS;
   uint32_t                           RX_OVERRUNS;
   uint32_t                           RX_DROPPED_INPUT;
} MCF52XX_UART_SERIAL_INFO_STRUCT, * MCF52XX_UART_SERIAL_INFO_STRUCT_PTR;

/*--------------------------------------------------------------------------*/
/*
**                        FUNCTION PROTOTYPES
*/

extern uint32_t _mcf52xx_uart_serial_polled_init(MCF52XX_UART_SERIAL_INIT_STRUCT_PTR,void **,char *);
extern uint32_t _mcf52xx_uart_serial_polled_install(char *, MCF52XX_UART_SERIAL_INIT_STRUCT_CPTR , uint32_t);
extern uint32_t _mcf52xx_uart_serial_int_install(char *, MCF52XX_UART_SERIAL_INIT_STRUCT_CPTR , uint32_t);
extern void   *_bsp_get_serial_base_address (uint8_t);
PSP_INTERRUPT_TABLE_INDEX _bsp_get_serial_vector (uint8_t);

#ifdef __cplusplus
}
#endif

#endif
/* EOF */
