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
* See license agreement file for full license terms including other
* restrictions.
*****************************************************************************
*
* Comments:
*
*   This file contains the type definitions for the mcf51jf microcontroller.
*
*
*END************************************************************************/

#ifndef __mcf51jf_h__
#define __mcf51jf_h__



/*
** PSP common definitions
*/
#define PSP_HAS_DSP                         0
#define PSP_HAS_EMAC                        0
#define PSP_HAS_MMU                         0
#define PSP_HAS_CODE_CACHE                  0
#define PSP_HAS_DATA_CACHE                  0
#define PSP_CACHE_LINE_SIZE                 (0x10)

#define PSP_STOP_ON_IDLE                    0

/*
/** Test for disable cpu wait mode macro.
*/
#if PSP_STOP_ON_IDLE
    #warning "With PSP_STOP_ON_IDLE and no idle_task USB doesn't recover from wait mode"
#endif

#ifndef __ASM__

/* Include mcu specific header file */
#if     MQX_CPU == PSP_CPU_MCF51JF128
    #include "mcf51jf128.h"
#endif

/* Do you want to verify kernel data can be read and written correctly */
#define PSP_KERNEL_DATA_VERIFY_ENABLE       ((uint32_t)__KERNEL_DATA_VERIFY_ENABLE)

/*
** Standard cache macros
*/
#define _DCACHE_FLUSH()
#define _DCACHE_FLUSH_LINE(p)
#define _DCACHE_FLUSH_MBYTES(p, m)
#define _DCACHE_INVALIDATE()
#define _DCACHE_INVALIDATE_LINE(p)
#define _DCACHE_INVALIDATE_MBYTES(p, m)

#define _CACHE_ENABLE(n)
#define _CACHE_DISABLE()

#define _ICACHE_INVALIDATE()
#define _ICACHE_INVALIDATE_LINE(p)
#define _ICACHE_INVALIDATE_MBYTES(p, m)

/*
** The PSP_SUPPORT_STRUCT structure is used to maintain the PSP support information
*/
typedef struct psp_support_struct
{
    uint32_t RESERVED;

} PSP_SUPPORT_STRUCT, * PSP_SUPPORT_STRUCT_PTR;

/*
**  FUNCTION PROTOTYPES AND GLOBAL EXTERNS
*/

#ifdef __cplusplus
extern "C" {
#endif

extern  unsigned char   __KERNEL_DATA_VERIFY_ENABLE[];

void _mcf51xx_initialize_support
    (
        /* [IN] dummy parameter */
        uint32_t param
    );

#ifdef __cplusplus
}
#endif

#endif /*__ASM__ */

#endif /* __mcf51jf_h__ */
/* EOF */
