/*HEADER**********************************************************************
*
* Copyright 2010 Freescale Semiconductor, Inc.
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
*   This file contains the type definitions for the MCF5XXX Ethernet Switch.
*
*
*END************************************************************************/

#ifndef __mcf5xxx_esw_h__
#define __mcf5xxx_esw_h__

#define __mcf5xxx_esw_h__version "$Version:3.6.1.0$"
#define __mcf5xxx_esw_h__date    "$Date:May-31-2010$"

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------*/
/*
** ESW  registers bit set.
*/
/* Bit definitions and macros for MCF5XXX_ESW_REV */
#define MCF5XXX_ESW_REV_CoreRevision(x)          (((x)&0xFFFF)<<0)
#define MCF5XXX_ESW_REV_CustomerRevision(x)      (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF5XXX_ESW_SCR */
#define MCF5XXX_ESW_SCR_SCRATCH(x)               (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF5XXX_ESW_PER */
#define MCF5XXX_ESW_PER_TE0                      (0x1)
#define MCF5XXX_ESW_PER_TE1                      (0x2)
#define MCF5XXX_ESW_PER_TE2                      (0x4)
#define MCF5XXX_ESW_PER_RE0                      (0x10000)
#define MCF5XXX_ESW_PER_RE1                      (0x20000)
#define MCF5XXX_ESW_PER_RE2                      (0x40000)

/* Bit definitions and macros for MCF5XXX_ESW_VLANV */
#define MCF5XXX_ESW_VLANV_VV0                    (0x1)
#define MCF5XXX_ESW_VLANV_VV1                    (0x2)
#define MCF5XXX_ESW_VLANV_VV2                    (0x4)
#define MCF5XXX_ESW_VLANV_DU0                    (0x10000)
#define MCF5XXX_ESW_VLANV_DU1                    (0x20000)
#define MCF5XXX_ESW_VLANV_DU2                    (0x40000)

/* Bit definitions and macros for MCF5XXX_ESW_DBCR */
#define MCF5XXX_ESW_DBCR_P0                      (0x1)
#define MCF5XXX_ESW_DBCR_P1                      (0x2)
#define MCF5XXX_ESW_DBCR_P2                      (0x4)

/* Bit definitions and macros for MCF5XXX_ESW_DMCR */
#define MCF5XXX_ESW_DMCR_P0                      (0x1)
#define MCF5XXX_ESW_DMCR_P1                      (0x2)
#define MCF5XXX_ESW_DMCR_P2                      (0x4)

/* Bit definitions and macros for MCF5XXX_ESW_BKLR */
#define MCF5XXX_ESW_BKLR_BE0                     (0x1)
#define MCF5XXX_ESW_BKLR_BE1                     (0x2)
#define MCF5XXX_ESW_BKLR_BE2                     (0x4)
#define MCF5XXX_ESW_BKLR_LD0                     (0x10000)
#define MCF5XXX_ESW_BKLR_LD1                     (0x20000)
#define MCF5XXX_ESW_BKLR_LD2                     (0x40000)

/* Bit definitions and macros for MCF5XXX_ESW_BMPC */
#define MCF5XXX_ESW_BMPC_PORT(x)                 (((x)&0xF)<<0)
#define MCF5XXX_ESW_BMPC_MSGTX                   (0x20)
#define MCF5XXX_ESW_BMPC_EN                      (0x40)
#define MCF5XXX_ESW_BMPC_DIS                     (0x80)
#define MCF5XXX_ESW_BMPC_PRIORITY(x)             (((x)&0x7)<<0xD)
#define MCF5XXX_ESW_BMPC_PORTMASK(x)             (((x)&0x7)<<0x10)
#define MCF5XXX_ESW_BMPC_PORTMASK_PORT0          (0x10000)
#define MCF5XXX_ESW_BMPC_PORTMASK_PORT1          (0x20000)
#define MCF5XXX_ESW_BMPC_PORTMASK_PORT2          (0x40000)

/* Bit definitions and macros for MCF5XXX_ESW_MODE */
#define MCF5XXX_ESW_MODE_SWRST                   (0x1)
#define MCF5XXX_ESW_MODE_SWEN                    (0x2)
#define MCF5XXX_ESW_MODE_STOP                    (0x80)
#define MCF5XXX_ESW_MODE_CRCTRAN                 (0x100)
#define MCF5XXX_ESW_MODE_P0CT                    (0x200)
#define MCF5XXX_ESW_MODE_STATRST                 (0x80000000)

