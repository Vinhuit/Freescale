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
*   This file contains the ADC driver CPU specific functions
*
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <fio.h>
#include <fio_prv.h>
#include "io.h"
#include "adc.h"
#include "adc_prv.h"
#include "adc_conf.h"
#include "adc_mcf544xx_prv.h"

#if BSPCFG_ENABLE_ADC

/* Peripherals pointer */
static VMCF544XX_ADC_STRUCT_PTR  mcf5441x_adc_ptr;

/* static variables */
static void (_CODE_PTR_ old_isr_adc1)(void *);
static void            *old_isr_adc1_param;
static ADC_DRIVER_BUNDLE_PTR driver_bundle = NULL;

/* Declarations of IRQ handlers */
static void _adc_g_conv_end(uint16_t *results, ADC_HW_CHANNEL_MASK mask);
static void adc_complete_isr(void *);
static uint32_t adc_channels_used = 0;

/*FUNCTION*****************************************************************
* 
* Function Name    : _adc_hw_ioctl
* Returned Value   : _mqx_int
* Comments         :
*    Returns result of ioctl operation.
*
*END*********************************************************************/
_mqx_int _adc_hw_ioctl(MQX_FILE_PTR f, _mqx_uint cmd, void *param_ptr)
{
    return IO_ERROR_INVALID_IOCTL_CMD;
}

/*FUNCTION*****************************************************************
* 
* Function Name    : _adc_hw_install
* Returned Value   : _mqx_int
* Comments         :
*    Performs CPU specific initialization for ADC driver (during install)
*
*END*********************************************************************/
_mqx_int _adc_hw_install(char *a, ADC_DRIVER_BUNDLE_PTR b, void *c)
{
   return IO_OK;
}

/*FUNCTION*****************************************************************
* 
* Function Name    : _adc_hw_init
* Returned Value   : _mqx_int
* Comments         :
*    Performs CPU specific initialization for ADC driver
*
*END*********************************************************************/
_mqx_int _adc_hw_init(MQX_FILE_PTR f, ADC_PTR adc)
{ /* Body */
    ADC_DRIVER_BUNDLE_PTR adc_d_bundle = (ADC_DRIVER_BUNDLE_PTR) f->DEV_PTR->DRIVER_INIT_PTR;

#if PSP_MQX_CPU_IS_MCF5441X
    VMCF5441_STRUCT_PTR mcf5441x_ptr;
#else
    #error This CPU is not supported here
#endif

    #define MAX_ADC_BUS_CLK   125000000
    #define MAX_ADC_CONV_CLK  12000000
    #define AUTO_STANDBY_CLK  200000
        
    _mqx_uint        div_register;
    _mqx_int         i;

    /* Prepare pointers to peripherals */
    mcf5441x_ptr = _PSP_GET_IPSBAR();
    mcf5441x_adc_ptr = _bsp_get_adc_base_address();

    if (adc->g.resolution != ADC_RESOLUTION_12BIT)
        return ADC_ERROR_BAD_PARAM; /* cannot set ADC resolution */
        
    /* Test for maximum busclock frequency */
    if( BSP_BUS_CLOCK > MAX_ADC_BUS_CLK )
        return ADC_ERROR_BAD_PARAM;

    /* enable ADC peripherial */
    (void)_bsp_adc_enable_clock();
    
    /* Prepare ADC clocking to be as fast as possible */
    div_register = ( ( BSP_BUS_CLOCK + MAX_ADC_CONV_CLK ) / ( 2 * MAX_ADC_CONV_CLK )) - 1;
    if (div_register > 127)
        return ADC_ERROR_BAD_PARAM;  /* cannot set ADC frequency */
    /* store information about exact ADC clock frequency */
    adc->g.adc_frq = (BSP_BUS_CLOCK) / (2*(div_register + 1));

    /* set standby frequency */
    mcf5441x_adc_ptr->ADC_ASDIV = ((BSP_BUS_CLOCK + AUTO_STANDBY_CLK) / ( 2*AUTO_STANDBY_CLK )) - 1;
    
    /* Prepare AD converter, but do not start it */
    /* Select the same clock divisors for converters A and B */
    mcf5441x_adc_ptr->ADC_DIV = ( div_register & 0x7F ) | (( div_register & 0x7F ) << 8 );
    mcf5441x_adc_ptr->ADC_CR2 = MCF544XX_ADC_CR2_EOSIE1; /* Channel B can run in parallel with channel A */
    mcf5441x_adc_ptr->ADC_CR1 = MCF544XX_ADC_CR1_EOSIE0; /* Once sequential mode, single conversion, all inputs single-ended */
    //mcf5441x_adc_ptr->ADC_PWR &= 0xfff8; /* Power up ADC */
    mcf5441x_adc_ptr->ADC_PWR |= MCF544XX_ADC_PWR_APD | MCF544XX_ADC_PWR_ASB | MCF544XX_ADC_PWR_PUDELAY_WRITE(0x3F);
    mcf5441x_adc_ptr->ADC_PWR &= ~(MCF544XX_ADC_PWR_PD(0) | MCF544XX_ADC_PWR_PD(1) | 
        MCF544XX_ADC_PWR_PD(2) | MCF544XX_ADC_PWR_PD(3) );
/* Install ISR for ADC */
#if PSP_MQX_CPU_IS_MCF5441X
    old_isr_adc1_param = _int_get_isr_data(BSP_ADC_INTERRUPT_VECTOR);
    old_isr_adc1 = _int_install_isr(BSP_ADC_INTERRUPT_VECTOR, adc_complete_isr, NULL);
#else
    #error This CPU is not supported here
#endif

    if (NULL == old_isr_adc1)
        return ADC_ERROR_ISR; /* could not install new interrupt routine */

#if PSP_MQX_CPU_IS_MCF5441X
    /* Setup ISR levels for ADC */
    _mcf5441_int_init(BSP_ADC_INTERRUPT_VECTOR, BSP_ADC_INTERRUPT_LEVEL, TRUE);
#else
        #error This CPU is not supported here
#endif

    driver_bundle = adc_d_bundle;
    return IO_OK;
} /* Endbody */

