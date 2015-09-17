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
*   This file contains the interrupt controller type definitions for
*   the mcf52xx processor.  It has been split out into a separate header
*   file to share with other Coldfire PSP's.
*
*
*END************************************************************************/

#ifndef __mcf52xx_ictl_h__
#define __mcf52xx_ictl_h__

#define __mcf52xx_ictl_h__version "$Version:3.6.9.0$"
#define __mcf52xx_ictl_h__date    "$Date:Jun-4-2010$"

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------------------------------------*/
/*
** ICTRL  registers bit set.
*/

/*
** Interrupt definitions
*/
#define MCF52xx_ICTRL_MASKALL   1
#define MCF52xx_ICTRL0_BASEx    64
#define MCF52xx_ICTRL1_BASEx    128

/* Bit definitions and macros for MCF52XX_ICTR_IPRH */
#define MCF52XX_ICTR_IPRH_INT32                  (0x1)
#define MCF52XX_ICTR_IPRH_INT33                  (0x2)
#define MCF52XX_ICTR_IPRH_INT34                  (0x4)
#define MCF52XX_ICTR_IPRH_INT35                  (0x8)
#define MCF52XX_ICTR_IPRH_INT36                  (0x10)
#define MCF52XX_ICTR_IPRH_INT37                  (0x20)
#define MCF52XX_ICTR_IPRH_INT38                  (0x40)
#define MCF52XX_ICTR_IPRH_INT39                  (0x80)
#define MCF52XX_ICTR_IPRH_INT40                  (0x100)
#define MCF52XX_ICTR_IPRH_INT41                  (0x200)
#define MCF52XX_ICTR_IPRH_INT42                  (0x400)
#define MCF52XX_ICTR_IPRH_INT43                  (0x800)
#define MCF52XX_ICTR_IPRH_INT44                  (0x1000)
#define MCF52XX_ICTR_IPRH_INT45                  (0x2000)
#define MCF52XX_ICTR_IPRH_INT46                  (0x4000)
#define MCF52XX_ICTR_IPRH_INT47                  (0x8000)
#define MCF52XX_ICTR_IPRH_INT48                  (0x10000)
#define MCF52XX_ICTR_IPRH_INT49                  (0x20000)
#define MCF52XX_ICTR_IPRH_INT50                  (0x40000)
#define MCF52XX_ICTR_IPRH_INT51                  (0x80000)
#define MCF52XX_ICTR_IPRH_INT52                  (0x100000)
#define MCF52XX_ICTR_IPRH_INT53                  (0x200000)
#define MCF52XX_ICTR_IPRH_INT54                  (0x400000)
#define MCF52XX_ICTR_IPRH_INT55                  (0x800000)
#define MCF52XX_ICTR_IPRH_INT56                  (0x1000000)
#define MCF52XX_ICTR_IPRH_INT57                  (0x2000000)
#define MCF52XX_ICTR_IPRH_INT58                  (0x4000000)
#define MCF52XX_ICTR_IPRH_INT59                  (0x8000000)
#define MCF52XX_ICTR_IPRH_INT60                  (0x10000000)
#define MCF52XX_ICTR_IPRH_INT61                  (0x20000000)
#define MCF52XX_ICTR_IPRH_INT62                  (0x40000000)
#define MCF52XX_ICTR_IPRH_INT63                  (0x80000000)

