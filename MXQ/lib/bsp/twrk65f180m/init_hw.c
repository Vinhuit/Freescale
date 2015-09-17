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
*   This file contains flash boot code to initialize chip selects,
*   disable the watchdog timer and initialize the PLL.
*
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"
#include "bsp_prv.h"


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _bsp_watchdog_disable
* Returned Value   : void
* Comments         :
*   Disable watchdog timer
*
*END*-------------------------------------------------------------------------*/

void _bsp_watchdog_disable(void) 
{
    WDOG_MemMapPtr reg = WDOG_BASE_PTR;

    /* NOTE: DO NOT SINGLE STEP THROUGH THIS FUNCTION!!!
    * There are timing requirements for the execution of the unlock. If
    * you single step through the code you will cause the CPU to reset.
    */

    /* unlock watchdog */
    reg->UNLOCK = 0xc520;
    reg->UNLOCK = 0xd928;

    /* disable watchdog */
    reg->STCTRLH &= ~(WDOG_STCTRLH_WDOGEN_MASK);
}

/*FUNCTION*---------------------------------------------------------------------
* 
* Function Name    : _bsp_flexbus_setup
* Returned Value   :  none
* Comments         :
*    Setup FlexBus pins for early MRAM operation
*
*END*-------------------------------------------------------------------------*/
static void _bsp_flexbus_setup (void) 
{
    #define ALT5                    0x05
    #define OFF_CHIP_ACCESS_ALLOW   3

    PORT_MemMapPtr  pctl;
    SIM_MemMapPtr   sim = SIM_BASE_PTR;
    FB_MemMapPtr    fb_ptr = FB_BASE_PTR;

    /* Enable clock to FlexBus module */
    SIM_SCGC7 |= SIM_SCGC7_FLEXBUS_MASK;
    sim->SOPT2 |= SIM_SOPT2_FBSL(OFF_CHIP_ACCESS_ALLOW);

    /* Configure flexbus fba_xx address */
    /* 1. set pin mux as fba_xx function */
    /* 2. no drive strength set */
    PORTB_PCR20 = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[31]  */
    PORTB_PCR21 = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[30]  */
    PORTB_PCR22 = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[29]  */
    PORTB_PCR23 = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[28]  */
    PORTC_PCR12 = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[27]  */
    PORTC_PCR13 = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[26]  */
    PORTC_PCR14 = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[25]  */
    PORTC_PCR15 = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[24]  */
    PORTB_PCR6  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[23]  */
    PORTB_PCR7  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[22]  */
    PORTB_PCR8  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[21]  */
    PORTB_PCR9  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[20]  */
    PORTB_PCR10 = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[19]  */
    PORTB_PCR11 = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[18]  */
    PORTB_PCR16 = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[17]  */
    PORTB_PCR17 = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[16]  */
    PORTB_PCR18 = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[15]  */
    PORTC_PCR0  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[14]  */
    PORTC_PCR1  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[13]  */
    PORTC_PCR2  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[12]  */
    PORTC_PCR4  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[11]  */
    PORTC_PCR5  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[10]  */
    PORTC_PCR6  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[9]   */
    PORTC_PCR7  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[8]   */
    PORTC_PCR8  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[7]   */
    PORTC_PCR9  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[6]   */
    PORTC_PCR10 = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[5]   */
    PORTD_PCR2  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[4]   */
    PORTD_PCR3  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[3]   */
    PORTD_PCR4  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[2]   */
    PORTD_PCR5  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[1]   */
    PORTD_PCR6  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /*  fb_ad[0]   */

    /* Config control signals */
    PORTC_PCR3  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /* fb_clkout, useless for mram  */
    PORTC_PCR11 = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /* fb_rw_b                      */
    PORTB_PCR19 = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /* fb_oe_b                      */
    PORTD_PCR1  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /* fb_cs0_b                     */
    PORTD_PCR0  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /* fb_ale                       */
    PORTC_PCR16 = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /* fb_be2                       */
    PORTC_PCR17 = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /* fb_be3                       */
    PORTC_PCR18 = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /* fb_be1                       */
    PORTC_PCR19 = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;     /* fb_be0                       */

    /* set be0/1 to MRAM */
    FB_CSPMCR |= 0x02200000;
}

