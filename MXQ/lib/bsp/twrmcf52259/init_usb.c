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
*   This file contains board-specific USB initialization functions.
*
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"
#include "bsp_prv.h"

/* This value is taken from linker file. We cannot use linker value, so we have to define it here.
** This approach is not clean, however it works if the IPSBAR is not changed during runtime.
*/
#undef  BSP_IPSBAR
#define BSP_IPSBAR (0x40000000)
const struct usb_khci_dev_init_struct _khci0_dev_init_param = {
    (void *)(&((VMCF5225_STRUCT_PTR)BSP_IPSBAR)->USBOTG),
    MCF5225_INT_USB, //vector
    3,  //priority
    16  //max. endpoints
};

const struct usb_khci_host_init_struct _khci0_host_init_param = {
    (void *)(&((VMCF5225_STRUCT_PTR)BSP_IPSBAR)->USBOTG),
    MCF5225_INT_USB, //vector
    3  //priority
};

const void *_bsp_usb_table[] = 
{
    (void *) &_khci0_dev_init_param,
    (void *) &_khci0_host_init_param
};
