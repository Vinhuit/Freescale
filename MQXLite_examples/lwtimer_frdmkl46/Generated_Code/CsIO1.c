/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : CsIO1.c
**     Project     : lwtimer_frdmkl46
**     Processor   : MKL46Z256VMC4
**     Component   : ConsoleIO
**     Version     : Component 01.016, Driver 01.00, CPU db: 3.00.000
**     Compiler    : Keil ARM C/C++ Compiler
**     Date/Time   : 2014-11-03, 16:51, # CodeGen: 1
**     Abstract    :
**         This component generates low-level methods for redirecting console I/O to the selected UART.
**         These methods are typically used by printf()/scanf() methods.
**     Settings    :
**          Component name                                 : CsIO1
**          Serial_LDD_Link                                : ConsoleIO_Serial_LDD
**     Contents    :
**         No public methods
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
** @file CsIO1.c
** @version 01.00
** @brief
**         This component generates low-level methods for redirecting console I/O to the selected UART.
**         These methods are typically used by printf()/scanf() methods.
*/         
/*!
**  @addtogroup CsIO1_module CsIO1 module documentation
**  @{
*/         

/* MODULE CsIO1. */

#include <stdio.h>
#include <rt_misc.h>
#include "IO_Map.h"
#include "stdio.h"
#include <stdarg.h>
#include "UART0_PDD.h"
//#pragma import(__use_no_semihosting_swi)

struct __FILE {
  int handle;
};
FILE __stdout;

/*
** ===================================================================
**     Method      :  CsIO1_fputc (component ConsoleIO)
**
**     Description :
**         fputc
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
int fputc(int ch, FILE *f) {
  if ((uint8_t)ch == '\n') {
    /* Wait until UART is ready to write a next character into output buffer */
    while ((UART0_PDD_ReadInterruptStatusReg(UART0_BASE_PTR) & UART0_S1_TDRE_MASK) == 0) {};
    /* Save a character into the transmit buffer of the UART device */
    UART0_PDD_PutChar8(UART0_BASE_PTR, '\r');
  }
  /* Save a character into the transmit buffer of the UART device */
  while ((UART0_PDD_ReadInterruptStatusReg(UART0_BASE_PTR) & UART0_S1_TDRE_MASK) == 0) {};
  /* Wait until UART is ready for saving a next character into the transmit buffer */
  UART0_PDD_PutChar8(UART0_BASE_PTR, (uint8_t)ch);
  return ch;
}

/*
** ===================================================================
**     Method      :  CsIO1_fgetc (component ConsoleIO)
**
**     Description :
**         fgetc
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
int fgetc(FILE *f) {
  while ((UART0_PDD_ReadInterruptStatusReg(UART0_BASE_PTR) & UART0_S1_RDRF_MASK) == 0) {};
  /* Save a character of the UARTx device into the receive buffer */
  return (int)UART0_PDD_GetChar8(UART0_BASE_PTR);
}

/*
** ===================================================================
**     Method      :  CsIO1_ferror (component ConsoleIO)
**
**     Description :
**         ferror
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}

/*
** ===================================================================
**     Method      :  CsIO1__ttywrch (component ConsoleIO)
**
**     Description :
**         _ttywrch
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void _ttywrch(int c) {
  if ((uint8_t)c == '\n') {
    /* Wait until UART is ready to write a next character into output buffer */
    while ((UART0_PDD_ReadInterruptStatusReg(UART0_BASE_PTR) & UART0_S1_TDRE_MASK) == 0) {};
    /* Save a character into the transmit buffer of the UART device */
    UART0_PDD_PutChar8(UART0_BASE_PTR, '\r');
  }
  /* Save a character into the transmit buffer of the UART device */
  while ((UART0_PDD_ReadInterruptStatusReg(UART0_BASE_PTR) & UART0_S1_TDRE_MASK) == 0) {};
  /* Wait until UART is ready for saving a next character into the transmit buffer */
  UART0_PDD_PutChar8(UART0_BASE_PTR, (uint8_t)c);
  return;
}


/* END CsIO1. */

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
