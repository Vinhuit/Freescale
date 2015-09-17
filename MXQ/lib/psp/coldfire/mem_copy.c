
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
*   This file contains the functions for copying memory.
*
*
*END************************************************************************/

#include "mqx_inc.h"

/*!
 * \brief This function copies the specified number of bytes from the source address 
 *  to the destination address. No attempt is made to handle overlapping copies 
 *  to prevent loss of data. The copying is optimized to avoid alignment problems, 
 *  and attempts to copy 32bit numbers optimally
 * 
 * \param[in] from_ptr address to copy data from
 * \param[in] to_ptr address to copy data to
 * \param[in] number_of_bytes number of bytes to copy
 */
#if MQXCFG_MEM_COPY

void _mem_copy
   (
      /* [IN] address to copy data from */
      const void    *from_ptr,

      /* [IN] address to copy data to */
      void    *to_ptr,

      /* [IN] number of bytes to copy */
      register _mem_size number_of_bytes
   )
{ /* Body */
#if MQX_USE_SMALL_MEM_COPY
   register uint8_t *from8_ptr = (uint8_t *) from_ptr;
   register uint8_t *to8_ptr = (uint8_t *) to_ptr;

   if (number_of_bytes) {
      while (number_of_bytes--) {
         *to8_ptr++ = *from8_ptr++;
      } /* Endwhile */      
   } /* Endif */
#else
   uint8_t *from8_ptr = (uint8_t *) from_ptr;
   uint8_t *to8_ptr = (uint8_t *) to_ptr;
   uint16_t *from16_ptr = (uint16_t *) from_ptr;
   uint16_t *to16_ptr = (uint16_t *) to_ptr;
   register uint32_t *from32_ptr = (uint32_t *) from_ptr;
   register uint32_t *to32_ptr = (uint32_t *) to_ptr;
   register uint32_t loops;

   if (number_of_bytes > 3) {
      /* Try to align source on word */
      if ((uint32_t)from_ptr & 1) {
         from8_ptr = (uint8_t *) from_ptr;
         to8_ptr = (uint8_t *) to_ptr;

         *to8_ptr++ = *from8_ptr++;

         from_ptr = from8_ptr;
         to_ptr = to8_ptr;
         --number_of_bytes;
      } /* Endif */

      /* Try to align source on longword */
      if ((uint32_t)from_ptr & 2) {
         from16_ptr = (uint16_t *) from_ptr;
         to16_ptr = (uint16_t *) to_ptr;

         *to16_ptr++ = *from16_ptr++;

         from_ptr = from16_ptr;
         to_ptr = to16_ptr;
         number_of_bytes -= 2;
      } /* Endif */

      from32_ptr = (uint32_t *) from_ptr;
      to32_ptr = (uint32_t *) to_ptr;
#if MQX_USE_BLOCK_MEM_COPY
      /* 
      ** To increase performance a bit, we will copy 64 bytes (16 * longwords) sequentially
      ** This consumes more RAM, more flash, but gets less instruction cycles.
      */
      for (loops = (number_of_bytes >> 6); loops > 0; loops--) {
         /* copy 16 longwords */
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
      } /* Endwhile */

      /* Now, write the rest of bytes */
      switch ((number_of_bytes >> 2) & 0xF) {
         case 15: *to32_ptr++ = *from32_ptr++;
         case 14: *to32_ptr++ = *from32_ptr++;
         case 13: *to32_ptr++ = *from32_ptr++;
         case 12: *to32_ptr++ = *from32_ptr++;
         case 11: *to32_ptr++ = *from32_ptr++;
         case 10: *to32_ptr++ = *from32_ptr++;
         case 9:  *to32_ptr++ = *from32_ptr++;
         case 8:  *to32_ptr++ = *from32_ptr++;
         case 7:  *to32_ptr++ = *from32_ptr++;
         case 6:  *to32_ptr++ = *from32_ptr++;
         case 5:  *to32_ptr++ = *from32_ptr++;
         case 4:  *to32_ptr++ = *from32_ptr++;
         case 3:  *to32_ptr++ = *from32_ptr++;
         case 2:  *to32_ptr++ = *from32_ptr++;
         case 1:  *to32_ptr++ = *from32_ptr++;
      } /* Endswitch */

#else /* MQX_USE_BLOCK_MEM_COPY */
      for (loops = number_of_bytes >> 2; loops != 0; loops--) {
         *to32_ptr++ = *from32_ptr++;
      }
#endif /* MQX_USE_BLOCK_MEM_COPY */
      from_ptr = from32_ptr;
      to_ptr = to32_ptr;
   } /* Endif */

   /* Copy all remaining bytes */
   if (number_of_bytes & 2) {
      from16_ptr = (uint16_t *) from_ptr;
      to16_ptr = (uint16_t *) to_ptr;

      *to16_ptr++ = *from16_ptr++;

      from_ptr = from16_ptr;
      to_ptr = to16_ptr;
   } /* Endif */

   if (number_of_bytes & 1) {
      * (uint8_t *) to_ptr = * (uint8_t *) from_ptr;
   } /* Endif */
#endif
} /* Endbody */

#endif

/* EOF */
