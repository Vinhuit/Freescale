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
*   This file contains low level functions for the I2C polled device driver
*   for 54XX Coldfire family.
*
*
*END************************************************************************/


#include <mqx.h>
#include <bsp.h>
#include <io_prv.h>
#include <fio_prv.h>
#include "i2c.h"
#include "i2c_pol_prv.h"
#include "i2c_mcf54xx_prv.h"


extern uint32_t _mcf54xx_i2c_polled_init (MCF54XX_I2C_INIT_STRUCT_PTR, void **, char *);
extern uint32_t _mcf54xx_i2c_polled_deinit (IO_I2C_POLLED_DEVICE_STRUCT_PTR, VMCF54XX_I2C_INFO_STRUCT_PTR);
extern uint32_t _mcf54xx_i2c_polled_rx (IO_I2C_POLLED_DEVICE_STRUCT_PTR, unsigned char *, uint32_t);
extern uint32_t _mcf54xx_i2c_polled_tx (IO_I2C_POLLED_DEVICE_STRUCT_PTR, unsigned char *, uint32_t);
extern uint32_t _mcf54xx_i2c_polled_ioctl (VMCF54XX_I2C_INFO_STRUCT_PTR, uint32_t, uint32_t *);
static uint8_t  _mcf54xx_i2c_find_baud_index (uint32_t);
static uint32_t _mcf54xx_i2c_polled_rx_tx (VMCF54XX_I2C_STRUCT_PTR, VMCF54XX_I2C_INFO_STRUCT_PTR, unsigned char *, uint32_t);


/* I2C frequency divider values taken from the MCF5282UM.pdf, Figure 24-3. */
/* duplicated items are commented out, if commented out value is wanted, uncoment it,
   comment out duplicated and rebuilt bsp */
static const MCF54XX_I2C_BAUDRATE_STRUCT _i2c_baudrate_tbl[] = {
   {BSP_I2C_CLOCK/3840, 0x1F},
   {BSP_I2C_CLOCK/3072, 0x1E},
   {BSP_I2C_CLOCK/2560, 0x1D},
   {BSP_I2C_CLOCK/2304, 0x1C},
   {BSP_I2C_CLOCK/2048, 0x3F},
   {BSP_I2C_CLOCK/1920, 0x1B},
   {BSP_I2C_CLOCK/1792, 0x3E},
   {BSP_I2C_CLOCK/1536, 0x1A},
//   {BSP_I2C_CLOCK/1536, 0x3D},
   {BSP_I2C_CLOCK/1280, 0x19},
//   {BSP_I2C_CLOCK/1280, 0x3C},
   {BSP_I2C_CLOCK/1152, 0x18},
   {BSP_I2C_CLOCK/1024, 0x3B},
   {BSP_I2C_CLOCK/960 , 0x17},
   {BSP_I2C_CLOCK/896 , 0x3A},
   {BSP_I2C_CLOCK/768 , 0x16},
//   {BSP_I2C_CLOCK/768 , 0x39},
   {BSP_I2C_CLOCK/640 , 0x15},
//   {BSP_I2C_CLOCK/640 , 0x38},
   {BSP_I2C_CLOCK/576 , 0x14},
   {BSP_I2C_CLOCK/512 , 0x37},
   {BSP_I2C_CLOCK/480 , 0x13},
   {BSP_I2C_CLOCK/448 , 0x36},
   {BSP_I2C_CLOCK/384 , 0x12},
//   {BSP_I2C_CLOCK/384 , 0x35},
   {BSP_I2C_CLOCK/320 , 0x11},
//   {BSP_I2C_CLOCK/320 , 0x34},
   {BSP_I2C_CLOCK/288 , 0x10},
   {BSP_I2C_CLOCK/256 , 0x33},
   {BSP_I2C_CLOCK/240 , 0x0F},
   {BSP_I2C_CLOCK/224 , 0x32},
//   {BSP_I2C_CLOCK/192 , 0x31},
   {BSP_I2C_CLOCK/192 , 0x0E},
   {BSP_I2C_CLOCK/160 , 0x0D},
//   {BSP_I2C_CLOCK/160 , 0x30},
   {BSP_I2C_CLOCK/144 , 0x0C},
   {BSP_I2C_CLOCK/128 , 0x0B},
//   {BSP_I2C_CLOCK/128 , 0x2F},
   {BSP_I2C_CLOCK/112 , 0x2E},
   {BSP_I2C_CLOCK/104 , 0x0A},
   {BSP_I2C_CLOCK/96  , 0x2D},
   {BSP_I2C_CLOCK/88  , 0x09},
//   {BSP_I2C_CLOCK/80  , 0x2C},
   {BSP_I2C_CLOCK/80  , 0x08},
   {BSP_I2C_CLOCK/72  , 0x2B},
   {BSP_I2C_CLOCK/68  , 0x07},
   {BSP_I2C_CLOCK/64  , 0x2A},
   {BSP_I2C_CLOCK/56  , 0x06},
//   {BSP_I2C_CLOCK/56  , 0x29},
   {BSP_I2C_CLOCK/48  , 0x05},
//   {BSP_I2C_CLOCK/48  , 0x28},
   {BSP_I2C_CLOCK/44  , 0x04},
   {BSP_I2C_CLOCK/40  , 0x03},
//   {BSP_I2C_CLOCK/40  , 0x27},
   {BSP_I2C_CLOCK/36  , 0x26},
   {BSP_I2C_CLOCK/34  , 0x02},
   {BSP_I2C_CLOCK/32  , 0x25},
   {BSP_I2C_CLOCK/30  , 0x01},
   {BSP_I2C_CLOCK/28  , 0x00},
//   {BSP_I2C_CLOCK/28  , 0x24},
   {BSP_I2C_CLOCK/26  , 0x23},
   {BSP_I2C_CLOCK/24  , 0x22},
   {BSP_I2C_CLOCK/22  , 0x21},
   {BSP_I2C_CLOCK/20  , 0x20},
};


