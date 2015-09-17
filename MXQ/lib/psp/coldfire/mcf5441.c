
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
*   This file contains utiltity functions for use with a mcf5441x.
*
*
*END************************************************************************/

#include "mqx_inc.h"
#include "mcf5441.h"
#include "mmu.h"

/* Copies of control registers */
volatile uint32_t _psp_saved_cacr = 0;
volatile uint32_t _psp_saved_acr0 = 0;
volatile uint32_t _psp_saved_acr1 = 0;
volatile uint32_t _psp_saved_acr2 = 0;
volatile uint32_t _psp_saved_acr3 = 0;
volatile uint32_t _psp_saved_mbar = 0x10000000;

/*!
 * \brief Initilize the support functions for the mcf548x
 * 
 * \param[in] param dummy parameter
 */
void _mcf5441_initialize_support
   (
      /* [IN] dummy parameter */
      uint32_t param
   )
{
#if PSP_HAS_SUPPORT_STRUCT
   KERNEL_DATA_STRUCT_PTR kernel_data;


   _GET_KERNEL_DATA(kernel_data);

   kernel_data->PSP_SUPPORT_PTR = _mem_alloc_system_zero((uint32_t)sizeof(PSP_SUPPORT_STRUCT) );

#endif  // PSP_HAS_SUPPORT_STRUCT
}

/*!
 * \brief This function enables the cache (Instruction cache only at the moment)
 * 
 * \param flag 
 */
void _mcf5441_icache_enable
   (
      uint32_t flag
   )
{
    uint32_t tmp = _PSP_GET_CACR();

    tmp &= ~(MCF54XX_CACR_IEC        // Disable instruction cache
        | MCF54XX_CACR_DNFB
        | MCF54XX_CACR_IDPI
        | MCF54XX_CACR_IHLCK
        | MCF54XX_CACR_IDCM
        | MCF54XX_CACR_IDSP);

    tmp &= ~MCF5441_CACR_CLEAR_BITS;

    // Invalidate the branch history table
    _PSP_SET_CACR(tmp | MCF54XX_CACR_BCINVA);

    // Invalidate instr cache
    _PSP_SET_CACR(tmp | MCF54XX_CACR_ICINVA);

    tmp |= flag;                 // Enable any bits specified by user
    tmp |= MCF54XX_CACR_IEC;     // Enable instruction cache
    
    _PSP_SET_CACR(tmp);
}


/*!
 * \brief This function disables the instruction cache
 */
void _mcf5441_icache_disable
   (
      void
   )
{
    uint32_t tmp = _PSP_GET_CACR();

    tmp &= ~(MCF54XX_CACR_IEC);
    tmp |= MCF54XX_CACR_ICINVA;
    _PSP_SET_CACR(tmp);

    // pipeline flush
    _ASM_NOP();
}


/*!
 * \brief This function invalidates the entire instruction cache
 */
void _mcf5441_icache_invalidate
   (
      void
   )
{

    uint32_t tmp = _PSP_GET_CACR();

    // Invalidate instr and branch cache
    tmp |= MCF54XX_CACR_ICINVA | MCF54XX_CACR_BCINVA;

    _PSP_SET_CACR(tmp);
}

//marks@4/07/04 This function is in dispatch.cw
//void _icache_invalidate_mlines(pointer,_mqx_uint,_mqx_uint);

/*!
 * \brief This function enables the DATA cache.
 * 
 * \param flag 
 */
void _mcf5441_dcache_enable
   (
      uint32_t flag
   )
{
    uint32_t tmp = _PSP_GET_CACR();

    // WARNING: DDPI must be clear otherwise ethernet data that is DMA'd
    // to a buffer cannot be invalidated before looking at it.  Thus,
    // when the data is read out of the DMA'd buffer, if the cache line
    // was valid we would be looking at old data.

    tmp &= ~(MCF54XX_CACR_DEC   // Disable data cache
        | MCF54XX_CACR_DNFB
        | MCF54XX_CACR_DDPI
        | MCF54XX_CACR_DHLCK
        | MCF54XX_CACR_DDSP);

    tmp &= ~MCF5441_CACR_CLEAR_BITS;

    // Invalidate the entire data cache
    _PSP_SET_CACR(tmp | MCF54XX_CACR_DCINVA);
   
    flag &= ~MCF54XX_CACR_DDPI;     // MUST always invalidate on cpushl
    tmp |= flag;                    // Enable any bits specified by user
    tmp |= MCF54XX_CACR_DEC;        // Enable data cache

    _PSP_SET_CACR(tmp);
}

