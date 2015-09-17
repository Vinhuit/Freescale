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
#include "serl_mcf52xx.h"

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_serial_base_address
* Returned Value   : Address upon success, NULL upon failure
* Comments         :
*    This function returns the base register address of the corresponding UART
*
*END*----------------------------------------------------------------------*/
void *_bsp_get_serial_base_address
(
    uint8_t dev_num
)
{
   if (dev_num >= MCF5225_NUM_UARTS)
   {
      return NULL;
   }
   
   return (void *)(&((VMCF5225_STRUCT_PTR)_PSP_GET_IPSBAR())->UART[dev_num]);
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
         vector = MCF5225_INT_UART0;
         break;
      case 1:
         vector = MCF5225_INT_UART1;
         break;
      case 2:
         vector = MCF5225_INT_UART2;
         break;
      default:
         vector = (PSP_INTERRUPT_TABLE_INDEX)0;
         break;
    } /* Endswitch */
    return vector;
}

/* EOF */
