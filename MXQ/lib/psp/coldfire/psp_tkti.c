
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
*   This file contains the functions for converting between ticks and
*   the TIME_STRUCT format.
*
*
*END************************************************************************/

#include "mqx_inc.h"

/* Start CR 1256 */

/*!
 * \brief This function converts ticks into the time struct format
 * 
 * \param[in] tick_ptr Pointer to the tick struct to store the results in
 * \param[out] time_ptr Pointer to the time struct to convert
 *
 * \return bool - Returns FALSE if overflow occurs
 */
#if MQX_HAS_TICK

#if 1


bool _psp_ticks_to_time
   (
      /* [IN] Pointer to the tick struct to store the results in */
      PSP_TICK_STRUCT_PTR tick_ptr,

      /* [OUT] Pointer to the time struct to convert */
      TIME_STRUCT_PTR     time_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   uint64_t ms;
   uint32_t tps;

   _GET_KERNEL_DATA(kernel_data);
   
   tps = kernel_data->TICKS_PER_SECOND;

   /* Saturate if ticks go out of range of time struct */
   if ( (tick_ptr->TICKS[0] / tps) > MAX_UINT_32) {
      time_ptr->SECONDS      = MAX_UINT_32;
      time_ptr->MILLISECONDS = 999;
      return FALSE;
   } /* Endif */

#ifdef __ICCCF__   /* IAR workaround - problem with 64bit local variables (compiler problem) */
 
   time_ptr->SECONDS = tick_ptr->TICKS[0] / kernel_data->TICKS_PER_SECOND;
   time_ptr->MILLISECONDS = ((tick_ptr->TICKS[0] * 1000) / kernel_data->TICKS_PER_SECOND) - time_ptr->SECONDS * 1000;
#else
 
   ms = (tick_ptr->TICKS[0] * 1000) / kernel_data->TICKS_PER_SECOND;

   time_ptr->SECONDS = ms / 1000;
   time_ptr->MILLISECONDS = ms - time_ptr->SECONDS * 1000;
#endif
   
   return TRUE;
} /* Endbody   */

#else

bool _psp_ticks_to_time
   (
      /* [IN] Pointer to the tick struct to store the results in */
      PSP_TICK_STRUCT_PTR tick_ptr,

      /* [OUT] Pointer to the time struct to convert */
      TIME_STRUCT_PTR     time_ptr
   )
{ /* Body */
   static uint64_t         max_ticks = 0;
   static uint64_t         hw_ticks_per_sec;
   static uint32_t         tpms;
   static bool         evenly_divisible;

   uint64_t                tmp;
   uint32_t                tps;
   KERNEL_DATA_STRUCT_PTR kernel_data;

   _GET_KERNEL_DATA(kernel_data);

   tps = kernel_data->TICKS_PER_SECOND;

   if (max_ticks == 0) {
      max_ticks = (uint64_t)tps * MAX_UINT_32;
      tpms = 1000 / tps;
      evenly_divisible = 1000 % tps == 0;
      hw_ticks_per_sec = (uint64_t)tps * kernel_data->HW_TICKS_PER_TICK;
   } /* Endif */

   /* Saturate if ticks go out of range of time struct */
   if (tick_ptr->TICKS[0] > max_ticks) {
      time_ptr->SECONDS      = MAX_UINT_32;
      time_ptr->MILLISECONDS = 999;
      return FALSE;
   } /* Endif */

   /* This is guaranteed to work since ticks/tps is less than MAX_UINT_32 */
   if (evenly_divisible) {
      tmp = tick_ptr->TICKS[0] * tpms;
   } else {
      tmp = (tick_ptr->TICKS[0] * 1000) / tps;
   } /* Endif */

   time_ptr->SECONDS      = tmp / 1000;
   time_ptr->MILLISECONDS = tmp - time_ptr->SECONDS * 1000;

   tmp =  (uint64_t)tick_ptr->HW_TICKS[0] * 1000 * 2;
   if (tmp > MAX_UINT_32) {
      tmp =  tmp / hw_ticks_per_sec;
      tmp += 1;
      tmp >>= 1;

      time_ptr->MILLISECONDS += (uint32_t)tmp;
   } else {
      uint32_t tmp32 = tmp;
      tmp32 = tmp32 / tps / kernel_data->HW_TICKS_PER_TICK;
      tmp32 += 1;
      tmp32 >>= 1;
      time_ptr->MILLISECONDS += tmp32;
   } /* Endif */

   if (time_ptr->MILLISECONDS >= 1000) {
      time_ptr->SECONDS++;
      time_ptr->MILLISECONDS -= 1000;
   } /* Endif */

   return TRUE;

} /* Endbody */
#endif
#endif

/* End CR 1256 */

/* EOF */
