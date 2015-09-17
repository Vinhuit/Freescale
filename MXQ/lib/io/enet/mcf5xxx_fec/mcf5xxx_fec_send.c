/*HEADER**********************************************************************
*
* Copyright 2008 Freescale Semiconductor, Inc.
* Copyright 2004-2008 Embedded Access Inc.
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
*   This file contains the MCF5xxx Ethernet send
*   support function.
*
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"
#include "enet.h"
#include "enetprv.h"
#include "mcf5xxx_fec_prv.h"          



/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : MCF5XXX_FEC_send
*  Returned Value : ENET_OK or error code
*  Comments       :
*        Sends a packet.
*
*END*-----------------------------------------------------------------*/

uint32_t MCF5XXX_FEC_send
   (
      ENET_CONTEXT_STRUCT_PTR  enet_ptr,
         /* [IN] the Ethernet state structure */
      PCB_PTR              packet,
         /* [IN] the packet to send */
      uint32_t              size,
         /* [IN] total size of the packet */
      uint32_t              frags,
         /* [IN] total fragments in the packet */
      uint32_t              flags
         /* [IN] optional flags, zero = default */
   )
{ 
   MCF5XXX_FEC_CONTEXT_STRUCT_PTR    fec_context_ptr = (MCF5XXX_FEC_CONTEXT_STRUCT_PTR) enet_ptr->MAC_CONTEXT_PTR;
   VMCF5XXX_FEC_STRUCT_PTR           fec_ptr= fec_context_ptr->FEC_ADDRESS;
   PCB_FRAGMENT_PTR                  frag_ptr;
   VMCF5XXX_FEC_BD_STRUCT_PTR        tx_bd_ptr;
   uint32_t                           len,totlen,frag;
   unsigned char                         *txmem;
   bool                           aligned;
   uint32_t                           err = ENET_OK;

   if (fec_ptr == NULL) 
      return ENETERR_INVALID_DEVICE;
   
   FEC_int_disable();
   
   /*
   ** Make sure there aren't too many fragments.  (We really should check
   ** this every time through the previous loop, but it is extremely
   ** unlikely that the fragment counter overflowed -- there would have
   ** to be over 2^32 fragments.)
   */
   if (fec_context_ptr->AvailableTxBDs < 1) {
      ENET_INC_STATS(COMMON.ST_TX_MISSED);
   
      err = ENETERR_SEND_FULL;      
      goto END;
   } 

   #if BSPCFG_ENABLE_ENET_HISTOGRAM
   {
      uint32_t index = size>> ENET_HISTOGRAM_SHIFT;
      
      if (index < ENET_HISTOGRAM_ENTRIES) {
         ENET_INC_STATS(TX_HISTOGRAM[index]);
      }
   }
   #endif
   
   aligned = TRUE;
   for (frag_ptr = packet->FRAG; frag_ptr->LENGTH; frag_ptr++) {
      if (MCF5XXX_FEC_TX_ALIGN((uint32_t)frag_ptr->FRAGMENT)!= (uint32_t)frag_ptr->FRAGMENT)
         aligned = FALSE;
   } 
   if (aligned) {
      ENET_INC_STATS(TX_ALL_ALIGNED);
   }
   
   /*
   ** Enqueue the packet on the transmit ring.  Don't set the ready
   ** bit in the first descriptor until all descriptors are enqueued.
   */
   tx_bd_ptr = &fec_context_ptr->FEC_TX_RING_PTR[fec_context_ptr->NextTxBD];

   frag_ptr = packet->FRAG;
   frag = (uint32_t) frag_ptr->FRAGMENT;
   if (frags > 1 || (MCF5XXX_FEC_TX_ALIGN(frag)!= frag)) {
      // Packet is fragmented and/or it is misaligned, needs to be copied
      txmem = NULL;
      // See if it fits in a small buffer
      if (size <= MCF5XXX_SMALL_PACKET_SIZE) {
         // it does
         txmem = ENET_Dequeue_Buffer((void **) &fec_context_ptr->SMALL_BUFFERS);
      }
      // If it didn't fit, or the small alloc failed, try for a large buffer
      if (txmem) {
         // signal buffer is to be deallocated.
         fec_context_ptr->FREE_TX_SMALL |= (1<<fec_context_ptr->NextTxBD);
         ENET_INC_STATS(ST_TX_COPY_SMALL);

      } else { 
         if (size <=  fec_context_ptr->AlignedTxBufferSize) {

            txmem = ENET_Dequeue_Buffer((void **) &fec_context_ptr->TX_BUFFERS);
         }
         if (txmem) {
            // signal buffer is to be deallocated.
            fec_context_ptr->FREE_TX |= (1<<fec_context_ptr->NextTxBD);
         } else {
            ENET_INC_STATS(COMMON.ST_TX_MISSED);
            
            err = ENETERR_NO_TX_BUFFER;      
            goto END;
         }
         
      }
      totlen = 0;
      
      for (len = frag_ptr->LENGTH; len != 0; len = frag_ptr->LENGTH) {
         _mem_copy(frag_ptr->FRAGMENT, txmem + totlen, len);
         totlen += len;
         frag_ptr++;
      } 

   } else {
      // Packet is not fragmented and it is not misaligned
      totlen = frag_ptr->LENGTH;
      txmem  = frag_ptr->FRAGMENT;
      ENET_INC_STATS(TX_ALIGNED);
   } 

   // Flush the buffer from cache
   _DCACHE_FLUSH_MBYTES(txmem, totlen);  

   // Invalidate the bd from cache
   _DCACHE_INVALIDATE_MBYTES((void *)tx_bd_ptr, sizeof(MCF5XXX_FEC_BD_STRUCT));  

   // set up the tx bd
   tx_bd_ptr->CONTROL &= MCF5XXX_FEC_BD_ETHER_TX_WRAP;
   tx_bd_ptr->BUFFER = txmem;
   tx_bd_ptr->LENGTH = totlen;
   tx_bd_ptr->CONTROL |= (MCF5XXX_FEC_BD_ETHER_TX_LAST | MCF5XXX_FEC_BD_ETHER_TX_SEND_CRC | MCF5XXX_FEC_BD_ETHER_TX_READY);
   
   // Flush the tx bd from cache
   _DCACHE_FLUSH_MBYTES((void *)tx_bd_ptr, sizeof(MCF5XXX_FEC_BD_STRUCT));  

   fec_context_ptr->TxPCBS_PTR[fec_context_ptr->NextTxBD] = packet;
   
   fec_context_ptr->AvailableTxBDs--;
   BD_INC(fec_context_ptr->NextTxBD,fec_context_ptr->NumTxBDs);

   fec_ptr->TDAR = MCF5XXX_FEC_TDAR_ACTIVE;
   
END:
   FEC_int_enable();
      
   return err;
}

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : MCF5XXX_FEC_process_tx_bds
*  Returned Value : void
*  Comments       :
*        Processes transmitted packets.
*
*END*-----------------------------------------------------------------*/

