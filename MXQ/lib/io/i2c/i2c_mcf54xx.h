#ifndef _i2c_mcf54xx_h
#define _i2c_mcf54xx_h 1
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
*   required for the I2C drivers for the MCF54XX.
*
*
*END************************************************************************/


/*--------------------------------------------------------------------------*/
/*
**                    DATATYPE DECLARATIONS
*/

/*
** MCF54XX_I2C_INIT_STRUCT
**
** This structure defines the initialization parameters to be used
** when a serial port is initialized.
*/
typedef struct mcf54xx_i2c_init_struct
{
   /* The I2C channel to initialize */
   uint8_t                 CHANNEL;

   /* Default operating mode */
   uint8_t                 MODE;

   /* The I2C station address for the channel */
   uint8_t                 STATION_ADDRESS;

   /* Desired baud rate */
   uint32_t                BAUD_RATE;

   /* Interrupt level to use if interrupt driven */
   _int_level             LEVEL;

   /* Tx buffer size (interrupt only) */
   uint32_t                TX_BUFFER_SIZE;

   /* Rx buffer size (interrupt only) */
   uint32_t                RX_BUFFER_SIZE;

} MCF54XX_I2C_INIT_STRUCT, * MCF54XX_I2C_INIT_STRUCT_PTR;
typedef const MCF54XX_I2C_INIT_STRUCT * MCF54XX_I2C_INIT_STRUCT_CPTR;

/*--------------------------------------------------------------------------*/
/* 
**                        FUNCTION PROTOTYPES
*/

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t _mcf54xx_i2c_polled_install (char *, MCF54XX_I2C_INIT_STRUCT_CPTR);
extern uint32_t _mcf54xx_i2c_int_install (char *, MCF54XX_I2C_INIT_STRUCT_CPTR);
extern void   *_bsp_get_i2c_base_address (uint8_t);
extern uint32_t _bsp_get_i2c_vector (uint8_t);

#ifdef __cplusplus
}
#endif

#endif
/* EOF */
