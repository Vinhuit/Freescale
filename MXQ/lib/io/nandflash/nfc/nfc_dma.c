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
*   The file contains functions to DMA interface NAND Flash Controller module.
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"
#include "nandflash.h"
#include "nandflashprv.h"
#include "nfc.h"
#include "nfc_dma.h"
#include "lwevent.h"

#if BSPCFG_NANDFLASH_USE_DMA
/* NFC ECC status word */
#define NFC_ECC_STATUS_CORFAIL      (0x80)
#define NFC_ECC_STATUS_ERROR_COUNT  (0x3F)
#define NFC_ECC_STATUS_OFFSET       (0x0C)

const NANDFLASH_DEVIF_STRUCT _nandflash_nfc_dma_devif = {
    /* INIT                 */   nfc_dma_init,
    /* DEINIT               */   nfc_dma_deinit,
    /* CHIP_ERASE           */   nfc_dma_erase_flash,
    /* BLOCK_ERASE          */   nfc_dma_erase_block,
    /* PAGE_READ            */   nfc_dma_read_page,
    /* PAGE_PROGRAM         */   nfc_dma_write_page,
    /* WRITE_PROTECT        */   NULL,
    /* IS_BLOCK_BAD         */   nfc_dma_check_block,
    /* MARK_BLOCK_AS_BAD    */   nfc_dma_mark_block_as_bad,
    /* IOCTL                */   nfc_dma_ioctl
};

uint32_t nfc_dma_swap_addr1, nfc_dma_swap_addr2;

static void nfc_dma_isr(void * temp);

/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : nfc_dma_isr
* Returned Value   : void
* Comments         :
*    ISR for NFC interrupt. Clear interrupt flags and set event mask.
*
*END*-------------------------------------------------------------------------*/

void nfc_dma_isr
    (
        void * temp
    )
{
    NFC_MemMapPtr nfc_ptr;
    IO_NANDFLASH_STRUCT_PTR nandflash_ptr = (IO_NANDFLASH_STRUCT_PTR)temp;

    /* Get the pointer to nfc registers structure */
    nfc_ptr = _bsp_get_nfc_address();

    if (nfc_ptr->ISR & NFC_ISR_WERR_MASK)
    {
        nfc_ptr->ISR |= NFC_ISR_WERRCLR_MASK;
        _lwevent_set(&nandflash_ptr->LWEVENT, NFC_DMA_WERR_EVENT_MASK);
    }
    if (nfc_ptr->ISR & NFC_ISR_DONE_MASK)
    {
        nfc_ptr->ISR |= NFC_ISR_DONECLR_MASK;
        _lwevent_set(&nandflash_ptr->LWEVENT, NFC_DMA_DONE_EVENT_MASK);
    }
    if (nfc_ptr->ISR & NFC_ISR_IDLE_MASK)
    {
        nfc_ptr->ISR |= NFC_ISR_IDLECLR_MASK;
        _lwevent_set(&nandflash_ptr->LWEVENT, NFC_DMA_IDLE_EVENT_MASK);
    }
}

/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : nfc_dma_invalidate_dcache
* Returned Value   : void
* Comments         :
*    Invalidate the DCACHE
*
*END*-------------------------------------------------------------------------*/

void nfc_dma_invalidate_dcache(uint32_t invalidate_address, uint32_t invalidate_size)
{
    uint32_t invalidate_address_bak;
    
    if (!(invalidate_address % PSP_CACHE_LINE_SIZE) && !(invalidate_size % PSP_CACHE_LINE_SIZE))
    {
        _DCACHE_INVALIDATE_MBYTES((void *)invalidate_address, invalidate_size);
    }
    else
    {
        if (invalidate_address % PSP_CACHE_LINE_SIZE)
        {
            invalidate_address_bak = invalidate_address;
            invalidate_address = invalidate_address / PSP_CACHE_LINE_SIZE * PSP_CACHE_LINE_SIZE;
            invalidate_size = (invalidate_address_bak - invalidate_address) + invalidate_size;
        }
        if (invalidate_size % PSP_CACHE_LINE_SIZE)
        {
            invalidate_size = (invalidate_size / PSP_CACHE_LINE_SIZE + 1) * PSP_CACHE_LINE_SIZE;
        }
        _DCACHE_INVALIDATE_MBYTES((void *)invalidate_address, invalidate_size);
    }
}

/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : nfc_dma_flush_dcache
* Returned Value   : void
* Comments         :
*    Flush the DCACHE
*
*END*-------------------------------------------------------------------------*/

void nfc_dma_flush_dcache(uint32_t flush_address, uint32_t flush_size)
{
    uint32_t flush_address_bak;
    
    if (!(flush_address % PSP_CACHE_LINE_SIZE) && !(flush_size % PSP_CACHE_LINE_SIZE))
    {
        _DCACHE_FLUSH_MBYTES((void *)flush_address, flush_size);
    }
    else
    {
        if (flush_address % PSP_CACHE_LINE_SIZE)
        {
            flush_address_bak = flush_address;
            flush_address = flush_address / PSP_CACHE_LINE_SIZE * PSP_CACHE_LINE_SIZE;
            flush_size = (flush_address_bak - flush_address) + flush_size;
        }
        if (flush_size % PSP_CACHE_LINE_SIZE)
        {
            flush_size = (flush_size / PSP_CACHE_LINE_SIZE + 1) * PSP_CACHE_LINE_SIZE;
        }
        _DCACHE_FLUSH_MBYTES((void *)flush_address, flush_size);
    }
}

/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : nfc_dma_init
* Returned Value   : NANDFLASHERR_NO_ERROR or
*                    NANDFLASHERR_IMPROPER_ECC_SIZE
*                    NANDFLASHERR_INFO_STRUC_MISSING
*                    MQX_OUT_OF_MEMORY
* Comments         :
*    This function initializes the driver.
*
*END*-------------------------------------------------------------------------*/

