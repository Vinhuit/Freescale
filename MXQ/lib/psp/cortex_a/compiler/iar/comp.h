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
*   This file defines the IAR compiler specific macros for MQX
*
*
*END************************************************************************/
#ifndef __comp_h__
#define __comp_h__ 1

#include <intrinsics.h>

#ifdef __cplusplus
extern "C" {
#endif


#if ((__VER__) < 6050000)
#error "MQX requires IAR version 6.5 or higher"
#endif


#define _ASM_NOP()    asm("nop")
#define _ASM_STOP(x)  asm("stop #(" #x ")")
#define _ASM_WFI()    asm("wfi")

#define MRC(coproc, opcode1, Rt, CRn, CRm, opcode2)		{ Rt = __MRC(coproc, opcode1, CRn, CRm, opcode2); }
#define MCR(coproc, opcode1, Rt, CRn, CRm, opcode2)		{ __MCR(coproc, opcode1, Rt, CRn, CRm, opcode2); }
#define DSB()	{ __DSB(); }
#define ISB()	{ __ISB(); }

#define _PSP_SYNC()     _ASM_NOP()

#define _PSP_GET_CPSR(x) (x = __get_CPSR())

// TODO move to GIC implementation
#define _PSP_SET_ENABLE_SR(x)   { GIC_ICCPMR = GIC_ICCPMR_PRIORITY(x); }
#define _PSP_SET_DISABLE_SR(x)  _PSP_SET_ENABLE_SR(x)

#define _WEAK_SYMBOL(x)     __weak x
#define _WEAK_FUNCTION(x)   __weak x

#if MQX_ENABLE_USER_MODE
#define KERNEL_ACCESS  _Pragma("location=\".kernel_data\"")
#define USER_RW_ACCESS _Pragma("location=\".rwuser\"")
#define USER_RO_ACCESS _Pragma("location=\".rouser\"")
#define USER_NO_ACCESS _Pragma("location=\".nouser\"")

#else /* MQX_ENABLE_USER_MODE */

#define KERNEL_ACCESS
#define USER_RW_ACCESS
#define USER_RO_ACCESS
#define USER_NO_ACCESS

#endif /* MQX_ENABLE_USER_MODE */

/* compiler dependent structure packing option */
#define PACKED_STRUCT_BEGIN __packed
#define PACKED_STRUCT_END

/* compiler dependent union packing option */
#define PACKED_UNION_BEGIN __packed
#define PACKED_UNION_END

#ifdef __cplusplus
}
#endif

#endif   /* __comp_h__ */
