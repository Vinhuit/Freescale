#ifndef _i2c_mcf54xx_prv_h
#define _i2c_mcf54xx_prv_h 
/*HEADER**********************************************************************
*
* Copyright 2008 Freescale Semiconductor, Inc.
*
* This software is owned or controlled by Freescale Semiconductor.
* Use of this software is governed by the Freescale MQX RTOS License
* distributed with this Material.
* See the MQX_RTOS_LICENSE file distributed for more details.
*
* Brief License Summary:
* This software is provided in source form for you to use free of charge,
* but it is not open source software. You are allowed to use this software
* but you cannot redistribute it or derivative works of it in source form.
* The software may be used only in connection with a product containing
* a Freescale microprocessor, microcontroller, or digital signal processor.
* See license agreement file for full license terms including other
* restrictions.
*****************************************************************************
*
* Comments:
*
*   This file contains the definitions of constants and structures
*   required for the I2C drivers for 54XX Coldfire family.
*
*
*END************************************************************************/


/*--------------------------------------------------------------------------*/
/*
**                    DATATYPE DECLARATIONS
*/

/*
** MCF54XX_I2C_INFO_STRUCT
** Run time state information for each serial channel
*/
typedef struct mcf54xx_i2c_info_struct
{  
   /* Current initialized values */
   MCF54XX_I2C_INIT_STRUCT           INIT;
   
   VMCF54XX_I2C_STRUCT_PTR           I2C_PTR;
   
   /* The previous interrupt handler and data */
   void                  (_CODE_PTR_ OLD_ISR)(void *);
   void                             *OLD_ISR_DATA;

   /* Interrupt vector */
   uint32_t                           VECTOR;
   
   /* Actual baudrate table index */
   uint8_t                            BAUD_INDEX;

   /* Actual mode */
   uint8_t                            MODE;

   /* Actual state */
   uint8_t                            STATE;

   /* Destination address */
   uint8_t                            ADDRESSEE;
   
   /* Operation flags */
   uint8_t                            OPERATION;
   
   /* Number of bytes requested for receive */
   uint32_t                           RX_REQUEST;

   /* Pointer to the buffer to use for Tx/Rx data */
   unsigned char                         *RX_BUFFER;

   /* Rx write index */
   uint32_t                           RX_IN;

   /* Rx read index */
   uint32_t                           RX_OUT;

   /* Pointer to the buffer to use for current Tx data */
   unsigned char                         *TX_BUFFER;

   /* Tx write index */
   uint32_t                           TX_IN;

   /* Tx read index */
   uint32_t                           TX_OUT;

   /* Statistical information */
   I2C_STATISTICS_STRUCT             STATISTICS;

} MCF54XX_I2C_INFO_STRUCT, * MCF54XX_I2C_INFO_STRUCT_PTR; 

typedef volatile struct mcf54xx_i2c_info_struct * VMCF54XX_I2C_INFO_STRUCT_PTR; 

/*
** MCF54XX_I2C_BAUDRATE_STRUCT
*/
typedef struct mcf54xx_i2c_baudrate_struct
{  
   /* Baudrate */
   uint32_t                           BAUD_RATE;
   
   /* Divider */
   uint8_t                            IC;   
   
} MCF54XX_I2C_BAUDRATE_STRUCT, * MCF54XX_I2C_BAUDRATE_STRUCT_PTR; 

#endif
/* EOF */
