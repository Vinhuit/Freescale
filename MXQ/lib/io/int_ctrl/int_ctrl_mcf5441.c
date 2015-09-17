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
*   This file contains definitions for the MFC5445 interrupt controller
*   functions.
*
*
*END************************************************************************/

#include "mqx_inc.h"
#include "bsp.h"

/*FUNCTION*-----------------------------------------------------------------
*
* Function Name   : _mcf5441_int_init
* Returned Value  : uint32_t
* Comments        :
*   Initialize a specific interrupt in the proper interrupt controller
*
*END*---------------------------------------------------------------------*/

uint32_t _mcf5441_int_init
   (
      // [IN} Interrupt number
      PSP_INTERRUPT_TABLE_INDEX irq,

      // [IN} Interrupt priority level
      _int_level                level,

      // [IN} Unmask the interrupt now?
      bool                   unmask
   )
{
    _mqx_int idx;
    uint32_t temp;

    if (irq >= PSP_INT_FIRST_EXTERNAL) {
        idx = irq - PSP_INT_FIRST_EXTERNAL;
   
        temp = _psp_get_sr();
        _psp_set_sr(temp | 0x0700);
        
        if (idx < 64) {
            PSP_GET_ICTRL0_BASE()->ICR[idx] = level & 7;
            
            if (unmask)
                PSP_GET_ICTRL0_BASE()->CIMR = MCF54XX_ICTRL_IMR_N(idx);
            else
                PSP_GET_ICTRL0_BASE()->SIMR = MCF54XX_ICTRL_IMR_N(idx);
        }
        else if (idx < 128) {
            idx -= 64;
            PSP_GET_ICTRL1_BASE()->ICR[idx] = level & 7;
            
            if (unmask)
                PSP_GET_ICTRL1_BASE()->CIMR = MCF54XX_ICTRL_IMR_N(idx);
            else
                PSP_GET_ICTRL1_BASE()->SIMR = MCF54XX_ICTRL_IMR_N(idx);
        }
        else {
            idx -= 128;
            PSP_GET_ICTRL2_BASE()->ICR[idx] = level & 7;
            
            if (unmask)
                PSP_GET_ICTRL2_BASE()->CIMR = MCF54XX_ICTRL_IMR_N(idx);
            else
                PSP_GET_ICTRL2_BASE()->SIMR = MCF54XX_ICTRL_IMR_N(idx);
        }
        
        _psp_set_sr(temp);
    
        return MQX_OK;
    }
    
    return MQX_INVALID_PARAMETER;
}


/*FUNCTION*-----------------------------------------------------------------
*
* Function Name   : _mcf5441_int_unmask
* Returned Value  : uint32_t
* Comments        :
*   Clear the mask bit that inhibits the interrupt
*
*END*---------------------------------------------------------------------*/

uint32_t _mcf5441_int_unmask
   (
      // [IN} Interrupt number
      PSP_INTERRUPT_TABLE_INDEX irq
   )
{ /* Body */
    _mqx_int idx;

    if (irq >= PSP_INT_FIRST_EXTERNAL) {
        idx = irq - PSP_INT_FIRST_EXTERNAL;
   
        if (idx < 64) {
            PSP_GET_ICTRL0_BASE()->CIMR = MCF54XX_ICTRL_IMR_N(idx);
        }
        else if (idx < 128) {
            PSP_GET_ICTRL1_BASE()->CIMR = MCF54XX_ICTRL_IMR_N(idx - 64);
        }
        else {
            PSP_GET_ICTRL2_BASE()->CIMR = MCF54XX_ICTRL_IMR_N(idx - 128);
        }
    
        return MQX_OK;
    }
    
    return MQX_INVALID_PARAMETER;
} /* Endbody */


/*FUNCTION*-----------------------------------------------------------------
*
* Function Name   : _mcf5441_int_mask
* Returned Value  : uint32_t
* Comments        :
*   Set the mask bit that inhibits the interrupt
*
*END*---------------------------------------------------------------------*/

uint32_t _mcf5441_int_mask
   (
      // [IN} Interrupt number
      PSP_INTERRUPT_TABLE_INDEX irq
   )
{
    _mqx_int idx;

    if (irq >= PSP_INT_FIRST_EXTERNAL) {
        idx = irq - PSP_INT_FIRST_EXTERNAL;
   
        if (idx < 64) {
            PSP_GET_ICTRL0_BASE()->SIMR = MCF54XX_ICTRL_IMR_N(idx);
        }
        else if (idx < 128) {
            PSP_GET_ICTRL1_BASE()->SIMR = MCF54XX_ICTRL_IMR_N(idx - 64);
        }
        else {
            PSP_GET_ICTRL2_BASE()->SIMR = MCF54XX_ICTRL_IMR_N(idx - 128);
        }
    
        return MQX_OK;
    }
    
    return MQX_INVALID_PARAMETER;
}


