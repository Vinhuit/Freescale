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
*   This file contains the type definitions for the MCF52XX PIT module.
*
*
*END************************************************************************/

#ifndef __mcf52xx_cfm_h__
#define __mcf52xx_cfm_h__

#define __mcf52xx_cfm_h__version "$Version:3.0.1.0$"
#define __mcf52xx_cfm_h__date    "$Date:May-21-2009$"

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------*/
/*
** CFM  registers bit set.
*/
/* Bit definitions and macros for MCF52XX_CFM_CFMMCR */
#define MCF52XX_CFM_CFMMCR_KEYACC                (0x20)
#define MCF52XX_CFM_CFMMCR_CCIE                  (0x40)
#define MCF52XX_CFM_CFMMCR_CBEIE                 (0x80)
#define MCF52XX_CFM_CFMMCR_AEIE                  (0x100)
#define MCF52XX_CFM_CFMMCR_PVIE                  (0x200)
#define MCF52XX_CFM_CFMMCR_LOCK                  (0x400)

/* Bit definitions and macros for MCF52XX_CFM_CFMCLKD */
#define MCF52XX_CFM_CFMCLKD_DIV(x)               (((x)&0x3F)<<0)
#define MCF52XX_CFM_CFMCLKD_PRDIV8               (0x40)
#define MCF52XX_CFM_CFMCLKD_DIVLD                (0x80)

/* Bit definitions and macros for MCF52XX_CFM_CFMSEC */
#define MCF52XX_CFM_CFMSEC_SEC(x)                (((x)&0xFFFF)<<0)
#define MCF52XX_CFM_CFMSEC_SECSTAT               (0x40000000)
#define MCF52XX_CFM_CFMSEC_KEYEN                 (0x80000000)

/* Bit definitions and macros for MCF52XX_CFM_CFMPROT */
#define MCF52XX_CFM_CFMPROT_PROTECT(x)           (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF52XX_CFM_CFMSACC */
#define MCF52XX_CFM_CFMSACC_SUPV(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF52XX_CFM_CFMDACC */
#define MCF52XX_CFM_CFMDACC_DACC(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF52XX_CFM_CFMUSTAT */
#define MCF52XX_CFM_CFMUSTAT_BLANK               (0x4)
#define MCF52XX_CFM_CFMUSTAT_ACCERR              (0x10)
#define MCF52XX_CFM_CFMUSTAT_PVIOL               (0x20)
#define MCF52XX_CFM_CFMUSTAT_CCIF                (0x40)
#define MCF52XX_CFM_CFMUSTAT_CBEIF               (0x80)

/* Bit definitions and macros for MCF52XX_CFM_CFMCMD */
#define MCF52XX_CFM_CFMCMD_CMD(x)                (((x)&0x7F)<<0)
#define MCF52XX_CFM_CFMCMD_BLANK_CHECK           (0x5)
#define MCF52XX_CFM_CFMCMD_PAGE_ERASE_VERIFY     (0x6)
#define MCF52XX_CFM_CFMCMD_WORD_PROGRAM          (0x20)
#define MCF52XX_CFM_CFMCMD_PAGE_ERASE            (0x40)
#define MCF52XX_CFM_CFMCMD_MASS_ERASE            (0x41)

/* Bit definitions and macros for MCF52XX_CFM_CFMCLKSEL */
#define MCF52XX_CFM_CFMCLKSEL_CLKSEL(x)          (((x)&0x3)<<0)

/*
** MCF5225_CFM_STRUCT                                                                   
** Internal flash registers
*/
typedef struct MCF52XX_cfm_struct
{
   uint16_t  CFMMCR;         // CFM Configuration Register
   unsigned char    CFMCLKD;        // CFM Clock Divider Register
   unsigned char    filler1[5];
   uint32_t  CFMSEC;         // CFM Security Register
   unsigned char    filler2[4];
   uint32_t  CFMPROT;        // CFM Protection Register
   uint32_t  CFMSACC;        // CFM Supervisor Access Register
   uint32_t  CFMDACC;        // CFM Data Access Register
   unsigned char    filler3[4];
   unsigned char    CFMUSTAT;       // CFM User Status Register
   unsigned char    filler4[3];
   unsigned char    CFMCMD;         // CFM Command Register
   unsigned char    filler5[37];
   uint16_t  CFMCLKSEL;      // CFM Clock Select Register
   unsigned char    filler[0xFFB3]; 
} MCF52XX_CFM_STRUCT, * MCF52XX_CFM_STRUCT_PTR;
typedef volatile struct MCF52XX_cfm_struct * VMCF52XX_CFM_STRUCT_PTR;

#ifdef __cplusplus
}
#endif

#endif  //__mcf52xx_cfm_h__
