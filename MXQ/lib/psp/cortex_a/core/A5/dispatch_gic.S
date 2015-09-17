/*HEADER**********************************************************************
*
* Copyright 2012 Freescale Semiconductor, Inc.
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
*
*END************************************************************************/

#include <asm_mac.h>

#include "mqx_cnfg.h"
#include "types.inc"
#include "psp_prv.inc"

#define __ASM__
#include "psp_cpu.h"
#include "mqx_prv.h"
#undef __ASM__

                ASM_CODE_SECTION(KERNEL)
                SET_FUNCTION_ALIGNMENT

                ASM_EXTERN(_mqx_kernel_data)
                ASM_EXTERN(_sched_execute_scheduler_internal_isr)
                ASM_EXTERN(reset)
                ASM_EXTERN(_psp_push_fp_context)
                ASM_EXTERN(_psp_pop_fp_context)

                ASM_EXTERN(_fiq_sp)
                ASM_EXTERN(_abort_sp)
                ASM_EXTERN(_undef_sp)

                ASM_PUBLIC(_psp_exception_return)
                ASM_PUBLIC(_int_kernel_isr_return_internal)
                ASM_PUBLIC(_int_kernel_isr)
                ASM_PUBLIC(_gic_init)
                
                ASM_PUBLIC(reset_addr)

                ASM_PUBLIC_BEGIN(_gic_init)
                ASM_PUBLIC_FUNC(_gic_init)
ASM_LABEL(_gic_init)
                push {r0}
                ldr r0, =_vector_tbl
                mcr p15, 0, r0, c12, c0, 0      /* write VBAR register */

                /* SCTLR bit V - location of exception vectors -> 0x00000000-0x0000001C */
                mrc p15, 0, r0, c1, c0, 0       /* read SCTLR */
                bic r0, r0, #0x2000
                mcr p15, 0, r0, c1, c0, 0       /* write SCTLR */
                pop {r0}

                bx lr
                ASM_PUBLIC_END(_gic_init)


                ASM_PUBLIC_BEGIN(undefined_handler)
                ASM_PUBLIC_FUNC(undefined_handler)
ASM_LABEL(undefined_handler)
                stmfd sp!, {r0-r12, lr}
                ldr r0, =1
                b _int_kernel_isr
                ASM_PUBLIC_END(undefined_handler)


                ASM_PUBLIC_BEGIN(svc_handler)
                ASM_PUBLIC_FUNC(svc_handler)
ASM_LABEL(svc_handler)
                stmfd sp!, {r0-r12, lr}
                ldr r0, =2
                b _int_kernel_isr
                ASM_PUBLIC_END(svc_handler)


                ASM_PUBLIC_BEGIN(prefetch_handler)
                ASM_PUBLIC_FUNC(prefetch_handler)
ASM_LABEL(prefetch_handler)
                stmfd sp!, {r0-r12, lr}
                ldr r0, =3
                b _int_kernel_isr
                ASM_PUBLIC_END(prefetch_handler)


                ASM_PUBLIC_BEGIN(abort_handler)
                ASM_PUBLIC_FUNC(abort_handler)
ASM_LABEL(abort_handler)
                stmfd sp!, {r0-r12, lr}
                ldr r0, =4
                b _int_kernel_isr
                ASM_PUBLIC_END(abort_handler)


                ASM_PUBLIC_BEGIN(irq_handle)
                ASM_PUBLIC_FUNC(irq_handle)
