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
*   This file contains the MCF5xxx Coldfire processor
*   Ethernet initialization
*
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"
#include "enetprv.h"
#include "mcf5xxx_fec_prv.h"          


/*NOTIFIER*-------------------------------------------------------------
*
*  Function Name  : MCF5XXX_FEC_Error_ISR
*  Returned Value : void
*  Comments       :
*        Interrupt service routine for Tx.
*
*END*-----------------------------------------------------------------*/

static void MCF5XXX_FEC_Error_ISR 
   (
         /* [IN] the Ethernet state structure */
      void    *enet
   )
{    
   ENET_CONTEXT_STRUCT_PTR          enet_ptr = (ENET_CONTEXT_STRUCT_PTR)enet;
   MCF5XXX_FEC_CONTEXT_STRUCT_PTR   fec_context_ptr = (MCF5XXX_FEC_CONTEXT_STRUCT_PTR) enet_ptr->MAC_CONTEXT_PTR;
   VMCF5XXX_FEC_STRUCT_PTR          fec_ptr= fec_context_ptr->FEC_ADDRESS;
   uint32_t                          events;
   
   if (fec_ptr == NULL) return;
   
   events = fec_ptr->EIR;
   while (events & MCF5XXX_FEC_EIR_HBERR) {

      /* clear the interrupt */
      fec_ptr->EIR = MCF5XXX_FEC_EIR_HBERR;

      events = fec_ptr->EIR;
   } 
} 

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : MCF5XXX_FEC_install_isr
*  Returned Value : ENET_OK or error code
*  Comments       :
*         
*
*END*-----------------------------------------------------------------*/

static bool MCF5XXX_FEC_install_isr( 
   ENET_CONTEXT_STRUCT_PTR enet_ptr, 
   uint32_t                 int_num, 
   uint32_t                 int_index, 
   INT_ISR_FPTR            isr, 
   uint32_t                 level, 
   uint32_t                 sublevel  ) 
{
   uint32_t  vector = MCF5XXX_FEC_get_vector(enet_ptr->PARAM_PTR->ENET_IF->MAC_NUMBER, int_index);
 
#if BSPCFG_ENET_RESTORE
   MCF5XXX_FEC_CONTEXT_STRUCT_PTR    fec_context_ptr = (MCF5XXX_FEC_CONTEXT_STRUCT_PTR) enet_ptr->MAC_CONTEXT_PTR;


   // Save old ISR and data
   fec_context_ptr->OLDISR_PTR[int_num]   = _int_get_isr(vector);
   fec_context_ptr->OLDISR_DATA[int_num] = _int_get_isr_data(vector);
#endif

   if (_int_install_isr(vector, isr, (void *)enet_ptr)==NULL) {
      return FALSE;
   }

   // Initialize interrupt priority and level
   _psp_set_int_prio_and_level((PSP_INTERRUPT_TABLE_INDEX)vector, level, sublevel, TRUE);

   return TRUE;
}


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : MCF5XXX_FEC_install_isrs
*  Returned Value :  
*  Comments       :
*        
*
*END*-----------------------------------------------------------------*/

bool MCF5XXX_FEC_install_isrs( ENET_CONTEXT_STRUCT_PTR enet_ptr, MCF5XXX_FEC_INIT_STRUCT const * enet_init_ptr  ) 
{
   bool bOK;
   bOK = MCF5XXX_FEC_install_isr(enet_ptr, 0, MCF5XXX_FEC_INT_TX_INTB, MCF5XXX_FEC_TX_ISR, enet_init_ptr->ETX_LEVEL,enet_init_ptr->ETX_SUBLEVEL  ) ;
   if (bOK) {
      bOK = MCF5XXX_FEC_install_isr(enet_ptr, 1, MCF5XXX_FEC_INT_TX_INTF, MCF5XXX_FEC_TX_ISR, enet_init_ptr->ETX_LEVEL,enet_init_ptr->ETX_SUBLEVEL  ) ;
   }
   if (bOK) {
      bOK = MCF5XXX_FEC_install_isr(enet_ptr, 2, MCF5XXX_FEC_INT_RX_INTB, MCF5XXX_FEC_RX_ISR, enet_init_ptr->ERX_LEVEL,enet_init_ptr->ERX_SUBLEVEL  ) ;
   }
   if (bOK) {
      bOK = MCF5XXX_FEC_install_isr(enet_ptr, 3, MCF5XXX_FEC_INT_RX_INTF, MCF5XXX_FEC_RX_ISR, enet_init_ptr->ERX_LEVEL,enet_init_ptr->ERX_SUBLEVEL  ) ;
   }
   
#if !PSP_MQX_CPU_IS_MCF5441X
   if (bOK) {
      bOK = MCF5XXX_FEC_install_isr(enet_ptr, 4, MCF5XXX_FEC_INT_HBERR, MCF5XXX_FEC_Error_ISR, enet_init_ptr->ERX_LEVEL,enet_init_ptr->ERX_SUBLEVEL  ) ;
   }
#endif 

   return bOK;
}

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : MCF5XXX_FEC_mask_interrupts
*  Returned Value :  
*  Comments       :
*        
*
*END*-----------------------------------------------------------------*/

