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
*   Coldfire family specific file needed for RTC module.
*
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>

/* local function prototypes */
static void _rtc_memcpy(void * dst, const void * src, uint32_t num);

/* Static variable declaration */
static uint32_t     alarm_time    = 0;
static uint32_t     alarm_period  = 0;

static volatile RTC_HANDLER   rtc_handler = {NULL, NULL};

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_lock
* Returned Value   : none
* Comments         :
*    This function locks RTC registers.
*
*END*********************************************************************/

void _rtc_lock
(
    void
)
{ /* Body */
    VMCF5XXX_IRTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address();

    rtc->RTC_CTRL &= ~RTC_CTRL_WE_MASK;     /* 0b00 */
    rtc->RTC_CTRL |=  RTC_CTRL_WE1_MASK;    /* 0b10 */
} /* Endbody */

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_unlock
* Returned Value   : MQX_OK if sucessfull
* Comments         :
*    This function unlocks RTC registers.
*
*END*********************************************************************/
int32_t _rtc_unlock
(
    void
)
{ /* Body */
   VMCF5XXX_IRTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address();
   uint16_t                 rtc_ctrl = rtc->RTC_CTRL;
   uint16_t                 cnt, i;

   for (cnt = 0; cnt < INTERNAL_RTC_UNLOCK_TEST; cnt ++) /* try to unlock in INTERNAL_RTC_UNLOCK_TEST times */
   {
        rtc_ctrl &= ~( RTC_CTRL_WE0_MASK | RTC_CTRL_WE1_MASK );
        rtc->RTC_CTRL = rtc_ctrl;        /* 0b00 */
        rtc_ctrl |= RTC_CTRL_WE0_MASK;
        rtc->RTC_CTRL = rtc_ctrl;        /* 0b01 */
        rtc_ctrl |= RTC_CTRL_WE1_MASK;
        rtc->RTC_CTRL = rtc_ctrl;        /* 0b11 */
        rtc_ctrl &= ~RTC_CTRL_WE0_MASK;
        rtc->RTC_CTRL = rtc_ctrl;        /* 0b10 */

        /* test, if unlock was successfull, there is some delay
        ** in setting the WPE bit, we repeat the test a few times */
        for(i = 0; i < INTERNAL_WPE_CNTR_TEST; i++)
        {
            if( !(rtc->RTC_STATUS & RTC_STATUS_WPE_MASK) )
            {
                return MQX_OK;
            } /* End if */
        } /* End for */

   } /* End for */

   return MQX_RTC_UNLOCK_FAILED;
} /* End body */

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
    void* param
)
{ /* Body */
    return MQX_OK;
} /* Endbody */

/*FUNCTION****************************************************************
*
* Function Name    : _rtc_callback_reg
* Returned Value   : MQX error code
* Comments         :
*    This function installs given user callback for RTC module.
*
*END*********************************************************************/
int32_t _rtc_callback_reg
(
    /* [IN] pointer to user ISR code */
    INT_ISR_FPTR func,
    /* [IN] pointer to callback data */
    void* data
)
{ /* Body */
    int32_t                  result;
    VMCF5XXX_IRTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address();

    result = _rtc_unlock();
    if  (func == NULL) /* Unregister alarm callback */
    {
        /* Disable alarm interrupt*/
        rtc->RTC_IER &= (~MCF5XXX_RTC_INT_ALM_MASK);
        /* Install default ISR for RTC interrupt */
        if (NULL == _int_install_isr (_bsp_get_rtc_vector(), _int_get_default_isr(), NULL))
        {
            result = _task_get_error();
        } /* End if */

        _mcf5441_int_init(BSP_RTC_INT_VECTOR, BSP_RTC_INT_LEVEL, FALSE);
        rtc_handler.isr_func = NULL;
    }
    else
    {
        /* Register callback function */
        rtc_handler.isr_func = NULL;
        rtc_handler.isr_dat  = data;
        rtc_handler.isr_func = func;

        /* RTC module is enabled after reset and its not possible to disable it */
        if (NULL == _int_install_isr (_bsp_get_rtc_vector (), _rtc_isr, (void *)rtc))
        {
            result = _task_get_error();
        } /* End if */

        if (MQX_OK == result)
        {
            /* Set INT_LEVEL for RTC interrupt */
            _mcf5441_int_init(BSP_RTC_INT_VECTOR, BSP_RTC_INT_LEVEL, TRUE);

            /* Enable Alarm interrupt */
            rtc->RTC_IER |= MCF5XXX_RTC_INT_ALM_MASK;

            /* Alarm occurs when seconds, minutes, hours, days, months, years are matched */
            rtc->RTC_CTRL = RTC_CR_AM_MASK;
           
        } /* Endif */
    } /* Endelse*/

    _rtc_lock();
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
    VMCF5XXX_IRTC_STRUCT_PTR rtc = (VMCF5XXX_IRTC_STRUCT_PTR)ptr;
    uint16_t mask;

    mask = (uint16_t)rtc->RTC_ISR;

    if (mask & MCF5XXX_RTC_INT_ALM_MASK)
    {
        if (alarm_period != 0)
        {
            alarm_time += alarm_period;
            /* Update next alarm time*/
            _rtc_update_alarm_reg(alarm_time);
        }
        else
        {
            /* Clear interrupt alarm status */
            alarm_time = 0;
        } /* End else */

        /* Call alarm callback */
        if ((INT_ISR_FPTR)NULL != rtc_handler.isr_func)
        {
            rtc_handler.isr_func (rtc_handler.isr_dat);
        } /* End if */
    } /* End if */

    _rtc_unlock();
    /* clear interrupt flag */
    rtc->RTC_ISR |= mask;
    _rtc_lock();

} /* End body */

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_set_time
* Returned Value   : none
* Comments         :
*    This function sets the RTC time according to given time struct.
*
*END*********************************************************************/