/*FUNCTION****************************************************************
* 
* Function Name    : _mcf54xx_i2c_find_baud_index
* Returned Value   : index into table of baudrate dividers
* Comments         :
*    Find best I2C setting for given baudrate.
*
*END*********************************************************************/

static uint8_t _mcf54xx_i2c_find_baud_index 
   (
      /* [IN] Desired baudrate */
      uint32_t input_baud
   )
{
   uint8_t low, high, i;
   
   low = 0; 
   high = sizeof(_i2c_baudrate_tbl)/sizeof(MCF54XX_I2C_BAUDRATE_STRUCT) - 1;
   do 
   {
      i = (low + high) >> 1;
      if (input_baud < _i2c_baudrate_tbl[i].BAUD_RATE)
      {
         high = i - 1;
      } else {
         low = i + 1;
      }
   } while ((_i2c_baudrate_tbl[i].BAUD_RATE != input_baud) && (high >= low));
   
   /* in case we did not found exact BAUD rate use nearest slower settings than requested */
   if ((_i2c_baudrate_tbl[i].BAUD_RATE > input_baud) && (i != 0))
   {
      i--;
   }
   
   return i;
}


/*FUNCTION****************************************************************
* 
* Function Name    : _mcf54xx_i2c_polled_install
* Returned Value   : MQX error code
* Comments         :
*    Install an I2C device.
*
*END*********************************************************************/

uint32_t _mcf54xx_i2c_polled_install
   (
      /* [IN] A string that identifies the device for fopen */
      char                     *identifier,
  
      /* [IN] The I/O init data pointer */
      MCF54XX_I2C_INIT_STRUCT_CPTR init_data_ptr
   )
{ /* Body */

   return _io_i2c_polled_install(identifier,
      (_mqx_uint (_CODE_PTR_)(void *, void **, char *))_mcf54xx_i2c_polled_init,
      (_mqx_uint (_CODE_PTR_)(void *, void *))_mcf54xx_i2c_polled_deinit,
      (_mqx_int (_CODE_PTR_)(void *, char *, _mqx_int))_mcf54xx_i2c_polled_rx,
      (_mqx_int (_CODE_PTR_)(void *, char *, _mqx_int))_mcf54xx_i2c_polled_tx,
      (_mqx_int (_CODE_PTR_)(void *, _mqx_uint, _mqx_uint_ptr))_mcf54xx_i2c_polled_ioctl, 
      (void *)init_data_ptr);

} /* Endbody */


/*FUNCTION****************************************************************
* 
* Function Name    : _mcf54xx_i2c_polled_init
* Returned Value   : MQX error code
* Comments         :
*    This function initializes an I2C device.
*
*END*********************************************************************/

