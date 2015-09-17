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
*   This file contains the definition for the baud rate for the I2C
*   channel
*
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"

const MCF52XX_I2C_INIT_STRUCT _bsp_i2c1_init = {
   1,                      /* I2C channel    */
   BSP_I2C1_MODE,          /* I2C mode       */ 
   BSP_I2C1_ADDRESS,       /* I2C address    */ 
   BSP_I2C1_BAUD_RATE,     /* I2C baud rate  */ 
   BSP_I2C1_INT_LEVEL,     /* I2C int level  */ 
   BSP_I2C1_INT_SUBLEVEL,  /* I2c int sublvl */ 
   BSP_I2C1_TX_BUFFER_SIZE,/* I2C int tx buf */
   BSP_I2C1_RX_BUFFER_SIZE /* I2C int rx buf */
};

/* EOF */
