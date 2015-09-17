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
*   Processor family specific file needed for enet module.
*
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <enet.h>
#include <enetprv.h>
#include <macnet_prv.h>

#if PSP_MQX_CPU_IS_VYBRID_A5
const uint32_t MACNET_vectors[MACNET_DEVICE_COUNT][ENET_NUM_INTS] = {
    {
    	GIC_ENET_MAC0,
    },
    {
    	GIC_ENET_MAC1,
    }
};
#elif PSP_MQX_CPU_IS_VYBRID_M4
const uint32_t MACNET_vectors[MACNET_DEVICE_COUNT][ENET_NUM_INTS] = {
    {
        NVIC_ENET_MAC0,
		
    },
    {
        NVIC_ENET_MAC1,
    }
};
#endif

#if ENETCFG_SUPPORT_PTP
extern uint64_t  MACNET_PTP_seconds;
#endif /* ENETCFG_SUPPORT_PTP */

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : MACNET_get_base_address  
* Returned Value   : Pointer to desired enem device or NULL if not present
* Comments         :
*    This function returns pointer to base address of address space of the 
*    desired enet device. Returns NULL otherwise.
*
*END*----------------------------------------------------------------------*/

void *MACNET_get_base_address(uint32_t device) {
    void   *addr = NULL;
    
    switch (device) {
    case 0:
        addr = (void *)ENET0_BASE_PTR;
        break;
    case 1:
        addr = (void *)ENET1_BASE_PTR;
        break;
    }
    
    return addr;
}


 
/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : MACNET_io_init
* Returned Value   : none
* Comments         :
*    This function performs BSP-specific initialization related to ENET
*
*END*----------------------------------------------------------------------*/

void MACNET_io_init( uint32_t	device )
{

   if (device >= MACNET_DEVICE_COUNT) 
      return;

   _bsp_enet_io_init(device);
}

#if ENETCFG_SUPPORT_PTP
/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : MACNET_ptp_install_ts_avail_isr
* Returned Value   : none
* Comments         :
*    This function initializes the TS_AVAIL interrupt
*
*END*----------------------------------------------------------------------*/
#if 0
bool MACNET_ptp_install_ts_avail_isr( ENET_CONTEXT_STRUCT_PTR enet_ptr, MACNET_INIT_STRUCT const * enet_init_ptr)
{
   return (MACNET_install_isr(enet_ptr, 4, ENET_INT_TS_AVAIL, MACNET_ptp_store_txstamp, enet_init_ptr->ERX_LEVEL,enet_init_ptr->ERX_SUBLEVEL)) ;
}

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : MACNET_ptp_install_ts_timer_isr
* Returned Value   : none
* Comments         :
*    This function initializes the TS_TIMER interrupt
*
*END*----------------------------------------------------------------------*/

bool MACNET_ptp_install_ts_timer_isr( ENET_CONTEXT_STRUCT_PTR enet_ptr, MACNET_INIT_STRUCT const * enet_init_ptr)
{
   return (MACNET_install_isr(enet_ptr, 5, ENET_INT_TS_TIMER, MACNET_ptp_increment_seconds, enet_init_ptr->ETX_LEVEL - 1, 0)) ;
}
#endif
/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : MACNET_ptp_increment_seconds
*  Returned Value :  
*  Comments       : 
*    This function is the TS_TIMER interrupt service routine
*
*END*-----------------------------------------------------------------*/
void MACNET_ptp_increment_seconds
   (
         /* [IN] the Ethernet state structure */
      void    *enet
   )
{ /* Body */
   ENET_CONTEXT_STRUCT_PTR    enet_ptr = (ENET_CONTEXT_STRUCT_PTR)enet;
   MACNET_CONTEXT_STRUCT_PTR  macnet_context_ptr = (MACNET_CONTEXT_STRUCT_PTR) enet_ptr->MAC_CONTEXT_PTR;
   ENET_MemMapPtr             macnet_ptr= macnet_context_ptr->MACNET_ADDRESS;

    if (macnet_ptr == NULL) return;

    while(macnet_ptr->EIR & ENET_EIR_TS_TIMER_MASK){
   	
        /* Clear the TS_TIMER interrupt */
        macnet_ptr->EIR = ENET_EIR_TS_TIMER_MASK;   
        /* Increment seconds */
        MACNET_PTP_seconds++;
    }
}

#endif /* ENETCFG_SUPPORT_PTP */
