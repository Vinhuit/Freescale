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
*   The file contains low level SPI driver functions for 5xxx Coldfire family.
*
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <io_prv.h>
#include <fio_prv.h>
#include "spi.h"
#include "spi_pol_prv.h"
#include "spi_spi16_prv.h"

uint32_t _spi16_polled_init(SPI16_INIT_STRUCT_PTR, void **, char *);
uint32_t _spi16_polled_ioctl(SPI16_INFO_STRUCT_PTR, uint32_t, uint32_t *, uint32_t);

static uint32_t _spi16_polled_deinit(IO_SPI_POLLED_DEVICE_STRUCT_PTR, SPI16_INFO_STRUCT_PTR);
static uint32_t _spi16_polled_rx(IO_SPI_POLLED_DEVICE_STRUCT_PTR, unsigned char *, uint32_t);
static uint32_t _spi16_polled_tx(IO_SPI_POLLED_DEVICE_STRUCT_PTR, unsigned char *, uint32_t);
static uint16_t _spi16_polled_tx_rx(SPI16_INFO_STRUCT_PTR, uint16_t, uint8_t);

/*FUNCTION****************************************************************
* 
* Function Name    : spi16_find_baudrate
* Returned Value   : uint8_t divider register setting
* Comments         :
*    Find closest setting of divider register for given baudrate.
*
*END*********************************************************************/
static uint8_t _spi16_find_baudrate 
   (
      /* [IN] Module input clock in Hz */
      uint32_t clock, 

      /* [IN] Desired baudrate in Hz */
      uint32_t baudrate
   ) 
{
  uint32_t sppr, spr, div, pres, min, tmp;
  int32_t val;
  
  spr = 8;
  sppr = 8;
  min = (uint32_t)-1;
  for (pres = 1; pres <= 8; pres++) {
    for (div = 1; div <= 9; div++) {
      val = (pres << div) * baudrate - clock;
      if (val < 0) val = -val;
      if (min > val) {
        min = val;
        spr = div;
        sppr = pres;
      }
    }
  }
  return (uint8_t)(((sppr - 1) << 4) | (spr - 1));
}
                      
/*FUNCTION****************************************************************
* 
* Function Name    : _spi16_polled_install
* Returned Value   : MQX error code
* Comments         :
*    Install an SPI device.
*
*END*********************************************************************/
uint32_t _spi16_polled_install
   (
      /* [IN] A string that identifies the device for fopen */
      char           *identifier,
  
      /* [IN] The I/O init data pointer */
      SPI16_INIT_STRUCT_CPTR    init_data_ptr
   )
{
   return _io_spi_polled_install(identifier,
      (uint32_t (_CODE_PTR_)(void *, void **, char *))_spi16_polled_init,
      (uint32_t (_CODE_PTR_)(void *, void *))_spi16_polled_deinit,
      (_mqx_int (_CODE_PTR_)(void *, char *, int32_t))_spi16_polled_rx,
      (_mqx_int (_CODE_PTR_)(void *, char *, int32_t))_spi16_polled_tx,
      (_mqx_int (_CODE_PTR_)(void *, uint32_t, _mqx_uint_ptr, _mqx_uint))_spi16_polled_ioctl, 
      (void *)init_data_ptr);
}

