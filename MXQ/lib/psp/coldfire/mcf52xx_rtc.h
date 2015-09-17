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
*   This file contains the type definitions for the mcf52xx RTC module.
*
*
*END************************************************************************/

#ifndef __mcf52xx_rtc_h__
#define __mcf52xx_rtc_h__

#define __mcf52xx_rtc_h__version "$Version:3.6.7.0$"
#define __mcf52xx_rtc_h__date    "$Date:Jun-4-2010$"

#ifdef __cplusplus
extern "C" {
#endif


#define MCF52XX_RTC_HOURMIN_HOURS_WRITE(x)          (((x)&0x1F)<<0x08)
#define MCF52XX_RTC_HOURMIN_MINUTES_WRITE(x)        ((x)&0x3F)
#define MCF52XX_RTC_HOURMIN_HOURS_READ(x)           (((x)>>0x08)&0x1F)
#define MCF52XX_RTC_HOURMIN_MINUTES_READ(x)         ((x)&0x3F)
#define MCF52XX_RTC_SECONDS_READ(x)                 ((x)&0x3F)
#define MCF52XX_RTC_SECONDS_WRITE(x)                ((x)&0x3F)
#define MCF52XX_RTC_ALRM_HM_HOURS_WRITE(x)          (((x)&0x1F)<<0x08)
#define MCF52XX_RTC_ALRM_HM_MINUTES_WRITE(x)        ((x)&0x3F)
#define MCF52XX_RTC_ALRM_HM_HOURS_READ(x)           (((x)>>0x08)&0x1F)
#define MCF52XX_RTC_ALRM_HM_MINUTES_READ(x)         ((x)&0x3F)
#define MCF52XX_RTC_ALRM_SEC_READ(x)                ((x)&0x3F)
#define MCF52XX_RTC_ALRM_SEC_WRITE(x)               ((x)&0x3F)
#define MCF52XX_RTC_RTCCTL_EN                       (1<<7)
#define MCF52XX_RTC_RTCCTL_SWR                      (1<<0)
#define MCF52XX_RTC_RTCISR_SW                       (1<<0)
#define MCF52XX_RTC_RTCISR_MIN                      (1<<1)
#define MCF52XX_RTC_RTCISR_ALM                      (1<<2)
#define MCF52XX_RTC_RTCISR_DAY                      (1<<3)
#define MCF52XX_RTC_RTCISR_1HZ                      (1<<4)
#define MCF52XX_RTC_RTCISR_HR                       (1<<5)
#define MCF52XX_RTC_RTCISR_2HZ                      (1<<7)
#define MCF52XX_RTC_RTCISR_SAM0                     (1<<8)
#define MCF52XX_RTC_RTCISR_SAM1                     (1<<9)
#define MCF52XX_RTC_RTCISR_SAM2                     (1<<10)
#define MCF52XX_RTC_RTCISR_SAM3                     (1<<11)
#define MCF52XX_RTC_RTCISR_SAM4                     (1<<12)
#define MCF52XX_RTC_RTCISR_SAM5                     (1<<13)
#define MCF52XX_RTC_RTCISR_SAM6                     (1<<14)
#define MCF52XX_RTC_RTCISR_SAM7                     (1<<15)
#define MCF52XX_RTC_RTCISR_EN                       (1<<31)
#define MCF52XX_RTC_RTCIENR_SW                      (1<<0)
#define MCF52XX_RTC_RTCIENR_MIN                     (1<<1)
#define MCF52XX_RTC_RTCIENR_ALM                     (1<<2)
#define MCF52XX_RTC_RTCIENR_DAY                     (1<<3)
#define MCF52XX_RTC_RTCIENR_1HZ                     (1<<4)
#define MCF52XX_RTC_RTCIENR_HR                      (1<<5)
#define MCF52XX_RTC_RTCIENR_2HZ                     (1<<7)
#define MCF52XX_RTC_RTCIENR_SAM0                    (1<<8)
#define MCF52XX_RTC_RTCIENR_SAM1                    (1<<9)
#define MCF52XX_RTC_RTCIENR_SAM2                    (1<<10)
#define MCF52XX_RTC_RTCIENR_SAM3                    (1<<11)
#define MCF52XX_RTC_RTCIENR_SAM4                    (1<<12)
#define MCF52XX_RTC_RTCIENR_SAM5                    (1<<13)
#define MCF52XX_RTC_RTCIENR_SAM6                    (1<<14)
#define MCF52XX_RTC_RTCIENR_SAM7                    (1<<15)
#define MCF52XX_RTC_STPWCH_CNT(x)                   ((x)&0x1F)
#define MCF52XX_RTC_DAYS_DAYS(x)                    ((x)&0xFFFF)
#define MCF52XX_RTC_ALRM_DAY_DAYS(x)                ((x)&0xFFFF)


/*
** MCF52XX_RTC_STRUCT
** Real Time Clock                                                                   
*/
typedef struct mcf52xx_rtc_struct
{
   uint32_t  HOURMIN;        // RTC Hours and Minutes Counter Register
   uint32_t  SECONDS;        // RTC Seconds Counter Register
   uint32_t  ALRM_HM;        // RTC Hours and Minutes Alarm Register
   uint32_t  ALRM_SEC;       // RTC Seconds Alarm Register
   uint32_t  RTCCTL;         // RTC Control Register
   uint32_t  RTCISR;         // RTC Interrupt Status Register
   uint32_t  RTCIENR;        // RTC Interrupt Enable Register
   uint32_t  STPWCH;         // Stopwatch Minutes Register
   uint16_t  filler0;        // ONLY 16 bit ACCESS IS ALLOWED !
   uint16_t  DAYS;           // RTC Days Counter Register
   uint16_t  filler1;        // ONLY 16 bit ACCESS IS ALLOWED !
   uint16_t  ALRM_DAY;       // RTC Days Alarm Register
   unsigned char    filler2[0xC];
   uint32_t  RTCGOCU;        // RTC General Oscillator Count Upper Register
   uint32_t  RTCGOCL;        // RTC General Oscillator Count Lower Register
   unsigned char    filler3[0x4];
   
} MCF52XX_RTC_STRUCT, * MCF52XX_RTC_STRUCT_PTR;
typedef volatile struct mcf52xx_rtc_struct * VMCF52XX_RTC_STRUCT_PTR;


#ifdef __cplusplus
}
#endif

#endif
