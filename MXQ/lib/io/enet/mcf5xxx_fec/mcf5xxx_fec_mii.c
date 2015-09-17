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
*   This file contains the MCF5xxx Ethernet MII Interface functions
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
*  Function Name  : MCF5XXX_FEC_mii_enabled
*  Returned Value : bool
*  Comments       :
*    We can only read the PHY registers if the PHY is enabled
*
*END*-----------------------------------------------------------------*/

static bool MCF5XXX_FEC_mii_enabled
   (
      VMCF5XXX_FEC_STRUCT_PTR fec_ptr
   )
{  
   return (fec_ptr->MSCR & 0x7E) != 0;
} 

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : MCF5XXX_FEC_read_write_mii
*  Returned Value : TRUE and MII Data register value, FALSE if timeout
*  Comments       :
*    Return entire MII data register value
*
*END*-----------------------------------------------------------------*/

static bool MCF5XXX_FEC_read_write_mii
   (
      VMCF5XXX_FEC_STRUCT_PTR    fec_ptr,
      unsigned char                      phy_addr,
      uint32_t                    reg_index,
      uint32_t                    op,
      uint32_t                    write_data,
      uint32_t                *read_data_ptr,
      uint32_t                    timeout
   )
{
   uint32_t                    tm;
    
   if (fec_ptr == NULL) 
      return FALSE;
   
   if (!MCF5XXX_FEC_mii_enabled(fec_ptr))
        return FALSE;
    
   // Clear the MII interrupt bit 
   fec_ptr->EIR = MCF5XXX_FEC_EIR_MII;

   // Kick-off the MII read or write operation 
   fec_ptr->MMFR = (volatile uint32_t)(0 
      | MCF5XXX_FEC_MII_START_OF_FRAME
      | op
      | MCF5XXX_FEC_MII_PA(phy_addr)
      | MCF5XXX_FEC_MII_RA(reg_index)
      | MCF5XXX_FEC_MII_TURNAROUND
      | (write_data & 0xffff));

    // Poll for MII complete
   for (tm = 0; tm < timeout; tm++)
   {
      if(fec_ptr->EIR & MCF5XXX_FEC_EIR_MII)
         break;
      _time_delay(0);
   }

   if (tm != timeout) 
      if (read_data_ptr) 
         *read_data_ptr = MCF5XXX_FEC_MII_DATA(fec_ptr->MMFR);

   // Clear the MII interrupt bit 
   fec_ptr->EIR = MCF5XXX_FEC_EIR_MII;

   return (tm != timeout);
}


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : MCF5XXX_FEC_read_mii
*  Returned Value : TRUE and MII Data register value, FALSE if timeout
*  Comments       :
*    Return entire MII data register value
*
*END*-----------------------------------------------------------------*/

bool MCF5XXX_FEC_read_mii
    (
        ENET_CONTEXT_STRUCT_PTR enet_ptr,
        uint32_t reg_index,
        uint32_t *data,
        uint32_t timeout
    )
{
    MCF5XXX_FEC_CONTEXT_STRUCT_PTR fec_context_ptr = (MCF5XXX_FEC_CONTEXT_STRUCT_PTR)enet_ptr->MAC_CONTEXT_PTR;

    return MCF5XXX_FEC_read_write_mii(fec_context_ptr->PHY_PTR, enet_ptr->PHY_ADDRESS, reg_index, MCF5XXX_FEC_MII_OPCODE_READ, 0, data, timeout);
}


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : MCF5XXX_FEC_write_mii
*  Returned Value : TRUE if success, FALSE if timeout
*  Comments       :
*    Write MII data register value
*
*END*-----------------------------------------------------------------*/

bool MCF5XXX_FEC_write_mii
    (
        ENET_CONTEXT_STRUCT_PTR enet_ptr,
        uint32_t reg_index,
        uint32_t data,
        uint32_t timeout
    )
{
    MCF5XXX_FEC_CONTEXT_STRUCT_PTR fec_context_ptr = (MCF5XXX_FEC_CONTEXT_STRUCT_PTR)enet_ptr->MAC_CONTEXT_PTR;

    return MCF5XXX_FEC_read_write_mii(fec_context_ptr->PHY_PTR, enet_ptr->PHY_ADDRESS, reg_index, MCF5XXX_FEC_MII_OPCODE_WRITE, data, NULL, timeout);
}
