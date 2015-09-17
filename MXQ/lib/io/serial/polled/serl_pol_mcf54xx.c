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
*   This file contains the functions for the polled serial I/O
*   low level functions for the MCF54XX UART.
*
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"
#include "io_prv.h"
#include "charq.h"
#include "fio_prv.h"
#include "serplprv.h"
#include "serl_mcf54xx.h"

/* Polled driver functions */
extern char    _mcf54xx_uart_serial_polled_getc(MCF54XX_UART_SERIAL_INFO_STRUCT_PTR);
extern void    _mcf54xx_uart_serial_polled_putc(MCF54XX_UART_SERIAL_INFO_STRUCT_PTR, char);
extern bool _mcf54xx_uart_serial_polled_status(MCF54XX_UART_SERIAL_INFO_STRUCT_PTR);
extern uint32_t _mcf54xx_uart_serial_polled_ioctl(MCF54XX_UART_SERIAL_INFO_STRUCT_PTR, uint32_t,
   uint32_t *);
extern uint32_t _mcf54xx_uart_serial_polled_init(MCF54XX_UART_SERIAL_INIT_STRUCT_PTR,
   void   **, char *);
extern uint32_t _mcf54xx_uart_serial_polled_deinit(MCF54XX_UART_SERIAL_INIT_STRUCT_PTR,
   MCF54XX_UART_SERIAL_INFO_STRUCT_PTR);

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _mcf54xx_uart_serial_polled_install
* Returned Value   : uint32_t a task error code or MQX_OK
* Comments         :
*    Install a polled uart serial device.
*
*END*----------------------------------------------------------------------*/

uint32_t _mcf54xx_uart_serial_polled_install
   (
      /* [IN] A string that identifies the device for fopen */
      char           *identifier,

      /* [IN] The I/O init data pointer */
      MCF54XX_UART_SERIAL_INIT_STRUCT_CPTR            init_data_ptr,

      /* [IN] The I/O queue size to use */
      uint32_t            queue_size
   )
{ /* Body */

   return _io_serial_polled_install(identifier,
      (uint32_t (_CODE_PTR_)(void *, void **, char *))_mcf54xx_uart_serial_polled_init,
      (uint32_t (_CODE_PTR_)(void *, void *))_mcf54xx_uart_serial_polled_deinit,
      (char    (_CODE_PTR_)(void *))_mcf54xx_uart_serial_polled_getc,
      (void    (_CODE_PTR_)(void *, char))_mcf54xx_uart_serial_polled_putc,
      (bool (_CODE_PTR_)(void *))_mcf54xx_uart_serial_polled_status,
      (uint32_t (_CODE_PTR_)(void *, uint32_t, void *))_mcf54xx_uart_serial_polled_ioctl,
      (void *)init_data_ptr, queue_size);

} /* Endbody */



/*FUNCTION****************************************************************
*
* Function Name    : _mcf54xx_uart_serial_polled_init
* Returned Value   : MQX_OK or a MQX error code.
* Comments         :
*    This function initializes the UART
*
*END*********************************************************************/

uint32_t _mcf54xx_uart_serial_polled_init
   (
      /* [IN] the initialization information for the device being opened */
      MCF54XX_UART_SERIAL_INIT_STRUCT_PTR io_init_ptr,

      /* [OUT] the address to store device specific information */
      void                          **io_info_ptr_ptr,

      /* [IN] the rest of the name of the device opened */
      char                           *open_name_ptr
   )
{ /* Body */
   volatile MCF54XX_UART_STRUCT       *uart_ptr;
   MCF54XX_UART_SERIAL_INFO_STRUCT      *uart_info_ptr;
   uint32_t                             channel;
   uint32_t                             baud;

   uart_info_ptr = _mem_alloc_system_zero((uint32_t)
      sizeof(MCF54XX_UART_SERIAL_INFO_STRUCT));

#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if ( uart_info_ptr == NULL ) {
        return(MQX_OUT_OF_MEMORY);
   }/* Endif */
