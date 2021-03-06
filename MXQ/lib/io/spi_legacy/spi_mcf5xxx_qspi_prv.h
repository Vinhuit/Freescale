#ifndef __spi_mcf5xxx_qspi_prv_h__ 
#define __spi_mcf5xxx_qspi_prv_h__
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
*   This file contains definitions private to the SPI driver.
*
*
*END************************************************************************/

#include "spi.h"
#include "spi_mcf5xxx_qspi.h"

/*--------------------------------------------------------------------------*/
/*
**                            CONSTANT DEFINITIONS
*/

#define MCF5XXX_QSPI_QDR_TX (0x0001)
#define MCF5XXX_QSPI_QDR_RX (0x0002)

/*--------------------------------------------------------------------------*/
/*
**                    DATATYPE DECLARATIONS
*/

/*
** MCF5XXX_QSPI_INFO_STRUCT
** Run time state information for each spi channel
*/
typedef struct mcf5xxx_qspi_info_struct
{
   /* The spi device register */
   VMCF5XXX_QSPI_STRUCT_PTR          QSPI_PTR;

   /* The current init values for this port */
   MCF5XXX_QSPI_INIT_STRUCT          INIT;

   /* The previous vector, interrupt handler and data */
   void                  (_CODE_PTR_ OLD_ISR)(void *);
   void                             *OLD_ISR_DATA;

   /* Receive/transmit flag */
   uint16_t                           RECEIVING;
   
   /* Difference between transmitted and received bytes */
   uint16_t                           ONTHEWAY;
   
   /* Number of bytes requested to receive */
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
   
   /* Chip select used during transfers */
   uint32_t                           CS;

   /* Previous state of chip select */
   uint32_t                           CS_ACTIVE;

   /* Chip select set callback */
   SPI_CS_CALLBACK                   CS_CALLBACK[MCF5XXX_QSPI_CS_COUNT];

   /* User data for callback */
   void                             *CS_USERDATA[MCF5XXX_QSPI_CS_COUNT];

   /* Statistical information */
   SPI_STATISTICS_STRUCT             STATS;
   
   /* Whether to hold CS low between subsequent transfers */
   bool                           CSIV_ACTIVE;

} MCF5XXX_QSPI_INFO_STRUCT, * MCF5XXX_QSPI_INFO_STRUCT_PTR;
      
#endif
/* EOF */