/* Bit definitions and macros for MCF52XX_ICTR_IPRL */
#define MCF52XX_ICTR_IPRL_INT1                   (0x2)
#define MCF52XX_ICTR_IPRL_INT2                   (0x4)
#define MCF52XX_ICTR_IPRL_INT3                   (0x8)
#define MCF52XX_ICTR_IPRL_INT4                   (0x10)
#define MCF52XX_ICTR_IPRL_INT5                   (0x20)
#define MCF52XX_ICTR_IPRL_INT6                   (0x40)
#define MCF52XX_ICTR_IPRL_INT7                   (0x80)
#define MCF52XX_ICTR_IPRL_INT8                   (0x100)
#define MCF52XX_ICTR_IPRL_INT9                   (0x200)
#define MCF52XX_ICTR_IPRL_INT10                  (0x400)
#define MCF52XX_ICTR_IPRL_INT11                  (0x800)
#define MCF52XX_ICTR_IPRL_INT12                  (0x1000)
#define MCF52XX_ICTR_IPRL_INT13                  (0x2000)
#define MCF52XX_ICTR_IPRL_INT14                  (0x4000)
#define MCF52XX_ICTR_IPRL_INT15                  (0x8000)
#define MCF52XX_ICTR_IPRL_INT16                  (0x10000)
#define MCF52XX_ICTR_IPRL_INT17                  (0x20000)
#define MCF52XX_ICTR_IPRL_INT18                  (0x40000)
#define MCF52XX_ICTR_IPRL_INT19                  (0x80000)
#define MCF52XX_ICTR_IPRL_INT20                  (0x100000)
#define MCF52XX_ICTR_IPRL_INT21                  (0x200000)
#define MCF52XX_ICTR_IPRL_INT22                  (0x400000)
#define MCF52XX_ICTR_IPRL_INT23                  (0x800000)
#define MCF52XX_ICTR_IPRL_INT24                  (0x1000000)
#define MCF52XX_ICTR_IPRL_INT25                  (0x2000000)
#define MCF52XX_ICTR_IPRL_INT26                  (0x4000000)
#define MCF52XX_ICTR_IPRL_INT27                  (0x8000000)
#define MCF52XX_ICTR_IPRL_INT28                  (0x10000000)
#define MCF52XX_ICTR_IPRL_INT29                  (0x20000000)
#define MCF52XX_ICTR_IPRL_INT30                  (0x40000000)
#define MCF52XX_ICTR_IPRL_INT31                  (0x80000000)

/* Bit definitions and macros for MCF52XX_ICTR_IMRH */
#define MCF52XX_ICTR_IMRH_INT_MASK32             (0x1)
#define MCF52XX_ICTR_IMRH_INT_MASK33             (0x2)
#define MCF52XX_ICTR_IMRH_INT_MASK34             (0x4)
#define MCF52XX_ICTR_IMRH_INT_MASK35             (0x8)
#define MCF52XX_ICTR_IMRH_INT_MASK36             (0x10)
#define MCF52XX_ICTR_IMRH_INT_MASK37             (0x20)
#define MCF52XX_ICTR_IMRH_INT_MASK38             (0x40)
#define MCF52XX_ICTR_IMRH_INT_MASK39             (0x80)
#define MCF52XX_ICTR_IMRH_INT_MASK40             (0x100)
#define MCF52XX_ICTR_IMRH_INT_MASK41             (0x200)
#define MCF52XX_ICTR_IMRH_INT_MASK42             (0x400)
#define MCF52XX_ICTR_IMRH_INT_MASK43             (0x800)
#define MCF52XX_ICTR_IMRH_INT_MASK44             (0x1000)
#define MCF52XX_ICTR_IMRH_INT_MASK45             (0x2000)
#define MCF52XX_ICTR_IMRH_INT_MASK46             (0x4000)
#define MCF52XX_ICTR_IMRH_INT_MASK47             (0x8000)
#define MCF52XX_ICTR_IMRH_INT_MASK48             (0x10000)
#define MCF52XX_ICTR_IMRH_INT_MASK49             (0x20000)
#define MCF52XX_ICTR_IMRH_INT_MASK50             (0x40000)
#define MCF52XX_ICTR_IMRH_INT_MASK51             (0x80000)
#define MCF52XX_ICTR_IMRH_INT_MASK52             (0x100000)
#define MCF52XX_ICTR_IMRH_INT_MASK53             (0x200000)
#define MCF52XX_ICTR_IMRH_INT_MASK54             (0x400000)
#define MCF52XX_ICTR_IMRH_INT_MASK55             (0x800000)
#define MCF52XX_ICTR_IMRH_INT_MASK56             (0x1000000)
#define MCF52XX_ICTR_IMRH_INT_MASK57             (0x2000000)
#define MCF52XX_ICTR_IMRH_INT_MASK58             (0x4000000)
#define MCF52XX_ICTR_IMRH_INT_MASK59             (0x8000000)
#define MCF52XX_ICTR_IMRH_INT_MASK60             (0x10000000)
#define MCF52XX_ICTR_IMRH_INT_MASK61             (0x20000000)
#define MCF52XX_ICTR_IMRH_INT_MASK62             (0x40000000)
#define MCF52XX_ICTR_IMRH_INT_MASK63             (0x80000000)

