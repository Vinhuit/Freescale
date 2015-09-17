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
*   Processor family specific file needed for RTC module.
*
*
*END************************************************************************/

#ifndef __irtc_mcf5xxx_h__
#define __irtc_mcf5xxx_h__

/******************************************************************************
 * RTC CONTROL MASK definitions 
 ******************************************************************************/
#define RTC_CTRL_WE0_MASK           (0x0001)
#define RTC_CTRL_WE1_MASK           (0x0002)
#define RTC_CTRL_WE_MASK            (0x0003)
#define RTC_CR_AM_MASK              (0x000C)
#define RTC_CTRL_SWR_MASK           (0x0100)

/******************************************************************************
 * RTC STATUS MASK definitions 
 ******************************************************************************/
#define RTC_STATUS_INVAL_MASK       (0x01)
#define RTC_STATUS_C_DON_MASK       (0x02)
#define RTC_STATUS_BERR_MASK        (0x04)
#define RTC_STATUS_WPE_MASK         (0x10)

/* RTC Base year */
#define RTC_BASE_YEAR               (2112)
#define IRTC_MINIMAL_YEAR           (1984)

#define INTERNAL_WPE_CNTR_TEST      (20)
#define INTERNAL_RTC_UNLOCK_TEST    (200)

/*----------------------------------------------------------------------*/
/*
**                    FUNCTION PROTOTYPES
*/

#ifdef __cplusplus
extern "C" {
#endif

void        _rtc_lock(void);
int32_t     _rtc_unlock(void);
void        _rtc_inc_upcounter(void);
uint32_t    _rtc_get_upcounter(void);
int32_t     _rtc_write_to_standby_ram(uint32_t dst_address, uint8_t * src_ptr, uint32_t  size);
int32_t     _rtc_read_from_standby_ram(uint32_t  src_address, uint8_t * dst_ptr, uint32_t  size);
void        _rtc_update_alarm_reg(uint32_t time);


#ifdef __cplusplus
}
#endif

#endif   /* __irtc_mcf5xxx_h__ */

/* EOF */
