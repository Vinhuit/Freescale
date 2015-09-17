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
*   The file contains low level SPI interrupt driver functions.
*
*
*END************************************************************************/
#include <mqx.h>
#include <bsp.h>
#include <io_prv.h>
#include <fio_prv.h>
#include "spi.h"
#include "spi_pol_prv.h"
#include "spi_int_prv.h"
#include "spi_spi16_prv.h"

extern uint32_t _spi16_polled_init(SPI16_INIT_STRUCT_PTR, void **, char *);
extern uint32_t _spi16_polled_ioctl(SPI16_INFO_STRUCT_PTR, uint32_t, uint32_t *, uint32_t);

static uint32_t _spi16_int_init(IO_SPI_INT_DEVICE_STRUCT_PTR, char *);
static uint32_t _spi16_int_deinit(IO_SPI_INT_DEVICE_STRUCT_PTR, SPI16_INFO_STRUCT_PTR);
static uint32_t _spi16_int_rx(IO_SPI_INT_DEVICE_STRUCT_PTR, unsigned char *, int32_t);
static uint32_t _spi16_int_tx(IO_SPI_INT_DEVICE_STRUCT_PTR, unsigned char *, int32_t);
static uint32_t _spi16_int_enable(SPI16_INFO_STRUCT_PTR io_info_ptr);
static void _spi16_int_isr(void *parameter);
                       
/*FUNCTION****************************************************************
* 
* Function Name    : _spi16_int_install
* Returned Value   : MQX error code
* Comments         :
*    Install an SPI device.
*
*END*********************************************************************/
uint32_t _spi16_int_install
   (
      /* [IN] A string that identifies the device for fopen */
      char           *identifier,
  
      /* [IN] The I/O init data pointer */
      SPI16_INIT_STRUCT_CPTR    init_data_ptr
   )
{
   return _io_spi_int_install(identifier,
      (uint32_t (_CODE_PTR_)(void *,char *))_spi16_int_init,
      (uint32_t (_CODE_PTR_)(void *))_spi16_int_enable,
      (uint32_t (_CODE_PTR_)(void *, void *))_spi16_int_deinit,
      (_mqx_int (_CODE_PTR_)(void *, char *, int32_t))_spi16_int_rx,
      (_mqx_int (_CODE_PTR_)(void *, char *, int32_t))_spi16_int_tx,
      (_mqx_int (_CODE_PTR_)(void *, uint32_t, _mqx_uint_ptr, _mqx_uint))_spi16_polled_ioctl, 
      (void *)init_data_ptr);
}

/*FUNCTION****************************************************************
* 
* Function Name    : _spi16_int_init  
* Returned Value   : MQX error code
* Comments         :
*    This function initializes the SPI module 
*
*END*********************************************************************/
static uint32_t _spi16_int_init
  (
     /* [IN] The address of the device specific information */
     IO_SPI_INT_DEVICE_STRUCT_PTR int_io_dev_ptr,

     /* [IN] The rest of the name of the device opened */
     char                     *open_name_ptr
  )
{
   SPI_MemMapPtr      spi_ptr;
   SPI16_INFO_STRUCT_PTR  io_info_ptr;
   SPI16_INIT_STRUCT_PTR  spi_init_ptr;
   uint32_t                        result = SPI_OK;

   spi_init_ptr = (SPI16_INIT_STRUCT_PTR)(int_io_dev_ptr->DEV_INIT_DATA_PTR);
   result = _spi16_polled_init(spi_init_ptr,
                                       &(int_io_dev_ptr->DEV_INFO_PTR),
                                       open_name_ptr);
   if (result)
   {
      return result;
   }
   
   io_info_ptr = int_io_dev_ptr->DEV_INFO_PTR;
   spi_ptr = io_info_ptr->SPI_PTR;
   
   io_info_ptr->RX_BUFFER = (void *)_mem_alloc_system(spi_init_ptr->RX_BUFFER_SIZE & (~ 1));
   if (NULL == io_info_ptr->RX_BUFFER) 
   {
      _mem_free (int_io_dev_ptr->DEV_INFO_PTR);
      int_io_dev_ptr->DEV_INFO_PTR = NULL;
      return MQX_OUT_OF_MEMORY;
   }
   io_info_ptr->TX_BUFFER = (void *)_mem_alloc_system(spi_init_ptr->TX_BUFFER_SIZE & (~ 1));
   if (NULL == io_info_ptr->TX_BUFFER) 
   {
      _mem_free (int_io_dev_ptr->DEV_INFO_PTR);
      int_io_dev_ptr->DEV_INFO_PTR = NULL;
      _mem_free (io_info_ptr->RX_BUFFER);
      io_info_ptr->RX_BUFFER = NULL;
      return MQX_OUT_OF_MEMORY;
   }
   _mem_set_type(io_info_ptr->RX_BUFFER,MEM_TYPE_IO_SPI_IN_BUFFER);       
   _mem_set_type(io_info_ptr->TX_BUFFER,MEM_TYPE_IO_SPI_OUT_BUFFER);       

   io_info_ptr->OLD_ISR_DATA = _int_get_isr_data(spi_init_ptr->VECTOR);
   io_info_ptr->OLD_ISR = _int_install_isr(spi_init_ptr->VECTOR, _spi16_int_isr, int_io_dev_ptr);
   
   return SPI_OK; 
}