uint32_t _mcf54xx_i2c_polled_init
   (
      /* [IN] Initialization information for the device being opened */
      MCF54XX_I2C_INIT_STRUCT_PTR io_init_ptr,

      /* [OUT] Address to store device specific information */
      void                  **io_info_ptr_ptr,
       
      /* [IN] The rest of the name of the device opened */
      char                    *open_name_ptr

   )
{ /* Body */
   VMCF54XX_I2C_STRUCT_PTR        i2c_ptr;
   VMCF54XX_I2C_INFO_STRUCT_PTR   io_info_ptr;

   i2c_ptr = _bsp_get_i2c_base_address (io_init_ptr->CHANNEL);
   if (NULL == i2c_ptr)
   {
        return I2C_ERROR_CHANNEL_INVALID;
   }
  
   io_info_ptr = (VMCF54XX_I2C_INFO_STRUCT_PTR)_mem_alloc_system_zero ((uint32_t)sizeof (MCF54XX_I2C_INFO_STRUCT));
   if (NULL == io_info_ptr) 
   {
      return MQX_OUT_OF_MEMORY;
   }
   _mem_set_type ((void *)io_info_ptr, MEM_TYPE_IO_I2C_INFO_STRUCT);

   /* I2C board specific initialization */
   _bsp_i2c_io_init (io_init_ptr->CHANNEL);

   *io_info_ptr_ptr                              = (void *)io_info_ptr;
   io_info_ptr->INIT                             = *io_init_ptr;
   io_info_ptr->I2C_PTR                          = i2c_ptr;
   io_info_ptr->OLD_ISR                          = NULL;
   io_info_ptr->OLD_ISR_DATA                     = NULL;
   io_info_ptr->VECTOR                           = 0;
   io_info_ptr->MODE                             = io_init_ptr->MODE;
   io_info_ptr->STATE                            = I2C_STATE_READY;
   io_info_ptr->ADDRESSEE                        = 0;
   io_info_ptr->OPERATION                        = 0;
   io_info_ptr->RX_REQUEST                       = 0;
   io_info_ptr->RX_BUFFER                        = NULL;
   io_info_ptr->RX_IN                            = 0;
   io_info_ptr->RX_OUT                           = 0;
   io_info_ptr->TX_BUFFER                        = NULL;
   io_info_ptr->TX_IN                            = 0;
   io_info_ptr->TX_OUT                           = 0;
   io_info_ptr->STATISTICS.INTERRUPTS            = 0;
   io_info_ptr->STATISTICS.RX_PACKETS            = 0;
   io_info_ptr->STATISTICS.TX_PACKETS            = 0;
   io_info_ptr->STATISTICS.TX_LOST_ARBITRATIONS  = 0;
   io_info_ptr->STATISTICS.TX_ADDRESSED_AS_SLAVE = 0;
   io_info_ptr->STATISTICS.TX_NAKS               = 0;

   /* Disable and clear I2C before initializing it */
   i2c_ptr->I2CR = 0x00;
  
   /* Clear out all I2C events */
   i2c_ptr->I2SR = 0x00;

   /* Set the station address for SLAVE receive operations */      
   i2c_ptr->I2ADR = io_init_ptr->STATION_ADDRESS << 1;
   
   /* Set the frequency divider for the nearest found baud rate */
   io_info_ptr->BAUD_INDEX = _mcf54xx_i2c_find_baud_index (io_init_ptr->BAUD_RATE);
   i2c_ptr->I2FDR = _i2c_baudrate_tbl[io_info_ptr->BAUD_INDEX].IC;
   
   /* Enable I2C - workaround - enable interrupt request flag on MCF54418 */
   i2c_ptr->I2CR |= MCF54XX_I2C_I2CR_IEN | MCF54XX_I2C_I2CR_IIEN;

   return I2C_OK;
   
} /* Endbody */


/*FUNCTION****************************************************************
* 
* Function Name    : _mcf54xx_i2c_polled_deinit
* Returned Value   : MQX error code
* Comments         :
*    This function de-initializes an I2C device.
*
*END*********************************************************************/

uint32_t _mcf54xx_i2c_polled_deinit
   (
      /* [IN] the initialization information for the device being opened */
      IO_I2C_POLLED_DEVICE_STRUCT_PTR pol_io_dev_ptr,

      /* [IN] the address of the device specific information */
      VMCF54XX_I2C_INFO_STRUCT_PTR    io_info_ptr
   )
{ /* Body */
   VMCF54XX_I2C_STRUCT_PTR            i2c_ptr;

   if ((NULL == io_info_ptr) || (NULL == pol_io_dev_ptr)) 
   {
      return I2C_ERROR_INVALID_PARAMETER;
   }
      
   i2c_ptr = io_info_ptr->I2C_PTR;
   if (i2c_ptr->I2SR & MCF54XX_I2C_I2SR_IBB) 
   {
      return I2C_ERROR_DEVICE_BUSY;
   }
   
   /* Disable the I2C */
   i2c_ptr->I2CR = 0x00;

   /* Clear the I2C events */
   i2c_ptr->I2SR = 0x00; 
 
   /* Free info struct */
   _mem_free (pol_io_dev_ptr->DEV_INFO_PTR);
   pol_io_dev_ptr->DEV_INFO_PTR = NULL;

   return MQX_OK;

} /* Endbody */

 
/*FUNCTION****************************************************************
* 
* Function Name    : _mcf54xx_i2c_polled_ioctl
* Returned Value   : MQX error code.
* Comments         : 
*    This function performs miscellaneous services for
*    the I2C I/O device.  
*
*END*********************************************************************/