int32_t _rtc_set_time
(
    /* [IN] given time to be set as RTC time */
    uint32_t time
)
{ /* Body */
    VMCF5XXX_IRTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address();
    DATE_STRUCT              date;
    TIME_STRUCT              ts;

    ts.SECONDS = time;
    ts.MILLISECONDS = 0;

    if (_time_to_date(&ts, &date) == FALSE) 
    {
        return MQX_ERROR;
    } /* End if */

    if (date.YEAR < IRTC_MINIMAL_YEAR)
    {
        /* Normalize the year of date */
        date.YEAR = IRTC_MINIMAL_YEAR;
    } /* End if */

    /* Unlock rtc register for writing */
    if (MQX_OK != _rtc_unlock())
    {
        return MQX_ERROR;
    } /* End if */

    /* Wait until time is coherent */
    while (rtc->RTC_STATUS & RTC_STATUS_INVAL_MASK){};

    rtc->RTC_YEAR        = (int8_t)((int16_t)date.YEAR - RTC_BASE_YEAR);
    rtc->RTC_MONTH       = (uint8_t)date.MONTH;
    rtc->RTC_DAYS        = (uint8_t)date.DAY;
    rtc->RTC_DAY_OF_WEEK = (uint8_t)date.WDAY;
    rtc->RTC_HOURS       = (uint8_t)date.HOUR;
    rtc->RTC_MINUTES     = (uint8_t)date.MINUTE;
    rtc->RTC_SECONDS     = (uint8_t)date.SECOND;

    _rtc_lock();
    return MQX_OK;
} /* End body */

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_get_time
* Returned Value   : MQX error code
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
    VMCF5XXX_IRTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address ();
    DATE_STRUCT date;
    TIME_STRUCT ts;

    /* Wait until time is coherent */
    while (rtc->RTC_STATUS & RTC_STATUS_INVAL_MASK){};

    date.YEAR     = (uint16_t)(rtc->RTC_YEAR + RTC_BASE_YEAR);
    date.MONTH    = (uint16_t)rtc->RTC_MONTH;
    date.DAY      = (uint16_t)rtc->RTC_DAYS;
    date.WDAY     = (uint16_t)rtc->RTC_DAY_OF_WEEK;
    date.HOUR     = (uint16_t)rtc->RTC_HOURS;
    date.MINUTE   = (uint16_t)rtc->RTC_MINUTES;
    date.SECOND   = (uint16_t)rtc->RTC_SECONDS;
    date.MILLISEC = 0;

    if (_time_from_date(&date, &ts) == FALSE)
    {
        return MQX_ERROR;
    } /* End if */

    *time = ts.SECONDS;
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