uint32_t nfc_dma_init
    (
        /* [IN] the NAND flash information */
        IO_NANDFLASH_STRUCT_PTR nandflash_ptr
    )
{ /* Body */

    uint32_t result = NANDFLASHERR_NO_ERROR;
    NFC_MemMapPtr nfc_ptr;
    uint32_t nand_id, block_size_kB, density, num_ecc_bytes;
    NANDFLASH_INFO_STRUCT_PTR nand_info_ptr;

    /* Initialize NFC I/O Pins */
    nfc_io_init();

    /* Get the pointer to nfc registers structure */
    nfc_ptr = _bsp_get_nfc_address();

    /* Create NANDFLASH_INFO_STRUCT based on NAND ID read from the NAND Flash,
    if not defined before manually */
    if((nandflash_ptr->NANDFLASH_INFO_PTR) == NULL)
    {
        nand_info_ptr = (NANDFLASH_INFO_STRUCT_PTR)_mem_alloc_system_zero(
        (_mem_size)sizeof(NANDFLASH_INFO_STRUCT));

#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
        if(nand_info_ptr == NULL)
        {
            return((uint32_t)MQX_OUT_OF_MEMORY);
        } /* Endif */
#endif

        if (((*nandflash_ptr->IOCTL)(nandflash_ptr,NANDFLASH_IOCTL_GET_ID,&nand_id)) == NANDFLASHERR_NO_ERROR)
        {
            nand_info_ptr->PHY_PAGE_SIZE =
            (_mem_size)(((nand_id & NANDFLASH_ID_PAGE_SIZE_MASK) + 1)*1024);

            if((nand_id & NANDFLASH_ID_SPARE_BYTES_NUM_MASK)>>2 == 1)
            {
                nand_info_ptr->SPARE_AREA_SIZE = (_mem_size)64;
            }

            if(nand_id & NANDFLASH_ID_BLOCK_SIZE_MASK)
            {
                nand_info_ptr->BLOCK_SIZE =
                (_mem_size)((nand_id & NANDFLASH_ID_BLOCK_SIZE_MASK)*8192);
            }
            else
            {
                nand_info_ptr->BLOCK_SIZE = 65536;
            }

            block_size_kB = (nand_info_ptr->BLOCK_SIZE)/1024;
            density = (nand_id & NANDFLASH_ID_DENSITY_MASK)>>16;
            switch(density)
            {
                case NANDFLASH_DENSITY_1Gb:
                    /* nand_info_ptr->NUM_BLOCKS = 1024/8*1024/block_size_kB; */
                    nand_info_ptr->NUM_BLOCKS = 131072/block_size_kB;
                    break;

                case NANDFLASH_DENSITY_2Gb:
                    nand_info_ptr->NUM_BLOCKS = 2*131072/block_size_kB;
                    break;

                case NANDFLASH_DENSITY_4Gb:
                    nand_info_ptr->NUM_BLOCKS = 4*131072/block_size_kB;
                    break;

                case NANDFLASH_DENSITY_8Gb:
                    nand_info_ptr->NUM_BLOCKS = 8*131072/block_size_kB;
                    break;

                case NANDFLASH_DENSITY_16Gb:
                    nand_info_ptr->NUM_BLOCKS = 16*131072/block_size_kB;
                    break;

                default:
                    break;
            }

            nand_info_ptr->WIDTH =
            (_mqx_uint)(8 + ((nand_id & NANDFLASH_ID_WIDTH_MASK)>>3));

            nandflash_ptr->NANDFLASH_INFO_PTR = nand_info_ptr;
            nandflash_ptr->NUM_VIRTUAL_PAGES  =
                ((nand_info_ptr->BLOCK_SIZE)/(nandflash_ptr->VIRTUAL_PAGE_SIZE))*(nand_info_ptr->NUM_BLOCKS);
            nandflash_ptr->PHY_PAGE_SIZE_TO_VIRTUAL_PAGE_SIZE_RATIO =
                (nand_info_ptr->PHY_PAGE_SIZE)/(nandflash_ptr->VIRTUAL_PAGE_SIZE);
        }
        else
        {
            return ((uint32_t)NANDFLASHERR_INFO_STRUC_MISSING);
        }
    }

    /* Set the CFG register */
    nfc_ptr->CFG  = NFC_CFG_STOPWERR_MASK |
                    NFC_CFG_ECCAD((nandflash_ptr->VIRTUAL_PAGE_SIZE / 8) + 1) |
                    NFC_CFG_ECCSRAM_MASK |
                    NFC_CFG_IDCNT(5) |
                    NFC_CFG_TIMEOUT(6) |
                    NFC_CFG_PAGECNT(1);

    if((nandflash_ptr->NANDFLASH_INFO_PTR->WIDTH) == 16)
        nfc_ptr->CFG |= NFC_CFG_BITWIDTH_MASK;
    else if((nandflash_ptr->NANDFLASH_INFO_PTR->WIDTH) == 8)
        nfc_ptr->CFG &= ~NFC_CFG_BITWIDTH_MASK;

    switch(nandflash_ptr->ECC_SIZE)
    {
        case 0:
            nfc_ptr->CFG |= NFC_CFG_ECCMODE(0);
            break;
        case 4:
            nfc_ptr->CFG |= NFC_CFG_ECCMODE(1);
            break;
        case 6:
            nfc_ptr->CFG |= NFC_CFG_ECCMODE(2);
            break;
        case 8:
            nfc_ptr->CFG |= NFC_CFG_ECCMODE(3);
            break;
        case 12:
            nfc_ptr->CFG |= NFC_CFG_ECCMODE(4);
            break;
        case 16:
            nfc_ptr->CFG |= NFC_CFG_ECCMODE(5);
            break;
        case 24:
            nfc_ptr->CFG |= NFC_CFG_ECCMODE(6);
            break;
        case 32:
            nfc_ptr->CFG |= NFC_CFG_ECCMODE(7);
            break;
        default:
            result = NANDFLASHERR_IMPROPER_ECC_SIZE;
            break;
    }

    /* Auto-increment of row address is not enabled */
    nfc_ptr->RAI  = 0;

    /* Set both addresses in the SWAP register to the same value equal to
    the possition of the bad block marker within the physical page, mapped
    to the virtual page */
    num_ecc_bytes = NANDFLASH_ECC_SIZE_TO_NUM_BYTES_CONV(nandflash_ptr->ECC_SIZE);
    nfc_dma_swap_addr1 = ((nandflash_ptr->NANDFLASH_INFO_PTR->PHY_PAGE_SIZE)-
        ((nandflash_ptr->PHY_PAGE_SIZE_TO_VIRTUAL_PAGE_SIZE_RATIO - 1) *
        (nandflash_ptr->VIRTUAL_PAGE_SIZE + num_ecc_bytes)))/8;
    nfc_dma_swap_addr2 = (nandflash_ptr->VIRTUAL_PAGE_SIZE)/8;

    /* Install ISR for NFC interrupt */
    nfc_ptr->ISR |= NFC_ISR_WERRCLR_MASK;
    nfc_ptr->ISR |= NFC_ISR_DONECLR_MASK;
    nfc_ptr->ISR |= NFC_ISR_IDLECLR_MASK;
    result = _lwevent_create(&nandflash_ptr->LWEVENT, 0);
    nandflash_ptr->OLD_ISR_DATA = _int_get_isr_data(BSPCFG_NANDFLASH_NFC_INTERRUPT_VECTOR);
    nandflash_ptr->OLD_ISR = _int_install_isr(BSPCFG_NANDFLASH_NFC_INTERRUPT_VECTOR, nfc_dma_isr, (void *)nandflash_ptr);
    _bsp_int_init(BSPCFG_NANDFLASH_NFC_INTERRUPT_VECTOR, BSPCFG_NANDFLASH_NFC_ISR_PRIOR, 0, TRUE);

    return(result);

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : nfc_dma_deinit
* Returned Value   :
* Comments         :
*    This function de-initializes the driver.
*
*END*----------------------------------------------------------------------*/

void nfc_dma_deinit
    (
        /* [IN] the NAND flash information */
        IO_NANDFLASH_STRUCT_PTR nandflash_ptr
    )
{ /* Body */
    NFC_MemMapPtr nfc_ptr;

    /* Get the pointer to nfc registers structure */
    nfc_ptr = _bsp_get_nfc_address();

    /* Uninstall ISR for NFC interrupt */
    nfc_ptr->ISR |= NFC_ISR_WERRCLR_MASK;
    nfc_ptr->ISR |= NFC_ISR_DONECLR_MASK;
    nfc_ptr->ISR |= NFC_ISR_IDLECLR_MASK;
    _lwevent_destroy(&nandflash_ptr->LWEVENT);
    _int_install_isr(BSPCFG_NANDFLASH_NFC_INTERRUPT_VECTOR, nandflash_ptr->OLD_ISR, nandflash_ptr->OLD_ISR_DATA);
    _bsp_int_disable(BSPCFG_NANDFLASH_NFC_INTERRUPT_VECTOR);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : nfc_dma_erase_flash
* Returned Value   : NANDFLASHERR_NO_ERROR or
*                    NANDFLASHERR_ERASE_FAILED
* Comments         :
*    This function erases the whole NAND Flash using the NFC module.
*
*END*----------------------------------------------------------------------*/

uint32_t nfc_dma_erase_flash
    (
        /* [IN] the NAND flash information */
        IO_NANDFLASH_STRUCT_PTR nandflash_ptr
    )
{ /* Body */

    uint32_t count;

    for (count = 0; count < (nandflash_ptr->NANDFLASH_INFO_PTR->NUM_BLOCKS); count++)
    {
        if (NANDFLASHERR_NO_ERROR != nfc_dma_erase_block(nandflash_ptr, count, FALSE))
            return NANDFLASHERR_ERASE_FAILED;
    }
    return NANDFLASHERR_NO_ERROR;
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : nfc_dma_erase_block
* Returned Value   : NANDFLASHERR_NO_ERROR or
*                    NANDFLASHERR_ERASE_FAILED or
*                    NANDFLASHERR_TIMEOUT
* Comments         :
*    This function erases the defined block of NAND Flash
*    using the NFC module.
*
*END*----------------------------------------------------------------------*/

uint32_t nfc_dma_erase_block
    (
        /* [IN] the NAND flash information */
        IO_NANDFLASH_STRUCT_PTR nandflash_ptr,

        /* [IN] the block to erase */
        uint32_t                block_number,

        /* [IN] TRUE = force block erase in case the block is marked as bad */
        bool                    force_flag
    )
{ /* Body */
    uint32_t result = NANDFLASHERR_TIMEOUT;
    NFC_MemMapPtr  nfc_ptr;

    /* Do not erase block if force_flag is zero and the block is marked as bad */
    if((!force_flag) &&
            (NANDFLASHERR_BLOCK_NOT_BAD != nfc_dma_check_block(nandflash_ptr, block_number)))
    {
        return NANDFLASHERR_ERASE_FAILED;
    }

    if (nandflash_ptr->WRITE_PROTECT) {
        (*nandflash_ptr->WRITE_PROTECT)(nandflash_ptr, FALSE);
    }/* Endif */

    /* Get the pointer to nfc registers structure */
    nfc_ptr = _bsp_get_nfc_address();

    nfc_ptr->CMD1 = NFC_CMD1_BYTE2(NANDFLASH_CMD_BLOCK_ERASE_CYCLE2) |
                    NFC_CMD1_BYTE3(NANDFLASH_CMD_READ_STATUS);
    nfc_ptr->CMD2 = NFC_CMD2_BYTE1(NANDFLASH_CMD_BLOCK_ERASE_CYCLE1) |
                    NFC_CMD2_CODE(0x4ED8);
    nfc_ptr->RAR  = NFC_RAR_CS0_MASK |
                    NFC_RAR_RB0_MASK |
                    ((nandflash_ptr->NANDFLASH_INFO_PTR->BLOCK_SIZE /
                    nandflash_ptr->NANDFLASH_INFO_PTR->PHY_PAGE_SIZE * block_number) & 0xFFFFFF);
    nfc_ptr->ISR |= (NFC_ISR_IDLECLR_MASK | NFC_ISR_IDLEEN_MASK |
    NFC_ISR_DONECLR_MASK | NFC_ISR_DONEEN_MASK);

    /* Start command execution */
    nfc_ptr->CMD2 |= NFC_CMD2_BUSY_START_MASK;

    if (MQX_OK == _lwevent_wait_ticks(&nandflash_ptr->LWEVENT, NFC_DMA_IDLE_EVENT_MASK, TRUE, NFC_DMA_CMD_TICK_TIMEOUT))
    {
        if (nfc_ptr->SR2 & NANDFLASH_STATUS_ERR)
            result = NANDFLASHERR_ERASE_FAILED;
        else
            result = NANDFLASHERR_NO_ERROR;
    }
    else
    {
        result = NANDFLASHERR_TIMEOUT;
    }
    _lwevent_clear(&nandflash_ptr->LWEVENT, NFC_DMA_ALL_EVENTS_MASK);

    if (nandflash_ptr->WRITE_PROTECT) {
        (*nandflash_ptr->WRITE_PROTECT)(nandflash_ptr, TRUE);
    }/* Endif */
    return(result);

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : nfc_dma_read_page
* Returned Value   : NANDFLASHERR_NO_ERROR or
*                    NANDFLASHERR_ECC_FAILED or
*                    NANDFLASHERR_ECC_CORRECTED or
*                    NANDFLASHERR_TIMEOUT
* Comments         :
*    This function reads one or several pages of NAND Flash
*    using the NFC module.
*
*END*----------------------------------------------------------------------*/

uint32_t nfc_dma_read_page
(
        /* [IN] the NAND flash information */
        IO_NANDFLASH_STRUCT_PTR nandflash_ptr,

        /* [OUT} where to copy data to */
        unsigned char           *to_ptr,

        /* [IN] the page to read */
        uint32_t                page_number,

        /* [IN] the amount of pages to read */
        uint32_t                page_count
    )
{ /* Body */
    uint32_t result = NANDFLASHERR_TIMEOUT;
    uint32_t count1, count2;
    bool ecc_corrected = FALSE, swap = FALSE;
    uint32_t row, col, num_ecc_bytes;
    uint32_t invalidate_address, invalidate_size;
    NFC_MemMapPtr  nfc_ptr;

    /* Get the pointer to nfc registers structure */
    nfc_ptr = _bsp_get_nfc_address();

    for (count1 = page_number; count1 < (page_number + page_count); count1++)
    {
        /* Check alignment of DMA address and size */
        if (((uint32_t)(to_ptr + (count1 - page_number) * nandflash_ptr->VIRTUAL_PAGE_SIZE) % 8) 
            || (nandflash_ptr->VIRTUAL_PAGE_SIZE % 8))
        {
            return NANDFLASHERR_DMA_NOT_ALIGNED;
        }

        num_ecc_bytes = NANDFLASH_ECC_SIZE_TO_NUM_BYTES_CONV(nandflash_ptr->ECC_SIZE);
        row = count1/nandflash_ptr->PHY_PAGE_SIZE_TO_VIRTUAL_PAGE_SIZE_RATIO;
        col = (count1 - (row * nandflash_ptr->PHY_PAGE_SIZE_TO_VIRTUAL_PAGE_SIZE_RATIO))*
            (nandflash_ptr->VIRTUAL_PAGE_SIZE + num_ecc_bytes);
        /* Is the bad block byte(s) about to be re-written? */
        count2 = count1 % ((nandflash_ptr->NANDFLASH_INFO_PTR->BLOCK_SIZE / nandflash_ptr->NANDFLASH_INFO_PTR->PHY_PAGE_SIZE) *     nandflash_ptr->PHY_PAGE_SIZE_TO_VIRTUAL_PAGE_SIZE_RATIO);
        swap = ((count2 == (nandflash_ptr->PHY_PAGE_SIZE_TO_VIRTUAL_PAGE_SIZE_RATIO - 1)) ||
            (count2 == (2*nandflash_ptr->PHY_PAGE_SIZE_TO_VIRTUAL_PAGE_SIZE_RATIO - 1)));

        if(nandflash_ptr->NANDFLASH_INFO_PTR->WIDTH == 16)
            col = col/2;

        nfc_ptr->CAR  = col & 0xFFFF;
        nfc_ptr->RAR  = NFC_RAR_CS0_MASK |
                        NFC_RAR_RB0_MASK |
                        (row & 0xFFFFFF);
        nfc_ptr->SECSZ  = (nandflash_ptr->VIRTUAL_PAGE_SIZE) + num_ecc_bytes;

        /* For 16-bit data width flash devices, only odd SIZE is supported */
        if((nandflash_ptr->NANDFLASH_INFO_PTR->WIDTH == 16) && !(nfc_ptr->SECSZ % 2))
            nfc_ptr->SECSZ  += 1;

        /* If the last virtual page of the first or the second physical page
        is about to be read the virtual page needs to be enlarged
        by 8 bytes and swapping switched on due to the bad block marking */
        if(swap)
        {
            nfc_ptr->SWAP  = NFC_SWAP_ADDR1(nfc_dma_swap_addr1) |
            NFC_SWAP_ADDR2(nfc_dma_swap_addr2);
            nfc_ptr->SECSZ  += 8;
        }
        else
        {
            nfc_ptr->SWAP  = NFC_SWAP_ADDR1(0x7FF) |
            NFC_SWAP_ADDR2(0x7FF);
        }

        nfc_ptr->ISR |= (NFC_ISR_DONECLR_MASK |
                        NFC_ISR_DONEEN_MASK |
                        NFC_ISR_IDLECLR_MASK |
                        NFC_ISR_IDLEEN_MASK);

        nfc_ptr->CMD1 = NFC_CMD1_BYTE2(NANDFLASH_CMD_PAGE_READ_CYCLE2);
        nfc_ptr->CMD2 = NFC_CMD2_BYTE1(NANDFLASH_CMD_PAGE_READ_CYCLE1) | NFC_CMD2_CODE(0x7EE0) | NFC_CMD2_BUFNO(1);
        nfc_ptr->DMA1 = (uint32_t)(to_ptr + (count1 - page_number) * nandflash_ptr->VIRTUAL_PAGE_SIZE);
        nfc_ptr->CFG |= NFC_CFG_DMAREQ_MASK;
        nfc_ptr->DMACFG = NFC_DMACFG_COUNT1(nandflash_ptr->VIRTUAL_PAGE_SIZE) | NFC_DMACFG_ACT1_MASK;

#if PSP_HAS_DATA_CACHE
        /* Check alignment of address and size invalidated */
        invalidate_address = (uint32_t)(to_ptr + (count1 - page_number) * nandflash_ptr->VIRTUAL_PAGE_SIZE);
        invalidate_size = nandflash_ptr->VIRTUAL_PAGE_SIZE;
        nfc_dma_invalidate_dcache(invalidate_address, invalidate_size);
#endif

        /* Start command execution */
        nfc_ptr->CMD2 |= NFC_CMD2_BUSY_START_MASK;

        if (MQX_OK == _lwevent_wait_ticks(&nandflash_ptr->LWEVENT, NFC_DMA_IDLE_EVENT_MASK, TRUE, NFC_DMA_CMD_TICK_TIMEOUT))
        {
            if ( nandflash_ptr->ECC_SIZE == 0)
            {
                if (nfc_ptr->SR2 & NANDFLASH_STATUS_ERR)
                {
                    return(NANDFLASHERR_ECC_FAILED);
                }
            }
            else
            {
                /* Check ECC status word */
                if (*(uint32_t*)&NFC_SRAM_B1_REG(nfc_ptr, nandflash_ptr->VIRTUAL_PAGE_SIZE + NFC_ECC_STATUS_OFFSET) & NFC_ECC_STATUS_CORFAIL)
                {
                    return(NANDFLASHERR_ECC_FAILED);
                }
                else
                {
                    if (*(uint32_t*)&NFC_SRAM_B1_REG(nfc_ptr, nandflash_ptr->VIRTUAL_PAGE_SIZE + NFC_ECC_STATUS_OFFSET) & NFC_ECC_STATUS_ERROR_COUNT)
                    {
                        ecc_corrected = TRUE;
                    }
                }
                result = NANDFLASHERR_NO_ERROR;
            }
        }
        else
        {
            result = NANDFLASHERR_TIMEOUT;
        }
        _lwevent_clear(&nandflash_ptr->LWEVENT, NFC_DMA_ALL_EVENTS_MASK);
    }
    if(ecc_corrected)
        result = NANDFLASHERR_ECC_CORRECTED;
    return(result);

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : nfc_dma_write_page
* Returned Value   : NANDFLASHERR_NO_ERROR or
*                    NANDFLASHERR_WRITE_FAILED or
*                    NANDFLASHERR_TIMEOUT
* Comments         :
*    This function writes one or several pages of NAND Flash
*    using the NFC module.
*
*END*----------------------------------------------------------------------*/

uint32_t nfc_dma_write_page
    (
        /* [IN] the NAND flash information */
        IO_NANDFLASH_STRUCT_PTR nandflash_ptr,

        /* [IN] where to copy data from */
        unsigned char           *from_ptr,

        /* [IN] the first page to write */
        uint32_t                page_number,

        /* [IN] the amount of pages to write */
        uint32_t                page_count
    )
{ /* Body */
    uint32_t result = NANDFLASHERR_TIMEOUT;
    uint32_t count1, count2;
    uint32_t row, col, num_ecc_bytes;
    uint32_t temp = 0;
    bool swap = FALSE;
    uint32_t flush_address, flush_size;
    NFC_MemMapPtr  nfc_ptr;

    if (nandflash_ptr->WRITE_PROTECT) {
        (*nandflash_ptr->WRITE_PROTECT)(nandflash_ptr, FALSE);
    }/* Endif */

    /* Get the pointer to nfc registers structure */
    nfc_ptr = _bsp_get_nfc_address();

    for (count1 = page_number; count1 < (page_number + page_count); count1++)
    {
        /* Check alignment of DMA address and size */
        if (((uint32_t)(from_ptr) % 8) || (nandflash_ptr->VIRTUAL_PAGE_SIZE % 8))
        {
            return NANDFLASHERR_DMA_NOT_ALIGNED;
        }

        num_ecc_bytes = NANDFLASH_ECC_SIZE_TO_NUM_BYTES_CONV(nandflash_ptr->ECC_SIZE);
        row = count1/nandflash_ptr->PHY_PAGE_SIZE_TO_VIRTUAL_PAGE_SIZE_RATIO;
        col = (count1 - (row * nandflash_ptr->PHY_PAGE_SIZE_TO_VIRTUAL_PAGE_SIZE_RATIO))*
            (nandflash_ptr->VIRTUAL_PAGE_SIZE + num_ecc_bytes);

        /* If the last virtual page of the first or the second physical page
        is about to be written the swapping needs to be switched on due
        to the bad block marking */
        count2 = count1 % ((nandflash_ptr->NANDFLASH_INFO_PTR->BLOCK_SIZE / nandflash_ptr->NANDFLASH_INFO_PTR->PHY_PAGE_SIZE) *     nandflash_ptr->PHY_PAGE_SIZE_TO_VIRTUAL_PAGE_SIZE_RATIO);
        swap = ((count2 == (nandflash_ptr->PHY_PAGE_SIZE_TO_VIRTUAL_PAGE_SIZE_RATIO - 1)) ||
            (count2 == (2*nandflash_ptr->PHY_PAGE_SIZE_TO_VIRTUAL_PAGE_SIZE_RATIO - 1)));
        if(swap)
        {
            nfc_ptr->SWAP  = NFC_SWAP_ADDR1(nfc_dma_swap_addr1) |
            NFC_SWAP_ADDR2(nfc_dma_swap_addr2);
        }
        else
        {
            nfc_ptr->SWAP  = NFC_SWAP_ADDR1(0x7FF) |
            NFC_SWAP_ADDR2(0x7FF);
        }

        if(nandflash_ptr->NANDFLASH_INFO_PTR->WIDTH == 16)
            col = col/2;

        nfc_ptr->CAR  = col & 0xFFFF;
        nfc_ptr->RAR  = NFC_RAR_CS0_MASK |
                        NFC_RAR_RB0_MASK |
                        (row & 0xFFFFFF);
        nfc_ptr->SECSZ  = (nandflash_ptr->VIRTUAL_PAGE_SIZE) + num_ecc_bytes;

        /* For 16-bit data width flash devices, only odd SIZE is supported */
        if((nandflash_ptr->NANDFLASH_INFO_PTR->WIDTH == 16) && !(nfc_ptr->SECSZ % 2))
        nfc_ptr->SECSZ  += 1;

        /* If the last virtual page of the first or the second physical page
        is about to be written the bad block marking (negative) has to be
        inserted and the virtual page needs to be enlarged by 8 bytes */
        if(swap)
        {
            temp = *(from_ptr + (nfc_dma_swap_addr2 * 8));
            *(from_ptr + (nfc_dma_swap_addr2 * 8)) = (unsigned char)0xFFFF0000;
            nfc_ptr->SECSZ  += 8;
        }

        nfc_ptr->ISR |= (NFC_ISR_DONECLR_MASK |
                        NFC_ISR_DONEEN_MASK  |
                        NFC_ISR_IDLECLR_MASK |
                        NFC_ISR_IDLEEN_MASK);

        nfc_ptr->CMD1 = NFC_CMD1_BYTE2(NANDFLASH_CMD_PAGE_PROGRAM_CYCLE2) | NFC_CMD1_BYTE3(NANDFLASH_CMD_READ_STATUS);
        nfc_ptr->CMD2 = NFC_CMD2_BYTE1(NANDFLASH_CMD_PAGE_PROGRAM_CYCLE1) | NFC_CMD2_CODE(0xFFD8) | NFC_CMD2_BUFNO(0);
        nfc_ptr->DMA1 = (uint32_t)(from_ptr);
        nfc_ptr->DMACFG = NFC_DMACFG_COUNT1(nandflash_ptr->VIRTUAL_PAGE_SIZE) | NFC_DMACFG_ACT1_MASK;

#if PSP_HAS_DATA_CACHE
        /* Check alignment of address and size flushed */
        flush_address = (uint32_t)from_ptr;
        flush_size = nandflash_ptr->VIRTUAL_PAGE_SIZE;
        nfc_dma_flush_dcache(flush_address, flush_size);
#endif

        /* Start command execution */
        nfc_ptr->CMD2 |= NFC_CMD2_BUSY_START_MASK;

        if (MQX_OK == _lwevent_wait_ticks(&nandflash_ptr->LWEVENT, NFC_DMA_IDLE_EVENT_MASK, TRUE, NFC_DMA_CMD_TICK_TIMEOUT))
        {
            if (nfc_ptr->SR2 & NANDFLASH_STATUS_ERR)
            {
                result = NANDFLASHERR_WRITE_FAILED;
                if (nandflash_ptr->WRITE_PROTECT) {
                    (*nandflash_ptr->WRITE_PROTECT)(nandflash_ptr, TRUE);
                }/* Endif */

                return(result);
            }
            else
            {
                result = NANDFLASHERR_NO_ERROR;
            }
        }
        else
        {
            result = NANDFLASHERR_TIMEOUT;
        }
        _lwevent_clear(&nandflash_ptr->LWEVENT, NFC_DMA_ALL_EVENTS_MASK);

        if(swap)
        {
            *(from_ptr + (nfc_dma_swap_addr2 * 8)) = temp;
        }
        from_ptr += nandflash_ptr->VIRTUAL_PAGE_SIZE;
    }

    if (nandflash_ptr->WRITE_PROTECT) {
        (*nandflash_ptr->WRITE_PROTECT)(nandflash_ptr, TRUE);
    }/* Endif */

    return(result);

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : nfc_dma_reset
* Returned Value   : NANDFLASHERR_NO_ERROR or
*                    NANDFLASHERR_TIMEOUT
* Comments         :
*    This function resets the NAND Flash using the NFC module.
*
*END*----------------------------------------------------------------------*/

uint32_t nfc_dma_reset
    (
        /* [IN] the NAND flash information */
        IO_NANDFLASH_STRUCT_PTR nandflash_ptr
    )
{ /* Body */

    uint32_t result = NANDFLASHERR_TIMEOUT;
    NFC_MemMapPtr  nfc_ptr;

    /* Get the pointer to nfc registers structure */
    nfc_ptr = _bsp_get_nfc_address();

    nfc_ptr->CMD2 = NFC_CMD2_BYTE1(NANDFLASH_CMD_RESET) |
                    NFC_CMD2_CODE(0x4002);
    nfc_ptr->ISR |= (NFC_ISR_IDLECLR_MASK | NFC_ISR_IDLEEN_MASK |
                    NFC_ISR_DONECLR_MASK | NFC_ISR_DONEEN_MASK);

    /* Start command execution */
    nfc_ptr->CMD2 |=  NFC_CMD2_BUSY_START_MASK;

    if (MQX_OK == _lwevent_wait_ticks(&nandflash_ptr->LWEVENT, NFC_DMA_IDLE_EVENT_MASK, TRUE, NFC_DMA_CMD_TICK_TIMEOUT))
    {
        result = NANDFLASHERR_NO_ERROR;
    }
    else
    {
        result = NANDFLASHERR_TIMEOUT;
    }
    _lwevent_clear(&nandflash_ptr->LWEVENT, NFC_DMA_ALL_EVENTS_MASK);

    return result;
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : nfc_dma_check_block
* Returned Value   : NANDFLASHERR_BLOCK_NOT_BAD or
*                    NANDFLASHERR_BLOCK_BAD or
*                    NANDFLASHERR_TIMEOUT
* Comments         :
*    This function checks if the defined NAND Flash block is bad or not
*    using the NFC module.
*
*END*----------------------------------------------------------------------*/

uint32_t nfc_dma_check_block
    (
        /* [IN] the NAND flash information */
        IO_NANDFLASH_STRUCT_PTR nandflash_ptr,

        /* [IN] block to check */
        uint32_t                block_number
    )
{ /* Body */
    uint32_t result = NANDFLASHERR_TIMEOUT;
    uint32_t cfg_bck, col;

    NFC_MemMapPtr  nfc_ptr;

    /* Get the pointer to nfc registers structure */
    nfc_ptr = _bsp_get_nfc_address();

    /* Set the ECCMODE to 0 - ECC bypass */
    cfg_bck = nfc_ptr->CFG;
    nfc_ptr->CFG &= ~(NFC_CFG_ECCMODE(7));

    /* Reset the swap register */
    nfc_ptr->SWAP = NFC_SWAP_ADDR1(0x7FF) |
                    NFC_SWAP_ADDR2(0x7FF);

    col = nandflash_ptr->NANDFLASH_INFO_PTR->PHY_PAGE_SIZE;
    if(nandflash_ptr->NANDFLASH_INFO_PTR->WIDTH == 16)
        col = col/2;

    nfc_ptr->CMD1 = NFC_CMD1_BYTE2(NANDFLASH_CMD_PAGE_READ_CYCLE2);
    nfc_ptr->CMD2 = NFC_CMD2_BYTE1(NANDFLASH_CMD_PAGE_READ_CYCLE1) |
                    NFC_CMD2_CODE(0x7EE0) |
                    NFC_CMD2_BUFNO(1);
    nfc_ptr->CAR  = col & 0xFFFF;
    nfc_ptr->RAR  = NFC_RAR_CS0_MASK |
                    NFC_RAR_RB0_MASK |
                    ((nandflash_ptr->NANDFLASH_INFO_PTR->BLOCK_SIZE /
    nandflash_ptr->NANDFLASH_INFO_PTR->PHY_PAGE_SIZE * block_number) & 0xFFFFFF);
    nfc_ptr->SECSZ = 3;
    nfc_ptr->ISR |= (NFC_ISR_DONECLR_MASK |
                    NFC_ISR_DONEEN_MASK  |
                    NFC_ISR_IDLECLR_MASK |
                    NFC_ISR_IDLEEN_MASK);

    /* Start command execution */
    nfc_ptr->CMD2 |= NFC_CMD2_BUSY_START_MASK;

    if (MQX_OK == _lwevent_wait_ticks(&nandflash_ptr->LWEVENT, NFC_DMA_IDLE_EVENT_MASK, TRUE, NFC_DMA_CMD_TICK_TIMEOUT))
    {
        if(!( (*(uint32_t *)&(NFC_SRAM_B1_REG(nfc_ptr, 0)) & 0xFF000000)))
        {
            result = NANDFLASHERR_BLOCK_BAD;
        }
        else
        {
            if(  (nandflash_ptr->NANDFLASH_INFO_PTR->WIDTH == 16)
                    &&(!(*(uint32_t *)&NFC_SRAM_B1_REG(nfc_ptr, 0) & 0x00FF0000))
                    )
            {
                result = NANDFLASHERR_BLOCK_BAD;
            }
            else
            {
                result = NANDFLASHERR_BLOCK_NOT_BAD;
            }
        }
    }
    else
    {
        result = NANDFLASHERR_TIMEOUT;
    }
    _lwevent_clear(&nandflash_ptr->LWEVENT, NFC_DMA_ALL_EVENTS_MASK);

    /* Set the ECCMODE back */
    nfc_ptr->CFG = cfg_bck;

    return(result);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : nfc_dma_mark_block_as_bad
* Returned Value   : NANDFLASHERR_NO_ERROR or
*                    NANDFLASHERR_WRITE_FAILED or
*                    NANDFLASHERR_TIMEOUT
* Comments         :
*    This function marks the defined NAND Flash block as bad
*    using the NFC module (writing 0x00 to the first spare area byte).
*
*END*----------------------------------------------------------------------*/

uint32_t nfc_dma_mark_block_as_bad
    (
        /* [IN] the NAND flash information */
        IO_NANDFLASH_STRUCT_PTR nandflash_ptr,

        /* [IN] block to mark as bad */
        uint32_t                block_number
    )
{ /* Body */

    uint32_t result = NANDFLASHERR_TIMEOUT;
    uint32_t cfg_bck, col;

    NFC_MemMapPtr  nfc_ptr;

    nfc_dma_erase_block(nandflash_ptr, block_number, FALSE);

    if (nandflash_ptr->WRITE_PROTECT) {
        (*nandflash_ptr->WRITE_PROTECT)(nandflash_ptr, FALSE);
    }/* Endif */

    /* Get the pointer to nfc registers structure */
    nfc_ptr = _bsp_get_nfc_address();

    /* Set the ECCMODE to 0 - ECC bypass */
    cfg_bck = nfc_ptr->CFG;
    nfc_ptr->CFG &= ~(NFC_CFG_ECCMODE(7));

    col = nandflash_ptr->NANDFLASH_INFO_PTR->PHY_PAGE_SIZE;
    if(nandflash_ptr->NANDFLASH_INFO_PTR->WIDTH == 16)
        col = col/2;

    /* Reset the swap register */
    nfc_ptr->SWAP = NFC_SWAP_ADDR1(0x7FF) |
                    NFC_SWAP_ADDR2(0x7FF);

    /* Prepare bad block bytes in the SRAM buffer #0,
    no matter if the NAND Flash width is 8 or 16 */
    *(uint32_t *)&NFC_SRAM_B0_REG(nfc_ptr, 0) = 0;

    nfc_ptr->CMD1 = NFC_CMD1_BYTE2(NANDFLASH_CMD_PAGE_PROGRAM_CYCLE2) |
                    NFC_CMD1_BYTE3(NANDFLASH_CMD_READ_STATUS);
    nfc_ptr->CMD2 = NFC_CMD2_BYTE1(NANDFLASH_CMD_PAGE_PROGRAM_CYCLE1) |
                    NFC_CMD2_CODE(0x7FD8) |
                    NFC_CMD2_BUFNO(0);
    nfc_ptr->CAR  = col & 0xFFFF;
    nfc_ptr->RAR  = NFC_RAR_CS0_MASK |
                    NFC_RAR_RB0_MASK |
                    ((nandflash_ptr->NANDFLASH_INFO_PTR->BLOCK_SIZE /
    nandflash_ptr->NANDFLASH_INFO_PTR->PHY_PAGE_SIZE * block_number) & 0xFFFFFF);
    nfc_ptr->SECSZ = 3;

    nfc_ptr->ISR |= (NFC_ISR_DONECLR_MASK |
                    NFC_ISR_DONEEN_MASK  |
                    NFC_ISR_IDLECLR_MASK |
                    NFC_ISR_IDLEEN_MASK);

    /* Start command execution */
    nfc_ptr->CMD2 |= NFC_CMD2_BUSY_START_MASK;

    if (MQX_OK == _lwevent_wait_ticks(&nandflash_ptr->LWEVENT, NFC_DMA_IDLE_EVENT_MASK, TRUE, NFC_DMA_CMD_TICK_TIMEOUT))
    {
        if (nfc_ptr->SR2 & NANDFLASH_STATUS_ERR)
        {
            result = NANDFLASHERR_WRITE_FAILED;
        }
        else
        {
            result = NANDFLASHERR_NO_ERROR;
        }
    }
    else
    {
        result = NANDFLASHERR_TIMEOUT;
    }
    _lwevent_clear(&nandflash_ptr->LWEVENT, NFC_DMA_ALL_EVENTS_MASK);

    if (nandflash_ptr->WRITE_PROTECT) {
        (*nandflash_ptr->WRITE_PROTECT)(nandflash_ptr, TRUE);
    }/* Endif */

    /* Set the ECCMODE back */
    nfc_ptr->CFG = cfg_bck;

    return(result);

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : nfc_dma_read_ID
* Returned Value   : NANDFLASHERR_NO_ERROR or
*                    NANDFLASHERR_TIMEOUT
* Comments         :
*    This function reads the ID from the NAND Flash
*    using the NFC module.
*
*END*----------------------------------------------------------------------*/

uint32_t nfc_dma_read_ID
    (
        /* [IN] the NAND flash information */
        IO_NANDFLASH_STRUCT_PTR nandflash_ptr,

        /* [IN] where to store ID */
        unsigned char           *to_ptr,

        /* [IN] the amount of ID bytes */
        _mem_size               size
    )
{ /* Body */

    uint32_t result = NANDFLASHERR_TIMEOUT;
    uint32_t count2;
    NFC_MemMapPtr  nfc_ptr;

    /* Get the pointer to nfc registers structure */
    nfc_ptr = _bsp_get_nfc_address();

    nfc_ptr->CMD2 = NFC_CMD2_BYTE1(NANDFLASH_CMD_READ_ID) |
                    NFC_CMD2_CODE(0x4804);
    nfc_ptr->ISR |= (NFC_ISR_IDLECLR_MASK | NFC_ISR_IDLEEN_MASK |
                    NFC_ISR_DONECLR_MASK | NFC_ISR_DONEEN_MASK);

    /* Start command execution */
    nfc_ptr->CMD2 |=  NFC_CMD2_BUSY_START_MASK;

    if (MQX_OK == _lwevent_wait_ticks(&nandflash_ptr->LWEVENT, NFC_DMA_DONE_EVENT_MASK, TRUE, NFC_DMA_CMD_TICK_TIMEOUT))
    {
        for (count2 = 0; count2 < size; count2++)
        {
            to_ptr[count2] = *(unsigned char *)((uint32_t)&nfc_ptr->SR1 + count2);
        }
        result = NANDFLASHERR_NO_ERROR;
    }
    else
    {
        result = NANDFLASHERR_TIMEOUT;
    }
    _lwevent_clear(&nandflash_ptr->LWEVENT, NFC_DMA_ALL_EVENTS_MASK);

    return result;
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : nfc_dma_ioctl
* Returned Value   : TRUE if successful
* Comments         :
*    nfc ioctl function
*
*END*----------------------------------------------------------------------*/
_mqx_int nfc_dma_ioctl
    (
        /* [IN] the handle returned from _fopen */
        IO_NANDFLASH_STRUCT_PTR handle_ptr,

        /* [IN] the ioctl command */
        _mqx_uint               cmd,

        /* [IN] the ioctl parameters */
        void                    *param_ptr
    )
{ /* Body */
    _mqx_uint result = MQX_OK;
    switch(cmd) {
        case NANDFLASH_IOCTL_GET_ID:
            /* Get the NAND Flash ID */
            if ((result = nfc_dma_read_ID(handle_ptr, param_ptr, 4)) == NANDFLASHERR_NO_ERROR)
            {
                result = MQX_OK;
            }
            break;
        default:
            result = IO_ERROR_INVALID_IOCTL_CMD;
            break;
    }
    return result;
}
#endif

/* EOF */
