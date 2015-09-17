
/*HEADER**********************************************************************
*
* Copyright 2008 Freescale Semiconductor, Inc.
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
* See license agreement file for full license terms including other restrictions.
*****************************************************************************
*
* Comments:
*
*   This file contains the functions used to access the vector
*   table locations.
*
*
*END************************************************************************/

#include "mqx_inc.h"

#if MQX_USE_INTERRUPTS

/*!
 * \brief Changes the the VBR to point to the new address and returns the old 
 * interuupt vector table pointer (VBR) value.
 * 
 * \param[in] new Address of the new interrupt vector table.
 * 
 * \return Address of the previous vector table.
 * 
 * \warning Behavior depends on the BSP and the PSP.
 * 
 * \see _int_get_vector_table
 * \see _int_get_previous_vector_table
 */ 
_mqx_max_type _int_set_vector_table
   (
      _mqx_max_type new
   )
{ /* Body */
   _mqx_max_type old;

   _PSP_GET_VBR(old);
   _PSP_SET_VBR(new);

   return (old);

} /* Endbody */

/*!
 * \brief Gets the current vector table pointer.
 * 
 * \return Address of the current interrupt vector table.
 * 
 * \see _int_set_vector_table
 * \see _int_get_previous_vector_table
 */
_mqx_max_type _int_get_vector_table
   (
      void
   )
{ /* Body */
   _mqx_max_type old;

   _PSP_GET_VBR(old);

   return(old);
   
} /* Endbody */

#endif
/* EOF */
