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
*   Processor family specific file needed for flexcan module.
*   Revision History:
*   Date             Version  Changes
*   ---------        -------  -------
*   Sep 26/08        2.50     Initial version
*
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <flexcan.h>

/*FUNCTION****************************************************************
* 
* Function Name    : _bsp_get_flexcan_base_address  
* Returned Value   : Pointer to desired CAN device or NULL if not present
* Comments         :
*    This function returns pointer to base address of address space of the 
*    desired CAN device. Returns NULL otherwise.
*
*END*********************************************************************/
void *_bsp_get_flexcan_base_address
(
   /* [IN] FlexCAN device number */
   uint8_t dev_num
)
{ /* Body */
    if (dev_num != 0)
    {
        return NULL;
    }
   
    return (void *)(&((VMCF5225_STRUCT_PTR)_PSP_GET_IPSBAR())->FCAN);
} /* EndBody */

/*FUNCTION****************************************************************
* 
* Function Name    : _bsp_get_flexcan_vector
* Returned Value   : MQX vector number for specified interrupt
* Comments         :
*    This function returns index into MQX interrupt vector table for
*    specified flexcan interrupt. If not known, returns 0.
*
*END*********************************************************************/
PSP_INTERRUPT_TABLE_INDEX _bsp_get_flexcan_vector 
(
   /* [IN] FlexCAN device number */
    uint8_t dev_num,
   /* [IN] FlexCAN interrupt vector type */
    uint8_t vector_type,
   /* [IN] FlexCAN interrupt vector index */
    uint32_t vector_index
)
{ /* Body */
   volatile PSP_INTERRUPT_TABLE_INDEX index = (PSP_INTERRUPT_TABLE_INDEX) 0;
   
   if (dev_num != 0) 
      return index;
   
   switch (vector_type)
   {
      case FLEXCAN_INT_BUF:
           switch (vector_index)
           {
               case (0):
                  index = MCF5225_INT_FLEXCAN_BUF0;
                  break;
               case (1):
                  index = MCF5225_INT_FLEXCAN_BUF1;
                  break;
               case (2):
                  index = MCF5225_INT_FLEXCAN_BUF2;
                  break;
               case (3):
                  index = MCF5225_INT_FLEXCAN_BUF3;
                  break;
               case (4):
                  index = MCF5225_INT_FLEXCAN_BUF4;
                  break;
               case (5):
                  index = MCF5225_INT_FLEXCAN_BUF5;
                  break;
               case (6):
                  index = MCF5225_INT_FLEXCAN_BUF6;
                  break;
               case (7):
                  index = MCF5225_INT_FLEXCAN_BUF7;
                  break;
               case (8):
                  index = MCF5225_INT_FLEXCAN_BUF8;
                  break;
               case (9):
                  index = MCF5225_INT_FLEXCAN_BUF9;
                  break;
               case (10):
                  index = MCF5225_INT_FLEXCAN_BUF10;
                  break;
               case (11):
                  index = MCF5225_INT_FLEXCAN_BUF11;
                  break;
               case (12):
                  index = MCF5225_INT_FLEXCAN_BUF12;
                  break;
               case (13):
                  index = MCF5225_INT_FLEXCAN_BUF13;
                  break;
               case (14):
                  index = MCF5225_INT_FLEXCAN_BUF14;
                  break;
               case (15):
                  index = MCF5225_INT_FLEXCAN_BUF15;
                  break;
               default:
                  break;
           }
           break;
      case FLEXCAN_INT_ERR:
           index = MCF5225_INT_FLEXCAN_ERR_INT;
           break;
      case FLEXCAN_INT_BOFF:
           index = MCF5225_INT_FLEXCAN_BOFF_INT;
           break;
      default:
           break;
   }
   return index;
} /* EndBody */
