/*HEADER**********************************************************************
*
* Copyright 2009 Freescale Semiconductor, Inc.
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
*   This file contains definitions for the ColdFire+ interrupt controller
*
*
*END************************************************************************/
#ifndef __int_ctrl_mcfplus_h__
    #define __int_ctrl_mcfplus_h__

/* type definitions */
typedef unsigned char _int_level;

#ifdef __cplusplus
extern "C" {
#endif

/* function prototypes */
extern void    _mcfplus_int_unmask_all(void);
extern void    _mcfplus_int_mask_all(void);
extern uint32_t _mcfplus_int_mask(uint32_t);
extern uint32_t _mcfplus_int_unmask(uint32_t);
extern uint32_t _mcfplus_swint_invoke(uint32_t);
extern uint32_t _mcfplus_swint_clear(uint32_t);

#ifdef __cplusplus
}
#endif

#endif /* __int_ctrl_mcfplus_h__ */
/* EOF */
