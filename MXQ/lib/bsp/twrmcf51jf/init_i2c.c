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

const KI2C_INIT_STRUCT _bsp_i2c0_init = {
/* Channel                          */  0,
/* Mode                             */  BSP_I2C0_MODE,
/* Address                          */  BSP_I2C0_ADDRESS,
/* Baud rate                        */  BSP_I2C0_BAUD_RATE,
/* TX Buffer Size (interrupt only)  */  BSP_I2C0_TX_BUFFER_SIZE,
/* RX Buffer Size (interrupt only)  */  BSP_I2C0_RX_BUFFER_SIZE,
#if BSPCFG_ENABLE_LEGACY_II2C_SLAVE
/* I2C port base address            */  BSP_I2C0_PORT_BASE,
/* I2C port vector number           */  BSP_I2C0_PORT_VECTOR,
/* I2C SDA pin number               */  BSP_I2C0_SDA_PIN_NUM,
#endif
};

const KI2C_INIT_STRUCT _bsp_i2c1_init = {
/* Channel                          */  1,
/* Mode                             */  BSP_I2C1_MODE,
/* Address                          */  BSP_I2C1_ADDRESS,
/* Baud rate                        */  BSP_I2C1_BAUD_RATE,
/* TX Buffer Size (interrupt only)  */  BSP_I2C1_TX_BUFFER_SIZE,
/* RX Buffer Size (interrupt only)  */  BSP_I2C1_RX_BUFFER_SIZE,
#if BSPCFG_ENABLE_LEGACY_II2C_SLAVE
/* I2C port base address            */  BSP_I2C1_PORT_BASE,
/* I2C port vector number           */  BSP_I2C1_PORT_VECTOR,
/* I2C SDA pin number               */  BSP_I2C1_SDA_PIN_NUM,
#endif
};

const KI2C_INIT_STRUCT _bsp_i2c2_init = {
/* Channel                          */  2,
/* Mode                             */  BSP_I2C2_MODE,
/* Address                          */  BSP_I2C2_ADDRESS,
/* Baud rate                        */  BSP_I2C2_BAUD_RATE,
/* TX Buffer Size (interrupt only)  */  BSP_I2C2_TX_BUFFER_SIZE,
/* RX Buffer Size (interrupt only)  */  BSP_I2C2_RX_BUFFER_SIZE,
#if BSPCFG_ENABLE_LEGACY_II2C_SLAVE
/* I2C port base address            */  BSP_I2C2_PORT_BASE,
/* I2C port vector number           */  BSP_I2C2_PORT_VECTOR,
/* I2C SDA pin number               */  BSP_I2C2_SDA_PIN_NUM,
#endif
};

const KI2C_INIT_STRUCT _bsp_i2c3_init = {
/* Channel                          */  3,
/* Mode                             */  BSP_I2C3_MODE,
/* Address                          */  BSP_I2C3_ADDRESS,
/* Baud rate                        */  BSP_I2C3_BAUD_RATE,
/* TX Buffer Size (interrupt only)  */  BSP_I2C3_TX_BUFFER_SIZE,
/* RX Buffer Size (interrupt only)  */  BSP_I2C3_RX_BUFFER_SIZE,
#if BSPCFG_ENABLE_LEGACY_II2C_SLAVE
/* I2C port base address            */  BSP_I2C3_PORT_BASE,
/* I2C port vector number           */  BSP_I2C3_PORT_VECTOR,
/* I2C SDA pin number               */  BSP_I2C3_SDA_PIN_NUM,
#endif
};

/* EOF */
