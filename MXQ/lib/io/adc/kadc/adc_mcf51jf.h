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
*   CPU specific ADC driver header file
*
*
*END************************************************************************/

#ifndef __adc_mcf51jf_h__
#define __adc_mcf51jf_h__


#define ADC_HAS_16BIT_RESOLUTION           1
#define ADC_HAS_DIFFERENTIAL_CHANNELS      0
#define ADC_HAS_PGA                        0

/* ADC source channel masks definitions */
#define ADC0_SOURCE_ADPM0   (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(0))
#define ADC0_SOURCE_ADPM1   (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(1))
#define ADC0_SOURCE_ADPM2   (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(2))
#define ADC0_SOURCE_ADPM3   (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(3))
#define ADC0_SOURCE_AD0     (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(0))
#define ADC0_SOURCE_AD1     (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(1))
#define ADC0_SOURCE_AD2     (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(2))
#define ADC0_SOURCE_AD3     (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(3))
#define ADC0_SOURCE_AD4B    (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_B | ADC_SOURCE_CHANNEL(4))
#define ADC0_SOURCE_AD5B    (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_B | ADC_SOURCE_CHANNEL(5))
#define ADC0_SOURCE_AD6B    (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_B | ADC_SOURCE_CHANNEL(6))
#define ADC0_SOURCE_AD7B    (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_B | ADC_SOURCE_CHANNEL(7))
#define ADC0_SOURCE_AD8     (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(8))
#define ADC0_SOURCE_AD9     (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(9))
#define ADC0_SOURCE_AD10    (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(10))
#define ADC0_SOURCE_AD11    (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(11))
#define ADC0_SOURCE_AD12    (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(12))
#define ADC0_SOURCE_AD13    (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(13))
#define ADC0_SOURCE_AD14    (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(14))
#define ADC0_SOURCE_AD15    (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(15))
#define ADC0_SOURCE_AD16    (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(16))
#define ADC0_SOURCE_AD17    (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(17))
#define ADC0_SOURCE_AD18    (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(18))
#define ADC0_SOURCE_AD19    (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(19))
#define ADC0_SOURCE_AD20    (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(20))
#define ADC0_SOURCE_AD21    (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(21))
#define ADC0_SOURCE_AD22    (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(22))
#define ADC0_SOURCE_AD23    (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(23))

#define ADC_SOURCE_PMTEMP   (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(26))
#define ADC_SOURCE_TEMP     (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(26))
#define ADC_SOURCE_PMBG     (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(27))
#define ADC_SOURCE_BGL      (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(27))
#define ADC_SOURCE_PMVREFH  (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(29))
#define ADC_SOURCE_VREFH    (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(29))
#define ADC_SOURCE_VREFL    (ADC_SOURCE_MODULE(0) | ADC_SOURCE_MUXSEL_X | ADC_SOURCE_CHANNEL(30))


#endif /* __adc_mcf51jf_h__ */