void _rtc_update_alarm_reg
(
    uint32_t time
)
{ /* Body */

    VMCF5XXX_IRTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address ();
    DATE_STRUCT              date;
    TIME_STRUCT              ts;

    _rtc_unlock();

    ts.SECONDS = time;
    ts.MILLISECONDS = 0;
    _time_to_date(&ts, &date);

    /* Wait until time is coherent */
    while (rtc->RTC_STATUS & RTC_STATUS_INVAL_MASK){};

    rtc->RTC_ALM_YEAR    = (int8_t)((uint16_t)date.YEAR - RTC_BASE_YEAR);
    rtc->RTC_ALM_MONTH   = (uint8_t)date.MONTH;
    rtc->RTC_ALM_DAYS    = (uint16_t)date.DAY; 
    rtc->RTC_ALM_HOURS   = (uint16_t)date.HOUR;
    rtc->RTC_ALM_MINUTES = (uint16_t)date.MINUTE;
    rtc->RTC_ALM_SEC     = (uint16_t)date.SECOND;

    _rtc_lock();

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
{ /* Body */
    uint32_t current_time ;

    /* Verify that time isn't smaller than current time */
    _rtc_get_time(&current_time);
    if (time < current_time)
    {
        return RTC_INVALID_TIME;
    } /* End if */

    /* Update alarm registers */
    alarm_time   = time;
    alarm_period = period;
    _rtc_update_alarm_reg(time);

    return MQX_OK;
}

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_get_alarm
* Returned Value   : none
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

} /* Body */

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_inc_upcounter
* Returned Value   : none
* Comments         :
*    This function increments up-counter register by 1.
*
*END*********************************************************************/

void _rtc_inc_upcounter
(
    void
) 
{ /* Body */
    VMCF5XXX_IRTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address();

    _rtc_unlock();
    rtc->RTC_UP_CNTR_L = 0;
    _rtc_lock();
} /* End body */

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_get_upcounter
* Returned Value   : value of up-counter register
* Comments         :
*    This function returns value of up-counter register.
*
*END*********************************************************************/

uint32_t _rtc_get_upcounter
(
    void
) 
{ /* Body */
    VMCF5XXX_IRTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address ();

    return ((uint32_t)rtc->RTC_UP_CNTR_L + ((uint32_t)rtc->RTC_UP_CNTR_H << 16));
} /* End body */

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_memcpy
* Returned Value   : void

* Comments         :
*   Local function for memory copy byte by byte
*
*END*********************************************************************/

static void _rtc_memcpy
(
    void * dst,
    const void * src,
    uint32_t num
)
{ /* Body */
    while (num--)
    {
        *((uint8_t *)dst)++ = *((uint8_t *)src)++;
    } /* Endwhile */
} /* End body */

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_write_to_standby_ram
* Returned Value   : MQX_OK           - operation successful
*                    MQX_INVALID_SIZE - write operation failed
* Comments         :
*   Function writes "size" bytes pointed by "src_ptr" 
*   into IRTC module stand-by RAM at address "dst_address"
*
*END*********************************************************************/

int32_t _rtc_write_to_standby_ram
(
    /* [IN]  Destination address in standby ram */
    uint32_t  dst_address, 
    /* [IN]  Source data pointer */
    uint8_t * src_ptr, 
    /* [IN]  Number of bytes to be written */
    uint32_t  size
)
{ /* Body */

    VMCF5XXX_IRTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address ();

    if (dst_address + size <= sizeof(rtc->RTC_STAND_BY_RAM))
    {
        _int_disable();
        _rtc_unlock();

        _rtc_memcpy (
            (void *)&(rtc->RTC_STAND_BY_RAM[dst_address]),
            src_ptr,
            size
        );

        _rtc_lock();
        _int_enable();

        return MQX_OK;
    } /* End if */

    return MQX_INVALID_SIZE;
} /* End body */

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_read_from_standby_ram
* Returned Value   : MQX_OK             - successful
*                    MQX_INVALID_SIZE   - read operation failed
* Comments         :
*   Function reads "size" bytes from "src_address" in stand-by RAM
*   into "dst_ptr" 
*
*END*********************************************************************/
int32_t _rtc_read_from_standby_ram
(
    /* [IN]  Source address in standby ram */
    uint32_t  src_address, 
    /* [OUT] Destination data pointer */
    uint8_t * dst_ptr, 
    /* [IN]  Number of bytes to be read */
    uint32_t  size
)
{ /* Body */
    VMCF5XXX_IRTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address ();

    if (src_address + size <= sizeof(rtc->RTC_STAND_BY_RAM))
    {
        _rtc_memcpy (
            dst_ptr,
            (void *)&(rtc->RTC_STAND_BY_RAM[src_address]),
            size
        );

        return MQX_OK;
    } /* End if */

    return MQX_INVALID_SIZE;
} /* End body */

/* EOF */