/*FUNCTION****************************************************************
* 
* Function Name    : _spi16_polled_init  
* Returned Value   : MQX error code
* Comments         :
*    This function initializes the SPI module 
*
*END*********************************************************************/
uint32_t _spi16_polled_init
(
   /* [IN] The initialization information for the device being opened */
   SPI16_INIT_STRUCT_PTR spi_init_ptr,

   /* [OUT] The address to store device specific information */
   void                    **io_info_ptr_ptr,  

   /* [IN] The rest of the name of the device opened */
   char                      *open_name_ptr
)
{
   SPI_MemMapPtr     spi_ptr;
   SPI16_INFO_STRUCT_PTR io_info_ptr;
   uint8_t                        SPIC1,SPIC2;
   uint32_t                       i;

   /* Check channel */
   spi_ptr = _bsp_get_spi_base_address (spi_init_ptr->CHANNEL);
   if (NULL == spi_ptr)
   {
      return SPI_ERROR_CHANNEL_INVALID;
   }

   if (_bsp_spi_io_init (spi_init_ptr->CHANNEL) == -1)
   {
      return SPI_ERROR_CHANNEL_INVALID;
   }

   /* Disable SPI */
   spi_ptr->C1 = 0;
   
   /* Initialize internal data */
   io_info_ptr = (SPI16_INFO_STRUCT *)_mem_alloc_system_zero((uint32_t)sizeof(SPI16_INFO_STRUCT));
   if (io_info_ptr == NULL) 
   {
      return MQX_OUT_OF_MEMORY;
   }
   _mem_set_type(io_info_ptr,MEM_TYPE_IO_SPI_INFO_STRUCT);

   *io_info_ptr_ptr = io_info_ptr;
   
   io_info_ptr->SPI_PTR            = spi_ptr;
   io_info_ptr->INIT               = *spi_init_ptr;
   io_info_ptr->RECEIVING          = 0;
   io_info_ptr->ONTHEWAY           = 0;
   io_info_ptr->RX_REQUEST         = 0;
   io_info_ptr->RX_BUFFER          = NULL;
   io_info_ptr->RX_IN              = 0;
   io_info_ptr->RX_OUT             = 0;
   io_info_ptr->TX_BUFFER          = NULL;
   io_info_ptr->TX_IN              = 0;
   io_info_ptr->TX_OUT             = 0;
   io_info_ptr->STATS.INTERRUPTS   = 0;
   io_info_ptr->STATS.RX_PACKETS   = 0;
   io_info_ptr->STATS.RX_OVERFLOWS = 0;
   io_info_ptr->STATS.TX_PACKETS   = 0;
   io_info_ptr->STATS.TX_ABORTS    = 0;
   io_info_ptr->STATS.TX_UNDERFLOWS= 0;
   io_info_ptr->CS                 = spi_init_ptr->CS;
   io_info_ptr->CS_ACTIVE          = 0;
   for (i = 0; i < SPI16_CS_COUNT; i++)
   {
      io_info_ptr->CS_CALLBACK[i] = NULL;
      io_info_ptr->CS_USERDATA[i] = NULL;
   }

   /* Set transfer mode */
   if (spi_init_ptr->TRANSFER_MODE == SPI_DEVICE_SLAVE_MODE)
   {
      SPIC1 = 0;
      SPIC2 = 0;
   }
   else if (spi_init_ptr->TRANSFER_MODE == SPI_DEVICE_MASTER_MODE)
   {
      SPIC1 = SPI_C1_MSTR_MASK | SPI_C1_SSOE_MASK;
      SPIC2 = 0;
   }
   else if (spi_init_ptr->TRANSFER_MODE == SPI_DEVICE_BIO_MASTER_MODE)
   {
      SPIC1 = SPI_C1_MSTR_MASK;
      SPIC2 = SPI_C2_SPC0_MASK;
   }
   else if (spi_init_ptr->TRANSFER_MODE == SPI_DEVICE_BIO_SLAVE_MODE)
   {
      SPIC1 = 0;
      SPIC2 = SPI_C2_SPC0_MASK;
   }   
   else
   {
      _mem_free (*io_info_ptr_ptr);
      *io_info_ptr_ptr = NULL;
      return SPI_ERROR_TRANSFER_MODE_INVALID;
   }

   /* Set the SPI clock baud rate divider */
   spi_ptr->BR = _spi16_find_baudrate (spi_init_ptr->CLOCK_SPEED, spi_init_ptr->BAUD_RATE);

   /* Set up SPI clock polarity and phase */
   switch (spi_init_ptr->CLOCK_POL_PHASE)
   {
   case (SPI_CLK_POL_PHA_MODE0):
       /* Inactive state of SPI_CLK = logic 0 */
      SPIC1 &= (~ SPI_C1_CPOL_MASK);
       /* SPI_CLK transitions middle of bit timing */
        SPIC1 &= (~ SPI_C1_CPHA_MASK);
      break;
   case (SPI_CLK_POL_PHA_MODE1):
       /* Inactive state of SPI_CLK = logic 0 */
      SPIC1 &= (~ SPI_C1_CPOL_MASK);
       /* SPI_CLK transitions begining of bit timing */
        SPIC1 |= SPI_C1_CPHA_MASK;
      break;
   case (SPI_CLK_POL_PHA_MODE2):
       /* Inactive state of SPI_CLK = logic 1 */
      SPIC1 |= SPI_C1_CPOL_MASK;
       /* SPI_CLK transitions middle of bit timing */
        SPIC1 &= (~ SPI_C1_CPHA_MASK);
      break;
   case (SPI_CLK_POL_PHA_MODE3):
       /* Inactive state of SPI_CLK = logic 1 */
      SPIC1 |= SPI_C1_CPOL_MASK;
       /* SPI_CLK transitions begining of bit timing */
        SPIC1 |= SPI_C1_CPHA_MASK;
      break;
   default:
      _mem_free (*io_info_ptr_ptr);
      *io_info_ptr_ptr = NULL;
      return SPI_ERROR_MODE_INVALID;       
   }

   /* Default: master mode, SSOE = 1, MODFEN = 0, clock stops in stop mode */
   spi_ptr-> C2= SPIC2 | SPI_C2_SPISWAI_MASK;
   spi_ptr-> C1 = SPIC1 | SPI_C1_SPE_MASK | SPI_C1_SSOE_MASK;
   return SPI_OK;
}

