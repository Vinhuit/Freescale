/*HEADER**********************************************************************
*
* Copyright 2009 Freescale Semiconductor, Inc.
* Copyright 1989-2009 ARC International
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
*   This file contains the type definitions for the ColdFire 32-bit FEC module.
*
*
*END************************************************************************/

#ifndef __mcf5xxx_fec_h__
#define __mcf5xxx_fec_h__

#define __mcf5xxx_fec_h__version "$Version:3.6.4.0$"
#define __mcf5xxx_fec_h__date    "$Date:Jun-4-2010$"

#ifdef __cplusplus
extern "C" {
#endif


/*------------------------------------------------------------------------*/
/*
** ENET FEC  registers bit set.
*/

/* FEC Ethernet Buffer descriptor bits */
#define MCF5XXX_FEC_BD_ETHER_RX_EMPTY                (1<<15)
#define MCF5XXX_FEC_BD_ETHER_RX_WRAP                 (1<<13)

/* The following are status bits set after reception */
#define MCF5XXX_FEC_BD_ETHER_RX_LAST_FRAME           (1<<11)
#define MCF5XXX_FEC_BD_ETHER_RX_MISS                 (1<<8)
#define MCF5XXX_FEC_BD_ETHER_RX_BROADCAST            (1<<7)
#define MCF5XXX_FEC_BD_ETHER_RX_MULTICAST            (1<<6)
#define MCF5XXX_FEC_BD_ETHER_RX_LENGTH_VIOLATION     (1<<5)
#define MCF5XXX_FEC_BD_ETHER_RX_NON_OCTET            (1<<4)
#define MCF5XXX_FEC_BD_ETHER_RX_CRC_ERROR            (1<<2)
#define MCF5XXX_FEC_BD_ETHER_RX_OVERRUN              (1<<1)
#define MCF5XXX_FEC_BD_ETHER_RX_TRUNCATED            (1<<0)

#define MCF5XXX_FEC_BD_ETHER_TX_READY                (1<<15)
#define MCF5XXX_FEC_BD_ETHER_TX_SOFTWARE_OWNERSHIP_1 (1<<14)
#define MCF5XXX_FEC_BD_ETHER_TX_WRAP                 (1<<13)
#define MCF5XXX_FEC_BD_ETHER_TX_SOFTWARE_OWNERSHIP_2 (1<<12)
#define MCF5XXX_FEC_BD_ETHER_TX_LAST                 (1<<11)
#define MCF5XXX_FEC_BD_ETHER_TX_SEND_CRC             (1<<10)
#define MCF5XXX_FEC_BD_ETHER_TX_BAD_CRC              (1<<9)

/* FEC Ethernet Control Register */
#define MCF5XXX_FEC_ECR_ETHER_EN                 (0x00000002)
#define MCF5XXX_FEC_ECR_RESET                    (0x00000001)

/* FEC Interrupt Event Register */
#define MCF5XXX_FEC_EIR_ALL_PENDING              (0xFFF80000)
#define MCF5XXX_FEC_EIR_HBERR                    (0x80000000)
#define MCF5XXX_FEC_EIR_BABR                     (0x40000000)
#define MCF5XXX_FEC_EIR_BABT                     (0x20000000)
#define MCF5XXX_FEC_EIR_GRA                      (0x10000000)
#define MCF5XXX_FEC_EIR_TXF                      (0x08000000)
#define MCF5XXX_FEC_EIR_TXB                      (0x04000000)
#define MCF5XXX_FEC_EIR_RXF                      (0x02000000)
#define MCF5XXX_FEC_EIR_RXB                      (0x01000000)
#define MCF5XXX_FEC_EIR_MII                      (0x00800000)
#define MCF5XXX_FEC_EIR_EBERR                    (0x00400000)
#define MCF5XXX_FEC_EIR_LC                       (0x00200000)
#define MCF5XXX_FEC_EIR_RL                       (0x00100000)
#define MCF5XXX_FEC_EIR_UN                       (0x00080000)

/* FEC Interrupt Mask Register */
#define MCF5XXX_FEC_EIMR_MASK_ALL                (0xFFF80000)
#define MCF5XXX_FEC_EIMR_HBERR                   (0x80000000)
#define MCF5XXX_FEC_EIMR_BABR                    (0x40000000)
#define MCF5XXX_FEC_EIMR_BABT                    (0x20000000)
#define MCF5XXX_FEC_EIMR_GRA                     (0x10000000)
#define MCF5XXX_FEC_EIMR_TXF                     (0x08000000)
#define MCF5XXX_FEC_EIMR_TXB                     (0x04000000)
#define MCF5XXX_FEC_EIMR_RXF                     (0x02000000)
#define MCF5XXX_FEC_EIMR_RXB                     (0x01000000)
#define MCF5XXX_FEC_EIMR_MII                     (0x00800000)
#define MCF5XXX_FEC_EIMR_EBERR                   (0x00400000)
#define MCF5XXX_FEC_EIMR_LC                      (0x00200000)
#define MCF5XXX_FEC_EIMR_RL                      (0x00100000)
#define MCF5XXX_FEC_EIMR_UN                      (0x00080000)


/* FEC Receive Descriptor Active Register */
#define MCF5XXX_FEC_RDAR_ACTIVE                  (1<<24)

/* FEC Transmit Descriptor Active Register */
#define MCF5XXX_FEC_TDAR_ACTIVE                  (1<<24)

/* FEC Receive Control Register */
#define MCF5XXX_FEC_RCR_RMII_10T                 (1 << 9)
#define MCF5XXX_FEC_RCR_RMII_MODE                (1 << 8)

#define MCF5XXX_FEC_RCR_FCE                      (0x00000020)
#define MCF5XXX_FEC_RCR_BC_REJ                   (0x00000010)
#define MCF5XXX_FEC_RCR_PROM                     (0x00000008)
#define MCF5XXX_FEC_RCR_MII_MODE                 (0x00000004)
#define MCF5XXX_FEC_RCR_DRT                      (0x00000002)
#define MCF5XXX_FEC_RCR_LOOP                     (0x00000001)
#define MCF5XXX_FEC_RCR_FRAME_LENGTH(x)          (((x)&0x7ff)<<16)

/* FEC Transmit Control Register */
#define MCF5XXX_FEC_TCR_FDEN                     (0x00000004)
#define MCF5XXX_FEC_TCR_HBC                      (0x00000002)
#define MCF5XXX_FEC_TCR_GTS                      (0x00000001)

/* MII Management Frame Register */
#define MCF5XXX_FEC_MII_START_OF_FRAME           (0x40000000)
#define MCF5XXX_FEC_MII_OPCODE_READ              (0x20000000)
#define MCF5XXX_FEC_MII_OPCODE_WRITE             (0x10000000)
#define MCF5XXX_FEC_MII_PA(x)                    (((x)&0x1F)<<23)
#define MCF5XXX_FEC_MII_RA(x)                    (((x)&0x1F)<<18)
#define MCF5XXX_FEC_MII_DATA(x)                  (((x)&0xFFFF))
#define MCF5XXX_FEC_MII_TURNAROUND               (0x00020000)
#define MCF5XXX_FEC_MII_MSCR_PREAMBLE            (1<<7)
#define MCF5XXX_FEC_MII_MSCR_SPEED(x)            (((x) 0x3F) << 1)

#define MCF5XXX_FEC_SIM_ICR_ERX_IL(a)                ( 0x00000800 | (((a)&0x07)<<8) )
#define MCF5XXX_FEC_SIM_ICR_ETX_IL(a)                ( 0x00000080 | (((a)&0x07)<<4) )
#define MCF5XXX_FEC_SIM_ICR_ENTC_IL(a)               ( 0x00000008 | ((a)&0x07) )


#define MCF5XXX_FEC_INT_TX_INTF 0       // Transmit frame interrupt
#define MCF5XXX_FEC_INT_TX_INTB 1       // Transmit buffer interrupt
#define MCF5XXX_FEC_INT_TX_UN   2       // Transmit FIFO underrun
#define MCF5XXX_FEC_INT_RL      3       // Collision retry limit
#define MCF5XXX_FEC_INT_RX_INTF 4       // Receive frame interrupt
#define MCF5XXX_FEC_INT_RX_INTB 5       // Receive buffer interrupt
#define MCF5XXX_FEC_INT_MII     6       // MII interrupt
#define MCF5XXX_FEC_INT_LC      7       // Late collision
#define MCF5XXX_FEC_INT_HBERR   8       // Heartbeat error
#define MCF5XXX_FEC_INT_GRA     9       // Graceful stop complete
#define MCF5XXX_FEC_INT_EBERR   10      // Ethernet bus error
#define MCF5XXX_FEC_INT_BABT    11      // Babbling transmit error
#define MCF5XXX_FEC_INT_BABR    12      // Babbling receive error
#define MCF5XXX_FEC_NUM_INTS    13      


/*
** MCF5XXX_FEC_STRUCT
** Fast Ethernet Controller registers
*/
typedef struct mcf5xxx_fec_struct
{
   unsigned char    filler1[4];
   uint32_t  EIR;             // Ethernet interrupt event register
   uint32_t  EIMR;            // Ethernet interrupt mask register
   uint32_t  removed_IVSR;    // does not exist - Ethernet interrupt vector status
   uint32_t  RDAR;            // Ethernet Rx ring updated flag
   uint32_t  TDAR;            // Ethernet Tx ring updated flag
   unsigned char    filler2[12];
   uint32_t  ECR;             // Ethernet control register
   unsigned char    filler3[24];
#define MMFR MDATA
   uint32_t  MDATA;           // Ethernet MII data register
   uint32_t  MSCR;            // Ethernet MII speed register
   unsigned char    filler4[28];
   uint32_t  MIBC;            // MIB control/status register
   unsigned char    filler5[28];
   uint32_t  RCR;             // Ethernet Rx control register
   unsigned char    filler6[60];
   uint32_t  TCR;             // Ethernet Tx control register
   unsigned char    filler7[28];
   uint32_t  PALR;            // Physical address low
   uint32_t  PAUR;            // Physical address high + type field
   uint32_t  OPD;             // Opcode + pause duration
   unsigned char    filler8[40];
   uint32_t  IAUR;            // Upper 32-bits of individual hash table
   uint32_t  IALR;            // Lower 32-bits of individual hash table
   uint32_t  GAUR;            // Upper 32-bits of group hash table
   uint32_t  GALR;            // Lower 32-bits of group hash table
   unsigned char    filler9[28];
   uint32_t  TFWR;            // Transmit FIFO watermark
   unsigned char    filler10[4];
   uint32_t  FRBR;            // Ethernet receive bound register
   uint32_t  FRSR;            // Ethernet Rx FIFO start address
   unsigned char    filler11[44];
   uint32_t  ERDSR;           // Ethernet Rx descriptor ring
   uint32_t  ETDSR;           // Ethernet Tx descriptor ring
   uint32_t  EMRBR;           // Ethernet Rx buffer size
   unsigned char    filler12[116];
   unsigned char    MIB_RAM[512];      // Ram to store FEC counters
   unsigned char    filler13[1024]; // FEC FIFO memory
} MCF5XXX_FEC_STRUCT, * MCF5XXX_FEC_STRUCT_PTR;
typedef volatile struct mcf5xxx_fec_struct * VMCF5XXX_FEC_STRUCT_PTR;

/*
** MCF5XXX_FEC_BD_STRUCT
** This structure defines what the Ethernet buffer descriptor looks like
*/
typedef struct mcf5xxx_fec_bd_struct
{
   uint16_t      CONTROL;
   uint16_t      LENGTH;
   unsigned char    *BUFFER;
} MCF5XXX_FEC_BD_STRUCT, * MCF5XXX_FEC_BD_STRUCT_PTR;
typedef volatile struct mcf5xxx_fec_bd_struct * VMCF5XXX_FEC_BD_STRUCT_PTR;

#ifdef __cplusplus
}
#endif


#endif
