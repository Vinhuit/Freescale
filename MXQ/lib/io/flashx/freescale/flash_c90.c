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
*   The file contains functions for internal flash read, write, erase 
*
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"
#include "flashx.h"
#include "flashxprv.h"
#include "flash_c90_prv.h"
#include "flash_c90.h"


/* Forward declarations */
static void c90_flash_exec(VC90_REG_STRUCT_PTR reg_ptr);
static void c90_flash_exec_end(void);


const FLASHX_DEVICE_IF_STRUCT _flashx_c90_if = {
    c90_flash_sector_erase,
    c90_flash_sector_program,
    c90_flash_chip_erase,
    NULL /* READ */, 
    c90_flash_init,
    c90_flash_deinit,
    c90_flash_write_protect,
    NULL /* IOCTL */
};


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : c90_flash_init
* Returned Value   : TRUE if successful, FALSE otherwise
* Comments         :
*   Initialize flash specific information.
*
*END*----------------------------------------------------------------------*/
bool c90_flash_init
(
    /* [IN] Flash info structure */
    IO_FLASHX_STRUCT_PTR flash_ptr
)
{
    C90_FLASH_INTERNAL_STRUCT_PTR dev_spec_ptr;
    VC90_REG_STRUCT_PTR reg_ptr;
    FLASHX_BLOCK_INFO_STRUCT_PTR block_info_ptr;

    void        *ram_code_ptr;
    uint32_t    ram_code_size;
    uint32_t    aspace;

    reg_ptr = _bsp_get_c90_address(flash_ptr->BASE_ADDR);
    if (reg_ptr == NULL)
    {
        return FALSE;
    }

    /* allocate internal structure */
    dev_spec_ptr = _mem_alloc_system(sizeof (C90_FLASH_INTERNAL_STRUCT));
    if( dev_spec_ptr == NULL)
    {
        return FALSE;
    }
    flash_ptr->DEVICE_SPECIFIC_DATA = dev_spec_ptr;

    dev_spec_ptr->reg_ptr = reg_ptr;

    /* allocate space to execute flashing from RAM, FIXME: this shall be in PSP */
    ram_code_size = (uint32_t)((char *)c90_flash_exec_end  - (char *)c90_flash_exec);
    ram_code_ptr = _mem_alloc_system(ram_code_size);
    
    if (ram_code_ptr == NULL)
    {
        _mem_free(dev_spec_ptr);
        return FALSE; 
    }
    dev_spec_ptr->flash_exec = ram_code_ptr;

    /* copy code to RAM */
    _mem_copy ((char *)c90_flash_exec, ram_code_ptr, ram_code_size);

    /* count how many sectors belong to each address space */
    dev_spec_ptr->aspace_sectors[C90_ASPACE_LAS] = 0;
    dev_spec_ptr->aspace_sectors[C90_ASPACE_MAS] = 0;
    dev_spec_ptr->aspace_sectors[C90_ASPACE_HAS] = 0;
    dev_spec_ptr->aspace_sectors[C90_ASPACE_SHADOW] = 0;

    block_info_ptr = flash_ptr->HW_BLOCK;
    
    while (block_info_ptr->NUM_SECTORS) {

        aspace = C90_ASPACE(block_info_ptr->SPECIAL_TAG);
        if (aspace >= 0 && aspace < C90_ASPACE_COUNT)
            dev_spec_ptr->aspace_sectors[aspace] += block_info_ptr->NUM_SECTORS;

        block_info_ptr++;
    }        

    return TRUE;    
}


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : c90_flash_deinit
* Returned Value   : none
* Comments         :
*   Release flash specific information.  
*
*END*----------------------------------------------------------------------*/
void c90_flash_deinit 
( 
    /* [IN] Flash info structure */
    IO_FLASHX_STRUCT_PTR flash_ptr 
)
{
    C90_FLASH_INTERNAL_STRUCT_PTR dev_spec_ptr;

    /* de-allocate the device specific structure */
    dev_spec_ptr = flash_ptr->DEVICE_SPECIFIC_DATA;
    _mem_free(dev_spec_ptr->flash_exec);
    _mem_free(dev_spec_ptr);

    flash_ptr->DEVICE_SPECIFIC_DATA = NULL;
}   


    
/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : c90_flash_addr_to_sector
* Returned Value   : pointer to matching FLASHX_BLOCK_INFO_STRUCT  
* Comments         :
*   Finds address space and sector number for given address
*
*END*----------------------------------------------------------------------*/
static FLASHX_BLOCK_INFO_STRUCT_PTR c90_flash_addr_to_sector
(
    /* [IN] Flash info structure */
    IO_FLASHX_STRUCT_PTR flash_ptr, 
    
    /* [IN] Address */
    uint32_t addr,
    
    /* [OUT] Address space */
    int *aspace,

    /* [OUT] Sector in the address space */
    int *aspace_sec
)  
{
    C90_FLASH_INTERNAL_STRUCT_PTR dev_spec_ptr;
    FLASHX_BLOCK_INFO_STRUCT_PTR block_info_ptr;

    dev_spec_ptr = flash_ptr->DEVICE_SPECIFIC_DATA;
    block_info_ptr = flash_ptr->HW_BLOCK;

    *aspace = 0;
    *aspace_sec = 0;    

    /* walk through the block info array, assuming it is:
       1) address space boundaries are between the blocks
       2) blocks belonging to different address spaces are not mixed */
    while (block_info_ptr->NUM_SECTORS) {

        if (*aspace != (int)C90_ASPACE(block_info_ptr->SPECIAL_TAG))
        {
            *aspace = (int)C90_ASPACE(block_info_ptr->SPECIAL_TAG);
            *aspace_sec=0;        
        } 
        
        if ((addr >= block_info_ptr->START_ADDR) && (addr < block_info_ptr->START_ADDR + block_info_ptr->NUM_SECTORS * block_info_ptr->SECTOR_SIZE))
            break; /* found matching sector */

        *aspace_sec += block_info_ptr->NUM_SECTORS;
        block_info_ptr++;
    }

    if (block_info_ptr->NUM_SECTORS == 0)
    {
        *aspace = 0;
        *aspace_sec = 0;
        return NULL; /*  no matching sector found */
    }

    *aspace_sec += (addr - block_info_ptr->START_ADDR) / block_info_ptr->SECTOR_SIZE;
    return block_info_ptr;
}


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : c90_flash_protection_check
* Returned Value   : TRUE if write protected, FALSE otherwise
* Comments         :
*   Check for protection of given sector
*
*END*----------------------------------------------------------------------*/
static bool c90_flash_protection_check
(
    /* [IN] Flash info structure */
    IO_FLASHX_STRUCT_PTR flash_ptr,
     
    /* [IN] Address space */
    int aspace,

    /* [IN] Sector in the address space */
    int aspace_sec
)
{
    C90_FLASH_INTERNAL_STRUCT_PTR dev_spec_ptr;
    VC90_REG_STRUCT_PTR reg_ptr;

    uint32_t aspace_mask;

    dev_spec_ptr = flash_ptr->DEVICE_SPECIFIC_DATA;
    reg_ptr = dev_spec_ptr->reg_ptr;
    
    aspace_mask = 1<<aspace_sec;

    switch (aspace) {
        case C90_ASPACE_LAS:
            if (reg_ptr->LML & C90_LML_LLOCK(aspace_mask)) return TRUE;        
            if (reg_ptr->SLL & C90_SLL_SLLOCK(aspace_mask)) return TRUE;        
            break;
        case C90_ASPACE_MAS:
            if (reg_ptr->LML & C90_LML_MLOCK(aspace_mask)) return TRUE;        
            if (reg_ptr->SLL & C90_SLL_SMLOCK(aspace_mask)) return TRUE;        
            break;
        case C90_ASPACE_HAS:
            if (reg_ptr->HBL & C90_HBL_HLOCK(aspace_mask)) return TRUE;
            break;
        case C90_ASPACE_SHADOW:
            if (reg_ptr->LML & C90_LML_SLOCK) return TRUE;
            if (reg_ptr->SLL & C90_SLL_SSLOC) return TRUE;
            break;
    }

    return FALSE;
}



