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
*   This file contains the SPI polled I/O (master) driver functions.
*
*
*END************************************************************************/

#include <mqx_inc.h>
#include <fio.h>
#include <fio_prv.h>
#include <io.h>
#include <io_prv.h>
#include "spi.h"
#include "spi_pol_prv.h"

/*FUNCTION****************************************************************
* 
* Function Name    : _io_spi_polled_install
* Returned Value   : MQX error code
* Comments         :
*    Install polled SPI device.
*
*END**********************************************************************/
_mqx_uint _io_spi_polled_install
   (
      /* [IN] A string that identifies the device for fopen */
      char              *identifier,
  
      /* [IN] The I/O init function */
      _mqx_uint (_CODE_PTR_ init)(void *, void **, char *),
      
      /* [IN] The I/O de-init function */
      _mqx_uint (_CODE_PTR_ deinit)(void *, void *),
      
      /* [IN] The input function */
      _mqx_int  (_CODE_PTR_ recv)(void *, char *, _mqx_int),
     
      /* [IN] The output function */
      _mqx_int  (_CODE_PTR_ xmit)(void *, char *, _mqx_int),

      /* [IN] The I/O ioctl function */
      _mqx_int  (_CODE_PTR_ ioctl)(void *, _mqx_uint, _mqx_uint_ptr, _mqx_uint),

      /* [IN] The I/O init data pointer */
      void                 *init_data_ptr
      
   )
{ /* Body */
   IO_SPI_POLLED_DEVICE_STRUCT_PTR polled_dev_ptr;

   polled_dev_ptr = (IO_SPI_POLLED_DEVICE_STRUCT_PTR)_mem_alloc_system_zero((_mem_size)sizeof(IO_SPI_POLLED_DEVICE_STRUCT));
   if (polled_dev_ptr == NULL) 
   {
      return MQX_OUT_OF_MEMORY;
   }
   _mem_set_type(polled_dev_ptr,MEM_TYPE_IO_SPI_POLLED_DEVICE_STRUCT);

   polled_dev_ptr->DEV_INIT          = init;
   polled_dev_ptr->DEV_DEINIT        = deinit;
   polled_dev_ptr->DEV_READ          = recv;
   polled_dev_ptr->DEV_WRITE         = xmit;
   polled_dev_ptr->DEV_IOCTL         = ioctl;
   polled_dev_ptr->DEV_INIT_DATA_PTR = init_data_ptr;
   
   return (_io_dev_install_ext(identifier,
      _io_spi_polled_open, _io_spi_polled_close,
      _io_spi_polled_read, 
      _io_spi_polled_write, 
      _io_spi_polled_ioctl, _io_spi_polled_uninstall,
      (void *)polled_dev_ptr));
} /* Endbody */

/*FUNCTION****************************************************************
*
* Function Name    : _io_spi_polled_uninstall
* Returned Value   : MQX error code
* Comments         :
*    Uninstall polled SPI device.
*
*END**********************************************************************/
_mqx_int _io_spi_polled_uninstall
   (
      /* [IN] The IO device structure for the device */
      IO_DEVICE_STRUCT_PTR         io_dev_ptr
   )
{ /* Body */
   IO_SPI_POLLED_DEVICE_STRUCT_PTR polled_dev_ptr = io_dev_ptr->DRIVER_INIT_PTR;

   if (polled_dev_ptr->COUNT == 0) 
   {
      if (polled_dev_ptr->DEV_DEINIT) 
      {
         (*polled_dev_ptr->DEV_DEINIT)(polled_dev_ptr->DEV_INIT_DATA_PTR,
            polled_dev_ptr->DEV_INFO_PTR);
      }
      _mem_free(io_dev_ptr->DRIVER_INIT_PTR);
      io_dev_ptr->DRIVER_INIT_PTR = NULL;
      return IO_OK;
   } else {
      return IO_ERROR_DEVICE_BUSY;
   } /* Endif */
} /* Endbody */

