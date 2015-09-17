/*HEADER**********************************************************************
*
* Copyright 2008 Freescale Semiconductor, Inc.
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
*   Processor family specific file needed for IRTC module.
*
*
*END************************************************************************/

#ifndef __irtc_mcf5441x__
#define __irtc_mcf5441x__ 1

extern void                      *_bsp_get_rtc_base_address(void);
extern PSP_INTERRUPT_TABLE_INDEX  _bsp_get_rtc_vector(void);

#endif   /* __irtc_mcf5441x__ */

/* EOF */
