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
*   This file contains the definitions of constants and structures
*   required for the ethernet drivers for the MCF5225x processor
*
*
*END************************************************************************/
#ifndef _mcf5225_fec_h
#define _mcf5225_fec_h 1

#define MCF5225_FEC0 0

#define MCF5XXX_FEC_DEVICE_COUNT 1
   
#define MCF5XXX_FEC_RX_BUFFER_ALIGNMENT   16  
#define MCF5XXX_FEC_TX_BUFFER_ALIGNMENT   4   
#define MCF5XXX_FEC_BD_ALIGNMENT          16 // may be 4.

// Needs to be here, because a SMALL packet must be a multiple of the RX_BUFFER_ALIGNMENT
#define MCF5XXX_SMALL_PACKET_SIZE         ENET_ALIGN(64,MCF5XXX_FEC_RX_BUFFER_ALIGNMENT)
       
// temporary - these should be generic PSP functions.
#define _psp_set_int_prio_and_level _mcf52xx_int_init
#define _psp_int_mask(x)            _mcf52xx_int_mask(x)

#endif
/* EOF */
