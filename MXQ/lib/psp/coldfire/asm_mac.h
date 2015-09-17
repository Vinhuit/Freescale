/*HEADER**********************************************************************
*
* Copyright 2008 Freescale Semiconductor, Inc.
* Copyright 2008 Embedded Access Inc.
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
*   This file contains macros used by the CW assembler 
*
*
*END************************************************************************/

#ifndef __asm_mac_cw_h__
#define __asm_mac_cw_h__ 1

#if defined(__CWARM__)
#define ASM_PREFIX(x)	x
#else
#define ASM_PREFIX(x) 	_##x 
#endif

#define ASM_EQUATE(label,value) label  .equ  value
#define ASM_EXTERN(label)       .extern ASM_PREFIX(label)
#define ASM_ALIGN(value)        .align value
#define ASM_PUBLIC(label)       .global ASM_PREFIX(label) 
#define ASM_CONST16(value)      .short value
#define ASM_CONST32(value)      .long value
#define ASM_LABEL(label)        label:
#define ASM_LABEL_CONST32(label,value) label .long value
#define ASM_CODE_SECTION(name)  .section name,16,x
#define ASM_DATA_SECTION(name)  TBD /* .section name,16,r */
#define ASM_END                 .end
#define ASM_COMP_SPECIFIC_DIRECTIVES

#define ASM_PUBLIC_BEGIN(name) 
#define ASM_PUBLIC_FUNC(name)     
#define ASM_PUBLIC_END(name)

#endif /* __asm_mac_cw_h__ */
