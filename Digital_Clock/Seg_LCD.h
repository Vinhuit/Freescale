/******************************************************************/
//Seg_LCD.h	Rev 1.0	8/20/2014 EPH Initial version									//
//Author: Ethan Hettwer																						//
//Purpose:  Allow for easy control of Seg_LCD											//
/******************************************************************/


#include "MKL46Z4.h"

//Function Declarations
void SegLCD_Init(void);
void SegLCD_Set(uint8_t Value,uint8_t Digit);
void SegLCD_DisplayDecimal(uint16_t Value);
void SegLCD_DisplayHex(uint16_t Value);
void SegLCD_DisplayTime(uint8_t Value1, uint8_t Value2);
void SegLCD_DisplayError(uint8_t ErrorNum);

//Define Number of Front and Back plane pins
#define LCD_NUM_FRONTPLANE_PINS 8
#define LCD_NUM_BACKPLANE_PINS 4

//Create macros for segments
#define LCD_SEG_D 0x11
#define LCD_SEG_E 0x22
#define LCD_SEG_G 0x44
#define LCD_SEG_F 0x88
#define LCD_SEG_DECIMAL 0x11
#define LCD_SEG_C 0x22
#define LCD_SEG_B 0x44
#define LCD_SEG_A 0x88
#define LCD_CLEAR 0x00

//Create Macros for each pin
#define LCD_FRONTPLANE0 37u
#define LCD_FRONTPLANE1 17u
#define LCD_FRONTPLANE2 7u
#define LCD_FRONTPLANE3 8u
#define LCD_FRONTPLANE4 53u
#define LCD_FRONTPLANE5 38u
#define LCD_FRONTPLANE6 10u
#define LCD_FRONTPLANE7 11u
#define LCD_BACKPLANE0 40u
#define LCD_BACKPLANE1 52u
#define LCD_BACKPLANE2 19u
#define LCD_BACKPLANE3 18u

//Macros for turning decimal points and colon on and off
#define SegLCD_DP1_On() LCD->WF8B[LCD_FRONTPLANE1]	 |= LCD_SEG_DECIMAL;
#define SegLCD_DP1_Off() LCD->WF8B[LCD_FRONTPLANE1] &= ~LCD_SEG_DECIMAL;
#define SegLCD_DP2_On() LCD->WF8B[LCD_FRONTPLANE3]	 |= LCD_SEG_DECIMAL;
#define SegLCD_DP2_Off() LCD->WF8B[LCD_FRONTPLANE3] &= ~LCD_SEG_DECIMAL;
#define SegLCD_DP3_On() LCD->WF8B[LCD_FRONTPLANE5]	 |= LCD_SEG_DECIMAL;
#define SegLCD_DP3_Off() LCD->WF8B[LCD_FRONTPLANE5] &= ~LCD_SEG_DECIMAL;
#define SegLCD_Col_On() LCD->WF8B[LCD_FRONTPLANE7]	 |= LCD_SEG_DECIMAL;
#define SegLCD_Col_Off() LCD->WF8B[LCD_FRONTPLANE7] &= ~LCD_SEG_DECIMAL;

const static uint8_t LCD_Frontplane_Pin[LCD_NUM_FRONTPLANE_PINS] = {LCD_FRONTPLANE0, LCD_FRONTPLANE1, LCD_FRONTPLANE2, LCD_FRONTPLANE3, LCD_FRONTPLANE4, LCD_FRONTPLANE5, LCD_FRONTPLANE6, LCD_FRONTPLANE7};
const static uint8_t LCD_Backplane_Pin[LCD_NUM_BACKPLANE_PINS] = {LCD_BACKPLANE0, LCD_BACKPLANE1, LCD_BACKPLANE2, LCD_BACKPLANE3};