/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _bsp_flexbus_mram_setup
* Returned Value   :
* Comments         :
*    Setup FlexBus for MRAM operation
*
*END*-------------------------------------------------------------------------*/
static void _bsp_flexbus_mram_setup (const uint32_t base_address)
{
    FB_MemMapPtr fb_ptr = FB_BASE_PTR;

     /* Enable external MRAM mapped on CS0 */
     /* Set Base address */
     FB_CSAR0 = base_address;

     /* Enable cs signal */
     FB_CSMR0 |= FB_CSMR_V_MASK;

     /* left justified mode cs bit9 = 0 for twr */
     FB_CSCR0 &= ~FB_CSCR_BLS_MASK;
     /* 8-bit port */
     FB_CSCR0 &= ~FB_CSCR_PS(3);
     FB_CSCR0 |= FB_CSCR_PS(1);

     /* set Auto-ack */
     FB_CSCR0 |= FB_CSCR_AA_MASK;
     /* set BEM set to 1 */
     FB_CSCR0 |= FB_CSCR_BEM_MASK;
     /* ASET 01 */
     FB_CSCR0 &= ~FB_CSCR_ASET(0x3); 
     FB_CSCR0 |= FB_CSCR_ASET(0x1); 
     /* RDAH 00 */
     FB_CSCR0 &= ~FB_CSCR_RDAH(0x3);
     FB_CSCR0 |= FB_CSCR_RDAH(0x0);
     /* WRAH 00 */
     FB_CSCR0 &= ~FB_CSCR_WRAH(0x3);
     FB_CSCR0 |= FB_CSCR_WRAH(0x0);
     /* WS 06 */
     FB_CSCR0 &= ~FB_CSCR_WS(0x3F);
     FB_CSCR0 |= FB_CSCR_WS(0x0);

     /* set BEM set to 1 */
     FB_CSCR0 |= FB_CSCR_BEM_MASK;
     /* address bit24-16 are not decoded */
     FB_CSMR0 |= FB_CSMR_BAM(0x1ff);

     /* disable burst read and write enable */
     FB_CSCR0 &= ~(FB_CSCR_BSTW_MASK|FB_CSCR_BSTR_MASK);
}

/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _bsp_flexbus_lcd_setup
* Returned Value   :
* Comments         :
*    Setup FlexBus for LCD operation
*
*END*-------------------------------------------------------------------------*/
void _bsp_flexbus_lcd_setup (const uint32_t base_address)
{
    FB_MemMapPtr fb_ptr = FB_BASE_PTR;

    /* Enable external LCD mapped on CS0 */
    fb_ptr->CS[0].CSAR = base_address;
    fb_ptr->CS[0].CSCR = FB_CSCR_BLS_MASK |
                         FB_CSCR_AA_MASK  |
                         FB_CSCR_PS(2)    |
                         FB_CSCR_BEM_MASK;

    /*
     * The address range is set to 128K because
     * the DC signal is connected on address wire
     */
    fb_ptr->CS[0].CSMR = FB_CSMR_BAM(1) | FB_CSMR_V_MASK;      
}

/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _bsp_flexbus_pccard_setup
* Returned Value   :
* Comments         :
*    Setup flexbus for pccard operation
*
*END*-------------------------------------------------------------------------*/
void _bsp_flexbus_pccard_setup (const uint32_t base_address)
{
    FB_MemMapPtr fb_ptr = FB_BASE_PTR;

    /* invalidate CS configuration first */
    fb_ptr->CS[1].CSMR = 0; 

    /* Enable external PCCARD mapped on CS1 */
    fb_ptr->CS[1].CSAR = base_address;
    /* CS1 control (8bit data, 5 wait states) */
    fb_ptr->CS[1].CSCR = FB_CSCR_AA_MASK |
                         FB_CSCR_WS(11) |
                         FB_CSCR_PS(1) |
                         FB_CSCR_BEM_MASK; 

    /* CS1 address mask (64 KB) and enable */
    fb_ptr->CS[1].CSMR = FB_CSMR_BAM(0) | FB_CSMR_V_MASK; 
}

