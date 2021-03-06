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
* See license agreement file for full license terms including other restrictions.
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

    /* Enable clock to FlexBus module */
#ifdef SIM_SCGC7_FLEXBUS_MASK
    sim->SCGC7 |= SIM_SCGC7_FLEXBUS_MASK;
#endif

#ifdef SIM_SOPT2_FBSL
    sim->SOPT2 |= SIM_SOPT2_FBSL(OFF_CHIP_ACCESS_ALLOW);
#endif

    pctl = (PORT_MemMapPtr)PORTB_BASE_PTR;
    pctl->PCR[10] = PORT_PCR_MUX(ALT5); /* FB_AD19 */
    pctl->PCR[11] = PORT_PCR_MUX(ALT5); /* FB_AD18 */
    pctl->PCR[16] = PORT_PCR_MUX(ALT5); /* FB_AD17 */
    pctl->PCR[17] = PORT_PCR_MUX(ALT5); /* FB_AD16 */
#if !BSPCFG_ENABLE_SAI
    pctl->PCR[18] = PORT_PCR_MUX(ALT5); /* FB_AD15 */
    pctl->PCR[19] = PORT_PCR_MUX(ALT5); /* FB_OE_B */
#endif
    pctl->PCR[20] = PORT_PCR_MUX(ALT5); /* FB_AD31 */
    pctl->PCR[21] = PORT_PCR_MUX(ALT5); /* FB_AD30 */
    pctl->PCR[22] = PORT_PCR_MUX(ALT5); /* FB_AD29 */
    pctl->PCR[23] = PORT_PCR_MUX(ALT5); /* FB_AD28 */

    pctl = (PORT_MemMapPtr)PORTC_BASE_PTR;
    pctl->PCR[0] = PORT_PCR_MUX(ALT5); /* FB_AD14 */
#if !BSPCFG_ENABLE_SAI
    pctl->PCR[1] = PORT_PCR_MUX(ALT5); /* FB_AD13 */
    pctl->PCR[5] = PORT_PCR_MUX(ALT5); /* FB_AD10 */
    pctl->PCR[8] = PORT_PCR_MUX(ALT5); /* FB_AD7 */
#endif
    pctl->PCR[2] = PORT_PCR_MUX(ALT5); /* FB_AD12 */
    pctl->PCR[3] = PORT_PCR_MUX(ALT5); /* FB_CLKOUT */
    pctl->PCR[4] = PORT_PCR_MUX(ALT5); /* FB_AD11 */
    pctl->PCR[6] = PORT_PCR_MUX(ALT5); /* FB_AD9 */
    pctl->PCR[7] = PORT_PCR_MUX(ALT5); /* FB_AD8 */
    pctl->PCR[9] = PORT_PCR_MUX(ALT5); /* FB_AD6 */
    pctl->PCR[10] = PORT_PCR_MUX(ALT5); /* FB_AD5 */
    pctl->PCR[11] = PORT_PCR_MUX(ALT5); /* FB_RW_B */
    pctl->PCR[12] = PORT_PCR_MUX(ALT5); /* FB_AD27 */
    pctl->PCR[13] = PORT_PCR_MUX(ALT5); /* FB_AD26 */
    pctl->PCR[14] = PORT_PCR_MUX(ALT5); /* FB_AD25 */
    pctl->PCR[15] = PORT_PCR_MUX(ALT5); /* FB_AD24 */
    
    pctl = (PORT_MemMapPtr)PORTD_BASE_PTR;
    pctl->PCR[0] = PORT_PCR_MUX(ALT5); /* FB_ALE */
    pctl->PCR[1] = PORT_PCR_MUX(ALT5); /* FB_CS0_b */
    pctl->PCR[2] = PORT_PCR_MUX(ALT5); /* FB_AD4 */
    pctl->PCR[3] = PORT_PCR_MUX(ALT5); /* FB_AD3 */
    pctl->PCR[4] = PORT_PCR_MUX(ALT5); /* FB_AD2 */
    pctl->PCR[5] = PORT_PCR_MUX(ALT5); /* FB_AD1 */
    pctl->PCR[6] = PORT_PCR_MUX(ALT5); /* FB_AD0 */

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
    /* Initialize FlexBus */
    _bsp_flexbus_setup();
}
/* EOF */
