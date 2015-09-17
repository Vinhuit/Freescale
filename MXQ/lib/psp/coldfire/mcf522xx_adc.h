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

#ifndef __mcf522xx_adc_h__
#define __mcf522xx_adc_h__

#define __mcf522xx_adc_h__version "$Version:3.0.1.0$"
#define __mcf522xx_adc_h__date    "$Date:Apr-2-2009$"

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------*/
/*
** ADC  registers bit set.
*/

#define MCF522XX_ADC_CTRL1_STOP0                     (1<<14)
#define MCF522XX_ADC_CTRL1_START0                    (1<<13)
#define MCF522XX_ADC_CTRL1_SYNC0                     (1<<12)
#define MCF522XX_ADC_CTRL1_EOSIE0                    (1<<11)
#define MCF522XX_ADC_CTRL1_ZCIE                      (1<<10)
#define MCF522XX_ADC_CTRL1_LLMTIE                    (1<<9)
#define MCF522XX_ADC_CTRL1_HLMTIE                    (1<<8)
#define MCF522XX_ADC_CTRL1_CHNCFG_READ(x)            (((x)>>0x04)&0x0F)
#define MCF522XX_ADC_CTRL1_CHNCFG_WRITE(x)           (((x)&0x0F)<<0x04)
#define MCF522XX_ADC_CTRL2_DIV(x)                    (((x)&0x1F))
#define MCF522XX_ADC_CTRL1_STOP1                     (1<<14)
#define MCF522XX_ADC_CTRL1_START1                    (1<<13)
#define MCF522XX_ADC_CTRL1_SYNC1                     (1<<12)
#define MCF522XX_ADC_CTRL1_EOSIE1                    (1<<11)
#define MCF522XX_ADC_CTRL1_SIMULT                    (1<<5)
#define MCF522XX_ADC_ADZCC_ZCE0_WRITE(x)             (((x)&0x03)<<0x00)
#define MCF522XX_ADC_ADZCC_ZCE1_WRITE(x)             (((x)&0x03)<<0x02)
#define MCF522XX_ADC_ADZCC_ZCE2_WRITE(x)             (((x)&0x03)<<0x04)
#define MCF522XX_ADC_ADZCC_ZCE3_WRITE(x)             (((x)&0x03)<<0x06)
#define MCF522XX_ADC_ADZCC_ZCE4_WRITE(x)             (((x)&0x03)<<0x08)
#define MCF522XX_ADC_ADZCC_ZCE5_WRITE(x)             (((x)&0x03)<<0x0A)
#define MCF522XX_ADC_ADZCC_ZCE6_WRITE(x)             (((x)&0x03)<<0x0C)
#define MCF522XX_ADC_ADZCC_ZCE7_WRITE(x)             (((x)&0x03)<<0x0E)
#define MCF522XX_ADC_ADZCC_ZCE0_READ(x)              (((x)>>0x00)&0x03)
#define MCF522XX_ADC_ADZCC_ZCE1_READ(x)              (((x)>>0x02)&0x03)
#define MCF522XX_ADC_ADZCC_ZCE2_READ(x)              (((x)>>0x04)&0x03)
#define MCF522XX_ADC_ADZCC_ZCE3_READ(x)              (((x)>>0x06)&0x03)
#define MCF522XX_ADC_ADZCC_ZCE4_READ(x)              (((x)>>0x08)&0x03)
#define MCF522XX_ADC_ADZCC_ZCE5_READ(x)              (((x)>>0x0A)&0x03)
#define MCF522XX_ADC_ADZCC_ZCE6_READ(x)              (((x)>>0x0C)&0x03)
#define MCF522XX_ADC_ADZCC_ZCE7_READ(x)              (((x)>>0x0E)&0x03)
#define MCF522XX_ADC_ADLST1_SAMPLE0_WRITE(x)         (((x)&0x07)<<0)
#define MCF522XX_ADC_ADLST1_SAMPLE1_WRITE(x)         (((x)&0x07)<<4)
#define MCF522XX_ADC_ADLST1_SAMPLE2_WRITE(x)         (((x)&0x07)<<8)
#define MCF522XX_ADC_ADLST1_SAMPLE3_WRITE(x)         (((x)&0x07)<<12)
#define MCF522XX_ADC_ADLST2_SAMPLE4_WRITE(x)         (((x)&0x07)<<0)
#define MCF522XX_ADC_ADLST2_SAMPLE5_WRITE(x)         (((x)&0x07)<<4)
#define MCF522XX_ADC_ADLST2_SAMPLE6_WRITE(x)         (((x)&0x07)<<8)
#define MCF522XX_ADC_ADLST2_SAMPLE7_WRITE(x)         (((x)&0x07)<<12)
#define MCF522XX_ADC_ADSDIS_DS(x)                    (1<<(x))
#define MCF522XX_ADC_ADSTAT_RDY(x)                   (1<<(x))
#define MCF522XX_ADC_ADSTAT_HLMTI                    (1<<8)
#define MCF522XX_ADC_ADSTAT_LLMTI                    (1<<9)
#define MCF522XX_ADC_ADSTAT_ZCI                      (1<<10)
#define MCF522XX_ADC_ADSTAT_EOSI0                    (1<<11)
#define MCF522XX_ADC_ADSTAT_EOSI1                    (1<<12)
#define MCF522XX_ADC_ADSTAT_CIP1                     (1<<14)
#define MCF522XX_ADC_ADSTAT_CIP0                     (1<<15)
#define MCF522XX_ADC_ADLSTAT_LLS(x)                  (1<<(x))
#define MCF522XX_ADC_ADLSTAT_HLS(x)                  (1<<(8 +(x)))
#define MCF522XX_ADC_ADZCSTAT_ZCS(x)                 (1<<(x))
#define MCF522XX_ADC_ADRSLT_RSLT_READ(x)             (((x)>>3)&0x0FFF)
#define MCF522XX_ADC_ADLLMT_LLMT_WRITE(x)            (((x)&0x0FFF)<<3)
#define MCF522XX_ADC_ADHLMT_HLMT_WRITE(x)            (((x)&0x0FFF)<<3)
#define MCF522XX_ADC_ADOFS_OFFSET_WRITE(x)           (((x)&0x0FFF)<<3)
#define MCF522XX_ADC_POWER_PUDELAY_WRITE(x)          (((x)&0x3F)<<4)
#define MCF522XX_ADC_POWER_PUDELAY_READ(x)           (((x)>>4)&0x3F)
#define MCF522XX_ADC_POWER_PD(x)                     (1<<(x))
#define MCF522XX_ADC_POWER_APD                       (1<<3)
#define MCF522XX_ADC_POWER_PSTS(x)                   (1<<(10+(x)))
#define MCF522XX_ADC_POWER_ASB                       (1<<15)
#define MCF522XX_ADC_CAL_SEL_VREFL                   (1<<14)
#define MCF522XX_ADC_CAL_SEL_VREFH                   (1<<15)

