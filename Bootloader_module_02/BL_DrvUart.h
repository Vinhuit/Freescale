/*---------------------------------------------------------------------------
*Name: Nguyen Tran Tien Dat
*Student ID: 12520561
*----------------------------------------------------------------------------
*Bootloader Driver Uart
*Baund = 112500,Data =8 bit, No piraty
*--------------------------------------------------------------------------*/
#include <stdio.h>
#include "MKL46Z4.h"
#include "string.h"


void UART0_Config();
void UART0_Transmitter(uint8_t data);
void UART0_Transmit_String(char* a);
uint8_t UART0_Receiver();
