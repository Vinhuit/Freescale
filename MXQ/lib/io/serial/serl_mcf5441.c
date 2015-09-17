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
*   This file contains board-specific serial initialization functions.
*
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include "serl_mcf54xx.h"

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_serial_base_address
* Returned Value   : Address upon success, NULL upon failure
* Comments         :
*    This function returns the base register address of the corresponding UART
*
*END*----------------------------------------------------------------------*/
void *_bsp_get_serial_base_address(uint8_t dev_num) {

   VMCF5441_STRUCT_PTR reg_base = (VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR();

/* find base addr accordign to device number */
   switch (dev_num) {
      case 0:
         return (void *)&reg_base->UART0;
         break;
      case 1:
         return (void *)&reg_base->UART1;
         break;
      case 2:
         return (void *)&reg_base->UART2;
         break;
      case 3:
         return (void *)&reg_base->UART3;
         break;
      case 4:
         return (void *)&reg_base->UART4;
         break;
      case 5:
         return (void *)&reg_base->UART5;
         break;
      case 6:
         return (void *)&reg_base->UART6;
         break;
      case 7:
         return (void *)&reg_base->UART7;
         break;
      case 8:
         return (void *)&reg_base->UART8;
         break;
      case 9:
         return (void *)&reg_base->UART9;
         break;
      default:
         return NULL;
   } /* Endswitch */ 
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_serial_vector
* Returned Value   : Vector number upon success, 0 upon failure
* Comments         :
*    This function returns desired interrupt vector number for specified UART
*
*END*----------------------------------------------------------------------*/
PSP_INTERRUPT_TABLE_INDEX _bsp_get_serial_vector
(
   uint8_t dev_num
)
{
    volatile PSP_INTERRUPT_TABLE_INDEX vector;
    switch (dev_num) {
      case 0:
         vector = MCF5441_INT_UART0;
         break;
      case 1:
         vector = MCF5441_INT_UART1;
         break;
      case 2:
         vector = MCF5441_INT_UART2;
         break;
      case 3:
         vector = MCF5441_INT_UART3;
         break;
      case 4:
         vector = MCF5441_INT_UART4;
         break;
      case 5:
         vector = MCF5441_INT_UART5;
         break;
      case 6:
         vector = MCF5441_INT_UART6;
         break;
      case 7:
         vector = MCF5441_INT_UART7;
         break;
      case 8:
         vector = MCF5441_INT_UART8;
         break;
      case 9:
         vector = MCF5441_INT_UART9;
         break;              
      default:
         vector = (PSP_INTERRUPT_TABLE_INDEX)0;
         break;
    } /* Endswitch */
    return vector;
}

/* EOF */