/*FUNCTION****************************************************************
* 
* Function Name    : spi16_polled_deinit  
* Returned Value   : MQX error code
* Comments         :
*    This function de-initializes the SPI module 
*
*END*********************************************************************/
static uint32_t _spi16_polled_deinit
   (
      /* [IN] The device information */
      IO_SPI_POLLED_DEVICE_STRUCT_PTR io_dev_ptr,

      /* [IN] The address of the device specific information */
      SPI16_INFO_STRUCT_PTR   io_info_ptr
   )
{
   SPI_MemMapPtr          spi_ptr;
   uint32_t                            index;

   if ((NULL == io_info_ptr) || (NULL == io_dev_ptr)) 
   {
      return SPI_ERROR_DEINIT_FAILED;
   }

   /* Disable the SPI */
   spi_ptr = io_info_ptr->SPI_PTR;
   spi_ptr-> C1 &= (~SPI_C1_SPE_MASK);

   /* Disable all chip selects */
   if (spi_ptr-> C1 & SPI_C1_MSTR_MASK) 
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
   
   _mem_free(io_dev_ptr->DEV_INFO_PTR);
   io_dev_ptr->DEV_INFO_PTR = NULL;
   return SPI_OK;
}

/*FUNCTION****************************************************************
* 
* Function Name    : spi16_polled_ioctl
* Returned Value   : MQX error code
* Comments         : 
*    This function performs miscellaneous services for
*    the SPI I/O device.  
*
*END*********************************************************************/
uint32_t _spi16_polled_ioctl
   (
      /* [IN] The address of the device specific information */
      SPI16_INFO_STRUCT_PTR io_info_ptr,

      /* [IN] The command to perform */
      uint32_t                       cmd,

      /* [IN] Parameters for the command */
      uint32_t                   *param_ptr,
      
      /* [IN] Opening flags */
      uint32_t                       flags
   )
{
   SPI_MemMapPtr        spi_ptr;
   SPI_CS_CALLBACK_STRUCT_PTR       callbacks;
   uint32_t                          index;
   uint32_t                          result = SPI_OK;
   uint32_t                          val, num, size;
   SPI_READ_WRITE_STRUCT_PTR        rw_ptr;
   unsigned char                        *input, *output;
   uint16_t                          data;
  
   spi_ptr = io_info_ptr->SPI_PTR;
   
   switch (cmd) 
   {
      case IO_IOCTL_SPI_GET_BAUD:
         if (NULL == param_ptr) 
         {
            result = SPI_ERROR_INVALID_PARAMETER;
         } 
         else 
         {
            uint32_t sppr,spr, div;
            sppr = (uint32_t)((uint8_t)(spi_ptr->BR)>>4) + 1;
            spr = (uint32_t)((uint8_t)(spi_ptr->BR) & 0x0F) + 1;
            div = sppr * ((uint32_t)0x0001 << spr);
            *param_ptr = (uint32_t)((io_info_ptr->INIT.CLOCK_SPEED) / div);
         }
         break;
      case IO_IOCTL_SPI_SET_BAUD:
         if (NULL == param_ptr) 
         {
            result = SPI_ERROR_INVALID_PARAMETER;
         } 
         else if (0 == (*param_ptr)) 
         {
            result = SPI_ERROR_BAUD_RATE_INVALID;
         } 
         else 
         {
            uint8_t val = _spi16_find_baudrate (io_info_ptr->INIT.CLOCK_SPEED, *param_ptr);
            /* Disable SPI module */
            spi_ptr-> C1 &= (~SPI_C1_SPE_MASK);
            /* Set the frequency divider */
            spi_ptr->BR = val;
            /* Enable SPI module */
            spi_ptr-> C1 |= SPI_C1_SPE_MASK;
         }
         break;
      case IO_IOCTL_SPI_DEVICE_DISABLE:
         spi_ptr->C1 &= (~SPI_C1_SPE_MASK);
         break;
      case IO_IOCTL_SPI_DEVICE_ENABLE:
         spi_ptr->C1 |= SPI_C1_SPE_MASK;
         break;
      case IO_IOCTL_SPI_GET_MODE:
         if (NULL == param_ptr) 
         {
            result = SPI_ERROR_INVALID_PARAMETER;
         } 
         else
         {
            if (spi_ptr->C1 & SPI_C1_CPOL_MASK)
            {
               if (spi_ptr->C1 & SPI_C1_CPHA_MASK)
               {
                  *param_ptr = SPI_CLK_POL_PHA_MODE3;
               }
               else
               {
                  *param_ptr = SPI_CLK_POL_PHA_MODE2;
               }
            }
            else
            {
               if (spi_ptr->C1 & SPI_C1_CPHA_MASK)
               {
                  *param_ptr = SPI_CLK_POL_PHA_MODE1;
               }
               else
               {
                  *param_ptr = SPI_CLK_POL_PHA_MODE0;
               }
            }
         }
         break;
      case IO_IOCTL_SPI_SET_MODE:
         if (NULL == param_ptr) 
         {
            result = SPI_ERROR_INVALID_PARAMETER;
         } 
         else
         {
            /* Disable SPI module */
            spi_ptr->C1 &= (~SPI_C1_SPE_MASK);
            switch (*param_ptr)
            {
               case (SPI_CLK_POL_PHA_MODE0):
                  /* Inactive state of SPI_CLK = logic 0 */
                  spi_ptr->C1 &= (~ SPI_C1_CPOL_MASK);
                  /* SPI_CLK transitions middle of bit timing */
                  spi_ptr->C1 &= (~ SPI_C1_CPHA_MASK);
                  break;
               case (SPI_CLK_POL_PHA_MODE1):
                  /* Inactive state of SPI_CLK = logic 0 */
                  spi_ptr->C1 &= (~ SPI_C1_CPOL_MASK);
                  /* SPI_CLK transitions begining of bit timing */
                  spi_ptr->C1 |= SPI_C1_CPHA_MASK;
                  break;
               case (SPI_CLK_POL_PHA_MODE2):
                  /* Inactive state of SPI_CLK = logic 1 */
                  spi_ptr->C1 |= SPI_C1_CPOL_MASK;
                  /* SPI_CLK transitions middle of bit timing */
                  spi_ptr->C1 &= (~ SPI_C1_CPHA_MASK);
                  break;
               case (SPI_CLK_POL_PHA_MODE3):
                  /* Inactive state of SPI_CLK = logic 1 */
                  spi_ptr->C1 |= SPI_C1_CPOL_MASK;
                  /* SPI_CLK transitions begining of bit timing */
                  spi_ptr->C1 |= SPI_C1_CPHA_MASK;
                  break;
               default:
                  result = SPI_ERROR_MODE_INVALID;
                  break;
             }
             /* Enable SPI module */
             spi_ptr->C1 |= SPI_C1_SPE_MASK;         
         }
         break;
      case IO_IOCTL_SPI_GET_TRANSFER_MODE:
         if (NULL == param_ptr) 
         {
            result = SPI_ERROR_INVALID_PARAMETER;
         } 
         else
         {
            if (spi_ptr->C1 & SPI_C1_MSTR_MASK) 
            {
               if (spi_ptr-> C2& SPI_C2_SPC0_MASK) 
               {
                  *param_ptr = SPI_DEVICE_BIO_MASTER_MODE; 
               }
               else 
               {
                  *param_ptr = SPI_DEVICE_MASTER_MODE;
               }
            }
            else 
            {
               if (spi_ptr-> C2& SPI_C2_SPC0_MASK) 
               {
                  *param_ptr = SPI_DEVICE_BIO_SLAVE_MODE; 
               } 
               else 
               {
                  *param_ptr = SPI_DEVICE_SLAVE_MODE;
               }
            }
         }
         break;
      case IO_IOCTL_SPI_SET_TRANSFER_MODE:
         if (NULL == param_ptr) 
         {
            result = SPI_ERROR_INVALID_PARAMETER;
         } 
         else
         {
            /* Disable SPI module */
            spi_ptr->C1 &= (~SPI_C1_SPE_MASK);
            switch (*param_ptr)
            {
               case SPI_DEVICE_BIO_SLAVE_MODE:
                  spi_ptr->C1 &= (~ (SPI_C1_MSTR_MASK | SPI_C1_SSOE_MASK));
                  spi_ptr-> C2|= SPI_C2_SPC0_MASK;
                  break;
               case SPI_DEVICE_SLAVE_MODE:
                  spi_ptr->C1 &= (~ (SPI_C1_MSTR_MASK | SPI_C1_SSOE_MASK));
                  spi_ptr-> C2&= (~ SPI_C2_SPC0_MASK);
                  break;
               case SPI_DEVICE_BIO_MASTER_MODE:
                  spi_ptr->C1 |= (SPI_C1_MSTR_MASK | SPI_C1_SSOE_MASK);
                  spi_ptr-> C2|= SPI_C2_SPC0_MASK;
                  break;
               case SPI_DEVICE_MASTER_MODE:
                  spi_ptr->C1 |= (SPI_C1_MSTR_MASK | SPI_C1_SSOE_MASK);
                  spi_ptr-> C2&= (~ SPI_C2_SPC0_MASK);
                  break;
               default:
                  result = SPI_ERROR_TRANSFER_MODE_INVALID;            
                  break;
            }
            /* Enable SPI module */
            spi_ptr->C1 |= SPI_C1_SPE_MASK;               
         }
         break;
      case IO_IOCTL_SPI_GET_ENDIAN:
         if (NULL == param_ptr) 
         {
            result = SPI_ERROR_INVALID_PARAMETER;
         } 
         else if (spi_ptr->C1 & SPI_C1_LSBFE_MASK)
         {
            *param_ptr = SPI_DEVICE_LITTLE_ENDIAN;   
         }
         else
         {
            *param_ptr = SPI_DEVICE_BIG_ENDIAN; 
         }
         break;
      case IO_IOCTL_SPI_SET_ENDIAN:
         if (NULL == param_ptr) 
         {
            result = SPI_ERROR_INVALID_PARAMETER;
         } 
         else if (*param_ptr == SPI_DEVICE_LITTLE_ENDIAN)
         {
            spi_ptr->C1 |= SPI_C1_LSBFE_MASK;
         }
         else if (*param_ptr == SPI_DEVICE_BIG_ENDIAN)
         {
            spi_ptr->C1 &= (~SPI_C1_LSBFE_MASK);
         }
         else
         {
            result = SPI_ERROR_ENDIAN_INVALID;
         }
         break;
      case IO_IOCTL_SPI_ENABLE_MODF:
         /* Disable SPI module */
         spi_ptr->C1 &= (~ SPI_C1_SPE_MASK);
         /* Enable to MODF mode */
         spi_ptr-> C2|= SPI_C2_MODFEN_MASK;
         spi_ptr->C1 |= SPI_C1_SSOE_MASK;
         /* Enable SPI module */
         spi_ptr->C1 |= SPI_C1_SPE_MASK;                 
         break;
      case IO_IOCTL_SPI_DISABLE_MODF:
         /* Disable SPI module */
         spi_ptr->C1 &= (~ SPI_C1_SPE_MASK);
         /* Disable MODF mode */
         spi_ptr-> C2&= (~ SPI_C2_MODFEN_MASK);
         spi_ptr->C1 |= SPI_C1_SSOE_MASK;
         /* Enable SPI module */
         spi_ptr->C1 |= SPI_C1_SPE_MASK;  
         break;
      case IO_IOCTL_SPI_GET_STATS:
         if (NULL == param_ptr) 
         {
            result = SPI_ERROR_INVALID_PARAMETER;
         } 
         else 
         {
            *((SPI_STATISTICS_STRUCT_PTR)param_ptr) = io_info_ptr->STATS;
         }
         break;
      case IO_IOCTL_SPI_CLEAR_STATS:
         io_info_ptr->STATS.INTERRUPTS    = 0;
         io_info_ptr->STATS.RX_PACKETS    = 0;
         io_info_ptr->STATS.RX_OVERFLOWS  = 0;
         io_info_ptr->STATS.TX_PACKETS    = 0;
         io_info_ptr->STATS.TX_ABORTS     = 0;
         io_info_ptr->STATS.TX_UNDERFLOWS = 0;
         break;
      case IO_IOCTL_FLUSH_OUTPUT:
      case IO_IOCTL_SPI_FLUSH_DEASSERT_CS:
         while ((0 != io_info_ptr->RECEIVING) || (0 != io_info_ptr->ONTHEWAY) || (spi_ptr->C1 & SPI_C1_SPTIE_MASK)) 
            { };    /* Waiting for completion */
            
         if ((0 == (flags & SPI_FLAG_NO_DEASSERT_ON_FLUSH)) || (IO_IOCTL_SPI_FLUSH_DEASSERT_CS == cmd))
         {
            /* Deassert all chip selects */
            if (spi_ptr->C1 & SPI_C1_MSTR_MASK) 
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
         }
         break;
      case IO_IOCTL_SPI_GET_FRAMESIZE:
         if (NULL == param_ptr) 
         {
            result = SPI_ERROR_INVALID_PARAMETER;
         } 
         else 
         {
            if (spi_ptr-> C2& SPI_C2_SPIMODE_MASK) 
            {
              *param_ptr = 16;
            } 
            else 
            {
              *param_ptr = 8;
            }
         }
         break;
      case IO_IOCTL_SPI_SET_FRAMESIZE:
         if (NULL == param_ptr) 
         {
            result = SPI_ERROR_INVALID_PARAMETER;
         } 
         else 
         {
            if (8 == *param_ptr)
            {
               /* Disable SPI module */
               spi_ptr->C1 &= (~ SPI_C1_SPE_MASK);
               /* Set 8 bit mode */
               spi_ptr-> C2&= (~ SPI_C2_SPIMODE_MASK);
               /* Enable SPI module */
               spi_ptr->C1 |= SPI_C1_SPE_MASK;  
            } 
            else if (16 == *param_ptr)
            {
               /* Disable SPI module */
               spi_ptr->C1 &= (~ SPI_C1_SPE_MASK);
               /* Set 8 bit mode */
               spi_ptr-> C2|= SPI_C2_SPIMODE_MASK;
               /* Enable SPI module */
               spi_ptr->C1 |= SPI_C1_SPE_MASK;  
            } 
            else 
            {
               result = MQX_IO_OPERATION_NOT_AVAILABLE;
            }
         }
         break;
      case IO_IOCTL_SPI_GET_CS:
         if (NULL == param_ptr) 
         {
            result = SPI_ERROR_INVALID_PARAMETER;
         } 
         else 
         {
            *param_ptr = io_info_ptr->CS;
         }
         break;
      case IO_IOCTL_SPI_SET_CS:
         if (NULL == param_ptr) 
         {
            result = SPI_ERROR_INVALID_PARAMETER;
         } 
         else 
         {
            io_info_ptr->CS = *param_ptr;
         }
         break;
      case IO_IOCTL_SPI_SET_CS_CALLBACK:
         if (NULL == param_ptr) 
         {
            result = SPI_ERROR_INVALID_PARAMETER;
         } 
         else 
         {
            callbacks = (SPI_CS_CALLBACK_STRUCT_PTR)(param_ptr);
            for (index = 0; index < SPI16_CS_COUNT; index++)
            {
               if (0 != (callbacks->MASK & (1 << index)))
               {
                  io_info_ptr->CS_CALLBACK[index] = callbacks->CALLBACK;
                  io_info_ptr->CS_USERDATA[index] = callbacks->USERDATA;
               }                
            }
         }
         break;
      case IO_IOCTL_SPI_READ_WRITE:
         if (NULL == param_ptr) 
         {
            result = SPI_ERROR_INVALID_PARAMETER;
         } 
         else if (NULL != io_info_ptr->RX_BUFFER) 
         {
            result = MQX_IO_OPERATION_NOT_AVAILABLE;
         } 
         else 
         {
            rw_ptr = (SPI_READ_WRITE_STRUCT_PTR)(param_ptr);
            size = rw_ptr->BUFFER_LENGTH;
            input = (unsigned char *)rw_ptr->WRITE_BUFFER;
            output = (unsigned char *)rw_ptr->READ_BUFFER;            
   
            if (spi_ptr-> C2& SPI_C2_SPC0_MASK)
            {
               spi_ptr-> C2|= SPI_C2_BIDIROE_MASK;   
            }

            for (num = 0; num < size; num++)
            {
               /* If MSTR = MOFEN = 1, SSOE = 0, should check whether fault has occurred */
               if (spi_ptr->S & SPI_S_MODF_MASK)
               {  
                  spi_ptr->C1 &= (~ SPI_C1_MSTR_MASK);
                  io_info_ptr->STATS.TX_ABORTS++;
                  break;
               }
               data = input[0];
               input++;
               if (spi_ptr-> C2& SPI_C2_SPIMODE_MASK) 
               {
                  data = (data << 8) | input[1];
                  input++;
               }
               data = _spi16_polled_tx_rx (io_info_ptr, data, 1);
               if (spi_ptr-> C2& SPI_C2_SPIMODE_MASK) 
               {
                  output[0] = (uint8_t)(data >> 8);
                  output++;
               }
               output[0] = (uint8_t)data;
               output++;
               io_info_ptr->STATS.RX_PACKETS++;
               io_info_ptr->STATS.TX_PACKETS++;
            }

            if (spi_ptr-> C2& SPI_C2_SPC0_MASK)
            {
               spi_ptr-> C2&= (~ SPI_C2_BIDIROE_MASK);
            }
         }
         break;
      case IO_IOCTL_SPI_KEEP_QSPI_CS_ACTIVE:
         result = MQX_IO_OPERATION_NOT_AVAILABLE;
         break;
      default:
         result = IO_ERROR_INVALID_IOCTL_CMD;
         break;
   }
   return result;
}

