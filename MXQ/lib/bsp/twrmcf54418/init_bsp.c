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
*   This file contains the source functions for functions required to
*   specifically initialize the card.
*   $Header:init_bsp.c, 7, 3/15/2007 11:39:41 AM, Jim Gard$
*   $NoKeywords$
*
*
*END************************************************************************/

#include "mqx_inc.h"
#include "bsp.h"
#include "bsp_prv.h"
#include "mmu.h"

#undef  _NELEM
#define _NELEM(ary)     (sizeof(ary) / sizeof(ary[0]))

extern unsigned long __SRAM_POOL[];

const char      *_mqx_bsp_revision = REAL_NUM_TO_STR(BSP_REVISION);
const char      *_mqx_io_revision  = REAL_NUM_TO_STR(IO_REVISION);
_mem_pool_id _BSP_sram_pool;

#define DMA_CHANNEL_TDB         0xFFFF

// Specify MQX_MONITOR_TYPE_BDM while debugging with JTAG to
// disable cache and MMU which interfere with debugging.
// volatile uint32_t _mqx_monitor_type = MQX_MONITOR_TYPE_BDM;
// #define BSP_TRAP_EXCEPTIONS  1

static void    _bsp_setup_watchdog(void);

/** Pre initialization - initializing requested modules for basic run of MQX.
 */
int _bsp_pre_init(void) {
   	KERNEL_DATA_STRUCT_PTR kernel_data;
   	uint32_t                result;
   	_mqx_int               i;

   	/* Set the CPU type */
   	_mqx_set_cpu_type(MQX_CPU);

   	/* Set the bsp exit handler, called by _mqx_exit */
   	_mqx_set_exit_handler(_bsp_exit_handler);

   	/* Initialize the MCF548x support functions */
   	_mcf5441_initialize_support(0);

   	/*
   	** Initialize the interrupt handling
   	*/
   	_int_set_vector_table(BSP_RAM_INTERRUPT_VECTOR_TABLE);

   	result =  _psp_int_init(BSP_FIRST_INTERRUPT_VECTOR_USED, BSP_LAST_INTERRUPT_VECTOR_USED);
   	if (result != MQX_OK) {
      	return result;
   	} /* Endif */

    /* Initialize the timer interrupt */
    _time_set_timer_vector(BSP_TIMER_INTERRUPT_VECTOR);
    if (_int_install_isr(BSP_TIMER_INTERRUPT_VECTOR, _bsp_timer_isr, NULL) == NULL) {
        return MQX_TIMER_ISR_INSTALL_FAIL;
    } /* Endif */

#if BSPCFG_HAS_SRAM_POOL
    /* When kernel data is placed outside of the SRAM memory create new _BSP_sram_pool in the SRAM, 
       otherwise if kernel data points to SRAM, the _BSP_sram_pool points to system pool. */
    if ( (((uint32_t)__INTERNAL_SRAM_BASE) < (uint32_t)BSP_DEFAULT_START_OF_KERNEL_MEMORY) &&
         (((uint32_t)BSP_DEFAULT_START_OF_KERNEL_MEMORY) < ((uint32_t)__INTERNAL_SRAM_BASE + (uint32_t)__INTERNAL_SRAM_SIZE)))
    {
        _BSP_sram_pool  = _mem_get_system_pool_id();
    }
    else
    {
        _BSP_sram_pool = _mem_create_pool(__SRAM_POOL, (uint32_t)__INTERNAL_SRAM_BASE + (uint32_t)__INTERNAL_SRAM_SIZE - (uint32_t)__SRAM_POOL);
    }
#endif

    _GET_KERNEL_DATA(kernel_data);

    // Initialize the slice timer to interrupt the specified
    // number of times per second
    kernel_data->TIMER_HW_REFERENCE = _pit_init_freq(BSP_TIMER, BSP_ALARM_FREQUENCY, BSP_SYSTEM_CLOCK/2, FALSE);    

   	_time_set_hwtick_function(_pit_get_hwticks, (void *)BSP_TIMER);
   	_time_set_hwticks_per_tick(kernel_data->TIMER_HW_REFERENCE);
   	_time_set_ticks_per_sec(BSP_ALARM_FREQUENCY);

   	_pit_unmask_int(BSP_TIMER);

   	// Initialize and enable the serial UART interrupts
   	_mcf5441_int_init(BSP_UART0_INT_VECTOR, BSP_UART0_INT_LEVEL, TRUE);
   	_mcf5441_int_init(BSP_UART2_INT_VECTOR, BSP_UART2_INT_LEVEL, TRUE);
   	_mcf5441_int_init(BSP_UART4_INT_VECTOR, BSP_UART4_INT_LEVEL, TRUE);
   	_mcf5441_int_init(BSP_UART6_INT_VECTOR, BSP_UART6_INT_LEVEL, TRUE);

   	// Install and mask the DMA interrupt handler
/*   
   	_int_install_isr(BSP_ENET_DMA_INTERRUPT, _mcf5445_dma_isr, (void *)0);
   	_mcf5445_int_init(BSP_ENET_DMA_INTERRUPT, BSP_ENET_DMA_INT_LEVEL, BSP_ENET_DMA_INT_PRIORITY, FALSE);
*/

   	// Initialize and disable the security engine interrupt
   	// _mcf54xx_int_init(MCF548x_INT_SEC, BSP_SEC_INT_LEVEL, /*BSP_SEC_INT_PRIORITY, */FALSE);

#if BSP_TRAP_EXCEPTIONS
   	_int_install_unexpected_isr();
#endif

   	// Always invalidate the caches even if not enabled.  This allows
   	// us to flush the cache always.  If we flush before invalidating
   	// very bad things happen.
   	_ICACHE_INVALIDATE();
   	_DCACHE_INVALIDATE();

   if (_mqx_monitor_type == MQX_MONITOR_TYPE_NONE) {

      static const PSP_MMU_INIT_STRUCT mmu_init = {
         /* We define the default cacheability of non-ACR mapped regions */
         /* as non-cacheable and unbuffered */
         MCF54XX_CACR_DDCM(MCF54XX_CACHE_NONCACHEABLE_UNBUFFERED)
      };        

      /* Initialize Cache Control Register CACR */
      _mmu_init((void *)&mmu_init);

      /* Set up 1 instruction and 1 data ACR in two separate SDRAM areas */
      /* Caution: Consider memory map in linker command file before changing regions */
      /* Note: Second arg to _mmu_add_region is used in mask value in ACR */
      
      result = _mmu_add_region(__CACHED_CODE_START, __CACHED_CODE_END - __CACHED_CODE_START, PSP_MMU_EXEC_ALLOWED);
      if (result != MQX_OK) return result; 
 
      result = _mmu_add_region(__CACHED_DATA_START, __CACHED_DATA_END - __CACHED_DATA_START, PSP_MMU_WRITE_THROUGH);
      if (result != MQX_OK) return result;
 
      /* Copy ACR table into ACR registers */
      _MMU_ENABLE();
      
      /* Enable instruction cache and branch history cache in CACR */
      _ICACHE_ENABLE(MCF54XX_CACR_IEC | MCF54XX_CACR_BEC); 
   
      /* Enable data cache bit in CACR */
      _DCACHE_ENABLE(0);
    
   } /* Endif */

#if BSPCFG_ENABLE_CPP
    /* initialize C++ constructors */
    __cpp_init();
#endif

   return 0;
}