/*FUNCTION****************************************************************
* 
* Function Name    : _spi16_int_deinit  
* Returned Value   : MQX error code
* Comments         :
*    This function de-initializes the SPI module 
*
*END*********************************************************************/
static uint32_t _spi16_int_deinit
   (
      /* [IN] The address of the device specific information */
      IO_SPI_INT_DEVICE_STRUCT_PTR  int_io_dev_ptr,

      /* [IN] The address of the channel specific information */
      SPI16_INFO_STRUCT_PTR io_info_ptr
   )
{
   SPI_MemMapPtr          spi_ptr;
   SPI16_INIT_STRUCT_PTR    spi_init_ptr;
   uint32_t                          index;
   
   if ((NULL == io_info_ptr) || (NULL == int_io_dev_ptr)) 
   {
      return SPI_ERROR_DEINIT_FAILED;
   }
   
   /* Disable the SPI and its interrupts */
   spi_ptr = io_info_ptr->SPI_PTR;
   index = spi_ptr->C1 & SPI_C1_MSTR_MASK;
   spi_ptr->C1 &= (~ (SPI_C1_SPE_MASK | SPI_C1_SPIE_MASK | SPI_C1_SPTIE_MASK));

   /* Disable all chip selects */
   if (0 != index) 
   {
      for (index = 0; index < SPI16_CS_COUNT; index++)
      {
         if ((NULL != io_info_ptr->CS_CALLBACK[index]) && (0 != (io_info_ptr->CS_ACTIVE & (1 << index))))
         {
            io_info_ptr->CS_CALLBACK[index] (1 << index, 1, io_info_ptr->CS_USERDATA[index]);
         }                
      }
      io_info_ptr->CS_ACTIVE = 0;
   }
   
   /* Return original interrupt vector */
   spi_init_ptr = (SPI16_INIT_STRUCT_PTR)(int_io_dev_ptr->DEV_INIT_DATA_PTR);
   _int_install_isr(spi_init_ptr->VECTOR, io_info_ptr->OLD_ISR, io_info_ptr->OLD_ISR_DATA);

   /* Release the buffers */   
   _mem_free(int_io_dev_ptr->DEV_INFO_PTR);
   int_io_dev_ptr->DEV_INFO_PTR = NULL;
   _mem_free(io_info_ptr->RX_BUFFER);
   io_info_ptr->RX_BUFFER = NULL;
   _mem_free(io_info_ptr->TX_BUFFER);
   io_info_ptr->TX_BUFFER = NULL;
       
   return SPI_OK;
}

