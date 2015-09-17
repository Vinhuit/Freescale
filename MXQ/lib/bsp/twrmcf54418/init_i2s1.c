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

#include "mqx.h"
#include "bsp.h"
#include "i2s_audio.h"

const AUDIO_DATA_FORMAT _bsp_audio_data_init = {
   AUDIO_BIG_ENDIAN,       /* Endian of input data */
   AUDIO_ALIGNMENT_LEFT,   /* Aligment of input data */
   8,                      /* Bit size of input data */
   1,                      /* Sample size in bytes */
   1                       /* Number of channels */
};

const MCF54XX_I2S_INIT_STRUCT _bsp_i2s1_init = {
   0,                       /* I2S channel    */
   BSP_I2S1_MODE,           /* I2S mode       */ 
   BSP_I2S1_DATA_BITS,      /* Number of valid data bits*/
   BSP_I2S1_CLOCK_SOURCE,   /* SSI_CLOCK source*/
   FALSE,                   /* Data are stereo */
   FALSE,                   /* Transmit dummy data */
   BSP_I2S1_INT_LEVEL,      /* Interrupt level to use */
   BSP_I2S1_BUFFER_SIZE,    /* Tx buffer size */
   &_bsp_audio_data_init    /* I/O data format */
};

/* EOF */