/*FUNCTION*****************************************************************
* 
* Function Name    : _adc_hw_deinit
* Returned Value   : _mqx_int
* Comments         :
*    Performs CPU specific deinitialization for ADC driver
*
*END*********************************************************************/
_mqx_int _adc_hw_deinit(MQX_FILE_PTR f, ADC_PTR adc) {
    return IO_OK;
}

/*FUNCTION*****************************************************************
* 
* Function Name    : _adc_hw_channel_init
* Returned Value   : int32_t
* Comments         :
*    Returns result of HW channel initialization
*
*END*********************************************************************/
_mqx_int _adc_hw_channel_init(MQX_FILE_PTR f, ADC_CHANNEL_PTR adc_ch)
{ /* Body */
    _mqx_uint result;

    if (adc_ch->g.source > ADC_SOURCE_AN6_AN7)
        return ADC_ERROR_BAD_PARAM; /* ADC_SOURCE_ constant does not match any source signal for this CPU */
    if ((1 << adc_ch->g.source) & adc_channels_used)
        return ADC_ERROR_OPENED; /* channel already used */
    /* configure GPIO and MUX for ADC */
    if (IO_OK == (result = _bsp_adc_io_init(adc_ch->g.source)))
        adc_channels_used |= 1 << adc_ch->g.source;

    return result;        
} /* Endbody */

/*FUNCTION*****************************************************************
* 
* Function Name    : _adc_hw_channel_deinit
* Returned Value   : _mqx_int
* Comments         :
*    Performs CPU specific deinitialization for ADC channel
*
*END*********************************************************************/
_mqx_int _adc_hw_channel_deinit(MQX_FILE_PTR f, ADC_CHANNEL_PTR adc_ch) {
    adc_channels_used &= ~(1 << adc_ch->g.source);
    return ADC_OK;
}

/*FUNCTION****************************************************************
* 
* Function Name    : _adc_hw_measure
* Returned Value   : void
* Comments         : Prepares and runs ADC to measure data from channel
*
*END*********************************************************************/
void _adc_hw_measure(ADC_HW_CHANNEL_MASK mask) {
    _mqx_int i, j;

    mcf5441x_adc_ptr->ADC_SDIS = 0x00ff; /* presume all disabled */
    mcf5441x_adc_ptr->ADC_LST1 = mcf5441x_adc_ptr->ADC_LST2 = 0;
    for (i = 0, j = 0; i < ADC_SOURCE_MAX + 1; i++) {
        if ((1 << i) & mask) {
            if (j < 4)
                mcf5441x_adc_ptr->ADC_LST1 |= i << (4 * j++);
            else
                mcf5441x_adc_ptr->ADC_LST2 |= i << (4 * (j++ - 4));
            
            mcf5441x_adc_ptr->ADC_SDIS = mcf5441x_adc_ptr->ADC_SDIS << 1;
        }
    }
    mcf5441x_adc_ptr->ADC_CR1 |= MCF544XX_ADC_CR1_START0; /* start AD conversion */ 
    driver_bundle->last_mask = mask;
}

