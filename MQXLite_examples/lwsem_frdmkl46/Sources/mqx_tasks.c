/* ###################################################################
**     Filename    : mqx_tasks.c
**     Project     : lwsem_frdmkl46
**     Processor   : MKL46Z256VMC4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : Keil ARM C/C++ Compiler
**     Date/Time   : 2014-11-04, 15:34, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         write_task - void write_task(uint32_t task_init_data);
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

/* Number of Writer Tasks */
#define NUM_WRITERS  3
	
/* User includes (#include below this line is not maintained by Processor Expert) */
typedef struct sw_fifo
{
   LWSEM_STRUCT   READ_SEM;
   LWSEM_STRUCT   WRITE_SEM;
   unsigned char          DATA;
} SW_FIFO, * SW_FIFO_PTR;

SW_FIFO fifo;

/*
** ===================================================================
**     Event       :  write_task (module mqx_tasks)
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
void write_task(uint32_t task_init_data)
{
   printf("\nWrite task created: 0x%lX", task_init_data);
   while (TRUE) {
      if (_lwsem_wait(&fifo.WRITE_SEM) != MQX_OK) {
         printf("\n_lwsem_wait failed");
         _task_block();
      }
      fifo.DATA = (unsigned char)task_init_data;
      _lwsem_post(&fifo.READ_SEM);
   }
}

/*
** ===================================================================
**     Event       :  read_task (module mqx_tasks)
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
void read_task(uint32_t task_init_data)
{
   _task_id  task_id;
   _mqx_uint result;
   _mqx_uint i;

   /* Create the lightweight semaphores */
   result = _lwsem_create(&fifo.READ_SEM, 0);
   if (result != MQX_OK) {
      printf("\nCreating read_sem failed: 0x%X", result);
      _task_block();
   }

   result = _lwsem_create(&fifo.WRITE_SEM, 1);
   if (result != MQX_OK) {
      printf("\nCreating write_sem failed: 0x%X", result);
      _task_block();
   }

   /* Create the write tasks */
   for (i = 0; i < NUM_WRITERS; i++) {
		  void *stack = _lwmem_alloc_system_zero(WRITE_TASK_STACK_SIZE);

      task_id = _task_create_at(0, WRITE_TASK, (uint32_t)('A' + i), stack, WRITE_TASK_STACK_SIZE);
      printf("\nwrite_task created, id 0x%lX", task_id);
   }

   while(TRUE) {
      result = _lwsem_wait(&fifo.READ_SEM);
      if (result != MQX_OK) {
         printf("\n_lwsem_wait failed: 0x%X", result);
         _task_block();
      }
      putchar('\n');
      putchar(fifo.DATA);
      _lwsem_post(&fifo.WRITE_SEM);
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