/* Bit definitions and macros for MCF52XX_ICTR_IMRL */
#define MCF52XX_ICTR_IMRL_MASKALL                (0x1)
#define MCF52XX_ICTR_IMRL_INT_MASK1              (0x2)
#define MCF52XX_ICTR_IMRL_INT_MASK2              (0x4)
#define MCF52XX_ICTR_IMRL_INT_MASK3              (0x8)
#define MCF52XX_ICTR_IMRL_INT_MASK4              (0x10)
#define MCF52XX_ICTR_IMRL_INT_MASK5              (0x20)
#define MCF52XX_ICTR_IMRL_INT_MASK6              (0x40)
#define MCF52XX_ICTR_IMRL_INT_MASK7              (0x80)
#define MCF52XX_ICTR_IMRL_INT_MASK8              (0x100)
#define MCF52XX_ICTR_IMRL_INT_MASK9              (0x200)
#define MCF52XX_ICTR_IMRL_INT_MASK10             (0x400)
#define MCF52XX_ICTR_IMRL_INT_MASK11             (0x800)
#define MCF52XX_ICTR_IMRL_INT_MASK12             (0x1000)
#define MCF52XX_ICTR_IMRL_INT_MASK13             (0x2000)
#define MCF52XX_ICTR_IMRL_INT_MASK14             (0x4000)
#define MCF52XX_ICTR_IMRL_INT_MASK15             (0x8000)
#define MCF52XX_ICTR_IMRL_INT_MASK16             (0x10000)
#define MCF52XX_ICTR_IMRL_INT_MASK17             (0x20000)
#define MCF52XX_ICTR_IMRL_INT_MASK18             (0x40000)
#define MCF52XX_ICTR_IMRL_INT_MASK19             (0x80000)
#define MCF52XX_ICTR_IMRL_INT_MASK20             (0x100000)
#define MCF52XX_ICTR_IMRL_INT_MASK21             (0x200000)
#define MCF52XX_ICTR_IMRL_INT_MASK22             (0x400000)
#define MCF52XX_ICTR_IMRL_INT_MASK23             (0x800000)
#define MCF52XX_ICTR_IMRL_INT_MASK24             (0x1000000)
#define MCF52XX_ICTR_IMRL_INT_MASK25             (0x2000000)
#define MCF52XX_ICTR_IMRL_INT_MASK26             (0x4000000)
#define MCF52XX_ICTR_IMRL_INT_MASK27             (0x8000000)
#define MCF52XX_ICTR_IMRL_INT_MASK28             (0x10000000)
#define MCF52XX_ICTR_IMRL_INT_MASK29             (0x20000000)
#define MCF52XX_ICTR_IMRL_INT_MASK30             (0x40000000)
#define MCF52XX_ICTR_IMRL_INT_MASK31             (0x80000000)

