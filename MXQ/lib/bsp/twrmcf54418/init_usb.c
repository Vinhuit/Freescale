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
#define BSP_IPSBAR (0xEC000000)
const struct usb_ehci_dev_init_struct _ehci0_dev_init_param = {
    (void *)(&((VMCF5441_STRUCT_PTR)BSP_IPSBAR)->USBOTG),
    (void *)((uint32_t)&(((VMCF5441_STRUCT_PTR)BSP_IPSBAR)->USBOTG) + 0x100),
    (void *)((uint32_t)&(((VMCF5441_STRUCT_PTR)BSP_IPSBAR)->USBOTG) + 0x080),
    (void *) &(((VMCF5441_STRUCT_PTR)BSP_IPSBAR)->CCM).UOCSR,
    MCF5441_INT_USB_OTG, //vector
    3  //priority
};

const struct usb_ehci_host_init_struct _ehci0_host_init_param = {
    (void *)(&((VMCF5441_STRUCT_PTR)BSP_IPSBAR)->USBOTG),
    (void *)((uint32_t)&(((VMCF5441_STRUCT_PTR)BSP_IPSBAR)->USBOTG) + 0x100),
    (void *)((uint32_t)&(((VMCF5441_STRUCT_PTR)BSP_IPSBAR)->USBOTG) + 0x080),
    BSP_EHCI_FS,
    MCF5441_INT_USB_OTG, //vector
    3,  //priority
    1024 //size of periodic list [items]
};

const struct usb_ehci_host_init_struct _ehci1_host_init_param = {
    (void *)(&((VMCF5441_STRUCT_PTR)BSP_IPSBAR)->USBHOST),
    (void *)((uint32_t)&(((VMCF5441_STRUCT_PTR)BSP_IPSBAR)->USBHOST) + 0x100),
    (void *)((uint32_t)&(((VMCF5441_STRUCT_PTR)BSP_IPSBAR)->USBHOST) + 0x080),
    BSP_EHCI_FS,
    MCF5441_INT_USB_HOST, //vector
    3,  //priority
    1024 //size of periodic list [items]
};

const void *_bsp_usb_table[] = 
{
    (void *) &_ehci0_dev_init_param,
    (void *) &_ehci0_host_init_param,
    (void *) &_ehci1_host_init_param
};