/* Bit definitions and macros for MCF5XXX_ESW_VIMSEL */
#define MCF5XXX_ESW_VIMSEL_IM0(x)                (((x)&0x3)<<0)
#define MCF5XXX_ESW_VIMSEL_IM0_MODE1             (0)
#define MCF5XXX_ESW_VIMSEL_IM0_MODE2             (0x1)
#define MCF5XXX_ESW_VIMSEL_IM0_MODE3             (0x2)
#define MCF5XXX_ESW_VIMSEL_IM0_MODE4             (0x3)
#define MCF5XXX_ESW_VIMSEL_IM1(x)                (((x)&0x3)<<0x2)
#define MCF5XXX_ESW_VIMSEL_IM1_MODE1             (0)
#define MCF5XXX_ESW_VIMSEL_IM1_MODE2             (0x4)
#define MCF5XXX_ESW_VIMSEL_IM1_MODE3             (0x8)
#define MCF5XXX_ESW_VIMSEL_IM1_MODE4             (0xC)
#define MCF5XXX_ESW_VIMSEL_IM2(x)                (((x)&0x3)<<0x4)
#define MCF5XXX_ESW_VIMSEL_IM2_MODE1             (0)
#define MCF5XXX_ESW_VIMSEL_IM2_MODE2             (0x10)
#define MCF5XXX_ESW_VIMSEL_IM2_MODE3             (0x20)
#define MCF5XXX_ESW_VIMSEL_IM2_MODE4             (0x30)

/* Bit definitions and macros for MCF5XXX_ESW_VOMSEL */
#define MCF5XXX_ESW_VOMSEL_OM0(x)                (((x)&0x3)<<0)
#define MCF5XXX_ESW_VOMSEL_OM0_NO                (0)
#define MCF5XXX_ESW_VOMSEL_OM0_MODE1             (0x1)
#define MCF5XXX_ESW_VOMSEL_OM0_MODE2             (0x2)
#define MCF5XXX_ESW_VOMSEL_OM0_MODE3             (0x3)
#define MCF5XXX_ESW_VOMSEL_OM1(x)                (((x)&0x3)<<0x2)
#define MCF5XXX_ESW_VOMSEL_OM1_NO                (0)
#define MCF5XXX_ESW_VOMSEL_OM1_MODE1             (0x4)
#define MCF5XXX_ESW_VOMSEL_OM1_MODE2             (0x8)
#define MCF5XXX_ESW_VOMSEL_OM1_MODE3             (0xC)
#define MCF5XXX_ESW_VOMSEL_OM2(x)                (((x)&0x3)<<0x4)
#define MCF5XXX_ESW_VOMSEL_OM2_NO                (0)
#define MCF5XXX_ESW_VOMSEL_OM2_MODE1             (0x10)
#define MCF5XXX_ESW_VOMSEL_OM2_MODE2             (0x20)
#define MCF5XXX_ESW_VOMSEL_OM2_MODE3             (0x30)

/* Bit definitions and macros for MCF5XXX_ESW_VIMEN */
#define MCF5XXX_ESW_VIMEN_EN0                    (0x1)
#define MCF5XXX_ESW_VIMEN_EN1                    (0x2)
#define MCF5XXX_ESW_VIMEN_EN2                    (0x4)

/* Bit definitions and macros for MCF5XXX_ESW_VID */
#define MCF5XXX_ESW_VID_TAG(x)                   (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF5XXX_ESW_MCR */
#define MCF5XXX_ESW_MCR_PORT(x)                  (((x)&0xF)<<0)
#define MCF5XXX_ESW_MCR_MEN                      (0x10)
#define MCF5XXX_ESW_MCR_INGMAP                   (0x20)
#define MCF5XXX_ESW_MCR_EGMAP                    (0x40)
#define MCF5XXX_ESW_MCR_INGSA                    (0x80)
#define MCF5XXX_ESW_MCR_INGDA                    (0x100)
#define MCF5XXX_ESW_MCR_EGSA                     (0x200)
#define MCF5XXX_ESW_MCR_EGDA                     (0x400)

/* Bit definitions and macros for MCF5XXX_ESW_EGMAP */
#define MCF5XXX_ESW_EGMAP_EG0                    (0x1)
#define MCF5XXX_ESW_EGMAP_EG1                    (0x2)
#define MCF5XXX_ESW_EGMAP_EG2                    (0x4)

