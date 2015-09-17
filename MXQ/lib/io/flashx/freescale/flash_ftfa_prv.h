#ifndef __flash_ftfa_prv_h__
#define __flash_ftfa_prv_h__
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
* See license agreement file for full license terms including other restrictions.
*****************************************************************************
*
* Comments:
*
*   The file contains function prototypes and defines for the internal 
*   flash driver.
*
*
*END************************************************************************/

#include "flashx.h"

/*----------------------------------------------------------------------*/
/*
**                          CONSTANT DEFINITIONS
*/

/* FTFA commands */
#define FTFA_VERIFY_BLOCK              0x00
#define FTFA_VERIFY_SECTION            0x01
#define FTFA_PROGRAM_CHECK             0x02
#define FTFA_READ_RESOURCE             0x03
#define FTFA_PROGRAM_LONGWORD          0x06
#define FTFA_PROGRAM_PHRASE            0x07
#define FTFA_ERASE_BLOCK               0x08
#define FTFA_ERASE_SECTOR              0x09
#define FTFA_PROGRAM_SECTION           0x0B
#define FTFA_VERIFY_ALL_BLOCK          0x40
#define FTFA_READ_ONCE                 0x41
#define FTFA_PROGRAM_ONCE              0x43
#define FTFA_ERASE_ALL_BLOCK           0x44
#define FTFA_SECURITY_BY_PASS          0x45
#define FTFA_PFLASH_SWAP               0x46
#define FTFA_PROGRAM_PARTITION         0x80
#define FTFA_SET_EERAM                 0x81

/* FTFA margin read settings */
#define FTFA_MARGIN_NORMAL             0x0000
#define FTFA_USER_MARGIN_LEVEL1        0x0001
#define FTFA_USER_MARGIN_LEVEL0        0x0002
#define FTFA_FACTORY_MARGIN_LEVEL0     0x0003
#define FTFA_FACTORY_MARGIN_LEVEL1     0x0004

/* FTFA sizes */
#define FTFA_WORD_SIZE                 0x0002
#define FTFA_LONGWORD_SIZE             0x0004
#define FTFA_PHRASE_SIZE               0x0008
#define FTFA_DPHRASE_SIZE              0x0010

/* FTFA error codes */
#define FTFA_OK                        0x0000
#define FTFA_ERR_SIZE                  0x0001
#define FTFA_ERR_RANGE                 0x0002
#define FTFA_ERR_ACCERR                0x0004
#define FTFA_ERR_PVIOL                 0x0008
#define FTFA_ERR_MGSTAT0               0x0010
#define FTFA_ERR_CHANGEPROT            0x0020
#define FTFA_ERR_EEESIZE               0x0040
#define FTFA_ERR_EFLASHSIZE            0x0080
#define FTFA_ERR_ADDR                  0x0100
#define FTFA_ERR_NOEEE                 0x0200
#define FTFA_ERR_EFLASHONLY            0x0400
#define FTFA_ERR_DFLASHONLY            0x0800
#define FTFA_ERR_RDCOLERR              0x1000
#define FTFA_ERR_RAMRDY                0x2000

/* Start and End Address of PFlash IFR and DFlash IFR */
#define PFLASH_IFR_START_ADDRESS       0x000000
#define PFLASH_IFR_END_ADDRESS         0x0000FF
#define DFLASH_IFR_START_ADDRESS       0x800000
#define DFLASH_IFR_END_ADDRESS         0x8000FF
#define DFLASH_IFR_LONGWORD_ADDRESS    0x8000FC

#define FLASHX_INVALIDATE_CACHE_ALL    0xFFFFFFFF
#define FLASHX_INVALIDATE_CACHE_BLOCK0 0x00000001
#define FLASHX_INVALIDATE_CACHE_BLOCK1 0x00000002
#define FLASHX_INVALIDATE_CACHE_BLOCK2 0x00000004
#define FLASHX_INVALIDATE_CACHE_BLOCK3 0x00000008

/* Flash Swap State */
#define FTFA_SWAP_UNINIT               0x00
#define FTFA_SWAP_READY                0x01
#define FTFA_SWAP_INIT                 0x01
#define FTFA_SWAP_UPDATE               0x02
#define FTFA_SWAP_UPDATE_ERASED        0x03
#define FTFA_SWAP_COMPLETE             0x04

/* PFlash swap control codes */
#define FTFA_SWAP_SET_INDICATOR_ADDR   0x01
#define FTFA_SWAP_SET_IN_PREPARE       0x02
#define FTFA_SWAP_SET_IN_COMPLETE      0x04
#define FTFA_SWAP_REPORT_STATUS        0x08

/* Check if flash swap feature is available */
#define FTFA_SWAP_SUPPORT \
(defined(PSP_HAS_FLASH_SWAP) && PSP_HAS_FLASH_SWAP && defined(BSPCFG_SWAP_INDICATOR_ADDR))

/*
Start address of FLASH_SWAP_INDICATOR valid range.
The start address cannot be placed in "flash configuration field" (0x400-0x40C)
so we use next 32B aligned value - 0x420.
*/
#define FTFA_SWAP_VALID_RANGE_START     (0x00000420)

/*
End address of FLASH_SWAP_INDICATOR valid range.
The end address is of flash space
*/
#define FTFA_SWAP_VALID_RANGE_END       (BSP_INTERNAL_FLASH_SIZE / 2)

/* Upper flash swap indicator address - symmetric to BSPCFG_SWAP_INDICATOR_ADDR */
#define FTFA_SWAP_UPPER_INDICATOR_ADDR \
(BSPCFG_SWAP_INDICATOR_ADDR + (BSP_INTERNAL_FLASH_SIZE / 2))

/*----------------------------------------------------------------------*/
/*
**                    Structure Definitions
*/

typedef struct ftfa_flash_internal_struct 
{
    volatile char  *ftfa_ptr;
    char   *flash_execute_code_ptr;
#if PSP_MQX_CPU_IS_KINETIS
    char   *flash_invalidate_code_ptr;
#endif
        
} FTFA_FLASH_INTERNAL_STRUCT, * FTFA_FLASH_INTERNAL_STRUCT_PTR;

/*----------------------------------------------------------------------*/
/*
**                    FUNCTION PROTOTYPES
*/

#ifdef __cplusplus
extern "C" {
#endif

bool ftfa_flash_init(IO_FLASHX_STRUCT_PTR);
void    ftfa_flash_deinit(IO_FLASHX_STRUCT_PTR);
bool ftfa_flash_erase_sector(IO_FLASHX_STRUCT_PTR, char *, _mem_size);
bool ftfa_flash_write_sector(IO_FLASHX_STRUCT_PTR, char *, char *, _mem_size);
bool ftfa_flash_write_sector_pa_ram(IO_FLASHX_STRUCT_PTR, char *, char *, _mem_size);
_mqx_int ftfa_flash_ioctl(IO_FLASHX_STRUCT_PTR, _mqx_uint, void *);

#ifdef __cplusplus
}
#endif

#endif //__flash_ftfa_prv_h__
