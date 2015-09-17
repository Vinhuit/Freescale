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
*   This file contains the type definitions for the mcf52xx I2C module.
*
*
*END************************************************************************/

#ifndef __mcf544xx_adc_h__
#define __mcf544xx_adc_h__

#define __mcf544xx_adc_h__version "$Version:3.5.1.0$"
#define __mcf544xx_adc_h__date    "$Date:Mar-24-2010$"

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------*/
/*
** ADC  registers bit set.
*/

#define MCF544XX_ADC_CR1_STOP0                     (1<<14)
#define MCF544XX_ADC_CR1_START0                    (1<<13)
#define MCF544XX_ADC_CR1_SYNC0                     (1<<12)
#define MCF544XX_ADC_CR1_EOSIE0                    (1<<11)
#define MCF544XX_ADC_CR1_ZCIE                      (1<<10)
#define MCF544XX_ADC_CR1_LLMTIE                    (1<<9)
#define MCF544XX_ADC_CR1_HLMTIE                    (1<<8)
#define MCF544XX_ADC_CR1_CHNCFG_READ(x)            (((x)>>0x04)&0x0F)
#define MCF544XX_ADC_CR1_CHNCFG_WRITE(x)           (((x)&0x0F)<<0x04)
#define MCF544XX_ADC_CR2_STOP1                     (1<<14)
#define MCF544XX_ADC_CR2_START1                    (1<<13)
#define MCF544XX_ADC_CR2_SYNC1                     (1<<12)
#define MCF544XX_ADC_CR2_EOSIE1                    (1<<11)
#define MCF544XX_ADC_CR2_SIMULT                    (1<<7)
#define MCF544XX_ADC_ZCCR_ZCE0_WRITE(x)            (((x)&0x03)<<0x00)
#define MCF544XX_ADC_ZCCR_ZCE1_WRITE(x)            (((x)&0x03)<<0x02)
#define MCF544XX_ADC_ZCCR_ZCE2_WRITE(x)            (((x)&0x03)<<0x04)
#define MCF544XX_ADC_ZCCR_ZCE3_WRITE(x)            (((x)&0x03)<<0x06)
#define MCF544XX_ADC_ZCCR_ZCE4_WRITE(x)            (((x)&0x03)<<0x08)
#define MCF544XX_ADC_ZCCR_ZCE5_WRITE(x)            (((x)&0x03)<<0x0A)
#define MCF544XX_ADC_ZCCR_ZCE6_WRITE(x)            (((x)&0x03)<<0x0C)
#define MCF544XX_ADC_ZCCR_ZCE7_WRITE(x)            (((x)&0x03)<<0x0E)
#define MCF544XX_ADC_ZCCR_ZCE0_READ(x)             (((x)>>0x00)&0x03)
#define MCF544XX_ADC_ZCCR_ZCE1_READ(x)             (((x)>>0x02)&0x03)
#define MCF544XX_ADC_ZCCR_ZCE2_READ(x)             (((x)>>0x04)&0x03)
#define MCF544XX_ADC_ZCCR_ZCE3_READ(x)             (((x)>>0x06)&0x03)
#define MCF544XX_ADC_ZCCR_ZCE4_READ(x)             (((x)>>0x08)&0x03)
#define MCF544XX_ADC_ZCCR_ZCE5_READ(x)             (((x)>>0x0A)&0x03)
#define MCF544XX_ADC_ZCCR_ZCE6_READ(x)             (((x)>>0x0C)&0x03)
#define MCF544XX_ADC_ZCCR_ZCE7_READ(x)             (((x)>>0x0E)&0x03)
#define MCF544XX_ADC_LST1_SAMPLE0_WRITE(x)         (((x)&0x07)<<0)
#define MCF544XX_ADC_LST1_SAMPLE1_WRITE(x)         (((x)&0x07)<<4)
#define MCF544XX_ADC_LST1_SAMPLE2_WRITE(x)         (((x)&0x07)<<8)
#define MCF544XX_ADC_LST1_SAMPLE3_WRITE(x)         (((x)&0x07)<<12)
#define MCF544XX_ADC_LST2_SAMPLE4_WRITE(x)         (((x)&0x07)<<0)
#define MCF544XX_ADC_LST2_SAMPLE5_WRITE(x)         (((x)&0x07)<<4)
#define MCF544XX_ADC_LST2_SAMPLE6_WRITE(x)         (((x)&0x07)<<8)
#define MCF544XX_ADC_LST2_SAMPLE7_WRITE(x)         (((x)&0x07)<<12)
#define MCF544XX_ADC_SDIS_DS(x)                    (1<<(x))
#define MCF544XX_ADC_SR_RDY(x)                     (1<<(x))
#define MCF544XX_ADC_SR_HLMTI                      (1<<8)
#define MCF544XX_ADC_SR_LLMTI                      (1<<9)
#define MCF544XX_ADC_SR_ZCI                        (1<<10)
#define MCF544XX_ADC_SR_EOSI0                      (1<<11)
#define MCF544XX_ADC_SR_EOSI1                      (1<<12)
#define MCF544XX_ADC_SR_CIP1                       (1<<14)
#define MCF544XX_ADC_SR_CIP0                       (1<<15)
#define MCF544XX_ADC_LSR_LLS(x)                    (1<<(x))
#define MCF544XX_ADC_LSR_HLS(x)                    (1<<(8 +(x)))
#define MCF544XX_ADC_ZCSR_ZCS(x)                   (1<<(x))
#define MCF544XX_ADC_RSLT_RSLT_READ(x)             (((x)>>3)&0x0FFF)
#define MCF544XX_ADC_RSLT_SEXT_READ                (1<<15)
#define MCF544XX_ADC_LLMT_WRITE(x)                 (((x)&0x0FFF)<<3)
#define MCF544XX_ADC_HLMT_WRITE(x)                 (((x)&0x0FFF)<<3)
#define MCF544XX_ADC_OFS_OFFSET_WRITE(x)           (((x)&0x0FFF)<<3)
#define MCF544XX_ADC_PWR_PUDELAY_WRITE(x)          (((x)&0x3F)<<4)
#define MCF544XX_ADC_PWR_PUDELAY_READ(x)           (((x)>>4)&0x3F)
#define MCF544XX_ADC_PWR_PD(x)                     (1<<(x))
#define MCF544XX_ADC_PWR_APD                       (1<<14)
#define MCF544XX_ADC_PWR_PSTS(x)                   (1<<(10+(x)))
#define MCF544XX_ADC_PWR_ASB                       (1<<15)
#define MCF544XX_ADC_CAL_VREFH1                    (1<<15)
#define MCF544XX_ADC_CAL_VREFL1                    (1<<14)
#define MCF544XX_ADC_CAL_VREFH0                    (1<<13)
#define MCF544XX_ADC_CAL_VREFL0                    (1<<12)
#define MCF544XX_ADC_CAL_TEST1                     (1<<3)
#define MCF544XX_ADC_CAL_TEST0                     (1<<2)
#define MCF544XX_ADC_CAL_DAC1                      (1<<1)
#define MCF544XX_ADC_CAL_DAC0                      (1<<0)
#define MCF544XX_ADC_PWR2_STNBY                    (1<<4)
#define MCF544XX_ADC_PWR2_SPEEDB(x)                (((x)&0x03) << 2)
#define MCF544XX_ADC_PWR2_SPEEDA(x)                (((x)&0x03) << 0)
#define MCF544XX_ADC_DIV_DIV0(x)                   (((x)&0x7F) << 0)
#define MCF544XX_ADC_DIV_DIV1(x)                   (((x)&0x7F) << 8)
#define MCF544XX_ADC_ASDIV_ASDIV(x)                (((x)&0x1FF) << 0)

