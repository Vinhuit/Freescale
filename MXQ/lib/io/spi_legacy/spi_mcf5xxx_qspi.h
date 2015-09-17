#ifndef __spi_mcf5xxx_qspi_h__
#define __spi_mcf5xxx_qspi_h__
/*HEADER**********************************************************************
*
* Copyright 2009 Freescale Semiconductor, Inc.
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
*   required for the SPI driver
*
*
*END************************************************************************/


/*--------------------------------------------------------------------------*/
/*
**                    CONSTANT DEFINITIONS
*/

#define MCF5XXX_QSPI_CS_COUNT (4)


/*--------------------------------------------------------------------------*/
/*
**                    DATATYPE DECLARATIONS
*/

/*
** MCF5XXX_QSPI_INIT_STRUCT
**
** This structure defines the initialization parameters to be used
** when a spi port is initialized.
*/
typedef struct mcf5xxx_qspi_init_struct
{
   /* spi channel number */
   uint32_t CHANNEL;

   /* The SPI default chip select */
   uint32_t CS; 
   
   /* The SPI transfer mode */
   uint32_t TRANSFER_MODE;

   /* The Baud rate for the channel */
   uint32_t BAUD_RATE;

   /* The clock speed of CPU */
   uint32_t CLOCK_SPEED;

   /* The SPI clock phase and polarity mode */
   uint32_t CLOCK_POL_PHASE; 

   /* The maximum size of each receive */
   uint32_t RX_BUFFER_SIZE;

   /* The maximum size of each transmit */
   uint32_t TX_BUFFER_SIZE;
   
} MCF5XXX_QSPI_INIT_STRUCT, * MCF5XXX_QSPI_INIT_STRUCT_PTR;

typedef const MCF5XXX_QSPI_INIT_STRUCT * MCF5XXX_QSPI_INIT_STRUCT_CPTR;

/*--------------------------------------------------------------------------*/
/* 
**                        FUNCTION PROTOTYPES
*/

#ifdef __cplusplus
extern "C" {
#endif

extern _mqx_uint _mcf5xxx_qspi_polled_install(char *, MCF5XXX_QSPI_INIT_STRUCT_CPTR);
extern _mqx_uint _mcf5xxx_qspi_int_install(char *, MCF5XXX_QSPI_INIT_STRUCT_CPTR);
extern void   *_bsp_get_qspi_base_address(uint8_t);
extern uint32_t _bsp_get_qspi_vector(uint8_t);

#ifdef __cplusplus
}
#endif


#endif 

/* EOF */
