/*HEADER**********************************************************************
*
* Copyright 2009 Freescale Semiconductor, Inc.
* Copyright 2004-2009 Embedded Access Inc.
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
*   This file contains the definitions of constants and structures
*   required for the ethernet drivers for the ColdFire 32-bit FEC 
*
*
*END************************************************************************/
#ifndef _mcf5xxx_fec_prv_h
#define _mcf5xxx_fec_prv_h 1

//#define MCF5XXX_FEC_MII_TIMEOUT         (0x10000)
#define MCF5XXX_FEC_RESET_TIMEOUT       (0x10000)

#define MCF5XXX_FEC_RX_ALIGN(n) ENET_ALIGN(n,MCF5XXX_FEC_RX_BUFFER_ALIGNMENT)
#define MCF5XXX_FEC_TX_ALIGN(n) ENET_ALIGN(n,MCF5XXX_FEC_TX_BUFFER_ALIGNMENT)
#define MCF5XXX_FEC_BD_ALIGN(n) ENET_ALIGN(n,MCF5XXX_FEC_BD_ALIGNMENT)

#define MCF5XXX_NUM_ISRS      5

#define MCF5XXX_MAX_FRAGS     6     // Assumes RX_BUFFER_SIZE >= 256, so that 256*6=1536, set higher for smaller RX_BUFFER_SIZE

#define FEC_int_disable()       _int_disable()
#define FEC_int_enable()        _int_enable()
                                 


// Set ENETCFG_SUPPORT_FRAGMENTED_PCBS to 1 if the upper layer can support receive PCBs with multiple
// fragments.  NOTE: RTCS currently does not support this.
#ifndef ENETCFG_SUPPORT_FRAGMENTED_PCBS
   #define ENETCFG_SUPPORT_FRAGMENTED_PCBS 0
#endif

#define BD_INC(index,maxnum)   if (++index == maxnum) index = 0