uint32_t _mcf54xx_i2c_polled_ioctl
   (
      /* [IN] the address of the device specific information */
      VMCF54XX_I2C_INFO_STRUCT_PTR io_info_ptr,

      /* [IN] The command to perform */
      uint32_t                      cmd,

      /* [IN] Parameters for the command */
      uint32_t                  *param_ptr
   )
{ /* Body */
   VMCF54XX_I2C_STRUCT_PTR         i2c_ptr;
   uint32_t                         result = MQX_OK;
   volatile uint8_t                 tmp, state;
  
   i2c_ptr = io_info_ptr->I2C_PTR;
   
   switch (cmd) 
   {
      case IO_IOCTL_FLUSH_OUTPUT:
         do 
         {
            for (tmp = 0; tmp < 0xFF; tmp++) 
               { };
            tmp = i2c_ptr->I2SR;
         } while ((i2c_ptr->I2CR & MCF54XX_I2C_I2CR_MTX) && ((MCF54XX_I2C_I2SR_IBB == (tmp & (MCF54XX_I2C_I2SR_ICF | MCF54XX_I2C_I2SR_IBB))) || (io_info_ptr->TX_IN != io_info_ptr->TX_OUT)));
         io_info_ptr->TX_IN = io_info_ptr->TX_OUT;
         if ((io_info_ptr->OPERATION & I2C_OPERATION_STARTED) && (0 == (tmp & MCF54XX_I2C_I2SR_IBB)))
         {
             io_info_ptr->OPERATION = 0;
             io_info_ptr->RX_REQUEST = 0;
             io_info_ptr->STATE = I2C_STATE_FINISHED;
         }
         if (NULL != param_ptr) 
         {
            *param_ptr = tmp & MCF54XX_I2C_I2SR_RXAK;
         }
         break;
      case IO_IOCTL_I2C_REPEATED_START:
         result = I2C_ERROR_DEVICE_BUSY;
         if (i2c_ptr->I2CR & MCF54XX_I2C_I2CR_MSTA)
         {
            if (((I2C_STATE_TRANSMIT == io_info_ptr->STATE) && (io_info_ptr->TX_IN == io_info_ptr->TX_OUT))
               || ((I2C_STATE_RECEIVE == io_info_ptr->STATE) && (0 == io_info_ptr->RX_REQUEST))
               || (I2C_STATE_FINISHED == io_info_ptr->STATE))
            {
               io_info_ptr->STATE = I2C_STATE_REPEATED_START;
               result = MQX_OK;
            }
         }
         break;
      case IO_IOCTL_I2C_STOP:
         i2c_ptr->I2CR &= (~ (MCF54XX_I2C_I2CR_MTX | MCF54XX_I2C_I2CR_TXAK));
         i2c_ptr->I2SR &= (~ (MCF54XX_I2C_I2SR_IAL | MCF54XX_I2C_I2SR_IIF));
         io_info_ptr->RX_REQUEST = 0;
         io_info_ptr->OPERATION = 0;
         io_info_ptr->STATE = I2C_STATE_READY;
         io_info_ptr->RX_OUT = io_info_ptr->RX_IN;

         if (! (i2c_ptr->I2CR & MCF54XX_I2C_I2CR_MSTA))
         {
            /* As slave, release the bus */
            tmp = i2c_ptr->I2DR;
         }
         else
         {
            /* As master, send STOP condition */ 
            i2c_ptr->I2CR &= (~ MCF54XX_I2C_I2CR_MSTA);
         }
         for (tmp = 0; tmp < 0xFF; tmp++) 
            { };

         /* Wait for bus idle */
         tmp = i2c_ptr->I2SR;
         while (tmp & MCF54XX_I2C_I2SR_IBB)
         { 
            if (tmp & MCF54XX_I2C_I2SR_IIF)
            {
               i2c_ptr->I2SR &= (~ MCF54XX_I2C_I2SR_IIF);
               tmp = (uint8_t)i2c_ptr->I2DR;
            }
            tmp = i2c_ptr->I2SR;
         }
         break;
      case IO_IOCTL_I2C_SET_RX_REQUEST:
         if (NULL == param_ptr) 
         {
            result = I2C_ERROR_INVALID_PARAMETER;
         } 
         else 
         {
            io_info_ptr->RX_REQUEST = *param_ptr;
         }
         break;
      case IO_IOCTL_I2C_DISABLE_DEVICE:
         i2c_ptr->I2CR &= (~ MCF54XX_I2C_I2CR_IEN);
         break;
      case IO_IOCTL_I2C_ENABLE_DEVICE:
         i2c_ptr->I2CR |= MCF54XX_I2C_I2CR_IEN;
         break;
      case IO_IOCTL_I2C_SET_MASTER_MODE:
         if (I2C_STATE_READY != io_info_ptr->STATE) 
         {
            result = I2C_ERROR_DEVICE_BUSY;
         } 
         else 
         {
            io_info_ptr->MODE = I2C_MODE_MASTER;
         }
         break;
      case IO_IOCTL_I2C_SET_SLAVE_MODE:
         if (I2C_STATE_READY != io_info_ptr->STATE) 
         {
            result = I2C_ERROR_DEVICE_BUSY;
         } 
         else 
         {
            io_info_ptr->MODE = I2C_MODE_SLAVE;
         }
         break;
      case IO_IOCTL_I2C_GET_MODE:
         if (NULL == param_ptr) 
         {
            result = I2C_ERROR_INVALID_PARAMETER;
         } 
         else 
         {
            *param_ptr = io_info_ptr->MODE;
         }
         break;
      case IO_IOCTL_I2C_SET_BAUD:
         if (NULL == param_ptr) 
         {
            result = I2C_ERROR_INVALID_PARAMETER;
         } 
         else if (i2c_ptr->I2SR & MCF54XX_I2C_I2SR_IBB) 
         {
            result = I2C_ERROR_DEVICE_BUSY;
         } 
         else 
         {
            io_info_ptr->BAUD_INDEX = _mcf54xx_i2c_find_baud_index (*param_ptr);
            i2c_ptr->I2FDR = _i2c_baudrate_tbl[io_info_ptr->BAUD_INDEX].IC;
         }
         break;
      case IO_IOCTL_I2C_GET_BAUD:
         if (NULL == param_ptr) 
         {
            result = I2C_ERROR_INVALID_PARAMETER;
         } 
         else 
         {
            *param_ptr = _i2c_baudrate_tbl[io_info_ptr->BAUD_INDEX].BAUD_RATE;
         }
         break;
      case IO_IOCTL_I2C_SET_DESTINATION_ADDRESS:
         if (NULL == param_ptr) 
         {
            result = I2C_ERROR_INVALID_PARAMETER;
         } 
         else 
         {
            io_info_ptr->ADDRESSEE = *param_ptr;
         }
         break;
      case IO_IOCTL_I2C_GET_DESTINATION_ADDRESS:
         if (NULL == param_ptr) 
         {
            result = I2C_ERROR_INVALID_PARAMETER;
         } 
         else 
         {
            *param_ptr = io_info_ptr->ADDRESSEE;
         }
         break;
      case IO_IOCTL_I2C_SET_STATION_ADDRESS:
         if (NULL == param_ptr) 
         {
            result = I2C_ERROR_INVALID_PARAMETER;
         } 
         else 
         {
            i2c_ptr->I2ADR = (*param_ptr) << 1;
         }
         break;
      case IO_IOCTL_I2C_GET_STATION_ADDRESS:
         if (NULL == param_ptr) 
         {
            result = I2C_ERROR_INVALID_PARAMETER;
         } 
         else 
         {
            *param_ptr = i2c_ptr->I2ADR >> 1;
         }
         break;
      case IO_IOCTL_I2C_GET_STATE:
         if (NULL == param_ptr) 
         {
            result = I2C_ERROR_INVALID_PARAMETER;
         } 
         else 
         {
            *param_ptr = io_info_ptr->STATE;
         }
         break;
      case IO_IOCTL_I2C_GET_STATISTICS:
         if (NULL == param_ptr) 
         {
            result = I2C_ERROR_INVALID_PARAMETER;
         } 
         else 
         {
            *((I2C_STATISTICS_STRUCT_PTR)param_ptr) = io_info_ptr->STATISTICS;
         }
         break;         
      case IO_IOCTL_I2C_CLEAR_STATISTICS:
         io_info_ptr->STATISTICS.INTERRUPTS = 0;
         io_info_ptr->STATISTICS.RX_PACKETS = 0;
         io_info_ptr->STATISTICS.TX_PACKETS = 0;
         io_info_ptr->STATISTICS.TX_LOST_ARBITRATIONS = 0;
         io_info_ptr->STATISTICS.TX_ADDRESSED_AS_SLAVE = 0;
         io_info_ptr->STATISTICS.TX_NAKS = 0;
         break;
      case IO_IOCTL_I2C_GET_BUS_AVAILABILITY:
         if (NULL == param_ptr) 
         {
            result = I2C_ERROR_INVALID_PARAMETER;
         } 
         else 
         {
            if (i2c_ptr->I2SR & MCF54XX_I2C_I2SR_IBB) 
            {
                *param_ptr = I2C_BUS_BUSY;
            } 
            else 
            {
                *param_ptr = I2C_BUS_IDLE;
            }
         }
         break;
      default:
         return IO_ERROR_INVALID_IOCTL_CMD;
   }
   return result;
} /* Endbody */


