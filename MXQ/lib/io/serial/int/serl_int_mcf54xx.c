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
*   This file contains the low level functions for the interrupt driven
*   serial I/O for the MCF54XX UART.
*
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"
#include "io_prv.h"
#include "charq.h"
#include "fio_prv.h"
#include "serinprv.h"
#include "serl_mcf54xx.h"

/* Polled functions used */
extern uint32_t _mcf54xx_uart_serial_polled_init(MCF54XX_UART_SERIAL_INIT_STRUCT_PTR,
   void   **, char *);
extern uint32_t _mcf54xx_uart_serial_polled_deinit(MCF54XX_UART_SERIAL_INIT_STRUCT_PTR,
   MCF54XX_UART_SERIAL_INFO_STRUCT_PTR);
extern uint32_t _mcf54xx_uart_serial_polled_ioctl(MCF54XX_UART_SERIAL_INFO_STRUCT_PTR, uint32_t,
   uint32_t *);

/* Interrupt driver functions */

extern void    _mcf54xx_uart_serial_int_putc(IO_SERIAL_INT_DEVICE_STRUCT_PTR, char);

extern uint32_t _mcf54xx_uart_serial_int_init(IO_SERIAL_INT_DEVICE_STRUCT_PTR,
 char *);
extern uint32_t _mcf54xx_uart_serial_int_deinit(MCF54XX_UART_SERIAL_INIT_STRUCT_PTR,
   MCF54XX_UART_SERIAL_INFO_STRUCT_PTR);
extern uint32_t _mcf54xx_uart_serial_int_enable(MCF54XX_UART_SERIAL_INFO_STRUCT_PTR);
extern void    _mcf54xx_uart_serial_int_isr(void *);


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _mcf54xx_uart_serial_int_install
* Returned Value   : uint32_t a task error code or MQX_OK
* Comments         :
*    Install an interrupt driven uart serial device.
*
*END*----------------------------------------------------------------------*/

uint32_t _mcf54xx_uart_serial_int_install
   (
      /* [IN] A string that identifies the device for fopen */
      char *identifier,

      /* [IN] The I/O init data pointer */
      MCF54XX_UART_SERIAL_INIT_STRUCT_CPTR  init_data_ptr,

      /* [IN] The I/O queue size to use */
      uint32_t  queue_size
   )
{ /* Body */

   return _io_serial_int_install(identifier,
      (uint32_t (_CODE_PTR_)(void *, char *))_mcf54xx_uart_serial_int_init,
      (uint32_t (_CODE_PTR_)(void *))_mcf54xx_uart_serial_int_enable,
      (uint32_t (_CODE_PTR_)(void *,void *))_mcf54xx_uart_serial_int_deinit,
      (void    (_CODE_PTR_)(void *, char))_mcf54xx_uart_serial_int_putc,
      (uint32_t (_CODE_PTR_)(void *, uint32_t, void *))_mcf54xx_uart_serial_polled_ioctl,
      (void *)init_data_ptr, queue_size);

} /* Endbody */


/*FUNCTION****************************************************************
*
* Function Name    : _mcf54xx_uart_serial_int_init
* Returned Value   : uint32_t a task error code or MQX_OK
* Comments         :
*    This function initializes the UART in interrupt mode.
*
*END*********************************************************************/

uint32_t _mcf54xx_uart_serial_int_init
   (
      /* [IN] the interrupt I/O initialization information */
      IO_SERIAL_INT_DEVICE_STRUCT_PTR int_io_dev_ptr,

      /* [IN] the rest of the name of the device opened */
      char                       *open_name_ptr
   )
{ /* Body */
   MCF54XX_UART_SERIAL_INFO_STRUCT_PTR uart_info_ptr;
   MCF54XX_UART_SERIAL_INIT_STRUCT_PTR uart_init_ptr;
   uint32_t                             result;

   uart_init_ptr = int_io_dev_ptr->DEV_INIT_DATA_PTR;
   result = _mcf54xx_uart_serial_polled_init((void *)uart_init_ptr,
      &int_io_dev_ptr->DEV_INFO_PTR, open_name_ptr);
   if (result != MQX_OK) {
      return(result);
   }/* Endif */

   uart_info_ptr = int_io_dev_ptr->DEV_INFO_PTR;
   uart_info_ptr->OLD_ISR_DATA = _int_get_isr_data(uart_init_ptr->VECTOR);
   uart_info_ptr->OLD_ISR_EXCEPTION_HANDLER =
      _int_get_exception_handler(uart_init_ptr->VECTOR);

   uart_info_ptr->OLD_ISR =
      _int_install_isr(uart_init_ptr->VECTOR, _mcf54xx_uart_serial_int_isr,
      int_io_dev_ptr);

   return(MQX_OK);

} /* Endbody */