/*FUNCTION****************************************************************
*
* Function Name    : _spi16_int_enable
* Returned Value   : MQX error code
* Comments         :
*    This function enables receive and error interrupt.
*
*END*********************************************************************/
static uint32_t _spi16_int_enable
   ( 
   /* [IN] The address of the channel specific information */
      SPI16_INFO_STRUCT_PTR io_info_ptr
   )
{
   SPI_MemMapPtr          spi_ptr;

   if (NULL == io_info_ptr)   
   {
      return SPI_ERROR_INVALID_PARAMETER;   
   }
   spi_ptr = io_info_ptr->SPI_PTR;
   spi_ptr->C1 |= SPI_C1_SPIE_MASK;
   return SPI_OK;
}

/*FUNCTION****************************************************************
* 
* Function Name    : _spi16_int_rx
* Returned Value   : Returns the number of bytes received
* Comments         : 
*   Reads the data into provided array.
*
*END*********************************************************************/
static uint32_t _spi16_int_rx
   (
      /* [IN] The address of the device specific information */
      IO_SPI_INT_DEVICE_STRUCT_PTR int_io_dev_ptr,

      /* [IN] The array characters are to be read from */
      unsigned char                    *buffer,
      
      /* [IN] Number of char's to transmit */
      int32_t                       size
   )
{
   SPI16_INFO_STRUCT_PTR   io_info_ptr;
   SPI_MemMapPtr           spi_ptr;
   SPI16_INIT_STRUCT_PTR   spi_init_ptr;
   uint32_t                         num, index;
   unsigned char                       *rx_ptr;

   io_info_ptr  = int_io_dev_ptr->DEV_INFO_PTR;
   spi_ptr = io_info_ptr->SPI_PTR;
   spi_init_ptr = &(io_info_ptr->INIT);
     
   /* Check buffer for any new data */
   rx_ptr = (unsigned char *)(io_info_ptr->RX_BUFFER);
   for (num = 0; num < size; num++) 
   {
      index = io_info_ptr->RX_OUT;
      if (index == (volatile uint32_t)io_info_ptr->RX_IN) break;
      *buffer++ = rx_ptr[index++];
      if (index >= io_info_ptr->INIT.RX_BUFFER_SIZE) index = 0;
      io_info_ptr->RX_OUT = index;
   }
   index = size - num;

   if (0 != index)
   {
      /* Not enough data, assert chip selects and enable further transfer */
      io_info_ptr->RX_REQUEST = (volatile uint32_t)index;
      
      if (spi_ptr->C1 & SPI_C1_MSTR_MASK) 
      {
         if (io_info_ptr->CS ^ io_info_ptr->CS_ACTIVE)
         {
           for (index = 0; index < SPI16_CS_COUNT; index++)
           {
              if ((0 != ((io_info_ptr->CS ^ io_info_ptr->CS_ACTIVE) & (1 << index))) && (NULL != io_info_ptr->CS_CALLBACK[index]))
              {
                 io_info_ptr->CS_CALLBACK[index] (1 << index, (io_info_ptr->CS_ACTIVE >> index) & 1, io_info_ptr->CS_USERDATA[index]);
              }
           }
           io_info_ptr->CS_ACTIVE = io_info_ptr->CS;
         }
      }
      spi_ptr->C1 |= SPI_C1_SPTIE_MASK;
   }
   return num;
}

