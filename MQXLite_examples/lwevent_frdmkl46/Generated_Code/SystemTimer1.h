/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : SystemTimer1.h
**     Project     : lwevent_frdmkl46
**     Processor   : MKL46Z256VMC4
**     Component   : TimerUnit_LDD
**     Version     : Component 01.164, Driver 01.11, CPU db: 3.00.000
**     Compiler    : Keil ARM C/C++ Compiler
**     Date/Time   : 2014-11-04, 14:44, # CodeGen: 4
**     Abstract    :
**          This TimerUnit component provides a low level API for unified hardware access across
**          various timer devices using the Prescaler-Counter-Compare-Capture timer structure.
**     Settings    :
**          Component name                                 : SystemTimer1
**          Module name                                    : SysTick
**          Counter                                        : SYST_CVR
**          Counter direction                              : Down
**          Counter width                                  : 24 bits
**          Value type                                     : Optimal
**          Input clock source                             : Internal
**            Counter frequency                            : Auto select
**          Counter restart                                : On-match
**            Period device                                : SYST_RVR
**            Period                                       : 5 ms
**            Interrupt                                    : Enabled
**              Interrupt                                  : INT_SysTick
**              Interrupt priority                         : medium priority
**          Channel list                                   : 0
**          Initialization                                 : 
**            Enabled in init. code                        : no
**            Auto initialization                          : no
**            Event mask                                   : 
**              OnCounterRestart                           : Enabled
**              OnChannel0                                 : Disabled
**              OnChannel1                                 : Disabled
**              OnChannel2                                 : Disabled
**              OnChannel3                                 : Disabled
**              OnChannel4                                 : Disabled
**              OnChannel5                                 : Disabled
**              OnChannel6                                 : Disabled
**              OnChannel7                                 : Disabled
**          CPU clock/configuration selection              : 
**            Clock configuration 0                        : This component enabled
**            Clock configuration 1                        : This component disabled
**            Clock configuration 2                        : This component disabled
**            Clock configuration 3                        : This component disabled
**            Clock configuration 4                        : This component disabled
**            Clock configuration 5                        : This component disabled
**            Clock configuration 6                        : This component disabled
**            Clock configuration 7                        : This component disabled
**     Contents    :
**         Init              - LDD_TDeviceData* SystemTimer1_Init(LDD_TUserData *UserDataPtr);
**         Deinit            - void SystemTimer1_Deinit(LDD_TDeviceData *DeviceDataPtr);
**         Enable            - LDD_TError SystemTimer1_Enable(LDD_TDeviceData *DeviceDataPtr);
**         Disable           - LDD_TError SystemTimer1_Disable(LDD_TDeviceData *DeviceDataPtr);
**         GetInputFrequency - uint32_t SystemTimer1_GetInputFrequency(LDD_TDeviceData *DeviceDataPtr);
**         GetPeriodTicks    - LDD_TError SystemTimer1_GetPeriodTicks(LDD_TDeviceData *DeviceDataPtr,...
**         ResetCounter      - LDD_TError SystemTimer1_ResetCounter(LDD_TDeviceData *DeviceDataPtr);
**         GetCounterValue   - SystemTimer1_TValueType SystemTimer1_GetCounterValue(LDD_TDeviceData...
**
**     Copyright : 1997 - 2014 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file SystemTimer1.h
** @version 01.11
** @brief
**          This TimerUnit component provides a low level API for unified hardware access across
**          various timer devices using the Prescaler-Counter-Compare-Capture timer structure.
*/         
/*!
**  @addtogroup SystemTimer1_module SystemTimer1 module documentation
**  @{
*/         

#ifndef __SystemTimer1_H
#define __SystemTimer1_H

/* MODULE SystemTimer1. */

/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited beans */

#include "SysTick_PDD.h"
#include "Cpu.h"

#ifdef __cplusplus
extern "C" {
#endif 


#ifndef __BWUserType_SystemTimer1_TValueType
#define __BWUserType_SystemTimer1_TValueType
  typedef uint32_t SystemTimer1_TValueType ; /* Type for data parameters of methods */
#endif
#define SystemTimer1_CNT_INP_FREQ_U_0 0x02DC6C00UL /* Counter input frequency in Hz */
#define SystemTimer1_CNT_INP_FREQ_R_0 48000768.0122882F /* Counter input frequency in Hz */
#define SystemTimer1_CNT_INP_FREQ_COUNT 0U /* Count of predefined counter input frequencies */
#define SystemTimer1_PERIOD_TICKS 0x0003A980UL /* Initialization value of period in 'counter ticks' */
#define SystemTimer1_NUMBER_OF_CHANNELS 0x00U /* Count of predefined channels */
#define SystemTimer1_COUNTER_WIDTH 0x18U /* Counter width in bits  */
#define SystemTimer1_COUNTER_DIR DIR_DOWN /* Direction of counting */
/*! Peripheral base address of a device allocated by the component. This constant can be used directly in PDD macros. */
#define SystemTimer1_PRPH_BASE_ADDRESS  0xE000E010U
  
/* Methods configuration constants - generated for all enabled component's methods */
#define SystemTimer1_Init_METHOD_ENABLED /*!< Init method of the component SystemTimer1 is enabled (generated) */
#define SystemTimer1_Deinit_METHOD_ENABLED /*!< Deinit method of the component SystemTimer1 is enabled (generated) */
#define SystemTimer1_Enable_METHOD_ENABLED /*!< Enable method of the component SystemTimer1 is enabled (generated) */
#define SystemTimer1_Disable_METHOD_ENABLED /*!< Disable method of the component SystemTimer1 is enabled (generated) */
#define SystemTimer1_GetInputFrequency_METHOD_ENABLED /*!< GetInputFrequency method of the component SystemTimer1 is enabled (generated) */
#define SystemTimer1_GetPeriodTicks_METHOD_ENABLED /*!< GetPeriodTicks method of the component SystemTimer1 is enabled (generated) */
#define SystemTimer1_ResetCounter_METHOD_ENABLED /*!< ResetCounter method of the component SystemTimer1 is enabled (generated) */
#define SystemTimer1_GetCounterValue_METHOD_ENABLED /*!< GetCounterValue method of the component SystemTimer1 is enabled (generated) */

/* Events configuration constants - generated for all enabled component's events */
#define SystemTimer1_OnCounterRestart_EVENT_ENABLED /*!< OnCounterRestart event of the component SystemTimer1 is enabled (generated) */



/*
** ===================================================================
**     Method      :  SystemTimer1_Init (component TimerUnit_LDD)
*/
/*!
**     @brief
**         Initializes the device. Allocates memory for the device data
**         structure, allocates interrupt vectors and sets interrupt
**         priority, sets pin routing, sets timing, etc. If the
**         property ["Enable in init. code"] is set to "yes" value then
**         the device is also enabled (see the description of the
**         [Enable] method). In this case the [Enable] method is not
**         necessary and needn't to be generated. This method can be
**         called only once. Before the second call of Init the [Deinit]
**         must be called first.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer will be
**                           passed as an event or callback parameter.
**     @return
**                         - Pointer to the dynamically allocated private
**                           structure or NULL if there was an error.
*/
/* ===================================================================*/
LDD_TDeviceData* SystemTimer1_Init(LDD_TUserData *UserDataPtr);

/*
** ===================================================================
**     Method      :  SystemTimer1_Deinit (component TimerUnit_LDD)
*/
/*!
**     @brief
**         Deinitializes the device. Switches off the device, frees the
**         device data structure memory, interrupts vectors, etc.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by Init method
*/
/* ===================================================================*/
void SystemTimer1_Deinit(LDD_TDeviceData *DeviceDataPtr);

/*
** ===================================================================
**     Method      :  SystemTimer1_Enable (component TimerUnit_LDD)
*/
/*!
**     @brief
**         Enables the component - it starts the signal generation.
**         Events may be generated (see SetEventMask). The method is
**         not available if the counter can't be disabled/enabled by HW.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - The component does not work in
**                           the active clock configuration
*/
/* ===================================================================*/
LDD_TError SystemTimer1_Enable(LDD_TDeviceData *DeviceDataPtr);

/*
** ===================================================================
**     Method      :  SystemTimer1_Disable (component TimerUnit_LDD)
*/
/*!
**     @brief
**         Disables the component - it stops signal generation and
**         events calling. The method is not available if the counter
**         can't be disabled/enabled by HW.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - The component does not work in
**                           the active clock configuration
*/
/* ===================================================================*/
LDD_TError SystemTimer1_Disable(LDD_TDeviceData *DeviceDataPtr);

/*
** ===================================================================
**     Method      :  SystemTimer1_GetInputFrequency (component TimerUnit_LDD)
*/
/*!
**     @brief
**         Returns current input frequency of the counter in Hz as
**         32-bit unsigned integer number. This method can be used only
**         if ["Input clock source"] property is set to "internal".
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @return
**                         - Input frequency
*/
/* ===================================================================*/
uint32_t SystemTimer1_GetInputFrequency(LDD_TDeviceData *DeviceDataPtr);

/*
** ===================================================================
**     Method      :  SystemTimer1_GetPeriodTicks (component TimerUnit_LDD)
*/
/*!
**     @brief
**         Returns the number of counter ticks before re-initialization.
**         See also method [SetPeriodTicks]. This method is available
**         only if the property ["Counter restart"] is switched to
**         'on-match' value.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         TicksPtr        - Pointer to return value of the
**                           number of counter ticks before
**                           re-initialization
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK 
**                           ERR_SPEED - The component does not work in
**                           the active clock configuration
*/
/* ===================================================================*/
LDD_TError SystemTimer1_GetPeriodTicks(LDD_TDeviceData *DeviceDataPtr, SystemTimer1_TValueType *TicksPtr);

/*
** ===================================================================
**     Method      :  SystemTimer1_ResetCounter (component TimerUnit_LDD)
*/
/*!
**     @brief
**         Resets counter. If counter is counting up then it is set to
**         zero. If counter is counting down then counter is updated to
**         the reload value.
**         The method is not available if HW doesn't allow resetting of
**         the counter.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK 
**                           ERR_SPEED - The component does not work in
**                           the active clock configuration
*/
/* ===================================================================*/
LDD_TError SystemTimer1_ResetCounter(LDD_TDeviceData *DeviceDataPtr);

/*
** ===================================================================
**     Method      :  SystemTimer1_GetCounterValue (component TimerUnit_LDD)
*/
/*!
**     @brief
**         Returns the content of counter register. This method can be
**         used both if counter is enabled and if counter is disabled.
**         The method is not available if HW doesn't allow reading of
**         the counter.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @return
**                         - Counter value (number of counted ticks).
*/
/* ===================================================================*/
SystemTimer1_TValueType SystemTimer1_GetCounterValue(LDD_TDeviceData *DeviceDataPtr);

/*
** ===================================================================
**     Method      :  SystemTimer1_Interrupt (component TimerUnit_LDD)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes event(s) of the component.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
/* {MQXLite RTOS Adapter} ISR function prototype */
void SystemTimer1_Interrupt(LDD_RTOS_TISRParameter _isrParameter);

/* END SystemTimer1. */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif
/* ifndef __SystemTimer1_H */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.4 [05.09]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