/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _bsp_sdramc_setup
* Returned Value   :
* Comments         :
*    Setup SDRAMC for sdram operation
*
*END*-------------------------------------------------------------------------*/
void _bsp_sdramc_setup(void)
{
    SDRAM_MemMapPtr sdram_ptr = SDRAM_BASE_PTR;
    uint32_t delay;
    uint32_t mrs_dat;
    
    //60MHz, 4096 ROWs, 64ms
    //1. each row refresh time = (64ms/4096)= 15.625us
    //2. bus clock cycles = 60MHz * 15.625 = 937.5 = 937
    //3. RC = 937/16  - 1 = 58.56 - 1 = 58 -1 = 57 = 0x39
    sdram_ptr->CTRL = SDRAM_CTRL_RTIM(1)|SDRAM_CTRL_RC(57);
    //Set Base address
    sdram_ptr->BLOCK[0].AC = BSP_SDRAM_BASE |SDRAM_AC_CASL(1)|SDRAM_AC_CBM(3)|SDRAM_AC_PS(0);
    // 64M Byte 
    sdram_ptr->BLOCK[0].CM = 0x00FF0001;
    
    /* Launch the precharge all command. */
    sdram_ptr->BLOCK[0].AC |= SDRAM_AC_IP_MASK; /* Trigger the command. */
    /* After the triggered, the next write to an appropriate SDRAM address
     * generates the PALL command to the SDRAM block. */
    *((uint8_t *)(BSP_SDRAM_BASE)) = 0xFF;
    
    /* Launch the refresh command. */
    sdram_ptr->BLOCK[0].AC |= SDRAM_AC_RE_MASK; /* Trigger the command. */
 
    /* wait 8 refresh commands. */
    delay = 2000;
    while (delay--);
    
    /* Launch the MRS commands. */
    sdram_ptr->BLOCK[0].AC |= SDRAM_AC_IMRS_MASK; /* Trigger the command. */
    /* Burst len = 000, Burst type = 0, CAS latency = 001, Open mode = 00,
    * Write burst mode = 0 */
    mrs_dat = 0x10; 
    mrs_dat |= BSP_SDRAM_BASE;
    *((uint8_t *)(mrs_dat)) = 0xAA;
}

/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : init_hardware
* Returned Value   : void
* Comments         :
*   Initialize Kinetis device.
*
*END*-------------------------------------------------------------------------*/

void init_hardware(void)
{
#if MQX_ENABLE_LOW_POWER
    /* Reset from LLWU wake up source */
    if (_lpm_get_reset_source() == MQX_RESET_SOURCE_LLWU)
    {
        PMC_REGSC |= PMC_REGSC_ACKISO_MASK;
    }
#endif
    
#if PE_LDD_VERSION
    /*  Watch Dog disabled by CPU bean (need to setup in CPU Inspector) */
    __pe_initialize_hardware();
#else
    _bsp_initialize_hardware();
    
#endif

    /* Enable pin clocks */
    _bsp_gpio_io_init ();

#if BSPCFG_ENABLE_FLEXBUS_MRAM
    /* Initialize FlexBus */
    _bsp_flexbus_setup();
    /* Initialize MRAM */
    _bsp_flexbus_mram_setup((uint32_t)BSP_EXTERNAL_MRAM_RAM_BASE);
#endif
    
#if BSPCFG_ENABLE_SDRAMC
    /* Enable SDRAMC clocks */
    _bsp_sdramc_io_init();
    /* Initialize SDRAM. */
    _bsp_sdramc_setup();
#endif /* BSPCFG_ENABLE_SDRAM */
}