/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : c90_flash_sector_erase
* Returned Value   : TRUE if successful, FALSE otherwise
* Comments         :
*   Erase a flash memory block.
*
*END*----------------------------------------------------------------------*/
bool c90_flash_sector_erase
(
    /* [IN] Flash info structure */
    IO_FLASHX_STRUCT_PTR flash_ptr, 

    /* [IN] Erased sector address */
    char             *erase_ptr,

    /* [IN] Erased sector size */
    _mem_size            size
)
{
    C90_FLASH_INTERNAL_STRUCT_PTR dev_spec_ptr;
    VC90_REG_STRUCT_PTR reg_ptr;
    FLASHX_BLOCK_INFO_STRUCT_PTR block_info_ptr;

    uint32_t erase_addr;

    int aspace; /* address space (LAS, MAS, HAS) */
    int aspace_sec; /* sector counter of current address space */
    uint32_t aspace_mask[4];

    /* -- end of declarations -- */

    dev_spec_ptr = flash_ptr->DEVICE_SPECIFIC_DATA;
    reg_ptr = dev_spec_ptr->reg_ptr;

    erase_addr = (uint32_t)erase_ptr - (uint32_t)(flash_ptr->BASE_ADDR);

    /* convert address to address space and sector number */
    block_info_ptr = c90_flash_addr_to_sector(flash_ptr, erase_addr, &aspace, &aspace_sec);  

    if (block_info_ptr == NULL)
        return FALSE; /* no matching sector found */
        
    if ((erase_addr - block_info_ptr->START_ADDR) % block_info_ptr->SECTOR_SIZE)
        return FALSE; /* misaligned address */

    if (size != block_info_ptr->SECTOR_SIZE)
        return FALSE; /* only single sector erase is supported */

    /* check for protection */
    if (c90_flash_protection_check(flash_ptr, aspace, aspace_sec))
        return FALSE;

    aspace_mask[0] = aspace_mask[1] = aspace_mask[2] = aspace_mask[3] = 0;
    aspace_mask[aspace] = 1<<aspace_sec;

    /* start erase sequence */
    reg_ptr->MCR |= C90_MCR_ERS;

    reg_ptr->LMS = C90_LMS_LSEL(aspace_mask[C90_ASPACE_LAS]) | C90_LMS_MSEL(aspace_mask[C90_ASPACE_MAS]);
    reg_ptr->HBS = C90_HBS_HSEL(aspace_mask[C90_ASPACE_HAS]);

    *((uint32_t *)erase_ptr) = 0; /* dummy write to flash required */
    dev_spec_ptr->flash_exec(reg_ptr);
    
    if ((reg_ptr->MCR & C90_MCR_PEG) == 0)
    {
        reg_ptr->MCR &= ~C90_MCR_ERS;
        return FALSE;
    }
    
    reg_ptr->MCR &= ~C90_MCR_ERS;
    
    return TRUE;
}


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : c90_flash_sector_program
* Returned Value   : TRUE if successful, FALSE otherwise
* Comments         :
*   Performs a write into flash memory.
*
*END*----------------------------------------------------------------------*/
bool c90_flash_sector_program
(
    /* [IN] Flash info structure */
    IO_FLASHX_STRUCT_PTR flash_ptr,

    /* [IN] Source address */
    char             *from_ptr,

    /* [IN] Destination address */
    char             *to_ptr,

    /* [IN] Number of bytes to write */
    _mem_size            size
) 
{
    C90_FLASH_INTERNAL_STRUCT_PTR dev_spec_ptr;
    VC90_REG_STRUCT_PTR reg_ptr;
    FLASHX_BLOCK_INFO_STRUCT_PTR block_info_ptr;

    uint32_t *src;
    volatile uint32_t *dst;
    
    int aspace;
    int aspace_sec;

    int align;
    int width;

    uint32_t to_addr;
           

    to_addr = (uint32_t)to_ptr - (uint32_t)(flash_ptr->BASE_ADDR);

    /* convert address to address space and sector number */
    block_info_ptr = c90_flash_addr_to_sector(flash_ptr, to_addr, &aspace, &aspace_sec);
    
    if (block_info_ptr == NULL)
        return FALSE; /* no matching sector found */

    /* check alignment if applicable, ECC might not be computed correctly for missaligned data */
    align = (int)C90_ALIGN(block_info_ptr->SPECIAL_TAG);
    
    if (align)
    {
        if (to_addr % align)
            return FALSE; /* misaligned address */
        
        if (size % align)
            return FALSE; /* misaligned size */
    }  

    if ((to_addr - block_info_ptr->START_ADDR) % block_info_ptr->SECTOR_SIZE + size > block_info_ptr->SECTOR_SIZE)
        return FALSE; /* write only within a single sector is supported */

    /* check for protection */
    if (c90_flash_protection_check(flash_ptr, aspace, aspace_sec))
        return FALSE;

    dev_spec_ptr = flash_ptr->DEVICE_SPECIFIC_DATA;
    reg_ptr = dev_spec_ptr->reg_ptr;

    src = (uint32_t *)from_ptr;    
    dst = (uint32_t *)to_ptr;    

    width = (int)C90_WIDTH(block_info_ptr->SPECIAL_TAG);

    while (size >= 4)
    {
        /* start programming sequence */
        reg_ptr->MCR |= C90_MCR_PGM;

        /* if there are at least 128 bits to be written and the destination address is aligned */ 
        if ((width >= 128) && (size >= 16 && (((uint32_t)dst % 16) == 0)))
        {
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
            size -= 16;    
        }         
        else if ((width >= 64) && (size >= 8 && (((uint32_t)dst % 8) == 0)) ){
            /* write 64 bits */
            *dst++ = *src++;
            *dst++ = *src++;
            size -= 8;
        } else {
            /* write 32 bits */
            *dst++ = *src++;
            size -= 4;
        }            
   
        dev_spec_ptr->flash_exec(reg_ptr);
    
        if ((reg_ptr->MCR & C90_MCR_PEG) == 0)
        {
            reg_ptr->MCR &= ~C90_MCR_PGM;    
            return FALSE;
        }

        reg_ptr->MCR &= ~C90_MCR_PGM;
    }

    return TRUE;
}


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : c90_flash_chip_erase
* Returned Value   : TRUE if successful, FALSE otherwise
* Comments         :
*   Erase the whole flash memory.
*
*END*----------------------------------------------------------------------*/
bool c90_flash_chip_erase 
(
    /* [IN] Flash info structure */
    IO_FLASHX_STRUCT_PTR flash_ptr
)
{
    C90_FLASH_INTERNAL_STRUCT_PTR dev_spec_ptr;
    VC90_REG_STRUCT_PTR reg_ptr;

    uint32_t las_mask;
    uint32_t mas_mask;
    uint32_t has_mask;

    dev_spec_ptr = flash_ptr->DEVICE_SPECIFIC_DATA;
    reg_ptr = dev_spec_ptr->reg_ptr;
     
    las_mask = (1 << dev_spec_ptr->aspace_sectors[C90_ASPACE_LAS]) - 1;
    mas_mask = (1 << dev_spec_ptr->aspace_sectors[C90_ASPACE_MAS]) - 1;
    has_mask = (1 << dev_spec_ptr->aspace_sectors[C90_ASPACE_HAS]) - 1;
    
    /* check for protection */
    if (reg_ptr->LML & (C90_LML_LLOCK(las_mask) | C90_LML_MLOCK(mas_mask)))
        return FALSE;

    if (reg_ptr->SLL & (C90_SLL_SLLOCK(las_mask) | C90_SLL_SMLOCK(mas_mask)))
        return FALSE;

    if (reg_ptr->HBL & C90_HBL_HLOCK(has_mask))
        return FALSE;
        
    /* start erase sequence */
    reg_ptr->MCR |= C90_MCR_ERS;

    reg_ptr->LMS = C90_LMS_LSEL(las_mask) | C90_LMS_MSEL(mas_mask);
    reg_ptr->HBS = C90_HBS_HSEL(has_mask);

    *((uint32_t *)(flash_ptr->BASE_ADDR)) = 0; /* dummy write to flash required */
    dev_spec_ptr->flash_exec(reg_ptr);
    
    if ((reg_ptr->MCR & C90_MCR_PEG) == 0)
    {
        reg_ptr->MCR &= ~C90_MCR_ERS;
        return FALSE;
    }
    
    reg_ptr->MCR &= ~C90_MCR_ERS;
    
    return TRUE;
}


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : c90_flash_write_protect
* Returned Value   : TRUE if successful, FALSE otherwise
* Comments         :
*   Sets/clears write protection of the FLASH
*
*END*----------------------------------------------------------------------*/
bool c90_flash_write_protect
(
    /* [IN] Flash info structure */
    IO_FLASHX_STRUCT_PTR flash_ptr,

    /* [IN] Requested protection state (true = protected) */
    _mqx_uint state
)
{
    uint32_t las_mask;
    uint32_t mas_mask;
    uint32_t has_mask;

    uint32_t lml_mask;
    uint32_t sll_mask;
    uint32_t hbl_mask;
    
    uint32_t lml;
    uint32_t sll;
    uint32_t hbl;

    C90_FLASH_INTERNAL_STRUCT_PTR dev_spec_ptr;
    VC90_REG_STRUCT_PTR reg_ptr;

    dev_spec_ptr = flash_ptr->DEVICE_SPECIFIC_DATA;
    reg_ptr = dev_spec_ptr->reg_ptr;

    las_mask = (1 << dev_spec_ptr->aspace_sectors[C90_ASPACE_LAS]) - 1;
    mas_mask = (1 << dev_spec_ptr->aspace_sectors[C90_ASPACE_MAS]) - 1;
    has_mask = (1 << dev_spec_ptr->aspace_sectors[C90_ASPACE_HAS]) - 1;

    lml_mask = C90_LML_SLOCK | C90_LML_MLOCK(mas_mask) | C90_LML_LLOCK(las_mask);
    sll_mask = C90_SLL_SSLOC | C90_SLL_SMLOCK(mas_mask) | C90_SLL_SLLOCK(las_mask);
    hbl_mask = C90_HBL_HLOCK(has_mask);

    if (state)
    {
        lml = lml_mask;
        sll = sll_mask;
        hbl = hbl_mask;
    }
    else
    {
        lml = 0;
        sll = 0;
        hbl = 0;
    }
    
    if ((reg_ptr->LML & lml_mask) != lml)
    {
        if ((reg_ptr->LML & C90_LML_LME) == 0)
            reg_ptr->LML = C90_LML_PASSWORD;
        reg_ptr->LML = lml;
    }

    if ((reg_ptr->SLL & sll_mask) != sll)
    {
        if ((reg_ptr->SLL & C90_SLL_SLE) == 0)
            reg_ptr->SLL = C90_SLL_PASSWORD;
        reg_ptr->SLL = sll;
    }

    if ((reg_ptr->HBL & hbl_mask) != hbl)
    {
        if ((reg_ptr->HBL & C90_HBL_HBE) == 0)
            reg_ptr->HBL = C90_HBL_PASSWORD;
        reg_ptr->HBL = hbl;
    }

    return TRUE;
}


/********************************************************************
*
*  Code required to run in SRAM to perform flash commands. 
*  All else can be run in flash.
*  Parameter is an address of flash status register.
*
********************************************************************/ 
static void c90_flash_exec
( 
    VC90_REG_STRUCT_PTR reg_ptr
) 
{
  
    reg_ptr->MCR |= C90_MCR_EHV;
    while ((reg_ptr->MCR & C90_MCR_DONE) == 0)
    	;
    reg_ptr->MCR &= ~C90_MCR_EHV;
}
         
/* Dummy function to obtain ram_function length. This assumes that linker places objects in the same order as they appear in the source */
static void c90_flash_exec_end
(
    void
)
{
}
