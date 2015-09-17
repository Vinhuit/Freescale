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
*   This file contains the definitions of constants and structures
*   required for initialization of the card.
*
*
*END************************************************************************/

#ifndef __bsp_prv_h__
    #define __bsp_prv_h__

#include "PE_LDD.h"
#ifdef PE_LDD_VERSION
    #include "PDD_Types.h"
#endif


/* Include BSP private header files */
#include "flash_mcf51jf.h"


/*
** I/O initialization controlled by initialization structures for each channel
*/

extern const KADC_INIT_STRUCT           _bsp_adc0_init;
extern const KPDB_INIT_STRUCT           _bsp_pdb0_init;

extern const KUART_INIT_STRUCT          _bsp_uart0_init;
extern const KUART_INIT_STRUCT          _bsp_uart1_init;

extern const SPI16_INIT_STRUCT          _bsp_spi0_init;
extern const SPI16_INIT_STRUCT          _bsp_spi1_init;

extern const KI2C_INIT_STRUCT           _bsp_i2c0_init;
extern const KI2C_INIT_STRUCT           _bsp_i2c1_init;
extern const KI2C_INIT_STRUCT           _bsp_i2c2_init;
extern const KI2C_INIT_STRUCT           _bsp_i2c3_init;

extern const FLASHX_FILE_BLOCK          _bsp_flashx_file_blocks[];
extern const FLASHX_INIT_STRUCT         _bsp_flashx_init;
extern const PCCARDFLEXBUS_INIT_STRUCT  _bsp_cfcard_init;

extern const KUSB_DCD_INIT_STRUCT       _bsp_usb_dcd_init;

#ifdef __cplusplus
extern "C" {
#endif

/*
**  FUNCTION PROTOTYPES
*/

extern void __init_hardware(void);
extern void __cpp_init(void);

extern void _bsp_exit_handler(void);
extern void _bsp_timer_isr(void *);
extern void _bsp_flexbus_pccard_setup (const uint32_t base_address);

#ifdef __cplusplus
}
#endif

#endif /* __bsp_prv_h__ */
/* EOF */