ASM_LABEL(irq_handle)
                stmfd sp!, {r0-r12, lr}

                ldr r1, =0x40002100             /* GICC */
                ldr r0, [r1, #0x0c]             /* GICC_IAR */
                add r0, r0, #16
                
                b _int_kernel_isr
                ASM_PUBLIC_END(irq_handle)


                ASM_PUBLIC_BEGIN(fiq_handle)
                ASM_PUBLIC_FUNC(fiq_handle)
ASM_LABEL(fiq_handle)
                stmfd sp!, {r0-r12, lr}
                ldr r0, =7
                b _int_kernel_isr
                ASM_PUBLIC_END(fiq_handle)

/*
 * This is the entry point for IRQ exception.  On entry, mode is changed to
 * IRQ and interrupts are disabled.  The lr and sp are banked and spsr
 * contains the previous cpsr from svc mode.
 */

/* interrupt prologue */
                ASM_PUBLIC_BEGIN(_int_kernel_isr)
                ASM_PUBLIC_FUNC(_int_kernel_isr)
ASM_LABEL(_int_kernel_isr)
/*                stmfd sp!, {r0-r12, lr}*/

                GET_KERNEL_DATA r3              /* get the kernel data address */

                /* increment in interrupt counter */
                ldrh r1, [r3, #KD_IN_ISR]
                add r1, r1, #1
                strh r1, [r3, #KD_IN_ISR]

#if MQX_SAVE_FP_ALWAYS && PSP_HAS_FPU
                blx _psp_push_fp_context
                /* restore link register - call of function change LR, but we need store original value*/
               ldr lr, [sp, #0x13c]
#endif

                /* create interrupt content */
                ldr r1, [r3, #KD_INTERRUPT_CONTEXT_PTR] /* previous interrupt content */
                push {r1}                               /* store in stack */
                push {lr}

                ldr r5, =0x40002100             /* GICC */
                ldr r2, [r5, #4]                /* GICC_PMR 0x40002104 */
                ldr r1, =0                      /* error code (set 0) */
                
                /*  r0 = vector number (GIC interrupt begin on 16)
                    r1 = error code
                    r2 = priority GICC_PMR */
                push {r0-r2}

                mrs r1, cpsr                    /* cpsr */
                push {r1}

                str sp, [r3, #KD_INTERRUPT_CONTEXT_PTR] /* previous interrupt content */

                /* check for spurious interrupt */

                /* store vector number */
                stmfd sp!, {r0}

#if MQX_KERNEL_LOGGING
                ldr r1, [r3, #KD_LOG_CONTROL]
                tst r1, #0x00000001
                beq _isr_no_logging

                /* prepare parameters for klog function, r0 = vector idx */
                push {r0-r3}
                blx ASM_PREFIX(_klog_isr_start_internal)
                pop {r0-r3}
ASM_LABEL(_isr_no_logging)
#endif // MQX_KERNEL_LOGGING

                // check vector range
		
                // reserved ?
                ldr r1, =1036   // IAR >= 1020; r0 = IAR + 16
                cmp r0, r1      // IAR >= 1020; r0 = IAR + 16
                bhs _int_kernel_isr_spurious
                
                // GIC interrupt ID to vector number
/*                add r1, r1, #16*/

                // check if isr vector is greater then last user vector
                ldr r1, [r3, #KD_LAST_USER_ISR_VECTOR]
                cmp r0, r1
                bhi _int_kernel_isr_vect_default

                // check if isr vector is lower than first user vector
                ldr r1, [r3, #KD_FIRST_USER_ISR_VECTOR]
                subs r0, r0, r1                         // r0 = actual exception number in table
                blo _int_kernel_isr_vect_default
		
                b _int_kernel_isr_vect_ok

ASM_LABEL(_int_kernel_isr_spurious)
                // spurious ?
                cmp r0, #1040   //  IAR < #1024... IAR >= 1020 & IAR < 1024; r0 = IAR + 16
                blo _int_kernel_isr_return_internal     // yes, spurious interrupt

ASM_LABEL(_int_kernel_isr_vect_default)
                // reserved vector - we will call default ISR
                ldr r1, [r3, #KD_DEFAULT_ISR]           // load address of default ISR
                b _isr_execute
ASM_LABEL(_int_kernel_isr_vect_ok)
                /* calculate offset in table */
                /* each table entry is 12 bytes in size */
                /* r0 = actual exception number in table */
                mov r1, #12
                mul r0, r0, r1

                ldr r1, [r3, #KD_INTERRUPT_TABLE_PTR]       /* pointer to interrupt table begin */
                add r0, r0, r1                              /* get address of entry in table */
                ldr r1, [r0, #0]
                ldr r0, [r0, #IT_APP_ISR_DATA]              /* move notifier data into r0 = first parameter in C func */

ASM_LABEL(_isr_execute)
                /* r0 = first parameter in C func */
                /* r1 contain interrupt function address */

                stmfd sp!, {r3}
                
/*#if MQX_SAVE_FP_ALWAYS && PSP_HAS_FPU
                blx _psp_push_fp_context
#endif*/

                /* Execute the ISR */
                blx r1

/*#if MQX_SAVE_FP_ALWAYS && PSP_HAS_FPU
                blx _psp_pop_fp_context
#endif*/

ASM_LABEL(_int_kernel_isr_epilog)
                ldmfd sp!, {r3}

ASM_LABEL(_int_kernel_isr_return_internal)

                /* load vector number */
                ldmfd sp!, {r0}
ASM_LABEL(_int_kernel_isr_return_internal2)

#if MQX_KERNEL_LOGGING
                ldr r1, [r3, #KD_LOG_CONTROL]
                tst r1, #0x00000001
                beq _isr_return_no_logging

                /* prepare parameters for klog function, r0 = exception number */
                push {r0-r3}
                blx ASM_PREFIX(_klog_isr_end_internal)
                pop {r0-r3}
ASM_LABEL(_isr_return_no_logging)
#endif /* MQX_KERNEL_LOGGING */

                /* remove interrupt content */
                pop {r0-r2, r4-r6}
                str r6, [r3, #KD_INTERRUPT_CONTEXT_PTR] /* update pointer to interrupt content */

                /* r1 = vector number */

#if MQX_SAVE_FP_ALWAYS && PSP_HAS_FPU
                blx _psp_pop_fp_context
#endif

                /* decrement interrupt counter */
                ldrh r2, [r3, #KD_IN_ISR]
                subs r2, r2, #1
                strh r2, [r3, #KD_IN_ISR]

                /* end of interrupt */
                cmp r1, #16
                blo _isr_no_gic_vector
                sub r1, r1, #16
                ldr r0, =0x40002100     /* GICC base address */
                str r1, [r0, #0x10]     /* GICC_EOIR */
                str r4, [r0, #4]        /* GICC_PMR */

ASM_LABEL(_isr_no_gic_vector)

                /* Check the result of IN_ISR.  If it's > 0 at this point, we were
                   more than one level deep.  Don't reschedule in that case. */
                tst r2, #0
                bne _isr_no_preempt

                // Check to see if we need to reschedule.
                ldr r1, [r3, #KD_ACTIVE_PTR]

                // If the task has preemption disabled, don't reschedule no matter what.
                ldr r2, [r1, #TD_FLAGS]
                ands r2, r2, #TASK_PREEMPTION_DISABLED
                bne _isr_set_flag_return_end

                ldr r2, [r3, #KD_CURRENT_READY_Q]
                ldr r2, [r2, #0]
                cmp r1, r2
                bne _isr_sched_start_internal
                
ASM_LABEL(_isr_no_preempt)
                // return from interrupt
                ldmfd sp!, {r0-r12, lr}
                subs pc, lr, #4
                
ASM_LABEL(_isr_set_flag_return_end)
                ldr r2, [r1, #TD_FLAGS]
                orr r2, r2, #SWITCH_REQUEST_WHEN_TASK_PREEMPTION_DISABLED
                str r2, [r1, #TD_FLAGS]
                // return from interrupt
                ldmfd sp!, {r0-r12, lr}
                subs pc, lr, #4


ASM_LABEL(_isr_sched_start_internal)
                /* At this point, we are in IRQ exception mode and we need to
                 * reschedule.  We need to save the sp_irq, lr_irq, and spsr_irq
                 * before changing modes from IRQ to SVC. */
                 
                /* prepare stack in SVC mode for correct task switch */
                /* interrupt stack (IRQ): r0-r12, lr */
                /* task stack (SVC): PMR, CPSR, r0-r12, lr, pc */
                mov r12, sp
                add sp, sp, #56     /* fake unstack - move sp to correct possition (ldmfd sp!, {r0-r12, lr}) */

                mrs r2, spsr
                mov r0, r2
                orr r0, r0, #0xc0	/* Disable interrupts */
                bic r0, r0, #0x20	/* Clean thumb bit (stay in ARM mode) */
                msr cpsr_cxsf, r0
                
                /* svc (supervisor mode) */
                
                /* load PMR to r1 */
                ldr r0, =0x40002100
                ldr r1, [r0, #4]                /* GICC_PMR 0x40002104 */
                
                /* load the lr_irq from interrupt stack and store it in actual stack */
                ldr r0, [r12, #52]      /* get lr_irq from interrupt stack */
                sub r0, r0, #4          /* Subtract 4 from lr_irq because arm loads pc+4 into the lr when entering IRQ */
                str r0, [sp, #-4]!      /* save as new pc for task - after new task switch switch */
                
                str lr, [sp, #-4]       /* save actual lr (lr_svc) to stack */
                
                /* move sp - create task from lowest address */
                sub sp, sp, #64

                stmia sp!, {r1,r2}      /* PMR, PSR */
                
                /* store r0 - r11 */
                ldmfd r12!, {r0-r11}
                stmia sp!, {r0-r11}

                ldmfd r12!, {r0-r1}     /* load registers r12, lr stored in irq stack */
                stmia sp!, {r0}         /* save r12 directly, lr was prepared before */
                
                sub sp, sp, #60         /* correct sp to actual stack position */

                /* Branch to internal scheduler.  This skips the saving of the stack
                 * frame (since we already did our context store) and launches right
                 * into reschedule. */
                b _sched_execute_scheduler_internal_isr
                ASM_PUBLIC_END(_int_kernel_isr)


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _psp_exception_return
* Returned Value   : none
* Comments         : This functions returns us from an isr exception
*
*END*----------------------------------------------------------------------*/
                ASM_PUBLIC_BEGIN(_psp_exception_return)
                ASM_PUBLIC_FUNC(_psp_exception_return)
ASM_LABEL(_psp_exception_return)
                /* r0 - fn param */
                
                mrs r1, CPSR
                and r2, r1, #0x1f
                
                cmp r2, #0x11
                itt eq
                ldreq r2, __fiq_sp
                ldreq sp, [r2]

                cmp r2, #0x17
                itt eq
                ldreq r2, __abort_sp
                ldreq sp, [r2]

                cmp r2, #0x1b
                itt eq
                ldreq r2, __undef_sp
                ldreq sp, [r2]

                cmp r2, #0x17
                it eq
                beq .                   /* TODO */
                
                mrs r2, SPSR
                msr CPSR_cxsf, r2

                /* sub sp, r0, #8 */
                sub r0, r0, #8
                mov sp, r0
                add r0, r0, #8

                b _int_kernel_isr_epilog
                ASM_PUBLIC_END(_psp_exception_return)

ASM_LABEL_CONST32(__fiq_sp, _fiq_sp)
ASM_LABEL_CONST32(__abort_sp, _abort_sp)
ASM_LABEL_CONST32(__undef_sp, _undef_sp)

#if MQX_ROM_VECTORS
#if defined(__CC_ARM)
/* Realview assembler requires a new area for 32b aligment */
                AREA |vector_rom|, CODE, ALIGN=5
#else
                ASM_CODE_SECTION(vector_rom)
#if defined(__IAR_SYSTEMS_ICC__)  || defined (__IASMARM__)
                /* Workaround for IAR 7.30, this section is aligned on Linker file */
#else
                ASM_ALIGN(32)
#endif
#endif

#else
                ASM_CODE_SECTION(vector_ram)
#endif
                /* this section must be aligned to 32B (VBAR) */
ASM_LABEL(_vector_tbl)
                ldr pc, reset_addr
                ldr pc, undefined_addr
                ldr pc, svc_addr
                ldr pc, prefetch_addr
                ldr pc, abort_addr
                nop                 /* reserved */
                ldr pc, irq_addr
                ldr pc, fiq_addr

ASM_LABEL(_vector_addr)
ASM_LABEL_CONST32(reset_addr, reset)
ASM_LABEL_CONST32(undefined_addr, undefined_handler)
ASM_LABEL_CONST32(svc_addr, svc_handler)
ASM_LABEL_CONST32(prefetch_addr, prefetch_handler)
ASM_LABEL_CONST32(abort_addr, abort_handler)
ASM_LABEL_CONST32(resvd_addr, 0)
ASM_LABEL_CONST32(irq_addr, irq_handle)
ASM_LABEL_CONST32(fiq_addr, fiq_handle)

                ASM_END