/*FUNCTION****************************************************************
* 
* Function Name    : _mcf54xx_i2c_polled_rx
* Returned Value   : number of bytes read
* Comments         : 
*   Returns the number of bytes received.
*   Reads the data into provided array when data is available.
*
*END*********************************************************************/

uint32_t _mcf54xx_i2c_polled_rx
   (
      /* [IN] the address of the device specific information */
      IO_I2C_POLLED_DEVICE_STRUCT_PTR pol_io_dev_ptr,

      /* [IN] The array to copy data into */
      unsigned char                       *buffer,
      
      /* [IN] number of bytes to read */
      uint32_t                         length
   )
{ /* Body */
   VMCF54XX_I2C_INFO_STRUCT_PTR       io_info_ptr;
   VMCF54XX_I2C_STRUCT_PTR            i2c_ptr;   
   uint8_t                             tmp;
         
   io_info_ptr = pol_io_dev_ptr->DEV_INFO_PTR;
   i2c_ptr = io_info_ptr->I2C_PTR;

   /* If beginning of transmission, set state and send address (master only) */
   io_info_ptr->OPERATION |= I2C_OPERATION_READ;
   tmp = io_info_ptr->STATE;
   if ((I2C_STATE_READY == tmp) || (I2C_STATE_REPEATED_START == tmp))
   {
      io_info_ptr->STATE = I2C_STATE_RECEIVE;
      if (I2C_MODE_MASTER == io_info_ptr->MODE)
      {
         i2c_ptr->I2CR |= MCF54XX_I2C_I2CR_MTX;
         i2c_ptr->I2SR &= (~ MCF54XX_I2C_I2SR_IIF);
         if (I2C_STATE_REPEATED_START == tmp)
         {
            i2c_ptr->I2CR |= MCF54XX_I2C_I2CR_RSTA;
         }
         else
         {
            i2c_ptr->I2CR |= MCF54XX_I2C_I2CR_MSTA;
         }
         io_info_ptr->OPERATION |= I2C_OPERATION_STARTED;
         i2c_ptr->I2DR = (io_info_ptr->ADDRESSEE << 1) | I2C_OPERATION_READ;
         io_info_ptr->STATISTICS.TX_PACKETS++;
         while (0 == (i2c_ptr->I2SR & MCF54XX_I2C_I2SR_IIF)) 
            { };   /* wait for address transferred */
      }
   }

   return _mcf54xx_i2c_polled_rx_tx (i2c_ptr, io_info_ptr, buffer, length);
} /* Endbody */


