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
*   This file contains the type definitions for the mcf522x usbotg module.
*
*
*END************************************************************************/

#ifndef __mcf5xxx_usbotg_h__
#define __mcf5xxx_usbotg_h__

#define __mcf5xxx_usbotg_h__version "$Version:3.7.9.0$"
#define __mcf5xxx_usbotg_h__date    "$Date:Feb-7-2011$"

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------*/
/*
** USB OTG
*/

/* Bit definitions and macros for OTG_PORTSC1 */
#define MCF5XXX_USBOTG_PORTSC1_PTS      (0xC0000000)
#define MCF5XXX_USBOTG_PORTSC1_PTS_ULPI (0x80000000) /* ULPI interface */
#define MCF5XXX_USBOTG_PORTSC1_PTS_OCT  (0xC0000000) /* on chip transciever */

#define MCF5XXX_USBOTG_PORTSC1_PSPD      (0x0C000000)
#define MCF5XXX_USBOTG_PORTSC1_PSPD_FS   (0x00000000)
#define MCF5XXX_USBOTG_PORTSC1_PSPD_LS   (0x04000000)
#define MCF5XXX_USBOTG_PORTSC1_PSPD_HS   (0x08000000)

#define MCF5XXX_USBOTG_PORTSC1_PFSC      (0x01000000)
#define MCF5XXX_USBOTG_PORTSC1_PHCD      (0x00800000)
#define MCF5XXX_USBOTG_PORTSC1_WKOC      (0x00400000)
#define MCF5XXX_USBOTG_PORTSC1_WKDS      (0x00200000)
#define MCF5XXX_USBOTG_PORTSC1_WLCN      (0x00100000)

#define MCF5XXX_USBOTG_PORTSC1_PTC       (0x000F0000)
#define MCF5XXX_USBOTG_PORTSC1_PTC_NEN   (0x00000000)
#define MCF5XXX_USBOTG_PORTSC1_PTC_J     (0x00010000)
#define MCF5XXX_USBOTG_PORTSC1_PTC_K     (0x00020000)
#define MCF5XXX_USBOTG_PORTSC1_PTC_SEQ_NAK (0x00030000)
#define MCF5XXX_USBOTG_PORTSC1_PTC_PACKET  (0x00040000)
#define MCF5XXX_USBOTG_PORTSC1_PTC_HS    (0x00050000)
#define MCF5XXX_USBOTG_PORTSC1_PTC_FS    (0x00060000)
#define MCF5XXX_USBOTG_PORTSC1_PTC_LS    (0x00070000)

#define MCF5XXX_USBOTG_PORTSC1_PIC       (0x0000C000)
#define MCF5XXX_USBOTG_PORTSC1_PO        (0x00002000)
#define MCF5XXX_USBOTG_PORTSC1_PP        (0x00001000)

#define MCF5XXX_USBOTG_PORTSC1_LS        (0x00000C00)
#define MCF5XXX_USBOTG_PORTSC1_LS_SE0    (0x00000000)
#define MCF5XXX_USBOTG_PORTSC1_LS_J      (0x00000400)
#define MCF5XXX_USBOTG_PORTSC1_LS_K      (0x00000800)

#define MCF5XXX_USBOTG_PORTSC1_HSP       (0x00000200)
#define MCF5XXX_USBOTG_PORTSC1_PR        (0x00000100)

#define MCF5XXX_USBOTG_PORTSC1_SUSP      (0x00000080)
#define MCF5XXX_USBOTG_PORTSC1_FPR       (0x00000040)
#define MCF5XXX_USBOTG_PORTSC1_OCC       (0x00000020)
#define MCF5XXX_USBOTG_PORTSC1_OCA       (0x00000010)
#define MCF5XXX_USBOTG_PORTSC1_PEC       (0x00000008)
#define MCF5XXX_USBOTG_PORTSC1_PE        (0x00000004)
#define MCF5XXX_USBOTG_PORTSC1_CSC       (0x00000002)
#define MCF5XXX_USBOTG_PORTSC1_CCS       (0x00000001)

#define MCF5XXX_USBOTG_GPTIMER_MODE(x)   ((x & 1) << 24)
#define MCF5XXX_USBOTG_GPTIMER_ONESHOT   (0x00000000)
#define MCF5XXX_USBOTG_GPTIMER_REPEAT    (0x01000000)

