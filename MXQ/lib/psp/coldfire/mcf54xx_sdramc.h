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
*   This file contains the SDRAM controler type definitions for the 
*   mcf54xx processor. It has been split out into a separate header 
*   file to share with other Coldfire PSP's.
*
*
*END************************************************************************/

#ifndef __mcf54xx_sdramc_h__
#define __mcf54xx_sdramc_h__

#define __mcf54xx_sdramc_h__version "$Version:3.0.2.0$"
#define __mcf54xx_sdramc_h__date    "$Date:Jun-11-2009$"

#ifdef __cplusplus
extern "C" {
#endif

#define MCF54XX_SDRAMC_SDMR_DDR2_AD(x)  ((x) & 0x3fff)
#define MCF54XX_SDRAMC_SDMR_CMD         (1 << 16)
#define MCF54XX_SDRAMC_SDMR_AD(x)       (((x) & 0xfff) << 18)
#define MCF54XX_SDRAMC_SDMR_BK(x)       (((x) & 3) << 30)
#define MCF54XX_SDRAMC_SDMR_LMR         0
#define MCF54XX_SDRAMC_SDMR_LEMR        (1 << 30)

#define MCF54XX_SDRAMC_SDCR_DPD             (1)
#define MCF54XX_SDRAMC_SDCR_IPALL           (1 << 1)
#define MCF54XX_SDRAMC_SDCR_IREF            (1 << 2)
#define MCF54XX_SDRAMC_SDCR_DQS_OE(x)       (((x) & 3) << 10)
#define MCF54XX_SDRAMC_SDCR_MEM_PS          (1 << 13)
//#define MCF54XX_SDRAMC_SDCR_PS_16           
#define MCF54XX_SDRAMC_SDCR_REF_CNT(x)      (((x) & 0x2f) << 16)
#define MCF54XX_SDRAMC_SDCR_OE_RULE         (1 << 22)
#define MCF54XX_SDRAMC_SDCR_ADDR_MUX(x)     (((x) & 3) << 24)
#define MCF54XX_SDRAMC_SDCR_DDR2_MODE       (1 << 27)
#define MCF54XX_SDRAMC_SDCR_REF_EN          (1 << 28)
#define MCF54XX_SDRAMC_SDCR_DDR_MODE        (1 << 29)
#define MCF54XX_SDRAMC_SDCR_CKE             (1 << 30)
#define MCF54XX_SDRAMC_SDCR_MODE_EN         (1 << 31)

#define MCF54XX_SDRAMC_SDCFG1_WT_LAT(x)     (((x) & 7) << 4)
#define MCF54XX_SDRAMC_SDCFG1_REF2ACT(x)    (((x) & 0xf) << 8)
#define MCF54XX_SDRAMC_SDCFG1_PRE2ACT(x)    (((x) & 7) << 12)
#define MCF54XX_SDRAMC_SDCFG1_ACT2RW(x)     (((x) & 7) << 16)
#define MCF54XX_SDRAMC_SDCFG1_RD_LAT(x)     (((x) & 0xf) << 20)
#define MCF54XX_SDRAMC_SDCFG1_SWT2RWP(x)    (((x) & 7) << 24)
#define MCF54XX_SDRAMC_SDCFG1_SRD2RWP(x)    (((x) & 0xf) << 28)

#define MCF54XX_SDRAMC_SDCFG2_BL(x)         (((x) & 0x0f) << 16)
#define MCF54XX_SDRAMC_SDCFG2_BRD2W(x)      (((x) & 0x0f) << 20)
#define MCF54XX_SDRAMC_SDCFG2_BWT2RWP(x)    (((x) & 0x0f) << 24)
#define MCF54XX_SDRAMC_SDCFG2_BRD2RP(x)     (((x) & 0x0f) << 28)

/* Bit definitions and macros for SDRAMC_SDCS */
#define MCF54XX_SDRAMC_SDCS_CSSZ(x)         ((x) & 0x1f)
#define MCF54XX_SDRAMC_SDCS_CSSZ_DISABLED   (0)
#define MCF54XX_SDRAMC_SDCS_CSSZ_1MBYTE     (0x13)
#define MCF54XX_SDRAMC_SDCS_CSSZ_2MBYTE     (0x14)
#define MCF54XX_SDRAMC_SDCS_CSSZ_4MBYTE     (0x15)
#define MCF54XX_SDRAMC_SDCS_CSSZ_8MBYTE     (0x16)
#define MCF54XX_SDRAMC_SDCS_CSSZ_16MBYTE    (0x17)
#define MCF54XX_SDRAMC_SDCS_CSSZ_32MBYTE    (0x18)
#define MCF54XX_SDRAMC_SDCS_CSSZ_64MBYTE    (0x19)
#define MCF54XX_SDRAMC_SDCS_CSSZ_128MBYTE   (0x1A)
#define MCF54XX_SDRAMC_SDCS_CSSZ_256MBYTE   (0x1B)
#define MCF54XX_SDRAMC_SDCS_CSSZ_512MBYTE   (0x1C)
#define MCF54XX_SDRAMC_SDCS_CSSZ_1GBYTE     (0x1D)
#define MCF54XX_SDRAMC_SDCS_CSSZ_2GBYTE     (0x1E)
#define MCF54XX_SDRAMC_SDCS_CSSZ_4GBYTE     (0x1F)
#define MCF54XX_SDRAMC_SDCS_CSBA(x)         (((x) & 0xFFF) << 20)
#define MCF54XX_SDRAMC_SDCS_BA(x)           ((x) & 0xFFF00000)


typedef struct mcf54xx_sdramc_struct {
    uint32_t SDMR;  
    uint32_t SDCR;  
    uint32_t SDCFG1;
    uint32_t SDCFG2;
    uint8_t  filler[0x100];
    uint32_t SDCS0; 
    uint32_t SDCS1; 
} MCF54XX_SDRAM_CTRL_STRUCT, * MCF54XX_SDRAM_CTRL_STRUCT_PTR;
typedef volatile struct mcf54xx_sdramc_struct * VMCF54XX_SDRAM_CTRL_STRUCT_PTR;

#ifdef __cplusplus
}
#endif

#endif /* __mcf54xx_sdramc_h__ */
