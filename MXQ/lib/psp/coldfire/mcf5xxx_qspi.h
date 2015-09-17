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
*   This file contains the type definitions for the mcf5xxx QSPI module.
*
*
*END************************************************************************/

#ifndef __mcf5xxx_qspi_h__
#define __mcf5xxx_qspi_h__

#define __mcf5xxx_qspi_h__version "$Version:3.7.5.0$"
#define __mcf5xxx_qspi_h__date    "$Date:Feb-7-2011$"

#ifdef __cplusplus
extern "C" {
#endif


/*------------------------------------------------------------------------*/
/*
** QSPI  registers bit set.
*/

/* Bit definitions and macros for MCF5XXX_QSPI_QMR */
#define MCF5XXX_QSPI_QMR_BAUD(x)                 (((x)&0xFF)<<0)
#define MCF5XXX_QSPI_QMR_CPHA                    (0x100)
#define MCF5XXX_QSPI_QMR_CPOL                    (0x200)
#define MCF5XXX_QSPI_QMR_BITS_GET(reg)           (((reg)>>0xA)&0xF)
#define MCF5XXX_QSPI_QMR_BITS_SET(val)           (((val)&0xF)<<0xA)
#define MCF5XXX_QSPI_QMR_DOHIE                   (0x4000)
#define MCF5XXX_QSPI_QMR_MSTR                    (0x8000)

/* Bit definitions and macros for MCF5XXX_QSPI_QDLYR */
#define MCF5XXX_QSPI_QDLYR_DTL(x)                (((x)&0xFF)<<0)
#define MCF5XXX_QSPI_QDLYR_QCD(x)                (((x)&0x7F)<<0x8)
#define MCF5XXX_QSPI_QDLYR_SPE                   (0x8000)

/* Bit definitions and macros for MCF5XXX_QSPI_QWR */
#define MCF5XXX_QSPI_QWR_NEWQP(x)                (((x)&0xF)<<0)
#define MCF5XXX_QSPI_QWR_CPTQP(x)                (((x)&0xF)<<0x4)
#define MCF5XXX_QSPI_QWR_ENDQP(x)                (((x)&0xF)<<0x8)
#define MCF5XXX_QSPI_QWR_CSIV                    (0x1000)
#define MCF5XXX_QSPI_QWR_WRTO                    (0x2000)
#define MCF5XXX_QSPI_QWR_WREN                    (0x4000)
#define MCF5XXX_QSPI_QWR_HALT                    (0x8000)

/* Bit definitions and macros for MCF5XXX_QSPI_QIR */
#define MCF5XXX_QSPI_QIR_SPIF                    (0x1)
#define MCF5XXX_QSPI_QIR_ABRT                    (0x4)
#define MCF5XXX_QSPI_QIR_WCEF                    (0x8)
#define MCF5XXX_QSPI_QIR_SPIFE                   (0x100)
#define MCF5XXX_QSPI_QIR_ABRTE                   (0x400)
#define MCF5XXX_QSPI_QIR_WCEFE                   (0x800)
#define MCF5XXX_QSPI_QIR_ABRTL                   (0x1000)
#define MCF5XXX_QSPI_QIR_ABRTB                   (0x4000)
#define MCF5XXX_QSPI_QIR_WCEFB                   (0x8000)

/* Bit definitions and macros for MCF5XXX_QSPI_QAR */
#define MCF5XXX_QSPI_QAR_ADDR(x)                 (((x)&0x3F)<<0)
#define MCF5XXX_QSPI_QAR_TRANS                   (0)
#define MCF5XXX_QSPI_QAR_RECV                    (0x10)
#define MCF5XXX_QSPI_QAR_CMD                     (0x20)

/* Bit definitions and macros for MCF5XXX_QSPI_QDR */
#define MCF5XXX_QSPI_QDR_DATA(x)                 (((x)&0xFFFF)<<0)
#define MCF5XXX_QSPI_QDR_CONT                    (0x8000)
#define MCF5XXX_QSPI_QDR_BITSE                   (0x4000)
#define MCF5XXX_QSPI_QDR_DT                      (0x2000)
#define MCF5XXX_QSPI_QDR_DSCK                    (0x1000)
#define MCF5XXX_QSPI_QDR_CS(x)                   (((x)&0x0F)<<8)
#define MCF5XXX_QSPI_QDR_QSPI_CS3                (0x08)
#define MCF5XXX_QSPI_QDR_QSPI_CS2                (0x04)
#define MCF5XXX_QSPI_QDR_QSPI_CS1                (0x02)
#define MCF5XXX_QSPI_QDR_QSPI_CS0                (0x01)

/*
** MCF5XXX_QSPI_STRUCT
** QSPI
*/
typedef struct mcf5xxx_qspi_struct
{
   uint16_t  QMR;            /* QSPI mode register */
   uint16_t  filler1;
   uint16_t  QDLYR;          /* QSPI delay register */
   uint16_t  filler2;
   uint16_t  QWR;            /* QSPI wrap register */
   uint16_t  filler3;
   uint16_t  QIR;            /* QSPI interrupt register */
   uint16_t  filler4;
   uint16_t  QAR;            /* QSPI address register */
   uint16_t  filler5;
   uint16_t  QDR;            /* QSPI data register */
   unsigned char    filler6[42];
} MCF5XXX_QSPI_STRUCT, * MCF5XXX_QSPI_STRUCT_PTR;
typedef volatile struct mcf5xxx_qspi_struct * VMCF5XXX_QSPI_STRUCT_PTR;

#ifdef __cplusplus
}
#endif

#endif