/** Initialization - called from init task, usually for io initialization.
 */
int _bsp_init(void) {
    uint32_t result;
    /* fixme */
    VMCF5441_STRUCT_PTR reg_ptr = _PSP_GET_IPSBAR();
    
   /* Initialize RTC and MQX time */
#if BSPCFG_ENABLE_RTCDEV
   _bsp_rtc_io_init();
   _rtc_init (NULL);
#endif

#if BSPCFG_ENABLE_IO_SUBSYSTEM
   /* Initialize the I/O Sub-system */
   result = _io_init();
   if (result != MQX_OK) {
      return result;
   }
  
#if BSPCFG_ENABLE_TTYA
    _mcf54xx_uart_serial_polled_install("ttya:", &_bsp_uart0_init, _bsp_uart0_init.QUEUE_SIZE);
#endif

#if BSPCFG_ENABLE_TTYB
    _mcf54xx_uart_serial_polled_install("ttyb:", &_bsp_uart2_init, _bsp_uart2_init.QUEUE_SIZE);
#endif

#if BSPCFG_ENABLE_TTYC
    _mcf54xx_uart_serial_polled_install("ttyc:", &_bsp_uart4_init, _bsp_uart4_init.QUEUE_SIZE);
#endif

#if BSPCFG_ENABLE_TTYD
    _mcf54xx_uart_serial_polled_install("ttyd:", &_bsp_uart5_init, _bsp_uart5_init.QUEUE_SIZE);
#endif

#if BSPCFG_ENABLE_TTYE
    _mcf54xx_uart_serial_polled_install("ttye:", &_bsp_uart6_init, _bsp_uart6_init.QUEUE_SIZE);
#endif

#if BSPCFG_ENABLE_ITTYA
    _mcf54xx_uart_serial_int_install("ittya:", &_bsp_uart0_init, _bsp_uart0_init.QUEUE_SIZE);
#endif

#if BSPCFG_ENABLE_ITTYB
    _mcf54xx_uart_serial_int_install("ittyb:", &_bsp_uart2_init, _bsp_uart2_init.QUEUE_SIZE);
#endif

#if BSPCFG_ENABLE_ITTYC
    _mcf54xx_uart_serial_int_install("ittyc:", &_bsp_uart4_init, _bsp_uart4_init.QUEUE_SIZE);
#endif

#if BSPCFG_ENABLE_ITTYD
    _mcf54xx_uart_serial_int_install("ittyd:", &_bsp_uart5_init, _bsp_uart5_init.QUEUE_SIZE);
#endif

#if BSPCFG_ENABLE_ITTYE
    _mcf54xx_uart_serial_int_install("ittye:", &_bsp_uart6_init, _bsp_uart6_init.QUEUE_SIZE);
#endif

#if BSPCFG_ENABLE_SPI0
    _io_spi_install("spi0:", &_bsp_spi0_init);
#endif

#if BSPCFG_ENABLE_SPI1
    _io_spi_install("spi1:", &_bsp_spi1_init);
#endif

#if BSPCFG_ENABLE_I2C0
    _mcf54xx_i2c_polled_install("i2c0:", &_bsp_i2c0_init);
#endif

#if BSPCFG_ENABLE_II2C0
    _mcf54xx_i2c_int_install("ii2c0:", &_bsp_i2c0_init);
#endif

#if BSPCFG_ENABLE_I2C5
    _mcf54xx_i2c_polled_install("i2c5:", &_bsp_i2c5_init);
#endif

#if BSPCFG_ENABLE_II2C5
    _mcf54xx_i2c_int_install("ii2c5:", &_bsp_i2c5_init);
#endif
#if BSPCFG_ENABLE_II2S0
    _mcf54xx_i2s_int_install("ii2s0:", &_bsp_i2s0_init);
#endif
#if BSPCFG_ENABLE_II2S1
    _mcf54xx_i2s_int_install("ii2s1:", &_bsp_i2s1_init);
#endif

   /* Install the PCCard Flash drivers */
#if BSPCFG_ENABLE_PCFLASH   
   _io_pccardflexbus_install("pccarda:", (PCCARDFLEXBUS_INIT_STRUCT *) &_bsp_cfcard_init);
   _io_apcflash_install("pcflasha:");
#endif

/* install the GPIO driver */
#if BSPCFG_ENABLE_GPIODEV   
   _io_gpio_install("gpio:");
#endif

/* install external flash */
#if BSPCFG_ENABLE_NANDFLASH
    result = _io_nandflash_install(&_bsp_nandflash_init);
#endif

/* ADC initialization */
#if BSPCFG_ENABLE_ADC
   _io_adc_install("adc:", NULL);
   reg_ptr->SCM_PMM.PMM.PPMCR0 = 37;   // clk enable ADC
#endif

#if BSPCFG_ENABLE_ESDHC
    _esdhc_install ("esdhc:", &_bsp_esdhc0_init);
#endif

   /* Initialize the default serial I/O */
   _io_serial_default_init();

#endif /* BSPCFG_ENABLE_IO_SUBSYSTEM */
   
    return 0;

}


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_exit_handler
* Returned Value   : none
* Comments         :
*    This function is called when MQX exits
*
*END*----------------------------------------------------------------------*/

void _bsp_exit_handler
   (
      void
   )
{ /* Body */

   _mcf5441_int_mask_all();
   _DCACHE_DISABLE();
   _ICACHE_DISABLE();
   _MMU_DISABLE();

} /* Endbody */


/*ISR*********************************************************************
*
* Function Name    : _bsp_timer_isr
* Returned Value   : void
* Comments         :
*    The timer ISR is the interrupt handler for the clock tick.
*
*END**********************************************************************/

void _bsp_timer_isr
   (
      void   *dummy
   )
{ /* Body */

   // Acknowlege the interrupt to clear it
   _pit_clear_int(BSP_TIMER);

   _time_notify_kernel();

} /* Endbody */
