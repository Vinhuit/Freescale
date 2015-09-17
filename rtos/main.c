/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "MKL46Z4.h"                    // Device header

const uint32_t led_mask[] = {1UL << 5,1UL << 29}; 	/*PTD5 (Led green),PTE29 (Led red)*/

/*
 * main: initialize and start the system
 */
void LED_Initialize(void);                  // Intialize the LED
void Blink(void const *arg);                // Blink function

osThreadDef(Blink, osPriorityNormal, 1, 0); // Define the blink task

/*
 * main: initialize and start the system
 */
int main (void) {
  osKernelInitialize ();                    // initialize CMSIS-RTOS
	SystemInit();                             // Initialize system core functions
  LED_Initialize();                         // GPIOB Pin 21 Setup
  osThreadCreate(osThread (Blink), NULL);   // Thread creation of blink a led
	osKernelStart ();                         // start thread execution 
	
}

void LED_Initialize(void) {

  SIM->SCGC5    |= (1UL << 13)|(1UL << 12);        		/* Enable Clock to Port E and Port D*/ 
  PORTE->PCR[29] = (1UL <<  8);        								/* Pin is GPIO Value MUX = 0x001 */
	PORTD->PCR[5] = (1UL << 8);
  PTE->PDOR |= led_mask[1];                						/* Data Output */
  PTD->PDOR |= led_mask[0];

  PTD->PDDR |= led_mask[0];                						/* Enable Output Led red  */
	PTE->PDDR |= led_mask[1];														/* Enable Output Led Green   */
}
void LED_Toggle(uint32_t led){
	if(led)PTE->PTOR   = led_mask[led]; 						/*Set LOW Output */
	else
	PTD->PTOR = led_mask[led]; 
};

void Blink(void const *arg) {
	while(1) {
		osDelay(2000);                          // Delay 1 sec
    LED_Toggle(0);
	}
}