/*FUNCTION****************************************************************
*
* Function Name    : _mcf54xx_uart_serial_int_deinit
* Returned Value   : uint32_t a task error code or MQX_OK
* Comments         :
*    This function de-initializes the UART in interrupt mode.
*
*END*********************************************************************/

uint32_t _mcf54xx_uart_serial_int_deinit
   (
      /* [IN] the interrupt I/O initialization information */
      MCF54XX_UART_SERIAL_INIT_STRUCT_PTR io_init_ptr,

      /* [IN] the address of the device specific information */
      MCF54XX_UART_SERIAL_INFO_STRUCT_PTR io_info_ptr
   )
{ /* Body */
   volatile MCF54XX_UART_STRUCT      *uart_ptr = io_info_ptr->UART_PTR;

   uart_ptr->WRITE.UIMR = 0; /* Disable all interrupts */
   _mcf54xx_uart_serial_polled_deinit(io_init_ptr, io_info_ptr);
   _int_install_isr(io_init_ptr->VECTOR, io_info_ptr->OLD_ISR,
      io_info_ptr->OLD_ISR_DATA);

   return(MQX_OK);

} /* Endbody */


/*FUNCTION****************************************************************
*
* Function Name    : _mcf54xx_uart_serial_int_enable
* Returned Value   : uint32_t a task error code or MQX_OK
* Comments         :
*    This function enables the UART interrupts mode.
*
*END*********************************************************************/

uint32_t _mcf54xx_uart_serial_int_enable
   (
      /* [IN] the address of the device specific information */
      MCF54XX_UART_SERIAL_INFO_STRUCT_PTR io_info_ptr
   )
{ /* Body */
   uint32_t status;

/* Start CR 2087 */
   status = _mcf54xx_int_init((PSP_INTERRUPT_TABLE_INDEX)io_info_ptr->INIT.VECTOR, io_info_ptr->INIT.LEVEL, /*io_info_ptr->INIT.SUBLEVEL, */TRUE);
/* End CR 2087 */      

   if (status == MQX_OK) {
      volatile MCF54XX_UART_STRUCT      *uart_ptr = io_info_ptr->UART_PTR;

      io_info_ptr->INT_ENABLE_BITS = MCF54XX_UART_UIMR_FFULL;
      uart_ptr->WRITE.UIMR = io_info_ptr->INT_ENABLE_BITS;
   } /* Endif */

   return status;

} /* Endbody */


/*FUNCTION****************************************************************
*
* Function Name    : _mcf54xx_uart_serial_int_isr
* Returned Value   : none
* Comments         :
*   interrupt handler for the serial I/O interrupts.
*
*************************************************************************/