/*FUNCTION****************************************************************
* 
* Function Name    : _io_spi_polled_open
* Returned Value   : MQX error code
* Comments         :
*    This routine initializes the SPI I/O channel. It acquires
*    memory, then stores information into it about the channel.
*    It then calls the hardware interface function to initialize the channel.
* 
*END**********************************************************************/
_mqx_int _io_spi_polled_open
   (
      /* [IN] The file handle for the device being opened */
      FILE_DEVICE_STRUCT_PTR         fd_ptr,
       
      /* [IN] The remaining portion of the name of the device */
      char                            *open_name_ptr,

      /* [IN] The flags to be used during operation */
      char                            *flags
   )
{ /* Body */
   IO_DEVICE_STRUCT_PTR              io_dev_ptr;
   IO_SPI_POLLED_DEVICE_STRUCT_PTR   polled_dev_ptr;
   _mqx_int                          result = MQX_OK;

   io_dev_ptr     = (IO_DEVICE_STRUCT_PTR)fd_ptr->DEV_PTR;
   polled_dev_ptr = (IO_SPI_POLLED_DEVICE_STRUCT_PTR)(io_dev_ptr->DRIVER_INIT_PTR);

   _int_disable();
   if (polled_dev_ptr->COUNT) 
   {
      /* Device is already opened */
      result = MQX_IO_OPERATION_NOT_AVAILABLE;
      _int_enable();
      return result;
   }
   polled_dev_ptr->COUNT = 1;
   _int_enable();
      
   polled_dev_ptr->FLAGS = (_mqx_uint)flags;
   fd_ptr->FLAGS         = (_mqx_uint)flags;
            
   result = (*polled_dev_ptr->DEV_INIT)((void *)polled_dev_ptr->DEV_INIT_DATA_PTR, 
                                        &polled_dev_ptr->DEV_INFO_PTR, open_name_ptr);
   if (result != MQX_OK) 
   {
      polled_dev_ptr->COUNT = 0;
   }
   return result;
} /* Endbody */

/*FUNCTION****************************************************************
* 
* Function Name    : _io_spi_polled_close
* Returned Value   : MQX error code
* Comments         :
*    This routine closes the SPI I/O channel.
* 
*END**********************************************************************/
_mqx_int _io_spi_polled_close
   (
      /* [IN] The file handle for the device being closed */
      FILE_DEVICE_STRUCT_PTR        fd_ptr
   )
{ /* Body */
   IO_DEVICE_STRUCT_PTR             io_dev_ptr;
   IO_SPI_POLLED_DEVICE_STRUCT_PTR  polled_dev_ptr;
   _mqx_int                         result = MQX_OK;

   io_dev_ptr     = (IO_DEVICE_STRUCT_PTR)fd_ptr->DEV_PTR;
   polled_dev_ptr = (void *)io_dev_ptr->DRIVER_INIT_PTR;

   _int_disable();
   if (--polled_dev_ptr->COUNT == 0) 
   {
      if (polled_dev_ptr->DEV_DEINIT) 
      {
         result = (*polled_dev_ptr->DEV_DEINIT)(polled_dev_ptr, polled_dev_ptr->DEV_INFO_PTR);
      }
   }
   _int_enable();
   return result;
} /* Endbody */
  
/*FUNCTION****************************************************************
* 
* Function Name    : _io_spi_polled_read
* Returned Value   : Number of bytes read
* Comments         :
*    Reads bytes into array from SPI input buffers.
*    Returns number of bytes received.
*
*END*********************************************************************/
_mqx_int _io_spi_polled_read
   (
      /* [IN] The handle returned from _fopen */
      FILE_DEVICE_STRUCT_PTR          fd_ptr,

      /* [OUT] Where the characters are to be stored */
      char                        *data_ptr,

      /* [IN] The number of bytes to read */
      _mqx_int                        n
   )
{  /* Body */
   IO_DEVICE_STRUCT_PTR              io_dev_ptr;
   IO_SPI_POLLED_DEVICE_STRUCT_PTR   polled_dev_ptr;
   
   io_dev_ptr     = (IO_DEVICE_STRUCT_PTR)fd_ptr->DEV_PTR;
   polled_dev_ptr = (void *)io_dev_ptr->DRIVER_INIT_PTR;

   return (*polled_dev_ptr->DEV_READ)(polled_dev_ptr, data_ptr, n);
}  /* Endbody */

