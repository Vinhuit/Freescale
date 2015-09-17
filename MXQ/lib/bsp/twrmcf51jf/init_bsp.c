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
*   This file contains the source functions for functions required to
*   specifically initialize the card.
*
 *
*END************************************************************************/

#include "mqx_inc.h"
#include "bsp.h"
#include "bsp_prv.h"

const char      *_mqx_bsp_revision = REAL_NUM_TO_STR(BSP_REVISION);
const char      *_mqx_io_revision  = REAL_NUM_TO_STR(IO_REVISION);

static void    _bsp_setup_watchdog(void);

/** Pre initialization - initializing requested modules for basic run of MQX.
 */
int _bsp_pre_init(void) {
    KERNEL_DATA_STRUCT_PTR        kernel_data;
    uint32_t                       result;

    _GET_KERNEL_DATA(kernel_data);

    _mqx_set_cpu_type(MQX_CPU);

#if MQX_EXIT_ENABLED
    /* Set the bsp exit handler, called by _mqx_exit */
    _mqx_set_exit_handler(_bsp_exit_handler);
#endif

    /* === Debugging is not allowed from here === */
    /* Initialize the interrupt handling */
    result = _psp_int_init(BSP_FIRST_INTERRUPT_VECTOR_USED, BSP_LAST_INTERRUPT_VECTOR_USED);
    /* === Debugging may now resume === */
    if (result != MQX_OK) {
        return result;
    }

    /* set possible new interrupt vector table - if MQX_ROM_VECTORS = 0 switch to
    ram interrupt table which was initialized in _psp_int_init) */
    (void)_int_set_vector_table(BSP_INTERRUPT_VECTOR_TABLE);

    /** bsp low level internal initialization. ***/
    _bsp_low_level_init();


    /* Initialize the system ticks */
    kernel_data->TIMER_HW_REFERENCE = (BSP_BUS_CLOCK / BSP_ALARM_FREQUENCY);
    /* Install the timer with interrupt */
    _mtim16_timer_install_kernel(BSP_TIMER, BSP_ALARM_FREQUENCY, BSP_BUS_CLOCK, BSPCFG_TIMER_INT_LEVEL, TRUE);


    /* MCG initialization and internal oscillators trimming */
    if (CM_ERR_OK != _bsp_set_clock_configuration(BSP_CLOCK_CONFIGURATION_AUTOTRIM))
    {
        return MQX_TIMER_ISR_INSTALL_FAIL;
    }

    if (CM_ERR_OK != _bsp_osc_autotrim())
    {
        return MQX_TIMER_ISR_INSTALL_FAIL;
    }

    /* Switch to startup clock configuration */
    if (CM_ERR_OK != _bsp_set_clock_configuration(BSP_CLOCK_CONFIGURATION_STARTUP))
    {
        return MQX_TIMER_ISR_INSTALL_FAIL;
    }


#if BSPCFG_ENABLE_CPP
   /* initialize C++ constructors */
   __cpp_init();
#endif  /* BSPCFG_ENABLE_CPP */

   /* Install low power support */
#if MQX_ENABLE_LOW_POWER
     /* PMPROT write once register is set in _bsp_low_level_init() */
    _lpm_install (LPM_CPU_OPERATION_MODES, LPM_OPERATION_MODE_RUN);
#endif /* MQX_ENABLE_LOW_POWER */
    
   return 0;
}

/** Initialization - called from init task, usually for io initialization.
 */
