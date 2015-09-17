#include  <stdio.h>
#include  "MKL46Z4.h" 										/*Device Header*/

#define LED_User 2												/* Led used */

/*---------------------------------------------------------------------------------*/
/*Config LED*/
/*---------------------------------------------------------------------------------*/
 void LED_Init(void);

/*-------------------------------------------------------------------------------*/
/* Led ON */
/*-------------------------------------------------------------------------------*/
 void LED_On (uint32_t led) ;
 
/*-------------------------------------------------------------------------------*/
/* Led Off */
/*-------------------------------------------------------------------------------*/
void LED_Off (uint32_t led) ;
/*LED Toggle*/
void LED_Toggle(uint32_t led);