#endif
   _mem_set_type(uart_info_ptr,MEM_TYPE_IO_SERIAL_INFO_STRUCT);    

   *io_info_ptr_ptr = uart_info_ptr;

   /* Save initialization values */
   uart_info_ptr->INIT = *io_init_ptr;

   channel = uart_info_ptr->INIT.DEVICE;
   _bsp_serial_io_init (channel);
   uart_ptr = _bsp_get_serial_base_address (channel);
   uart_info_ptr->UART_PTR = (void *)uart_ptr;

   uart_ptr->WRITE.UCR = MCF54XX_UART_UCR_RESET_RX;
   uart_ptr->WRITE.UCR = MCF54XX_UART_UCR_RESET_TX;
   uart_ptr->WRITE.UCR = MCF54XX_UART_UCR_RESET_ERROR;
   uart_ptr->WRITE.UCR = MCF54XX_UART_UCR_RESET_BREAK;
   uart_ptr->WRITE.UCR = MCF54XX_UART_UCR_RESET_POINTER;

   uart_ptr->WRITE.UMR = uart_info_ptr->INIT.UMR1_VALUE;
   uart_ptr->WRITE.UMR = uart_info_ptr->INIT.UMR2_VALUE;

   uart_ptr->WRITE.UCSR = MCF54XX_UART_UCSR_RX_TIMER |
                          MCF54XX_UART_UCSR_TX_TIMER;

   uart_ptr->WRITE.UIMR = 0; /* Disable all interrupts */

   /* Set up the baud rate */
   /* baud = fsck / (32 * baudrate) + 0.5 */ 
   baud = (uart_info_ptr->INIT.CLOCK_SPEED + (16 * uart_info_ptr->INIT.BAUD_RATE) ) / (32 * uart_info_ptr->INIT.BAUD_RATE);   
   uart_ptr->WRITE.UBG1 = (unsigned char)((baud >> 8) & 0xFF);
   uart_ptr->WRITE.UBG2 = (unsigned char)(baud & 0xFF);

   /* If in multi-drop mode disable RX */
   if ((uart_info_ptr->INIT.UMR1_VALUE & MCF54XX_UART_UMR1_MULTI_DROP) == MCF54XX_UART_UMR1_MULTI_DROP){
      /* Disable RX, enable TX */
      uart_ptr->WRITE.UCR = MCF54XX_UART_UCR_TX_ENABLE |
                           MCF54XX_UART_UCR_RX_DISABLE;
   } else {
      /* Enable RX & TX */
      uart_ptr->WRITE.UCR = MCF54XX_UART_UCR_TX_ENABLE |
                           MCF54XX_UART_UCR_RX_ENABLE;
   }

   return( MQX_OK );

} /* Endbody */


/*FUNCTION****************************************************************
*
* Function Name    : _mcf54xx_uart_serial_polled_deinit
* Returned Value   : MQX_OK or a mqx error code.
* Comments         :
*    This function de-initializes the UART.
*
*END*********************************************************************/

uint32_t _mcf54xx_uart_serial_polled_deinit
   (
      /* [IN] the initialization information for the device being opened */
      MCF54XX_UART_SERIAL_INIT_STRUCT_PTR io_init_ptr,

      /* [IN] the address of the device specific information */
      MCF54XX_UART_SERIAL_INFO_STRUCT_PTR io_info_ptr
   )
{ /* Body */

   _mem_free(io_info_ptr);

   return(MQX_OK);

} /* Endbody */


/*FUNCTION****************************************************************
*
* Function Name    : _mcf54xx_uart_serial_polled_getc
* Returned Value   : char
* Comments         :
*   Return a character when it is available.  This function polls the
* device for input.
*
*************************************************************************/