/*FUNCTION****************************************************************
* 
* Function Name    : _spi16_polled_tx_rx
* Returned Value   : byte received   
* Comments         : 
*   Actual transmit and receive function.
*
*END*********************************************************************/
static uint16_t _spi16_polled_tx_rx 
   (
      /* [IN] The address of the device registers */
      SPI16_INFO_STRUCT_PTR io_info_ptr, 
      
      /* [IN] Transmitted byte */
      uint16_t                       output, 
      
      /* [IN] Flag to bypass transmit for slave configuration */
      uint8_t                        flag
   )
{
   uint32_t                          index;
   SPI_MemMapPtr        spi_ptr = io_info_ptr->SPI_PTR;
   
   /* Assert actual chip select */ 
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
   
   if (flag)
   {
      /* Wait write buffer empty flag */
      while (! (spi_ptr->S & SPI_S_SPTEF_MASK)) 
         { };
      
          spi_ptr-> DH = (uint8_t)(output>>8);
          spi_ptr-> DL = (uint8_t)output ;
   }
   
   /* Wait read buffer full flag */
   while (! (spi_ptr->S & SPI_S_SPRF_MASK)) 
      { };
   return ((((uint16_t)spi_ptr-> DH)<<8)|((uint8_t)spi_ptr-> DL));
}

