#ifndef __spi_h__
#define __spi_h__
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
**                            CONSTANT DEFINITIONS
*/

/* 
** IOCTL calls specific to SPI 
*/
#define IO_IOCTL_SPI_GET_BAUD              _IO(IO_TYPE_SPI,0x01)
#define IO_IOCTL_SPI_SET_BAUD              _IO(IO_TYPE_SPI,0x02)
#define IO_IOCTL_SPI_GET_MODE              _IO(IO_TYPE_SPI,0x03)
#define IO_IOCTL_SPI_SET_MODE              _IO(IO_TYPE_SPI,0x04)
#define IO_IOCTL_SPI_ENABLE_MODF           _IO(IO_TYPE_SPI,0x05)
#define IO_IOCTL_SPI_DISABLE_MODF          _IO(IO_TYPE_SPI,0x06)
#define IO_IOCTL_SPI_GET_TRANSFER_MODE     _IO(IO_TYPE_SPI,0x07)
#define IO_IOCTL_SPI_SET_TRANSFER_MODE     _IO(IO_TYPE_SPI,0x08)
#define IO_IOCTL_SPI_GET_ENDIAN            _IO(IO_TYPE_SPI,0x09)
#define IO_IOCTL_SPI_SET_ENDIAN            _IO(IO_TYPE_SPI,0x0A)
#define IO_IOCTL_SPI_DEVICE_ENABLE         _IO(IO_TYPE_SPI,0x0B)
#define IO_IOCTL_SPI_DEVICE_DISABLE        _IO(IO_TYPE_SPI,0x0C)
#define IO_IOCTL_SPI_GET_FLAGS             _IO(IO_TYPE_SPI,0x0D)
#define IO_IOCTL_SPI_SET_FLAGS             _IO(IO_TYPE_SPI,0x0E)
#define IO_IOCTL_SPI_GET_STATS             _IO(IO_TYPE_SPI,0x0F)
#define IO_IOCTL_SPI_CLEAR_STATS           _IO(IO_TYPE_SPI,0x10)
#define IO_IOCTL_SPI_GET_FRAMESIZE         _IO(IO_TYPE_SPI,0x11)
#define IO_IOCTL_SPI_SET_FRAMESIZE         _IO(IO_TYPE_SPI,0x12)
#define IO_IOCTL_SPI_GET_CS                _IO(IO_TYPE_SPI,0x13)
#define IO_IOCTL_SPI_SET_CS                _IO(IO_TYPE_SPI,0x14)
#define IO_IOCTL_SPI_SET_CS_CALLBACK       _IO(IO_TYPE_SPI,0x15)
#define IO_IOCTL_SPI_READ_WRITE            _IO(IO_TYPE_SPI,0x16)
#define IO_IOCTL_SPI_FLUSH_DEASSERT_CS     _IO(IO_TYPE_SPI,0x17)
#define IO_IOCTL_SPI_KEEP_QSPI_CS_ACTIVE   _IO(IO_TYPE_SPI,0x18)

/* 
** SPI device modes of operation 
*/
#define SPI_DEVICE_MASTER_MODE             (0x00) 
#define SPI_DEVICE_SLAVE_MODE              (0x01) 
#define SPI_DEVICE_BIO_MASTER_MODE         (0x02) 
#define SPI_DEVICE_BIO_SLAVE_MODE          (0x03)

/*
** SPI data bits transferring format
*/
#define SPI_DEVICE_BIG_ENDIAN              (0x0001)
#define SPI_DEVICE_LITTLE_ENDIAN           (0x8000)

/*
** SPI clock polarity and phase
*/
#define SPI_CLK_POL_PHA_MODE0              (0x00)   /* Inactive SPICLK low & sample rising */
#define SPI_CLK_POL_PHA_MODE1              (0x01)   /* Inactive SPICLK low & sample falling */
#define SPI_CLK_POL_PHA_MODE2              (0x02)   /* Inactive SPICLK high & sample falling */
#define SPI_CLK_POL_PHA_MODE3              (0x03)   /* Inactive SPICLK high & sample rising */

/*
** SPI Open Flags
*/
#define SPI_FLAG_HALF_DUPLEX               (0 << IO_O_USER_SHIFT)
#define SPI_FLAG_FULL_DUPLEX               (1 << IO_O_USER_SHIFT)
#define SPI_FLAG_NO_DEASSERT_ON_FLUSH      (2 << IO_O_USER_SHIFT)

