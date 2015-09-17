
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
*   Load/store reversed endian functions (usually for PCI).
*
*
*END************************************************************************/

#include "mqx_inc.h"

/*!
 * \brief This function loads a 32-bit value and converts its endian byte order
 * 
 * \param[in] p address to read
 *
 * \return Reversed endian 32-bit value
 */
uint32_t _psp_load_long_swapped
   (
      /* [IN] address to read */
      void   *p
   )
{ /* Body */

   uint32_t v = *(uint32_t *)p;

   return (v >> 24)
      |  ((v >> 8) & 0x00FF00)
      |  ((v << 8) & 0xFF0000)
      |   (v << 24);

} /* Endbody */

/*!
 * \brief This function converts a 32-bit value to the reverse endian byte order and 
 *  then stores it in the specified memory location.
 * 
 * \param[in] p address to write
 * \param[in] v data to swap and store
 */
void _psp_store_long_swapped
   (
      /* [IN] address to write */
      void   *p,

      /* [IN] data to swap and store */
      uint32_t v
   )
{ /* Body */

   uint32_t t;

   t =    (v >> 24)
      |  ((v >> 8) & 0x00FF00)
      |  ((v << 8) & 0xFF0000)
      |   (v << 24);

   *(uint32_t *)p = t;

} /* Endbody */

/* EOF */
