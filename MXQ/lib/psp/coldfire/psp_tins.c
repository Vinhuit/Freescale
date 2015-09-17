
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
*   This file contains the functions for converting ticks to nanoseconds
*
*
*END************************************************************************/

#include "mqx_inc.h"

#if MQX_HAS_TICK

/*!
 * \brief This function converts ticks into nanoseconds, with rounding or truncating
 * 
 * \param[in] tick_ptr Ticks to be converted
 * \param[out] overflow_ptr pointer to where the overflow bool is to be written
 * \param[in] round bool to control rounding or truncating
 *
 * \return uint32_t - number of nanoseconds
 */
static uint32_t __psp_ticks_to_nanoseconds
   (
      /* [IN] Ticks to be converted */
      PSP_TICK_STRUCT_PTR tick_ptr,

      /* [OUT] pointer to where the overflow bool is to be written */
      bool        *overflow_ptr,
      
      /* [IN] bool to control rounding or truncating */
      bool             round
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   uint64_t                tmp;
   uint32_t                ns_per_tick;

   _GET_KERNEL_DATA(kernel_data);

   tmp = ((uint64_t)kernel_data->HW_TICKS_PER_TICK * tick_ptr->TICKS[0]) +
      tick_ptr->HW_TICKS[0];

   /* 
   ** Convert hardware ticks to ns. (H / (T/S * H/T) * 1000000)
   */
   /* Convert to hardware ticks 
   ** use 2000000000 (extra * 2 for rounding)
   */

   ns_per_tick = 2000000000 / kernel_data->TICKS_PER_SECOND;
   if ((ns_per_tick * kernel_data->TICKS_PER_SECOND) == 2000000000) {
      /* Integral ns per tick */
      tmp = tmp * ns_per_tick;
   } else {
      tmp = tmp * 2000000000;
      tmp = tmp / kernel_data->TICKS_PER_SECOND;
   }/* Endif */

   tmp = tmp / kernel_data->HW_TICKS_PER_TICK;
   /* round or truncate*/
   if (round)
   {
       tmp++;
   }
   tmp = tmp / 2; /* For rounding */
   *overflow_ptr = (bool)(tmp > MAX_UINT_32);
   return (uint32_t)tmp;
      
} /* Endbody */

/*!
 * \brief This function converts ticks into nanoseconds, with rounding
 * 
 * \param[in] tick_ptr Ticks to be converted
 * \param[out] overflow_ptr pointer to where the overflow bool is to be written
 *
 * \return uint32_t - number of nanoseconds
 */
uint32_t _psp_ticks_to_nanoseconds
   (
      /* [IN] Ticks to be converted */
      PSP_TICK_STRUCT_PTR tick_ptr,

      /* [OUT] pointer to where the overflow bool is to be written */
      bool        *overflow_ptr
   )
{ /* Body */
    return __psp_ticks_to_nanoseconds(tick_ptr, overflow_ptr, TRUE);
}

/*!
 * \brief This function converts ticks into nanoseconds, with truncating
 * 
 * \param[in] tick_ptr Ticks to be converted
 * \param[out] overflow_ptr pointer to where the overflow bool is to be written
 *
 * \return uint32_t - number of nanoseconds
 */
uint32_t _psp_ticks_to_nanoseconds_truncate
   (
      /* [IN] Ticks to be converted */
      PSP_TICK_STRUCT_PTR tick_ptr,

      /* [OUT] pointer to where the overflow bool is to be written */
      bool        *overflow_ptr
   )
{ /* Body */
    return __psp_ticks_to_nanoseconds(tick_ptr, overflow_ptr, FALSE);
}
#endif

/* EOF */