/*!
 * \brief This function disables the data cache
 */
void _mcf5441_dcache_disable
   (
      void
   )
{ /* Body */

   uint32_t tmp = _PSP_GET_CACR();

   _DCACHE_FLUSH();

   tmp &= ~(MCF54XX_CACR_DEC);
   tmp |= MCF54XX_CACR_DCINVA;
   _PSP_SET_CACR(tmp);

   // Pipeline flush
   _ASM_NOP();

} /* Endbody */

/*!
 * \brief This function invalidates the entire data cache
 */
void _mcf5441_dcache_invalidate
   (
      void
   )
{
    uint32_t tmp = _PSP_GET_CACR();

    // Invalidate data cache
    tmp |= MCF54XX_CACR_DCINVA;

    _PSP_SET_CACR(tmp);
}


/*!
 * \brief Initialize the mmu and set default properties for regions not mapped by 
 *  the ACR registers.
 * 
 * \param[in] mmu_init default properties
 */
void _mmu_init
   (
      // [IN] default properties
      void   *mmu_init
   )
{ /* Body */

   PSP_MMU_INIT_STRUCT_PTR mmu = mmu_init;
   KERNEL_DATA_STRUCT_PTR  kernel_data;
   PSP_SUPPORT_STRUCT_PTR  psp_support_ptr;
   uint32_t  mode;
   uint32_t  tmp;
   _mqx_int i;

   _GET_KERNEL_DATA(kernel_data);
   psp_support_ptr = (PSP_SUPPORT_STRUCT_PTR)kernel_data->PSP_SUPPORT_PTR;

   if (psp_support_ptr == NULL || mmu == 0) {
      _mqx_fatal_error(MQX_INVALID_POINTER);
   } /* Endif */

   /* Stop and invalidate the cache */
   _DCACHE_DISABLE();
   _ICACHE_DISABLE();

   _mem_zero(psp_support_ptr->ACR_VALS, sizeof(psp_support_ptr->ACR_VALS));

   mode = mmu->INITIAL_CACR_ENABLE_BITS & ~(MCF54XX_CACR_EUSP) | MCF54XX_CACR_SPA;  // do not remove SPA flag - fn for flush/invalidate cache use phy address features
   tmp = _PSP_GET_CACR();
   tmp |= mode;
   _PSP_SET_CACR(tmp);

} /* Endbody */


/*!
 * \brief Enables all ACRs in use
 */
void _mmu_enable
   (
      void
   )
{ /* Body */

   KERNEL_DATA_STRUCT_PTR  kernel_data;
   PSP_SUPPORT_STRUCT_PTR  psp_support_ptr;

   _GET_KERNEL_DATA(kernel_data);
   psp_support_ptr = (PSP_SUPPORT_STRUCT_PTR)kernel_data->PSP_SUPPORT_PTR;

   if (psp_support_ptr == NULL) {
      _mqx_fatal_error(MQX_INVALID_POINTER);
   } /* Endif */

   __psp_enable_acrs(psp_support_ptr->ACR_VALS);

} /* Endbody */


/*!
 * \brief Disables all ACRs
 */
void _mmu_disable
   (
      void
   )
{ /* Body */

   __psp_clear_acrs();

} /* Endbody */


/*!
 * \brief Change the properties for a given region
 * 
 * \param[in] mem_ptr the starting location of the memory block 
 * \param[in] mem_size the mask to apply to the memory block
 * \param[in] mem_attrs flags indicating what type of memory this is
 *
 * \return uint32_t MQX_OK or an error code
 */
