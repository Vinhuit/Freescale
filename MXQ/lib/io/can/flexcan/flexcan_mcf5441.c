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
    void   *result = NULL;
    
    switch (dev_num)
    {
        case 0: 
            result = (void *)(&((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->FCAN0);
            break;
        case 1: 
            result = (void *)(&((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->FCAN1);
            break;
        default: 
            break;
    }
   
    return result;
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
   
   switch (dev_num)
   {
      case 0: 
          switch (vector_type)
          {
              case FLEXCAN_INT_BUF:
                   index = MCF5441_INT_IFLAG0_BUFnI;
                   break;
              case FLEXCAN_INT_ERR:
                   index = MCF5441_INT_ERRSTAT0_TXWRN_or_RXWRN;
                   break;
              case FLEXCAN_INT_BOFF:
                   index = MCF5441_INT_ERRSTAT0_BOFFINT;
                   break;
              default:
                   break;
          }
          break;
      case 1: 
          switch (vector_type)
          {
              case FLEXCAN_INT_BUF:
                   index = MCF5441_INT_IFLAG1_BUFnI;
                   break;
              case FLEXCAN_INT_ERR:
                   index = MCF5441_INT_ERRSTAT1_TXWRN_or_RXWRN;
                   break;
              case FLEXCAN_INT_BOFF:
                   index = MCF5441_INT_ERRSTAT1_BOFFINT;
                   break;
              default:
                   break;
          }
          break;
      default: break;
   }
   
   return index;
} /* EndBody */
