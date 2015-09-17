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
*   This file contains the type definitions for the mcf52xx QSPI module.
*
*
*END************************************************************************/

#ifndef __mcf54xx_uart_h__
#define __mcf54xx_uart_h__

#define __mcf54xx_uart_h__version "$Version:3.8.7.0$"
#define __mcf54xx_uart_h__date    "$Date:Jun-6-2012$"

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------*/
/*
** UART registers
*/
#define MCF54XX_UART_UMR1_RXRTS             (0x80)
#define MCF54XX_UART_UMR1_RXIRQ             (0x40)
#define MCF54XX_UART_UMR1_ERR               (0x20)
#define MCF54XX_UART_UMR1_EVEN_PARITY       (0x00)
#define MCF54XX_UART_UMR1_ODD_PARITY        (0x04)
#define MCF54XX_UART_UMR1_LOW_PARITY        (0x08)
#define MCF54XX_UART_UMR1_HI_PARITY         (0x0C)
#define MCF54XX_UART_UMR1_NO_PARITY         (0x10)
#define MCF54XX_UART_UMR1_MULTI_DROP        (0x18)
#define MCF54XX_UART_UMR1_MD_ADDRESS        (0x1C)
#define MCF54XX_UART_UMR1_5_BITS            (0x00)
#define MCF54XX_UART_UMR1_6_BITS            (0x01)
#define MCF54XX_UART_UMR1_7_BITS            (0x02)
#define MCF54XX_UART_UMR1_8_BITS            (0x03)
#define MCF54XX_UART_UMR1_MASK_DATA_BITS    (0x03)

#define MCF54XX_UART_UMR2_AUTO_ECHO         (0x40)
#define MCF54XX_UART_UMR2_LOCAL_LOOP        (0x80)
#define MCF54XX_UART_UMR2_RMT_LOOP          (0xC0)
#define MCF54XX_UART_UMR2_TXRTS             (0x20)
#define MCF54XX_UART_UMR2_TXCTS             (0x10)
#define MCF54XX_UART_UMR2_1_STOP_BIT        (0x07)
#define MCF54XX_UART_UMR2_2_STOP_BITS       (0x0F)
#define MCF54XX_UART_UMR2_MASK_STOP_BITS    (0x0F)

#define MCF54XX_UART_USR_RB                 (0x80)
#define MCF54XX_UART_USR_FE                 (0x40)
#define MCF54XX_UART_USR_PE                 (0x20)
#define MCF54XX_UART_USR_OE                 (0x10)
#define MCF54XX_UART_USR_TXEMP              (0x08)
#define MCF54XX_UART_USR_TXRDY              (0x04)
#define MCF54XX_UART_USR_FFULL              (0x02)
#define MCF54XX_UART_USR_RXRDY              (0x01)

#define MCF54XX_UART_UCSR_RX_TIMER          (0xD0)
#define MCF54XX_UART_UCSR_RXCLK16           (0xE0)
#define MCF54XX_UART_UCSR_RXCLK1            (0xF0)
#define MCF54XX_UART_UCSR_TX_TIMER          (0x0D)
#define MCF54XX_UART_UCSR_TXCLK16           (0x0E)
#define MCF54XX_UART_UCSR_TXCLK1            (0x0F)

#define MCF54XX_UART_UCR_RESET_POINTER      (0x10)
#define MCF54XX_UART_UCR_RESET_RX           (0x20)
#define MCF54XX_UART_UCR_RESET_TX           (0x30)
#define MCF54XX_UART_UCR_RESET_ERROR        (0x40)
#define MCF54XX_UART_UCR_RESET_BREAK        (0x50)
#define MCF54XX_UART_UCR_START_BREAK        (0x60)
#define MCF54XX_UART_UCR_STOP_BREAK         (0x70)
#define MCF54XX_UART_UCR_AUTOBAUD           (0x80)
#define MCF54XX_UART_UCR_TX_ENABLE          (0x04)
#define MCF54XX_UART_UCR_TX_DISABLE         (0x08)
#define MCF54XX_UART_UCR_RX_ENABLE          (0x01)
#define MCF54XX_UART_UCR_RX_DISABLE         (0x02)