_mqx_uint _mmu_add_region
   (
      /* [IN] the starting location of the memory block */
      unsigned char *mem_ptr,

      /* [IN] the mask to apply to the memory block */
      _mem_size mem_size,

      /* [IN] flags indicating what type of memory this is */
      _mqx_uint mem_attrs
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR  kernel_data;
   PSP_SUPPORT_STRUCT_PTR  psp_support_ptr;
   uint32_t                 acr_val = 0;
   uint32_t                 acr_num, base, base_mask;
   _mqx_int                i;

   _GET_KERNEL_DATA(kernel_data);
   psp_support_ptr = (PSP_SUPPORT_STRUCT_PTR)kernel_data->PSP_SUPPORT_PTR;

   if (psp_support_ptr == NULL) {
      return MQX_OUT_OF_MEMORY;
   } /* Endif */

   // Have we used up all data ACRs?
   if (psp_support_ptr->ACR_VALS[MCF5441_FIRST_DATA_ACR + MCF5441_NUM_DATA_ACRS - 1] != 0) {
      return MCF5441_OUT_OF_ACRS;
   } /* Endif */

   // Have we used up all instruction ACRs?
   if ((mem_attrs & PSP_MMU_EXEC_ALLOWED) != 0 &&
       psp_support_ptr->ACR_VALS[MCF5441_FIRST_INSTR_ACR + MCF5441_NUM_INSTR_ACRS - 1] != 0) {
      return MCF5441_OUT_OF_ACRS;
   } /* Endif */

   /* Build value to place in ACR */
   base = (uint32_t)mem_ptr;

   if (mem_size < (16*1024*1024)) { /* Address Mask Mode (AMM) set */
      base_mask  = (mem_size-1)>>4;
      base_mask &= 0x000F0000;
      base      &= 0xFFF00000;
      acr_val   = base | base_mask;
      acr_val   |= MCF54XX_ACR_AMM;
      
   } else {
   
      base_mask  = (mem_size-1)>>8;
      base_mask &= MCF54XX_ACR_BASE_ADDR_MASK; /* 0x00ff0000 */
      base      &= MCF54XX_ACR_BASE_ADDR;      /* 0xff000000 */
      acr_val = base | base_mask;
      
   } /* Endif */

#undef  ATTRMAP
#define ATTRMAP(x,y)        { if ((mem_attrs & x) != 0) acr_val |= y; }
   
   ATTRMAP(PSP_MMU_WRITE_BUFFERED, MCF54XX_ACR_BUFFER_WRITE_ENABLE);

   if ((mem_attrs & PSP_MMU_EXEC_ALLOWED) != 0) {
      
      /* Create Code region */
      
      ATTRMAP(PSP_MMU_CODE_CACHE_INHIBITED, MCF54XX_ACR_NONCACHEABLE_MODE);
      ATTRMAP(PSP_MMU_WRITE_PROTECTED, MCF54XX_ACR_WRITE_PROTECT);
      
      // Execute cache matching on all accesses
      acr_val |=  MCF54XX_ACR_EXEC_CACHE_MATCH;

      acr_num = MCF5441_FIRST_INSTR_ACR;
      for (i = 0; i < MCF5441_NUM_INSTR_ACRS; i++, acr_num++) {
         if (psp_support_ptr->ACR_VALS[acr_num] == 0) {
            psp_support_ptr->ACR_VALS[acr_num] = acr_val | MCF54XX_ACR_ENABLE;
            break;
         } /* Endif */
      }
   } else {
   
      /* Create Data region */

      ATTRMAP(PSP_MMU_DATA_CACHE_INHIBITED, MCF54XX_ACR_NONCACHEABLE_MODE);
      ATTRMAP(PSP_MMU_WRITE_THROUGH, MCF54XX_ACR_CACHEABLE_MODE);
      ATTRMAP(PSP_MMU_WRITE_BUFFERED, MCF54XX_ACR_BUFFER_WRITE_ENABLE);
   
      // Execute cache matching on all accesses
      acr_val |=  MCF54XX_ACR_EXEC_CACHE_MATCH;
   
      acr_num = MCF5441_FIRST_DATA_ACR;
      for (i = 0; i < MCF5441_NUM_DATA_ACRS; i++, acr_num++) {
         if (psp_support_ptr->ACR_VALS[acr_num] == 0) {
            psp_support_ptr->ACR_VALS[acr_num] = acr_val | MCF54XX_ACR_ENABLE;
            break;
         } /* Endif */
      }
   } /* Endif */

   return MQX_OK;

} /* Endbody */
