/*HEADER**********************************************************************
*
* Copyright 2008 Freescale Semiconductor, Inc.
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
*   This file contains the type definitions for the mcf5xxx Flex BUS module.
*
*
*END************************************************************************/

#ifndef __mcf5xxx_fb_h__
#define __mcf5xxx_fb_h__

#define __mcf5xxx_fb_h__version "$Version:3.0.3.0$"
#define __mcf5xxx_fb_h__date    "$Date:Aug-10-2009$"

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------*/
/*
** Flex BUS registers
*/


/*------------------------------------------------------------------------*/
/*

// Mini-FlexBus related constants

/* Bit definitions and macros for MCF5XXX_FBCS_CSAR */
#define MCF5XXX_FBCS_CSAR_BA(x)                  ((x)&0xFFFF0000)

/* Bit definitions and macros for FBCS_CSMR */
#define MCF5XXX_FBCS_CSMR_V                     (1)
#define MCF5XXX_FBCS_CSMR_WP                    (1 << 8)
#define MCF5XXX_FBCS_CSMR_BAM(x)                (((x) & 0xFFFF) << 16)
#define MCF5XXX_FBCS_CSMR_BAM_4G                (0xFFFF0000)
#define MCF5XXX_FBCS_CSMR_BAM_2G                (0x7FFF0000)
#define MCF5XXX_FBCS_CSMR_BAM_1G                (0x3FFF0000)
#define MCF5XXX_FBCS_CSMR_BAM_1024M             (0x3FFF0000)
#define MCF5XXX_FBCS_CSMR_BAM_512M              (0x1FFF0000)
#define MCF5XXX_FBCS_CSMR_BAM_256M              (0xFFF0000)
#define MCF5XXX_FBCS_CSMR_BAM_128M              (0x7FF0000)
#define MCF5XXX_FBCS_CSMR_BAM_64M               (0x3FF0000)
#define MCF5XXX_FBCS_CSMR_BAM_32M               (0x1FF0000)
#define MCF5XXX_FBCS_CSMR_BAM_16M               (0xFF0000)
#define MCF5XXX_FBCS_CSMR_BAM_8M                (0x7F0000)
#define MCF5XXX_FBCS_CSMR_BAM_4M                (0x3F0000)
#define MCF5XXX_FBCS_CSMR_BAM_2M                (0x1F0000)
#define MCF5XXX_FBCS_CSMR_BAM_1M                (0xF0000)
#define MCF5XXX_FBCS_CSMR_BAM_1024K             (0xF0000)
#define MCF5XXX_FBCS_CSMR_BAM_512K              (0x70000)
#define MCF5XXX_FBCS_CSMR_BAM_256K              (0x30000)
#define MCF5XXX_FBCS_CSMR_BAM_128K              (0x10000)
#define MCF5XXX_FBCS_CSMR_BAM_64K               (0)

/* Bit definitions and macros for FBCS_CSCR */
#define MCF5XXX_FBCS_CSCR_BSTW                  (1 << 3)
#define MCF5XXX_FBCS_CSCR_BSTR                  (1 << 4)
#define MCF5XXX_FBCS_CSCR_BEM                   (1 << 5)
#define MCF5XXX_FBCS_CSCR_PS(x)                 (((x) & 3) << 6)
#define MCF5XXX_FBCS_CSCR_PS_32                 (0)
#define MCF5XXX_FBCS_CSCR_PS_8                  (1 << 6)
#define MCF5XXX_FBCS_CSCR_PS_16                 (1 << 7)
#define MCF5XXX_FBCS_CSCR_AA                    (1 << 8)
#define MCF5XXX_FBCS_CSCR_SBM                   (1 << 9)
#define MCF5XXX_FBCS_CSCR_WS(x)                 (((x) & 0x3f) << 10)
#define MCF5XXX_FBCS_CSCR_WRAH(x)               (((x) & 3) << 16)
#define MCF5XXX_FBCS_CSCR_RDAH(x)               (((x) & 3) << 18)
#define MCF5XXX_FBCS_CSCR_ASET(x)               (((x) & 3) << 20)
#define MCF5XXX_FBCS_CSCR_SWSEN                 (1 << 23)
#define MCF5XXX_FBCS_CSCR_SWS(x)                (((x) & 0x3f) << 26)

/*
** MCF5XXX_FB_STRUCT
** This structure defines FlexBus registers
*/
typedef struct mcf5xxx_fb_struct {
    uint32_t CSAR;           // Chip-Select Address Register
    uint32_t CSMR;           // Chip-Select Mask Register
    uint32_t CSCR;           // Chip-Select Control Register
} MCF5XXX_FB_STRUCT, * MCF5XXX_FB_STRUCT_PTR;
typedef volatile struct mcf5xxx_fb_struct * VMCF5XXX_FB_STRUCT_PTR;

#ifdef __cplusplus
}
#endif

#endif /* __mcf5xxx_fb_h__ */
