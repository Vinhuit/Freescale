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

#pragma define_section bootstrap_nfcsram_code ".bootstrap_nfcsram_code" far_absolute R
#pragma define_section bootstrap_sram_code ".bootstrap_sram_code" far_absolute R
#pragma define_section warm_start ".warm_start" far_absolute R

extern unsigned long far __BOOT_STACK_ADDRESS[];

void warm_start (void);
asm void __boot(void);
asm void __boot_exception(void);

__declspec(bootstrap_nfcsram_code) 
asm void __boot_nfc(void);

__declspec(bootstrap_sram_code) 
asm void __boot_sram_exception(void);

__declspec(bootstrap_nfcsram_code) 
asm void __boot_nfcsram_exception(void);

void _start(void);
void mcf5441_init(void);
void _mcf5441_int_mask_all(void);
extern void bootstrap_nfcsram_copy_code_to_sram (void);
extern void bootstrap_sram_main(void);
extern void* bootstrap_nfcsram_vector[64];
extern void* bootstrap_sram_vector[64];

#if   __REGABI__
    __declspec(register_abi)
#elif __STDABI__
      __declspec(standard_abi)
#endif

__declspec(warm_start) 
void warm_start (void)
{
        __boot();
}

#if   __REGABI__
    __declspec(register_abi)
#elif __STDABI__
      __declspec(standard_abi)
#endif

asm void __boot(void) {
        /* Assume we could start here via the debugger or jump here at any time */
        move.w #0x2700, sr                  /* Disable all interrupts */
        move.l #__BOOT_STACK_ADDRESS, sp    /* Establish a stack pointer */

        /* Invalidate the cache and disable it */
        move.l #0x01000000, d0
        movec d0, CACR

        /* Disable ACRs */
        moveq.l #0, d0
        movec d0, ACR0
        movec d0, ACR1
        movec d0, ACR2
        movec d0, ACR3

        /* Initialize RAMBAR - locate it on the data bus */
        move.l #__INTERNAL_SRAM_BASE, d0
        andi.l #0xFFF80000, d0
        add.l #0x221, d0
        movec d0, RAMBAR

        move.l #__VECTOR_TABLE_ROM_START, d0
        movec d0, VBR

        /* Mask all interrupts */
        jsr _mcf5441_int_mask_all
        
        /* Initialize mcf5441 peripherals */
        jsr mcf5441_init

        /* Jump to the main process */
        jmp _start
}

#if   __REGABI__
    __declspec(register_abi)
#elif __STDABI__
      __declspec(standard_abi)
#endif

__declspec(bootstrap_nfcsram_code) 
asm void __boot_nfc(void) {
        /* Assume we could start here via the debugger or jump here at any time */
        move.w #0x2700, sr                  /* Disable all interrupts*/

        /* Initialize RAMBAR - locate it on the data bus*/
        move.l #__INTERNAL_SRAM_BASE, d0
        andi.l #0xFFF80000, d0
        add.l #0x221, d0
        movec d0, RAMBAR

        /* Establish a stack pointer */
        move.l #__BOOT_STACK_ADDRESS, sp    

        /* Invalidate the cache and disable it */
        move.l #0x01000000, d0
        movec d0, CACR

        /* Disable ACRs */
        moveq.l #0, d0
        movec d0, ACR0
        movec d0, ACR1
        movec d0, ACR2
        movec d0, ACR3


        /* Set VBR to NFC_SRAM vector table address */
        move.l #bootstrap_nfcsram_vector, d0 
        movec d0, VBR

        /* Copy the rest of the bootstrap code to the SRAM memory */
        jsr bootstrap_nfcsram_copy_code_to_sram
        
        /* Change VBR to SRAM vector table address */
        move.l #bootstrap_sram_vector, d0 
        movec d0, VBR

        /* Jump to the SRAM portion of the bootstrap code */
        jmp bootstrap_sram_main
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

#if   __REGABI__
    __declspec(register_abi)
#elif __STDABI__
      __declspec(standard_abi)
#endif

__declspec(bootstrap_sram_code) 
asm void __boot_sram_exception(void)
{
   halt
   rte
}

#if   __REGABI__
    __declspec(register_abi)
#elif __STDABI__
      __declspec(standard_abi)
#endif

__declspec(bootstrap_nfcsram_code) 
asm void __boot_nfcsram_exception(void)
{
   halt
   rte
}