/*FUNCTION*-----------------------------------------------------------------
*
* Function Name   : _mcf5441_int_mask_all
* Returned Value  : void
* Comments        :
*   Mask all external interrupts
*
*END*---------------------------------------------------------------------*/

void  _mcf5441_int_mask_all
   (
      void
   )
{ /* Body */

   PSP_GET_ICTRL0_BASE()->SIMR = MCF54XX_ICTRL_IMR_ALL;
   PSP_GET_ICTRL1_BASE()->SIMR = MCF54XX_ICTRL_IMR_ALL;
   PSP_GET_ICTRL2_BASE()->SIMR = MCF54XX_ICTRL_IMR_ALL;

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mcf5441_swint_invoke
* Returned Value   : 
* Comments         :
*    This function triggers software interrupt.
*
*END*----------------------------------------------------------------------*/

uint32_t _mcf5441_swint_invoke
   (     
      _mqx_uint vector
   )

   { /* Body */
	VMCF5441_STRUCT_PTR reg = (VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR();	
	uint32_t ctrlnum = (vector-64)/64;
	uint32_t intnum = (vector-(ctrlnum*64)) - 64;

	if(ctrlnum == 0){
		if(intnum >= 0 && intnum <= 31){
				reg->ICTRL0.ICTRL.INTFRCL = (1 << intnum);
			}
			else if(intnum >= 32 && intnum <= 63){
				reg->ICTRL0.ICTRL.INTFRCH = (1 << (intnum-32));
			}
			else
				return MQX_INVALID_PARAMETER;
	}
	else if(ctrlnum == 1){
		if(intnum >= 0 && intnum <= 31){
				reg->ICTRL1.ICTRL.INTFRCL = (1 << intnum);
			}
			else if(intnum >= 32 && intnum <= 63){
				reg->ICTRL1.ICTRL.INTFRCH = (1 << (intnum-32));
			}
			else
				return MQX_INVALID_PARAMETER;
	}
	else if(ctrlnum == 2){
		if(intnum >= 0 && intnum <= 31){
				reg->ICTRL2.ICTRL.INTFRCL = (1 << intnum);
			}
			else if(intnum >= 32 && intnum <= 63){
				reg->ICTRL2.ICTRL.INTFRCH = (1 << (intnum-32));
			}
			else
				return MQX_INVALID_PARAMETER;
	}	
	else
		return MQX_INVALID_PARAMETER;
	
  return MQX_OK;
  
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mcf5441_swint_clear
* Returned Value   : 
* Comments         :
*    This function clears interrupt pending flag.
*
*END*----------------------------------------------------------------------*/

uint32_t _mcf5441_swint_clear
(     
   _mqx_uint vector
)

{ /* Body */
	VMCF5441_STRUCT_PTR reg = (VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR();	
	uint32_t ctrlnum = (vector-64)/64;
	uint32_t intnum = (vector-(ctrlnum*64)) - 64;

	if(ctrlnum == 0){
		if(intnum >= 0 && intnum <= 31){
				reg->ICTRL0.ICTRL.INTFRCL = (0 << intnum);
			}
			else if(intnum >= 32 && intnum <= 63){
				reg->ICTRL0.ICTRL.INTFRCH = (0 << (intnum-32));
			}
			else
				return MQX_INVALID_PARAMETER;
	}
	else if(ctrlnum == 1){
		if(intnum >= 0 && intnum <= 31){
				reg->ICTRL1.ICTRL.INTFRCL = (0 << intnum);
			}
			else if(intnum >= 32 && intnum <= 63){
				reg->ICTRL1.ICTRL.INTFRCH = (0 << (intnum-32));
			}
			else
				return MQX_INVALID_PARAMETER;
	}
	else if(ctrlnum == 2){
		if(intnum >= 0 && intnum <= 31){
				reg->ICTRL2.ICTRL.INTFRCL = (0 << intnum);
			}
			else if(intnum >= 32 && intnum <= 63){
				reg->ICTRL2.ICTRL.INTFRCH = (0 << (intnum-32));
			}
			else
				return MQX_INVALID_PARAMETER;
	}	
	else
		return MQX_INVALID_PARAMETER;
	
return MQX_OK;

} /* Endbody */

/* EOF */
