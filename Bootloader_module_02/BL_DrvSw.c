#include "MKL46Z4.h"  
#include "BL_DrvSw.h"
/*----------------------------------------------------------------------------
  BUTTON_Config
 *----------------------------------------------------------------------------*/
void BUTTON_Config(void) {
  SIM->SCGC5    |= (1UL << 11) ;        /* Enable Clock to Port C*/ 
	PORTC->PCR[3]  = (1UL <<  0 |1UL <<  1| 1UL <<  8);//thiet lap sw1 : PS,PE,MUX=1 (GPIO)
	PORTC->PCR[12]  = (1UL <<  0 |1UL <<  1| 1UL <<  8);//thiet lap sw3 : PS,PE,MUX=1 (GPIO)
  PTC->PDOR |= 0UL <<  3 | 0UL << 12; // Thiet lap muc data logic
  PTC->PDDR |= 0UL <<  3 | 0UL << 12; //Xac dinh huong GPIO la input     	      

	PORTC->PCR[3] |=PORT_PCR_IRQC(10);//Chon ngat kich canh xuong cho sw1
	PORTC->PCR[12]|=PORT_PCR_IRQC(10);//Chon ngat kich canh xuong cho sw3
	
	NVIC_ClearPendingIRQ(31);// Dam bao ngat dang trong
	NVIC_EnableIRQ(31);//Bat ngat
}  
/*----------------------------------------------------------------------------
  SW_Get
	Lay gia tri cua switch.
	tra ve 1 khi sw1 duoc nhan, 3 khi sw duoc nhan, con lai tra ve 0
 *----------------------------------------------------------------------------*/
int SW_Get(void)
	{
	int sw;
	sw = PTC->PDIR;// doc chan PDIR
	if ((sw | 0xFFFFFFF7) == 0xFFFFFFF7) return 1;//sw 1 duoc nhan tra ve 1
	else if ((sw | 0xFFFFEFFF) == 0xFFFFEFFF) return 3;//sw 3 duoc nhan tra ve 3
	else return 0;// con lai tra ve  0
};
	
