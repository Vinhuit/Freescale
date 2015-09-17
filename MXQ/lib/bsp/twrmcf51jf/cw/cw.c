/*HEADER**********************************************************************
*
* Copyright 2008- Freescale Semiconductor, Inc.
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
*   This file contains runtime support for the CodeWarrior.
*
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"

#if BSPCFG_ENABLE_CPP
typedef void (*initializer_fcn_t)(void);
extern initializer_fcn_t __sinit__[];
#endif /* BSPCFG_ENABLE_CPP */

void    __destroy_global_chain(void);

/* CodeWarrior overrides prototypes */
#if BSPCFG_ENABLE_CPP
void __cpp_init(void);
static void __cpp_fini(void);
#endif /* BSPCFG_ENABLE_CPP */
int  exit(int);
void   *malloc(_mem_size);
void   *calloc(_mem_size, _mem_size);
void    free(void *);

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : __cpp_init
* Returned Value   : void
* Comments         :
*   Override C++ initializer of static instances
*
*END*-----------------------------------------------------------------------*/
#if BSPCFG_ENABLE_CPP
void __cpp_init(void)
{
   initializer_fcn_t *constructor;

   /* call constructor of static instances, ctors in CodeWarrior 10 is not used anymore */
   for (constructor = __sinit__; *constructor; constructor++) {
      (*constructor)();
   }
}
#endif /* BSPCFG_ENABLE_CPP */

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : __cpp_fini
* Returned Value   : void
* Comments         :
*   Override C++ deinitializer of static instances
*
*END*-----------------------------------------------------------------------*/
#if BSPCFG_ENABLE_CPP
static void __cpp_fini(void)
{
    /*
    initializer_fcn_t *destructor;
    */

    /* call destructor of static instances */
    /*
    for (destructor = _dtors; *destructor; destructor++) {
      (*destructor)();
    }
    */
}
#endif /* BSPCFG_ENABLE_CPP */

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : exit
* Returned Value   : should not return
* Comments         :
*   Replacement for CodeWarrior's exit function
*
*END*-----------------------------------------------------------------------*/
#if MQX_EXIT_ENABLED
int exit(int status)
{
/* Semi-normal CodeWarrior shutdown sequence */
#if BSPCFG_ENABLE_CPP
    __destroy_global_chain();
    __cpp_fini();
#endif

/*
** Change for whatever is appropriate for your board
** and application.  Perhaps a software reset or
** some kind of trap/call to the kernel soft re-boot.
*/
    for(;;) {
      volatile uint8_t a = 0;
    }

    return(0);
}

#else  /* MQX_EXIT_ENABLED */

int exit(int status)
{
    return 0;
}

#endif /* MQX_EXIT_ENABLED */

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : malloc
* Returned Value   : pointer
* Comments         :
*   Override C/C++ runtime heap allocation function
*
*END*-----------------------------------------------------------------------*/
void *malloc(_mem_size bytes)
{
    return _mem_alloc_system(bytes);
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : calloc
* Returned Value   : pointer
* Comments         :
*   Override C/C++ runtime heap deallocation
*
*END*-----------------------------------------------------------------------*/
void *calloc(_mem_size n, _mem_size z)
{
    return _mem_alloc_system_zero(n*z);
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : free
* Returned Value   : void
* Comments         :
*   Override C/C++ runtime free
*
*END*-----------------------------------------------------------------------*/
void free(void *p)
{
    _mem_free(p);
}

/* EOF */