void MCF5XXX_FEC_mask_interrupts( ENET_CONTEXT_STRUCT_PTR enet_ptr ) 
{
   uint32_t  i;
   
   for (i=0;i<MCF5XXX_FEC_NUM_INTS;i++) {
      _psp_int_mask((PSP_INTERRUPT_TABLE_INDEX)(MCF5XXX_FEC_get_vector(enet_ptr->PARAM_PTR->ENET_IF->MAC_NUMBER,i)) );
   }
}


#if BSPCFG_ENET_RESTORE

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : MCF5XXX_FEC_uninstall_isr
*  Returned Value :  
*  Comments       :
*        
*
*END*-----------------------------------------------------------------*/

static void MCF5XXX_FEC_uninstall_isr( ENET_CONTEXT_STRUCT_PTR enet_ptr, uint32_t int_num, uint32_t int_index  ) 
{
   uint32_t  vector = MCF5XXX_FEC_get_vector(enet_ptr->PARAM_PTR->ENET_IF->MAC_NUMBER, int_index);
   MCF5XXX_FEC_CONTEXT_STRUCT_PTR    fec_context_ptr = (MCF5XXX_FEC_CONTEXT_STRUCT_PTR) enet_ptr->MAC_CONTEXT_PTR;
 
   _psp_int_mask((PSP_INTERRUPT_TABLE_INDEX)vector);
   if (fec_context_ptr->OLDISR_PTR[int_num]) {
      _int_install_isr(vector, fec_context_ptr->OLDISR_PTR[int_num], fec_context_ptr->OLDISR_DATA[int_num]);
      fec_context_ptr->OLDISR_PTR[int_num] = (INT_ISR_FPTR)NULL;
   }
}


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : MCF5XXX_FEC_uninstall_all_isrs
*  Returned Value :  
*  Comments       :
*        
*
*END*-----------------------------------------------------------------*/

void MCF5XXX_FEC_uninstall_all_isrs( ENET_CONTEXT_STRUCT_PTR enet_ptr ) 
{
   MCF5XXX_FEC_uninstall_isr(enet_ptr, 0, MCF5XXX_FEC_INT_TX_INTB) ;
   MCF5XXX_FEC_uninstall_isr(enet_ptr, 1, MCF5XXX_FEC_INT_TX_INTF) ;
   MCF5XXX_FEC_uninstall_isr(enet_ptr, 2, MCF5XXX_FEC_INT_RX_INTB) ;
   MCF5XXX_FEC_uninstall_isr(enet_ptr, 3, MCF5XXX_FEC_INT_RX_INTF) ;
   MCF5XXX_FEC_uninstall_isr(enet_ptr, 4, MCF5XXX_FEC_INT_RX_INTF) ;
}
#endif


/*FUNCTION*-------------------------------------------------------------
* 
* Function Name    : MCF5XXX_FEC_get_vector
* Returned Value   : MQX vector number for specified interrupt
* Comments         :
*    This function returns index into MQX interrupt vector table for
*    specified enet interrupt. If not known, returns 0.
*
*END*-----------------------------------------------------------------*/


uint32_t MCF5XXX_FEC_get_vector 
(
    uint32_t device,
    uint32_t vector_index
)
{ 
    uint32_t index = 0;

    if ((device < MCF5XXX_FEC_DEVICE_COUNT) && (vector_index < MCF5XXX_FEC_NUM_INTS)) {
      index = MCF5XXX_FEC_vectors[device][vector_index];
    }

    return index;
} 


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : MCF5XXX_FEC_free_context
*  Returned Value :  
*  Comments       :
*        
*
*END*-----------------------------------------------------------------*/

void MCF5XXX_FEC_free_context( MCF5XXX_FEC_CONTEXT_STRUCT_PTR fec_context_ptr ) 
{
   if (fec_context_ptr) {
      if (fec_context_ptr->UNALIGNED_BUFFERS) {
         _mem_free((void *)fec_context_ptr->UNALIGNED_BUFFERS);
      }
      if (fec_context_ptr->RX_PCB_BASE) {
         _mem_free((void *)fec_context_ptr->RX_PCB_BASE);
      }
      if (fec_context_ptr->TxPCBS_PTR) {
         _mem_free((void *)fec_context_ptr->TxPCBS_PTR);
      }
      if (fec_context_ptr->UNALIGNED_RING_PTR) {
         _mem_free((void *)fec_context_ptr->UNALIGNED_RING_PTR);
      }
   
      _mem_free((void *)fec_context_ptr);
   }
}


/* EOF */
