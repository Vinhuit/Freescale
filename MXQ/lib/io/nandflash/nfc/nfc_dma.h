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
*   This file contains defines and functions prototype for the  
*   NAND Flash aController (NFC) low level driver.
*
*END************************************************************************/

#ifndef _nfc_dma_h_
#define _nfc_dma_h_

#include "nandflash.h"

#define MAX_WAIT_COMMAND                        0x00100000
#define NFC_DMA_CMD_TICK_TIMEOUT                (1000)

#define NFC_DMA_WERR_EVENT_MASK     (1 << 0)
#define NFC_DMA_DONE_EVENT_MASK     (1 << 1)
#define NFC_DMA_IDLE_EVENT_MASK     (1 << 2)
#define NFC_DMA_ALL_EVENTS_MASK     (NFC_DMA_WERR_EVENT_MASK | NFC_DMA_DONE_EVENT_MASK | NFC_DMA_IDLE_EVENT_MASK)

extern const NANDFLASH_DEVIF_STRUCT _nandflash_nfc_dma_devif;

/*----------------------------------------------------------------------*/
/*
**                     FUNCTION PROTOTYPES
*/
#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t nfc_dma_init( IO_NANDFLASH_STRUCT_PTR );

extern void     nfc_dma_deinit( IO_NANDFLASH_STRUCT_PTR );

extern uint32_t nfc_dma_erase_flash( IO_NANDFLASH_STRUCT_PTR );

extern uint32_t nfc_dma_erase_block( IO_NANDFLASH_STRUCT_PTR, uint32_t, bool );

extern uint32_t nfc_dma_read_page( IO_NANDFLASH_STRUCT_PTR, unsigned char *, 
    uint32_t, uint32_t );

extern uint32_t nfc_dma_write_page( IO_NANDFLASH_STRUCT_PTR, unsigned char *, 
    uint32_t, uint32_t );
    
extern uint32_t nfc_dma_reset( IO_NANDFLASH_STRUCT_PTR );

extern uint32_t nfc_dma_check_block( IO_NANDFLASH_STRUCT_PTR, uint32_t );
    
extern uint32_t nfc_dma_mark_block_as_bad( IO_NANDFLASH_STRUCT_PTR, uint32_t);
    
extern uint32_t nfc_dma_read_ID( IO_NANDFLASH_STRUCT_PTR, unsigned char *, _mem_size );
    
extern _mqx_int nfc_dma_ioctl(IO_NANDFLASH_STRUCT_PTR, _mqx_uint, void *);

extern void nfc_dma_invalidate_dcache(uint32_t, uint32_t);

extern void nfc_dma_flush_dcache(uint32_t, uint32_t);

#ifdef __cplusplus
}
#endif

#endif
/* EOF */