/*
** MCF544XX_ADC_STRUCT                                                                   
** Programmable interrupt timer
*/
typedef struct MCF544XX_adc_struct
{
   uint16_t  ADC_CR1;        // Control Register 1
   uint16_t  ADC_CR2;        // Control Register 2
   uint16_t  ADC_ZCCR;       // Zero Crossing Control Register
   uint16_t  ADC_LST1;       // Channel List Register 1
   uint16_t  ADC_LST2;       // Channel List Register 2
   uint16_t  ADC_SDIS;       // Sample Disable Register
   uint16_t  ADC_SR;         // Status Register
   uint16_t  ADC_LSR;        // Limit Status Register
   uint16_t  ADC_ZCSR;       // Zero Crossing Status Register
   uint16_t  ADC_RSLT[8];    // Result Registers 0-7
   uint16_t  ADC_LLMT[8];    // Low Limit Registers 0-7
   uint16_t  ADC_HLMT[8];    // High Limit Registers 0-7
   uint16_t  ADC_OFS[8];     // Offset Registers 0-7
   uint16_t  ADC_PWR;        // Power Control Register
   uint16_t  ADC_CAL;        // Callibration Register
   uint16_t  ADC_PWR2;       // Power Control Register 2
   uint16_t  ADC_DIV;        // Conversion Divisor Register
   uint16_t  ADC_ASDIV;      // Auto-standby Divisor Register
} MCF544XX_ADC_STRUCT, * MCF544XX_ADC_STRUCT_PTR;
typedef volatile struct MCF544XX_adc_struct * VMCF544XX_ADC_STRUCT_PTR;

#ifdef __cplusplus
}
#endif

#endif  //__mcf544xx_adc_h__