/* SPI Error Codes */
#define SPI_OK                             (0x00)
#define SPI_ERROR_MODE_INVALID             (SPI_ERROR_BASE | 0x01)
#define SPI_ERROR_TRANSFER_MODE_INVALID    (SPI_ERROR_BASE | 0x02)
#define SPI_ERROR_BAUD_RATE_INVALID        (SPI_ERROR_BASE | 0x03)
#define SPI_ERROR_ENDIAN_INVALID           (SPI_ERROR_BASE | 0x04)
#define SPI_ERROR_CHANNEL_INVALID          (SPI_ERROR_BASE | 0x05)
#define SPI_ERROR_DEINIT_FAILED            (SPI_ERROR_BASE | 0x06)
#define SPI_ERROR_INVALID_PARAMETER        (SPI_ERROR_BASE | 0x07)
 
/*--------------------------------------------------------------------------*/
/*
**                    DATATYPE DECLARATIONS
*/
 
typedef struct spi_statistics_struct 
{
   /* Number of SPI interrupts so far */
   uint32_t                           INTERRUPTS;

   /* Number of valid bytes received (not dummy receives) */
   uint32_t                           RX_PACKETS;

   /* Number of times data was rewritten in input HW buffer */
   uint32_t                           RX_OVERFLOWS;

   /* Number of valid bytes transmitted (not dummy transmits) */
   uint32_t                           TX_PACKETS;

   /* Number of master transmit aborts (master to master failure) */
   uint32_t                           TX_ABORTS;

   /* Number of times data was not suplied before transfer (slave) */
   uint32_t                           TX_UNDERFLOWS;
   
} SPI_STATISTICS_STRUCT, * SPI_STATISTICS_STRUCT_PTR;
 

/*
** SPI_CS_CALLBACK
**
** This callback function is used to provide external CS handling funtionality.
*/

typedef void (_CODE_PTR_ SPI_CS_CALLBACK)(uint32_t cs_mask, uint32_t logic_level, void *user_data);


/*
** SPI_CS_CALLBACK_STRUCT
**
** This structure defines the parameters of the external chip select callback.
*/

typedef struct spi_cs_callback_struct
{
   /* The SPI CS mask */
   uint32_t          MASK;

   /* The SPI callback itself */
   SPI_CS_CALLBACK  CALLBACK;

   /* User data */
   void            *USERDATA;

} SPI_CS_CALLBACK_STRUCT, * SPI_CS_CALLBACK_STRUCT_PTR;


/*
** SPI_READ_WRITE_STRUCT
**
** This structure defines the parameters of the external chip select callback.
*/

typedef struct spi_read_write_struct
{
   /* Byte length of each buffer */
   uint32_t          BUFFER_LENGTH;

   /* Buffer containing bytes to write */
   char         *WRITE_BUFFER;

   /* Buffer containing bytes read after transfer */
   char         *READ_BUFFER;

} SPI_READ_WRITE_STRUCT, * SPI_READ_WRITE_STRUCT_PTR;
 
 
/*--------------------------------------------------------------------------*/
/* 
**                        FUNCTION PROTOTYPES
*/

#ifdef __cplusplus
extern "C" {
#endif


extern _mqx_uint _io_spi_polled_install(
      char              *identifier,
      _mqx_uint (_CODE_PTR_ init)(void *, void **, char *),
      _mqx_uint (_CODE_PTR_ deinit)(void *, void *),
      _mqx_int (_CODE_PTR_ recv)(void *, char *, _mqx_int),
      _mqx_int (_CODE_PTR_ xmit)(void *, char *, _mqx_int),
      _mqx_int (_CODE_PTR_ ioctl)(void *, _mqx_uint, _mqx_uint_ptr, _mqx_uint),
      void                 *init_data_ptr
      
   );

extern _mqx_uint _io_spi_int_install(
      char              *identifier,
      _mqx_uint (_CODE_PTR_ init)(void *, char *),
      _mqx_uint (_CODE_PTR_ enable_ints)(void *),
      _mqx_uint (_CODE_PTR_ deinit)(void *, void *),
      _mqx_int (_CODE_PTR_ recv)(void *, char *, _mqx_int),
      _mqx_int (_CODE_PTR_ xmit)(void *, char *, _mqx_int),
      _mqx_int (_CODE_PTR_ ioctl)(void *, _mqx_uint, _mqx_uint_ptr, _mqx_uint),
      void                 *init_data_ptr
      
   );


#ifdef __cplusplus
}
#endif


#endif 

/* EOF */