/*FUNCTION****************************************************************
* 
* Function Name    : _spi16_polled_rx
* Returned Value   : Returns the number of bytes received
* Comments         : 
*   Reads the data into provided array.
*
*END*********************************************************************/
static uint32_t _spi16_polled_rx
   (
      /* [IN] The device information */
      IO_SPI_POLLED_DEVICE_STRUCT_PTR io_dev_ptr,

      /* [IN] The array characters are to be read from */
      unsigned char                       *buffer,
      
      /* [IN] Number of chars to transmit */
      uint32_t                         size
   )
{
   SPI16_INFO_STRUCT_PTR      io_info_ptr;
   SPI_MemMapPtr          spi_ptr;
   uint32_t                            index, num = 0;
   uint16_t                            data;
   uint8_t                             master, full;
   
   io_info_ptr  = io_dev_ptr->DEV_INFO_PTR;
   spi_ptr = io_info_ptr->SPI_PTR;
   
   if (spi_ptr-> C2& SPI_C2_SPC0_MASK)
   {
      spi_ptr-> C2&= (~ SPI_C2_BIDIROE_MASK);
   }
   
   master = spi_ptr->C1 & SPI_C1_MSTR_MASK;
   full = io_dev_ptr->FLAGS & SPI_FLAG_FULL_DUPLEX;
   
   for (num = 0; num < size; num++)
   {
      /* If MSTR = MOFEN = 1, SSOE = 0, should check whether fault has occurred */
      if (spi_ptr-> S & SPI_S_MODF_MASK)
      {
         spi_ptr->C1 &= (~ SPI_C1_MSTR_MASK);
         io_info_ptr->STATS.TX_ABORTS++;
         break;
      }
      
      data = buffer[0];
      if (spi_ptr-> C2& SPI_C2_SPIMODE_MASK) 
      {
         data = (data << 8) | buffer[1];
      }
      data = _spi16_polled_tx_rx (io_info_ptr, data, master | full);
      if (spi_ptr-> C2& SPI_C2_SPIMODE_MASK) 
      {
         buffer[0] = (uint8_t)(data >> 8);
         buffer++;
         num++;
      }
      buffer[0] = (uint8_t)data;
      buffer++;
      if (0 != full) 
      {
         io_info_ptr->STATS.TX_PACKETS++;
      }
      io_info_ptr->STATS.RX_PACKETS++;   
   }
   return num;
}