/* Bit definitions and macros for MCF5XXX_ESW_INGMAP */
#define MCF5XXX_ESW_INGMAP_ING0                  (0x1)
#define MCF5XXX_ESW_INGMAP_ING1                  (0x2)
#define MCF5XXX_ESW_INGMAP_ING2                  (0x4)

/* Bit definitions and macros for MCF5XXX_ESW_INGSAL */
#define MCF5XXX_ESW_INGSAL_ADDLOW(x)             (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF5XXX_ESW_INGSAH */
#define MCF5XXX_ESW_INGSAH_ADDHIGH(x)            (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF5XXX_ESW_INGDAL */
#define MCF5XXX_ESW_INGDAL_ADDLOW(x)             (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF5XXX_ESW_INGDAH */
#define MCF5XXX_ESW_INGDAH_ADDHIGH(x)            (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF5XXX_ESW_EGSAL */
#define MCF5XXX_ESW_EGSAL_ADDLOW(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF5XXX_ESW_EGSAH */
#define MCF5XXX_ESW_EGSAH_ADDHIGH(x)             (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF5XXX_ESW_EGDAL */
#define MCF5XXX_ESW_EGDAL_ADDLOW(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF5XXX_ESW_EGDAH */
#define MCF5XXX_ESW_EGDAH_ADDHIGH(x)             (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF5XXX_ESW_MCVAL */
#define MCF5XXX_ESW_MCVAL_COUNT(x)               (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF5XXX_ESW_MMSR */
#define MCF5XXX_ESW_MMSR_BUSY                    (0x1)
#define MCF5XXX_ESW_MMSR_NOCELL                  (0x2)
#define MCF5XXX_ESW_MMSR_MEMFULL                 (0x4)
#define MCF5XXX_ESW_MMSR_MFLATCH                 (0x8)
#define MCF5XXX_ESW_MMSR_DQGRNT                  (0x40)
#define MCF5XXX_ESW_MMSR_CELLS_AVAIL(x)          (((x)&0xFF)<<0x10)

/* Bit definitions and macros for MCF5XXX_ESW_LMT */
#define MCF5XXX_ESW_LMT_THRESH(x)                (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF5XXX_ESW_LFC */
#define MCF5XXX_ESW_LFC_COUNT(x)                 (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF5XXX_ESW_PCSR */
#define MCF5XXX_ESW_PCSR_PC0                     (0x1)
#define MCF5XXX_ESW_PCSR_PC1                     (0x2)
#define MCF5XXX_ESW_PCSR_PC2                     (0x4)

/* Bit definitions and macros for MCF5XXX_ESW_IOSR */
#define MCF5XXX_ESW_IOSR_OR0                     (0x1)
#define MCF5XXX_ESW_IOSR_OR1                     (0x2)
#define MCF5XXX_ESW_IOSR_OR2                     (0x4)
#define MCF5XXX_ESW_IOSR_IR0                     (0x10000)
#define MCF5XXX_ESW_IOSR_IR1                     (0x20000)
#define MCF5XXX_ESW_IOSR_IR2                     (0x40000)

/* Bit definitions and macros for MCF5XXX_ESW_QWT */
#define MCF5XXX_ESW_QWT_Q0WT(x)                  (((x)&0x1F)<<0)
#define MCF5XXX_ESW_QWT_Q1WT(x)                  (((x)&0x1F)<<0x8)
#define MCF5XXX_ESW_QWT_Q2WT(x)                  (((x)&0x1F)<<0x10)
#define MCF5XXX_ESW_QWT_Q3WT(x)                  (((x)&0x1F)<<0x18)

/* Bit definitions and macros for MCF5XXX_ESW_P0BCT */
#define MCF5XXX_ESW_P0BCT_THRESH(x)              (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF5XXX_ESW_FFEN */
#define MCF5XXX_ESW_FFEN_FEN                     (0x1)
#define MCF5XXX_ESW_FFEN_FD(x)                   (((x)&0x3)<<0x2)
#define MCF5XXX_ESW_FFEN_FD_NOT                  (0)
#define MCF5XXX_ESW_FFEN_FD_PORT1                (0x4)
#define MCF5XXX_ESW_FFEN_FD_PORT2                (0x8)
#define MCF5XXX_ESW_FFEN_FD_BOTH                 (0xC)

