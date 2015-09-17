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
*   This file contains the SDRAM controler type definitions for the 
*   mcf54xx processor. It has been split out into a separate header 
*   file to share with other Coldfire PSP's.
*
*
*END************************************************************************/

#ifndef __mcf5xxx_irtc_h__
    #define __mcf5xxx_irtc_h__

#define __mcf5xxx_irtc_h__version "$Version:3.6.2.0$"
#define __mcf5xxx_irtc_h__date    "$Date:Apr-15-2010$"


#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************
 * interrupt masks definitions (RTC_ISR and RTC_IER registers)                *
 ******************************************************************************/  

#define MCF5XXX_RTC_CTRL_SWR_MASK       (0x0001 << 8)
#define MCF5XXX_RTC_INT_STW_MASK        (0x0001 << 1) 
#define MCF5XXX_RTC_INT_ALM_MASK        (0x0001 << 2)
#define MCF5XXX_RTC_INT_DAY_MASK        (0x0001 << 3)
#define MCF5XXX_RTC_INT_HR_MASK         (0x0001 << 4)
#define MCF5XXX_RTC_INT_MIN_MASK        (0x0001 << 5)
#define MCF5XXX_RTC_INT_1HZ_MASK        (0x0001 << 6)
#define MCF5XXX_RTC_INT_2HZ_MASK        (0x0001 << 7)
#define MCF5XXX_RTC_INT_SAM0_MASK       (0x0001 << 8)
#define MCF5XXX_RTC_INT_SAM1_MASK       (0x0001 << 9)
#define MCF5XXX_RTC_INT_SAM2_MASK       (0x0001 <<10) 
#define MCF5XXX_RTC_INT_SAM3_MASK       (0x0001 <<11) 
#define MCF5XXX_RTC_INT_SAM4_MASK       (0x0001 <<12)  
#define MCF5XXX_RTC_INT_SAM5_MASK       (0x0001 <<13) 
#define MCF5XXX_RTC_INT_SAM6_MASK       (0x0001 <<14) 
#define MCF5XXX_RTC_INT_SAM7_MASK       (0x0001 <<15) 
#define MCF5XXX_RTC_INT_ALL_MASK        (0xFFFF) 


typedef struct MCF5XXX_irtc_struct
{
    int8_t   RTC_YEAR;               /* 0x00 Year Counter Register                   */
    uint8_t  RTC_MONTH;              /* 0x01 Month Counter Register                  */
    uint8_t  RTC_DAY_OF_WEEK;        /* 0x02 Day-of-Week Counter Register            */
    uint8_t  RTC_DAYS;               /* 0x03 Day Counter Register                    */
    uint8_t  RTC_HOURS;              /* 0x04 Hour Counter Register                   */
    uint8_t  RTC_MINUTES;            /* 0x05 Minute Counter Register                 */
    uint8_t  reserved01;             /* 0x06                                         */
    uint8_t  RTC_SECONDS;            /* 0x07 Seconds Counter Register reserved       */
    int8_t   RTC_ALM_YEAR;           /* 0x08 Year Alarm Register                     */
    uint8_t  RTC_ALM_MONTH;          /* 0x09 Month Alarm Register                    */
    uint8_t  reserved02;             /* 0x0A                                         */
    uint8_t  RTC_ALM_DAYS;           /* 0x0B Days Alarm Register                     */
    uint8_t  RTC_ALM_HOURS;          /* 0x0C HOURS Alarm Register                    */
    uint8_t  RTC_ALM_MINUTES;        /* 0x0D Minutes Alarm Register                  */
    uint16_t RTC_ALM_SEC;            /* 0x0E Seconds Alarm Register                  */
    uint16_t RTC_CTRL;               /* 0x10 Control Register                        */
    uint8_t  reserved03;             /* 0x12                                         */
    uint8_t  RTC_STATUS;             /* 0x13 Status Register                         */
    uint16_t RTC_ISR;                /* 0x14 Interrupt Status Register               */
    uint16_t RTC_IER;                /* 0x16 Interrupt Enable Register               */
    uint16_t RTC_COUNTDOWN_COUNT;    /* 0x18 Countdown (Minutes) Timer Register      */
    uint8_t  reserved05[0x05];       
    uint16_t RTC_CFG_DATA;           /* 0x20 Configuration Data Register             */
    uint8_t  RTC_DST_START_HOUR;     /* 0x22 Daylight Saving Hour Register           */
    uint8_t  RTC_DST_END_HOUR;       /* 0x23 Daylight Saving Hour Register           */
    uint8_t  RTC_DST_START_MONTH;    /* 0x24 Daylight Saving Month Register          */
    uint8_t  RTC_DST_END_MONTH;      /* 0x25 Daylight Saving Month Register          */
    uint8_t  RTC_DST_START_DAY;      /* 0x26 Daylight Saving Day Register            */
    uint8_t  RTC_DST_END_DAY;        /* 0x27 Daylight Saving Day Register            */
    uint8_t  RTC_COMPEN_INTERVAL;    /* 0x28 Compensation Interval Register          */
    uint8_t  RTC_COMPEN_VALUE;       /* 0x29 Compensation Interval Register          */
    uint8_t  reserved06[8];          /* 0x2C                                         */
    uint16_t RTC_UP_CNTR_H;          /* 0x32 Counter Register High                   */
    uint16_t RTC_UP_CNTR_L;          /* 0x34 Counter Register Low                    */
    uint8_t  reserved07[10];
    uint8_t  RTC_STAND_BY_RAM[1985]; /* 0x40 Stand-by RAM                            */           
} MCF5XXX_IRTC_STRUCT, * MCF5XXX_IRTC_STRUCT_PTR;

typedef volatile struct MCF5XXX_irtc_struct * VMCF5XXX_IRTC_STRUCT_PTR;           


#ifdef __cplusplus
}
#endif

#endif /* __mcf5xxx_irtc_h__ */
