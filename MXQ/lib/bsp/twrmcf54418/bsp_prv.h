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
*   This file contains the definitions of constants and structures
*   required for initialization of the card.
*   $Header:bsp_prv.h, 4, 3/15/2007 11:19:19 AM, Jim Gard$
*   $NoKeywords$
*
*
*END************************************************************************/

#ifndef _bsp_prv_h_
#define _bsp_prv_h_ 1

#ifdef __cplusplus
extern "C" {
#endif

/* NAND Flash info */
#define NANDFLASH_PHYSICAL_PAGE_SIZE      2048
#define NANDFLASH_SPARE_AREA_SIZE         64
#define NANDFLASH_BLOCK_SIZE              131072 /* 128kB */

/*
 * SDRAM initialization
 */
 
#define SDRAM_DDR1  0x0
#define SDRAM_DDR2  0x4

/* Defines for different CAS latency settings */
#define CASL_2      0x2
#define CASL_2_5    0x102
#define CASL_3      0x3
#define CASL_4      0x4
#define CASL_5      0x5
#define CASL_6      0x6
#define CASL_7      0x7

/* Define a structure used to pass info on the SDRAM into the init */
typedef struct {
    uint32_t memtype;        // type of memory (SDRAM_DDR1 and SDRAM_DDR2 are valid values) 
    uint32_t num_row_addr;   // number of row addresses for the sdram
    uint32_t num_col_addr;   // number of column addresses for the sdram
    uint32_t num_banks;      // number of banks in the memory (valid values are 4 and 8)
    uint32_t casl;           // CAS latency setting (use defines above for valid values)
    uint32_t trrd;           // ACTIVE a to ACTIVE b command delay (trrd) in ns
    uint32_t twtr;           // WRITE to READ delay (twtr) in ns
    uint32_t trtp;           // READ to PRECHARDGE delay in ns
    uint32_t twr;            // WRITE recovery time in ns
    uint32_t trp;            // PRECHARGE command period in ns
    uint32_t tmrd;           // LOAD MODE REGISTER command cycle time in ns
    uint32_t tfaw;           // 4-bank activate period in ns
    uint32_t tras_min;       // minimum row active time (ACTIVATE to PRE) in ns
    uint32_t tras_max;       // max row active time in ns
    uint32_t trc;            // ACTIVATE to ACTIVATE delay in ns
    uint32_t trfc;           // REFRESH command period in ns
    uint32_t trcd;           // ACTIVE to READ/WRITE delay in ns
    uint32_t trefi;          // REF to REF internal in ns (NOTE: this is normall spec's in us)
    uint32_t xard;           // self refresh exit time in clocks
    uint32_t tmod;           // MRS to ODT enable delay in ns (use SDCLK period x 2 for DDR1)    
        
} SDRAM_INFO;

/*
**  FUNCTION PROTOTYPES
*/
void mcf5441_init(void);

#if BSPCFG_ENABLE_CPP
extern void    __cpp_init(void);
#endif
//extern void    _ExitProcess(void); /* Codewarrior */

extern void    _bsp_enet_gpio_init(uint32_t fecnum, uint32_t flags);
extern void    _bsp_dspi_gpio_init(void);
extern void    _bsp_exit_handler(void);
extern void    _bsp_timer_isr(void *);
extern uint32_t _bsp_uart_frequency(void);

extern void _bsp_spin_time(uint32_t);    // Wait by spinning for N microseconds

/* I/O initialization controlled by initialization structures for each
** channel
*/

extern MCF54XX_UART_SERIAL_INIT_STRUCT  _bsp_uart0_init;
extern MCF54XX_UART_SERIAL_INIT_STRUCT  _bsp_uart2_init;
extern MCF54XX_UART_SERIAL_INIT_STRUCT  _bsp_uart4_init;
extern MCF54XX_UART_SERIAL_INIT_STRUCT  _bsp_uart5_init;
extern MCF54XX_UART_SERIAL_INIT_STRUCT  _bsp_uart6_init;

extern MCF54XX_I2C_INIT_STRUCT _bsp_i2c0_init;
extern MCF54XX_I2C_INIT_STRUCT _bsp_i2c5_init;
extern MCF54XX_I2S_INIT_STRUCT _bsp_i2s0_init;
extern MCF54XX_I2S_INIT_STRUCT _bsp_i2s1_init;

extern const SPI_INIT_STRUCT _bsp_spi0_init;
extern const SPI_INIT_STRUCT _bsp_spi1_init;

extern const PCCARDFLEXBUS_INIT_STRUCT  _bsp_cfcard_init;

extern NANDFLASH_INIT_STRUCT _bsp_nandflash_init;

extern void _bsp_flexbus_pccard_setup (const uint32_t base_address);
extern const ESDHC_INIT_STRUCT _bsp_esdhc0_init;

extern const struct usb_ehci_dev_init_struct _ehci0_dev_init_param;
extern const struct usb_ehci_host_init_struct _ehci0_host_init_param;
extern const struct usb_ehci_host_init_struct _ehci1_host_init_param;
                                                                     
#ifdef __cplusplus
}
#endif

#endif
/* EOF */

