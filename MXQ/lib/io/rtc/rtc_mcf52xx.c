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
*
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <rtc.h>
#include <mcf52xx_rtc.h>

static void _rtc_update_alarm_reg(uint32_t time);

static uint32_t   alarm_time    = 0;
static uint32_t   alarm_period  = 0;

static volatile RTC_HANDLER rtc_handler = {NULL, NULL};

/*FUNCTION****************************************************************
*
* Function Name    : _rtc_init
* Returned Value   : MQX error code
* Comments         :
*    This function (re)initializes/clears/enables RTC module.
*
*END*********************************************************************/
int32_t _rtc_init
(
    /* [IN] param identifying init operations */
    void *param
)
{
    VMCF52XX_RTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address ();
    /* Enable RTC counter */
    rtc->RTCCTL |= MCF52XX_RTC_RTCCTL_EN;
    return MQX_OK;
}

/*FUNCTION****************************************************************
*
* Function Name    : _rtc_callback_reg
* Returned Value   : MQX error code
* Comments         :
*    This function installs given user callback for RTC module.
*
*END*********************************************************************/
int32_t  _rtc_callback_reg
(
        /* [IN] pointer to user callback code */
        INT_ISR_FPTR func,
        /* [IN] pointer to call back data */
        void* data
)
{ /* Body */
    VMCF52XX_RTC_STRUCT_PTR rtc    = _bsp_get_rtc_base_address ();
    int32_t                 result = MQX_OK;

    if  (func == NULL)
    {
        /* Disable alarm interrupt */
        rtc->RTCIENR &= ~MCF52XX_RTC_RTCIENR_ALM;

        /* Install default ISR for RTC interrupt */
        if (NULL == _int_install_isr (MCF5225_INT_RTC, _int_get_default_isr(), NULL))
        {
            result = _task_get_error ();
        }

        _bsp_int_init(MCF5225_INT_RTC, BSP_RTC_INT_LEVEL, 0, FALSE);
        rtc_handler.isr_func = NULL;
    }
    else
    {
        /* Install call back function & data */
        rtc_handler.isr_func = NULL;
        rtc_handler.isr_dat  = data;
        rtc_handler.isr_func = func;

        /* Install rtc isr */
        if (NULL == _int_install_isr (MCF5225_INT_RTC, _rtc_isr, (void*)rtc))
        {
            result = _task_get_error ();
        } /* Endif */

        if (MQX_OK == result)
        {
            /* Set INT_LEVEL for RTC interrupt */
            _bsp_int_init(MCF5225_INT_RTC, BSP_RTC_INT_LEVEL, 0, TRUE);

            /* Enable alarm interrupt */
            rtc->RTCIENR |= MCF52XX_RTC_RTCIENR_ALM;
        } /* Endif */
    } /* Endelse */

    return result;
} /* Endbody */

/*FUNCTION****************************************************************
*
* Function Name    : _rtc_isr
* Returned Value   : none
* Comments         :
*    This is template ISR for RTC module.
*
*END*********************************************************************/
void _rtc_isr
(
    /* [IN] data pointer passed to interrupt */
    void   *ptr
)
{ /* Body */
    VMCF52XX_RTC_STRUCT_PTR rtc = (VMCF52XX_RTC_STRUCT_PTR)ptr;
    uint32_t isr_status = rtc->RTCISR; /* Store isr status */

    if (isr_status & MCF52XX_RTC_RTCISR_ALM) /* Alarm occur */
    {
        if (alarm_period != 0) {
            /* Update next alarm time */
            alarm_time += alarm_period;
            _rtc_update_alarm_reg(alarm_time);
        } else {
            /* Clear alarm flag */
            alarm_time = 0;
        } /* End else */

        /* Call alarm callback */
        if ((INT_ISR_FPTR)NULL != rtc_handler.isr_func) {
            rtc_handler.isr_func(rtc_handler.isr_dat);
        } /* End if */
    } /* End if */

    /* Clear interrupt status */
    rtc->RTCISR |= isr_status;
} /* End body */

