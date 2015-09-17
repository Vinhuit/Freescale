/* ###################################################################
**     Filename    : mqx_tasks.c
**     Project     : lwmsgq_frdmkl46
**     Processor   : MKL46Z256VMC4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : Keil ARM C/C++ Compiler
**     Date/Time   : 2014-11-03, 16:12, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         server_task - void server_task(uint32_t task_init_data);
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
	
/* This structure contains a data field and a message header structure */
#define NUM_MESSAGES  3
#define MSG_SIZE      1
/* Number of clients */
#define NUM_CLIENTS 3

uint32_t server_queue[sizeof(LWMSGQ_STRUCT)/sizeof(uint32_t) +
   NUM_MESSAGES * MSG_SIZE];
uint32_t client_queue[sizeof(LWMSGQ_STRUCT)/sizeof(uint32_t) +
   NUM_MESSAGES * MSG_SIZE];
/*
** ===================================================================
**     Event       :  server_task (module mqx_tasks)
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
void server_task(uint32_t task_init_data)
{
   uint32_t          msg[MSG_SIZE];
   uint32_t          i;
   uint32_t          result;

   result = _lwmsgq_init((void *)server_queue, NUM_MESSAGES, MSG_SIZE);
   if (result != MQX_OK) {
      // lwmsgq_init failed
   } /* Endif */
   result = _lwmsgq_init((void *)client_queue, NUM_MESSAGES, MSG_SIZE);
   if (result != MQX_OK) {
      // lwmsgq_init failed
   } /* Endif */
   
   /* create the client tasks */
   for (i = 0; i < NUM_CLIENTS; i++) {
		  void *stack = _lwmem_alloc_system_zero(CLIENT_TASK_STACK_SIZE);
      _task_create_at(0, CLIENT_TASK, (uint32_t)i, stack, CLIENT_TASK_STACK_SIZE);
   }
      
   while (TRUE) {
      _lwmsgq_receive((void *)server_queue, (_mqx_max_type_ptr)msg, LWMSGQ_RECEIVE_BLOCK_ON_EMPTY, 0, 0);
      printf(" %c \n", msg[0]);
      
      _lwmsgq_send((void *)client_queue, (_mqx_max_type_ptr)msg, LWMSGQ_SEND_BLOCK_ON_FULL);
   }
}

/*
** ===================================================================
**     Event       :  client_task (module mqx_tasks)
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
void client_task(uint32_t task_init_data)
{
   uint32_t          msg[MSG_SIZE];
  
   while (TRUE) {
      msg[0] = ('A'+ "index");
     
      printf("Client Task %d\n", index);
      _lwmsgq_send((void *)server_queue, (_mqx_max_type_ptr)msg, LWMSGQ_SEND_BLOCK_ON_FULL);

      _time_delay_ticks(1);
      
      /* wait for a return message */
      _lwmsgq_receive((void *)client_queue, (_mqx_max_type_ptr)msg, LWMSGQ_RECEIVE_BLOCK_ON_EMPTY, 0, 0);
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
