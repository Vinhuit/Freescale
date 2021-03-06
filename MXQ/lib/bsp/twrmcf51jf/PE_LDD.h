/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : PE_LDD.h.h
**     Project     : ProcessorExpert
**     Processor   : MCF51JF128VLH
**     Version     : Component 01.000, Driver 01.02, CPU db: 3.00.000
**     Compiler    : CodeWarrior MCF C Compiler
**     Date/Time   : 2012-02-07, 12:18, # CodeGen: 7
**     Abstract  : Default PE_LDD.h header file.
**                 MQX BSP provides its own "default" version of PE_LDD.h file
**                 (used as the main include of the PE files).
**                 Once the PE generates the file in <project>/Generated_Code folder,
**                 the PE_LDD.h file should be taken from the new location.
**                 The include search path order will be used to accomplish this behavior.
**
**     Copyright : 1997 - 2011 Freescale Semiconductor, Inc. All Rights Reserved.
**
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/
#ifndef __PE_LDD_H
#define __PE_LDD_H

/* MODULE PE_LDD. */

/* Default version of PE_LDD.h */
#undef PE_LDD_VERSION

#include "PE_Types.h"


/* ============================================================================
   ================== General register manipulating routines ==================
   ============================================================================ */


#define setReg8(Reg, val)                                   ((Reg)  = (uint8_t)(val))
#define getReg8(Reg)                                        (Reg)
#define testReg8Bits(Reg, GetMask)                          ((Reg) & (uint8_t)(GetMask))
#define clrReg8Bits(Reg, ClrMask)                           ((Reg) &= (uint8_t)(~(uint8_t)(ClrMask)))
#define setReg8Bits(Reg, SetMask)                           ((Reg) |= (uint8_t)(SetMask))
#define invertReg8Bits(Reg, InvMask)                        ((Reg) ^= (uint8_t)(InvMask))
#define clrSetReg8Bits(Reg, ClrMask, SetMask)               ((Reg)  = (uint8_t)(((Reg) & (~(uint8_t)(ClrMask))) | (uint8_t)(SetMask)))

#define setReg16(Reg, val)                                  ((Reg)  = (uint16_t)(val))
#define getReg16(Reg)                                       (Reg)
#define testReg16Bits(Reg, GetMask)                         ((Reg) & (uint16_t)(GetMask))
#define clrReg16Bits(Reg, ClrMask)                          ((Reg) &= (uint16_t)(~(uint16_t)(ClrMask)))
#define setReg16Bits(Reg, SetMask)                          ((Reg) |= (uint16_t)(SetMask))
#define invertReg16Bits(Reg, InvMask)                       ((Reg) ^= (uint16_t)(InvMask))
#define clrSetReg16Bits(Reg, ClrMask, SetMask)              ((Reg)  = (uint16_t)(((Reg) & (uint16_t)((~(uint16_t)(ClrMask))) | (uint16_t)(SetMask))))

#define setReg32(Reg, val)                                  ((Reg)  = (uint32_t)(val))
#define getReg32(Reg)                                       (Reg)
#define testReg32Bits(Reg, GetMask)                         ((Reg) & (uint32_t)(GetMask))
#define clrReg32Bits(Reg, ClrMask)                          ((Reg) &= (uint32_t)(~(uint32_t)(ClrMask)))
#define setReg32Bits(Reg, SetMask)                          ((Reg) |= (uint32_t)(SetMask))
#define invertReg32Bits(Reg, InvMask)                       ((Reg) ^= (uint32_t)(InvMask))
#define clrSetReg32Bits(Reg, ClrMask, SetMask)              ((Reg)  = (uint32_t)(((Reg) & (~(uint32_t)(ClrMask))) | (uint32_t)(SetMask)))


/*
** ===================================================================
** Global HAL types and constants
** ===================================================================
*/
typedef uint32_t LDD_TPinMask;         /* Pin mask type. */

typedef uint16_t LDD_TError;           /* Error type. */

typedef uint32_t LDD_TEventMask;       /* Event mask type. */

typedef uint8_t LDD_TClockConfiguration; /* CPU clock configuration type. */

typedef void LDD_TDeviceData;          /* Pointer to private device structure managed and used by HAL components. */
typedef void* LDD_TDeviceDataPtr;      /* Obsolete type for backward compatibility. */

typedef void LDD_TData;                /* General pointer to data. */

typedef void LDD_TUserData;            /* Pointer to this type specifies the user or RTOS specific data will be passed as an event or callback parameter. */

typedef enum {
  DOM_NONE,
  DOM_RUN,
  DOM_WAIT,
  DOM_SLEEP,
  DOM_STOP
} LDD_TDriverOperationMode;            /* Driver operation mode type. */

typedef uint16_t LDD_TDriverState;     /* Driver state type. */

typedef void LDD_TCallbackParam;       /* Pointer to this type specifies the user data to be passed as a callback parameter. */
typedef void (* LDD_TCallback)(LDD_TCallbackParam *CallbackParam); /* Callback type used for definition of callback functions. */


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
** Function prototypes
** ===================================================================
*/

/*
** ===================================================================
**     Method      :  PE_FillMemory (component MCF51JF128_64)
**
**     Description :
**         Fill the memory area by specified value.
** ===================================================================
*/
void PE_FillMemory(register void* SourceAddressPtr, register uint8_t c, register uint32_t len);

/*
** ===================================================================
**     Method      :  PE_PeripheralUsed (component MCF51JF128_64)
**
**     Description :
**         Returns the information whether a peripheral is allocated by
**         PE or not.
** ===================================================================
*/
bool PE_PeripheralUsed(uint32_t PrphBaseAddress);

/*
** ===================================================================
**     Method      :  LDD_SetClockConfiguration (component MCF51JF128_64)
**
**     Description :
**         This method changes the clock configuration of all LDD
**         components in the project.
** ===================================================================
*/
void LDD_SetClockConfiguration(LDD_TClockConfiguration ClockConfiguration);

/* END PE_LDD. */


#endif
/* __PE_LDD_H */