#ifdef __cplusplus
extern "C" {
#endif

typedef uint16_t bd_index_t;


#if ENETCFG_SUPPORT_FRAGMENTED_PCBS

   typedef struct mcf5xxx_fec_rx_pcb
   {
      PCB            _FRAG0;
      PCB_FRAGMENT   _FRAGS[MCF5XXX_MAX_FRAGS];
      PCB_FRAGMENT   _FREE_FRAG;
   } MCF5XXX_FEC_RX_PCB, * MCF5XXX_FEC_RX_PCB_PTR;
#else
   typedef PCB2 MCF5XXX_FEC_RX_PCB, * MCF5XXX_FEC_RX_PCB_PTR;
#endif



/* The Ethernet initialization structure */
typedef struct {
   uint32_t            BUS_CLOCK;     
   _int_level         ERX_LEVEL;      // Receive interrupt level
   _int_priority      ERX_SUBLEVEL;   // Receive interrupt sublevel
   _int_level         ETX_LEVEL;      // Transmit interrupt level
   _int_priority      ETX_SUBLEVEL;   // Transmit interrupt sublevel
} MCF5XXX_FEC_INIT_STRUCT, * MCF5XXX_FEC_INIT_STRUCT_PTR;


typedef struct mcf_fec_context_struct {
   VMCF5XXX_FEC_STRUCT_PTR FEC_ADDRESS;
   VMCF5XXX_FEC_STRUCT_PTR PHY_PTR;

   uint16_t                 AlignedRxBufferSize;
   uint16_t                 AlignedTxBufferSize;
   
   /*
   **    The Receive-Side State
   **
   ** RxPCBHead and RxPCBTail are the head and tail of a linked list
   ** of available PCBs for receiving packets.  RxEntries is
   ** the length of this list.
   **
   ** NextRxBD points to the 'first' queued receive descriptor, i.e., usually
   ** the one that RBPTR is pointing to.  When an RXF interrupt occurs,
   ** NextRxBD is pointing to the first descriptor with a received packet.
   ** The packets is forwarded to the upper layers, and NextRxBD is incremented.
   **
   ** LastRxBD points to the 'last' queued receive descriptor, i.e., the last
   ** one with E=1.  The LastRxBD descriptor usually precedes the LastRxBD descriptor.
   ** When the upper layers free a received packet, it gets placed at LastRxBD
   ** (with E=1), and LastRxBD is incremented.
   */

   PCB_PTR                       RxPCBHead;
   PCB_PTR                       RxPCBTail;
   
   uint32_t                       CurrentRxFrag;
   ENET_ECB_STRUCT_PTR           CurrentRxECB;
#if ENETCFG_SUPPORT_FRAGMENTED_PCBS
   PCB_PTR                       CurrentRxPCB;     // The PCB that is currently being built
   bool                       RxDiscarding;
#else
   PCB_FRAGMENT                  FRAGS[MCF5XXX_MAX_FRAGS];
#endif


   bd_index_t                    ActiveRxBDs;
   bd_index_t                    NextRxBD;
   bd_index_t                    LastRxBD;
   bd_index_t                    NumRxBDs;

   /*
   **    The Transmit-Side State
   **
   ** TxPCBS_PTR is an array of pointers to the PCBs queued on the transmit ring.
   ** AvailableTxBDs is the number of free entries on the ring.
   **
   ** NextTxBD points to the first available transmit descriptor (when AvailableTxBDs>0),
   ** or to LastTxBD (when AvailableTxBDs==NumTxBDs).  It is the descriptor that will next
   ** be queued onto the transmit ring, as soon as there's a packet to send
   ** and room on the transmit ring.
   **
   ** LastTxBD points to the 'last' queued transmit descriptor, i.e., the one
   ** that TBPTR is pointing to.  When a TXB interrupt occurs, LastTxBD is
   ** pointing to the transmitted buffer.  The buffer is freed, and LastTxBD is
   ** incremented.
   **
   ** TxErrors keeps track of all the errors that occur during transmission
   ** of a frame comprised of multiple buffers.
   */

   PCB_PTR *                     TxPCBS_PTR;
   bd_index_t                    AvailableTxBDs;
   bd_index_t                    NextTxBD;
   bd_index_t                    LastTxBD;
   bd_index_t                    NumTxBDs;
   uint32_t                       TxErrors;



   MCF5XXX_FEC_RX_PCB_PTR        RX_PCB_BASE;
   void                         *UNALIGNED_RING_PTR;        
   VMCF5XXX_FEC_BD_STRUCT_PTR    FEC_RX_RING_PTR; 
   VMCF5XXX_FEC_BD_STRUCT_PTR    FEC_TX_RING_PTR; 
   void                         *UNALIGNED_BUFFERS;  
   unsigned char                     *RX_BUFFERS;  
   unsigned char                     *TX_BUFFERS;  
   unsigned char                     *SMALL_BUFFERS;  
   unsigned char                     *LARGE_BUFFERS;  
   uint32_t                       FREE_TX;
   uint32_t                       FREE_TX_SMALL;
   
#if BSPCFG_ENET_RESTORE
   /* These fields are kept only for ENET_shutdown() */
   INT_ISR_FPTR                  OLDISR_PTR[MCF5XXX_NUM_ISRS];
   void                         *OLDISR_DATA[MCF5XXX_NUM_ISRS];
#endif
      
} MCF5XXX_FEC_CONTEXT_STRUCT, * MCF5XXX_FEC_CONTEXT_STRUCT_PTR;


extern const   MCF5XXX_FEC_INIT_STRUCT MCF5XXX_FEC_device[];
extern const   ENET_MAC_IF_STRUCT MCF5xxx_FEC_IF;
extern const   uint32_t MCF5XXX_FEC_vectors[MCF5XXX_FEC_DEVICE_COUNT][MCF5XXX_FEC_NUM_INTS];

extern bool MCF5XXX_FEC_install_isrs( ENET_CONTEXT_STRUCT_PTR enet_ptr, MCF5XXX_FEC_INIT_STRUCT const * enet_init_ptr  ); 
extern void    MCF5XXX_FEC_add_buffers_to_rx_ring(MCF5XXX_FEC_CONTEXT_STRUCT_PTR fec_context_ptr);
extern uint32_t MCF5XXX_FEC_initialize(ENET_CONTEXT_STRUCT_PTR);
extern uint32_t MCF5XXX_FEC_shutdown( ENET_CONTEXT_STRUCT_PTR );
extern uint32_t MCF5XXX_FEC_send( ENET_CONTEXT_STRUCT_PTR, PCB_PTR packet, uint32_t size, uint32_t frags, uint32_t flags );
extern uint32_t MCF5XXX_FEC_join( ENET_CONTEXT_STRUCT_PTR, ENET_MCB_STRUCT_PTR mcb_ptr );
extern uint32_t MCF5XXX_FEC_rejoin( ENET_CONTEXT_STRUCT_PTR );
extern void    MCF5XXX_FEC_mask_interrupts( ENET_CONTEXT_STRUCT_PTR enet_ptr ); 
extern void    MCF5XXX_FEC_uninstall_all_isrs( ENET_CONTEXT_STRUCT_PTR enet_ptr );
extern void    MCF5XXX_FEC_TX_ISR(void *);
extern void    MCF5XXX_FEC_RX_ISR(void *);
extern void   *MCF5XXX_FEC_get_base_address (uint32_t);
extern uint32_t MCF5XXX_FEC_get_vector(uint32_t,uint32_t);
extern void    MCF5XXX_FEC_io_init(uint32_t);
extern void    MCF5XXX_FEC_free_context( MCF5XXX_FEC_CONTEXT_STRUCT_PTR fec_context_ptr );

bool MCF5XXX_FEC_read_mii(ENET_CONTEXT_STRUCT_PTR enet_ptr, uint32_t reg_index, uint32_t *data, uint32_t timeout);
bool MCF5XXX_FEC_write_mii(ENET_CONTEXT_STRUCT_PTR enet_ptr, uint32_t reg_index, uint32_t data, uint32_t timeout);

#ifdef __cplusplus
}
#endif

#endif
/* EOF */

