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
*   This file contains the SCM type definitions for the mcf54xx processor.
*   It has been split out into a separate header file to share with 
*   other Coldfire PSP's.
*
*
*END************************************************************************/

#ifndef __mcf54xx_ccm_h__
#define __mcf54xx_ccm_h__

#define __mcf54xx_ccm_h__version "$Version:3.5.9.0$"
#define __mcf54xx_ccm_h__date    "$Date:Feb-24-2010$"

#ifdef __cplusplus
extern "C" {
#endif

#define MCF54XX_CCM_MISCCR_USBSRC               (0x1)
#define MCF54XX_CCM_MISCCR_USBOC                (0x2)
#define MCF54XX_CCM_MISCCR_USBPUE               (0x4)
#define MCF54XX_CCM_MISCCR_SSISRC               (0x10)
#define MCF54XX_CCM_MISCCR_TIMDMA               (0x20)
#define MCF54XX_CCM_MISCCR_SSIPUS               (0x40)
#define MCF54XX_CCM_MISCCR_SSIPUE               (0x80)
#define MCF54XX_CCM_MISCCR_BMT(x)               (((x) & 7) << 8)
#define MCF54XX_CCM_MISCCR_BMT_65536            (0)
#define MCF54XX_CCM_MISCCR_BMT_32768            (0x100)
#define MCF54XX_CCM_MISCCR_BMT_16384            (0x200)
#define MCF54XX_CCM_MISCCR_BMT_8192             (0x300)
#define MCF54XX_CCM_MISCCR_BMT_4096             (0x400)
#define MCF54XX_CCM_MISCCR_BMT_2048             (0x500)
#define MCF54XX_CCM_MISCCR_BMT_1024             (0x600)
#define MCF54XX_CCM_MISCCR_BMT_512              (0x700)
#define MCF54XX_CCM_MISCCR_BME                  (0x800)
#define MCF54XX_CCM_MISCCR_LIMP                 (0x1000)
#define MCF54XX_CCM_MISCCR_PLL_LOCK             (1 << 13)

/*
** MCF54XX_CCM_STRUCT
** ColdFire Integration Module
*/
typedef struct mcf54xx_ccm_struct {
#if PSP_MQX_CPU_IS_MCF5441X
    unsigned char   RCR;
    unsigned char   RSR;
    unsigned char   filler1[2];
#else // !PSP_MQX_CPU_IS_MCF5441X
    unsigned char   filler1[4];
#endif
    uint16_t CCR;            // Chip Configuration Register
    uint16_t filler2;
    uint16_t RCON;           // Reset Configuration Register
    uint16_t CIR;            // Chip Identification Register
#if PSP_MQX_CPU_IS_MCF5441X
    unsigned char   filler3[2];
    uint16_t MISCCR;         // Miscellaneous Control Register
    uint16_t CDRH;           // Clock Divider Register
    uint16_t CDRL;           // Clock Divider Register
    uint16_t UOCSR;          // USB On-the-Go Controller Status Register
    uint16_t UHCSR;          // USB Host Controller Status Register
    uint16_t MISCCR3;        // Miscellaneous control register 3
    uint16_t MISCCR2;        // Miscellaneous control register 2
    uint16_t ADCTSR;         // ADC trigger select register
    uint16_t DACTSR;         // DAC trigger select register
    uint16_t SBFSR;          // Serial boot facility status register
    uint16_t SBFCR;          // Serial boot facility control register
    uint16_t FNACR;          // FlexBus/NAND flash arbiter control register
#else  // !PSP_MQX_CPU_IS_MCF5441X
    unsigned char   filler3[4];
    uint16_t MISCCR;         // Miscellaneous Control Register
    uint16_t CDR;            // Clock Divider Register
    uint16_t UOCSR;          // USB On-the-Go Controller Status Register
    uint8_t  filler4[2];
    uint16_t SBFSR;          // Serial Boot Facility Status Register
    uint16_t SBFCR;          // Serial Boot Facility Control Register
#endif
} MCF54XX_CCM_STRUCT, * MCF54XX_CCM_STRUCT_PTR;
typedef volatile struct mcf54xx_ccm_struct * VMCF54XX_CCM_STRUCT_PTR;

#ifdef __cplusplus
}
#endif

#endif /* __mcf54xx_ccm_h__ */
