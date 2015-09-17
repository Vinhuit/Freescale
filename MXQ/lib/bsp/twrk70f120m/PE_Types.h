/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : PE_Types.h
**     Project     : ProcessorExpert
**     Component   : PE_Types
**     Version     : Driver 01.01
**     Abstract    :
**         PE_Types.h - contains definitions of basic types,
**         register access macros and hardware specific macros
**         which can be used in user application.
**     Settings    :
**     Contents    :
**         No public methods
**
**     Copyright : 1997 - 2013 Freescale Semiconductor, Inc. All Rights Reserved.
**     SOURCE DISTRIBUTION PERMISSIBLE as directed in End User License Agreement.
**
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/
/*!
** @file PE_Types.h
** @version 01.01
** @brief
**         PE_Types.h - contains definitions of basic types,
**         register access macros and hardware specific macros
**         which can be used in user application.
*/
/*!
**  @addtogroup PE_Types_module PE_Types module documentation
**  @{
*/

#ifndef __PE_Types_H
#define __PE_Types_H

/* Standard ANSI C types */
#include <stdint.h>

#ifndef FALSE
  #define  FALSE  0x00u                /* Boolean value FALSE. FALSE is defined always as a zero value. */
#endif
#ifndef TRUE
  #define  TRUE   0x01u                /* Boolean value TRUE. TRUE is defined always as a non zero value. */
#endif

#ifndef NULL
  #define  NULL   0x00u
#endif

/* PE types definition */
#ifndef __cplusplus
#ifndef bool
typedef unsigned char           bool;
#endif
#endif
typedef unsigned char           byte;
/* typedef unsigned short          word;*/ /* To avod compilation conflict with CyaSSL */
typedef unsigned long           dword;
typedef unsigned long long      dlong;
typedef unsigned char           TPE_ErrCode;
#ifndef TPE_Float
typedef float                   TPE_Float;
#endif
#ifndef char_t
typedef char                    char_t;
#endif

/* Other basic data types */
typedef signed char             int8;
typedef signed short int        int16;
typedef signed long int         int32;

typedef unsigned char           uint8;
typedef unsigned short int      uint16;
typedef unsigned long int       uint32;


/**********************************************************/
/* Uniform multiplatform 8-bits peripheral access macros */
/**********************************************************/
#ifdef __CC_ARM

/* Enable maskable interrupts */
#define __EI()\
 do {\
    __enable_fiq();\
 } while(0)

/* Disable maskable interrupts */
#define __DI() \
 do {\
     __disable_fiq();\
 } while(0)


/* Save status register and disable interrupts */
#define EnterCritical() \
 do {\
   if (++SR_lock == 1u) {\
                Cpu_EnterCritical(&SR_reg);\
   }\
 } while(0)

/* Restore status register  */
#define ExitCritical() \
 do {\
   if (--SR_lock == 0u) { \
     Cpu_ExitCritical(&SR_reg);\
   }\
 } while(0)


#define PE_DEBUGHALT() \
  __breakpoint(255);

#define PE_NOP() \
  __nop();

#define PE_WFI() \
  __wfi();

#else
/* Enable maskable interrupts */
#define __EI()\
 do {\
  /*lint -save  -e950 Disable MISRA rule (1.1) checking. */\
     asm("CPSIE f");\
  /*lint -restore Enable MISRA rule (1.1) checking. */\
 } while(0)

/* Disable maskable interrupts */
#define __DI() \
 do {\
  /*lint -save  -e950 Disable MISRA rule (1.1) checking. */\
     asm ("CPSID f");\
  /*lint -restore Enable MISRA rule (1.1) checking. */\
 } while(0)


/* Save status register and disable interrupts */
#define EnterCritical() \
 do {\
   if (++SR_lock == 1u) {\
  /*lint -save  -e586 -e950 Disable MISRA rule (2.1,1.1) checking. */\
     asm ( \
     "MRS R0, FAULTMASK\n\t" \
     "CPSID f\n\t"            \
     "STRB R0, %[output]"  \
     : [output] "=m" (SR_reg)\
     :: "r0");\
  /*lint -restore Enable MISRA rule (2.1,1.1) checking. */\
   }\
 } while(0)

/* Restore status register  */
#define ExitCritical() \
 do {\
   if (--SR_lock == 0u) { \
  /*lint -save  -e586 -e950 Disable MISRA rule (2.1,1.1) checking. */\
     asm (                 \
       "ldrb r0, %[input]\n\t"\
       "msr FAULTMASK,r0;\n\t" \
       ::[input] "m" (SR_reg)  \
       : "r0");                \
  /*lint -restore Enable MISRA rule (2.1,1.1) checking. */\
   }\
 } while(0)


#define PE_DEBUGHALT() \
  /*lint -save  -e586 -e950 Disable MISRA rule (2.1,1.1) checking. */\
  asm( "BKPT 255") \
  /*lint -restore Enable MISRA rule (2.1,1.1) checking. */

#define PE_NOP() \
  /*lint -save  -e586 -e950 Disable MISRA rule (2.1,1.1) checking. */\
  asm( "NOP") \
  /*lint -restore Enable MISRA rule (2.1,1.1) checking. */

#define PE_WFI() \
  /*lint -save  -e586 -e950 Disable MISRA rule (2.1,1.1) checking. */\
  asm("WFI") \
  /*lint -restore Enable MISRA rule (2.1,1.1) checking. */

#endif

/*
** ===================================================================
** Global HAL types and constants
** ===================================================================
*/
typedef uint32_t LDD_TPinMask;         /*!< Pin mask type. */
typedef uint16_t LDD_TError;           /*!< Error type. */
typedef uint32_t LDD_TEventMask;       /*!< Event mask type. */
typedef uint8_t LDD_TClockConfiguration; /*!< CPU clock configuration type. */
typedef void LDD_TDeviceData;          /*!< Pointer to private device structure managed and used by HAL components. */
typedef void* LDD_TDeviceDataPtr;      /*!< Obsolete type for backward compatibility. */
typedef void LDD_TData;                /*!< General pointer to data. */
typedef void LDD_TUserData;            /*!< Pointer to this type specifies the user or RTOS specific data will be passed as an event or callback parameter. */

/*! Driver operation mode type. */
typedef enum {
  DOM_NONE,
  DOM_RUN,
  DOM_WAIT,
  DOM_SLEEP,
  DOM_STOP
} LDD_TDriverOperationMode;

typedef uint16_t LDD_TDriverState;     /*!< Driver state type. */
typedef void LDD_TCallbackParam;       /*!< Pointer to this type specifies the user data to be passed as a callback parameter. */
typedef void (* LDD_TCallback)(LDD_TCallbackParam *CallbackParam); /*!< Callback type used for definition of callback functions. */


/* Fills a memory area block by a specified value. Function defined in PE_LDD.c */
extern void PE_FillMemory(register void* SourceAddressPtr, register uint8_t c, register uint32_t len);


/*
** ===================================================================
** RTOS specific types and constants
** ===================================================================
*/
/* {MQX RTOS Adapter} Type of the parameter passed into ISR from RTOS interrupt dispatcher */
typedef void *LDD_RTOS_TISRParameter;

/* {MQX RTOS Adapter} Structure for saving/restoring interrupt vector */
typedef struct {
  void (*isrFunction)(LDD_RTOS_TISRParameter); /* ISR function handler */
  LDD_RTOS_TISRParameter isrData;      /* ISR parameter */
} LDD_RTOS_TISRVectorSettings;


/*
** ===================================================================
** Published RTOS settings and constants
** ===================================================================
*/


/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.08]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
#endif /* __PE_Types_H */

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.08]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
