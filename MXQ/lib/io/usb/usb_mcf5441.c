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

struct usb_host_if_struct _bsp_usb_host_ehci0_if = {
    &_usb_ehci_host_callback_table,
    (void *) &_ehci0_host_init_param,
    NULL
};

struct usb_host_if_struct _bsp_usb_host_ehci1_if = {
    &_usb_ehci_host_callback_table,
    (void *) &_ehci1_host_init_param,
    NULL
};

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_usb_host_init
* Returned Value   : 0 for success, -1 for failure
* Comments         :
*    This function performs BSP-specific initialization related to USB
*
*END*----------------------------------------------------------------------*/
_mqx_int _bsp_usb_host_init(struct usb_host_if_struct *usb_if)
{
    VMCF5441_STRUCT_PTR reg_ptr = _PSP_GET_IPSBAR();
    VMCF5XXX_USBOTG_STRUCT_PTR usb_ptr;
    uint32_t status;

    if (usb_if->HOST_INIT_PARAM == &_ehci0_host_init_param) {
        usb_ptr = &((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->USBOTG;

        /* disable second FEC clock */
        reg_ptr->SCM_PMM.PMM.PPMSR0 = 54;
        
        /* Reset USB PHY chip as from POR it is in unknown state */
        * ((volatile uint8_t *) 0xec090000) = 0x40;
        _time_delay(1);
        * ((volatile uint8_t *) 0xec090000) = 0x00;

        /* Enable peripheral clock for USB_OTG module */    
        reg_ptr->SCM_PMM.PMM.PPMCR0 = 44;

        if (USBCFG_USBOTG_CONNECTOR == USB_CONNECTOR_ULPI) 
        {
            /* set that USB OTG is using ULPI */
            reg_ptr->CCM.MISCCR2 &= ~0x0001;
            
            usb_ptr->PORTSC1 &= ~MCF5XXX_USBOTG_PORTSC1_PTS;
            usb_ptr->PORTSC1 |= MCF5XXX_USBOTG_PORTSC1_PTS_ULPI | MCF5XXX_USBOTG_PORTSC1_PFSC;
            
             /* FEC0 as RMII mode */
             reg_ptr->MACNET0.RCR = 0x104;
        }
        else
        {
            /* set that USB HOST is using ULPI */
            reg_ptr->CCM.MISCCR2 |= 0x0001;
            usb_ptr->PORTSC1 &= ~MCF5XXX_USBOTG_PORTSC1_PTS;
            usb_ptr->PORTSC1 |= MCF5XXX_USBOTG_PORTSC1_PTS_OCT;
        }

        _bsp_usb_io_init(USBCFG_USBOTG_CONNECTOR, USB_MODE_HOST);

        /* Enable external peripheral clock for USB_OTG and USB_HOST modules */
        reg_ptr->CCM.MISCCR &= ~0x0001;

        /* setup interrupt */
        status = _mcf5441_int_init(MCF5441_INT_USB_OTG, 3, TRUE);

    }
    else if (usb_if->HOST_INIT_PARAM == &_ehci1_host_init_param) {
        usb_ptr = &((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->USBHOST;

        /* Enable peripheral clock for USB_HOST module */    
        reg_ptr->SCM_PMM.PMM.PPMCR0 = 45;

        if (USBCFG_USBHOST_CONNECTOR == USB_CONNECTOR_ULPI)
        {
            /* set that USB HOST is using ULPI */
            reg_ptr->CCM.MISCCR2 |= 0x0001;
            usb_ptr->PORTSC1 &= ~MCF5XXX_USBOTG_PORTSC1_PTS;
            usb_ptr->PORTSC1 |= MCF5XXX_USBOTG_PORTSC1_PTS_ULPI;
        }
        else
        {
            /* set that USB OTG is using ULPI */
            reg_ptr->CCM.MISCCR2 &= ~0x0001;
            usb_ptr->PORTSC1 &= ~MCF5XXX_USBOTG_PORTSC1_PTS;
            usb_ptr->PORTSC1 |= MCF5XXX_USBOTG_PORTSC1_PTS_OCT;
        }

        _bsp_usb_io_init(USBCFG_USBHOST_CONNECTOR, USB_MODE_HOST);
    
        /* Enable external peripheral clock for USB_OTG and USB_HOST module */    
        reg_ptr->CCM.MISCCR &= ~0x0001;

        /* setup interrupt */
        status = _mcf5441_int_init(MCF5441_INT_USB_HOST, 2, TRUE);

    }
    else
        status = USBBSP_ERROR;
    
    return status;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_usb_base
* Returned Value   : Address upon success, NULL upon failure
* Comments         :
*    This function returns the address of USB OTG module
*
*END*----------------------------------------------------------------------*/
void   *_bsp_get_usb_base(uint8_t dev_num);
void *_bsp_get_usb_base(uint8_t dev_num)
{
    if (dev_num == USBCFG_CONTROLLER_OTG) {
        return (void *)(&((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->USBOTG);
    }
    else if (dev_num == USBCFG_CONTROLLER_HOST) {
        return (void *)(&((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->USBHOST);
    }

    return NULL;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_usb_capability_register_base
* Returned Value   : Address upon success, NULL upon failure
* Comments         :
*    This function returns the address of the VUSBHS Capability Registers
*
*END*----------------------------------------------------------------------*/
void   *_bsp_get_usb_capability_register_base(uint8_t dev_num);
void *_bsp_get_usb_capability_register_base(uint8_t dev_num)
{
    if (dev_num == USBCFG_CONTROLLER_OTG) {
        return (void *)((uint32_t)&(((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->USBOTG) + 0x100);
    }
    else if (dev_num == USBCFG_CONTROLLER_HOST) {
        return (void *)((uint32_t)&(((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->USBHOST) + 0x100);
    }

    return NULL;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_usb_timer_register_base
* Returned Value   : Address upon success, NULL upon failure
* Comments         :
*    This function returns the address of the VUSBHS Timer Registers
*
*END*----------------------------------------------------------------------*/
void   *_bsp_get_usb_timer_register_base(uint8_t dev_num);
void *_bsp_get_usb_timer_register_base(uint8_t dev_num)
{
    if (dev_num == USBCFG_CONTROLLER_OTG) {
        return (void *)((uint32_t)&(((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->USBOTG) + 0x080);
    }
    else if (dev_num == USBCFG_CONTROLLER_HOST) {
        return (void *)((uint32_t)&(((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->USBHOST) + 0x080);
    }

    return NULL;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_usb_otg_csr
* Returned Value   : Address upon success, NULL upon failure
* Comments         :
*    This function returns the address of the U0CSR register
*
*END*----------------------------------------------------------------------*/
void   *_bsp_get_usb_otg_csr(uint8_t dev_num);
void *_bsp_get_usb_otg_csr(uint8_t dev_num)
{
    if (dev_num == USBCFG_CONTROLLER_OTG) {
        return (void *) &(((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->CCM).UOCSR;
    }
    else if (dev_num == USBCFG_CONTROLLER_HOST) {
        return (void *) &(((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->CCM).UHCSR;
    }
    
    return NULL;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_usb_vector
* Returned Value   : Vector Number upon success, 0 upon failure
* Comments         :
*    This function returns the vector number of the VUSBHS Host Module
*
*END*----------------------------------------------------------------------*/
uint8_t _bsp_get_usb_vector(uint8_t dev_num);
uint8_t _bsp_get_usb_vector
(
    uint8_t dev_num
)
{
    if (dev_num == USBCFG_CONTROLLER_OTG) {
        return MCF5441_INT_USB_OTG;
    }
    else if (dev_num == USBCFG_CONTROLLER_HOST) {
        return MCF5441_INT_USB_HOST;
    }

    return 0;
}