#define MCF5XXX_USBOTG_GPTIMER_RESET     (0x40000000)

#define MCF5XXX_USBOTG_GPTIMER_RUN       (0x80000000)

/*------------------------------------------------------------------------*/
/*
** MCF5XXX_USBOTG_STRUCT
** USB OTG
*/
typedef struct mcf5xxx_usbotg_struct {
    /* Module Identification Registers */
    uint32_t ID;             /* Identification Register */
    uint32_t HWGENERAL;      /* General Hardware Parameters */
    uint32_t HWHOST;         /* Host Hardware Parameters */
    uint32_t HWDEVICE;       /* Device Hardware Parameters */
    uint32_t HWTXBUF;        /* TX Buffer Hardware Parameters */
    uint32_t HWRXBUF;        /* RX Buffer Hardware Parameters */

    uint8_t  filler0[104];
    
    /* Device/Host Timer Registers */
    uint32_t GPTIMER0LD;     /* General Purpose Timer 0 Load */
    uint32_t GPTIMER0CTL;    /* General Purpose Timer 0 Control */
    uint32_t GPTIMER1LD;     /* General Purpose Timer 1 Load */
    uint32_t GPTIMER1CTL;    /* General Purpose Timer 1 Control */

    uint8_t  filler1[112];

    /* Capability Registers */
    uint16_t HCIVERSION;     /* Host Interface Version Number */
    uint8_t  filler2;
    uint8_t  CAPLENGTH;      /* Capability Register Length */
    uint32_t HCSPARAMS;      /* Host Structural Parameters  */
    uint32_t HCCPARAMS;      /* Host Capability Parameters */

    uint8_t filler3[22];

    uint16_t DCIVERSION;     /* Device Interface Version Number */
    uint32_t DCCPARAMS;      /* Device Capability Parameters */

    uint8_t filler4[24];
    
    /* Operational Registers */
    uint32_t USBCMD;         /* USB Command */
    uint32_t USBSTS;         /* USB Status */
    uint32_t USBINTR;        /* USB Interrupt Enable */
    uint32_t FRINDEX;        /* USB Frame Index */
    
    uint8_t filler5[4];
    
    union {
        uint32_t PERIODICLISTBASE;   /* Periodic Frame List Base Address */
        uint32_t DEVICEADDR;         /* Device Address */
    };
    
    union {
        uint32_t ASYNCLISTADDR;  /* Current Asynchronous List Address */
        uint32_t EPLISTADDR;     /* Address at Endpoint List */
    };
    
    uint32_t TTCTRL;         /* Host TT Asynchronous Buffer Control */
    uint32_t BURSTSIZE;      /* Master Interface Data Burst Size */
    uint32_t TXFILLTUNING;   /* Host Transmit FIFO Tuning Control */
    
    uint8_t filler6[8];
    
    uint32_t ULPI_VIEWPORT;  /* ULPI control register */
    
    uint8_t filler7[12];
    
    uint32_t CONFIGFLAG;     /* Configure Flag Register */
    uint32_t PORTSC1;        /* Port Status/Control */
    
    uint8_t filler8[28];
    
    uint32_t OTGSC;          /* On-The-Go Status and Control */
    uint32_t MODE;           /* USB Mode Register */
    uint32_t EPSETUPSR;      /* Endpoint Setup Status Register */
    uint32_t EPPRIME;        /* Endpoint Initialization */
    uint32_t EPFLUSH;        /* Endpoint De-initialize */
    uint32_t EPSR;           /* Endpoint Status Register */
    uint32_t EPCOMPLETE;     /* Endpoint Complete */
    uint32_t EPCR0;          /* Endpoint Control Register 0 */
    uint32_t EPCR1;          /* Endpoint Control Register 1 */
    uint32_t EPCR2;          /* Endpoint Control Register 2 */
    uint32_t EPCR3;          /* Endpoint Control Register 3 */
} MCF5XXX_USBOTG_STRUCT, * MCF5XXX_USBOTG_STRUCT_PTR;
typedef volatile struct mcf5xxx_usbotg_struct * VMCF5XXX_USBOTG_STRUCT_PTR;

#ifdef __cplusplus
}
#endif


#endif