/*
** MCF522XX_ADC_STRUCT                                                                   
** Programmable interrupt timer
*/
typedef struct mcf522xx_adc_struct
{
   uint16_t  CTRL1;          // Control Register 1
   uint16_t  CTRL2;          // Control Register 2
   uint16_t  ADZCC;          // Zero Crossing Control Register
   uint16_t  ADLST1;         // Channel List Register 1
   uint16_t  ADLST2;         // Channel List Register 2
   uint16_t  ADSDIS;         // Sample Disable Register
   uint16_t  ADSTAT;         // Status Register
   uint16_t  ADLSTAT;        // Limit Status Register
   uint16_t  ADZCSTAT;       // Zero Crossing Status Register
   uint16_t  ADRSLT[8];      // Result Registers 0-7
   uint16_t  ADLLMT[8];      // Low Limit Registers 0-7
   uint16_t  ADHLMT[8];      // High Limit Registers 0-7
   uint16_t  ADOFS[8];       // Offset Registers 0-7
   uint16_t  POWER;          // Power Control Register
   uint16_t  CAL;            // Voltage Reference Register
   unsigned char    filler[0xFFA9];
} MCF522XX_ADC_STRUCT, * MCF522XX_ADC_STRUCT_PTR;
typedef volatile struct mcf522xx_adc_struct * VMCF522XX_ADC_STRUCT_PTR;

#ifdef __cplusplus
}
#endif

#endif  //__mcf52xx_i2c_h__
