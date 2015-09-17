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
*   This file contains the type definitions for the mcf5xxx DSPI module.
*
*
*END************************************************************************/


#ifndef __mcf5xxx_dspi_h__
#define __mcf5xxx_dspi_h__

#define __mcf5xxx_dspi_h__version "$Version:3.8.5.0$"
#define __mcf5xxx_dspi_h__date    "$Date:Jun-6-2012$"

#ifdef __cplusplus
extern "C" {
#endif


/*------------------------------------------------------------------------*/
/*
** DSPI  (reg)isters bit set.
*/

/* Bit definitions and macros for MCF5XXX_DSPI_MCR */
#define MCF5XXX_DSPI_MCR_MSTR                       (0x80000000)
#define MCF5XXX_DSPI_MCR_CONT_SCKE                  (0x40000000)
#define MCF5XXX_DSPI_MCR_DCONF_MASK                 (0x30000000)
#define MCF5XXX_DSPI_MCR_FRZ                        (0x08000000)
#define MCF5XXX_DSPI_MCR_MTFE                       (0x04000000)
#define MCF5XXX_DSPI_MCR_PCSSE                      (0x02000000)
#define MCF5XXX_DSPI_MCR_ROOE                       (0x01000000)
#define MCF5XXX_DSPI_MCR_PCSIS7                     (0x00800000)
#define MCF5XXX_DSPI_MCR_PCSIS6                     (0x00400000)
#define MCF5XXX_DSPI_MCR_PCSIS5                     (0x00200000)
#define MCF5XXX_DSPI_MCR_PCSIS4                     (0x00100000)
#define MCF5XXX_DSPI_MCR_PCSIS3                     (0x00080000)
#define MCF5XXX_DSPI_MCR_PCSIS2                     (0x00040000)
#define MCF5XXX_DSPI_MCR_PCSIS1                     (0x00020000)
#define MCF5XXX_DSPI_MCR_PCSIS0                     (0x00010000)
#define MCF5XXX_DSPI_MCR_PCSIS_GET(reg)             (((reg) >> 16) & 0xFF)
#define MCF5XXX_DSPI_MCR_PCSIS_SET(val)             (((val) & 0xFF) << 16)
#define MCF5XXX_DSPI_MCR_MDIS                       (0x00004000)
#define MCF5XXX_DSPI_MCR_DIS_TXF                    (0x00002000)
#define MCF5XXX_DSPI_MCR_DIS_RXF                    (0x00001000)
#define MCF5XXX_DSPI_MCR_CLR_TXF                    (0x00000800)
#define MCF5XXX_DSPI_MCR_CLR_RXF                    (0x00000400)
#define MCF5XXX_DSPI_MCR_SMPL_PT_MASK               (0x00000300)
#define MCF5XXX_DSPI_MCR_SMPL_PT_2                  (0x00000200)
#define MCF5XXX_DSPI_MCR_SMPL_PT_1                  (0x00000100)
#define MCF5XXX_DSPI_MCR_SMPL_PT_0                  (0x00000000)
#define MCF5XXX_DSPI_MCR_HALT                       (0x00000001)

/* Bit definitions and macros for MCF5XXX_DSPI_TCR */
#define MCF5XXX_DSPI_TCR_GET(reg)                   ((reg) >> 16)
#define MCF5XXX_DSPI_TCR_SET(val)                   ((val) << 16)

/* Bit definitions and macros for MCF5XXX_DSPI_CTARn */
#define MCF5XXX_DSPI_CTAR_DBR                       (0x80000000)
#define MCF5XXX_DSPI_CTAR_FMSZ_GET(reg)             ((((reg) >> 27) & 0x0F) + 1)
#define MCF5XXX_DSPI_CTAR_FMSZ_SET(val)             (((val - 1) & 0x0F) << 27)
#define MCF5XXX_DSPI_CTAR_CPOL                      (0x04000000)
#define MCF5XXX_DSPI_CTAR_CPHA                      (0x02000000)
#define MCF5XXX_DSPI_CTAR_LSBFE                     (0x01000000)
#define MCF5XXX_DSPI_CTAR_PCSSCK_MASK               (0x00C00000)
#define MCF5XXX_DSPI_CTAR_PCSSCK_7                  (0x00C00000)
#define MCF5XXX_DSPI_CTAR_PCSSCK_5                  (0x00800000)
#define MCF5XXX_DSPI_CTAR_PCSSCK_3                  (0x00400000)
#define MCF5XXX_DSPI_CTAR_PCSSCK_1                  (0x00000000)
#define MCF5XXX_DSPI_CTAR_PASC_MASK                 (0x00300000)
#define MCF5XXX_DSPI_CTAR_PASC_7                    (0x00300000)
#define MCF5XXX_DSPI_CTAR_PASC_5                    (0x00200000)
#define MCF5XXX_DSPI_CTAR_PASC_3                    (0x00100000)
#define MCF5XXX_DSPI_CTAR_PASC_1                    (0x00000000)
#define MCF5XXX_DSPI_CTAR_PDT_MASK                  (0x000C0000)
#define MCF5XXX_DSPI_CTAR_PDT_7                     (0x000C0000)
#define MCF5XXX_DSPI_CTAR_PDT_5                     (0x00080000)
#define MCF5XXX_DSPI_CTAR_PDT_3                     (0x00040000)
#define MCF5XXX_DSPI_CTAR_PDT_1                     (0x00000000)
#define MCF5XXX_DSPI_CTAR_PBR_MASK                  (0x00030000)
#define MCF5XXX_DSPI_CTAR_PBR_7                     (0x00030000)
#define MCF5XXX_DSPI_CTAR_PBR_5                     (0x00020000)
#define MCF5XXX_DSPI_CTAR_PBR_3                     (0x00010000)
#define MCF5XXX_DSPI_CTAR_PBR_2                     (0x00000000)
#define MCF5XXX_DSPI_CTAR_PCSSCK_GET(reg)           (((reg) >> 22) & 0x03)
#define MCF5XXX_DSPI_CTAR_PCSSCK_SET(val)           (((val) & 0x03) << 22)
#define MCF5XXX_DSPI_CTAR_PASC_GET(reg)             (((reg) >> 20) & 0x03)
#define MCF5XXX_DSPI_CTAR_PASC_SET(val)             (((val) & 0x03) << 20)
#define MCF5XXX_DSPI_CTAR_PDT_GET(reg)              (((reg) >> 18) & 0x03)
#define MCF5XXX_DSPI_CTAR_PDT_SET(val)              (((val) & 0x03) << 18)
#define MCF5XXX_DSPI_CTAR_PBR_GET(reg)              (((reg) >> 16) & 0x03)
#define MCF5XXX_DSPI_CTAR_PBR_SET(val)              (((val) & 0x03) << 16)
#define MCF5XXX_DSPI_CTAR_CSSCK_GET(reg)            (((reg) >> 12) & 0x0F)
#define MCF5XXX_DSPI_CTAR_CSSCK_SET(val)            (((val) & 0x0F) << 12)
#define MCF5XXX_DSPI_CTAR_ASC_GET(reg)              (((reg) >> 8) & 0x0F)
#define MCF5XXX_DSPI_CTAR_ASC_SET(val)              (((val) & 0x0F) << 8)
#define MCF5XXX_DSPI_CTAR_DT_GET(reg)               (((reg) >> 4) & 0x0F)
#define MCF5XXX_DSPI_CTAR_DT_SET(val)               (((val) & 0x0F) << 4)
#define MCF5XXX_DSPI_CTAR_BR_GET(reg)               ((reg) & 0x0F)
#define MCF5XXX_DSPI_CTAR_BR_SET(val)               ((val) & 0x0F)

/* Bit definitions and macros for MCF5XXX_DSPI_SR */
#define MCF5XXX_DSPI_SR_TCF                         (0x80000000)
#define MCF5XXX_DSPI_SR_TXRXS                       (0x40000000)
#define MCF5XXX_DSPI_SR_EOQF                        (0x10000000)
#define MCF5XXX_DSPI_SR_TFUF                        (0x08000000)
#define MCF5XXX_DSPI_SR_TFFF                        (0x02000000)
#define MCF5XXX_DSPI_SR_RFOF                        (0x00080000)
#define MCF5XXX_DSPI_SR_RFDF                        (0x00020000)
#define MCF5XXX_DSPI_SR_TXCTR_GET(reg)              (((reg) >> 12) & 0x0F)
#define MCF5XXX_DSPI_SR_TXCTR_SET(val)              (((val) & 0x0F) << 12)
#define MCF5XXX_DSPI_SR_TXNXTPTR_GET(reg)           (((reg) >> 8) & 0x0F)
#define MCF5XXX_DSPI_SR_TXNXTPTR_SET(val)           (((val) & 0x0F) << 8)
#define MCF5XXX_DSPI_SR_RXCTR_GET(reg)              (((reg) >> 4) & 0x0F)
#define MCF5XXX_DSPI_SR_RXCTR_SET(val)              (((val) & 0x0F) << 4)
#define MCF5XXX_DSPI_SR_POPNXTPTR_GET(reg)          ((reg) & 0x0F)
#define MCF5XXX_DSPI_SR_POPNXTPTR_SET(val)          ((val) & 0x0F)

/* Bit definitions and macros for MCF5XXX_DSPI_RSER */
#define MCF5XXX_DSPI_RSER_TCFRE                     (0x80000000)
#define MCF5XXX_DSPI_RSER_EOQF_RE                   (0x10000000)
#define MCF5XXX_DSPI_RSER_TFUF_RE                   (0x08000000)
#define MCF5XXX_DSPI_RSER_TFFF_RE                   (0x02000000)
#define MCF5XXX_DSPI_RSER_TFFF_DIRS                 (0x01000000)
#define MCF5XXX_DSPI_RSER_RFOF_RE                   (0x00080000)
#define MCF5XXX_DSPI_RSER_RFDF_RE                   (0x00020000)
#define MCF5XXX_DSPI_RSER_RFDF_DIRS                 (0x00010000)

/* Bit definitions and macros for MCF5XXX_DSPI_PUSHR */
#define MCF5XXX_DSPI_PUSHR_CONT                     (0x80000000)
#define MCF5XXX_DSPI_PUSHR_CTAS_GET(reg)            (((reg) >> 28) & 0x07)
#define MCF5XXX_DSPI_PUSHR_CTAS_SET(val)            (((val) & 0x07) << 28)
#define MCF5XXX_DSPI_PUSHR_EOQ                      (0x08000000)
#define MCF5XXX_DSPI_PUSHR_CTCNT                    (0x04000000)
#define MCF5XXX_DSPI_PUSHR_PCS7                     (0x00800000)
#define MCF5XXX_DSPI_PUSHR_PCS6                     (0x00400000)
#define MCF5XXX_DSPI_PUSHR_PCS5                     (0x00200000)
#define MCF5XXX_DSPI_PUSHR_PCS4                     (0x00100000)
#define MCF5XXX_DSPI_PUSHR_PCS3                     (0x00080000)
#define MCF5XXX_DSPI_PUSHR_PCS2                     (0x00040000)
#define MCF5XXX_DSPI_PUSHR_PCS1                     (0x00020000)
#define MCF5XXX_DSPI_PUSHR_PCS0                     (0x00010000)
#define MCF5XXX_DSPI_PUSHR_PCS_GET(reg)             (((reg) >> 16) & 0xFF)
#define MCF5XXX_DSPI_PUSHR_PCS_SET(val)             (((val) & 0xFF) << 16)
#define MCF5XXX_DSPI_PUSHR_TXDATA_GET(reg)          ((reg) & 0xFFFF)
#define MCF5XXX_DSPI_PUSHR_TXDATA_SET(val)          ((val) & 0xFFFF)

/* Bit definitions and macros for MCF5XXX_DSPI_POPR */
#define MCF5XXX_DSPI_POPR_RXDATA_GET(reg)           (((uint16_t)(reg)) & 0xFFFF)

/* Bit definitions and macros for MCF5XXX_DSPI_TXFRn */
#define MCF5XXX_DSPI_TXFR_TXCMD_GET(reg)            (((reg) >> 16) & 0xFFFF)
#define MCF5XXX_DSPI_TXFR_TXDATA_GET(reg)           ((reg) & 0xFFFF)

/* Bit definitions and macros for MCF5XXX_DSPI_RXFRn */
#define MCF5XXX_DSPI_RXFR_RXDATA_GET(reg)           ((reg) & 0xFFFF)


/*
** MCF5XXX_DSPI_STRUCT
** DSPI
*/
typedef struct mcf5xxx_dspi_struct
{
    uint32_t DSPI_MCR;
    uint32_t filler1;
    uint32_t DSPI_TCR;
    uint32_t DSPI_CTAR[8];
    uint32_t DSPI_SR;
    uint32_t DSPI_RSER;
    uint32_t DSPI_PUSHR;
    uint32_t DSPI_POPR;
    uint32_t DSPI_TXFR[16];
    uint32_t DSPI_RXFR[16];
    
} MCF5XXX_DSPI_STRUCT, * MCF5XXX_DSPI_STRUCT_PTR;
typedef volatile struct mcf5xxx_dspi_struct * VMCF5XXX_DSPI_STRUCT_PTR;

#ifdef __cplusplus
}
#endif

#endif
