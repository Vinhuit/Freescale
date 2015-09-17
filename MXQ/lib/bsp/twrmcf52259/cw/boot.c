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
*    This file contains the boot and exception code.
*
*
*END************************************************************************/

#include "mqx_inc.h"
#include "bsp.h"

extern unsigned long far __BOOT_STACK_ADDRESS[];

asm void __boot(void);
asm void __boot_exception(void);
void _start(void);
void mcf5225_init(void);

#if   __REGABI__
    __declspec(register_abi)
#elif __STDABI__
      __declspec(standard_abi)
#endif

asm void __boot(void) {
        /* Assume we could start here via the debugger or jump here at any time */
        move.w #0x2700, sr                  /* Disable all interrupts */
        move.l #__BOOT_STACK_ADDRESS, sp    /* Establish a stack pointer */

        move.l #__VECTOR_TABLE_ROM_START, d0
        movec d0, VBR

        /* Initialize IPSBAR: peripheral base, default is 0x40000000 */
        move.l __IPSBAR, d0
        andi.l #0xC0000000, d0
        add.l #0x1, d0
        move.l d0, 0x40000000

        /* Initialize FLASHBAR: locate internal Flash and validate it */
        move.l #__INTERNAL_FLASH_BASE, d0   /* May be code+data */
        andi.l #0xFFF80000, d0
        add.l #0x61, d0
        movec d0, RAMBAR0

        /* Initialize RAMBAR1: locate SRAM and validate it */
        move.l #__INTERNAL_SRAM_BASE, d0    /* May be code+data */
        andi.l #0xFFF80000, d0
        add.l #0x21, d0
        movec d0, RAMBAR1

        /* Mask all interrupts */
        movea.l  #__IPSBAR,a0
        moveq    #1,d0
        move.l   d0,0x0C0C(a0)
        movea.l  #__IPSBAR,a0
        moveq    #1,d0
        move.l   d0,0x0D0C(a0)

        /* Initialize mcf5225 peripherals */
        jsr mcf5225_init

        /* Jump to the main process */
        jmp _start
}

#if   __REGABI__
    __declspec(register_abi)
#elif __STDABI__
      __declspec(standard_abi)
#endif

asm void __boot_exception(void)
{
   halt
   rte
}
