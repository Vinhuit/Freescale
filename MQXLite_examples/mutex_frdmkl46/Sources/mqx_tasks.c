/* ###################################################################
**     Filename    : mqx_tasks.c
**     Project     : mutex_frdmkl46
**     Processor   : MKL46Z256VMC4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : Keil ARM C/C++ Compiler
**     Date/Time   : 2014-11-03, 16:38, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         Main_task - void Main_task(uint32_t task_init_data);
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

MUTEX_STRUCT   print_mutex;
/*
** ===================================================================
**     Event       :  Main_task (module mqx_tasks)
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
void Main_task(uint32_t task_init_data)
{
   MUTEX_ATTR_STRUCT mutexattr;
   char*            strings1[] = { "1: ", "Hello from Print task 1\n" };
   char*            strings2[] = { "2: ", "Print task 2 is alive\n" };
	 void						  *stack1, *stack2;

   /* Initialize mutex attributes */
   if (_mutatr_init(&mutexattr) != MQX_OK) {
      printf("Initialize mutex attributes failed.\n");
      _task_block();
   }
   
   /* Initialize the mutex */ 
   if (_mutex_init(&print_mutex, &mutexattr) != MQX_OK) {
      printf("Initialize print mutex failed.\n");
      _task_block();
   }
	 stack1 = _lwmem_alloc_system_zero(PRINT_TASK_STACK_SIZE);
	 stack2 = _lwmem_alloc_system_zero(PRINT_TASK_STACK_SIZE);
   /* Create the print tasks */
   _task_create_at(0, PRINT_TASK, (uint32_t)strings1, stack1, PRINT_TASK_STACK_SIZE);
   _task_create_at(0, PRINT_TASK, (uint32_t)strings2, stack2, PRINT_TASK_STACK_SIZE);

   _task_block();
}

/*
** ===================================================================
**     Event       :  Print_task (module mqx_tasks)
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
void Print_task(uint32_t task_init_data)
{
   char **strings = (char **)task_init_data;

   while(TRUE) {

      if (_mutex_lock(&print_mutex) != MQX_OK) 
			{
         printf("Mutex lock failed.\n");
         _task_block();
      }

      /*
       * The yield in between the puts calls just returnes back,
       * as the other task is blocked waiting for the mutex.
       * Both strings are always printed together on a single line.
       */
      puts(strings[0]);     
      _sched_yield(); 
      puts(strings[1]);

      _mutex_unlock(&print_mutex);

      /* Passes control to the other task (which then obtains the mutex) */
      _sched_yield();
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
