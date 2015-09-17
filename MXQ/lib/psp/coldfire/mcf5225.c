
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
*   This file contains utiltity functions for use with a mcf5225x.
*
*
*END************************************************************************/

#include "mqx_inc.h"
#include "mcf5225.h"


/* Copies of control registers */
volatile uint32_t _psp_saved_cacr = 0;
volatile uint32_t _psp_saved_mbar = 0x40000000;

/*
** Define padding needed to make the STOREBLOCK_STRUCT align properly
** to cache line size (see mem_prv.h)
*/
// No cache on 5225x, so don't need to align to cache line
//#define PSP_MEM_STOREBLOCK_ALIGNMENT   (2) /* padding in _mqx_uints */


#define NYI()

/*!
 * \brief Initilize the support functions for the mcf5225
 * 
 * \param[in] param dummy parameter
 */
void _mcf5225_initialize_support
   (
      /* [IN] dummy parameter */
      uint32_t param
   )
{ /* Body */
#if PSP_HAS_SUPPORT_STRUCT
   KERNEL_DATA_STRUCT_PTR kernel_data;

   _GET_KERNEL_DATA(kernel_data);

   kernel_data->PSP_SUPPORT_PTR = _mem_alloc_system_zero((uint32_t)sizeof(PSP_SUPPORT_STRUCT));
   _mem_set_type(kernel_data->PSP_SUPPORT_PTR, MEM_TYPE_PSP_SUPPORT_STRUCT);
#endif
} /* Endbody */

/* EOF */