char _mcf54xx_uart_serial_polled_getc
   (
      /* [IN] the address of the device specific information */
      MCF54XX_UART_SERIAL_INFO_STRUCT_PTR io_info_ptr
   )
{ /* Body */
   volatile MCF54XX_UART_STRUCT      *uart_ptr = io_info_ptr->UART_PTR;
   bool data = FALSE;
   /* Wait until a character is available */
   while ( !data ){
      while ( (uart_ptr->READ.USR & MCF54XX_UART_USR_RXRDY) == 0 ) {
      } /* Endwhile */
   
      if ( ((io_info_ptr->INIT.UMR1_VALUE & MCF54XX_UART_UMR1_MULTI_DROP) == MCF54XX_UART_UMR1_MULTI_DROP) && 
      (uart_ptr->READ.USR & MCF54XX_UART_USR_PE) )
      {  /*input character is address*/
         if ( (unsigned char )io_info_ptr->INIT.STATION_ADDR == uart_ptr->READ.URB ){
            /* Enable RX */
            uart_ptr->WRITE.UCR &= ~MCF54XX_UART_UCR_RX_DISABLE;
            uart_ptr->WRITE.UCR |= MCF54XX_UART_UCR_RX_ENABLE;
         } else {
            /* Disable RX */
            uart_ptr->WRITE.UCR &= ~MCF54XX_UART_UCR_RX_ENABLE;
            uart_ptr->WRITE.UCR |= MCF54XX_UART_UCR_RX_DISABLE;
         } 
      } else {
         data = TRUE;
      }  
   }
   io_info_ptr->RX_CHARS++;
   
   return uart_ptr->READ.URB;

} /* Endbody */


/*FUNCTION****************************************************************
*
* Function Name    : _mcf54xx_uart_serial_polled_putc
* Returned Value   : void
* Comments         :
*   Writes the provided character.
*
*END*********************************************************************/

void _mcf54xx_uart_serial_polled_putc
   (
      /* [IN] the address of the device specific information */
      MCF54XX_UART_SERIAL_INFO_STRUCT_PTR io_info_ptr,

      /* [IN] the character to write */
      char                               c
   )
{ /* Body */
   volatile MCF54XX_UART_STRUCT      *uart_ptr = io_info_ptr->UART_PTR;

   /* Wait until free to send a character */
   while ( (uart_ptr->READ.USR & MCF54XX_UART_USR_TXRDY) == 0 ) {
   } /* Endwhile */

   uart_ptr->WRITE.UTB = c;
   io_info_ptr->TX_CHARS++;

} /* Endbody */


/*FUNCTION****************************************************************
*
* Function Name    : _mcf54xx_uart_serial_polled_status
* Returned Value   : bool
* Comments         :
*            This function returns TRUE if a character is available
*            on the on I/O device, otherwise it returns FALSE.
*
*END*********************************************************************/

bool _mcf54xx_uart_serial_polled_status
   (
      /* [IN] the address of the device specific information */
      MCF54XX_UART_SERIAL_INFO_STRUCT_PTR io_info_ptr
   )
{ /* Body */
   volatile MCF54XX_UART_STRUCT      *uart_ptr = io_info_ptr->UART_PTR;
   return (uart_ptr->READ.USR & MCF54XX_UART_USR_RXRDY) != 0;
} /* Endbody */


/*FUNCTION****************************************************************
*
* Function Name    : _mcf54xx_uart_serial_polled_ioctl
* Returned Value   : uint32_t MQX_OK or a mqx error code.
* Comments         :
*    This function performs miscellaneous services for
*    the I/O device.
*
*END*********************************************************************/