/*FUNCTION****************************************************************
* 
* Function Name    : _io_spi_polled_write
* Returned Value   : Number of bytes to write
* Comments         :
*   Returns  
*   This routine calls the appropriate write routine.
*
*END**********************************************************************/
_mqx_int _io_spi_polled_write
   (
      /* [IN] The handle returned from _fopen */
      FILE_DEVICE_STRUCT_PTR       fd_ptr,

      /* [IN] Where the bytes are stored */
      char                     *data_ptr,

      /* [IN] The number of bytes to output */
      _mqx_int                     n
   )
{  /* Body */
   IO_DEVICE_STRUCT_PTR            io_dev_ptr;
   IO_SPI_POLLED_DEVICE_STRUCT_PTR polled_dev_ptr;
   
   io_dev_ptr     = (IO_DEVICE_STRUCT_PTR)fd_ptr->DEV_PTR;
   polled_dev_ptr = (void *)io_dev_ptr->DRIVER_INIT_PTR;
               
   return (*polled_dev_ptr->DEV_WRITE)(polled_dev_ptr, data_ptr, n);
} /* Endbody */
   
/*FUNCTION*****************************************************************
* 
* Function Name    : _io_spi_polled_ioctl
* Returned Value   : MQX error code
* Comments         :
*    Returns result of SPI ioctl operation.
*
*END*********************************************************************/
_mqx_int _io_spi_polled_ioctl
   (
      /* [IN] the file handle for the device */
      FILE_DEVICE_STRUCT_PTR       fd_ptr,

      /* [IN] the ioctl command */
      _mqx_uint                    cmd,

      /* [IN] the ioctl parameters */
      void                        *input_param_ptr
   )
{ /* Body */
   IO_DEVICE_STRUCT_PTR            io_dev_ptr;
   IO_SPI_POLLED_DEVICE_STRUCT_PTR polled_dev_ptr;
   _mqx_int                        result = MQX_OK;
   _mqx_uint_ptr                   param_ptr = (_mqx_uint_ptr)input_param_ptr;

   io_dev_ptr     = (IO_DEVICE_STRUCT_PTR)fd_ptr->DEV_PTR;
   polled_dev_ptr = (void *)io_dev_ptr->DRIVER_INIT_PTR;

   switch (cmd) {
      case IO_IOCTL_SPI_GET_FLAGS:
         *param_ptr = polled_dev_ptr->FLAGS;
         break;
         
      case IO_IOCTL_SPI_SET_FLAGS:
         polled_dev_ptr->FLAGS = *param_ptr;
         fd_ptr->FLAGS = *param_ptr;
         break;
      
      case IO_IOCTL_DEVICE_IDENTIFY :
          /* return the device identify */
          param_ptr[0] = IO_DEV_TYPE_PHYS_SPI;
          param_ptr[1] = 0;
          param_ptr[2] = IO_DEV_ATTR_POLL | IO_DEV_ATTR_READ | IO_DEV_ATTR_WRITE;
          result = MQX_OK;  
          break;  
            
      default:
          if (polled_dev_ptr->DEV_IOCTL != NULL) {
              result = (*polled_dev_ptr->DEV_IOCTL)(polled_dev_ptr->DEV_INFO_PTR, cmd, param_ptr, polled_dev_ptr->FLAGS);
          }
          break;             
   }
   return result;
} /* Endbody */

/* EOF */
