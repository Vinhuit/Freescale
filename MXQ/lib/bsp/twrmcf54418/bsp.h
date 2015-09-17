/*HEADER**********************************************************************
*
* Copyright 2008 Freescale Semiconductor, Inc.
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
*   This file includes all include files specific to this target system.
*   $Header:bsp.h, 5, 3/15/2007 11:18:32 AM, Jim Gard$
*   $NoKeywords$
*
*
*END************************************************************************/
#ifndef _bsp_h_
#define _bsp_h_ 1

#include <bsp_rev.h>
#include <psp.h>
#include <fio.h>
#include <io.h>
#include <bsp_cm.h>
#include <twrmcf54418.h>       /* Target definitions */
#include <int_ctrl_mcf5441.h>  /* Interrupt controller */
#include <timer_pit.h>         /* Timer */
#include <enet.h>              /* Ethernet */
#include <macnet_mcf5441.h>    /* Underlying Ethernet device */
#include <phy_dp83xxx.h>       /* Ethernet Physical Interface */
#include <serial.h>            /* Serial */
#include <serl_mcf54xx.h>      /* Serial */
#include <i2c.h>               /* I2C */
#include <i2c_mcf54xx.h>       /* I2C */
#include <i2s.h>               /* I2S */
#include <i2s_mcf54xx.h>       /* I2S */
#include <spi.h>               /* SPI */
#include <spi_dspi.h>          /* SPI */
#include <apccard.h>           /* CFCARD */
#include <pccardflexbus.h>     /* CFCARD */
#include <pcflash.h>           /* CFCARD */
#include <sdcard.h>            /* SDCARD */
#include <sdcard_spi.h>        /* SDCARD */
#include <io_gpio.h>           /* GPIODEV */
#include <lwgpio_mcf5441.h>    /* lwgpio  */
#include <lwgpio.h>            /* lwgpio  */
#include <adc.h>               /* ADC */
#include <esdhc.h>             /* ESDHC */
#include <irtc_mcf5xxx.h>      /* Independent Real Time Clock IRTC     */
#include <irtc_mcf5441x.h>     /* IRTC platform specific functions     */
#include <rtc.h>
#include <usb_bsp.h>

#include <pcb.h>
#include <io_mem.h>
#include <io_null.h>

#include <nandflash.h>          /* external flash */
#include <nfc.h>                /* NAND Flash Controller */

#define _mcf54xx_int_init     _mcf5441_int_init
#define _mcf54xx_int_unmask   _mcf5441_int_unmask
#define _mcf54xx_int_mask     _mcf5441_int_mask
#define _mcf54xx_int_mask_all _mcf5441_int_mask_all
#define _mcf54xx_swint_invoke _mcf5441_swint_invoke
#define _mcf54xx_swint_clear  _mcf5441_swint_clear

#define _bsp_int_init(irq, level, sublevel, unmask)  _mcf5441_int_init(irq, level, unmask)
#define _bsp_int_enable       _mcf5441_int_unmask
#define _bsp_int_disable      _mcf5441_int_mask
#define _software_interrupt_invoke(num)            _mcf5441_swint_invoke(num)
#define _software_interrupt_clear(num)             _mcf5441_swint_clear(num)

/* Definition of software interrupt vector number */
#define BSP_SOFTWARE_INTERRUPT_VECTOR              (MCF5441_INT0_rsvd44) // Using MCF5441_INT0_rsvd44 because no INT_SWI defined for this board

typedef enum usb_interface {
    USB_CONNECTOR_STANDARD = 0,
    USB_CONNECTOR_ULPI
} usb_interface_t;

typedef enum usb_mode {
    USB_MODE_DEVICE = 0,
    USB_MODE_HOST,
    USB_MODE_OTG
} usb_mode_t;

_mqx_int _bsp_serial_io_init(uint8_t);
void     _bsp_rtc_io_init (void);
 void    _bsp_flexbus_io_init (void);
_mqx_int _bsp_i2c_io_init (uint8_t);
_mqx_int _bsp_dspi_io_init (uint8_t);
void     _bsp_usb_io_init (usb_interface_t, usb_mode_t);
_mqx_int _bsp_flexcan_io_init (uint8_t);
_mqx_int _bsp_adc_io_init(_mqx_uint);
_mqx_int _bsp_adc_enable_clock( void );
_mqx_int _bsp_esdhc_io_init (uint8_t, uint16_t);
void     _bsp_enet_io_init(_mqx_uint);
bool  _bsp_get_mac_address(uint32_t,uint32_t,_enet_address);
_mqx_int _bsp_dtimer_gpio_init(uint8_t dev_num);

extern const SDCARD_INIT_STRUCT _bsp_sdcard0_init;


#endif
/* EOF */