int _bsp_init(void) {
    uint32_t result;

#if BSPCFG_ENABLE_IO_SUBSYSTEM
    /*------------------------------------------------------------------------*/
    /*
    ** Initialize the I/O Sub-system
    */
    result = _io_init();
    if (result != MQX_OK) {
        return result;
    }

/* Install device drivers */

#if BSPCFG_ENABLE_TTYA
    _kuart_polled_install("ttya:", &_bsp_uart0_init, _bsp_uart0_init.QUEUE_SIZE);
#endif

#if BSPCFG_ENABLE_ITTYA
    _kuart_int_install("ittya:", &_bsp_uart0_init, _bsp_uart0_init.QUEUE_SIZE);
#endif

#if BSPCFG_ENABLE_TTYB
    _kuart_polled_install("ttyb:", &_bsp_uart1_init, _bsp_uart1_init.QUEUE_SIZE);
#endif

#if BSPCFG_ENABLE_ITTYB
    _kuart_int_install("ittyb:", &_bsp_uart1_init, _bsp_uart1_init.QUEUE_SIZE);
#endif

#if BSPCFG_ENABLE_SPI0
   _spi16_polled_install("spi0:", &_bsp_spi0_init);
#endif

#if BSPCFG_ENABLE_ISPI0
   _spi16_int_install("ispi0:", &_bsp_spi0_init);
#endif

#if BSPCFG_ENABLE_SPI1
   _spi16_polled_install("spi1:", &_bsp_spi1_init);
#endif

#if BSPCFG_ENABLE_ISPI1
   _spi16_int_install("ispi1:", &_bsp_spi1_init);
#endif

#if BSPCFG_ENABLE_I2C0
    _ki2c_polled_install("i2c0:", &_bsp_i2c0_init);
#endif
#if BSPCFG_ENABLE_II2C0
    _ki2c_int_install("ii2c0:", &_bsp_i2c0_init);
#endif

#if BSPCFG_ENABLE_I2C1
    _ki2c_polled_install("i2c1:", &_bsp_i2c1_init);
#endif
#if BSPCFG_ENABLE_II2C1
    _ki2c_int_install("ii2c1:", &_bsp_i2c1_init);
#endif

#if BSPCFG_ENABLE_I2C2
    _ki2c_polled_install("i2c2:", &_bsp_i2c2_init);
#endif
#if BSPCFG_ENABLE_II2C2
    _ki2c_int_install("ii2c2:", &_bsp_i2c2_init);
#endif

#if BSPCFG_ENABLE_I2C3
    _ki2c_polled_install("i2c3:", &_bsp_i2c3_init);
#endif
#if BSPCFG_ENABLE_II2C3
    _ki2c_int_install("ii2c3:", &_bsp_i2c3_init);
#endif

/* Install the USB DCD driver */
#if BSPCFG_ENABLE_USBDCD
    _kusb_dcd_polled_install("usbdcd:", &_bsp_usb_dcd_init);
#endif
#if BSPCFG_ENABLE_IUSBDCD
    _kusb_dcd_int_install("usbdcd:", &_bsp_usb_dcd_init);
#endif

#if BSPCFG_ENABLE_FLASHX
    _io_flashx_install("flashx:", &_bsp_flashx_init);
#endif

/* Install the PCCard Flash drivers */
#if BSPCFG_ENABLE_PCFLASH
    _io_pccardflexbus_install("pccarda:", (PCCARDFLEXBUS_INIT_STRUCT *) &_bsp_cfcard_init);
    _io_apcflash_install("pcflasha:");
#endif

/* Install the GPIO driver */
#if BSPCFG_ENABLE_GPIODEV
    _io_gpio_install("gpio:");
#endif

#if BSPCFG_ENABLE_ADC
    _io_adc_install("adc:", (void *) &_bsp_adc0_init);
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
{
    _int_mask_all();
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_setup_watchdog
* Returned Value   : uint32_t
* Comments         :
*   This function initializes the watchdog timer according to the definition
*   MCF51JF_WATCHDOG_INITIALIZATION in <BOARD_NAME>.h
*
*END*----------------------------------------------------------------------*/

static void _bsp_setup_watchdog
    (
        void
    )
{

    __RESET_WATCHDOG();

#if MCF51JF_WATCHDOG_INITIALIZATION != MCF51JF_WATCHDOG_DISABLED
    SIM_SOPT1 = 0
#if MCF51JF_WATCHDOG_INITIALIZATION == MCF51JF_WATCHDOG_RESET
#error User manual says SOPT should be set to zero always
             | SIM_COPC_COPT(0)
#endif
             | SIM_COPC_COPT(MCF51JF_WATCHDOG_DELAY)&(~ SIM_COPC_COPW_MASK)
             | SIM_COPC_COPW_MASK;
#endif
} /* Endbody */

/* EOF */