void _mcf54xx_uart_serial_int_isr
   (
      /* [IN] the address of the device specific information */
      void   *parameter
   )
{ /* Body */
   IO_SERIAL_INT_DEVICE_STRUCT_PTR        int_io_dev_ptr = parameter;
   MCF54XX_UART_SERIAL_INFO_STRUCT_PTR    uart_info_ptr;
   volatile MCF54XX_UART_STRUCT           *uart_ptr;
   int32_t                                c;
   uint16_t                               stat;
   bool                                   work;
   bool                                   data = FALSE;

   uart_info_ptr = int_io_dev_ptr->DEV_INFO_PTR;
   uart_ptr = uart_info_ptr->UART_PTR;

   ++uart_info_ptr->INTERRUPTS;

   /* Receive loop */
   work = TRUE;
   while (work) {
      work = FALSE;
      stat = uart_ptr->READ.USR;

      while (stat & MCF54XX_UART_USR_RXRDY) {
         c = uart_ptr->READ.URB;    /* this clears the status bit */
         /* Multi - Drop mode*/
         if ( ((uart_info_ptr->INIT.UMR1_VALUE & MCF54XX_UART_UMR1_MULTI_DROP) == MCF54XX_UART_UMR1_MULTI_DROP) &&
         (stat & MCF54XX_UART_USR_PE) )
         {
            /*input character is address*/
            if ( (int32_t)uart_info_ptr->INIT.STATION_ADDR == c ){
               /* Enable RX */
               uart_ptr->WRITE.UCR &= ~MCF54XX_UART_UCR_RX_DISABLE;
               uart_ptr->WRITE.UCR |= MCF54XX_UART_UCR_RX_ENABLE;
            } else {
               /* Disable RX */
               uart_ptr->WRITE.UCR &= ~MCF54XX_UART_UCR_RX_ENABLE;
               uart_ptr->WRITE.UCR |= MCF54XX_UART_UCR_RX_DISABLE;
            }
         } else {
            if (stat & (MCF54XX_UART_USR_RB | MCF54XX_UART_USR_FE |
                     MCF54XX_UART_USR_PE | MCF54XX_UART_USR_OE))
            {
               if (stat & MCF54XX_UART_USR_RB) {
                  ++uart_info_ptr->RX_BREAKS;
                  c = uart_ptr->READ.URB;    /* read break character */
                  uart_ptr->WRITE.UCR = MCF54XX_UART_UCR_RESET_BREAK;
               } /* Endif */
               if (stat & MCF54XX_UART_USR_FE) {
                  ++uart_info_ptr->RX_FRAMING_ERRORS;
               } /* Endif */
               if (stat & MCF54XX_UART_USR_PE) {
                  ++uart_info_ptr->RX_PARITY_ERRORS;
               } /* Endif */
               if (stat & MCF54XX_UART_USR_OE) {
                  ++uart_info_ptr->RX_OVERRUNS;
               } /* Endif */
               uart_ptr->WRITE.UCR = MCF54XX_UART_UCR_RESET_ERROR;
            } /* Endif */
            data = TRUE;
         } /* Endif */
         uart_info_ptr->RX_CHARS++;
         work = TRUE;
         /* function when non multi-drop mode or data character*/
         if(data){
            if (!_io_serial_int_addc(int_io_dev_ptr, c)) {
            uart_info_ptr->RX_DROPPED_INPUT++;
         } /* Endif */
         }
         stat = uart_ptr->READ.USR;

      } /* Endwhile */

      while (stat & MCF54XX_UART_USR_TXRDY)  {

         c = _io_serial_int_nextc(int_io_dev_ptr);
         if (c >= 0) {
            work = TRUE;
            uart_ptr->WRITE.UTB = c;
            uart_info_ptr->INT_ENABLE_BITS |= MCF54XX_UART_UIMR_TXRDY;
            uart_ptr->WRITE.UIMR = uart_info_ptr->INT_ENABLE_BITS;
         } else {
            /* No output. Turn off transmit interrupt */
            uart_info_ptr->INT_ENABLE_BITS &= ~MCF54XX_UART_UIMR_TXRDY;
            uart_ptr->WRITE.UIMR = uart_info_ptr->INT_ENABLE_BITS;
            break;
         } /* Endif */

         stat = uart_ptr->READ.USR;
         uart_info_ptr->TX_CHARS++;

      } /* Endwhile */

   } /* Endwhile */

} /* Endbody */


/*FUNCTION****************************************************************
*
* Function Name    : _mcf54xx_uart_serial_int_putc
* Returned Value   : none
* Comments         :
*   This function is called to write out the first character, when
* the output serial device and output ring buffers are empty.
*
*END*********************************************************************/

void _mcf54xx_uart_serial_int_putc
   (
      /* [IN] the address of the device specific information */
      IO_SERIAL_INT_DEVICE_STRUCT_PTR int_io_dev_ptr,

      /* [IN] the character to write out now */
      char                       c
   )
{ /* Body */
   MCF54XX_UART_SERIAL_INFO_STRUCT_PTR uart_info_ptr;
   volatile MCF54XX_UART_STRUCT       *uart_ptr;

   uart_info_ptr = int_io_dev_ptr->DEV_INFO_PTR;
   uart_ptr = uart_info_ptr->UART_PTR;

   while (!(uart_ptr->READ.USR & MCF54XX_UART_USR_TXRDY)) {
   } /* Endwhile */
   uart_ptr->WRITE.UTB = c;
   uart_info_ptr->TX_CHARS++;

   /* Interrupt when character has been sent */
   uart_info_ptr->INT_ENABLE_BITS |= MCF54XX_UART_UIMR_TXRDY;
   uart_ptr->WRITE.UIMR = uart_info_ptr->INT_ENABLE_BITS;

} /* Endbody */

/* EOF */