#define MCF54XX_UART_UIPCR_CTS_CHANGE       (0x10)
#define MCF54XX_UART_UIPCR_CTS              (0x01)

#define MCF54XX_UART_UACR_INPUT_CNTRL       (0x01)

#define MCF54XX_UART_UISR_CHANGE_STATE      (0x80)
#define MCF54XX_UART_UISR_CHANGE_BRK        (0x04)
#define MCF54XX_UART_UISR_RXRDY             (0x02)
#define MCF54XX_UART_UISR_TXRDY             (0x01)

#define MCF54XX_UART_UIMR_CHANGE_STATE      (0x80)
#define MCF54XX_UART_UIMR_CHANGE_BRK        (0x04)
#define MCF54XX_UART_UIMR_FFULL             (0x02)
#define MCF54XX_UART_UIMR_TXRDY             (0x01)

#define MCF54XX_UART_UIP_CTS                (0x01)

#define MCF54XX_UART_UOP_RTS                (0x01)

/*------------------------------------------------------------------------*/
/*

/*
** MCF54XX_UART_STRUCT
** This structure defines what one uart register look like
*/
typedef union MCF54XX_uart_struct
{
   struct  {
      unsigned char UMR;             // UART mode register
      unsigned char filler1[3];
      unsigned char USR;             // UART status register
      unsigned char filler2[3];
      unsigned char filler4a;             
      unsigned char filler4[3];
      unsigned char URB;             // (Read) UART Receive Buffer 
      unsigned char filler5[3];
      unsigned char UIPCR;           // (Read) UART Input Port Change Register 
      unsigned char filler6[3];
      unsigned char UISR;            // (Read) UART Interrupt Status Register 
      unsigned char filler7[3];
      unsigned char filler9a;            
      unsigned char filler9[3];
      unsigned char filler10a;            
      unsigned char filler10[3];
      unsigned char filler11[0x0234 - 0x021C - 3 - 1];
      unsigned char UIP;             // (Read) UART Input Port Register
      unsigned char filler12[3];
      unsigned char filler13a;            
      unsigned char filler13[3];
      unsigned char filler14a;     // (Write) UART Output Port Bit Reset Command Register
      unsigned char filler14[3];
   } READ;
   struct  {
      unsigned char UMR;             // UART mode register
      unsigned char filler1[3];
      unsigned char UCSR;             // UART status register
      unsigned char filler2[3];
      unsigned char UCR;             // (Write) UART Command Register
      unsigned char filler4[3];
      unsigned char UTB;             //  (Write) UART Transmit Buffer
      unsigned char filler5[3];
      unsigned char UACR;            //  (Write) UART Auxiliary Control Register
      unsigned char filler6[3];
      unsigned char UIMR;            // (Write) UART Interrupt Mask Register
      unsigned char filler7[3];
      unsigned char UBG1;            // (Write) UART Baud Rate Generator Register 1
      unsigned char filler9[3];
      unsigned char UBG2;            // (Write) UART Baud Rate Generator Register 2
      unsigned char filler10[3];
      unsigned char filler11[0x0234 - 0x021C - 3 - 1];
      unsigned char filler12a;             
      unsigned char filler12[3];
      unsigned char UOP1;            // (Write) UART Output Port Bit Set Command Register 0
      unsigned char filler13[3];
      unsigned char UOP0;             // (Write) UART Output Port Bit Reset Command Register
   } WRITE;
   
   uint8_t filler[0x4000];
} MCF54XX_UART_STRUCT, * MCF54XX_UART_STRUCT_PTR;

#ifdef __cplusplus
}
#endif

#endif  //__MCF54XX_uart_h__
