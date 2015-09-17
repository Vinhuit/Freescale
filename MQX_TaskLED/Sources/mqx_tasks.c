/* ###################################################################
**     Filename    : mqx_tasks.c
**     Project     : gpio_frdmkl46
**     Processor   : MKL46Z256VLH4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : Keil ARM C/C++ Compiler
**     Date/Time   : 2014-11-03, 17:21, # CodeGen: 0
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
#include "mutex.h"
#include "Seg_LCD.h"
#include "lwtimer.h"
#ifdef __cplusplus

extern "C" {
#endif 



/*
** ===================================================================
**     Event       :  Timer_task (module mqx_tasks)
**
**     Component   :  Timer [MQXLite_task]
**     Description :
**         MQX task routine. The routine is generated into mqx_tasks.c
**         file.
**     Parameters  :
**         NAME            - DESCRIPTION
**         task_init_data  - 
**     Returns     : Nothing
** ===================================================================
*/
	/* Config  Led Red and Led Green */
unsigned int count = 0;
uint32_t          	    time =65535; // time in seconds 
void LEDs_Init(void) {

	LWSEM_STRUCT lwsem;
	const uint32_t led_mask[] = {1UL << 5,1UL << 29}; 	/*PTD5 (Led green),PTE29 (Led red)*/
	_lwsem_create(&lwsem, 0);

	
	SIM_SCGC5    |= (1UL << 13)|(1UL << 12);        		/* Enable Clock to Port E and Port D*/ 
  PORTE_PCR29 = (1UL <<  8);        								/* Pin is GPIO Value MUX = 0x001 */
	PORTD_PCR5 = (1UL << 8);
	
  GPIOD_PDOR |= led_mask[0];
	GPIOE_PDOR|=led_mask[1]; 													/* Data Output */


  GPIOD_PDDR |= led_mask[0];                						/* Enable Output Led red  */
	GPIOE_PDDR |= led_mask[1];														/* Enable Output Led Green   */                  
}
/*LED Toggle*/
void LED_Toggle(uint32_t led){
	const uint32_t led_mask[] = {1UL << 5,1UL << 29}; 	/*PTD5 (Led green),PTE29 (Led red)*/
		if(led)
			GPIOE_PTOR   = led_mask[led]; 						/*Set LOW Output */
		else
			GPIOD_PTOR = led_mask[led]; 
};
void Show_LCD(void * data_ptr)
{
	LEDs_Init();
	SegLCD_Init(); //init LCD
	SegLCD_DisplayDecimal(count++);
	if(count > 8888)
		count = 0;
	printf("number:%d\n",count);
	if(count%2)
	LED_Toggle(1);
	
}

/* Task  */
void Timer_task(uint32_t task_init_data)
{
	 LWTIMER_PERIOD_STRUCT  period;
	 LWTIMER_STRUCT         on_timer;
	 LWTIMER_STRUCT         off_timer;

   printf("\n\nTwo lwtimers are created, each of a period of 1 seconds show number couter to LCD");
   printf("Task runs for %d seconds,\nthen timers are closed and task finishes.\n\n", time);           

   /* 
   ** Create the timer component with more stack than the default
   ** in order to handle printf() requirements: 
   */
	 _lwtimer_create_periodic_queue(&period, 400, 0);
		_lwtimer_add_timer_to_queue(&period, &on_timer, 0, Show_LCD, NULL);
	  _lwtimer_add_timer_to_queue(&period, &off_timer, 200, Show_LCD, NULL);
	
   _time_delay_ticks(time * 200); // wait
		
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
