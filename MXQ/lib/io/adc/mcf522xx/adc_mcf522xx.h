#ifndef __adc_mcf52xx_h__
#define __adc_mcf52xx_h__
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

/*----------------------------------------------------------------------*/
/*
**                          CONSTANT DEFINITIONS
*/

#ifdef __cplusplus
extern "C" {
#endif

#define ADC_HW_CHANNELS         8

#define ADC_RESOLUTION_12BIT   0x1000
#define ADC_RESOLUTION_DEFAULT ADC_RESOLUTION_12BIT

#define ADC_SOURCE_AN0      0x00
#define ADC_SOURCE_AN1      0x01
#define ADC_SOURCE_AN2      0x02
#define ADC_SOURCE_AN3      0x03
#define ADC_SOURCE_AN4      0x04
#define ADC_SOURCE_AN5      0x05
#define ADC_SOURCE_AN6      0x06
#define ADC_SOURCE_AN7      0x07

#define ADC_SOURCE_AN0_AN1  0x08
#define ADC_SOURCE_AN2_AN3  0x09
#define ADC_SOURCE_AN4_AN5  0x10
#define ADC_SOURCE_AN6_AN7  0x11
#define ADC_SOURCE_MAX      0x11

void   *_bsp_get_adc_base_address(void);

#ifdef __cplusplus
}
#endif

#endif

/* EOF */