/*FUNCTION****************************************************************
* 
* Function Name    : _mcf54xx_i2c_polled_tx
* Returned Value   : number of bytes transmitted
* Comments         : 
*   Writes the provided data buffer and loops until transmission complete.
*
*END*********************************************************************/

uint32_t _mcf54xx_i2c_polled_tx
   (
      /* [IN] the address of the device specific information */
      IO_I2C_POLLED_DEVICE_STRUCT_PTR pol_io_dev_ptr,

      /* [IN] The array characters are to be read from */
      unsigned char                       *buffer,
      
      /* [IN] number of bytes to output */
      uint32_t                         length
   )
{ /* Body */
   VMCF54XX_I2C_INFO_STRUCT_PTR       io_info_ptr;
   VMCF54XX_I2C_STRUCT_PTR            i2c_ptr;   
   uint8_t                             tmp;
   
   io_info_ptr  = pol_io_dev_ptr->DEV_INFO_PTR;
   i2c_ptr = io_info_ptr->I2C_PTR;

   /* If beginning of transmission, set state and send address (master only) */
   io_info_ptr->OPERATION &= (~ I2C_OPERATION_READ);
   tmp = io_info_ptr->STATE;
   if ((I2C_STATE_READY == tmp) || (I2C_STATE_REPEATED_START == tmp))
   {
      io_info_ptr->STATE = I2C_STATE_TRANSMIT;
      if (I2C_MODE_MASTER == io_info_ptr->MODE)
      {
         i2c_ptr->I2CR |= MCF54XX_I2C_I2CR_MTX;
         i2c_ptr->I2SR &= (~ MCF54XX_I2C_I2SR_IIF);
         if (I2C_STATE_REPEATED_START == tmp)
         {
            i2c_ptr->I2CR |= MCF54XX_I2C_I2CR_RSTA;
         }
         else
         {
            i2c_ptr->I2CR |= MCF54XX_I2C_I2CR_MSTA;
         }
         io_info_ptr->OPERATION |= I2C_OPERATION_STARTED;
         i2c_ptr->I2DR = (io_info_ptr->ADDRESSEE << 1) | I2C_OPERATION_WRITE;
         io_info_ptr->STATISTICS.TX_PACKETS++;
         while (0 == (i2c_ptr->I2SR & MCF54XX_I2C_I2SR_IIF))
            { };   /* wait for address transferred */
      }
   }

   return _mcf54xx_i2c_polled_rx_tx (i2c_ptr, io_info_ptr, buffer, length);
} /* Endbody */


/*FUNCTION****************************************************************
* 
* Function Name    : _mcf5xx_i2c_polled_rx_tx
* Returned Value   : number of bytes processed
* Comments         : 
*   Actual data transfer on I2C bus.
*
*END*********************************************************************/

