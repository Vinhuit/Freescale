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
*   Processor family specific file needed for enet module.
*   Revision History:
*   Date             Version  Changes
*   ---------        -------  -------
*   Sep 26/08        2.50     Initial version
*
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <enet.h>
#include <enetprv.h>
#include <mcf5xxx_fec_prv.h>


const uint32_t MCF5XXX_FEC_vectors[MCF5XXX_FEC_DEVICE_COUNT][MCF5XXX_FEC_NUM_INTS] = {
    {
        MCF5225_INT_FEC_X_INTF,
        MCF5225_INT_FEC_X_INTB,
        MCF5225_INT_FEC_X_UN,
        MCF5225_INT_FEC_RL,
        MCF5225_INT_FEC_R_INTF,
        MCF5225_INT_FEC_R_INTB,
        MCF5225_INT_FEC_MII,
        MCF5225_INT_FEC_LC,
        MCF5225_INT_FEC_HBERR,
        MCF5225_INT_FEC_GRA,
        MCF5225_INT_FEC_EBERR,
        MCF5225_INT_FEC_BABT,
        MCF5225_INT_FEC_BABR
    }
};

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : MCF5XXX_FEC_get_base_address  
* Returned Value   : Pointer to desired enem device or NULL if not present
* Comments         :
*    This function returns pointer to base address of address space of the 
*    desired enet device. Returns NULL otherwise.
*
*END*----------------------------------------------------------------------*/

void *MCF5XXX_FEC_get_base_address( uint32_t   device )
{ 
    if (device >= MCF5XXX_FEC_DEVICE_COUNT) return NULL;
    return (void *)(&((VMCF5225_STRUCT_PTR)_PSP_GET_IPSBAR())->FEC);
}


 
/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : MCF5XXX_FEC_io_init
* Returned Value   : none
* Comments         :
*    This function performs BSP-specific initialization related to ENET
*
*END*----------------------------------------------------------------------*/

void MCF5XXX_FEC_io_init( uint32_t   device )
{

   if (device >= MCF5XXX_FEC_DEVICE_COUNT) 
      return;

   _bsp_enet_io_init(device);
}
