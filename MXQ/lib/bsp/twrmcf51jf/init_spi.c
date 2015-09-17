/*HEADER**********************************************************************
*
* Copyright 2009 Freescale Semiconductor, Inc.
* Copyright 1989-2009 ARC International
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
*   This file contains the definition for the baud rate for the SPI
*   channel
*
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>

/*
** parameters of spi0 initialization
*/

const SPI16_INIT_STRUCT _bsp_spi0_init = {
/* Channel                          */  0,
/* Chip select                      */  SPI16_CS0,
/* Input Clock [Hz]                 */  BSP_SPI_CLOCK,
/* Baud Rate                        */  BSP_SPI0_BAUDRATE,
/* RX Buffer Size (interrupt only)  */  BSP_SPI0_RX_BUFFER_SIZE,
/* TX Buffer Size (interrupt only)  */  BSP_SPI0_TX_BUFFER_SIZE,
/* Int Vector                       */  Vspi0,
/* Transfer mode                    */  SPI_DEVICE_MASTER_MODE,
/* Clock phase                      */  SPI_CLK_POL_PHA_MODE0
};

/*
** parameters of spi1 initialization
*/

const SPI16_INIT_STRUCT _bsp_spi1_init = {
/* Channel                          */  1,
/* Chip select                      */  SPI16_CS0,
/* Input Clock [Hz]                 */  BSP_SPI_CLOCK,
/* Baud Rate                        */  BSP_SPI1_BAUDRATE,
/* RX Buffer Size (interrupt only)  */  BSP_SPI1_RX_BUFFER_SIZE,
/* TX Buffer Size (interrupt only)  */  BSP_SPI1_TX_BUFFER_SIZE,
/* Int Vector                       */  Vspi1,
/* Transfer mode                    */  SPI_DEVICE_MASTER_MODE,
/* Clock phase                      */  SPI_CLK_POL_PHA_MODE0
};


/* EOF */
