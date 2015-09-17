/* ###################################################################
**     Filename    : mqx_tasks.c
**     Project     : lwevent_frdmkl46
**     Processor   : MKL46Z256VMC4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : Keil ARM C/C++ Compiler
**     Date/Time   : 2014-11-03, 15:34, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         service_task - void service_task(uint32_t task_init_data);
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

/* LW Event Definitions */
LWEVENT_STRUCT lwevent;

/*
** ===================================================================
**     Event       :  service_task (module mqx_tasks)
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
void service_task(uint32_t task_init_data)
{
   while (TRUE) {
      _time_delay_ticks(200);
      if (_lwevent_set(&lwevent,0x01) != MQX_OK) {
         printf("\nSet Event failed");
         _task_block();
      }
   }
}

/*
** ===================================================================
**     Event       :  simulated_ISR_task (module mqx_tasks)
**
**     Component   :  Task2 [MQXLite_task]
**     Description :
**         MQX task routine. The routine is generated into mqx_tasks.c
**         file.
**     Parameters  :
**         NAME            - DESCRIPTION
**         task_init_data  - 
**     Returns     : Nothing
** ===================================================================
*/

void simulated_ISR_task(uint32_t task_init_data)
{
   _task_id second_task_id;
	 void *stack;

   /* create lwevent group */
   if (_lwevent_create(&lwevent,0) != MQX_OK) {
      printf("\nMake event failed");
      _task_block();
   }

	 stack = _lwmem_alloc_system_zero(SERVICE_TASK_STACK_SIZE);
   /* Create the ISR task */
   second_task_id = _task_create_at(0, SERVICE_TASK, 0, stack, SERVICE_TASK_STACK_SIZE);
   if (second_task_id == MQX_NULL_TASK_ID) {
      printf("Could not create simulated_ISR_task \n");
      _task_block();
   }

   while (TRUE) {
      if (_lwevent_wait_ticks(&lwevent,1,TRUE,0) != MQX_OK) {
         printf("\nEvent Wait failed");
         _task_block();
      }

      if (_lwevent_clear(&lwevent,0x01) != MQX_OK) {
         printf("\nEvent Clear failed");
         _task_block();
      }
      printf(" Tick \n");
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
