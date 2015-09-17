/*HEADER**********************************************************************
*
* Copyright 2009 Freescale Semiconductor, Inc.
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
*   This file contains ColdFire+ interrupt controller functions
*
*
*END************************************************************************/

#include "mqx_inc.h"
#include "int_ctrl_mcfplus.h"

/* local variables */
static uint16_t  status_reg;

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name   : _mcfplus_int_mask_all
* Returned Value  : void
* Comments        : Mask all interrupts (setting actual int level to 7)
*
*END*-----------------------------------------------------------------------*/

void  _mcfplus_int_mask_all
    (
        void
    )
{ /* Body */
    status_reg = _psp_get_sr();
    _psp_set_sr(0x2700);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name   : _mcfplus_int_mask_all
* Returned Value  : void
* Comments        : Unmask all interrupts (restoring actual int level from 7)
*
*END*-----------------------------------------------------------------------*/

void  _mcfplus_int_unmask_all
    (
        void
    )
{ /* Body */
    _psp_set_sr(status_reg);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name   : _mcfplus_int_mask
* Returned Value  : MQX error code
* Comments        : Disable interrupt from given vector.
*
*END*-----------------------------------------------------------------------*/

uint32_t _mcfplus_int_mask
    (
        uint32_t vector
    )
{ /* Body */
    if ((vector >= 64) && (vector <= 102))    {
        vector -= 64;
    }
    else if ((vector >= 110) && (vector <= 114))    {
        vector -= 71;
    }
    else    {
        return MQX_INVALID_PARAMETER;
    }

    if (vector < 32)    {
        INTC_IMRL |= (1 << vector);
    }
    else    {
        INTC_IMRH |= (1 << (vector - 32));
    }

    return MQX_OK;
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name   : _mcfplus_int_unmask
* Returned Value  : MQX error code
* Comments        : Enable interrupt from given vector.
*
*END*-----------------------------------------------------------------------*/

uint32_t _mcfplus_int_unmask
    (
        uint32_t vector
    )
{ /* Body */
    if ((vector >= 64) && (vector <= 102))    {
        vector -= 64;
    }
    else if ((vector >= 110) && (vector <= 114))    {
        vector -= 71;
    }
    else    {
        return MQX_INVALID_PARAMETER;
    }

    if (vector < 32)    {
        INTC_IMRL &= (~ (1 << vector));
    }
    else    {
        INTC_IMRH &= (~ (1 << (vector - 32)));
    }

    return MQX_OK;
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mcfplus_swint_invoke
* Returned Value   : uint32_t
* Comments         :
*    This function triggers software interrupt.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _mcfplus_swint_invoke
   (
      /* [IN] Number of interrupt to invoke */
      _mqx_uint vector

   )
   { /* Body */
	uint32_t intnum = vector - 103;

	if(intnum >= 0 && intnum <= 6){
	INTC_FRC = (1 << intnum);
	}
	else
		return MQX_INVALID_PARAMETER;
	
  return MQX_OK;
  
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mcfplus_swint_clear
* Returned Value   : uint32_t
* Comments         :
*    This function clears interrupt pending flag.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _mcfplus_swint_clear
   (
      /* [IN] Number of interrupt to invoke */
      _mqx_uint vector

   )
   { /* Body */
	uint32_t intnum = vector - 103;

	if(intnum >= 0 && intnum <= 6){
	INTC_FRC = (0 << intnum);
	}
	else
		return MQX_INVALID_PARAMETER;
	
  return MQX_OK;
  
} /* Endbody */

/* EOF */
