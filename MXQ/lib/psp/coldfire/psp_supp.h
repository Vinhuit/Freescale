/*HEADER**********************************************************************
*
* Copyright 2010 Freescale Semiconductor, Inc.
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
*   This file contains the support functions for byte/word and others 
*   manipulations.
*
*
*END************************************************************************/
#ifndef __PSP_SUPP_H__
#define __PSP_SUPP_H__ 1

#ifndef __ASM__

/*----------------------------------------------------------------------*/
/*
**                    FUNCTION PROTOTYPES
*/

#ifdef __cplusplus
extern "C" {
#endif

#define _PSP_SWAP2BYTE(n)   _psp_swap2byte(n)
#define _PSP_SWAP4BYTE(n)   _psp_swap4byte(n)

#ifdef __ICCCF__

uint16_t _psp_swap2byte(uint16_t n);
uint32_t _psp_swap4byte(uint32_t n);

#elif defined(__CODEWARRIOR__)

uint16_t _psp_swap2byte(uint16_t n);
uint32_t _psp_swap4byte(uint32_t n);

#elif defined(__ICCARM__)

#define _psp_swap2byte(n)   (uint16_t)__REVSH(n)
#define _psp_swap4byte(n)   __REV(n)

#else // __ICCCF__, __CODEWARRIOR__, __ICCARM__

uint16_t _psp_swap2byte(uint16_t n);
uint32_t _psp_swap4byte(uint32_t n);

#endif // __ICCCF__, __CODEWARRIOR__, __ICCARM__

#ifdef __cplusplus
}
#endif

#endif // __ASM__

#endif // __PSP_SUPP_H__