/* Bit definitions and macros for MCF52XX_ICTR_INTFRCH */
#define MCF52XX_ICTR_INTFRCH_INTFRC32            (0x1)
#define MCF52XX_ICTR_INTFRCH_INTFRC33            (0x2)
#define MCF52XX_ICTR_INTFRCH_INTFRC34            (0x4)
#define MCF52XX_ICTR_INTFRCH_INTFRC35            (0x8)
#define MCF52XX_ICTR_INTFRCH_INTFRC36            (0x10)
#define MCF52XX_ICTR_INTFRCH_INTFRC37            (0x20)
#define MCF52XX_ICTR_INTFRCH_INTFRC38            (0x40)
#define MCF52XX_ICTR_INTFRCH_INTFRC39            (0x80)
#define MCF52XX_ICTR_INTFRCH_INTFRC40            (0x100)
#define MCF52XX_ICTR_INTFRCH_INTFRC41            (0x200)
#define MCF52XX_ICTR_INTFRCH_INTFRC42            (0x400)
#define MCF52XX_ICTR_INTFRCH_INTFRC43            (0x800)
#define MCF52XX_ICTR_INTFRCH_INTFRC44            (0x1000)
#define MCF52XX_ICTR_INTFRCH_INTFRC45            (0x2000)
#define MCF52XX_ICTR_INTFRCH_INTFRC46            (0x4000)
#define MCF52XX_ICTR_INTFRCH_INTFRC47            (0x8000)
#define MCF52XX_ICTR_INTFRCH_INTFRC48            (0x10000)
#define MCF52XX_ICTR_INTFRCH_INTFRC49            (0x20000)
#define MCF52XX_ICTR_INTFRCH_INTFRC50            (0x40000)
#define MCF52XX_ICTR_INTFRCH_INTFRC51            (0x80000)
#define MCF52XX_ICTR_INTFRCH_INTFRC52            (0x100000)
#define MCF52XX_ICTR_INTFRCH_INTFRC53            (0x200000)
#define MCF52XX_ICTR_INTFRCH_INTFRC54            (0x400000)
#define MCF52XX_ICTR_INTFRCH_INTFRC55            (0x800000)
#define MCF52XX_ICTR_INTFRCH_INTFRC56            (0x1000000)
#define MCF52XX_ICTR_INTFRCH_INTFRC57            (0x2000000)
#define MCF52XX_ICTR_INTFRCH_INTFRC58            (0x4000000)
#define MCF52XX_ICTR_INTFRCH_INTFRC59            (0x8000000)
#define MCF52XX_ICTR_INTFRCH_INTFRC60            (0x10000000)
#define MCF52XX_ICTR_INTFRCH_INTFRC61            (0x20000000)
#define MCF52XX_ICTR_INTFRCH_INTFRC62            (0x40000000)
#define MCF52XX_ICTR_INTFRCH_INTFRC63            (0x80000000)

/* Bit definitions and macros for MCF52XX_ICTR_INTFRCL */
#define MCF52XX_ICTR_INTFRCL_INTFRC1             (0x2)
#define MCF52XX_ICTR_INTFRCL_INTFRC2             (0x4)
#define MCF52XX_ICTR_INTFRCL_INTFRC3             (0x8)
#define MCF52XX_ICTR_INTFRCL_INTFRC4             (0x10)
#define MCF52XX_ICTR_INTFRCL_INTFRC5             (0x20)
#define MCF52XX_ICTR_INTFRCL_INTFRC6             (0x40)
#define MCF52XX_ICTR_INTFRCL_INTFRC7             (0x80)
#define MCF52XX_ICTR_INTFRCL_INTFRC8             (0x100)
#define MCF52XX_ICTR_INTFRCL_INTFRC9             (0x200)
#define MCF52XX_ICTR_INTFRCL_INTFRC10            (0x400)
#define MCF52XX_ICTR_INTFRCL_INTFRC11            (0x800)
#define MCF52XX_ICTR_INTFRCL_INTFRC12            (0x1000)
#define MCF52XX_ICTR_INTFRCL_INTFRC13            (0x2000)
#define MCF52XX_ICTR_INTFRCL_INTFRC14            (0x4000)
#define MCF52XX_ICTR_INTFRCL_INTFRC15            (0x8000)
#define MCF52XX_ICTR_INTFRCL_INTFRC16            (0x10000)
#define MCF52XX_ICTR_INTFRCL_INTFRC17            (0x20000)
#define MCF52XX_ICTR_INTFRCL_INTFRC18            (0x40000)
#define MCF52XX_ICTR_INTFRCL_INTFRC19            (0x80000)
#define MCF52XX_ICTR_INTFRCL_INTFRC20            (0x100000)
#define MCF52XX_ICTR_INTFRCL_INTFRC21            (0x200000)
#define MCF52XX_ICTR_INTFRCL_INTFRC22            (0x400000)
#define MCF52XX_ICTR_INTFRCL_INTFRC23            (0x800000)
#define MCF52XX_ICTR_INTFRCL_INTFRC24            (0x1000000)
#define MCF52XX_ICTR_INTFRCL_INTFRC25            (0x2000000)
#define MCF52XX_ICTR_INTFRCL_INTFRC26            (0x4000000)
#define MCF52XX_ICTR_INTFRCL_INTFRC27            (0x8000000)
#define MCF52XX_ICTR_INTFRCL_INTFRC28            (0x10000000)
#define MCF52XX_ICTR_INTFRCL_INTFRC29            (0x20000000)
#define MCF52XX_ICTR_INTFRCL_INTFRC30            (0x40000000)
#define MCF52XX_ICTR_INTFRCL_INTFRC31            (0x80000000)

