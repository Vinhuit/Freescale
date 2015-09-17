/**HEADER*********************************************************************
*
* Copyright (c) 2010 Freescale Semiconductor;
* All Rights Reserved
*
******************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*
* $FileName: comp.h$
* $Version : 3.8.9.0$
* $Date    : Oct-5-2012$
*
* Comments:
*
*   This file defines the RealView compiler specific macros for MQX
*
*END*************************************************************************/
#ifndef __comp_h__
#define __comp_h__   1

/* Disable some warning messages */

/**
 * pointless comparison of unsigned integer with zero ... MQX often compares
 * a variable with a general macros value which is sometimes set to 0
 */
#pragma diag_suppress 186

/**
 * variable was set but never used ... MQX often assigns "kernel_data" variable
 * for subsequent KLOG macros. The warning is issued when KLOG is disabled.
 */
#pragma diag_suppress 550

/**
 * variable was declared but never referenced  ... MQX conditional compilation
 * sometimes causes the declared variable is not used
 */
#pragma diag_suppress 177


#ifdef __cplusplus
extern "C" {
#endif

register int _pc __asm("pc");
register int _lr __asm("lr");
#define __get_PC() _pc
#define __get_LR() _lr

extern void __set_BASEPRI(uint_32);

#define _ASM_NOP()      __nop()
#define _ASM_WFI()      __wfi()
#define _ASM_SLEEP()    __wfi()

#if PSP_MQX_CPU_IS_ARM_CORTEX_M4
#define _PSP_SET_ENABLE_SR(x)   {   \
    VCORTEX_SCB_STRUCT_PTR tmp = (VCORTEX_SCB_STRUCT_PTR)&(((CORTEX_SCS_STRUCT_PTR)CORTEX_PRI_PERIPH_IN_BASE)->SCB);    \
    if ((x & 0xf0) == 0x20) while (1) {};   \
    if (!(tmp->ICSR & (1 << 28))) tmp->SHPR3 = (tmp->SHPR3 & 0xff00ffff) | ((((x) - 0x20) & 0xff) << 16);   \
    if ((x & 0xf0) == 0x10) while (1) {};   \
    __set_BASEPRI(x);\
}
#define _PSP_SET_DISABLE_SR(x)  _PSP_SET_ENABLE_SR(x)
#elif PSP_MQX_CPU_IS_ARM_CORTEX_M0P
#define _PSP_SET_ENABLE_SR(x)   __enable_irq()
#define _PSP_SET_DISABLE_SR(x)  __disable_irq()
#else
#error Invalid platform selected
#endif

#define MRC(coproc, opcode1, Rt, CRn, CRm, opcode2)     __asm{mrc p##coproc, opcode1, Rt, c##CRn, c##CRm, opcode2}
#define MCR(coproc, opcode1, Rt, CRn, CRm, opcode2)     __asm{mcr p##coproc, opcode1, Rt, c##CRn, c##CRm, opcode2}
#define DSB()   __asm{dsb}
#define ISB()   __asm{isb}

#define _PSP_SYNC()

#define _WEAK_SYMBOL(x)     __weak x
#define _WEAK_FUNCTION(x)   __weak x

/* Kinetis User mode definitions */
#if MQX_ENABLE_USER_MODE
#define KERNEL_ACCESS  __attribute__((section(".kernel_data")))
#define USER_RW_ACCESS __attribute__((section(".rwuser")))
#define USER_RO_ACCESS __attribute__((section(".rouser")))
#define USER_NO_ACCESS __attribute__((section(".nouser")))

#else /* MQX_ENABLE_USER_MODE */

#define KERNEL_ACCESS
#define USER_RW_ACCESS
#define USER_RO_ACCESS
#define USER_NO_ACCESS

#endif /* MQX_ENABLE_USER_MODE */

/*
 *      DATATYPE MODIFIERS
 */
/* compiler dependent structure packing option */
#define PACKED_STRUCT_BEGIN __packed
#define PACKED_STRUCT_END

/* compiler dependent union packing option */
#define PACKED_UNION_BEGIN  __packed
#define PACKED_UNION_END

#ifdef __cplusplus
}
#endif

#endif   /* __comp_h__ */
