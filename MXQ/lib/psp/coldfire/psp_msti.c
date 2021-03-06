
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
*   This file contains the function for converting milliseconds to a ticks
*
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_HAS_TICK

/*!
 * \brief This function converts milliseconds into ticks
 * 
 * \param[in] msecs The number of milliseconds to convert
 * \param[out] tick_ptr Pointer to tick structure where the result will be stored
 */
void _psp_msecs_to_ticks
   (
       /* [IN] The number of milliseconds to convert */
       _mqx_uint           msecs,

       /* [OUT] Pointer to tick structure where the result will be stored */
       PSP_TICK_STRUCT_PTR tick_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   uint64_t                tmp;
   uint32_t                hwticks_calc;

   _GET_KERNEL_DATA(kernel_data);

   tmp = (uint64_t)msecs * kernel_data->TICKS_PER_SECOND;

   tick_ptr->TICKS[0] = tmp / 1000;
   /* Calculate the remaining milliticks */
   tmp %= 1000;


   /* Convert to hardware ticks 
   ** (use 500 which is 1000/2) to avoid extra * 2 for rounding
   */
   hwticks_calc = 500 / kernel_data->HW_TICKS_PER_TICK;
   if ((hwticks_calc * kernel_data->HW_TICKS_PER_TICK) == 500) {
      tmp = tmp / hwticks_calc;
   } else {
      tmp = ((tmp * kernel_data->HW_TICKS_PER_TICK ) * 2) / 1000;
   }/* Endif */
   tmp++;
   tmp /= 2; /* For rounding */
   tick_ptr->HW_TICKS[0] = (uint32_t)tmp;

} /* Endbody */
#endif

/* EOF */

