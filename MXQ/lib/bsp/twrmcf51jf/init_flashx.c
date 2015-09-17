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
*   This file contains the global generic settings for FLASHX driver.
*
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"

extern char __FLASHX_START_ADDR[];
extern char __FLASHX_END_ADDR[];


const FLASHX_FILE_BLOCK _bsp_flashx_file_blocks[] = {
    { "bank0"   , (uint32_t) 0x00000000, (uint32_t) __FLASHX_END_ADDR },
    { "flexram0", (_mem_size) BSP_INTERNAL_FLEXRAM_BASE, INTERNAL_FLEXRAM_BASE + BSP_INTERNAL_FLEXRAM_SIZE - 1 },
    { ""        , (uint32_t) __FLASHX_START_ADDR, (uint32_t) __FLASHX_END_ADDR },
    { NULL      ,                             0,                           0 }
};

const FLASHX_INIT_STRUCT _bsp_flashx_init = {
    0x00000000, /* BASE_ADDR should be 0 for internal flashes */
    _flashx_mcf51xx_plus_block_map, /* HW block map for ColdFire+ devices */
    _bsp_flashx_file_blocks, /* Files on the device defined by the BSP */
    &_flashx_ftfl_if, /* Interface for low level driver */
    32, /* For external devices, data lines for the flash. Not used for internal flash devices. */
    1,  /* Number of parallel external flash devices. Not used for internal flash devices. */
    0,  /* 0 if the write verify is requested, non-zero otherwise */
    NULL /* low level driver specific data */
};

/* EOF */
