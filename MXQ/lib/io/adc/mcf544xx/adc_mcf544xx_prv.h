#ifndef __adc_mcf52xx_prv_h__
#define __adc_mcf52xx_prv_h__
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
#include "adc.h"
#include "adc_prv.h"
#include "adc_conf.h"

/*----------------------------------------------------------------------*/
/*
**                          CONSTANT DEFINITIONS
*/

#ifdef __cplusplus
extern "C" {
#endif

#define ADC_MAX_CHANNELS ADC_MAX_SW_CHANNELS

/* Structure holding ADC driver settings */
typedef struct adc {
   ADC_GENERIC           g; /* this must be included as the first item at the beginning of structure */
   /* below, CPU specific items are used */
   ADC_HW_CHANNEL_MASK   last_mask;
} ADC, * ADC_PTR;

/* Structure holding ADC driver settings */
typedef struct adt {
   ADT_GENERIC           g; /* this must be included as the first item at the beginning of structure */
   /* below, CPU specific items are used */
   _mqx_int              shift; /* this is the radix we can shift to get from (prescaler+preset) scale to the lowest time */
   uint32_t   time_to_next_adc; /* stores time needed to next AD conversion */
   uint32_t   time_to_next_irq; /* stores time needed to next timer IRQ */
   uint32_t   dispatch_time;    /* stores absolute time for dispatcher */
   bool   dispatch_next;    /* informs if any data within channels_to_next_adc is valid */
} ADT, * ADT_PTR;

/* Structure holding ADC driver channel settings, must be declared */
typedef struct adc_channel {
   ADC_CHANNEL_GENERIC   g; /* this must be included as the first member at the beginning of structure */
   /* below, CPU specific members are used */
} ADC_CHANNEL, * ADC_CHANNEL_PTR;

/* Structure holding ADT driver channel settings, must be declared */
typedef struct adt_channel {
   ADT_CHANNEL_GENERIC   g; /* this must be included as the first member at the beginning of structure */
   /* below, CPU specific members are used */
} ADT_CHANNEL, * ADT_CHANNEL_PTR;

/* ADC converter + trigger channel bundle structure */
typedef struct adc_channel_bundle {
   ADC_CHANNEL_PTR  adc_ch;
   ADT_CHANNEL_PTR  adt_ch;
   /* below, CPU specific members are used */
} ADC_CHANNEL_BUNDLE, * ADC_CHANNEL_BUNDLE_PTR;

/* ADC converter + trigger bundle structure */
typedef struct adc_driver_bundle {
   ADC_PTR          adc;                      /* ADC driver status information (generic + HW specific) */
   ADT_PTR          adt;                      /* ADT driver status information (generic + HW specific) */
   ADC_CHANNEL_PTR  adc_ch[ADC_MAX_CHANNELS];
   ADT_CHANNEL_PTR  adt_ch[ADC_MAX_CHANNELS];
   unsigned char        *adc_name[ADC_MAX_CHANNELS]; /* filename of channel(s) */
   /* below, CPU specific members are used */
   uint32_t          last_mask; /* mask of ADC channels to perform ADC next time */
} ADC_DRIVER_BUNDLE, * ADC_DRIVER_BUNDLE_PTR;

/* anything static can be shared within ADC and ADT */
/* typedef struct adc_bundle {
} ADC_BUNDLE, ADC_BUNDLE_PTR; */

_mqx_int _adc_hw_install(char *, ADC_DRIVER_BUNDLE_PTR, void *);
_mqx_int _adt_hw_install(char *, ADC_DRIVER_BUNDLE_PTR, void *);
_mqx_int _adc_hw_init(MQX_FILE_PTR, ADC_PTR);
_mqx_int _adt_hw_init(MQX_FILE_PTR, ADT_PTR);
_mqx_int _adc_hw_deinit(MQX_FILE_PTR, ADC_PTR);
_mqx_int _adt_hw_deinit(MQX_FILE_PTR, ADT_PTR);
_mqx_int _adc_hw_channel_init(MQX_FILE_PTR, ADC_CHANNEL_PTR);
_mqx_int _adt_hw_channel_init(MQX_FILE_PTR, ADT_CHANNEL_PTR);
_mqx_int _adc_hw_channel_deinit(MQX_FILE_PTR, ADC_CHANNEL_PTR);
_mqx_int _adt_hw_channel_deinit(MQX_FILE_PTR, ADT_CHANNEL_PTR);
_mqx_int _adc_hw_ioctl(MQX_FILE_PTR, _mqx_uint, void *);
_mqx_int _adt_hw_ioctl(MQX_FILE_PTR, _mqx_uint, void *);

void _adc_hw_measure(ADC_HW_CHANNEL_MASK);

#ifdef __cplusplus
}
#endif

#endif
