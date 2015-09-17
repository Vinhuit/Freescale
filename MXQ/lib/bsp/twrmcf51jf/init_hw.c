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


/* local function prototypes */
static void _bsp_bdm_init(void);

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_flexbus_setup
* Returned Value   :  none
* Comments         :
*    Setup FlexBus pins for early MRAM operation
*
*END*-----------------------------------------------------------------------*/

static void _bsp_flexbus_setup (void)
{
#define FLEXBUS_OFF_CHIP_ACCESS_ALLOW   3

    MXC_MemMapPtr   mxc = MXC_BASE_PTR ;
    SIM_MemMapPtr   sim = SIM_BASE_PTR;

    /* Enable clock to FlexBus module */
    sim->SCGC5 |= SIM_SCGC5_MFBUS_MASK;
    sim->SOPT6 |= SIM_SOPT6_MBSL(FLEXBUS_OFF_CHIP_ACCESS_ALLOW);

    mxc->PTAPF1 |= MXC_PTAPF1_A6(6); /* FB_AD17 */
    mxc->PTAPF4 |= MXC_PTAPF4_A1(6); /* FB_AD16 */
    mxc->PTAPF4 |= MXC_PTAPF4_A0(6); /* FB_AD15 */
    mxc->PTBPF3 |= MXC_PTBPF3_B3(6); /* FB_ALE  */
    mxc->PTBPF3 |= MXC_PTBPF3_B2(6); /* FB_CS0_b*/
    mxc->PTCPF1 |= MXC_PTCPF1_C7(6); /* FB_AD12 */
    mxc->PTCPF1 |= MXC_PTCPF1_C6(6); /* FB_AD11 */
    mxc->PTCPF3 |= MXC_PTCPF3_C2(6); /* FB_AD18 */
    mxc->PTDPF3 |= MXC_PTDPF3_D3(6); /* FB_AD0  */
    mxc->PTDPF4 |= MXC_PTDPF4_D1(6); /* FB_AD14 */
    mxc->PTDPF4 |= MXC_PTDPF4_D0(6); /* FB_AD13 */
    mxc->PTEPF1 |= MXC_PTEPF1_E7(6); /* FB_AD14 */
    mxc->PTEPF1 |= MXC_PTEPF1_E6(6); /* FB_AD3  */
    mxc->PTEPF2 |= MXC_PTEPF2_E5(6); /* FB_AD2  */
    mxc->PTEPF2 |= MXC_PTEPF2_E4(6); /* FB_AD1  */
    mxc->PTEPF3 |= MXC_PTEPF3_E3(6); /* FB_OE_b */
    mxc->PTFPF1 |= MXC_PTFPF1_F7(6); /* FB_AD10 */
    mxc->PTFPF1 |= MXC_PTFPF1_F6(6); /* FB_AD9  */
    mxc->PTFPF2 |= MXC_PTFPF2_F5(6); /* FB_RW_b */
    mxc->PTFPF2 |= MXC_PTFPF2_F4(6); /* FB_AD19 */
    mxc->PTFPF3 |= MXC_PTFPF3_F3(6); /* FB_AD8  */
    mxc->PTFPF3 |= MXC_PTFPF3_F2(6); /* FB_AD7  */
    mxc->PTFPF4 |= MXC_PTFPF4_F1(6); /* FB_AD6  */
    mxc->PTFPF4 |= MXC_PTFPF4_F0(6); /* FB_AD5  */
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_flexbus_mram_setup
* Returned Value   :
* Comments         :
*    Setup FlexBus for MRAM operation
*
*END*-----------------------------------------------------------------------*/
static void _bsp_flexbus_mram_setup (const uint32_t base_address)
{
    FB_MemMapPtr fb_ptr = FB_BASE_PTR;

    /* Enable external MRAM mapped on CS0 on base address */
    FB_CSAR_REG(FB_BASE_PTR, 0) = base_address;
    /* CS0 control (8bit data, 1 wait state, multiplexed mode) */
    FB_CSCR_REG(FB_BASE_PTR, 0) = FB_CSCR_ASET(1)  |
                                  FB_CSCR_AA_MASK  |
                                  FB_CSCR_WS(1)    |
                                  FB_CSCR_PS(1)    |
                                  FB_CSCR_MUX_MASK;
    /* CS0 address mask and enable */
    FB_CSMR_REG(FB_BASE_PTR, 0) = FB_CSMR_BAM(0x07) | FB_CSMR_V_MASK;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_flexbus_pccard_setup
* Returned Value   :
* Comments         :
*    Setup flexbus for pccard operation
*
*END*-----------------------------------------------------------------------*/
void _bsp_flexbus_pccard_setup (const uint32_t base_address)
{
    FB_MemMapPtr fb_ptr = FB_BASE_PTR;

    /* invalidate CS configuration first */
    fb_ptr->CS[0].CSMR = 0;

    /* Enable external PCCARD mapped on CS0 */
    fb_ptr->CS[0].CSAR = base_address;
    /* CS0 control (8bit data, 19 waitstates, multiplexed mode) */
    fb_ptr->CS[0].CSCR = FB_CSCR_ASET(1)  |
                         FB_CSCR_AA_MASK  |
                         FB_CSCR_WS(19)   |
                         FB_CSCR_PS(1)    |
                         FB_CSCR_MUX_MASK;


    /* CS0 address mask (64 KB) and enable */
    fb_ptr->CS[0].CSMR = FB_CSMR_BAM(0) | FB_CSMR_V_MASK;
}


static void _bsp_bdm_init(void)
{
    /* enable clock for all porta & portb modules */
    SIM_SCGC6 = SIM_SCGC6_PORTA_MASK | SIM_SCGC6_PORTB_MASK;

    /* enable high drive strength to support high toggle rate */
    PCTLA_DS  |= PCTL_DS_PTDS(0x20);
    /* turn off digital filter for BKGD */
    PCTLB_PFE &= PCTL_PFE_PTPFE(0xef);
    /* make sure to disable pin filter */
    PCTLB_DFE &= PCTL_DFE_PTDFE(0xef);
    /* 1 Pulling resistor is enabled. */
    PCTLB_PUE |= PCTL_PUE_PTPUE(0x10);
    /* 1 pull-up resistor is selected. */
    PCTLB_PUS |= PCTL_PUS_PTPUS(0x10);
    /* to avoid bdm communication issue */
    PCTLB_DS  |= PCTL_DS_PTDS(0x10);
}


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : __init_hardware
* Returned Value   : void
* Comments         :
*   Initialize CF+ device. Function is called from __boot() function
*
*END*-----------------------------------------------------------------------*/

void __init_hardware(void)
{
    /* Init gpio registers for fast bdm communication */
    _bsp_bdm_init();
    /* Init clocks */
    _bsp_initialize_hardware();
    /* Enable pin clocks */
    _bsp_gpio_io_init_new ();
    /* Init FlexBus */
    _bsp_flexbus_setup();
    /* Init MRAM */
    _bsp_flexbus_mram_setup((uint32_t)BSP_EXTERNAL_MRAM_RAM_BASE);
}
