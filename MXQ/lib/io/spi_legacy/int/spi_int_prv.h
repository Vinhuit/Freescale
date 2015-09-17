#ifndef __spi_int_prv_h__
#define __spi_int_prv_h__
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
*   This file includes the private definitions for the interrupt SPI
*   I/O drivers.
*
*
*END************************************************************************/

/*--------------------------------------------------------------------------*/
/*
**                            DATATYPE DECLARATIONS
*/

/*---------------------------------------------------------------------
**
** IO SPI INT DEVICE STRUCT
**
** This structure used to store information about an SPI interrupt io device
** for the IO device table
*/
typedef struct io_spi_int_device_struct
{

   /* The I/O init function */
   _mqx_uint (_CODE_PTR_ DEV_INIT)(void *, char *);

   /* The I/O deinit function */
   _mqx_uint (_CODE_PTR_ DEV_DEINIT)(void *, void *);
   
   /* The enable interrupts function */
   _mqx_uint (_CODE_PTR_ DEV_ENABLE_INTS)(void *);
   
   /* [IN] The READ function */
   _mqx_int (_CODE_PTR_  DEV_READ)(void *, char *, _mqx_int);
      
   /* [IN] The WRITE function */
   _mqx_int (_CODE_PTR_  DEV_WRITE)(void *, char *, _mqx_int);
   
   /* The ioctl function, (change bauds etc) */
   _mqx_int (_CODE_PTR_  DEV_IOCTL)(void *, _mqx_uint, _mqx_uint_ptr, _mqx_uint);

   /* The I/O channel initialization data */
   void                 *DEV_INIT_DATA_PTR;
   
   /* Device specific information */
   void                 *DEV_INFO_PTR;

   /* Open count for number of accessing file descriptors */
   uint32_t               COUNT;

   /* Open flags for this channel */
   uint32_t               FLAGS;
   
} IO_SPI_INT_DEVICE_STRUCT, * IO_SPI_INT_DEVICE_STRUCT_PTR;

/*--------------------------------------------------------------------------*/
/* 
**                        FUNCTION PROTOTYPES
*/

#ifdef __cplusplus
extern "C" {
#endif

extern _mqx_int _io_spi_int_open(FILE_DEVICE_STRUCT_PTR, char *, char *);
extern _mqx_int _io_spi_int_close(FILE_DEVICE_STRUCT_PTR);
extern _mqx_int _io_spi_int_read(FILE_DEVICE_STRUCT_PTR, char *, _mqx_int);
extern _mqx_int _io_spi_int_write(FILE_DEVICE_STRUCT_PTR, char *, _mqx_int);
extern _mqx_int _io_spi_int_ioctl(FILE_DEVICE_STRUCT_PTR, _mqx_uint, void *);
extern _mqx_int _io_spi_int_uninstall(IO_DEVICE_STRUCT_PTR);

#ifdef __cplusplus
}
#endif

#endif

/* EOF */
