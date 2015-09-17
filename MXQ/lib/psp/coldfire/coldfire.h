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
*   This file contains the type definitions for the COLDFIRE core
*   processor.
*
*
*END************************************************************************/
#ifndef __coldfire_h__
#define __coldfire_h__

#ifdef __cplusplus
extern "C" {
#endif

/*==========================================================================*/
/*
**                   MQX REQUIRED DEFINITIONS
**
** Other MQX kernel and component functions require these definitions to exist.
*/

/* Indicate which endian this PSP is */
#define PSP_ENDIAN      MQX_BIG_ENDIAN

/* PSP_HAS_EMAC macro specifies if processor has Enhanced Multiply-Accumulate Unit (EMAC)*/
#ifndef PSP_HAS_EMAC
#error  PSP_HAS_EMAC macro has to be defined in mqx\source\psp\<proc_family>\<proc_type>.h header file
#endif

/* PSP_HAS_DSP macro specifies if processor has Multiply-Accumulate Unit (MAC)*/
#ifndef PSP_HAS_DSP
#define PSP_HAS_DSP macro has to be defined in mqx\source\psp\<proc_family>\<proc_type>.h header file
#endif

#ifndef PSP_HAS_FPU
   #define PSP_HAS_FPU     0
#endif

#ifndef PSP_HAS_MMU
   #define PSP_HAS_MMU     0
#endif

#ifndef PSP_ACR_CNT
   #define PSP_ACR_CNT     0
#endif

#ifndef PSP_HAS_SUPPORT_STRUCT
    #define PSP_HAS_SUPPORT_STRUCT  PSP_ACR_CNT
#endif    

#ifndef PSP_STOP_ON_IDLE
    #define PSP_STOP_ON_IDLE   1
#endif  

/*
** Memory alignment requirements.
** The alignment indicates how memory is to be aligned for all memory
** accesses.   This is to avoid mis-aligned transfers of data, thus
** optimizing memory accesses.
*/

#ifdef PSP_CACHE_LINE_SIZE
   #define PSP_MEMORY_ALIGNMENT       (PSP_CACHE_LINE_SIZE-1)
#else
   #define PSP_MEMORY_ALIGNMENT       (4-1)
#endif
#define PSP_MEMORY_ALIGNMENT_MASK  (~PSP_MEMORY_ALIGNMENT)

/*
** Stack alignment requirements.
** The alignment indicates how the stack is to be initially aligned.
** This is to avoid mis-aligned types on the stack
*/
#define PSP_STACK_ALIGNMENT       PSP_MEMORY_ALIGNMENT
#define PSP_STACK_ALIGNMENT_MASK  PSP_MEMORY_ALIGNMENT_MASK

/*
** Indicate the direction of the stack
*/
#define PSP_STACK_GROWS_TO_LOWER_MEM         (1)

/*
** Indicate addressing capability of the CPU
** This is the memory width. i.e., the number of bits addressed
** by each memory location.
*/
#define PSP_MEMORY_ADDRESSING_CAPABILITY (8)

/*
** Indicate alignment restrictions on memory accesses
** For an n-bit memory access,
**
** if n <  PSP_MEMORY_ACCESSING_CAPABILITY,
**         n-bit accesses must be n-bit-aligned
**
** if n >= PSP_MEMORY_ACCESSING_CAPABILITY,
**         n-bit accesses must be PSP_MEMORY_ACCESSING_CAPABILITY-bit-aligned
*/
#define PSP_MEMORY_ACCESSING_CAPABILITY (8)

/*
** Cache and MMU definition values
*/
/* See CPU specific header file */

/*
** The maximum number of hardware interrupt vectors
*/
#define PSP_MAXIMUM_INTERRUPT_VECTORS (256)

/*
**   MINIMUM STACK SIZE FOR A TASK
*/

/*
** Memory overhead on users stack before switching over to the interrupt stack.
**
**     6 * (HW interrupt stack frame: SR, PC, Format, reg d0) = 72
**     1 * Bus error stack frame, reg d0 = 28
**     4 more registers before switch to interrupt stack = 16
**
*/
#define PSP_STACK_INTERRUPT_OVERHEAD (116)

/* This much extra stack is required for the logging of mqx functions */
#if MQX_KERNEL_LOGGING
#define PSP_STACK_KLOG_OVERHEAD      (128)
#else
#define PSP_STACK_KLOG_OVERHEAD      (0)
#endif

/* This much extra stack is required if a task destroys itself. */
#if MQX_TASK_DESTRUCTION
#define PSP_TASK_DESTRUCTION_STACK_OVERHEAD (88)
#else
#define PSP_TASK_DESTRUCTION_STACK_OVERHEAD (0)
#endif

/* Minimum stack size for all tasks. */
#define PSP_MINSTACKSIZE \
   (sizeof(PSP_STACK_START_STRUCT) + \
   PSP_TASK_DESTRUCTION_STACK_OVERHEAD + \
   PSP_STACK_INTERRUPT_OVERHEAD + \
   PSP_STACK_KLOG_OVERHEAD)

/* Minimum stack size for the Idle Task
**   Idle task written in C uses at most 8 bytes of stack:
**     1 Link Instruction, 1 local variable (void *)
*/
#define PSP_IDLE_STACK_SIZE (PSP_MINSTACKSIZE + 8)

/* Init task stack size
*/
#define PSP_INIT_STACK_SIZE                         (PSP_MINSTACKSIZE * 10)

/* COLDFIRE addresses do not need to be normalized
** (ie as for example the Intel chips in real mode do require this)
*/
#define _PSP_NORMALIZE_MEMORY(x) (x)

/*
** Standard cache macros
*/
/* See CPU specific header file */

/*==========================================================================*/
/*
**                    PSP SPECIFIC DEFINITIONS
**
** These definitions will change from PSP to PSP
*/

/* Processor family */
#define PSP_COLDFIRE 1

/*
** Exception vector numbers
*/
#define PSP_EXCPT_ACCESS_ERROR    (2)
#define PSP_EXCPT_ADDRESS_ERROR   (3)
#define PSP_EXCPT_ILLEGAL_INSTR   (4)
#define PSP_EXCPT_PRIVILEGE_VIOL  (8)
#define PSP_EXCPT_TRACE           (9)
#define PSP_EXCPT_1010_EMUL       (10)
#define PSP_EXCPT_1111_EMUL       (11)
#define PSP_EXCPT_DEBUG           (12)
#define PSP_EXCPT_FORMAT_ERROR    (14)
#define PSP_EXCPT_UNINIT_INT_VEC  (15)
#define PSP_EXCPT_SPURIOUS_INT    (24)
#define PSP_EXCPT_LEVEL1_AUTO     (25)
#define PSP_EXCPT_LEVEL2_AUTO     (26)
#define PSP_EXCPT_LEVEL3_AUTO     (27)
#define PSP_EXCPT_LEVEL4_AUTO     (28)
#define PSP_EXCPT_LEVEL5_AUTO     (29)
#define PSP_EXCPT_LEVEL6_AUTO     (30)
#define PSP_EXCPT_LEVEL7_AUTO     (31)
#define PSP_EXCPT_TRAP0           (32)
#define PSP_EXCPT_TRAP1           (33)
#define PSP_EXCPT_TRAP2           (34)
#define PSP_EXCPT_TRAP3           (35)
#define PSP_EXCPT_TRAP4           (36)
#define PSP_EXCPT_TRAP5           (37)
#define PSP_EXCPT_TRAP6           (38)
#define PSP_EXCPT_TRAP7           (39)
#define PSP_EXCPT_TRAP8           (40)
#define PSP_EXCPT_TRAP9           (41)
#define PSP_EXCPT_TRAP10          (42)
#define PSP_EXCPT_TRAP11          (43)
#define PSP_EXCPT_TRAP12          (44)
#define PSP_EXCPT_TRAP13          (45)
#define PSP_EXCPT_TRAP14          (46)
#define PSP_EXCPT_TRAP15          (47)


/*
** Bits found in the Status Register
*/
#define PSP_SR_TRACE_ALL          (0x8000)
#define PSP_SR_TRACE_FLOW         (0x4000)
#define PSP_SR_SUPERVISORY_STATE  (0x2000)
#define PSP_SR_INTERRUPT_STATE    (0x1000)
#define PSP_SR_PRIORITY_MASK      (0x0700)
#define PSP_SR_EXTEND             (0x0010)
#define PSP_SR_NEGATIVE           (0x0008)
#define PSP_SR_ZERO               (0x0004)
#define PSP_SR_OVERFLOW           (0x0002)
#define PSP_SR_CARRY              (0x0001)

/*
** Function code values
*/
#define PSP_FC_USER_DATA_SPACE     (0x01)
#define PSP_FC_USER_PROGRAM_SPACE  (0x02)
#define PSP_FC_SUPER_DATA_SPACE    (0x05)
#define PSP_FC_SUPER_PROGRAM_SPACE (0x06)
#define PSP_FC_CPU_SPACE           (0x07)

/*
** Defined Format values for the CPU32 processor found in the format
** and vector word of the stack frame.
** NOTE: only the top nibble of the format and vector is the format.
*/
#define PSP_NORMAL_STACK_FRAME                      (0x4000)

/* Obtain the format bits from the format and vector (FAV) word */
#define PSP_FAV_GET_FORMAT(x)                       ((x) & 0xF000)

/* Obtain the vector bits from the format and vector word */
#define PSP_FAV_GET_VECTOR_OFFSET(x) ((x) & 0x3FC)
#define PSP_FAV_GET_VECTOR_NUMBER(x) (PSP_FAV_GET_VECTOR_OFFSET(x)>>2)

#ifndef __ASM__

#if PSP_HAS_FPU
/*-----------------------------------------------------------------------*/
/*
** PSP BLOCKED FP STRUCT
**
** This structure defines the registers stored by MQX when a floating
** point task is blocked.  When the FLOATING_POINT_REGISTERS_STORED bit
** is set task's FLAGS, then this structure is valid.
*/
typedef struct psp_blocked_fp_struct
{
   uint32_t  FPCR;       // Floating point control register
   uint32_t  FPSR;       // Floating point status register
   uint32_t  FPIAR;      // Floating point instruction address register
   uint32_t  TID;        // Testing for correctness
   double   FPR[8];     // The floating point computation registers
} PSP_BLOCKED_FP_STRUCT, * PSP_BLOCKED_FP_STRUCT_PTR;

#endif

/*==========================================================================*/
/*                     PSP SPECIFIC DATA TYPES                              */

/*-----------------------------------------------------------------------*/
/*
** PSP NORMAL STACK FRAME STRUCT
**
** The stack frame pushed onto the stack for a normal stack frame
**
** The format in the FORMAT_AND_VECTOR indicates a
**   NORMAL_STACK_FRAME: 0x0---
*/
typedef struct psp_normal_stack_frame_struct
{
   uint16_t              FORMAT_AND_VECTOR;
   uint16_t              STATUS_REGISTER;
   void     (_CODE_PTR_ RETURN_ADDRESS)(void);
} PSP_NORMAL_STACK_FRAME_STRUCT, * PSP_NORMAL_STACK_FRAME_STRUCT_PTR;

/*-----------------------------------------------------------------------*/
/*
** PSP FUNCTION CALL STRUCT
**
** This structure defines what a function call pushes onto the stack
*/
typedef struct psp_function_call_struct
{

   /* Pushed onto the stack by the LINK instrution when a function is entered*/
   void                *A6_REGISTER;

   /* Pushed onto the stack by the JSR instruction */
   void     (_CODE_PTR_ RETURN_ADDRESS)(void);

} PSP_FUNCTION_CALL_STRUCT, * PSP_FUNCTION_CALL_STRUCT_PTR;

/*==========================================================================*/
/*                         MQX DATA TYPES                                   */


#if PSP_HAS_DSP
/*--------------------------------------------------------------------------*/
/*
** TD EXTENSION STRUCT
**
** Extra context information for a task, currently kept on the stack of
** a task.
*/
typedef struct td_extension_struct
{

// TODO add EMAC define - reduce regs count for DSP only
   /* DSP registers are saved here */
   uint32_t                  ACC0;
   uint32_t                  ACC1;
   uint32_t                  ACC2;
   uint32_t                  ACC3;
   uint32_t                  ACCEXT10;
   uint32_t                  ACCEXT23;
   uint32_t                  MACSR;
   uint32_t                  MASK;

} TD_EXTEND_STRUCT, * TD_EXTEND_STRUCT_PTR;
#endif


/*-----------------------------------------------------------------------*/
/*
** PSP STORED REGISTERS STRUCT
**
** This structure defines the registers stored on the stack by MQX
** for a task that is not running.
*/
typedef struct psp_stored_registers_struct
{
   uint32_t D0;
   uint32_t D1;
   uint32_t D2;
   uint32_t D3;
   uint32_t D4;
   uint32_t D5;
   uint32_t D6;
   uint32_t D7;
   void   *A0;
   void   *A1;
   void   *A2;
   void   *A3;
   void   *A4;
   void   *A5;
   void   *A6;
} PSP_STORED_REGISTERS_STRUCT, * PSP_STORED_REGISTERS_STRUCT_PTR;


/*-----------------------------------------------------------------------*/
/*
** PSP BLOCKED STACK STRUCT
**
** This is what a stack looks like for a task that is NOT the active
** task
*/
typedef struct psp_blocked_stack_struct
{

   /* the registers of the task */
   PSP_STORED_REGISTERS_STRUCT   REGISTERS;

   /* An interrupt frame created by software so that the task can be
   ** activated using a return from interrupt
   */
   PSP_NORMAL_STACK_FRAME_STRUCT FRAME;

} PSP_BLOCKED_STACK_STRUCT, * PSP_BLOCKED_STACK_STRUCT_PTR;


/*-----------------------------------------------------------------------*/
/*
** PSP INTERRUPT CONTEXT STRUCT
**
** This structure provides a "context" for mqx primitives to use while executing
** in an interrupt handler.  A link list of these contexts is built on the
** interrupt stack.  The head of this list (the current interrupt context) is
** pointed to by the INTERRUPT_CONTEXT_PTR field of the kernel data structure.
**
*/
typedef struct psp_int_context_struct
{

   /* The "task" error code for use by mqx functions while in the ISR */
   uint32_t ERROR_CODE;

   /* Used by the _int_enable function while in the ISR */
   uint16_t ENABLE_SR;

   /* The interrupt vector number for this context */
   uint16_t EXCEPTION_NUMBER;

   /* Address of previous context, NULL if none */
   struct psp_int_context_struct      *PREV_CONTEXT;

} PSP_INT_CONTEXT_STRUCT, * PSP_INT_CONTEXT_STRUCT_PTR;


/*-----------------------------------------------------------------------*/
/*
** PSP BASIC INT FRAME STRUCT
**
** This structure is what is pushed onto the memory on the current stack
** when an interrupt occurs.
**
** For the first interrupt that interrupts a running task, this structure
** defines what the top of the stack for the task looks like,
** and the STACK_PTR field of the task descriptor will point to this structure.
**
** The rest of the interrupt frame is then built upon the interrupt stack.
**
** For a nested interrupt, this basic frame is also built upon the interrupt
** stack.
**
*/

typedef struct psp_basic_int_frame_struct
{

   /* The registers pushed by _int_kernel_isr
   ** Not all are stored to
   */
   PSP_STORED_REGISTERS_STRUCT   REGISTERS;

   /* The stack frame pushed by the CPU */
   PSP_NORMAL_STACK_FRAME_STRUCT FRAME;

} PSP_BASIC_INT_FRAME_STRUCT, * PSP_BASIC_INT_FRAME_STRUCT_PTR;


/*-----------------------------------------------------------------------*/
/*
** PSP INTERRUPT FRAME STRUCT
**
** This structure defines the stack as it appears presented to an interrupt
** service routine.  Note that this frame is on the interrupt stack.
**
** However for the very first interrupt (IN_ISR == 1)
** part of this structure (BASIC_INT_FRAME_STRUCT) is found on the
** stack of the interrupted task.
**
*/

typedef struct  psp_interrupt_frame_struct
{

   /* the isr data, passed as a parameter to the ISR */
   void                            *ISR_DATA;

   /* Interrupt context information for the current ISR, used by MQX
   ** primitives.
   */
   PSP_INT_CONTEXT_STRUCT           INTERRUPT_CONTEXT;

   /* The stack frame pushed by the CPU at the start of _int_kernel_isr */
   PSP_BASIC_INT_FRAME_STRUCT       BASIC_FRAME;

} PSP_INTERRUPT_FRAME_STRUCT, * PSP_INTERRUPT_FRAME_STRUCT_PTR;


/*-----------------------------------------------------------------------*/
/*
** PSP STACK START STRUCT
**
** This structure is used during the initialization of a new task.
** It is overlaid on the bottom of the task's stack
**
*/
typedef struct psp_stack_start_struct
{

   /* The start up registers for the task */
   PSP_BLOCKED_STACK_STRUCT     INITIAL_CONTEXT;

   /* The function to call when the task "returns" */
   void             (_CODE_PTR_ EXIT_ADDRESS)();

   /* The task's parameter */
   uint32_t                      PARAMETER;

   /* The following two fields are used to create a "bottom" of stack
   ** that debuggers will recognize
   */
   /* base of frame pointer , value 0, the starting A6 register points here */
   uint32_t                      ZERO_LINK_ADDRESS;

   /* close the stack frame with a return address of 0 */
   uint32_t                      ZERO_RETURN_ADDRESS;

#if PSP_HAS_DSP
   /* Extra task context information kept here, rather than in the
   ** task descriptor
   */
   TD_EXTEND_STRUCT             TD_EXTENSION;
#endif

} PSP_STACK_START_STRUCT, * PSP_STACK_START_STRUCT_PTR;


/*--------------------------------------------------------------------------*/
/*
**                  EXTERNS FOR PSP GLOBAL VARIABLES
*/

/*--------------------------------------------------------------------------*/
/*
**                  PROTOTYPES OF PSP FUNCTIONS
*/

extern volatile uint32_t _psp_saved_mbar;
extern volatile uint32_t _psp_saved_vbr;
extern volatile uint32_t _psp_saved_cacr;

#if PSP_ACR_CNT
extern volatile uint32_t _psp_saved_acr0;
extern volatile uint32_t _psp_saved_acr1;
#endif // PSP_ACR_CNT

#if PSP_ACR_CNT == 4
extern volatile uint32_t _psp_saved_acr2;
extern volatile uint32_t _psp_saved_acr3;
#endif // PSP_ACR_CNT == 4

/* Generic PSP prototypes */
extern _mqx_uint  _psp_int_init(_mqx_uint, _mqx_uint);
extern void       _psp_int_install(void);


/* PSP Specific prototypes */
extern uint32_t   _psp_get_sr(void);
extern uint32_t   _psp_set_sr(uint32_t);

#if PSP_ACR_CNT
extern uint32_t   _psp_get_acr(uint32_t);
extern uint32_t   _psp_set_acr(uint32_t,uint32_t);
#endif

extern void      _psp_set_cacr(uint32_t);
extern void      _psp_set_mbar(uint32_t);

extern uint32_t   _psp_load_long_swapped(void *);
extern void      _psp_store_long_swapped(void *, uint32_t);

// cache
extern void      _dcache_flush_mlines(void *, uint32_t, uint32_t);
extern void      _icache_invalidate_mlines(void *, uint32_t, uint32_t);

#endif // __ASM__

#ifdef __cplusplus
}
#endif


#endif
/* EOF */
