/*HEADER**********************************************************************
*
* Copyright 2011 Freescale Semiconductor, Inc.
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
*   This file contains definitions for the 16-bit MTIM functions.
*
*
*END************************************************************************/

#ifndef __timer_mtim16_h__
#define __timer_mtim16_h__

#include <mqx.h>
#include <bsp.h>

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------*/
/*                      FUNCTION PROTOTYPES
*/

void   *_bsp_get_mtim16_base_address (uint8_t);
_mqx_uint _bsp_get_mtim16_vector(uint8_t);
void _bsp_mtim16_clk_en (uint8_t);

_mqx_int _mtim16_timer_install(uint8_t, uint32_t, uint32_t, uint32_t, INT_ISR_FPTR, bool);
_mqx_int _mtim16_timer_install_kernel(uint8_t, uint32_t, uint32_t, uint32_t, bool);
uint32_t _mtim16_timer_init(uint8_t, uint32_t, uint32_t, bool);

uint32_t _mtim16_get_hwticks(void *);

void _mtim16_mask_int(uint8_t);
void _mtim16_unmask_int(uint8_t);
void _mtim16_clear_int(uint8_t);


#ifdef __cplusplus
}
#endif

#endif
