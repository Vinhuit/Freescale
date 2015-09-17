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

#include <mqx.h>
#include <bsp.h>

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_serial_io_init
* Returned Value   : MQX_OK for success, -1 for failure
* Comments         :
*    This function performs BSP-specific initialization related to serial
*
*END*----------------------------------------------------------------------*/

_mqx_int _bsp_serial_io_init
(
    /* [IN] Serial device number */
    uint8_t dev_num,

    /* [IN] Required functionality */
    uint8_t flags
)
{
    SIM_MemMapPtr   sim = SIM_BASE_PTR;
    PORT_MemMapPtr  pctl = NULL;

    /* Setup GPIO for UART devices */
    switch (dev_num)
    {
        case 0:
            pctl = (PORT_MemMapPtr)PORTA_BASE_PTR;
            if (flags & IO_PERIPHERAL_PIN_MUX_ENABLE)
            {
                /* PTA1 as RX function (Alt.2) + drive strength */
                pctl->PCR[1] = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;
                /* PTA2 as TX function (Alt.2) + drive strength */
                pctl->PCR[2] = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;
            }
            if (flags & IO_PERIPHERAL_PIN_MUX_DISABLE)
            {
                /* PTA1 default */
                pctl->PCR[1] = 0;
                /* PTA2 default */
                pctl->PCR[2] = 0;
            }
            if (flags & IO_PERIPHERAL_CLOCK_ENABLE)
            {
                /* start SGI clock */
                sim->SCGC4 |= SIM_SCGC4_UART0_MASK;
            }
            if (flags & IO_PERIPHERAL_CLOCK_DISABLE)
            {
                /* stop SGI clock */
                sim->SCGC4 &= (~ SIM_SCGC4_UART0_MASK);
            }
            break;
        case 1:
            pctl = (PORT_MemMapPtr)PORTC_BASE_PTR;
            if (flags & IO_PERIPHERAL_PIN_MUX_ENABLE)
            {
                /* PTC3 as RX function (Alt.3) + drive strength */
                pctl->PCR[3] = 0 | PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK;
                /* PTC4 as TX function (Alt.3) + drive strength */
                pctl->PCR[4] = 0 | PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK;
            }
            if (flags & IO_PERIPHERAL_PIN_MUX_DISABLE)
            {
                /* PTC3 default */
                pctl->PCR[3] = 0;
                /* PTC4 default */
                pctl->PCR[4] = 0;
            }
            if (flags & IO_PERIPHERAL_CLOCK_ENABLE)
            {
                /* start SGI clock */
                sim->SCGC4 |= SIM_SCGC4_UART1_MASK;
            }
            if (flags & IO_PERIPHERAL_CLOCK_DISABLE)
            {
                /* start SGI clock */
                sim->SCGC4 &= (~ SIM_SCGC4_UART1_MASK);
            }
            break;

        case 2:
            pctl = (PORT_MemMapPtr)PORTE_BASE_PTR;
            if (flags & IO_PERIPHERAL_PIN_MUX_ENABLE)
            {
                /* PTE17 as RX function (Alt.3) + drive strength */
                pctl->PCR[17] = 0 | PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK;
                /* PTE16 as TX function (Alt.3) + drive strength */
                pctl->PCR[16] = 0 | PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK;
            }
            if (flags & IO_PERIPHERAL_PIN_MUX_DISABLE)
            {
                /* PTE17 default */
                pctl->PCR[17] = 0;
                /* PTE16 default */
                pctl->PCR[16] = 0;
            }
            if (flags & IO_PERIPHERAL_CLOCK_ENABLE)
            {
                /* start SGI clock */
                sim->SCGC4 |= SIM_SCGC4_UART2_MASK;
            }
            if (flags & IO_PERIPHERAL_CLOCK_DISABLE)
            {
                /* stop SGI clock */
                sim->SCGC4 &= (~ SIM_SCGC4_UART2_MASK);
            }
            break;

        case 3:
            pctl = (PORT_MemMapPtr)PORTC_BASE_PTR;
            if (flags & IO_PERIPHERAL_PIN_MUX_ENABLE)
            {
                /* PTC16 as RX function (Alt.3) + drive strength */
                pctl->PCR[16] = 0 | PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK;
                /* PTC17 as TX function (Alt.3) + drive strength */
                pctl->PCR[17] = 0 | PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK;
            }
            if (flags & IO_PERIPHERAL_PIN_MUX_DISABLE)
            {
                /* PTC16 default */
                pctl->PCR[16] = 0;
                /* PTC17 default */
                pctl->PCR[17] = 0;
            }
            if (flags & IO_PERIPHERAL_CLOCK_ENABLE)
            {
                /* start SGI clock */
                sim->SCGC4 |= SIM_SCGC4_UART3_MASK;
            }
            if (flags & IO_PERIPHERAL_CLOCK_DISABLE)
            {
                /* stop SGI clock */
                sim->SCGC4 &= (~ SIM_SCGC4_UART3_MASK);
            }
            break;

        case 4:
            pctl = (PORT_MemMapPtr) PORTC_BASE_PTR;
            if (flags & IO_PERIPHERAL_PIN_MUX_ENABLE) {

            /* PTC14 as RX function (Alt.3) + drive strength */
            pctl->PCR[14] = 0 | PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK;
            /* PTC15 as TX function (Alt.3) + drive strength */
            pctl->PCR[15] = 0 | PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK;

            }
            if (flags & IO_PERIPHERAL_PIN_MUX_DISABLE)
            {
                /* PTC14 default */
                pctl->PCR[14] = 0;
                /* PTC15 default */
                pctl->PCR[15] = 0;
            }
            if (flags & IO_PERIPHERAL_CLOCK_ENABLE)
            {
                /* starting SGI clock */
                sim->SCGC1 |= SIM_SCGC1_UART4_MASK;
            }
            if (flags & IO_PERIPHERAL_CLOCK_DISABLE)
            {
                /* starting SGI clock */
                sim->SCGC1 &= (~ SIM_SCGC1_UART4_MASK);
            }
            break;

         default:
            return -1;
  }

  return 0;
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
     /* [IN] number of ADC device on which to perform hardware initialization */
    _mqx_uint adc_num
)
{
    SIM_MemMapPtr sim_ptr = SIM_BASE_PTR;

    /* Enable ADC clocks */
    switch (adc_num)
    {
        case 0:
            sim_ptr->SCGC6 |= SIM_SCGC6_ADC0_MASK;
            break;
        case 1:
            sim_ptr->SCGC3 |= SIM_SCGC3_ADC1_MASK;
            break;
        default:
            return IO_ERROR;
    }

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
    uint8_t ch = ADC_GET_CHANNEL(source);
    /*
    uint8_t mux = ADC_GET_MUXSEL(source);
    */
    uint8_t gpio_port;
    PORT_MemMapPtr pctl;

    #define ADC_SIG_PORT_SHIFT (5)
    #define ADC_SIG_PORTA   (0x01 << ADC_SIG_PORT_SHIFT)
    #define ADC_SIG_PORTB   (0x02 << ADC_SIG_PORT_SHIFT)
    #define ADC_SIG_PORTC   (0x03 << ADC_SIG_PORT_SHIFT)
    #define ADC_SIG_PORTD   (0x04 << ADC_SIG_PORT_SHIFT)
    #define ADC_SIG_PORTE   (0x05 << ADC_SIG_PORT_SHIFT)
    #define ADC_SIG_PORTF   (0x06 << ADC_SIG_PORT_SHIFT)
    #define ADC_SIG_NA      (0x00) /* signal not available */
    #define ADC_SIG_NC      (0x01) /* signal not configurable */



/* Conversion table for ADC0x inputs, where x is 0 to 23, mux is defaultly "B" OR "X" */
    const static uint8_t adc0_conv_table_bx[] = {
        ADC_SIG_NC,         /* 0 leave as default */
        ADC_SIG_NA,         /* 1 not implemented */
        ADC_SIG_NA,         /* 2 not implemented */
        ADC_SIG_NC,         /* 3 leave as default */
        ADC_SIG_PORTC | 2,  /* ADC0_SE4b */
        ADC_SIG_PORTD | 1,  /* ADC0_SE5b */
        ADC_SIG_PORTD | 5,  /* ADC0_SE6b */
        ADC_SIG_PORTD | 6,  /* ADC0_SE7b */
        ADC_SIG_PORTB | 0,  /* ADC0_SE8 */
        ADC_SIG_PORTB | 1,  /* ADC0_SE9 */
        ADC_SIG_PORTA | 7,  /* ADC0_SE10 */
        ADC_SIG_PORTA | 8,  /* ADC0_SE11 */
        ADC_SIG_PORTB | 2,  /* ADC0_SE12 */
        ADC_SIG_PORTB | 3,  /* ADC0_SE13 */
        ADC_SIG_PORTC | 0,  /* ADC0_SE14 */
        ADC_SIG_PORTC | 1,  /* ADC0_SE15 */
        ADC_SIG_NC,         /* ADC0_SE16 , leave as default */
        ADC_SIG_PORTE | 24, /* ADC0_SE17 */
        ADC_SIG_PORTE | 25, /* ADC0_SE18 */
        ADC_SIG_NC,         /* 19 leave as default , ADC0_DM0*/
        ADC_SIG_NA,         /* 20 Reserved and not implemented */
        ADC_SIG_NC,         /* ADC0_SE21 */
        ADC_SIG_NC,         /* ADC0_SE22 */
        ADC_SIG_NC,         /* ADC0_SE23 */
        ADC_SIG_NA,         /* 24 not implemented */
        ADC_SIG_NA,         /* 25 not implemented */
    };

        /* Conversion table for ADC0x inputs, where x is 4 to 7, mux is defaultly "A" */
    const static uint8_t adc0_conv_table_a[] = {
        ADC_SIG_PORTE | 16, /* ADC0_SE4a */
        ADC_SIG_PORTE | 17, /* ADC0_SE5a */
        ADC_SIG_PORTE | 18, /* ADC0_SE6a */
        ADC_SIG_PORTE | 19  /* ADC0_SE7a */
    };

    /* Conversion table for ADC1x inputs, where x is 0 to 23, mux is defaultly "B" OR "X"*/
    const static uint8_t adc1_conv_table_bx[] = {
        ADC_SIG_NC,         /* 0 leave as default */
        ADC_SIG_NC,         /* 1 leave as default */
        ADC_SIG_NA,         /* 2 not implemented */
        ADC_SIG_NC,         /* 3 leave as default */
        ADC_SIG_PORTC | 8,  /* ADC1_SE4b */
        ADC_SIG_PORTC | 9,  /* ADC1_SE5b */
        ADC_SIG_PORTC | 10, /* ADC1_SE6b */
        ADC_SIG_PORTC | 11, /* ADC1_SE7b */
        ADC_SIG_PORTB | 0,  /* ADC0_SE8 */
        ADC_SIG_PORTB | 1,  /* ADC0_SE9 */
        ADC_SIG_PORTB | 4,  /* ADC1_SE10 */
        ADC_SIG_PORTB | 5,  /* ADC1_SE11 */
        ADC_SIG_PORTB | 6,  /* ADC1_SE12 */
        ADC_SIG_PORTB | 7,  /* ADC1_SE13 */
        ADC_SIG_PORTB | 10, /* ADC1_SE14 */
        ADC_SIG_PORTB | 11, /* ADC1_SE15 */
        ADC_SIG_NC,         /* ADC1_SE16, leave as default */
        ADC_SIG_PORTA | 17, /* ADC1_SE17 */
        ADC_SIG_NC,         /* VREF_OUT/CMP1_IN5/CMP0_IN5/ADC1_SE18, leave as default*/
        ADC_SIG_NC,         /* 19 leave as default */
        ADC_SIG_NA,         /* 20 not implemented */
        ADC_SIG_NA,         /* 21 not implemented */
        ADC_SIG_NA,         /* 22 not implemented */
        ADC_SIG_NC,         /* DAC1_OUT/CMP0_IN4/CMP2_IN3/ADC1_SE23 */
        ADC_SIG_NA,         /* 24 not implemented */
        ADC_SIG_NA,         /* 25 not implemented */
    };
    /* Conversion table for ADC1x inputs, where x is 4 to 7, mux is defaultly "A" */
    const static uint8_t adc1_conv_table_a[] = {
        ADC_SIG_PORTE | 0, /* ADC1_SE4a */
        ADC_SIG_PORTE | 1, /* ADC1_SE5a */
        ADC_SIG_PORTE | 2, /* ADC1_SE6a */
        ADC_SIG_PORTE | 3  /* ADC1_SE7a */
    };

    if (ADC_GET_DIFF(source) && (ch > 1))
    {
        return IO_ERROR; /* signal not available */
    }

    switch(ADC_GET_MODULE(source))
    {
        case ADC_SOURCE_MODULE(1): /* ADC0 */
            if (ADC_GET_MUXSEL(source) == ADC_SOURCE_MUXSEL_A)
            {
                if(ch < 4 || ch > 7)
                {
                    gpio_port = ADC_SIG_NA; /* channel does not exist */
                }
                else
                {
                    gpio_port = adc0_conv_table_a[ch - 4];
                }
            }
            else /* ADC_SOURCE_MUXSEL_B OR ADC_SOURCE_MUXSEL_X */
            {
                if(ch < 26)
                {
                    gpio_port = adc0_conv_table_bx[ch];
                }
                else
                {
                    gpio_port = ADC_SIG_NC;
                }
            }
            break;
        case ADC_SOURCE_MODULE(2): /* ADC1 */
            if (ADC_GET_MUXSEL(source) == ADC_SOURCE_MUXSEL_A)
            {
                if(ch < 4 || ch > 7)
                {
                    gpio_port = ADC_SIG_NA; /* channel does not exist */
                }
                else
                {
                    gpio_port = adc1_conv_table_a[ch - 4];
                }
            }
            else /* ADC_SOURCE_MUXSEL_B OR ADC_SOURCE_MUXSEL_X */
            {
                if(ch < 26)
                {
                    gpio_port = adc1_conv_table_bx[ch];
                }
                else
                {
                    gpio_port = ADC_SIG_NC;
                }
            }
            break;
        default :
            return IO_ERROR; /*ADC0 - ADC1 only*/
    }

    if (gpio_port == ADC_SIG_NA)
    {
        return IO_ERROR; /* signal not available */
    }

    if (gpio_port == ADC_SIG_NC)
    {
        return IO_OK; /* no need to configure signal */
    }

    switch (gpio_port >> ADC_SIG_PORT_SHIFT)
    {
        case 1: /* PORTA */
            pctl = (PORT_MemMapPtr) PORTA_BASE_PTR;
            break;
        case 2: /* PORTB */
            pctl = (PORT_MemMapPtr) PORTB_BASE_PTR;
            break;
        case 3: /* PORTC */
            pctl = (PORT_MemMapPtr) PORTC_BASE_PTR;
            break;
        case 4: /* PORTD */
            pctl = (PORT_MemMapPtr) PORTD_BASE_PTR;
            break;
        case 5: /* PORTE */
            pctl = (PORT_MemMapPtr) PORTE_BASE_PTR;
            break;
        /* There is no possibility to get other port from table */
    }
    pctl->PCR[gpio_port & 0x1F] &= ~PORT_PCR_MUX_MASK; /* set pin's multiplexer to analog */

    return IO_OK;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_rtc_io_init
* Returned Value   : MQX_OK or -1
* Comments         :
*    This function performs BSP-specific initialization related to RTC
*
*END*----------------------------------------------------------------------*/

_mqx_int _bsp_rtc_io_init
(
    void
)
{

#if PE_LDD_VERSION
    /* Check if peripheral is not used by Processor Expert RTC_LDD component */
    if (PE_PeripheralUsed((uint32_t)RTC_BASE_PTR) == TRUE)    {
        /* IO Device used by PE Component*/
        return IO_ERROR;
    }
#endif

    /* Enable the clock gate to the RTC module. */
    SIM_SCGC6 |= SIM_SCGC6_RTC_MASK;

    return MQX_OK;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_dspi_io_init
* Returned Value   : MQX_OK or -1
* Comments         :
*    This function performs BSP-specific initialization related to DSPI
*
*END*----------------------------------------------------------------------*/

_mqx_int _bsp_dspi_io_init
(
    uint32_t dev_num
)
{
    SIM_MemMapPtr   sim = SIM_BASE_PTR;
    PORT_MemMapPtr  pctl;

    switch (dev_num)
    {
        case 0:
            /* Configure GPIOD for DSPI0 peripheral function */
            pctl = (PORT_MemMapPtr)PORTD_BASE_PTR;

            pctl->PCR[0] = PORT_PCR_MUX(2);     /* DSPI0.PCS0   */
            pctl->PCR[1] = PORT_PCR_MUX(2);     /* DSPI0.SCK    */
            pctl->PCR[2] = PORT_PCR_MUX(2);     /* DSPI0.SOUT   */
            pctl->PCR[3] = PORT_PCR_MUX(2);     /* DSPI0.SIN    */

            /* Enable clock gate to SPI0 module */
            sim->SCGC6 |= SIM_SCGC6_SPI0_MASK;
            break;

        case 1:
            /* Configure GPIOE for DSPI1 peripheral function     */
            pctl = (PORT_MemMapPtr)PORTE_BASE_PTR;

            pctl->PCR[1] = PORT_PCR_MUX(2);     /* DSPI1.SOUT   */
            pctl->PCR[2] = PORT_PCR_MUX(2);     /* DSPI1.SCK    */
            pctl->PCR[3] = PORT_PCR_MUX(2);     /* DSPI1.SIN    */
            pctl->PCR[4] = PORT_PCR_MUX(2);     /* DSPI1.PCS0   */

            /* Enable clock gate to SPI1 module */
            sim->SCGC6 |= SIM_SCGC6_SPI1_MASK;
            break;

        case 2:
            /* Configure GPIOD for DSPI2 peripheral function     */
            pctl = (PORT_MemMapPtr)PORTD_BASE_PTR;

            pctl->PCR[11] = PORT_PCR_MUX(2);    /* DSPI2.PCS0   */
            pctl->PCR[12] = PORT_PCR_MUX(2);    /* DSPI2.SCK    */
            pctl->PCR[13] = PORT_PCR_MUX(2);    /* DSPI2.SOUT   */
            pctl->PCR[14] = PORT_PCR_MUX(2);    /* DSPI2.SIN    */
            pctl->PCR[15] = PORT_PCR_MUX(2);    /* DSPI2.PCS1   */

            /* Enable clock gate to SPI2 module */
            sim->SCGC3 |= SIM_SCGC3_SPI2_MASK;
            break;

        default:
            /* do nothing if bad dev_num was selected */
            return -1;
    }

    return MQX_OK;

}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_esdhc_io_init
* Returned Value   : MQX_OK or -1
* Comments         :
*    This function performs BSP-specific initialization related to ESDHC
*
*END*----------------------------------------------------------------------*/

_mqx_int _bsp_esdhc_io_init
(
    uint8_t  dev_num,
    uint16_t value
)
{
    SIM_MemMapPtr   sim  = SIM_BASE_PTR;
    PORT_MemMapPtr  pctl;

    switch (dev_num)
    {
        case 0:
            /* Configure GPIO for SDHC peripheral function */
            pctl = (PORT_MemMapPtr)PORTE_BASE_PTR;

            pctl->PCR[0] = value & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.D1  */
            pctl->PCR[1] = value & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.D0  */
            pctl->PCR[2] = value & (PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK);                                          /* ESDHC.CLK */
            pctl->PCR[3] = value & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.CMD */
            pctl->PCR[4] = value & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.D3  */
            pctl->PCR[5] = value & (PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK);    /* ESDHC.D2  */

            /* Enable clock gate to SDHC module */
            sim->SCGC3 |= SIM_SCGC3_SDHC_MASK;
            break;

        default:
            /* Do nothing if bad dev_num was selected */
            return -1;
    }

    return MQX_OK;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_flexcan_io_init
* Returned Value   : 0 or 1 for success, -1 for failure
* Comments         :
*    This function performs BSP-specific initialization related to FLEXCAN
*
*END*----------------------------------------------------------------------*/
_mqx_int _bsp_flexcan_io_init
(
    uint8_t dev_num
)
{
    volatile CAN_MemMapPtr  can_reg_ptr;
    SIM_MemMapPtr   sim = SIM_BASE_PTR;
    PORT_MemMapPtr  pctl;

    OSC_CR |= OSC_CR_ERCLKEN_MASK;


    switch (dev_num)
    {
        case 0:
            /* Configure GPIO for FlexCAN 0 peripheral function */
            pctl = (PORT_MemMapPtr)PORTA_BASE_PTR;
            pctl->PCR[30] = (PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK);    /* CAN0_TX.A30  */
            pctl->PCR[31] = (PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK);    /* CAN0_RX.A31  */

            /* Enable clock gate to FlexCAN 0 module */
            sim->SCGC6 |= SIM_SCGC6_FLEXCAN0_MASK;

            /* Select the bus clock as CAN engine clock source */
            can_reg_ptr = _bsp_get_flexcan_base_address(0);
            can_reg_ptr->CTRL1 |= CAN_CTRL1_CLKSRC_MASK;

            break;
        case 1:
            /* Configure GPIO for FlexCAN 1 peripheral function */
            pctl = (PORT_MemMapPtr)PORTC_BASE_PTR;
            pctl->PCR[17] = (PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK);    /* CAN1_TX.C17  */
            pctl->PCR[16] = (PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK);    /* CAN1_RX.C16  */

            /* Enable clock gate to FlexCAN 1 module */
            sim->SCGC3 |= SIM_SCGC3_FLEXCAN1_MASK;

            /* Select the bus clock as CAN engine clock source */
            can_reg_ptr = _bsp_get_flexcan_base_address(1);
            can_reg_ptr->CTRL1 |= CAN_CTRL1_CLKSRC_MASK;

            break;

        default:
            return -1;
    }
    return 0;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_i2c_io_init
* Returned Value   : MQX_OK or -1
* Comments         :
*    This function performs BSP-specific initialization related to I2C
*
*END*----------------------------------------------------------------------*/

_mqx_int _bsp_i2c_io_init(uint32_t dev_num) {

   PORT_MemMapPtr pctl;
   SIM_MemMapPtr sim = SIM_BASE_PTR;

   switch (dev_num) {
   case 0:
      pctl = (PORT_MemMapPtr) PORTE_BASE_PTR;

      pctl->PCR[18] = PORT_PCR_MUX(4) | PORT_PCR_ODE_MASK; /* IIC0_SDA */
      pctl->PCR[19] = PORT_PCR_MUX(4) | PORT_PCR_ODE_MASK; /* IIC0_SCL */

      sim->SCGC4 |= SIM_SCGC4_I2C0_MASK;

      break;
   case 1:
      pctl = (PORT_MemMapPtr) PORTE_BASE_PTR;

      pctl->PCR[0] = PORT_PCR_MUX(6) | PORT_PCR_ODE_MASK; /* I2C1_SCL */
      pctl->PCR[1] = PORT_PCR_MUX(6) | PORT_PCR_ODE_MASK; /* I2C1_SDA */

      sim->SCGC4 |= SIM_SCGC4_I2C1_MASK;

      break;
   case 2:
      pctl = (PORT_MemMapPtr) PORTA_BASE_PTR;

      pctl->PCR[11] = PORT_PCR_MUX(5) | PORT_PCR_ODE_MASK; /* I2C2_SDA */
      pctl->PCR[12] = PORT_PCR_MUX(5) | PORT_PCR_ODE_MASK; /* I2C2_SCL */

      sim->SCGC1 |= SIM_SCGC1_I2C2_MASK;

      break;
   case 3:
      pctl = (PORT_MemMapPtr) PORTC_BASE_PTR;

      pctl->PCR[28] = PORT_PCR_MUX(2) | PORT_PCR_ODE_MASK; /* I2C3_SDA */
      pctl->PCR[29] = PORT_PCR_MUX(2) | PORT_PCR_ODE_MASK; /* I2C2_SCL */

      sim->SCGC1 |= SIM_SCGC1_I2C3_MASK;

      break;
   default:
      /* Do nothing if bad dev_num was selected */
      return -1;
   }
   return MQX_OK;

}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_enet_io_init
* Returned Value   : MQX_OK or -1
* Comments         :
*    This function performs BSP-specific initialization related to ENET
*
*END*----------------------------------------------------------------------*/

_mqx_int _bsp_enet_io_init
(
    uint32_t device
)
{
    PORT_MemMapPtr pctl;
    SIM_MemMapPtr  sim  = (SIM_MemMapPtr)SIM_BASE_PTR;

    pctl = (PORT_MemMapPtr)PORTA_BASE_PTR;
    pctl->PCR[12] = 0x00000400;     /* PTA12, RMII0_RXD1/MII0_RXD1      */
    pctl->PCR[13] = 0x00000400;     /* PTA13, RMII0_RXD0/MII0_RXD0      */
    pctl->PCR[14] = 0x00000400;     /* PTA14, RMII0_CRS_DV/MII0_RXDV    */
    pctl->PCR[15] = 0x00000400;     /* PTA15, RMII0_TXEN/MII0_TXEN      */
    pctl->PCR[16] = 0x00000400;     /* PTA16, RMII0_TXD0/MII0_TXD0      */
    pctl->PCR[17] = 0x00000400;     /* PTA17, RMII0_TXD1/MII0_TXD1      */


    pctl = (PORT_MemMapPtr)PORTA_BASE_PTR;
    pctl->PCR[7] = PORT_PCR_MUX(5) | PORT_PCR_ODE_MASK; /* PTA7, RMII0_MDIO/MII0_MDIO   */
    pctl->PCR[8] = PORT_PCR_MUX(5);                     /* PTA8, RMII0_MDC/MII0_MDC     */

#if ENETCFG_SUPPORT_PTP
    pctl = (PORT_MemMapPtr)PORTC_BASE_PTR;
    pctl->PCR[16+MACNET_PTP_TIMER] = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK; /* PTC16, ENET0_1588_TMR0   */
#endif

    /* Enable clock for ENET module */
    sim->SCGC2 |= SIM_SCGC2_ENET_MASK;

    return MQX_OK;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_usb_io_init
* Returned Value   : MQX_OK or -1
* Comments         :
*    This function performs BSP-specific I/O initialization related to USB
*
*END*----------------------------------------------------------------------*/

_mqx_int _bsp_usb_io_init
(
    _mqx_uint dev_num
)
{
#ifndef BSP_TWR_K65F180M
    if (dev_num == 0) {
#if PE_LDD_VERSION
        /* USB clock is configured using CPU component */

        /* Check if peripheral is not used by Processor Expert USB_LDD component */
        if (PE_PeripheralUsed((uint32_t)USB0_BASE_PTR) == TRUE) {
            /* IO Device used by PE Component*/
            return IO_ERROR;
        }
#endif
        /* Configure USB to be clocked from PLL0 */
        SIM_SOPT2_REG(SIM_BASE_PTR) &= ~(SIM_SOPT2_USBFSRC_MASK);
        SIM_SOPT2_REG(SIM_BASE_PTR) |= SIM_SOPT2_USBFSRC(1);
        /* Configure USB to be clocked from clock divider */
        SIM_SOPT2_REG(SIM_BASE_PTR) |= SIM_SOPT2_USBF_CLKSEL_MASK;
        /* Configure USB divider to be 120MHz * 2 / 5 = 48 MHz */
        SIM_CLKDIV2_REG(SIM_BASE_PTR) &= ~(SIM_CLKDIV2_USBFSDIV_MASK | SIM_CLKDIV2_USBFSFRAC_MASK);
        SIM_CLKDIV2_REG(SIM_BASE_PTR) |= SIM_CLKDIV2_USBFSDIV(4) | SIM_CLKDIV2_USBFSFRAC_MASK;

        /* Enable USB-OTG IP clocking */
        SIM_SCGC4_REG(SIM_BASE_PTR) |= SIM_SCGC4_USBFS_MASK;

        /* USB D+ and USB D- are standalone not multiplexed one-purpose pins */
        /* VREFIN for device is standalone not multiplexed one-purpose pin */

#if BSP_USB_TWR_SER2
        /* TWR-SER2 board has 2 connectors: on channel A, there is Micro-USB connector,
        ** which is not routed to TWRK60 board. On channel B, there is standard
        ** A-type host connector routed to the USB0 peripheral on TWRK60. To enable
        ** power to this connector, GPIO PB8 must be set as GPIO output
        */
        PORT_PCR_REG(PORTB_BASE_PTR, 8) = PORT_PCR_MUX(0x01);
        GPIO_PDDR_REG(PTB_BASE_PTR) |= 1 << 8; // PB8 as output
        GPIO_PDOR_REG(PTB_BASE_PTR) |= 1 << 8; // PB8 in high level
#endif
    }
    else if (dev_num == 1) {
        //Disable MPU so the module can access RAM
        MPU_CESR &= ~MPU_CESR_VLD_MASK;

        //Enable clock to the module
        SIM_SCGC6 |= SIM_SCGC6_USBHS_MASK;

        SIM_CLKDIV2 |= SIM_CLKDIV2_USBHSFRAC_MASK | SIM_CLKDIV2_USBHSDIV_MASK; // Divide reference clock to obtain 60MHz
        SIM_SOPT2 &= ~SIM_SOPT2_USBHSRC_MASK;  // MCGPLLCLK for the USB 60MHz CLKC source
        SIM_SOPT2 |= SIM_SOPT2_USBHSRC(1);     // MCGPLLCLK for the USB 60MHz CLKC source

        PORTA_PCR7 = PORT_PCR_MUX(2);   //ULPI DIR
        PORTA_PCR8 = PORT_PCR_MUX(2);   //ULPI NXT
        PORTA_PCR10 = PORT_PCR_MUX(2);  //ULPI DATA0
        PORTA_PCR11 = PORT_PCR_MUX(2);  //ULPI DATA1
        PORTA_PCR24 = PORT_PCR_MUX(2);  //ULPI DATA2
        PORTA_PCR25 = PORT_PCR_MUX(2);  //ULPI DATA3
        PORTA_PCR26 = PORT_PCR_MUX(2);  //ULPI DATA4
        PORTA_PCR27 = PORT_PCR_MUX(2);  //ULPI DATA5
        PORTA_PCR28 = PORT_PCR_MUX(2);  //ULPI DATA6
        PORTA_PCR29 = PORT_PCR_MUX(2);  //ULPI DATA7
        PORTA_PCR6 = PORT_PCR_MUX(2);   //ULPI CLK
        PORTA_PCR9 = PORT_PCR_MUX(2);   //ULPI STP
    }
    else {
        return IO_ERROR; //unknown controller
    }
#endif
    return MQX_OK;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_usb_dev_io_init
* Returned Value   : MQX_OK or -1
* Comments         :
*    This function performs BSP-specific I/O initialization related to USB
*
*END*----------------------------------------------------------------------*/

_mqx_int _bsp_usb_dev_io_init
(
    struct usb_dev_if_struct *dev_if
)
{
    if (dev_if->DEV_INIT_PARAM == &_khci0_dev_init_param) {
        _bsp_usb_io_init(0);
    }
    else if (dev_if->DEV_INIT_PARAM == &_ehci0_dev_init_param) {
        _bsp_usb_io_init(1);
    }
    else {
        return IO_ERROR; //unknown controller
    }

    return MQX_OK;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_usb_host_io_init
* Returned Value   : MQX_OK or -1
* Comments         :
*    This function performs BSP-specific I/O initialization related to USB
*
*END*----------------------------------------------------------------------*/

_mqx_int _bsp_usb_host_io_init
(
    struct usb_host_if_struct *dev_if
)
{
    if (dev_if->HOST_INIT_PARAM == &_khci0_host_init_param) {
        _bsp_usb_io_init(0);
    }
    else if (dev_if->HOST_INIT_PARAM == &_ehci0_host_init_param) {
        _bsp_usb_io_init(1);
    }
    else {
        return IO_ERROR; //unknown controller
    }

    return MQX_OK;
}


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_gpio_io_init
* Returned Value   : MQX_OK or -1
* Comments         :
*    This function performs BSP-specific initialization related to GPIO
*
*END*----------------------------------------------------------------------*/

_mqx_int _bsp_gpio_io_init
(
    void
)
{
    /* Enable clock gating to all ports */
    SIM_SCGC5 |=   SIM_SCGC5_PORTA_MASK \
                 | SIM_SCGC5_PORTB_MASK \
                 | SIM_SCGC5_PORTC_MASK \
                 | SIM_SCGC5_PORTD_MASK \
                 | SIM_SCGC5_PORTE_MASK;

    return MQX_OK;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_serial_rts_init
* Returned Value   : MQX_OK or -1
* Comments         :
*    This function performs BSP-specific initialization related to GPIO
*
*END*----------------------------------------------------------------------*/

_mqx_int _bsp_serial_rts_init
(
    uint32_t device_index
)
{
   PORT_MemMapPtr           pctl;

   /* set pin to RTS functionality */
   switch( device_index )
   {
      case 3:
         pctl = (PORT_MemMapPtr)PORTC_BASE_PTR;
         pctl->PCR[18] = 0 | PORT_PCR_MUX(3);
         break;
      case 4:
         pctl = (PORT_MemMapPtr)PORTC_BASE_PTR;
         pctl->PCR[12] = 0 | PORT_PCR_MUX(3);
         break;
      default:
         /* not used on this board */
         break;
   }
   return MQX_OK;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_ftfx_io_init
* Returned Value   : 0 for success, -1 for failure
* Comments         :
*    This function performs BSP-specific initialization related to Flash Memory Clock
*
*END*----------------------------------------------------------------------*/
_mqx_int _bsp_ftfx_io_init
(
    _mqx_uint device_index
)
{
    SIM_MemMapPtr sim = SIM_BASE_PTR;

    if (device_index > 0)
        return IO_ERROR;

    /* Clock the controller */
    sim->SCGC6 |= SIM_SCGC6_FTF_MASK;

    return MQX_OK;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_sai_io_init
* Returned Value   : 0 for success, -1 for failure
* Comments         :
*    This function performs BSP-specific initialization related to SAI
*
*END*----------------------------------------------------------------------*/
_mqx_int _bsp_sai_io_init
(
    uint32_t dev_num
)
{
    switch(dev_num)
    {
        case 0:

            SIM_SCGC6 |= SIM_SCGC6_I2S_MASK; /* Enable system clock to the I2S module */

            PORTE_PCR6  |= PORT_PCR_MUX(4); /* Pin Mux for I2S0_MCLK */
            PORTE_PCR7  |= PORT_PCR_MUX(4); /* Pin Mux for I2S0_RXD0 */
            PORTE_PCR8  |= PORT_PCR_MUX(4); /* Pin Mux for I2S0_RX_FS */
            PORTE_PCR9  |= PORT_PCR_MUX(4); /* Pin Mux for I2S0_RX_BCLK */
            PORTE_PCR10 |= PORT_PCR_MUX(4); /* Pin Mux for I2S0_TXD0 */
            PORTE_PCR11 |= PORT_PCR_MUX(4); /* Pin Mux for I2S0_TX_FS */
            PORTE_PCR12 |= PORT_PCR_MUX(4); /* Pin Mux for I2S0_TX_BCLK */

            break;
        default:
            break;
    }

    return MQX_OK;
}

/*FUNCTION*-------------------------------------------------------------------
 *
 * Function Name    : _bsp_serial_irda_tx_init
 * Returned Value   : MQX_OK or -1
 * Comments         :
 *    This function performs BSP-specific initialization related to IrDA
 *
 *END*----------------------------------------------------------------------*/

_mqx_int _bsp_serial_irda_tx_init(uint32_t device_index, bool enable)
{
    PORT_MemMapPtr pctl;
    /*
    SIM_MemMapPtr sim = SIM_BASE_PTR;
    */

    /* Only UART0 support IrDA transmit */
    if (0 != device_index)
    {
        return -1;
    }
    if (TRUE == enable)
    {
        /* PTD7 as UART TX function (Alt.3) + drive strength */
        pctl = (PORT_MemMapPtr) PORTD_BASE_PTR;
        pctl->PCR[7] = PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK;
    }
    else
    {
        return MQX_OK;
    }
    return MQX_OK;
}

/*FUNCTION*-------------------------------------------------------------------
 *
 * Function Name    : _bsp_serial_irda_rx_init
 * Returned Value   : MQX_OK or -1
 * Comments         :
 *    This function performs BSP-specific initialization related to IrDA
 *
 *END*----------------------------------------------------------------------*/

_mqx_int _bsp_serial_irda_rx_init(uint32_t device_index, bool enable)
{
    PORT_MemMapPtr pctl;
    SIM_MemMapPtr sim = SIM_BASE_PTR;
    CMP_MemMapPtr cmp = CMP0_BASE_PTR;

    if (TRUE == enable)
    {
        /* Enable comparator module */
        sim->SCGC4 |= SIM_SCGC4_CMP_MASK;

        /* Select RX source for UART */
        switch (device_index) {
        case 0:
            sim->SOPT5 &= ~SIM_SOPT5_UART0RXSRC_MASK;
            /* Select CMP0 as RX source for UART0*/
            sim->SOPT5 |= SIM_SOPT5_UART0RXSRC(1);
            break;
        case 1:
            sim->SOPT5 &= ~SIM_SOPT5_UART1RXSRC_MASK
            /* Select CMP0 as RX source for UART1*/;
            sim->SOPT5 |= SIM_SOPT5_UART1RXSRC(1);
        default:
            /* Do not support source select for other UART module*/
            return -1;
        }

        /* Setting Comparator module: Minus pin is DAC source, Plus pin is IrDA RX*/
        cmp->MUXCR =  CMP_MUXCR_MSEL(7) | CMP_MUXCR_PSEL(0);
        /* Enable DAC input comparator */
        cmp->DACCR = CMP_DACCR_DACEN_MASK | CMP_DACCR_VOSEL(BSP_SERIAL_IRDA_DAC_VREF)
                | CMP_DACCR_VRSEL_MASK;

        /* PTC6 as CPM IN function (Alt.0) */
        pctl = (PORT_MemMapPtr) PORTC_BASE_PTR;
        pctl->PCR[6] = PORT_PCR_MUX(0);

        /* Enable Comparator module */
        cmp->CR1 |= CMP_CR1_EN_MASK | CMP_CR1_COS_MASK
                | CMP_CR1_OPE_MASK | CMP_CR1_PMODE_MASK;
    }
    else
    {
        switch (device_index) {
        case 0:
            sim->SOPT5 &= ~SIM_SOPT5_UART0RXSRC_MASK;
            /* Select UART0_RX pin as RX source for UART0*/
            sim->SOPT5 |= SIM_SOPT5_UART0RXSRC(0);
            break;
        case 1:
            sim->SOPT5 &= ~SIM_SOPT5_UART1RXSRC_MASK
            /* Select UART1_RX pin as RX source for UART1*/;
            sim->SOPT5 |= SIM_SOPT5_UART1RXSRC(0);
        default:
            /* Do not support source select for other UART module*/
            return -1;
        }
        /* Disable comparator module */
        sim->SCGC4 &= ~SIM_SCGC4_CMP_MASK;
    }
    return MQX_OK;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_crc_io_init
* Returned Value   : MQX_OK
* Comments         :
*    This function performs BSP-specific initialization related to CRC.
*
*END*----------------------------------------------------------------------*/

_mqx_int _bsp_crc_io_init(void)
{
    SIM_MemMapPtr sim = SIM_BASE_PTR;

    sim->SCGC6 |= SIM_SCGC6_CRC_MASK;

    return MQX_OK;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_sdramc_io_init
* Returned Value   : MQX_OK
* Comments         :
*    This function performs BSP-specific initialization related to SDRAM.
*
*END*----------------------------------------------------------------------*/
_mqx_int _bsp_sdramc_io_init(void)
{
    SIM_MemMapPtr sim = SIM_BASE_PTR;
    FB_MemMapPtr fb = FB_BASE_PTR;
    PORT_MemMapPtr port;

    sim->SCGC7 |= (SIM_SCGC7_SDRAMC_MASK | SIM_SCGC7_FLEXBUS_MASK);
    /* fb_byte select and sdram_dqm functionality enabled, otherwise, no sdram_dqm output */
    fb->CSPMCR = FB_CSPMCR_GROUP2(2)|FB_CSPMCR_GROUP3(2)|FB_CSPMCR_GROUP4(2)|FB_CSPMCR_GROUP5(2);

    /* For K65, K66 don't have SDRAMC_D11-D0
     * Configure SDRAM Data  GPIOs
     * 1. set pin mux as SDRAM_xx function
     * 2. high drive strength set */
    /* BS3 */
    port = PORTB_BASE_PTR;
    port->PCR[20] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[31] */
    port->PCR[21] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[30] */
    port->PCR[22] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[29] */
    port->PCR[23] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[28] */
    port = PORTC_BASE_PTR;
    port->PCR[12] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[27] */
    port->PCR[13] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[26] */
    port->PCR[14] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[25] */
    port->PCR[15] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[24] */
    /* BS2 */
    port = PORTB_BASE_PTR;
    port->PCR[6]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[23] */
    port->PCR[7]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[22] */
    port->PCR[8]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[21] */
    port->PCR[9]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[20] */
    port->PCR[10] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[19] */
    port->PCR[11] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[18] */
    port->PCR[16] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[17] */
    port->PCR[17] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[16] */
    /* BS1 */
    port = PORTA_BASE_PTR;
    port->PCR[24] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[15] */
    port->PCR[25] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[14] */
    port->PCR[26] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[13] */
    port->PCR[27] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[12] */
    port->PCR[30] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[11] */
    port->PCR[31] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[10] */
    port = PORTB_BASE_PTR;
    port->PCR[12] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[9] */
    port->PCR[13] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[8] */
    /* BS0 */
    port->PCR[14] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[7] */
    port->PCR[15] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[6] */
    port = PORTC_BASE_PTR;
    port->PCR[24] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[5] */
    port->PCR[25] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[4] */
    port->PCR[26] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[3] */
    port->PCR[27] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[2] */
    port->PCR[28] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[1] */
    port->PCR[29] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_D[0] */

    /* Configure SDRAM Address  GPIOs
     *1. set pin mux as SDRAM_xx function
     *2. high drive strength set */
    port = PORTD_BASE_PTR;
    port->PCR[5] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_A[9] */
    port->PCR[4] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_A[10] */
    port->PCR[3] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_A[11] */
    port->PCR[2] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_A[12] */
    port = PORTC_BASE_PTR;
    port->PCR[10] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_A[13] */
    port->PCR[9] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_A[14] */
    port->PCR[8] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_A[15] */
    port->PCR[7] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_A[16] */
    port->PCR[6] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_A[17] */
    port->PCR[5] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_A[18] */
    port->PCR[4] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_A[19] */
    port->PCR[2] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_A[20] */
    port->PCR[1] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_A[21] */
    port->PCR[0] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_A[22] */
    port = PORTB_BASE_PTR;
    port->PCR[18]= PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           /* SDRAM_A[23] */

    /* Config control signals */
    /* Enable the SDRAMC_CLKOUT function on PTC3 (alt5 function) */
    sim->SOPT2 &= ~SIM_SOPT2_CLKOUTSEL_MASK; /* clear clkoout field */
    sim->SOPT2 |= SIM_SOPT2_CLKOUTSEL(0);    /* select Flexbus clock */

    /* for twr pta6 */
    port = PORTA_BASE_PTR;
    port->PCR[6] = ( PORT_PCR_MUX(0x5)| PORT_PCR_DSE_MASK);     /* SDRAM_CLK for TWR */
    port = PORTD_BASE_PTR;
    port->PCR[7]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;          /* SDRAM_CKE */
    port = PORTB_BASE_PTR;
    port->PCR[0]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;          /* SDRAM_CAS_B */
    port->PCR[1]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;          /* SDRAM_RAS_B */
    port->PCR[3]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;          /* SDRAM_CS0_B */
    port->PCR[4]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;          /* SDRAM_CS1_B */
    port->PCR[2]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;          /* SDRAM_WE */
    port = PORTC_BASE_PTR;
    port->PCR[17] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;          /* SDRAM_DQM3 */
    port->PCR[16] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;          /* SDRAM_DQM2 */
    port->PCR[18] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;          /* SDRAM_DQM1 */
    port->PCR[19] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;          /* SDRAM_DQM0 */

    return MQX_OK;
}

/* EOF */
