
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
*   This file contains the support functions for byte/word and others 
*   manipulations.
*
*
*END************************************************************************/

#include "mqx_inc.h"

#ifdef __ICCCF__

uint16_t _psp_swap2byte(uint16_t n)
{
#if __STDABI__ == 1
    asm ("move.l 4(sp), d0");
#endif /* __STDABI__ */
    asm ("byterev.l d0");
    asm ("swap d0");
    asm ("rts");
}

uint32_t _psp_swap4byte(uint32_t n)
{
#if __STDABI__ == 1
    asm ("move.l 4(sp), d0");
#endif /* __STDABI__ */
    asm ("byterev.l d0");
    asm ("rts");
}

#elif defined(__CODEWARRIOR__)

#if   __REGABI__
    __declspec(register_abi)
#elif __STDABI__
      __declspec(standard_abi)
#endif

asm uint16_t _psp_swap2byte(uint16_t val)
{
#if __STDABI__ == 1
    move.l 4(sp), d0
#endif /* __STDABI__ */
    byterev.l d0
    swap d0
    rts    
}

#if   __REGABI__
    __declspec(register_abi)
#elif __STDABI__
      __declspec(standard_abi)
#endif

asm uint32_t _psp_swap4byte(uint32_t val)
{
#if __STDABI__ == 1
    move.l 4(sp), d0
#endif /* __STDABI__ */
    byterev.l d0
    rts    
}

#else /* __ICCCF__, __CODEWARRIOR__ */

/*!
 * \brief Swap 2 bytes ticks
 * 
 * \param n 
 *
 * \return uint16_t
 */
uint16_t _psp_swap2byte(uint16_t n)
{ /* Body */
    unsigned char temp[2];

    temp[1] = *((unsigned char *)&n);
    temp[0] = ((unsigned char *)&n)[1];
    return (*(uint16_t *)temp);
} /* Endbody */

/*!
 * \brief Swap 4 bytes ticks
 * 
 * \param n 
 *
 * \return uint32_t
 */
uint32_t _psp_swap4byte(uint32_t n)
{ /* Body */
    unsigned char temp[4];

    temp[3] = *((unsigned char *)&n);
    temp[2] = ((unsigned char *)&n)[1];
    temp[1] = ((unsigned char *)&n)[2];
    temp[0] = ((unsigned char *)&n)[3];
    return (*(uint32_t *)temp);
} /* Endbody */

#endif /* __ICCCF__, __CODEWARRIOR__ */