/* Bit definitions and macros for MCF5XXX_ESW_PSNP */
#define MCF5XXX_ESW_PSNP_EN                      (0x1)
#define MCF5XXX_ESW_PSNP_MODE(x)                 (((x)&0x3)<<0x1)
#define MCF5XXX_ESW_PSNP_MODE_FWD                (0)
#define MCF5XXX_ESW_PSNP_MODE_COPY_FWD           (0x2)
#define MCF5XXX_ESW_PSNP_MODE_DISCARD            (0x4)
#define MCF5XXX_ESW_PSNP_CD                      (0x8)
#define MCF5XXX_ESW_PSNP_CS                      (0x10)
#define MCF5XXX_ESW_PSNP_PORT_COMPARE(x)         (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF5XXX_ESW_IPSNP */
#define MCF5XXX_ESW_IPSNP_EN                     (0x1)
#define MCF5XXX_ESW_IPSNP_MODE(x)                (((x)&0x3)<<0x1)
#define MCF5XXX_ESW_IPSNP_MODE_FWD               (0)
#define MCF5XXX_ESW_IPSNP_MODE_COPY_FWD          (0x2)
#define MCF5XXX_ESW_IPSNP_MODE_DISCARD           (0x4)
#define MCF5XXX_ESW_IPSNP_PROTOCOL(x)            (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF5XXX_ESW_PVRES */
#define MCF5XXX_ESW_PVRES_PRI0(x)                (((x)&0x7)<<0)
#define MCF5XXX_ESW_PVRES_PRI1(x)                (((x)&0x7)<<0x3)
#define MCF5XXX_ESW_PVRES_PRI2(x)                (((x)&0x7)<<0x6)
#define MCF5XXX_ESW_PVRES_PRI3(x)                (((x)&0x7)<<0x9)
#define MCF5XXX_ESW_PVRES_PRI4(x)                (((x)&0x7)<<0xC)
#define MCF5XXX_ESW_PVRES_PRI5(x)                (((x)&0x7)<<0xF)
#define MCF5XXX_ESW_PVRES_PRI6(x)                (((x)&0x7)<<0x12)
#define MCF5XXX_ESW_PVRES_PRI7(x)                (((x)&0x7)<<0x15)

/* Bit definitions and macros for MCF5XXX_ESW_IPRES */
#define MCF5XXX_ESW_IPRES_ADDRESS(x)             (((x)&0xFF)<<0)
#define MCF5XXX_ESW_IPRES_IPV4SEL                (0x100)
#define MCF5XXX_ESW_IPRES_PRI0(x)                (((x)&0x3)<<0x9)
#define MCF5XXX_ESW_IPRES_PRI1(x)                (((x)&0x3)<<0xB)
#define MCF5XXX_ESW_IPRES_PRI2(x)                (((x)&0x3)<<0xD)
#define MCF5XXX_ESW_IPRES_READ                   (0x80000000)

/* Bit definitions and macros for MCF5XXX_ESW_PRES */
#define MCF5XXX_ESW_PRES_VLAN                    (0x1)
#define MCF5XXX_ESW_PRES_IP                      (0x2)
#define MCF5XXX_ESW_PRES_MAC                     (0x4)
#define MCF5XXX_ESW_PRES_DFLT_PRI(x)             (((x)&0x7)<<0x4)

/* Bit definitions and macros for MCF5XXX_ESW_PID */
#define MCF5XXX_ESW_PID_VLANID(x)                (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF5XXX_ESW_VRES */
#define MCF5XXX_ESW_VRES_P0                      (0x1)
#define MCF5XXX_ESW_VRES_P1                      (0x2)
#define MCF5XXX_ESW_VRES_P2                      (0x4)
#define MCF5XXX_ESW_VRES_VLANID(x)               (((x)&0x1FFF)<<0x3)

