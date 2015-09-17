
/*HEADER**********************************************************************
*
* Copyright 2008-2012 Freescale Semiconductor, Inc.
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
*   This file contains runtime support for the RV Compiler.
*
*
*END************************************************************************/

#include "mqx.h"
#include <stdio.h>


void __user_setup_stackheap(void);
int __aeabi_atexit(void *, void *, void *);

void *malloc(_mem_size);
void *calloc(_mem_size, _mem_size);
void *realloc(void*,_mem_size);
void free(void *);
void init_hardware(void);
void __main(void);

/*!
 * \brief To create a version of __user_setup_stackheap() that inherits sp from the
 *  execution environment and does not have a heap, set r0 and r2 to zero and
 *  return.
 */
__asm void __user_setup_stackheap(void)
{
    mov r0, #0
    mov r2, #0
    bx lr
}

/*!
 * \brief Override C/C++ runtime __aeabi_atexit function in KEIL to avoid the call
 *  to malloc and free before MQX starts http//infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.faqs/ka3951.html
 *
 * \param object_ptr
 * \param destructor_ptr
 * \param __dso_handle_ptr
 *
 * \return int
 */
int __aeabi_atexit(void *object_ptr, void *destructor_ptr, void *__dso_handle_ptr)
{
    return (1);
}

/*!
 * \brief Override C/C++ runtime heap allocation function in KEIL
 *
 * \param bytes
 *
 * \return pointer
 */
void *malloc(_mem_size bytes)
{
    return _mem_alloc_system(bytes);
}

/*!
 * \brief Override C/C++ runtime heap allocation function in KEIL
 *
 * \param n
 * \param z
 *
 * \return pointer
 */
void *calloc(_mem_size n, _mem_size z)
{
    return _mem_alloc_system_zero(n*z);
}

/*!
 * \brief Override C/C++ runtime heap reallocation function in IAR's DLIB
 *
 * \param ptr
 * \param bytes
 *
 * \return pointer
 */
void *realloc(void* ptr,_mem_size bytes)
{
    return _mem_realloc(ptr,bytes);
}

/*!
 * \brief Override C/C++ runtime heap deallocation function in KEIL
 *
 * \param p
 */
void free(void *p)
{
    _mem_free(p);
}

// NOTE: This is a workaround
// Overload "_initio" at keil startup.
// Linker has at the least two phases.
// - firstly it resolve the regular symbols (because of _fclose_internal)
// - secondly it replace regular symbols with "$Sub$$" one
int $Sub$$_initio(void)
{
    return 0;
}

// NOTE: This is a workaround
// MQX_SDKLIB does not implement freopen and
// KEIL linker requires present of freopen at startup (their freeopen use _fclose_internal)
// but it's never used, so here is the dummy implementation.
FILE * freopen( const char * filename, const char * mode, FILE * stream )
{
    return NULL;
}

// NOTE: This is a workaround
// hang here if ever used
int _fclose_internal(FILE * stream )
{
    __asm("cpsid i");
    while(1);
//  return EOF;
}

/*!
 * \brief Perform necessary toolchain startup routines before main()
 */
void toolchain_startup(void)
{
    init_hardware();
    __main();
}

/* EOF */
