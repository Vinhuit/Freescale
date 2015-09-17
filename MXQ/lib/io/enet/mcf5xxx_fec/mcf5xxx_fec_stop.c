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
*   This file contains the MCF5xxx Ethernet shutdown
*   functions.
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
*  Function Name  : MCF5XXX_FEC_shutdown
*  Returned Value : ENET_OK or error code
*  Comments       :
*        Stops the chip.
*
*END*-----------------------------------------------------------------*/

uint32_t MCF5XXX_FEC_shutdown
   (
      ENET_CONTEXT_STRUCT_PTR  enet_ptr
         /* [IN] the Ethernet state structure */
   )
{ /* Body */
   MCF5XXX_FEC_CONTEXT_STRUCT_PTR    fec_context_ptr = (MCF5XXX_FEC_CONTEXT_STRUCT_PTR) enet_ptr->MAC_CONTEXT_PTR;
   VMCF5XXX_FEC_STRUCT_PTR           fec_ptr= fec_context_ptr->FEC_ADDRESS;

   if (fec_ptr == NULL) return ENETERR_INVALID_DEVICE;

   /* Stop the chip */
   fec_ptr->ECR = MCF5XXX_FEC_ECR_RESET;

   /* Disable all FEC interrupts */
   fec_ptr->EIMR = 0;

   /* clear any pending interrpts */
   fec_ptr->EIR = MCF5XXX_FEC_EIR_ALL_PENDING;

   /* Stop the FEC from interrupting the core */
    MCF5XXX_FEC_mask_interrupts( enet_ptr );

   #if BSPCFG_ENET_RESTORE
      /* Restore old ISRs */
      MCF5XXX_FEC_uninstall_all_isrs( enet_ptr );
   #endif

    /*
    ** Make sure all PCBs are free
    */
    if (fec_context_ptr->ActiveRxBDs != fec_context_ptr->NumRxBDs) {
       return ENETERR_FREE_PCB;
    } 

   MCF5XXX_FEC_free_context(fec_context_ptr);
   return ENET_OK;

} 


/* EOF */
