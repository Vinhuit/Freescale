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
*
*END************************************************************************/
#ifndef _psp_stdabi_h_
#define _psp_stdabi_h_

#include <psp_abi.h>

#ifdef __STDABI__
    #if __STDABI__ == 0
        #error Project for standard ABI, but compiler do not use standard ABI
    #endif
#endif

#define PSP_ABI         PSP_ABI_STD

#endif
