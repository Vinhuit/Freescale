
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
*   This file contains functions to manipulate the SR register
*
*
*END************************************************************************/

#include "mqx_inc.h"

/*
 * \brief Sets the value of the SR register and returns the old value.
 * 
 * \param[in] sr_value The new value for the status register.
 * 
 * \return The old SR register value.
 *  
 * \see _psp_get_sr
 */ 
uint32_t _psp_set_sr
   (
      uint32_t sr_value
   )
{ /* Body */
   uint32_t old_sr_value;

   _PSP_GET_SR(old_sr_value);
   _PSP_SET_SR(sr_value);
   return(old_sr_value);

} /* Endbody */

/*
 * \brief Returns the current SR register value.
 * 
 * \return The current SR register value.
 * 
 * \see _psp_set_sr 
 */ 
uint32_t _psp_get_sr
   (
      void
   )
{ /* Body */
   uint32_t old_sr_value;

   _PSP_GET_SR(old_sr_value);
   return(old_sr_value);

} /* Endbody */

/* EOF */