/*FUNCTION*****************************************************************
* 
* Function Name    : adc_complete_isr
* Returned Value   : void
* Comments         : IRQ handler for completed AD conversion(s)
*    In adc_c.channels_to_next_adc we have list of channels that
*    have been sampled and whole we need to handle the results.
*    We will copy results from AD converter to channels buffers.
*    Since AD converter can hold 8 samples a time, we check if any
*    other channel needs to be sampled and if yes, prepare next AD
*    conversion.
*
*END*********************************************************************/
static void adc_complete_isr
    (
        /* [IN] the address of the device specific information */
        void   *parameter
    )
{ /* Body */
    _mqx_int    i, j;
    uint16_t *results = (void *) &mcf5441x_adc_ptr->ADC_RSLT[0]; /* prepare pointer to peripheral results */
    uint16_t     results_internal[ADC_SOURCE_MAX]; /* 8 - number of results ADC can produce in one shot */
    mcf5441x_adc_ptr->ADC_SR |= MCF544XX_ADC_SR_EOSI0 | MCF544XX_ADC_SR_EOSI1; /* clear interrupt flags */
   
    /* perform copies from ADC */
    for (i = 0; ((mcf5441x_adc_ptr->ADC_SDIS & 0x0001) == 0x0000) && (i < 8); i++, mcf5441x_adc_ptr->ADC_SDIS >>= 1) {
        if (i < 4) {
            j = mcf5441x_adc_ptr->ADC_LST1 & 0x000f;
            mcf5441x_adc_ptr->ADC_LST1 >>= 4;
        }
        else {
            j = mcf5441x_adc_ptr->ADC_LST2 & 0x000f;
            mcf5441x_adc_ptr->ADC_LST2 >>= 4;
        }
        results_internal[j] = results[i] >> 3;
    }
    _adc_g_conv_end(results_internal, driver_bundle->last_mask);
} /* Endbody */

/*FUNCTION*****************************************************************
* 
* Function Name    : _adc_g_conv_end
* Returned Value   : 
* Comments         : This is called by low-level driver to inform generic
*                    driver, that some conversion was performed. Function
*                    moves stored data to the channel buffers.
*END*********************************************************************/
static void _adc_g_conv_end(uint16_t *results_buf, ADC_HW_CHANNEL_MASK mask)
{
    _mqx_int i;
    ADC_CHANNEL_PTR *adc_ch = driver_bundle->adc_ch;
    ADT_CHANNEL_PTR *adt_ch = driver_bundle->adt_ch;

    for (i = 0; i < ADC_MAX_CHANNELS; i++) {
        if (!adt_ch[i] || !(adt_ch[i]->g.runtime_flags & ADC_CHANNEL_RESUMED) || !(mask & (1 << adc_ch[i]->g.source)))
            continue;
            
        if (adc_ch[i]->g.init_flags & ADC_CHANNEL_ACCUMULATE)
            /* if the channel has ADC_CHANNEL_ACCUMULATE, then accumulate result */
            adc_ch[i]->g.buffer_driver->result += results_buf[adc_ch[i]->g.source];

        else
            /* else copy the result to the channel buffer */
            adc_ch[i]->g.buffer_driver->result = results_buf[adc_ch[i]->g.source];

        if (!(adc_ch[i]->g.init_flags & ADC_CHANNEL_ACCUMULATE) || !(adt_ch[i]->g.samples)) {
            /* move driver pointer to next index, circullarily */
            if (++adc_ch[i]->g.buffer_driver >= adc_ch[i]->g.buffer_end)
                adc_ch[i]->g.buffer_driver = adc_ch[i]->g.buffer_start;

            /* if new value should overwrite old value in ring buffer, move read pointer to next value */
            if (adc_ch[i]->g.buffer_driver == adc_ch[i]->g.buffer_task) {
                if (++adc_ch[i]->g.buffer_task >= adc_ch[i]->g.buffer_end)
                    adc_ch[i]->g.buffer_task = adc_ch[i]->g.buffer_start;
            }
        }
        
        if (adt_ch[i]->g.samples) {
            adt_ch[i]->g.samples--;
        }
        else {
#if MQX_USE_LWEVENTS
            /* if it is the last sample in the batch, then set event */
            if (adt_ch[i]->g.complete_event)
                _lwevent_set(adt_ch[i]->g.complete_event, adt_ch[i]->g.event_mask); /* set completion event */
#endif
                
            if (adc_ch[i]->g.init_flags & ADC_CHANNEL_MEASURE_ONCE)
                adt_ch[i]->g.runtime_flags &= ~ADC_CHANNEL_RUNNING;
            
            adt_ch[i]->g.samples = adt_ch[i]->g.samples_preset;
        }
    }
}

/*FUNCTION****************************************************************
* 
* Function Name    : _bsp_get_adc_base_address
* Returned Value   : pointer to base of ADC registers
* Comments         :
*    This function returns base address of ADC related register space.
*
*END*********************************************************************/
void *_bsp_get_adc_base_address
(
    void
)
{
#if PSP_MQX_CPU_IS_MCF5441X
    return (void *)(&((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->ADC);
#else
    #error This CPU is not supported here
    return NULL;
#endif
}

#endif // BSPCFG_ENABLE_ADC
/* EOF */
