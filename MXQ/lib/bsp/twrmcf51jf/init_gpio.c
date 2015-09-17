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
* See license agreement file for full license terms including other
* restrictions.
*****************************************************************************
*
* Comments:
*
*   This file contains board-specific pin initialization functions.
*
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"
#include "bsp_prv.h"


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_serial_io_init
* Returned Value   : IO_OK for success, IO_ERROR for failure
* Comments         :
*    This function performs BSP-specific initialization related to serial
*
*END*-----------------------------------------------------------------------*/

_mqx_int _bsp_serial_io_init
    (
        /* [IN] Serial device number */
        uint8_t dev_num,
        /* [IN] Required functionality */
        uint8_t flags
    )
{
    /* Setup I/O for UARTs */
    switch (dev_num)
    {
        /* UART0 I/O configuration */
        case 0:
            if (flags & IO_PERIPHERAL_PIN_MUX_ENABLE)
            {
                /* Set pin multiplexer to UART0_TX pin */
                clrSetReg8Bits(MXC_PTAPF1, MXC_PTAPF1_A7_MASK, MXC_PTAPF1_A7(2));
                /* Set pin multiplexer to UART0_RX pin */
                clrSetReg8Bits(MXC_PTDPF1, MXC_PTDPF1_D6_MASK, MXC_PTDPF1_D6(2));
            }
            if (flags & IO_PERIPHERAL_PIN_MUX_DISABLE)
            {
                /* Set pin multiplexer to default value */
                clrReg8Bits(MXC_PTAPF1, MXC_PTAPF1_A7_MASK);
                /* Set pin multiplexer to default value */
                clrReg8Bits(MXC_PTAPF1, MXC_PTDPF1_D6_MASK);
            }

            if (flags & IO_PERIPHERAL_CLOCK_ENABLE)
            {
                /* Enable clock to UART0 module*/
                setReg8Bits(SIM_SCGC1, SIM_SCGC1_UART0_MASK);
            }
            if (flags & IO_PERIPHERAL_CLOCK_DISABLE)
            {
                /* Disable clock to UART0 module*/
                clrReg8Bits(SIM_SCGC1, SIM_SCGC1_UART0_MASK);
            }
            break;
        /* I2C0 I/O configuration */
        case 1:
            if (flags & IO_PERIPHERAL_PIN_MUX_ENABLE)
            {
                /* Set pin multiplexer to UART1_TX pin */
                clrSetReg8Bits(MXC_PTFPF1, MXC_PTFPF1_F6_MASK, MXC_PTFPF1_F6(2));
                /* Set pin multiplexer to UART1_RX pin */
                clrSetReg8Bits(MXC_PTFPF2, MXC_PTFPF2_F5_MASK, MXC_PTFPF2_F5(2));
            }
            if (flags & IO_PERIPHERAL_PIN_MUX_DISABLE)
            {
                /* Set pin multiplexer to default value */
                clrReg8Bits(MXC_PTFPF1, MXC_PTFPF1_F6_MASK);
                /* Set pin multiplexer to default value */
                clrReg8Bits(MXC_PTFPF2, MXC_PTFPF2_F5_MASK);
            }
            if (flags & IO_PERIPHERAL_CLOCK_ENABLE)
            {
                /* Enable clock to UART1 module*/
                setReg8Bits(SIM_SCGC1, SIM_SCGC1_UART1_MASK);
            }
            if (flags & IO_PERIPHERAL_CLOCK_DISABLE)
            {
                /* Disable clock to UART1 module*/
                setReg8Bits(SIM_SCGC1, SIM_SCGC1_UART1_MASK);
            }
            break;
        default:
            return IO_ERROR;
    }

    return IO_OK;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_serial_rts_init
* Returned Value   : IO_OK for success, IO_ERROR for failure
* Comments         :
*    This function performs BSP-specific initialization related to GPIO
*
*END*----------------------------------------------------------------------*/
_mqx_int _bsp_serial_rts_init
    (
        uint8_t channel
    )
{
    /* set pin to RTS functionality */
    switch( channel )
    {
        case 0:
            /* Set pin multiplexer to UART0_RTS pin */
            clrSetReg8Bits(MXC_PTEPF4, MXC_PTEPF4_E0_MASK, MXC_PTEPF4_E0(2));
            break;
        default:
            /* not used on this board */
            return IO_ERROR;
            break;
    }
    return IO_OK;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_spi_io_init
* Returned Value   : IO_OK for success, IO_ERROR for failure
* Comments         :
*    This function performs BSP-specific initialization related to SPI
*
*END*----------------------------------------------------------------------*/

_mqx_int _bsp_spi_io_init
    (
        uint8_t dev_num
    )
{

    switch (dev_num)
    {
        /* SPI0 I/O configuration */
        case 0:
            /* Set pin multiplexer to SPI0_SS pin */
            clrSetReg8Bits(MXC_PTFPF4, MXC_PTFPF4_F0_MASK, MXC_PTFPF4_F0(2));
            /* Set pin multiplexer to SPI0_SCLK pin */
            clrSetReg8Bits(MXC_PTFPF4, MXC_PTFPF4_F1_MASK, MXC_PTFPF4_F1(2));
            /* Set pin multiplexer to SPI0_MISO pin */
            clrSetReg8Bits(MXC_PTFPF3, MXC_PTFPF3_F2_MASK, MXC_PTFPF3_F2(2));
            /* Set pin multiplexer to SPI0_MOSI pin */
            clrSetReg8Bits(MXC_PTFPF3, MXC_PTFPF3_F3_MASK, MXC_PTFPF3_F3(2));
            /* Enable clock to SPI0 module*/
            setReg8Bits(SIM_SCGC1, SIM_SCGC1_SPI0_MASK);
        break;
        /* SPI1 I/O configuration */
        case 1:
            /* Set pin multiplexer to SPI1_SS pin */
            clrSetReg8Bits(MXC_PTAPF3, MXC_PTAPF3_A2_MASK, MXC_PTAPF3_A2(5));
            /* Set pin multiplexer to SPI1_SCLK pin */
            clrSetReg8Bits(MXC_PTAPF3, MXC_PTAPF3_A3_MASK, MXC_PTAPF3_A3(5));
            /* Set pin multiplexer to SPI1_MISO pin */
            clrSetReg8Bits(MXC_PTAPF2, MXC_PTAPF2_A4_MASK, MXC_PTAPF2_A4(5));
            /* Set pin multiplexer to SPI1_MOSI pin */
            clrSetReg8Bits(MXC_PTAPF2, MXC_PTAPF2_A5_MASK, MXC_PTAPF2_A5(5));
            /* Enable clock to SPI1 module*/
            setReg8Bits(SIM_SCGC1, SIM_SCGC1_SPI1_MASK);
        break;
        /* Unsupported channel*/
        default:
            return IO_ERROR;
    }

    return IO_OK;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_i2c_io_init
* Returned Value   : IO_OK for success, IO_ERROR for failure
* Comments         :
*    This function performs BSP-specific initialization related to I2C
*
*END*----------------------------------------------------------------------*/

_mqx_int _bsp_i2c_io_init
    (
        uint8_t dev_num
    )
{

    /* Setup I/O for I2C */
    switch(dev_num)
    {
        /* I2C0 I/O configuration */
        case 0:
            /* Set pin multiplexer to I2C0_SDA pin */
            clrSetReg8Bits(MXC_PTCPF1, MXC_PTCPF1_C7_MASK, MXC_PTCPF1_C7(3));
            /* Set pin multiplexer to I2C0_SCL pin */
            clrSetReg8Bits(MXC_PTCPF1, MXC_PTCPF1_C6_MASK, MXC_PTCPF1_C6(3));
            /* Enable SDA rising edge detection */
#if BSPCFG_ENABLE_LEGACY_II2C_SLAVE
            PCTLC_IES |= (1 << 7);            /* Enable rising edge/high level interrupt */
            PCTLC_IF  |= (1 << 7);            /* Clear interrupt flag */
            PCTLC_IPE |= (1 << 7);            /* Enable pin for pin interrupt */
            PCTLC_IC  |= PCTL_IC_PTIE_MASK;   /* Enable pin interrupt request */
            PCTLC_IC  &= ~PCTL_IC_PTMOD_MASK; /* Set pin interrupt detects edges only */
#endif
            /* Enable clock to I2C0 module*/
            setReg8Bits(SIM_SCGC1, SIM_SCGC1_I2C0_MASK);
        break;
        /* I2C1 I/O configuration */
        case 1:
            /* Set pin multiplexer to I2C1_SDA pin */
            clrSetReg8Bits(MXC_PTDPF4, MXC_PTDPF4_D0_MASK, MXC_PTDPF4_D0(3));
            /* Set pin multiplexer to I2C1_SCL pin */
            clrSetReg8Bits(MXC_PTDPF4, MXC_PTDPF4_D1_MASK, MXC_PTDPF4_D1(3));
            /* Enable SDA rising edge detection */
#if BSPCFG_ENABLE_LEGACY_II2C_SLAVE
            PCTLD_IES |= (1 << 0);            /* Enable rising edge/high level interrupt */
            PCTLD_IF  |= (1 << 0);            /* Clear interrupt flag */
            PCTLD_IPE |= (1 << 0);            /* Enable pin for pin interrupt */
            PCTLD_IC  |= PCTL_IC_PTIE_MASK;   /* Enable pin interrupt request */
            PCTLD_IC  &= ~PCTL_IC_PTMOD_MASK; /* Set pin interrupt detects edges only */
#endif
            /* Enable clock to I2C1 module*/
            setReg8Bits(SIM_SCGC1, SIM_SCGC1_I2C1_MASK);
        break;
        /* I2C2 I/O configuration */
        case 2:
            /* Set pin multiplexer to I2C2_SDA pin */
            clrSetReg8Bits(MXC_PTAPF4, MXC_PTAPF4_A1_MASK, MXC_PTAPF4_A1(3));
            /* Set pin multiplexer to I2C2_SCL pin */
            clrSetReg8Bits(MXC_PTAPF4, MXC_PTAPF4_A0_MASK, MXC_PTAPF4_A0(3));
            /* Enable SDA rising edge detection */
#if BSPCFG_ENABLE_LEGACY_II2C_SLAVE
            PCTLA_IES |= (1 << 1);            /* Enable rising edge/high level interrupt */
            PCTLA_IF  |= (1 << 1);            /* Clear interrupt flag */
            PCTLA_IPE |= (1 << 1);            /* Enable pin for pin interrupt */
            PCTLA_IC  |= PCTL_IC_PTIE_MASK;   /* Enable pin interrupt request */
            PCTLA_IC  &= ~PCTL_IC_PTMOD_MASK; /* Set pin interrupt detects edges only */
#endif
            /* Enable clock to I2C2 module*/
            setReg8Bits(SIM_SCGC1, SIM_SCGC1_I2C2_MASK);
        break;
        /* I2C3 I/O configuration */
        case 3:
            /* Set pin multiplexer to I2C3_SDA pin */
            clrSetReg8Bits(MXC_PTAPF4, MXC_PTEPF4_E0_MASK, MXC_PTEPF4_E0(3));
            /* Set pin multiplexer to I2C3_SCL pin */
            clrSetReg8Bits(MXC_PTEPF3, MXC_PTEPF3_E2_MASK, MXC_PTEPF3_E2(3));
            /* Enable SDA rising edge detection */
#if BSPCFG_ENABLE_LEGACY_II2C_SLAVE
            PCTLE_IES |= (1 << 0);            /* Enable rising edge/high level interrupt */
            PCTLE_IF  |= (1 << 0);            /* Clear interrupt flag */
            PCTLE_IPE |= (1 << 0);            /* Enable pin for pin interrupt */
            PCTLE_IC  |= PCTL_IC_PTIE_MASK;   /* Enable pin interrupt request */
            PCTLE_IC  &= ~PCTL_IC_PTMOD_MASK; /* Set pin interrupt detects edges only */
#endif
            /* Enable clock to I2C3 module*/
            setReg8Bits(SIM_SCGC1, SIM_SCGC1_I2C3_MASK);
        break;

        default:
            /* Do nothing if bad dev_num was selected */
            return IO_ERROR;
    }

    return IO_OK;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_usb_io_init
* Returned Value   : IO_OK for success, IO_ERROR for failure
* Comments         :
*    This function performs BSP-specific I/O initialization related to USB
*
*END*----------------------------------------------------------------------*/

// TODO check if setting for TWR-SER2 is valid

_mqx_int _bsp_usb_io_init
    (
        void
    )
{
    /* Enable USB-OTG IP clocking */
    setReg8Bits(SIM_SCGC6, SIM_SCGC6_USBOTG_MASK);

    /* PTB7 enables 5v on USB connector */
    MXC_PTBPF1 |= 0x10;
    PTB_DD |= 0x80;
    PTB_D |= 0x80;

    /* USB D+ and USB D- are standalone not multiplexed one-purpose pins */
    /* VREFIN for device is standalone not multiplexed one-purpose pin */

#if BSP_USB_TWR_SER2
    /* TWR-SER2 board has 2 connectors: on channel A, there is Micro-USB connector,
    ** which is not routed to TWRMCF51JF board. On channel B, there is standard
    ** A-type host connector routed to the USB0 peripheral on TWRMCF51JF. To enable
    ** power to this connector, GPIO PF7 must be set as GPIO output.
    */
    PORT_PCR_REG(PORTF_BASE_PTR, 7) = PORT_PCR_MUX(0x01);
    GPIO_PDDR_REG(PTF_BASE_PTR) |= 1 << 7; /*  PF7 as output */
    GPIO_PDOR_REG(PTF_BASE_PTR) |= 1 << 7; /*  PF7 in high level */
#endif

    return IO_OK;
}


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_gpio_io_init
* Returned Value   : IO_OK for success, IO_ERROR for failure
* Comments         :
*    This function initialize for pin mux control as general purpose I/O
*
*END*----------------------------------------------------------------------*/

_mqx_int _bsp_gpio_io_init
    (
        uint8_t addr,
        uint8_t pin
    )
{
    uint8_t           *tmp;
    uint8_t               port;
    tmp = (uint8_t *)(MXC_BASE_PTR);

    if((addr >= GPIO_PORT_MAX) || (pin > 7))
        return IO_ERROR;

    port = addr*4 + (3-pin/2);

    if(pin & 0x01){
        *(tmp+port) &= ~0xf0;
        *(tmp+port) |= 0x10;
    }
    else{
        *(tmp+port) &= ~0x0f;
        *(tmp+port) |= 0x01;
    }
    return IO_OK;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_gpio_io_init_new
* Returned Value   : IO_OK
* Comments         :
*    This function performs BSP-specific initialization related to GPIO
*
*END*----------------------------------------------------------------------*/

_mqx_int _bsp_gpio_io_init_new
    (
        void
    )
{
    /* Enable clock to IRQ module to enable GPIO interrupts */
    setReg8Bits(SIM_SCGC4, SIM_SCGC4_IRQ_MASK);

    /* Enable clock gating to all ports */
    setReg8Bits(SIM_SCGC6,   SIM_SCGC6_PORTA_MASK \
                           | SIM_SCGC6_PORTB_MASK \
                           | SIM_SCGC6_PORTC_MASK \
                           | SIM_SCGC6_PORTD_MASK \
                           | SIM_SCGC6_PORTE_MASK \
                           | SIM_SCGC6_PORTF_MASK \
    );

    return IO_OK;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_adc_io_init
* Returned Value   : 0 for success, -1 for failure
* Comments         :
*    This function performs BSP-specific initialization related to ADC
*
*END*----------------------------------------------------------------------*/

_mqx_int _bsp_adc_io_init
    (
         /* [IN] number of channel on which to perform hardware initialization */
        _mqx_uint adc_num
    )
{
    /* Enable ADC clock */
    if (adc_num == 0)
        setReg8Bits(SIM_SCGC2, SIM_SCGC2_ADC_MASK);
    else
        return IO_ERROR;

    return IO_OK;
}
/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_adc_channel_io_init
* Returned Value   : 0 for success, -1 for failure
* Comments         :
*    This function performs BSP-specific initialization related to ADC channel
*
*END*----------------------------------------------------------------------*/

_mqx_int _bsp_adc_channel_io_init
    (
         /* [IN] number of channel on which to perform hardware initialization */
        uint16_t   source
    )
{
    MXC_MemMapPtr   MXC = MXC_BASE_PTR;
    uint8_t          ch  = ADC_GET_CHANNEL(source);
    uint8_t          mux = ADC_GET_MUXSEL(source);
    uint8_t          gpio_port;


    #define ADC_SIG_PORTA   (0x01 << 5)
    #define ADC_SIG_PORTB   (0x02 << 5)
    #define ADC_SIG_PORTC   (0x03 << 5)
    #define ADC_SIG_PORTD   (0x04 << 5)
    #define ADC_SIG_PORTE   (0x05 << 5)
    #define ADC_SIG_NA      (0x00) /* signal not available */
    #define ADC_SIG_NC      (0x01) /* signal not configurable */

    /* Conversion table for ADC0x inputs, where x is 0 to 23, mux is defaultly "B" */
    const static uint8_t adc0_conv_table[] = {
        ADC_SIG_NC,        /* 0 leave as default    */
        ADC_SIG_NC,        /* 1 leave as default    */
        ADC_SIG_PORTA | 4, /* 2     */
        ADC_SIG_PORTA | 5, /* 3     */
        ADC_SIG_NA,        /* 4 not implemented, leave as default   */
        ADC_SIG_NA,        /* 5 not implemented, leave as default   */
        ADC_SIG_NA,        /* 6 not implemented, leave as default   */
        ADC_SIG_NA,        /* 7 not implemented, leave as default   */
        ADC_SIG_PORTA | 6, /* 8     */
        ADC_SIG_PORTD | 2, /* 9     */
        ADC_SIG_PORTD | 3, /* 10    */
        ADC_SIG_PORTD | 4, /* 11    */
        ADC_SIG_PORTD | 5, /* 12    */
        ADC_SIG_PORTA | 7, /* 13    */
        ADC_SIG_PORTD | 6, /* 14    */
        ADC_SIG_PORTD | 7, /* 15    */
        ADC_SIG_PORTE | 3, /* 16 leave as default */
        ADC_SIG_PORTB | 2, /* 17    */
        ADC_SIG_PORTB | 3, /* 18    */
        ADC_SIG_PORTE | 4, /* 19    */
        ADC_SIG_PORTE | 5, /* 20    */
        ADC_SIG_PORTE | 6, /* 21    */
        ADC_SIG_PORTE | 7, /* 22    */
        ADC_SIG_NA,        /* 23 not implemented, leave as default  */
    };

    if (ch < 24) {
        if (ADC_GET_MODULE(source) == ADC_SOURCE_MODULE(0)) {
            /* Get result for module 0 */
           gpio_port = adc0_conv_table[ch];
        }
    }
    else
        gpio_port = ADC_SIG_NC;

    if (gpio_port == ADC_SIG_NA)
        return IO_ERROR; /* signal not available */
    if (gpio_port == ADC_SIG_NC)
        return IO_OK; /* no need to configure signal */
    switch (gpio_port >> 5) {
        case 1: /* PORTA */
            MXC->PTAPF1 &=~MXC_PTAPF1_A7_MASK;
            MXC->PTAPF1 &=~MXC_PTAPF1_A6_MASK;  /* set pin's multiplexer to analog */
            MXC->PTAPF2 &=~MXC_PTAPF2_A5_MASK;
            MXC->PTAPF2 &=~MXC_PTAPF2_A4_MASK;
            break;
        case 2: /* PORTB */
            MXC->PTBPF3 &=~MXC_PTBPF3_B3_MASK;
            MXC->PTBPF3 &=~MXC_PTBPF3_B2_MASK;  /* set pin's multiplexer to analog */
            break;
        case 3: /* PORTC */
            break;
        case 4: /* PORTD */
            MXC->PTDPF2 &=~MXC_PTDPF2_D5_MASK;  /* set pin's multiplexer to analog */
            break;
        case 5: /* PORTE */
            MXC->PTEPF1 &=~MXC_PTEPF1_E7_MASK;
            MXC->PTEPF1 &=~MXC_PTEPF1_E6_MASK;
            MXC->PTEPF2 &=~MXC_PTEPF2_E5_MASK;
            MXC->PTEPF2 &=~MXC_PTEPF2_E4_MASK;
            MXC->PTEPF3 &=~MXC_PTEPF3_E3_MASK;
            break;
        /* There is no possibility to get other port from table */
    }

    return IO_OK;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_ftfx_io_init
* Returned Value   : IO_OK for success, IO_ERROR for failure
* Comments         :
*    This function performs BSP-specific initialization related to FLFL
*
*END*----------------------------------------------------------------------*/
_mqx_int _bsp_ftfx_io_init
(
    _mqx_uint device_index
)
{
    if (device_index > 0)
        return IO_ERROR;

    /* Enable FTFL clock */
    setReg8Bits(SIM_SCGC4, SIM_SCGC4_FTFL_MASK);

    return IO_OK;
}

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _bsp_tss_io_init
* Returned Value   : MQX_OK
* Comments         :
*    This function performs BSP-specific initialization related to TSS
*
*END*----------------------------------------------------------------------*/

_mqx_int _bsp_tss_io_init
(
        void
)
{
    SIM_MemMapPtr sim = SIM_BASE_PTR;  
    
    /* Modules Clock enablement */
    sim->SCGC2 |= SIM_SCGC2_TSI_MASK;  /* TSI clock enablement */      
    
    /* Set Electrodes for TSI function */
    MXC_PTDPF3 &= 0xF0; // Select as TSICH1
    MXC_PTEPF4 &= 0x0F; // Select as TSICH9   /* EL0 */
    MXC_PTBPF4 &= 0x0F; // Select as TSICH10  /* EL1 */
    MXC_PTEPF3 &= 0xF0; // Select as TSICH11
    MXC_PTEPF3 &= 0x0F; // Select as TSICH12
    MXC_PTBPF3 &= 0xF0; // Select as TSICH13  

    return MQX_OK;
}

/* Endbody */