/* Bit definitions and macros for MCF5XXX_ESW_DISCN */
#define MCF5XXX_ESW_DISCN_COUNT(x)               (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF5XXX_ESW_DISCB */
#define MCF5XXX_ESW_DISCB_COUNT(x)               (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF5XXX_ESW_NDISCN */
#define MCF5XXX_ESW_NDISCN_COUNT(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF5XXX_ESW_NDISCB */
#define MCF5XXX_ESW_NDISCB_COUNT(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF5XXX_ESW_POQC */
#define MCF5XXX_ESW_POQC_COUNT(x)                (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF5XXX_ESW_PMVID */
#define MCF5XXX_ESW_PMVID_COUNT(x)               (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF5XXX_ESW_PMVTAG */
#define MCF5XXX_ESW_PMVTAG_COUNT(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF5XXX_ESW_PBL */
#define MCF5XXX_ESW_PBL_COUNT(x)                 (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF5XXX_ESW_ISR */
#define MCF5XXX_ESW_ISR_EBERR                    (0x1)
#define MCF5XXX_ESW_ISR_RXB                      (0x2)
#define MCF5XXX_ESW_ISR_RXF                      (0x4)
#define MCF5XXX_ESW_ISR_TXB                      (0x8)
#define MCF5XXX_ESW_ISR_TXF                      (0x10)
#define MCF5XXX_ESW_ISR_QM                       (0x20)
#define MCF5XXX_ESW_ISR_OD0                      (0x40)
#define MCF5XXX_ESW_ISR_OD1                      (0x80)
#define MCF5XXX_ESW_ISR_OD2                      (0x100)
#define MCF5XXX_ESW_ISR_LRN                      (0x200)

/* Bit definitions and macros for MCF5XXX_ESW_IMR */
#define MCF5XXX_ESW_IMR_EBERR                    (0x1)
#define MCF5XXX_ESW_IMR_RXB                      (0x2)
#define MCF5XXX_ESW_IMR_RXF                      (0x4)
#define MCF5XXX_ESW_IMR_TXB                      (0x8)
#define MCF5XXX_ESW_IMR_TXF                      (0x10)
#define MCF5XXX_ESW_IMR_QM                       (0x20)
#define MCF5XXX_ESW_IMR_OD0                      (0x40)
#define MCF5XXX_ESW_IMR_OD1                      (0x80)
#define MCF5XXX_ESW_IMR_OD2                      (0x100)
#define MCF5XXX_ESW_IMR_LRN                      (0x200)

/* Bit definitions and macros for MCF5XXX_ESW_RDSR */
#define MCF5XXX_ESW_RDSR_ADDRESS(x)              (((x)&0x3FFFFFFF)<<0x2)

/* Bit definitions and macros for MCF5XXX_ESW_TDSR */
#define MCF5XXX_ESW_TDSR_ADDRESS(x)              (((x)&0x3FFFFFFF)<<0x2)

/* Bit definitions and macros for MCF5XXX_ESW_MRBR */
#define MCF5XXX_ESW_MRBR_SIZE(x)                 (((x)&0x3FF)<<0x4)

/* Bit definitions and macros for MCF5XXX_ESW_RDAR */
#define MCF5XXX_ESW_RDAR_RDAR                    (0x1000000)

/* Bit definitions and macros for MCF5XXX_ESW_TDAR */
#define MCF5XXX_ESW_TDAR_TDAR                    (0x1000000)

/* Bit definitions and macros for MCF5XXX_ESW_LREC0 */
#define MCF5XXX_ESW_LREC0_MAC_ADDR0(x)           (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF5XXX_ESW_LREC1 */
#define MCF5XXX_ESW_LREC1_MAC_ADDR1(x)           (((x)&0xFFFF)<<0)
#define MCF5XXX_ESW_LREC1_HASH(x)                (((x)&0xFF)<<0x10)
#define MCF5XXX_ESW_LREC1_SWPORT(x)              (((x)&0x3)<<0x18)

/* Bit definitions and macros for MCF5XXX_ESW_LSR */
#define MCF5XXX_ESW_LSR_DA                       (0x1)