/*FUNCTION****************************************************************
* 
* Function Name    : _mcf51_spi_polled_tx
* Returned Value   : return number of bytes transmitted
* Comments         : 
*   Writes the provided data buffer and loops until transmission complete
*
*END*********************************************************************/
static uint32_t _spi16_polled_tx
   (
      /* [IN] The device information */
      IO_SPI_POLLED_DEVICE_STRUCT_PTR io_dev_ptr,

      /* [IN] The array to store data */
      unsigned char                       *buffer,
      
      /* [IN] Number of chars to transmit */
      uint32_t                         size
   )
{
   SPI16_INFO_STRUCT_PTR      io_info_ptr;
   SPI_MemMapPtr          spi_ptr;
   uint32_t                            num;
   uint16_t                            data;

   io_info_ptr  = io_dev_ptr->DEV_INFO_PTR;
   spi_ptr = io_info_ptr->SPI_PTR;
   
   if (spi_ptr-> C2& SPI_C2_SPC0_MASK)
   {
      spi_ptr-> C2|= SPI_C2_BIDIROE_MASK;   
   }
   
   for (num = 0; num < size; num++)
   {
      /* If MSTR = MOFEN = 1, SSOE = 0, should check whether fault has occurred */
      if (spi_ptr->S & SPI_S_MODF_MASK)
      {  
         spi_ptr->C1 &= (~ SPI_C1_MSTR_MASK);
         io_info_ptr->STATS.TX_ABORTS++;
         break;
      }
      
      data = buffer[0];
      buffer++;
      if (spi_ptr-> C2& SPI_C2_SPIMODE_MASK) 
      {
         data = (data << 8) | buffer[0];
         buffer++;
         num++;
      }
      data = _spi16_polled_tx_rx (io_info_ptr, data, 1);
      io_info_ptr->STATS.TX_PACKETS++;
   }

   if (spi_ptr-> C2& SPI_C2_SPC0_MASK)
   {
      spi_ptr-> C2&= (~ SPI_C2_BIDIROE_MASK);
   }
   return num;
}
/* EOF */