uint32_t _mcf54xx_uart_serial_polled_ioctl
   (
      /* [IN] the address of the device specific information */
      MCF54XX_UART_SERIAL_INFO_STRUCT_PTR io_info_ptr,

      /* [IN] The command to perform */
      uint32_t                    cmd,

      /* [IN] Parameters for the command */
      uint32_t                *param_ptr
   )
{ /* Body */
   volatile MCF54XX_UART_STRUCT      *uart_ptr = io_info_ptr->UART_PTR;
   uint32_t                            baud,tmp;

   switch (cmd) {
      case IO_IOCTL_SERIAL_GET_STOP_BITS:
         tmp = io_info_ptr->INIT.UMR2_VALUE & MCF54XX_UART_UMR2_2_STOP_BITS;
         *param_ptr = (tmp == MCF54XX_UART_UMR2_1_STOP_BIT)
            ? MCF54XX_UART_UMR2_1_STOP_BIT
            : MCF54XX_UART_UMR2_2_STOP_BITS;
         break;
      case IO_IOCTL_SERIAL_SET_STOP_BITS:
         tmp = io_info_ptr->INIT.UMR2_VALUE;
         tmp &= ~MCF54XX_UART_UMR2_MASK_STOP_BITS;
         /* Tell user to change terminal settings */
         switch (*param_ptr) {
         case IO_SERIAL_STOP_BITS_1:
            tmp |= MCF54XX_UART_UMR2_1_STOP_BIT;
            break;
         case IO_SERIAL_STOP_BITS_2:
            tmp |= MCF54XX_UART_UMR2_2_STOP_BITS;
            break;
         default:
            return MQX_INVALID_PARAMETER;
         } /* Endswitch */
         io_info_ptr->INIT.UMR2_VALUE = tmp;
         uart_ptr->WRITE.UCR = MCF54XX_UART_UCR_RESET_POINTER;
         uart_ptr->WRITE.UMR = io_info_ptr->INIT.UMR1_VALUE;
         uart_ptr->WRITE.UMR = io_info_ptr->INIT.UMR2_VALUE;
         break;
      case IO_IOCTL_SERIAL_GET_DATA_BITS:
         tmp = io_info_ptr->INIT.UMR1_VALUE & MCF54XX_UART_UMR1_MASK_DATA_BITS;
         *param_ptr = tmp + 5;
         break;
      case IO_IOCTL_SERIAL_SET_DATA_BITS:
         tmp = io_info_ptr->INIT.UMR1_VALUE & ~MCF54XX_UART_UMR1_MASK_DATA_BITS;
         tmp |=                          (*param_ptr == 5)
            ? MCF54XX_UART_UMR1_5_BITS : (*param_ptr == 6)
            ? MCF54XX_UART_UMR1_6_BITS : (*param_ptr == 7)
            ? MCF54XX_UART_UMR1_7_BITS : MCF54XX_UART_UMR1_8_BITS;
         io_info_ptr->INIT.UMR1_VALUE = tmp;
         uart_ptr->WRITE.UCR = MCF54XX_UART_UCR_RESET_POINTER;
         uart_ptr->WRITE.UMR = io_info_ptr->INIT.UMR1_VALUE;
         uart_ptr->WRITE.UMR = io_info_ptr->INIT.UMR2_VALUE;
         break;
      case IO_IOCTL_SERIAL_GET_BAUD:
         *param_ptr = io_info_ptr->INIT.BAUD_RATE;
         break;
      case IO_IOCTL_SERIAL_SET_BAUD:
         io_info_ptr->INIT.BAUD_RATE = *param_ptr;
         /* baudrate calculation */
         /* baud = fsck / (32 * baudrate) + 0.5 */ 
         baud = (io_info_ptr->INIT.CLOCK_SPEED + (16 * io_info_ptr->INIT.BAUD_RATE) ) / (32 * io_info_ptr->INIT.BAUD_RATE);
         uart_ptr->WRITE.UBG1 = (unsigned char)((baud >> 8) & 0xFF);
         uart_ptr->WRITE.UBG2 = (unsigned char)(baud & 0xFF);
         break;
      case IO_IOCTL_SERIAL_GET_STATS:
         *param_ptr++ = io_info_ptr->INTERRUPTS;
         *param_ptr++ = io_info_ptr->RX_CHARS;
         *param_ptr++ = io_info_ptr->TX_CHARS;
         *param_ptr++ = io_info_ptr->RX_BREAKS;
         *param_ptr++ = io_info_ptr->RX_PARITY_ERRORS;
         *param_ptr++ = io_info_ptr->RX_FRAMING_ERRORS;
         *param_ptr++ = io_info_ptr->RX_OVERRUNS;
         *param_ptr++ = io_info_ptr->RX_DROPPED_INPUT;
         break;
      case IO_IOCTL_SERIAL_CLEAR_STATS:
         io_info_ptr->INTERRUPTS = 0;
         io_info_ptr->RX_CHARS = 0;
         io_info_ptr->TX_CHARS = 0;
         io_info_ptr->RX_BREAKS = 0;
         io_info_ptr->RX_PARITY_ERRORS = 0;
         io_info_ptr->RX_FRAMING_ERRORS = 0;
         io_info_ptr->RX_OVERRUNS = 0;
         io_info_ptr->RX_DROPPED_INPUT = 0;
         break;
      case IO_IOCTL_SERIAL_CAN_TRANSMIT:
         *param_ptr = io_info_ptr->UART_PTR->READ.USR & MCF54XX_UART_USR_TXRDY ? 1 : 0;
         break;
      case IO_IOCTL_SERIAL_CAN_RECEIVE:
         *param_ptr = io_info_ptr->UART_PTR->READ.USR & MCF54XX_UART_USR_RXRDY ? 1 : 0;
         break;
      case IO_IOCTL_SERIAL_GET_PARITY:
         switch (io_info_ptr->INIT.UMR1_VALUE & MCF54XX_UART_UMR1_MD_ADDRESS) {
            case MCF54XX_UART_UMR1_EVEN_PARITY:
               tmp = IO_SERIAL_PARITY_EVEN;
               break;
            case MCF54XX_UART_UMR1_ODD_PARITY:
               tmp = IO_SERIAL_PARITY_ODD;
               break;
            case MCF54XX_UART_UMR1_LOW_PARITY:
               tmp = IO_SERIAL_PARITY_SPACE;
               break;
            case MCF54XX_UART_UMR1_HI_PARITY:
               tmp = IO_SERIAL_PARITY_MARK;
               break;
            case MCF54XX_UART_UMR1_MULTI_DROP:
               tmp = IO_SERIAL_PARITY_MULTI_DATA;
               break;
            case MCF54XX_UART_UMR1_MD_ADDRESS:
               tmp = IO_SERIAL_PARITY_MULTI_ADDRESS;
               break;
            default:
               tmp = IO_SERIAL_PARITY_NONE;
               break;
         }
         *param_ptr = tmp;
         break;
      case IO_IOCTL_SERIAL_SET_PARITY:
         tmp = io_info_ptr->INIT.UMR1_VALUE & ~MCF54XX_UART_UMR1_MD_ADDRESS;
         switch (*param_ptr) {
            case IO_SERIAL_PARITY_NONE:
               tmp |= MCF54XX_UART_UMR1_NO_PARITY;
               break;
            case IO_SERIAL_PARITY_ODD:
               tmp |= MCF54XX_UART_UMR1_ODD_PARITY;
               break;
            case IO_SERIAL_PARITY_EVEN:
               tmp |= MCF54XX_UART_UMR1_EVEN_PARITY;
               break;
            case IO_SERIAL_PARITY_MARK:
               tmp |= MCF54XX_UART_UMR1_HI_PARITY;
               break;
            case IO_SERIAL_PARITY_SPACE:
               tmp |= MCF54XX_UART_UMR1_LOW_PARITY;
               break;
            case IO_SERIAL_PARITY_MULTI_DATA:
               tmp |= MCF54XX_UART_UMR1_MULTI_DROP;
               break;
            case IO_SERIAL_PARITY_MULTI_ADDRESS:
               tmp |= MCF54XX_UART_UMR1_MD_ADDRESS;
               break;
            default:
               return MQX_INVALID_PARAMETER;
         }
         io_info_ptr->INIT.UMR1_VALUE = tmp;
         uart_ptr->WRITE.UCR = MCF54XX_UART_UCR_RESET_POINTER;
         uart_ptr->WRITE.UMR = io_info_ptr->INIT.UMR1_VALUE;
         uart_ptr->WRITE.UMR = io_info_ptr->INIT.UMR2_VALUE;
         break;
      case IO_IOCTL_SERIAL_GET_HW_SIGNAL:
         tmp = 0;
         if (0 == (uart_ptr->READ.UIPCR & MCF54XX_UART_UIP_CTS)) {
            tmp |= IO_SERIAL_CTS;
         }
         if (io_info_ptr->CONTROL_BITS & MCF54XX_UART_UOP_RTS) {
            tmp |= IO_SERIAL_RTS;
         }
         *param_ptr = tmp;
         break;
      case IO_IOCTL_SERIAL_SET_HW_SIGNAL:  /* Assert */
         if (*param_ptr & IO_SERIAL_RTS) {
            if (io_info_ptr->INIT.UMR1_VALUE & MCF54XX_UART_UMR1_RXRTS) {
               return MQX_IO_OPERATION_NOT_AVAILABLE;
            }
            io_info_ptr->CONTROL_BITS |= MCF54XX_UART_UOP_RTS;
         }
         else {
            return MQX_INVALID_PARAMETER;
         }
         uart_ptr->WRITE.UOP1 = MCF54XX_UART_UOP_RTS;
         break;
      case IO_IOCTL_SERIAL_CLEAR_HW_SIGNAL:  /* Negate */
         if (*param_ptr & IO_SERIAL_RTS) {
            if (io_info_ptr->INIT.UMR1_VALUE & MCF54XX_UART_UMR1_RXRTS) {
               return MQX_IO_OPERATION_NOT_AVAILABLE;
            }
            io_info_ptr->CONTROL_BITS &= ~MCF54XX_UART_UOP_RTS;
         }
         else {
            return MQX_INVALID_PARAMETER;
         }
         uart_ptr->WRITE.UOP0 = MCF54XX_UART_UOP_RTS;
         break;         
      case IO_IOCTL_SERIAL_SET_FLAGS:
         /* automatic HW flow control in polled mode only */
         if (0 == io_info_ptr->INT_ENABLE_BITS) {
            io_info_ptr->INIT.UMR1_VALUE &= (~MCF54XX_UART_UMR1_RXRTS);
            io_info_ptr->INIT.UMR2_VALUE &= (~MCF54XX_UART_UMR2_TXCTS);
            if (*param_ptr & IO_SERIAL_HW_FLOW_CONTROL) {
               io_info_ptr->INIT.UMR1_VALUE |= MCF54XX_UART_UMR1_RXRTS;
               io_info_ptr->INIT.UMR2_VALUE |= MCF54XX_UART_UMR2_TXCTS;
            }
            uart_ptr->WRITE.UCR = MCF54XX_UART_UCR_RESET_POINTER;
            uart_ptr->WRITE.UMR = io_info_ptr->INIT.UMR1_VALUE;
            uart_ptr->WRITE.UMR = io_info_ptr->INIT.UMR2_VALUE;
         }
         break;
      case IO_IOCTL_SERIAL_SET_ADDRESS_DEVICE:
         /* Check address */ 
         if( *param_ptr <= 0xFF ){
            io_info_ptr->INIT.STATION_ADDR = *param_ptr;
         } else {
            return MQX_INVALID_PARAMETER;
         }
         break;
      case IO_IOCTL_SERIAL_GET_ADDRESS_DEVICE:
         *param_ptr = io_info_ptr->INIT.STATION_ADDR;
         break;
      case IO_IOCTL_SERIAL_GET_CONFIG:
      {
         MCF54XX_UART_SERIAL_INIT_STRUCT_PTR io_init_ptr = (MCF54XX_UART_SERIAL_INIT_STRUCT_PTR)param_ptr;
         *io_init_ptr = io_info_ptr->INIT;
         break;
      }
      default:
         return IO_ERROR_INVALID_IOCTL_CMD;
   } /* Endswitch */

   return (MQX_OK);

} /* Endbody */

/* EOF */