/*
** MCF5XXX_ESW_STRUCT                                                                   
** This structure defines ESW registers
*/
typedef struct mcf5xxx_esw_struct
{
   uint32_t  ESW_REV;     // Revision
   uint32_t  ESW_SCR;     // Scratch register
   uint32_t  ESW_PER;     // Port enable register
   uint32_t  filler0;
   uint32_t  ESW_VLANV;   // VLAN verify
   uint32_t  ESW_DBCR;    // Default broadcast resolution
   uint32_t  ESW_DMCR;    // Default multicast resolution
   uint32_t  ESW_BKLR;    // Blocking and learning enable
   uint32_t  ESW_BMPC;    // Bridge management port configuration
   uint32_t  ESW_MODE;    // Mode configuration
   uint32_t  ESW_VIMSEL;  // VLAN input manipulation select
   uint32_t  ESW_VOMSEL;  // VLAN output manipulation select
   uint32_t  ESW_VIMEN;   // VLAN input manipulation enable
   uint32_t  ESW_VID;     // VLAN tag ID
   uint32_t  filler1[2];
   uint32_t  ESW_MCR;     // Mirror control register
   uint32_t  ESW_EGMAP;   // Egress port definitions
   uint32_t  ESW_INGMAP;  // Ingress port definitions
   uint32_t  ESW_INGSAL;  // Ingress source MAC address low
   uint32_t  ESW_INGSAH;  // Ingress source MAC address high
   uint32_t  ESW_INGDAL;  // Ingress destination MAC address low
   uint32_t  ESW_INGDAH;  // Ingress destination MAC address high
   uint32_t  ESW_EGSAL;   // Egress source MAC address low
   uint32_t  ESW_EGSAH;   // Egress source MAC address high
   uint32_t  ESW_EGDAL;   // Egress destination MAC address low
   uint32_t  ESW_EGDAH;   // Egress destination MAC address high
   uint32_t  ESW_MCVAL;   // Mirror count value
   uint32_t  filler2[4];
   uint32_t  ESW_MMSR;    // Memory manager status
   uint32_t  ESW_LMT;     // Low memory threshold
   uint32_t  ESW_LFC;     // Lowest number of free cells
   uint32_t  ESW_PCSR;    // Port congestion status
   uint32_t  ESW_IOSR;    // Switch input and output interface status
   uint32_t  ESW_QWT;     // Queue weights
   uint32_t  filler3;
   uint32_t  ESW_P0BCT;   // Por 0 Backpressure Congestion Threshold
   uint32_t  filler4[7];
   uint32_t  ESW_FFEN;    // Port 0 forced forwarding enable
   uint32_t  ESW_PSNP1;   // Port snooping registers 1
   uint32_t  ESW_PSNP2;   // Port snooping registers 2 
   uint32_t  ESW_PSNP3;   // Port snooping registers 3
   uint32_t  ESW_PSNP4;   // Port snooping registers 4 
   uint32_t  ESW_PSNP5;   // Port snooping registers 5
   uint32_t  ESW_PSNP6;   // Port snooping registers 6 
   uint32_t  ESW_PSNP7;   // Port snooping registers 7 
   uint32_t  ESW_PSNP8;   // Port snooping registers 8
   uint32_t  ESW_IPSNP1;  // IP snooping registers 1
   uint32_t  ESW_IPSNP2;  // IP snooping registers 2 
   uint32_t  ESW_IPSNP3;  // IP snooping registers 3
   uint32_t  ESW_IPSNP4;  // IP snooping registers 4
   uint32_t  ESW_IPSNP5;  // IP snooping registers 5
   uint32_t  ESW_IPSNP6;  // IP snooping registers 6
   uint32_t  ESW_IPSNP7;  // IP snooping registers 7
   uint32_t  ESW_IPSNP8;  // IP snooping registers 8
   uint32_t  ESW_P0VRES;  // Port 0 VLAN priority resolution map
   uint32_t  ESW_P1VRES;  // Port 1 VLAN priority resolution map
   uint32_t  ESW_P2VRES;  // Port 2 VLAN priority resolution map
   uint32_t  filler5[13];
   uint32_t  ESW_IPRES;   // IPv4/v6 priority resolution table
   uint32_t  filler6[15];
   uint32_t  ESW_P0RES;   // Port 0 priority resolution configuration
   uint32_t  ESW_P1RES;   // Port 1 priority resolution configuration
   uint32_t  ESW_P2RES;   // Port 2 priority resolution configuration
   uint32_t  filler7[29];
   uint32_t  ESW_P0ID;    // Port 0 VLAN ID
   uint32_t  ESW_P1ID;    // Port 1 VLAN ID
   uint32_t  ESW_P2ID;    // Port 2 VLAN ID
   uint32_t  filler8[29];
   uint32_t  ESW_VRES0;   // VLAN domain resolution entry 0
   uint32_t  ESW_VRES1;   // VLAN domain resolution entry 1
   uint32_t  ESW_VRES2;   // VLAN domain resolution entry 2
   uint32_t  ESW_VRES3;   // VLAN domain resolution entry 3
   uint32_t  ESW_VRES4;   // VLAN domain resolution entry 4
   uint32_t  ESW_VRES5;   // VLAN domain resolution entry 5
   uint32_t  ESW_VRES6;   // VLAN domain resolution entry 6
   uint32_t  ESW_VRES7;   // VLAN domain resolution entry 7
   uint32_t  ESW_VRES8;   // VLAN domain resolution entry 8
   uint32_t  ESW_VRES9;   // VLAN domain resolution entry 9
   uint32_t  ESW_VRES10;  // VLAN domain resolution entry 10
   uint32_t  ESW_VRES11;  // VLAN domain resolution entry 11
   uint32_t  ESW_VRES12;  // VLAN domain resolution entry 12
   uint32_t  ESW_VRES13;  // VLAN domain resolution entry 13
   uint32_t  ESW_VRES14;  // VLAN domain resolution entry 14
   uint32_t  ESW_VRES15;  // VLAN domain resolution entry 15
   uint32_t  ESW_VRES16;  // VLAN domain resolution entry 16
   uint32_t  ESW_VRES17;  // VLAN domain resolution entry 17
   uint32_t  ESW_VRES18;  // VLAN domain resolution entry 18
   uint32_t  ESW_VRES19;  // VLAN domain resolution entry 19
   uint32_t  ESW_VRES20;  // VLAN domain resolution entry 20
   uint32_t  ESW_VRES21;  // VLAN domain resolution entry 21
   uint32_t  ESW_VRES22;  // VLAN domain resolution entry 22
   uint32_t  ESW_VRES23;  // VLAN domain resolution entry 23
   uint32_t  ESW_VRES24;  // VLAN domain resolution entry 24
   uint32_t  ESW_VRES25;  // VLAN domain resolution entry 25
   uint32_t  ESW_VRES26;  // VLAN domain resolution entry 26
   uint32_t  ESW_VRES27;  // VLAN domain resolution entry 27
   uint32_t  ESW_VRES28;  // VLAN domain resolution entry 28
   uint32_t  ESW_VRES29;  // VLAN domain resolution entry 29
   uint32_t  ESW_VRES30;  // VLAN domain resolution entry 30
   uint32_t  ESW_VRES31;  // VLAN domain resolution entry 31
   uint32_t  ESW_DISCN;   // Number of discarded frames
   uint32_t  ESW_DISCB;   // Bytes of discarded frames
   uint32_t  ESW_NDISCN;  // Number of non-discarded frames
   uint32_t  ESW_NDISCB;  // Bytes of non-discarded frames
   uint32_t  ESW_P0OQC;   // Port 0 output queue congestion
   uint32_t  ESW_P0MVID;  // Port 0 mismatching VLAN ID
   uint32_t  ESW_P0MVTAG; // Port 0 missing VLAN tag
   uint32_t  ESW_P0BL;    // Port 0 blocked
   uint32_t  ESW_P1OQC;   // Port 1 output queue congestion
   uint32_t  ESW_P1MVID;  // Port 1 mismatching VLAN ID
   uint32_t  ESW_P1MVTAG; // Port 1 missing VLAN tag
   uint32_t  ESW_P1BL;    // Port 1 blocked
   uint32_t  ESW_P2OQC;   // Port 2 output queue congestion
   uint32_t  ESW_P2MVID;  // Port 2 mismatching VLAN ID
   uint32_t  ESW_P2MVTAG; // Port 2 missing VLAN tag
   uint32_t  ESW_P2BL;    // Port 2 blocked
   uint32_t  filler9[48];
   uint32_t  ESW_ISR;     // Interrupt status register
   uint32_t  ESW_IMR;     // Interrupt mask register
   uint32_t  ESW_RDSR;    // Receive descriptor ring pointer
   uint32_t  ESW_TDSR;    // Transmit descriptor ring pointer
   uint32_t  ESW_MRBR;    // Maximum receive buffer size
   uint32_t  ESW_RDAR;    // Receive descriptor active
   uint32_t  ESW_TDAR;    // Transmit descriptor active
   uint32_t  filler10[57];
   uint32_t  ESW_LREC0;   // Learning records A0 & B1
   uint32_t  ESW_LREC1;   // Learning record B1
   uint32_t  ESW_LSR;     // Learning data available status
} MCF5XXX_ESW_STRUCT, * MCF5XXX_ESW_STRUCT_PTR;
typedef volatile struct mcf5xxx_esw_struct * VMCF5XXX_ESW_STRUCT_PTR;

#ifdef __cplusplus
}
#endif

#endif  //__mcf5xxx_esw_h__
