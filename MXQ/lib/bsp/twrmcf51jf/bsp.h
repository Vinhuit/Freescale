/*HEADER**********************************************************************
*
* Copyright 2011 Freescale Semiconductor, Inc.
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
*
*
*END************************************************************************/

#ifndef __bsp_h__
#define __bsp_h__   1


/* Platform specific files */
#include <psp.h>

/* Processor Expert files */
#include <PE_LDD.h>
#ifdef PE_LDD_VERSION
#include <Events.h>
#endif

#include <bsp_rev.h>
#include <twrmcf51jf.h>

/* Generic IO drivers */
#include <fio.h>
#include <io.h>
#include <io_mem.h>
#include <io_null.h>

/* Clock manager */
#include <cm_cfplus.h>
#include <bsp_cm.h>
#include <cm.h>

/* Low power manager*/
#include <init_lpm.h>
#include <lpm_cfplus.h>
#include <lpm.h>

/* Core and System Modules */
#include <int_ctrl_mcfplus.h>

/* Memories and Memory Interfaces */
#include <flashx.h>
#include <flash_ftfl.h>
#include <flash_mcf51jf.h>

/* Security and Integrity Modules */

/* Analog drivers */
#include <adc_mcf51jf.h>
#include <adc.h>
#include <adc_kadc.h>

/* Timers */
#include <hwtimer.h>
#include <hwtimer_lpt.h>
#include <timer_mtim16.h>

/* Communication Interfaces */
#include <serial.h>
#include <serl_kuart.h>
#include <i2c.h>
#include <i2c_ki2c.h>
#include <spi.h>
#include <spi_spi16.h>

#include <usb_dcd.h>
#include <usb_dcd_kn.h>

/* Human-Machine Interfaces */
#include <lwgpio_mcf51jf.h>
#include <lwgpio.h>
#include <io_gpio.h>

#include <hmi_uids.h>
#include <btnled.h>
#include <hmi_client.h>
#include <hmi_lwgpio_provider.h>
#include <hmi_tss_provider.h>
#include <hmi_provider.h>

/* Other drivers */
#include <sdcard.h>
#include <apccard.h>
#include <pccardflexbus.h>
#include <pcflash.h>

#include <enet.h>

#include <usb_bsp.h>

/* New definitions of interrupt macros */
#define _int_init(irq, level, sublevel, unmask)
#define _int_enable_vector          _mcfplus_int_unmask
#define _int_disable_vector         _mcfplus_int_mask

#define _int_mask_all               _mcfplus_int_mask_all
#define _int_unmask_all             _mcfplus_int_unmask_all
#define _int_mask_vector            _mcfplus_int_mask
#define _int_unmask_vector          _mcfplus_int_unmask

#define _bsp_int_enable             _mcfplus_int_unmask
#define _bsp_int_disable            _mcfplus_int_mask
#define _bsp_int_init(irq, level, sublevel, unmask)
#define _software_interrupt_invoke(num)            _mcfplus_swint_invoke(num)
#define _software_interrupt_clear(num)             _mcfplus_swint_clear(num)

/* Definition of software interrupt vector number */
#define BSP_SOFTWARE_INTERRUPT_VECTOR              (VL1swi)

extern const LPM_CPU_OPERATION_MODE     LPM_CPU_OPERATION_MODES[];
extern const SDCARD_INIT_STRUCT         _bsp_sdcard0_init;

#ifdef __cplusplus
extern "C" {
#endif

/* Generic IO drivers */
/* Clock manager */
/* Low power manager*/
/* Core and System Modules */

/* Memories and Memory Interfaces */
_mqx_int _bsp_ftfx_io_init(_mqx_uint);

/* Security and Integrity Modules */

/* Analog drivers */
_mqx_int _bsp_adc_io_init(_mqx_uint adc_num);
_mqx_int _bsp_adc_channel_io_init(uint16_t source);

/* Timers */

/* Communication Interfaces */
_mqx_int _bsp_serial_io_init(uint8_t dev_num,  uint8_t flags);
_mqx_int _bsp_serial_rts_init(uint8_t);
_mqx_int _bsp_spi_io_init (uint8_t);
_mqx_int _bsp_i2c_io_init (uint8_t);
_mqx_int _bsp_sdio_io_init (uint8_t);
_mqx_int _bsp_usb_io_init(void);

/* Human-Machine Interfaces */
_mqx_int _bsp_gpio_io_init (uint8_t,uint8_t);
_mqx_int _bsp_gpio_io_init_new(void);

_mqx_int _bsp_tss_io_init(void);
HMI_CLIENT_STRUCT_PTR _bsp_btnled_init(void);
_mqx_int _bsp_btnled_set_twrpi(_mqx_int);
_mqx_int _bsp_btnled_get_twrpi(void);

/* Other drivers */

#ifdef __cplusplus
}
#endif

#endif  /* __bsp_h__ */
/* EOF */
