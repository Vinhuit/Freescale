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

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */
typedef enum {
    BUTTON_NOT_INITIALIZED = -1,
    BUTTON_RELEASED,
    BUTTON_PRESSED
} button_state_t;
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
  int counter = 0;

	IO1_Init(NULL);
	
	printf("Hello World\n");

  while(1) {
    counter++;

    /* Write your code here ... */

  }
}
void Task1_task2(uint32_t task_init_data)
{
		LWSEM_STRUCT lwsem;
		LDD_TDeviceData* btn1_ptr, *led_ptr;
	  uint32_t button_press_count = 0;
	  button_state_t  button_state, button_last_state;

	  _lwsem_create(&lwsem, 0);
	
    btn1_ptr = GPIO1_Init(&lwsem);
		/* TODO: Enable pull up because board does not have external pull up resistor */
		PORTC_PCR3 |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;

		led_ptr = LED_Init(NULL);
	
	  printf("\n====================== GPIO Example ======================\n");
    printf("The (SW1) button is configured to trigger GPIO interrupt.\n");
    printf("Press the (SW1) button 3x to continue.\n\n");
    button_press_count = 1;

    while(button_press_count < 4){
        /* wait for button press, lwsem is set in button isr */
        _lwsem_wait(&lwsem);
        printf("Button pressed %dx\r", button_press_count++);
    }

		printf("The (SW1) button state is now polled.\n");
    printf("Press the (SW1) button to switch LED on or off\n\n");
		while (1)
		{

			  if (0 == GPIO1_GetFieldValue(btn1_ptr, BUTTON1))
        {
            button_state = BUTTON_PRESSED;
        }
        else
        {
            button_state = BUTTON_RELEASED;
        }

				if (button_state != button_last_state)  {
            printf("Button %s\r", button_state == BUTTON_PRESSED ? "pressed " : "released");
            button_last_state = button_state;
					
					  /* Set LED on or off arcodingly */
						LED_PutVal(led_ptr, button_state == BUTTON_PRESSED ? FALSE : TRUE);
        }
        /* Check button state every 20 ticks*/
        _time_delay_ticks(20);
		}
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