/*FUNCTION****************************************************************
* 
* Function Name    : _spi16_int_tx
* Returned Value   : return number of byte transmitted
* Comments         : 
*   Writes the provided data into trasmit buffer
*
*END*********************************************************************/
static uint32_t _spi16_int_tx
   (
      /* [IN] The address of the device specific information */
      IO_SPI_INT_DEVICE_STRUCT_PTR int_io_dev_ptr,

      /* [IN] The array to store data */
      unsigned char                    *buffer,
      
      /* [IN] Number of char's to transmit */
      int32_t                       size
   )
{
   SPI16_INFO_STRUCT_PTR   io_info_ptr;
   SPI_MemMapPtr         spi_ptr;
   SPI16_INIT_STRUCT_PTR   spi_init_ptr;
   uint32_t                         num, index, tmp;
   unsigned char                       *tx_ptr;
    
   io_info_ptr = int_io_dev_ptr->DEV_INFO_PTR;
   spi_ptr = io_info_ptr->SPI_PTR;
   spi_init_ptr = &(io_info_ptr->INIT);

   /* Fill new data into transmit buffer */
   tx_ptr = (unsigned char *)(io_info_ptr->TX_BUFFER);
   index = io_info_ptr->TX_IN;
   for (num = 0; num < size; num++) 
   {
      tmp = index + 1;
      if (tmp >= io_info_ptr->INIT.TX_BUFFER_SIZE) tmp = 0;
      if (tmp == io_info_ptr->TX_OUT) break;
      tx_ptr[index] = *buffer++;
      index = tmp;
   }
   io_info_ptr->TX_IN = index;
   
   if (0 != num) 
   {
      /* At least one byte to transmit, assert chip selects and enable transfer */
      if (spi_ptr->C1 & SPI_C1_MSTR_MASK) 
      {
         if (io_info_ptr->CS ^ io_info_ptr->CS_ACTIVE)
         {
           for (index = 0; index < SPI16_CS_COUNT; index++)
           {
              if ((0 != ((io_info_ptr->CS ^ io_info_ptr->CS_ACTIVE) & (1 << index))) && (NULL != io_info_ptr->CS_CALLBACK[index]))
              {
                 io_info_ptr->CS_CALLBACK[index] (1 << index, (io_info_ptr->CS_ACTIVE >> index) & 1, io_info_ptr->CS_USERDATA[index]);
              }
           }
           io_info_ptr->CS_ACTIVE = io_info_ptr->CS;
         }
      }
      spi_ptr->C1 |= SPI_C1_SPTIE_MASK;
   }
   return num;
}