/* Bit definitions and macros for MCF52XX_ICTR_IRLR */
#define MCF52XX_ICTR_IRLR_IRQ(x)                 (((x)&0x7F)<<0x1)

/* Bit definitions and macros for MCF52XX_ICTR_IACKLPR */
#define MCF52XX_ICTR_IACKLPR_PRI(x)              (((x)&0xF)<<0)
#define MCF52XX_ICTR_IACKLPR_LEVEL(x)            (((x)&0x7)<<0x4)

/* Bit definitions and macros for MCF52XX_ICTR_ICR */
#define MCF52XX_ICTR_ICR_IP(x)                   (((x)&0x7)<<0)
#define MCF52XX_ICTR_ICR_IL(x)                   (((x)&0x7)<<0x3)

/* Bit definitions and macros for MCF52XX_ICTR_SWIACK */
#define MCF52XX_ICTR_SWIACK_VECTOR(x)            (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF52XX_ICTRL_LIACK */
#define MCF52XX_ICTR_LIACK_VECTOR(x)             (((x)&0xFF)<<0)


/*------------------------------------------------------------------------*/
/*
** GIACK  registers bit set.
*/

/* Bit definitions and macros for MCF52XX_GIACR_GSWIACK */
#define MCF52XX_GIACK_GSWIACK_VECTOR(x)          (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF52XX_GIACR_GLIACK */
#define MCF52XX_GIACK_GLIACK_VECTOR(x)           (((x)&0xFF)<<0)


/*
** Interrupt controller
*/
typedef struct mcf52xx_int_ctrl_struct
{
   uint32_t IPRH;                // Interrupt pending register high
   uint32_t IPRL;                // Interrupt pending register low
   uint32_t IMRH;                // Interrupt mask register high
   uint32_t IMRL;                // Interrupt mask register low
   uint32_t INTFRCH;             // Interrupt force register high
   uint32_t INTFRCL;             // Interrupt force register low
} MCF52XX_INT_CTRL_STRUCT, * MCF52XX_INT_CTRL_STRUCT_PTR;
typedef volatile struct mcf52xx_int_ctrl_struct * VMCF52xx_INT_CTRL_STRUCT_PTR;