/*FUNCTION****************************************************************
*
* Function Name    : _rtc_set_time
* Returned Value   : MQX_OK
* Comments         :
*    This function sets the RTC time according to given time struct.
*
*END*********************************************************************/
int32_t _rtc_set_time
(
    /* [IN] given time to be set as RTC time */
    uint32_t  time
)
{ /* Body */
    VMCF52XX_RTC_STRUCT_PTR rtc    = _bsp_get_rtc_base_address ();
    uint32_t                  second = time;
    uint32_t                day, hour, mintue;

    /* Calculate current time */
    day     = second / 86400;
    second -= day    * 86400;
    hour    = second /  3600;
    second -= hour   *  3600;
    mintue  = second /    60;
    second -= mintue *    60;

    /* Disable RTC counter */
    rtc->RTCCTL = 0x00;

    rtc->DAYS   = (uint16_t)day;
    /* Update HOUR & MINUTE register */
    rtc->HOURMIN = MCF52XX_RTC_HOURMIN_HOURS_WRITE(hour) | MCF52XX_RTC_HOURMIN_MINUTES_WRITE(mintue);
    /* Update SECOND register */
    rtc->SECONDS = second;

    /* Enable RTC counter */
    rtc->RTCCTL = MCF52XX_RTC_RTCCTL_EN;

    return MQX_OK;
} /* End body */

/*FUNCTION****************************************************************
*
* Function Name    : _rtc_get_time
* Returned Value   : MQX_OK
* Comments         :
*    This function gets the RTC time and stores it in given time struct.
*
*END*********************************************************************/
int32_t _rtc_get_time
(
    /* [OUT] this parameter gets actual RTC time */
    uint32_t *time
)
{ /* Body */
    VMCF52XX_RTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address ();
    uint32_t tmp = 0;

    do {
        *time = tmp;

        /* Get current time */
        tmp  = rtc->SECONDS;
        tmp += (uint32_t)(MCF52XX_RTC_HOURMIN_MINUTES_READ(rtc->HOURMIN) * 60);
        tmp += (uint32_t)(MCF52XX_RTC_HOURMIN_HOURS_READ(rtc->HOURMIN) * 3600);
        tmp += (uint32_t)(MCF52XX_RTC_DAYS_DAYS(rtc->DAYS) * 86400);

    } while (tmp != *time); /* Ensure that none of registers is updated while getting current time */

    return MQX_OK;
} /* End body */

/*FUNCTION****************************************************************
*
* Function Name    : _rtc_update_alarm_reg
* Returned Value   : none
* Comments         :
*    This function update alarm register for RTC
*
*END*********************************************************************/

static void _rtc_update_alarm_reg
(
    uint32_t time
)
{ /* Body */
    VMCF52XX_RTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address ();
    uint32_t day, hour, mintue, second;

    second = time;

    /* Calculate alarm time */
    day     = second / 86400;
    second -= day    * 86400;
    hour    = second /  3600;
    second -= hour   *  3600;
    mintue  = second /    60;
    second -= mintue *    60;

    /* Disable RTC counter */
    rtc->RTCCTL = 0x00;

    /* Update alarm day reg */
    rtc->ALRM_DAY = (uint16_t)day;
    /* Update alarm hour & minute reg */
    rtc->ALRM_HM = MCF52XX_RTC_ALRM_HM_HOURS_WRITE(hour) | MCF52XX_RTC_ALRM_HM_MINUTES_WRITE(mintue);
    /* Update second reg */
    rtc->ALRM_SEC = second;

    /* Enable RTC counter */
    rtc->RTCCTL = MCF52XX_RTC_RTCCTL_EN;
} /* Endbody */

/*FUNCTION****************************************************************
*
* Function Name    : _rtc_set_alarm
* Returned Value   : MQX_OK
* Comments         :
*    This function sets RTC alarm time according to given time struct.
*    Alarm happens a half second after match.
*
*END*********************************************************************/
int32_t _rtc_set_alarm
(
    /* [IN] time to be set as RTC next alarm time */
    uint32_t time,
    /* [IN] time to be set as RTC alarm period time */
    uint32_t period
)
{
    uint32_t current_time = 0;

    _rtc_get_time(&current_time);
    /* Alarm time must be larger than current time */
    if (time < current_time)
    {
        return RTC_INVALID_TIME;
    }
    alarm_time     = time;
    alarm_period   = period;
    _rtc_update_alarm_reg(time);

    return MQX_OK;
}

/*FUNCTION****************************************************************
*
* Function Name    : _rtc_get_alarm
* Returned Value   : MQX_OK
* Comments         :
*    This function gets the RTC alarm time and stores it in given time struct.
*
*END*********************************************************************/
int32_t _rtc_get_alarm
(
    /* [OUT] this parameter gets the RTC alarm time */
    uint32_t *time
)
{

    *time = alarm_time;
    return MQX_OK;

}

/* EOF */