/*FUNCTION****************************************************************
* 
* Function Name    : _spi16_int_isr
* Returned Value   : SPI interrupt routine
* Comments         : 
*   Reads/writes bytes transmitted from/to buffers.
*
*END*********************************************************************/
static void _spi16_int_isr
   (
      /* [IN] The address of the device specific information */
      void                      *parameter
   )
{
   SPI16_INFO_STRUCT_PTR io_info_ptr;
   SPI_MemMapPtr       spi_ptr;
   volatile uint8_t               spis;
   uint8_t                        index;
   bool                       tx_and_rx;
   uint16_t                       data;
   
   tx_and_rx = (((IO_SPI_INT_DEVICE_STRUCT_PTR)parameter)->FLAGS & SPI_FLAG_FULL_DUPLEX) != 0;
   io_info_ptr = ((IO_SPI_INT_DEVICE_STRUCT_PTR)parameter)->DEV_INFO_PTR;
   spi_ptr = (SPI_MemMapPtr)(io_info_ptr->SPI_PTR);
   
   /* Read interrupt status */
   spis = spi_ptr->S;
   io_info_ptr->STATS.INTERRUPTS++;
  
   /* Master mode fault, switch to slave */
   if (spis & SPI_S_MODF_MASK)
   {
      spi_ptr->C1 &= (~ SPI_C1_MSTR_MASK);
      io_info_ptr->STATS.TX_ABORTS++;
   }

   /* Receive is temporarily disabled during read */
   if (spis & SPI_S_SPRF_MASK)
   {
      /* Always clear receive int flag when enabled */
      data = ((uint16_t)spi_ptr->DH)<<8 |spi_ptr->DL;
      if (io_info_ptr->ONTHEWAY > 0) io_info_ptr->ONTHEWAY--;

      /* If request for receive and receive allowed (full duplex or first byte skipped already) */
      if (((tx_and_rx) || (1 == io_info_ptr->RECEIVING)) && (io_info_ptr->RX_REQUEST > 0))
      {
         /* Actual receive into buffer, if overflow, incoming bytes are thrown away */
         index = io_info_ptr->RX_IN + 1;
         if (index >= io_info_ptr->INIT.RX_BUFFER_SIZE) index = 0;
         if (index != io_info_ptr->RX_OUT) 
         {
            if (spi_ptr->C2 & SPI_C2_SPIMODE_MASK) 
            {
               io_info_ptr->RX_BUFFER[io_info_ptr->RX_IN] = (uint8_t)(data >> 8);
               io_info_ptr->RX_IN = index++;
               if (index >= io_info_ptr->INIT.RX_BUFFER_SIZE) index = 0;
            }
            io_info_ptr->RX_BUFFER[io_info_ptr->RX_IN] = (uint8_t)data;
            io_info_ptr->RX_IN = index;
            io_info_ptr->STATS.RX_PACKETS++;
         } else {
            io_info_ptr->STATS.RX_OVERFLOWS++;
         }
         if (--io_info_ptr->RX_REQUEST == 0) 
         {
            io_info_ptr->RECEIVING = 0;
         }
      } 

      /* Skipping bytes sent before receive */
      if (io_info_ptr->RECEIVING > 1) io_info_ptr->RECEIVING--;
   }

   /* Transmit is disabled when idle */
   if ((spi_ptr->C1 & SPI_C1_SPTIE_MASK) && (spis & SPI_S_SPTEF_MASK))
   {
      index = io_info_ptr->RX_IN + 1;
      if (index >= io_info_ptr->INIT.RX_BUFFER_SIZE) index = 0;
      if ((tx_and_rx) || (io_info_ptr->RECEIVING == 0)) 
      {
         if (io_info_ptr->TX_OUT != io_info_ptr->TX_IN)
         {
            /* Change pin direction to output in single wire mode */
            if (spi_ptr->C2 & SPI_C2_SPC0_MASK)
            {
               spi_ptr->C2 |= SPI_C2_BIDIROE_MASK;   
            }
            /* Actual transmit */
            data = io_info_ptr->TX_BUFFER[io_info_ptr->TX_OUT++];
            if (io_info_ptr->TX_OUT >= io_info_ptr->INIT.TX_BUFFER_SIZE) io_info_ptr->TX_OUT = 0;
            if (spi_ptr->C2 & SPI_C2_SPIMODE_MASK) 
            {
               data = (data << 8) | io_info_ptr->TX_BUFFER[io_info_ptr->TX_OUT++];
               if (io_info_ptr->TX_OUT >= io_info_ptr->INIT.TX_BUFFER_SIZE) io_info_ptr->TX_OUT = 0;
            }
            spi_ptr->DL = data & 0xFF;
            spi_ptr->DH = data >>8;
            io_info_ptr->STATS.TX_PACKETS++;
            io_info_ptr->ONTHEWAY++;
         } 
         else 
         {
            /* Nothing to transmit, change pin direction to input in single wire mode */
            if (spi_ptr->C2 & SPI_C2_SPC0_MASK)
            {
               spi_ptr->C2 &= (~ SPI_C2_BIDIROE_MASK);
            }
            if ((index != io_info_ptr->RX_OUT) && (io_info_ptr->RX_REQUEST > 0))
            {
               /* Dummy transmit, setup receive */
               spi_ptr->DL = 0xFF;
               spi_ptr->DH = 0xFF ;
               io_info_ptr->ONTHEWAY++;
               io_info_ptr->RECEIVING = io_info_ptr->ONTHEWAY;
            } 
            else
            {
               /* No reason for transmit, block transmit interrupt */
               spi_ptr->C1 &= (~ SPI_C1_SPTIE_MASK);
            }
         }
      } 
      else 
      {
         if ((index != io_info_ptr->RX_OUT) && (io_info_ptr->RX_REQUEST > 0))
         {
            /* Dummy transmit for receiving */
            spi_ptr->DL = 0xFF;
            spi_ptr->DH = 0xFF ;
            io_info_ptr->ONTHEWAY++;
         } 
         else
         {
            /* No reason for transmit, block transmit interrupt */
            spi_ptr->C1 &= (~ SPI_C1_SPTIE_MASK);
         }
      }
   }
}
/* EOF */