static uint32_t _mcf54xx_i2c_polled_rx_tx
   (
      /* [IN] I2C register structure */
      VMCF54XX_I2C_STRUCT_PTR      i2c_ptr,

      /* [IN] I2C state structure */
      VMCF54XX_I2C_INFO_STRUCT_PTR io_info_ptr,
      
      /* [IN] The buffer for IO operation */
      unsigned char                    *buffer,
      
      /* [IN] Number of bytes in buffer */
      uint32_t                      length
   )
{ /* Body */
   uint32_t                         i;
   uint8_t                          i2csr;
   volatile uint8_t                 tmp;

   for (i = 0; i <= length; i++)
   {
      do 
      {
         i2csr = i2c_ptr->I2SR;
         if (0 == (i2c_ptr->I2CR & MCF54XX_I2C_I2CR_MSTA))
         {
            if (i == length)
            {
               return i;
            }
            if ((io_info_ptr->OPERATION & I2C_OPERATION_STARTED) && (0 == (i2csr & MCF54XX_I2C_I2SR_IBB)))
            {
               io_info_ptr->OPERATION = 0;
               io_info_ptr->RX_REQUEST = 0;
               io_info_ptr->STATE = I2C_STATE_FINISHED;
               return i;
            }
         }
      } while (0 == (i2csr & MCF54XX_I2C_I2SR_IIF));

      io_info_ptr->OPERATION |= I2C_OPERATION_STARTED;
   
      /* Master */
      if (i2c_ptr->I2CR & MCF54XX_I2C_I2CR_MSTA)
      {
         /* Transmit */
         if (i2c_ptr->I2CR & MCF54XX_I2C_I2CR_MTX)
         {
            /* Not ack */
            if (i2csr & MCF54XX_I2C_I2SR_RXAK)
            {
               i2c_ptr->I2SR &= (~ MCF54XX_I2C_I2SR_IIF);
               io_info_ptr->STATE = I2C_STATE_FINISHED;
               io_info_ptr->STATISTICS.TX_NAKS++;
               length = i;
            }
            /* Ack */
            else
            {
               /* Transmit requested */
               if (I2C_STATE_TRANSMIT == io_info_ptr->STATE)
               {
                  if (0 == (io_info_ptr->OPERATION & I2C_OPERATION_READ))
                  {
                     /* Anything to transmit? */
                     if (i < length)
                     {
                        i2c_ptr->I2SR &= (~ MCF54XX_I2C_I2SR_IIF);
                        i2c_ptr->I2DR = *buffer++;   /* transmit data */
                        io_info_ptr->STATISTICS.TX_PACKETS++;
                     }
                  }
                  else
                  {
                     length = i;
                  }
               }
               /* Receive requested */
               else if (I2C_STATE_RECEIVE == io_info_ptr->STATE)
               {
                  if (0 != (io_info_ptr->OPERATION & I2C_OPERATION_READ))
                  {
                     i2c_ptr->I2CR &= (~ MCF54XX_I2C_I2CR_MTX);
                     if (1 == io_info_ptr->RX_REQUEST)
                     {
                        i2c_ptr->I2CR |= MCF54XX_I2C_I2CR_TXAK;
                     }
                     else
                     {
                        i2c_ptr->I2CR &= (~ MCF54XX_I2C_I2CR_TXAK);
                     }
                     i2c_ptr->I2SR &= (~ MCF54XX_I2C_I2SR_IIF);
                     if (0 == io_info_ptr->RX_REQUEST)
                     {
                        io_info_ptr->STATE = I2C_STATE_FINISHED;
                        length = i;
                     }
                     else
                     {
                        tmp = i2c_ptr->I2DR;   /* dummy read to clock in 1st byte */
                        i--;
                     }
                  }
                  else
                  {
                     length = i;
                  }
               }
               /* Others are unwanted/not handled states */
               else
               {
                  length = i;
               }
            }
         }
         /* Receive */
         else
         {
            /* Receive requested */
            if (I2C_STATE_RECEIVE == io_info_ptr->STATE)
            {
               if ((0 != (io_info_ptr->OPERATION & I2C_OPERATION_READ)) && (0 != io_info_ptr->RX_REQUEST))
               {
                  if (i < length)
                  {
                     i2c_ptr->I2SR &= (~ MCF54XX_I2C_I2SR_IIF);
                     io_info_ptr->RX_REQUEST--;
                     if (1 == io_info_ptr->RX_REQUEST)
                     {
                        i2c_ptr->I2CR |= MCF54XX_I2C_I2CR_TXAK;
                     }
                     else
                     {
                        i2c_ptr->I2CR &= (~ MCF54XX_I2C_I2CR_TXAK);
                     }
                     if (0 == io_info_ptr->RX_REQUEST)
                     {
                        i2c_ptr->I2CR |= MCF54XX_I2C_I2CR_MTX;  /* no more reading */
                     }
                     *buffer++ = i2c_ptr->I2DR;   /* receive data */
                     if (0 == io_info_ptr->RX_REQUEST)
                     {
                        i2c_ptr->I2CR &= (~ MCF54XX_I2C_I2CR_MTX);
                        io_info_ptr->STATE = I2C_STATE_FINISHED;
                        length = i + 1;
                        i = length;
                     }
                     io_info_ptr->STATISTICS.RX_PACKETS++;
                  }
               }
               else
               {
                  length = i;
               }
            }
            /* Others are unwanted/not handled states */
            else 
            {
               length = i;
            }
         }
      }
      /* Slave */
      else
      {
         /* Master arbitration lost */
         if (i2csr & MCF54XX_I2C_I2SR_IAL)
         {
            i2c_ptr->I2SR &= (~ MCF54XX_I2C_I2SR_IAL);
            io_info_ptr->STATE = I2C_STATE_LOST_ARBITRATION;
            io_info_ptr->STATISTICS.TX_LOST_ARBITRATIONS++;
            length = i;
         }
         /* Addressed as slave */
         if (i2csr & MCF54XX_I2C_I2SR_IAAS)
         {
            if (I2C_MODE_MASTER == io_info_ptr->MODE)
            {
               io_info_ptr->STATISTICS.TX_ADDRESSED_AS_SLAVE++;
            }
            /* Transmit requested */
            if (i2csr & MCF54XX_I2C_I2SR_SRW)
            {  
               io_info_ptr->STATE = I2C_STATE_ADDRESSED_AS_SLAVE_TX;
               if (0 == (io_info_ptr->OPERATION & I2C_OPERATION_READ))
               {
                  if (i < length)
                  {
                     i2c_ptr->I2CR |= MCF54XX_I2C_I2CR_MTX;
                     i2c_ptr->I2SR &= (~ MCF54XX_I2C_I2SR_IIF);
                     i2c_ptr->I2DR = *buffer++;   /* transmit data */
                     io_info_ptr->STATISTICS.TX_PACKETS++;
                  }
               }
               else
               {
                  length = i;
               }
            }
            /* Receive requested */
            else
            {
               io_info_ptr->STATE = I2C_STATE_ADDRESSED_AS_SLAVE_RX;
               if ((0 != (io_info_ptr->OPERATION & I2C_OPERATION_READ)) && (i == 0) && (0 != io_info_ptr->RX_REQUEST))
               {
                  i2c_ptr->I2CR &= (~ MCF54XX_I2C_I2CR_MTX);
                  i2c_ptr->I2SR &= (~ MCF54XX_I2C_I2SR_IIF);
                  if (1 == io_info_ptr->RX_REQUEST)
                  {
                     i2c_ptr->I2CR |= MCF54XX_I2C_I2CR_TXAK;
                  }
                  else
                  {
                     i2c_ptr->I2CR &= (~ MCF54XX_I2C_I2CR_TXAK);
                  }
                  tmp = i2c_ptr->I2DR;   /* dummy read to release bus */
                  i--;
               }
               else
               {
                  length = i;
               }
            }
         }
         /* Normal slave operation */
         else
         {
            /* No master arbitration lost */
            if (! (i2csr & MCF54XX_I2C_I2SR_IAL))
            {
               /* Transmit */
               if (i2c_ptr->I2CR & MCF54XX_I2C_I2CR_MTX)
               {
                  /* Not ack */
                  if (i2csr & MCF54XX_I2C_I2SR_RXAK)
                  {
                     i2c_ptr->I2SR &= (~ MCF54XX_I2C_I2SR_IIF);
                     io_info_ptr->STATE = I2C_STATE_FINISHED;
                     io_info_ptr->STATISTICS.TX_NAKS++;
                     length = i;
                  }
                  /* Ack */
                  else
                  {
                     /* Transmit requested */
                     if (((I2C_STATE_TRANSMIT == io_info_ptr->STATE) || (I2C_STATE_ADDRESSED_AS_SLAVE_TX == io_info_ptr->STATE))
                        && (0 == (io_info_ptr->OPERATION & I2C_OPERATION_READ)))
                     {
                        if (i < length)
                        {
                           i2c_ptr->I2SR &= (~ MCF54XX_I2C_I2SR_IIF);
                           i2c_ptr->I2DR = *buffer++;   /* transmit data */
                           io_info_ptr->STATISTICS.TX_PACKETS++;
                        }
                     }
                     else
                     {
                        length = i;
                     }
                  }
               }
               /* Receive */
               else
               {
                  /* Receive requested */
                  if (((I2C_STATE_RECEIVE == io_info_ptr->STATE) || (I2C_STATE_ADDRESSED_AS_SLAVE_RX == io_info_ptr->STATE))
                     && (0 != (io_info_ptr->OPERATION & I2C_OPERATION_READ)) && (0 != io_info_ptr->RX_REQUEST))
                  {
                     if (i < length)
                     {
                        i2c_ptr->I2SR &= (~ MCF54XX_I2C_I2SR_IIF);
                        io_info_ptr->RX_REQUEST--;
                        if (1 == io_info_ptr->RX_REQUEST)
                        {
                           i2c_ptr->I2CR |= MCF54XX_I2C_I2CR_TXAK;
                        }
                        else
                        {
                           i2c_ptr->I2CR &= (~ MCF54XX_I2C_I2CR_TXAK);
                        }
                        if (0 == io_info_ptr->RX_REQUEST)
                        {
                           io_info_ptr->STATE = I2C_STATE_FINISHED;
                        }
                        *buffer++ = i2c_ptr->I2DR;   /* receive data */
                        io_info_ptr->STATISTICS.RX_PACKETS++;
                     }
                  }
                  else
                  {
                     length = i;
                  }
               }
            }
            else
            {
               length = i;
            }
         }
      }
   }
   return length;
} /* Endbody */

/* EOF */
