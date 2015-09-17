/* ###################################################################
**     Filename    : mqx_tasks.c
**     Project     : lwtimer_frdmkl46
**     Processor   : MKL46Z256VMC4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : Keil ARM C/C++ Compiler
**     Date/Time   : 2014-11-03, 16:49, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         Task1_task - void Task1_task(uint32_t task_init_data);
**
** ###################################################################*/
/*!
** @file mqx_tasks.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup mqx_tasks_module mqx_tasks module documentation
**  @{
*/         
/* MODULE mqx_tasks */

#include "Cpu.h"
#include "Events.h"
#include "mqx_tasks.h"
#include "lwtimer.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */
/*FUNCTION*------------------------------------------------------
*
* Function Name  : LED_on
* Returned Value : none
* Comments       :
*     This timer function prints out "ON"
*END*-----------------------------------------------------------*/

static void LED_on
   (
      void   *data_ptr
   )
{
   printf("ON\n");
}

/*FUNCTION*------------------------------------------------------
*
* Function Name  : LED_off
* Returned Value : none
* Comments       :
*     This timer function prints out "OFF"
*END*-----------------------------------------------------------*/

static void LED_off
   (
      void   *data_ptr
   )
{
   printf("OFF\n");
}
/*
** ===================================================================
**     Event       :  Task1_task (module mqx_tasks)
**
**     Component   :  Task1 [MQXLite_task]
**     Description :
**         MQX task routine. The routine is generated into mqx_tasks.c
**         file.
**     Parameters  :
**         NAME            - DESCRIPTION
**         task_init_data  - 
**     Returns     : Nothing
** ===================================================================
*/
void Task1_task(uint32_t task_init_data)
{
	 LWTIMER_PERIOD_STRUCT  period;
	 LWTIMER_STRUCT         on_timer;
	 LWTIMER_STRUCT         off_timer;
   uint8_t          	    time = 6; // time in seconds 

   printf("\n\nTwo lwtimers are created, each of a period of 2 seconds,\nthe second timer offset by 1 second from the first.\n");
   printf("Task runs for %d seconds,\nthen timers are closed and task finishes.\n\n", time);           

   /* 
   ** Create the timer component with more stack than the default
   ** in order to handle printf() requirements: 
   */
	 _lwtimer_create_periodic_queue(&period, 400, 0);
		
	 _lwtimer_add_timer_to_queue(&period, &on_timer, 0, LED_on, NULL);
	 _lwtimer_add_timer_to_queue(&period, &off_timer, 200, LED_off, NULL);

   _time_delay_ticks(time * 200); // wait 6 seconds
   printf("\nThe task is finished!");

   _lwtimer_cancel_timer(&on_timer);
   _lwtimer_cancel_timer(&off_timer);
	 _lwtimer_cancel_period(&period);

   _task_block();
}

/* END mqx_tasks */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.4 [05.09]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