/*
** MCF52XX_IACK_STRUCT
** Interrupt acknowledgements
*/
typedef struct mcf52xx_iack_struct
{
   unsigned char SWACK;                // Interrupt acknowledge register
   unsigned char filler1[3];
   unsigned char L1IACK;               // Interrupt acknowledge register
   unsigned char filler2[3];
   unsigned char L2IACK;               // Interrupt acknowledge register
   unsigned char filler3[3];
   unsigned char L3IACK;               // Interrupt acknowledge register
   unsigned char filler4[3];
   unsigned char L4IACK;               // Interrupt acknowledge register
   unsigned char filler5[3];
   unsigned char L5IACK;               // Interrupt acknowledge register
   unsigned char filler6[3];
   unsigned char L6IACK;               // Interrupt acknowledge register
   unsigned char filler7[3];
   unsigned char L7IACK;               // Interrupt acknowledge register
   unsigned char filler8[3];
} MCF52XX_IACK_STRUCT, * MCF52XX_IACK_STRUCT_PTR;
typedef volatile struct mcf52xx_iack_struct * VMCF52XX_IACK_STRUCT_PTR;

typedef struct mcf52xx_glack_struct
{
   unsigned char GSWIACK;                // Global Software Interrupt Acknowledge Register
   unsigned char filler1[3];
   unsigned char GL1IACK;               // Global Level 1 Interrupt Acknowledge Register
   unsigned char filler2[3];
   unsigned char GL2IACK;               // Global Level 1 Interrupt Acknowledge Register
   unsigned char filler3[3];
   unsigned char GL3IACK;               // Global Level 1 Interrupt Acknowledge Register
   unsigned char filler4[3];
   unsigned char GL4IACK;               // Global Level 1 Interrupt Acknowledge Register
   unsigned char filler5[3];
   unsigned char GL5IACK;               // Global Level 1 Interrupt Acknowledge Register
   unsigned char filler6[3];
   unsigned char GL6IACK;               // Global Level 1 Interrupt Acknowledge Register
   unsigned char filler7[3];
   unsigned char GL7IACK;               // Global Level 1 Interrupt Acknowledge Register
   unsigned char filler8[3];
} MCF52XX_GIACK_STRUCT, * MCF52XX_GIACK_STRUCT_PTR;
typedef volatile struct mcf52xx_giack_struct * VMCF52XX_GIACK_STRUCT_PTR;

/*
** MCF52XX_ICTRL0_STRUCT
** Interrupt controller 0
*/
typedef struct mcf52xx_ictrl0_struct
{
   MCF52XX_INT_CTRL_STRUCT  ICTRL;
   unsigned char                    IRLR;           // Interrupt Request Level Register
   unsigned char                    IACKLPR;         // Interrupt Acknowledge Level and Priority Register
   unsigned char                    filler0[38];
   unsigned char                    ICR[64];
   unsigned char                    filler1[96];
   MCF52XX_IACK_STRUCT      IACK;
//   unsigned char                    filler11[736];
//   MCF52XX_GIACK_STRUCT     GIACK;
} MCF52XX_ICTRL0_STRUCT, * MCF52XX_ICTRL0_STRUCT_PTR;
typedef volatile struct mcf52xx_ictrl0_struct * VMCF52XX_ICTRL0_STRUCT_PTR;

/*
** MCF52XX_ICTRL1_STRUCT
** Interrupt controller 1
*/
typedef struct mcf52xx_ictrl1_struct
{
   MCF52XX_INT_CTRL_STRUCT  ICTRL;
   unsigned char                    IRLR;           // Interrupt Request Level Register
   unsigned char                    IACKLPR;         // Interrupt Acknowledge Level and Priority Register
   unsigned char                    filler0[38];
   unsigned char                    ICR[64];
   unsigned char                    filler1[96];
   MCF52XX_IACK_STRUCT      IACK;
//   unsigned char                    filler11[736];
//   MCF52XX_GIACK_STRUCT     GIACK;
} MCF52XX_ICTRL1_STRUCT, * MCF52XX_ICTRL1_STRUCT_PTR;
typedef volatile struct mcf52xx_ictrl1_struct * VMCF52XX_ICTRL1_STRUCT_PTR;


#ifdef __cplusplus
}
#endif

#endif /* __mcf52xx_ictl_h__ */
/* EOF */
