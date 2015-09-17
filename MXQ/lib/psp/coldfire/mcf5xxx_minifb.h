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
*   This file contains the type definitions for the mcf5xxx Mini Flex BUS module.
*
*
*END************************************************************************/

#ifndef __mcf5xxx_minifb_h__
#define __mcf5xxx_minifb_h__

#define __mcf5xxx_minifb_h__version "$Version:3.6.5.0$"
#define __mcf5xxx_minifb_h__date    "$Date:Aug-31-2010$"

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------*/
/*
** Mini Flex BUS registers
*/


/*------------------------------------------------------------------------*/

// Mini-FlexBus related constants

/* Bit definitions and macros for MCF5XXX_FBCS_CSAR */
#define MCF5XXX_FBCS_CSAR_BA(x)                  ((x)&0xFFFF0000)

/* Bit definitions and macros for MCF5XXX_FBCS_CSMR */
#define MCF5XXX_FBCS_CSMR_V                      (0x1)
#define MCF5XXX_FBCS_CSMR_WP                     (0x100)
#define MCF5XXX_FBCS_CSMR_BAM(x)                 (((x)&0xFFFF)<<0x10)
#define MCF5XXX_FBCS_CSMR_BAM_4G                 (0xFFFF0000)
#define MCF5XXX_FBCS_CSMR_BAM_2G                 (0x7FFF0000)
#define MCF5XXX_FBCS_CSMR_BAM_1G                 (0x3FFF0000)
#define MCF5XXX_FBCS_CSMR_BAM_1024M              (0x3FFF0000)
#define MCF5XXX_FBCS_CSMR_BAM_512M               (0x1FFF0000)
#define MCF5XXX_FBCS_CSMR_BAM_256M               (0xFFF0000)
#define MCF5XXX_FBCS_CSMR_BAM_128M               (0x7FF0000)
#define MCF5XXX_FBCS_CSMR_BAM_64M                (0x3FF0000)
#define MCF5XXX_FBCS_CSMR_BAM_32M                (0x1FF0000)
#define MCF5XXX_FBCS_CSMR_BAM_16M                (0xFF0000)
#define MCF5XXX_FBCS_CSMR_BAM_8M                 (0x7F0000)
#define MCF5XXX_FBCS_CSMR_BAM_4M                 (0x3F0000)
#define MCF5XXX_FBCS_CSMR_BAM_2M                 (0x1F0000)
#define MCF5XXX_FBCS_CSMR_BAM_1M                 (0xF0000)
#define MCF5XXX_FBCS_CSMR_BAM_1024K              (0xF0000)
#define MCF5XXX_FBCS_CSMR_BAM_512K               (0x70000)
#define MCF5XXX_FBCS_CSMR_BAM_256K               (0x30000)
#define MCF5XXX_FBCS_CSMR_BAM_128K               (0x10000)
#define MCF5XXX_FBCS_CSMR_BAM_64K                (0)

/* Bit definitions and macros for MCF5XXX_FBCS_CSCR */
#define MCF5XXX_FBCS_CSCR_BSTW                   (0x8)
#define MCF5XXX_FBCS_CSCR_BSTR                   (0x10)
#define MCF5XXX_FBCS_CSCR_PS(x)                  (((x)&0x3)<<0x6)
#define MCF5XXX_FBCS_CSCR_PS_8                   (0x40)
#define MCF5XXX_FBCS_CSCR_PS_16                  (0x80)
#define MCF5XXX_FBCS_CSCR_AA                     (0x100)
#define MCF5XXX_FBCS_CSCR_MUX                    (0x200)
#define MCF5XXX_FBCS_CSCR_WS(x)                  (((x)&0x3F)<<0xA)
#define MCF5XXX_FBCS_CSCR_WRAH(x)                (((x)&0x3)<<0x10)
#define MCF5XXX_FBCS_CSCR_RDAH(x)                (((x)&0x3)<<0x12)
#define MCF5XXX_FBCS_CSCR_ASET(x)                (((x)&0x3)<<0x14)
#define MCF5XXX_FBCS_CSCR_SWSEN                  (0x800000)
#define MCF5XXX_FBCS_CSCR_SWS(x)                 (((x)&0x3F)<<0x1A)



/*
** MCF5XXX_MINIFB_STRUCT
** This structure defines Mini-FlexBus registers
*/
typedef struct mcf5xxx_minifb_struct
{
    uint32_t CSAR0;          // Chip-Select Address Register
    uint32_t CSMR0;          // Chip-Select Mask Register
    uint32_t CSCR0;          // Chip-Select Control Register

    uint32_t CSAR1;          // Chip-Select Address Register
    uint32_t CSMR1;          // Chip-Select Mask Register
    uint32_t CSCR1;          // Chip-Select Control Register
} MCF5XXX_MINIFB_STRUCT, * MCF5XXX_MINIFB_STRUCT_PTR;

typedef volatile struct mcf5xxx_minifb_struct * VMCF5XXX_MINIFB_STRUCT_PTR;

#ifdef __cplusplus
}
#endif

#endif  //__mcf5xxx_fcan_h__