static void MCF5XXX_FEC_process_tx_bds
   (
         /* [IN] the Ethernet state structure */
      ENET_CONTEXT_STRUCT_PTR  enet_ptr
   )
{ 
   MCF5XXX_FEC_CONTEXT_STRUCT_PTR   fec_context_ptr = (MCF5XXX_FEC_CONTEXT_STRUCT_PTR) enet_ptr->MAC_CONTEXT_PTR;
   uint16_t                          tx_status;
   

   /* Dequeue all transmitted frames */
   while (fec_context_ptr->AvailableTxBDs < fec_context_ptr->NumTxBDs) {
      VMCF5XXX_FEC_BD_STRUCT_PTR bd_ptr = &fec_context_ptr->FEC_TX_RING_PTR[fec_context_ptr->LastTxBD];

      _DCACHE_INVALIDATE_MBYTES((void *)bd_ptr, sizeof(MCF5XXX_FEC_BD_STRUCT));  

      if (bd_ptr->CONTROL & MCF5XXX_FEC_BD_ETHER_TX_READY) {
         break;
      } /* Endif */

      fec_context_ptr->TxErrors |= bd_ptr->CONTROL;
      tx_status = fec_context_ptr->TxErrors;

      /* Record statistics for each frame (not each buffer) */
      if (tx_status & MCF5XXX_FEC_BD_ETHER_TX_LAST) {
         PCB_PTR pcb_ptr;
         fec_context_ptr->TxErrors = 0;

         ENET_INC_STATS(COMMON.ST_TX_TOTAL);

         pcb_ptr = fec_context_ptr->TxPCBS_PTR[fec_context_ptr->LastTxBD];
         PCB_free(pcb_ptr);
      } 

      if (fec_context_ptr->FREE_TX_SMALL & (1<<fec_context_ptr->LastTxBD)) {
         ENET_Enqueue_Buffer((void **) &fec_context_ptr->SMALL_BUFFERS,bd_ptr->BUFFER);
         fec_context_ptr->FREE_TX_SMALL &= ~(1<<fec_context_ptr->LastTxBD);
      } else if (fec_context_ptr->FREE_TX & (1<<fec_context_ptr->LastTxBD)) {
         ENET_Enqueue_Buffer((void **) &fec_context_ptr->TX_BUFFERS,bd_ptr->BUFFER);
         fec_context_ptr->FREE_TX &= ~(1<<fec_context_ptr->LastTxBD);
      }      
      BD_INC(fec_context_ptr->LastTxBD,fec_context_ptr->NumTxBDs);
      fec_context_ptr->AvailableTxBDs++;

   } 
} 


/*NOTIFIER*-------------------------------------------------------------
*
*  Function Name  : MCF5XXX_FEC_TX_ISR
*  Returned Value : void
*  Comments       :
*        Interrupt service routine for Tx.
*
*END*-----------------------------------------------------------------*/

void MCF5XXX_FEC_TX_ISR 
   (
         /* [IN] the Ethernet state structure */
      void    *enet
   )
{    
   ENET_CONTEXT_STRUCT_PTR          enet_ptr = (ENET_CONTEXT_STRUCT_PTR)enet;
   MCF5XXX_FEC_CONTEXT_STRUCT_PTR   fec_context_ptr = (MCF5XXX_FEC_CONTEXT_STRUCT_PTR) enet_ptr->MAC_CONTEXT_PTR;
   VMCF5XXX_FEC_STRUCT_PTR          fec_ptr= fec_context_ptr->FEC_ADDRESS;
   uint32_t                 events;

   if (fec_ptr == NULL) return;
   
   events = fec_ptr->EIR;
   while (events & MCF5XXX_FEC_EIR_TXB) {

      /* clear the interrupt */
      fec_ptr->EIR = MCF5XXX_FEC_EIR_TXB;
      MCF5XXX_FEC_process_tx_bds(enet_ptr);

      events = fec_ptr->EIR;
   } 
} 




/* EOF */
