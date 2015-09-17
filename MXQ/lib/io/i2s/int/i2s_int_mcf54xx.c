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
*   This file contains low level functions for the I2s interrupt device driver
*   for 54XX Coldfire family.
*
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <lwevent.h>
#include "i2s.h"
#include "i2s_int_prv.h"
#include "i2s_mcf54xx_prv.h"

uint32_t _mcf54xx_i2s_int_init (IO_I2S_DEVICE_STRUCT_PTR, char *);
uint32_t _mcf54xx_i2s_int_deinit (IO_I2S_DEVICE_STRUCT_PTR, MCF54XX_I2S_INFO_STRUCT_PTR);
uint32_t _mcf54xx_i2s_int_rx (IO_I2S_DEVICE_STRUCT_PTR, char *, uint32_t);
uint32_t _mcf54xx_i2s_int_tx (IO_I2S_DEVICE_STRUCT_PTR, char *, uint32_t);
uint32_t _mcf54xx_i2s_int_ioctl (MCF54XX_I2S_INFO_STRUCT_PTR io_info_ptr, uint32_t cmd, void *param_ptr);
static void _mcf54xx_i2s_int_isr (void *);
static uint32_t _mcf54xx_i2s_format_in_data(char *input, AUDIO_DATA_FORMAT_PTR format);
static void _mcf54xx_i2s_format_out_data(uint32_t input, char *output, AUDIO_DATA_FORMAT_PTR format);
static uint32_t _mcf54xx_i2s_sine_wave(uint32_t fs, uint32_t freq, int16_t **output);

/*
Table 35-34. SSI Control Bits Requiring SSI to be Disabled Before Change
-------------------------------------------
|Control register   |       Bit                     |
-------------------------------------------
|                       |[9]=CIS                    |
|                       |[8]=TCH                    |
|   SSIn_CR             |[7]=MCE                    |
|                       |[6:5]=I2S                  |
|                       |[4]=SYN                    |
|                       |[3]=NET                    |
-------------------------------------------
|   SSIn_IER            |[22]=RDMAE                 |
|                       |[20]=TDMAE                 |
-------------------------------------------
|                       |[9]=RXBIT0 and TXBIT0      |
|                       |[8]=RFEN1 and TFEN1        |
|                       |[7]=RFEN0 and TFEN0        |
|   SSIn_RCR            |[6]=TFDIR                  |
|   SSIn_TCR            |[5]=RXDIR and TXDIR        |
|                       |[4]=RSHFD and TSHFD        |
|                       |[3]=RSCKP and TSCKP        |
|                       |[2]=RFSI and TFSI          |
|                       |[1]=RFSL and TFSL          |
|                       |[0]=REFS and TEFS          |
-------------------------------------------
|   SSIn_CCR            |[16:13]=WL                 |
-------------------------------------------
|   SSIn_ACR            |[1]=FV                     |
|                       |[10:5]=FRDIV               |
-------------------------------------------

*/

/*FUNCTION****************************************************************
*
* Function Name    : _mcf54xx_i2s_int_install
* Returned Value   : MQX error code
* Comments         :
*    Install an I2S device.
*
*END*********************************************************************/

uint32_t _mcf54xx_i2s_int_install
(
    /* [IN] A string that identifies the device for fopen */
    char                        *identifier,

    /* [IN] The I/O init data pointer */
    MCF54XX_I2S_INIT_STRUCT_CPTR    init_data_ptr
)
{ /* Body */

    return _io_i2s_install(identifier,
        (uint32_t (_CODE_PTR_)(void *, char *))_mcf54xx_i2s_int_init,
        (uint32_t (_CODE_PTR_)(void *, void *))_mcf54xx_i2s_int_deinit,
        (int32_t (_CODE_PTR_)(void *, char *, int32_t))_mcf54xx_i2s_int_rx,
        (int32_t (_CODE_PTR_)(void *, char *, int32_t))_mcf54xx_i2s_int_tx,
        (int32_t (_CODE_PTR_)(void *, uint32_t, uint32_t *))_mcf54xx_i2s_int_ioctl,
        (void *)init_data_ptr);

} /* Endbody */


/*FUNCTION****************************************************************
*
* Function Name    : _mcf54xx_i2s_int_init
* Returned Value   : MQX error code
* Comments         :
*    This function initializes an I2S device.
*
*END*********************************************************************/

uint32_t _mcf54xx_i2s_int_init
(
    /* [IN] Initialization information for the device being opened */
    IO_I2S_DEVICE_STRUCT_PTR    io_i2s_dev_ptr,

    /* [IN] The rest of the name of the device opened */
    char                    *open_name_ptr
)
{ /* Body */
    VMCF5441_SSI_STRUCT_PTR         ssi_reg_ptr;
    MCF54XX_I2S_INIT_STRUCT_PTR     i2s_init_ptr;
    MCF54XX_I2S_INFO_STRUCT_PTR     io_info_ptr;
    uint8_t channel, mode, data_bits, clk_source, endianity, aligment, bits, size, channels;
    uint32_t     vector, buffer_size;
    _mqx_uint result = 0;

    /*Variables initialization*/
    i2s_init_ptr = (MCF54XX_I2S_INIT_STRUCT_PTR)(io_i2s_dev_ptr->DEV_INIT_DATA_PTR);
    if (i2s_init_ptr == NULL)
    {
        #ifdef I2S_DEBUG
        printf("_i2s_init: Error - Invalid init struct pointer.\n");
        #endif
        return(I2S_ERROR_INVALID_PARAMETER);
    }
    else
    {
        channel     = i2s_init_ptr->CHANNEL;
        mode        = i2s_init_ptr->MODE;
        data_bits   = i2s_init_ptr->DATA_BITS;
        clk_source  = i2s_init_ptr->CLOCK_SOURCE;
        buffer_size = i2s_init_ptr->BUFFER_SIZE;
        endianity   = i2s_init_ptr->IO_FORMAT->ENDIAN;
        aligment    = i2s_init_ptr->IO_FORMAT->ALIGNMENT;
        bits        = i2s_init_ptr->IO_FORMAT->BITS;
        size        = i2s_init_ptr->IO_FORMAT->SIZE;
        channels    = i2s_init_ptr->IO_FORMAT->CHANNELS;
    }

    if ((data_bits < 8) || (data_bits > 24) || (data_bits == 14) || (data_bits % 2))
    {
        #ifdef I2S_DEBUG
        printf("_i2s_init: Error - Unsupported data word length.\n");
        #endif
        return (I2S_ERROR_WORD_LENGTH_UNSUPPORTED);
    }

    if ((channel > 1) || (channel < 0))
    {
        #ifdef I2S_DEBUG
        printf("_i2s_init: Error - Invalid I2S channel.\n");
        #endif
        return(I2S_ERROR_CHANNEL_INVALID);
    }

    if (mode > I2S_MODE_SLAVE)
    {
        #ifdef I2S_DEBUG
        printf("_i2s_init: Error - Invalid I2S mode.\n");
        #endif
        return(I2S_ERROR_MODE_INVALID);
    }
    if (clk_source > I2S_CLK_EXT)
    {
        #ifdef I2S_DEBUG
        printf("_i2s_init: Error - Invalid I2S clock source.\n");
        #endif
        return(I2S_ERROR_CLK_INVALID);
    }
    if (buffer_size < 2)
    {
        #ifdef I2S_DEBUG
        printf("_i2s_init: Error - I2S transmit buffer size too small.\n");
        #endif
        return(I2S_ERROR_BUFFER_SMALL);
    }
    if ((aligment > AUDIO_ALIGNMENT_LEFT) || (bits < AUDIO_BIT_SIZE_MIN) ||
        (bits > AUDIO_BIT_SIZE_MAX) || (endianity > AUDIO_LITTLE_ENDIAN) ||
        (size * I2S_CHAR_BIT < bits) || (channels > 2) || (channels == 0))
    {
        #ifdef I2S_DEBUG
        printf("_i2s_init: Error - Invalid I/O data format.\n");
        #endif
        return(AUDIO_ERROR_INVALID_IO_FORMAT);
    }
    ssi_reg_ptr = (VMCF5441_SSI_STRUCT_PTR) _bsp_get_i2s_base_address(channel);
    io_info_ptr = (MCF54XX_I2S_INFO_STRUCT_PTR) _mem_alloc_system_zero ((uint32_t)sizeof (MCF54XX_I2S_INFO_STRUCT));
    if (NULL == io_info_ptr)
    {
        return MQX_OUT_OF_MEMORY;
    }
    _mem_set_type (io_info_ptr, MEM_TYPE_IO_I2S_INFO_STRUCT);

    /* I2S board specific IO pins initialization */
    _bsp_i2s_io_init(channel, mode, clk_source);

    /* I2S board specific registers initialization */
    _bsp_i2s_hw_init(i2s_init_ptr, ssi_reg_ptr);

    /*Info struct setup*/
    io_info_ptr->INIT           = *i2s_init_ptr;
    io_info_ptr->I2S_PTR        = ssi_reg_ptr;
    io_info_ptr->MODE           = i2s_init_ptr->MODE;
    io_info_ptr->DATA_BITS      = i2s_init_ptr->DATA_BITS;
    io_info_ptr->CLOCK_SOURCE   = i2s_init_ptr->CLOCK_SOURCE;
    if (io_info_ptr->CLOCK_SOURCE == I2S_CLK_INT)
    {
        io_info_ptr->MCLK_FREQ = BSP_SYSTEM_CLOCK / SSIDIV_MIN;
        io_info_ptr->BCLK_FREQ = io_info_ptr->MCLK_FREQ / 4; /* DIV2 = 1 in default and 2 is inner division
        for further info see MCF5441X reference manual, page 51 in chapter 35*/
        io_info_ptr->FS_FREQ = io_info_ptr->BCLK_FREQ / io_info_ptr->DATA_BITS;
    }
    else
    {
        io_info_ptr->MCLK_FREQ  = 0;
        io_info_ptr->BCLK_FREQ  = 0;
        io_info_ptr->FS_FREQ    = 0;
    }
    io_info_ptr->MCLK_DIV           = SSIDIV_MIN;
    io_info_ptr->BCLK_DIV           = 4;
    io_info_ptr->TX_DUMMY           = i2s_init_ptr->TX_DUMMY;
    io_info_ptr->ONTHEWAY           = FALSE;
    io_info_ptr->BUFFER_L.IN        = 0;
    io_info_ptr->BUFFER_L.OUT       = 0;
    io_info_ptr->BUFFER_L.DATA      = NULL;
    io_info_ptr->BUFFER_L.SIZE      = 0;
    io_info_ptr->BUFFER_L.SPACE     = 0;
    io_info_ptr->BUFFER_R.IN        = 0;
    io_info_ptr->BUFFER_R.OUT       = 0;
    io_info_ptr->BUFFER_R.DATA      = NULL;
    io_info_ptr->BUFFER_R.SIZE      = 0;
    io_info_ptr->BUFFER_R.SPACE     = 0;
    io_info_ptr->STATS.INTERRUPTS   = 0;
    io_info_ptr->STATS.UNDERRUNS_L  = 0;
    io_info_ptr->STATS.UNDERRUNS_R  = 0;
    io_info_ptr->STATS.OVERRUNS_L   = 0;
    io_info_ptr->STATS.OVERRUNS_R   = 0;
    io_info_ptr->STATS.RX_PACKETS   = 0;
    io_info_ptr->STATS.TX_PACKETS   = 0;
    io_info_ptr->STATS.PACKETS_REQUESTED_L  = 0;
    io_info_ptr->STATS.PACKETS_PROCESSED_L  = 0;
    io_info_ptr->STATS.PACKETS_QUEUED_L     = 0;
    io_info_ptr->STATS.PACKETS_REQUESTED_R  = 0;
    io_info_ptr->STATS.PACKETS_PROCESSED_R  = 0;
    io_info_ptr->STATS.PACKETS_QUEUED_R     = 0;
    io_info_ptr->DATA_L                 = NULL;
    io_info_ptr->DATA_R                 = NULL;
    io_info_ptr->EVENT_IO_FINISHED = _mem_alloc_system_zero(sizeof(LWEVENT_STRUCT));
    result = _lwevent_create(io_info_ptr->EVENT_IO_FINISHED, LWEVENT_AUTO_CLEAR);
    if (result != MQX_OK) {
        #ifdef I2S_DEBUG
        printf("i2s_init - Error: Unable to create event_int_done.\n");
        #endif
        _task_set_error(result);
   }
    io_info_ptr->IO_FORMAT.ENDIAN       = endianity;
    io_info_ptr->IO_FORMAT.BITS         = bits;
    io_info_ptr->IO_FORMAT.ALIGNMENT    = aligment;
    io_info_ptr->IO_FORMAT.SIZE         = size;
    io_info_ptr->IO_FORMAT.CHANNELS     = channels;
    io_info_ptr->DUMMY.DATA             = NULL;
    io_info_ptr->DUMMY.LENGTH           = 0;
    io_info_ptr->DUMMY.INDEX_L          = 0;
    io_info_ptr->DUMMY.INDEX_R          = 0;
    io_info_ptr->CLK_ALWAYS_ON          = TRUE;
    io_info_ptr->FIRST_IO               = TRUE;
    /* Buffers allocation */
    io_info_ptr->BUFFER_L.DATA          = (uint32_t *) _mem_alloc_system(buffer_size * sizeof(uint32_t));
    if (io_info_ptr->BUFFER_L.DATA == NULL)
    {
        return MQX_OUT_OF_MEMORY;
    }
    io_info_ptr->BUFFER_L.SPACE = buffer_size;
    io_info_ptr->BUFFER_L.SIZE = buffer_size;

    io_info_ptr->BUFFER_R.DATA = (uint32_t *) _mem_alloc_system(buffer_size * sizeof(uint32_t));
    if (io_info_ptr->BUFFER_R.DATA == NULL)
    {
        uint32_t errcode = _mem_free(io_info_ptr->BUFFER_L.DATA);
        if (errcode != MQX_OK)
        {
            return errcode;
        }
        io_info_ptr->BUFFER_L.DATA  = NULL;
        io_info_ptr->BUFFER_L.SPACE = 0;
        io_info_ptr->BUFFER_L.SIZE  = 0;
        return MQX_OUT_OF_MEMORY;
    }
    io_info_ptr->BUFFER_R.SPACE = buffer_size;
    io_info_ptr->BUFFER_R.SIZE  = buffer_size;

    _mem_set_type (io_info_ptr->BUFFER_L.DATA, MEM_TYPE_IO_I2S_LEFT_BUFFER);
    _mem_set_type (io_info_ptr->BUFFER_R.DATA, MEM_TYPE_IO_I2S_RIGHT_BUFFER);

    io_i2s_dev_ptr->DEV_INFO_PTR = io_info_ptr;

    /*Interrupt vector setup*/
    vector = _bsp_get_i2s_vector(channel);
    io_info_ptr->OLD_ISR_DATA = _int_get_isr_data (vector);
    io_info_ptr->OLD_ISR = _int_install_isr (vector, _mcf54xx_i2s_int_isr, io_info_ptr);
    _mcf54xx_int_init((PSP_INTERRUPT_TABLE_INDEX)vector, io_info_ptr->INIT.LEVEL, TRUE);

    return(I2S_OK);
} /* Endbody */


/*FUNCTION****************************************************************
*
* Function Name    : _mcf54xx_i2s_int_deinit
* Returned Value   : MQX error code
* Comments         :
*    This function de-initializes an I2S device.
*
*END*********************************************************************/

uint32_t _mcf54xx_i2s_int_deinit
(
    /* [IN] the initialization information for the device being opened */
    IO_I2S_DEVICE_STRUCT_PTR        io_i2s_dev_ptr,

    /* [IN] the address of the device specific information */
    MCF54XX_I2S_INFO_STRUCT_PTR     io_info_ptr
)
{ /* Body */
    VMCF5441_SSI_STRUCT_PTR         ssi_reg_ptr;
    MCF54XX_I2S_INIT_STRUCT_PTR     i2s_init_ptr;
    uint32_t                         errcode;
   _mqx_uint result = 0;

    if ((NULL == io_info_ptr) || (NULL == io_i2s_dev_ptr))
    {
        return I2S_ERROR_INVALID_PARAMETER;
    }

    ssi_reg_ptr = io_info_ptr->I2S_PTR;

    /*SSI disable*/
    ssi_reg_ptr->SSI_CR &= ~(MCF5441_SSI_CR_SSI_EN);

    /* Clear the SSI events */
    ssi_reg_ptr->SSI_ISR = 0x00;

    /* Install original vectors */
    i2s_init_ptr = (MCF54XX_I2S_INIT_STRUCT_PTR)(io_i2s_dev_ptr->DEV_INIT_DATA_PTR);
    _mcf54xx_int_init(_bsp_get_i2s_vector (i2s_init_ptr->CHANNEL), i2s_init_ptr->LEVEL, FALSE);
    _int_install_isr (_bsp_get_i2s_vector (i2s_init_ptr->CHANNEL), io_info_ptr->OLD_ISR, io_info_ptr->OLD_ISR_DATA);
    /* Destroy event */
    result = _lwevent_destroy(io_info_ptr->EVENT_IO_FINISHED);
    if (result != MQX_OK)
    {
       #ifdef I2S_DEBUG
       printf("i2s_rx - Error: unable to destroy event_int_done.\n");
       #endif
       _task_set_error(result);
    }
    /* Free buffers and info struct */
    errcode = _mem_free (io_info_ptr->BUFFER_L.DATA);
    if (errcode != MQX_OK)
    {
        return errcode;
    }
    io_info_ptr->BUFFER_L.IN    = 0;
    io_info_ptr->BUFFER_L.OUT   = 0;
    io_info_ptr->BUFFER_L.DATA  = NULL;
    io_info_ptr->BUFFER_L.SPACE = 0;
    io_info_ptr->BUFFER_L.SIZE  = 0;

    errcode = _mem_free (io_info_ptr->BUFFER_R.DATA);
    if (errcode != MQX_OK)
    {
        return errcode;
    }
    io_info_ptr->BUFFER_R.IN    = 0;
    io_info_ptr->BUFFER_R.OUT   = 0;
    io_info_ptr->BUFFER_R.DATA  = NULL;
    io_info_ptr->BUFFER_R.SPACE = 0;
    io_info_ptr->BUFFER_R.SIZE  = 0;

    if (io_info_ptr->TX_DUMMY)
    {
        errcode = _mem_free (io_info_ptr->DUMMY.DATA);
        if (errcode != MQX_OK)
        {
            return errcode;
        }
        io_info_ptr->DUMMY.DATA = NULL;
    }

    errcode = _mem_free (io_i2s_dev_ptr->DEV_INFO_PTR);
    if (errcode != MQX_OK)
    {
        return errcode;
    }
    io_i2s_dev_ptr->DEV_INFO_PTR = NULL;
    return I2S_OK;
} /* Endbody */


/*FUNCTION****************************************************************
*
* Function Name    : _mcf54xx_i2s_int_rx
* Returned Value   : number of bytes read
* Comments         :
*   Returns the number of bytes received.
*   Reads the data into provided array when data is available.
*
*END*********************************************************************/

uint32_t _mcf54xx_i2s_int_rx
(
    /* [IN] the address of the device specific information */
    IO_I2S_DEVICE_STRUCT_PTR   io_i2s_dev_ptr,

    /* [IN] The array to copy data into */
    char                   *buffer,

    /* [IN] number of bytes to read */
    uint32_t                    length
)
{ /* Body */
    MCF54XX_I2S_INFO_STRUCT_PTR io_info_ptr = io_i2s_dev_ptr->DEV_INFO_PTR;
    VMCF5441_SSI_STRUCT_PTR ssi_reg_ptr     = io_info_ptr->I2S_PTR;
    MCF54XX_I2S_BUFFER_PTR buffer_l         = &(io_info_ptr->BUFFER_L);
    MCF54XX_I2S_BUFFER_PTR buffer_r         = &(io_info_ptr->BUFFER_R);
    I2S_STATISTICS_STRUCT_PRV_PTR stats         = &(io_info_ptr->STATS);
    AUDIO_DATA_FORMAT_PTR io_format         = &(io_info_ptr->IO_FORMAT);
    LWEVENT_STRUCT_PTR event_int_done = io_info_ptr->EVENT_IO_FINISHED;
    _mqx_uint result                = MQX_OK;
    uint32_t i = 0;
    /* disable transmitter interrupts */
    ssi_reg_ptr->SSI_IER &= ~(
       MCF5441_SSI_IER_TFE0 |
       MCF5441_SSI_IER_TFE1 |
       MCF5441_SSI_IER_TUE0 |
       MCF5441_SSI_IER_TUE1
       );
    /* enable receiver interrupts */
    ssi_reg_ptr->SSI_IER |= (
       MCF5441_SSI_IER_RFF0 |
       MCF5441_SSI_IER_RFF1 |
       MCF5441_SSI_IER_ROE0 |
       MCF5441_SSI_IER_ROE1
       );
    io_info_ptr->IO_MODE            = I2S_IO_READ;
    /* Init stats and buffers to default values */
    stats->PACKETS_REQUESTED_L += length/(io_format->CHANNELS * io_format->SIZE);
    switch (io_format->CHANNELS)
   {
      case 1:
         io_info_ptr->DATA_L = buffer;
         io_info_ptr->DATA_R = buffer;
      break;

      default:
         io_info_ptr->DATA_L = buffer;
          io_info_ptr->DATA_R = buffer + io_format->SIZE;
          stats->PACKETS_REQUESTED_R += length/(io_format->CHANNELS * io_format->SIZE);
      break;
   }

    io_info_ptr->ONTHEWAY = TRUE;

    while(io_info_ptr->ONTHEWAY)
    {
        if (io_info_ptr->FIRST_IO) /* enable receiver and interrupts in case of first run to get some data */
        {
            io_info_ptr->FIRST_IO = FALSE;
            ssi_reg_ptr->SSI_IER |= (MCF5441_SSI_IER_RIE); /* Receive interrupts enable */
            ssi_reg_ptr->SSI_CR |= (MCF5441_SSI_CR_RE); /* Enable receiver */
        }
        /* Wait for receive finish */
        result = _lwevent_wait_ticks(event_int_done, 1, TRUE, 0);
        if (result != MQX_OK)
        {
            #ifdef I2S_DEBUG
             printf("i2s_rx - Error: Wait for event_int_done failed.\n");
            #endif
            _task_set_error(result);
        }
        /* All data received, disable receiver */
        if ((stats->PACKETS_QUEUED_L == stats->PACKETS_REQUESTED_L) &&
            (stats->PACKETS_QUEUED_R == stats->PACKETS_REQUESTED_R))
        {
            io_info_ptr->ONTHEWAY = FALSE;
        }
        while ((buffer_l->SPACE < buffer_l->SIZE) &&
               (stats->PACKETS_PROCESSED_L < stats->PACKETS_QUEUED_L) &&
               (stats->PACKETS_PROCESSED_L < stats->PACKETS_REQUESTED_L))
        {
            if (buffer_l->OUT >= buffer_l->SIZE)
            {
                buffer_l->OUT = 0;
            }
            _int_disable();
            ssi_reg_ptr->SSI_IER &= ~(MCF5441_SSI_IER_RFF0);
            _int_enable();
            _mcf54xx_i2s_format_out_data(buffer_l->DATA[buffer_l->OUT], io_info_ptr->DATA_L, io_format);
            buffer_l->OUT++;
            io_info_ptr->DATA_L += io_format->CHANNELS * io_format->SIZE;
            stats->PACKETS_PROCESSED_L++;
            buffer_l->SPACE++;
            ssi_reg_ptr->SSI_IER |= (MCF5441_SSI_IER_RFF0);
        }

        while ((buffer_r->SPACE < buffer_r->SIZE) &&
               (stats->PACKETS_PROCESSED_R < stats->PACKETS_QUEUED_R) &&
               (stats->PACKETS_PROCESSED_R < stats->PACKETS_REQUESTED_R))
        {
            if (buffer_r->OUT >= buffer_r->SIZE)
            {
                buffer_r->OUT = 0;
            }

            _int_disable();
            ssi_reg_ptr->SSI_IER &= ~(MCF5441_SSI_IER_RFF1);
            _int_enable();
            *(io_info_ptr->DATA_R) = buffer_r->DATA[buffer_r->OUT];
            _mcf54xx_i2s_format_out_data(buffer_r->DATA[buffer_r->OUT], io_info_ptr->DATA_R, io_format);
            buffer_r->OUT++;
            io_info_ptr->DATA_R += io_format->CHANNELS * io_format->SIZE;
            stats->PACKETS_PROCESSED_R++;
            buffer_r->SPACE++;

            ssi_reg_ptr->SSI_IER |= (MCF5441_SSI_IER_RFF1);
        }

    }
    return((stats->PACKETS_PROCESSED_R * io_format->SIZE) + (stats->PACKETS_PROCESSED_L * io_format->SIZE));
} /* Endbody */


/*FUNCTION****************************************************************
*
* Function Name    : _mcf54xx_i2s_int_tx
* Returned Value   : number of bytes written
* Comments         :
*   Returns the number of bytes written.
*   Writes the data provided into transmition buffer if available.
*
*END*********************************************************************/

uint32_t _mcf54xx_i2s_int_tx
(
    /* [IN] the address of the device specific information */
    IO_I2S_DEVICE_STRUCT_PTR    io_i2s_dev_ptr,

    /* [IN] The array to be read from */
    char                    *buffer,

      /* [IN] Length of buffer */
    uint32_t                     length
)
{ /* Body */
    MCF54XX_I2S_INFO_STRUCT_PTR io_info_ptr = io_i2s_dev_ptr->DEV_INFO_PTR;
    VMCF5441_SSI_STRUCT_PTR ssi_reg_ptr     = io_info_ptr->I2S_PTR;
    MCF54XX_I2S_BUFFER_PTR buffer_l         = &(io_info_ptr->BUFFER_L);
    MCF54XX_I2S_BUFFER_PTR buffer_r         = &(io_info_ptr->BUFFER_R);
    I2S_STATISTICS_STRUCT_PRV_PTR stats         = &(io_info_ptr->STATS);
    AUDIO_DATA_FORMAT_PTR io_format         = &(io_info_ptr->IO_FORMAT);
    LWEVENT_STRUCT_PTR event_int_done = io_info_ptr->EVENT_IO_FINISHED;
    _mqx_uint result                        = MQX_OK;

    /* enable transmitter interrupts */
    ssi_reg_ptr->SSI_IER |= (
       MCF5441_SSI_IER_TFE0 |
       MCF5441_SSI_IER_TFE1 |
       MCF5441_SSI_IER_TUE0 |
       MCF5441_SSI_IER_TUE1
       );
    /* disable receiver interrupts */
    ssi_reg_ptr->SSI_IER &= ~(
       MCF5441_SSI_IER_RFF0 |
       MCF5441_SSI_IER_RFF1 |
       MCF5441_SSI_IER_ROE0 |
       MCF5441_SSI_IER_ROE1
       );
    if ((buffer == NULL) && !(io_info_ptr->TX_DUMMY))
    {
        _task_set_error(I2S_ERROR_INVALID_PARAMETER);
        return (0);
    }
    else
    {
        switch (io_format->CHANNELS)
        {
            case 1:
                io_info_ptr->DATA_L = buffer;
                io_info_ptr->DATA_R = buffer;
                break;
            default:
                io_info_ptr->DATA_L = buffer;
                io_info_ptr->DATA_R = buffer + io_format->SIZE;
                break;
        }
    }
    io_info_ptr->IO_MODE = I2S_IO_WRITE;
    /* Init stats and buffers */
    stats->PACKETS_REQUESTED_R += length/(io_format->CHANNELS * io_format->SIZE);
    stats->PACKETS_REQUESTED_L += length/(io_format->CHANNELS * io_format->SIZE);
    if (io_info_ptr->TX_DUMMY)
    {
        stats->PACKETS_QUEUED_L = stats->PACKETS_REQUESTED_L;
        stats->PACKETS_QUEUED_R = stats->PACKETS_REQUESTED_R;
    }
    io_info_ptr->ONTHEWAY = TRUE;

    while(io_info_ptr->ONTHEWAY)
    {
        if (!(io_info_ptr->TX_DUMMY))
        {
            /* Copy data from input to left channel buffer */
            while ((buffer_l->SPACE > 0) && (stats->PACKETS_QUEUED_L < stats->PACKETS_REQUESTED_L))
            {
                if (buffer_l->IN >= buffer_l->SIZE)
                {
                    buffer_l->IN = 0;
                }
                /* buffer access, critical section - disable interrupt with buffer access */
                _int_disable();
                ssi_reg_ptr->SSI_IER &= ~(MCF5441_SSI_IER_TFE0);
                _int_enable();
                buffer_l->DATA[buffer_l->IN++] = _mcf54xx_i2s_format_in_data(io_info_ptr->DATA_L, io_format);
                buffer_l->SPACE--;
                stats->PACKETS_QUEUED_L++;
                /* buffer only even samples */
                io_info_ptr->DATA_L += io_format->CHANNELS * io_format->SIZE;
                /* critical section end - enable interrupt */
                ssi_reg_ptr->SSI_IER |= (MCF5441_SSI_IER_TFE0);
            }
            /* Copy data from input to right channel buffer */
            while ((buffer_r->SPACE > 0) && (stats->PACKETS_QUEUED_R < stats->PACKETS_REQUESTED_R))
            {
                if (buffer_r->IN >= buffer_r->SIZE)
                {
                    buffer_r->IN = 0;
                }
                /* buffer access, critical section - disable interrupt with buffer access */
                _int_disable();
                ssi_reg_ptr->SSI_IER &= ~(MCF5441_SSI_IER_TFE1);
                _int_enable();
                buffer_r->DATA[buffer_r->IN++] = _mcf54xx_i2s_format_in_data(io_info_ptr->DATA_R, io_format);
                buffer_r->SPACE--;
                stats->PACKETS_QUEUED_R++;
                /* buffer only odd samples */
                io_info_ptr->DATA_R += io_format->CHANNELS * io_format->SIZE;
                /* critical section end - enable interrupt */
                ssi_reg_ptr->SSI_IER |= (MCF5441_SSI_IER_TFE1);
            }
        }
        if (io_info_ptr->FIRST_IO)
        {
            io_info_ptr->FIRST_IO = FALSE;
            /* Transmit interrupts enable*/
            ssi_reg_ptr->SSI_IER |= (MCF5441_SSI_IER_TIE);
            /* Enable transmitter */
            ssi_reg_ptr->SSI_CR |= (MCF5441_SSI_CR_TE);
        }
        /* If all requested packets are queued break and return from function */
        if ((stats->PACKETS_QUEUED_L == stats->PACKETS_REQUESTED_L) && (stats->PACKETS_QUEUED_R == stats->PACKETS_REQUESTED_R))
        {
            break;
        }
        /* Wait for transmit finish and make current task blocked */
        result = _lwevent_wait_ticks(event_int_done, 1, TRUE, 0);
        if (result != MQX_OK)
        {
            #ifdef I2S_DEBUG
            printf("i2s_tx - Error: Wait for event_int_done failed.\n");
            #endif
            _task_set_error(result);
        }

    }
    /* return number of processed data */
    return((stats->PACKETS_PROCESSED_R * io_format->SIZE) + (stats->PACKETS_PROCESSED_L * io_format->SIZE));
} /* Endbody */


/*FUNCTION****************************************************************
*
* Function Name    :_mcf54xx_i2s_int_isr
* Returned Value   : none
*
*END*********************************************************************/
static void _mcf54xx_i2s_int_isr
(
    void         *parameter
)
{ /* Body */
    MCF54XX_I2S_INFO_STRUCT_PTR io_info_ptr = parameter;
    VMCF5441_SSI_STRUCT_PTR ssi_reg_ptr     = io_info_ptr->I2S_PTR;
    MCF54XX_I2S_BUFFER_PTR buffer_l         = &(io_info_ptr->BUFFER_L);
    MCF54XX_I2S_BUFFER_PTR buffer_r         = &(io_info_ptr->BUFFER_R);
    I2S_STATISTICS_STRUCT_PRV_PTR stats         = &(io_info_ptr->STATS);
    MCF54XX_I2S_DUMMY_DATA_PTR dummy        = &(io_info_ptr->DUMMY);
    stats->INTERRUPTS++;

    if (io_info_ptr->IO_MODE == I2S_IO_READ)
    {
        /* Receive FIFO0 full */
        if (ssi_reg_ptr->SSI_ISR & MCF5441_SSI_ISR_RFF0)
        {
            uint8_t i, data_n;
            /* Read value of Receive FIFO Counter0 */
            data_n = ((ssi_reg_ptr->SSI_FCSR >> 12) & 0xF);
            if (stats->PACKETS_QUEUED_L + data_n >= stats->PACKETS_REQUESTED_L)
            {
               data_n = stats->PACKETS_REQUESTED_L - stats->PACKETS_QUEUED_L;
            }
            if (data_n > buffer_l->SPACE)
            {
               data_n = buffer_l->SPACE;
            }
            /* Copy data from left channel FIFO to buffer */
            for (i = 0; i < data_n; i++)
            {
                if (buffer_l->IN >= buffer_l->SIZE)
                {
                    buffer_l->IN = 0;
                }
                buffer_l->DATA[buffer_l->IN++] = ssi_reg_ptr->SSI_RX0;
            }
            stats->RX_PACKETS += data_n;
            stats->PACKETS_QUEUED_L += data_n;
           buffer_l->SPACE -= data_n;
        }

        /* Receive FIFO1 full */
        if (ssi_reg_ptr->SSI_ISR & MCF5441_SSI_ISR_RFF1)
        {
            uint8_t i, data_n;
            /* Read value of Receive FIFO Counter1 */
            data_n = ((ssi_reg_ptr->SSI_FCSR >> 28) & 0xF);
           if (stats->PACKETS_QUEUED_R + data_n >= stats->PACKETS_REQUESTED_R)
            {
               data_n = stats->PACKETS_REQUESTED_R - stats->PACKETS_QUEUED_R;
            }
            if (data_n > buffer_r->SPACE)
            {
               data_n = buffer_r->SPACE;
            }
            /* Copy data from right channel FIFO to buffer */
            for (i = 0; i < data_n; i++)
            {
                if (buffer_r->IN >= buffer_r->SIZE)
                {
                    buffer_r->IN = 0;
                }
                buffer_r->DATA[buffer_r->IN++] = ssi_reg_ptr->SSI_RX1;

            }
            stats->RX_PACKETS += data_n;
            stats->PACKETS_QUEUED_R += data_n;
           buffer_r->SPACE -= data_n;
        }

        /* Receiver overrun error 0 */
        if (ssi_reg_ptr->SSI_ISR & MCF5441_SSI_ISR_ROE0)
        {
            #ifdef I2S_DEBUG
            printf("i2s_isr - Error: Receiver 0 overrun.\n");
            #endif
            stats->OVERRUNS_L++;
            /* Clear error flag */
            ssi_reg_ptr->SSI_ISR |= (MCF5441_SSI_ISR_ROE0);
        }

        /* Receiver overrun error 1 */
        if (ssi_reg_ptr->SSI_ISR & MCF5441_SSI_ISR_ROE1)
        {
            #ifdef I2S_DEBUG
            printf("i2s_isr - Error: Receiver 1 overrun.\n");
            #endif
            stats->OVERRUNS_R++;
            /* Clear error flag */
            ssi_reg_ptr->SSI_ISR |= (MCF5441_SSI_ISR_ROE1);
        }
    }
    else
    {
        /* All data transmitted, disable transmitter */
        if ((stats->PACKETS_PROCESSED_L == stats->PACKETS_REQUESTED_L) &&
            (stats->PACKETS_PROCESSED_R == stats->PACKETS_REQUESTED_R) &&
            (ssi_reg_ptr->SSI_ISR & MCF5441_SSI_ISR_TFE0) &&
            (ssi_reg_ptr->SSI_ISR & MCF5441_SSI_ISR_TFE1))
        {
            ssi_reg_ptr->SSI_CR &= ~(MCF5441_SSI_CR_TE);
            _int_disable();
            ssi_reg_ptr->SSI_IER &= ~(MCF5441_SSI_IER_TIE);
            _int_enable();
            io_info_ptr->ONTHEWAY = FALSE;
            io_info_ptr->FIRST_IO = TRUE;
        }

        /* Transmit FIFO0 empty - need more data for left channel*/
        if (ssi_reg_ptr->SSI_ISR & MCF5441_SSI_ISR_TFE0)
        {
            uint8_t i, space;
            /* Read value of Transmit FIFO Counter0 */
            space = FIFO_WATERMARK_MAX - ((ssi_reg_ptr->SSI_FCSR >> I2S_CHAR_BIT) & 0xF);
            /* Copy data from buffer to left channel FIFO */
            for (i = 0; ((i < space) && (stats->PACKETS_PROCESSED_L < stats->PACKETS_QUEUED_L)); i++)
            {
                if (io_info_ptr->TX_DUMMY)
                {
                    ssi_reg_ptr->SSI_TX0 = (*(dummy->DATA + dummy->INDEX_L)) & 0xFFFFFFFF;
                    if (dummy->INDEX_L++ >= dummy->LENGTH)
                    {
                        dummy->INDEX_L = 0;
                    }
                    stats->PACKETS_PROCESSED_L++;
                }
                else
                {
                    if (buffer_l->OUT >= buffer_l->SIZE)
                    {
                        buffer_l->OUT = 0;
                    }
                    ssi_reg_ptr->SSI_TX0 = buffer_l->DATA[buffer_l->OUT++];
                    stats->TX_PACKETS++;
                    stats->PACKETS_PROCESSED_L++;
                    buffer_l->SPACE++;
                }
            }
        }
        /* Transmit FIFO1 empty - need more data for right channel  */
        if (ssi_reg_ptr->SSI_ISR & MCF5441_SSI_ISR_TFE1)
        {
            uint8_t i, space;
            /* Read value of Transmit FIFO Counter1 */
            space = FIFO_WATERMARK_MAX - ((ssi_reg_ptr->SSI_FCSR >> (I2S_CHAR_BIT * 3)) & 0xF);

            /* Copy data from buffer to right channel FIFO */
            for (i = 0; ((i < space) && (stats->PACKETS_PROCESSED_R < stats->PACKETS_QUEUED_R)); i++)
            {
                if (io_info_ptr->TX_DUMMY)
                {
                    ssi_reg_ptr->SSI_TX1 = (*(dummy->DATA + dummy->INDEX_R)) & 0xFFFFFFFF;
                    if (dummy->INDEX_R++ >= dummy->LENGTH)
                    {
                        dummy->INDEX_R = 0;
                    }
                    stats->PACKETS_PROCESSED_R++;
                }
                else
                {
                    if (buffer_r->OUT >= buffer_r->SIZE)
                    {
                        buffer_r->OUT = 0;
                    }
                    ssi_reg_ptr->SSI_TX1 = buffer_r->DATA[buffer_r->OUT++];
                    stats->TX_PACKETS++;
                    stats->PACKETS_PROCESSED_R++;
                    buffer_r->SPACE++;
                }
            }
        }
        /* Check for transmit underrun error 0 */
        if (ssi_reg_ptr->SSI_ISR & MCF5441_SSI_ISR_TUE0)
        {
            #ifdef I2S_DEBUG
            printf("i2s_isr - Error: Left channel underrun.\n");
            #endif
            stats->UNDERRUNS_L++;
            /* Clear error flag */
            ssi_reg_ptr->SSI_ISR |= (MCF5441_SSI_ISR_TUE0);
        }

        /* Check for transmit underrun error 1 */
        if (ssi_reg_ptr->SSI_ISR & MCF5441_SSI_ISR_TUE1)
        {
            #ifdef I2S_DEBUG
            printf("i2s_isr - Error: Right channel underrun.\n");
            #endif
            stats->UNDERRUNS_R++;
            /* Clear error flag */
            ssi_reg_ptr->SSI_ISR |= (MCF5441_SSI_ISR_TUE1);
        }
    }
    if (_lwevent_set(io_info_ptr->EVENT_IO_FINISHED,0x01) != MQX_OK)
    {
        #ifdef I2S_DEBUG
        printf("i2s_isr - Error: Unable to set EVENT_IO_FINISHED.\n");
        #endif
    }
} /* Endbody */

/*FUNCTION****************************************************************
*
* Function Name    : _mcf54xx_i2s_int_ioctl
* Returned Value   : MQX error code.
* Comments         :
*    This function performs miscellaneous services for
*    the I2S I/O device.
*
*END*********************************************************************/

uint32_t _mcf54xx_i2s_int_ioctl
(
    /* [IN] the address of the device specific information */
    MCF54XX_I2S_INFO_STRUCT_PTR io_info_ptr,

    /* [IN] The command to perform */
    uint32_t                     cmd,

    /* [IN] Parameters for the command */
    void                       *param_ptr
)
{ /* Body */
    uint32_t result                      = MQX_OK;
    VMCF5441_SSI_STRUCT_PTR ssi_reg_ptr = io_info_ptr->I2S_PTR;
    VMCF5441_STRUCT_PTR pReg            = _PSP_GET_IPSBAR();
    uint8_t watermark                    = 0;
    MCF54XX_I2S_DUMMY_DATA_PTR dummy    = &io_info_ptr->DUMMY;
    switch(cmd)
    {
        case IO_IOCTL_I2S_SET_MODE_MASTER:
            if(io_info_ptr->ONTHEWAY)
            {
                #ifdef I2S_DEBUG
                printf("_i2s_ioctl - IO_IOCTL_I2S_SET_MODE_MASTER: Error - Cannot chnage I2S mode while I/O actions are in progress.\n");
                #endif
                result = I2S_ERROR_DEVICE_BUSY;
                break;
            }
            ssi_reg_ptr->SSI_CR &= ~(MCF5441_SSI_CR_SSI_EN);
            ssi_reg_ptr->SSI_CR |= (MCF5441_SSI_CR_I2S_MASTER);

            if (io_info_ptr->CLOCK_SOURCE == I2S_CLK_EXT)
            {
                /* Internal master clock disable */
                ssi_reg_ptr->SSI_CR  &= ~(MCF5441_SSI_CR_MCE);
                /* change pin function to receive external MCLK signal */
                pReg->GPIO.PAR_SSI0H |= (MCF5441_GPIO_PAR_SSI0H_MCLK & ALTERNATE_FC_1_10);
            }
            else if (io_info_ptr->CLOCK_SOURCE == I2S_CLK_INT)
            {
                /* Change pin function to MCLK output pin */
                pReg->GPIO.PAR_SSI0H |= (MCF5441_GPIO_PAR_SSI0H_MCLK);
                /* Master clock enable */
                ssi_reg_ptr->SSI_CR  |= (MCF5441_SSI_CR_MCE);
                io_info_ptr->MCLK_FREQ = SYSTEM_CLOCK / io_info_ptr->MCLK_DIV;
                io_info_ptr->BCLK_FREQ = io_info_ptr->MCLK_FREQ / io_info_ptr->BCLK_DIV;
                /* The word length is fixed to 32 in I2S master mode and DC is 1
                thus frame sync frequency is always BCLK_FREQ/64 */
                io_info_ptr->FS_FREQ = io_info_ptr->BCLK_FREQ / 64;
            }

            ssi_reg_ptr->SSI_TCR |= (
                                    MCF5441_SSI_TCR_TXDIR | /* Bit clock generated internally */
                                    MCF5441_SSI_TCR_TFDIR   /* Frame sync generated internally */
                                      );
            ssi_reg_ptr->SSI_CR |= (MCF5441_SSI_CR_SSI_EN);
            io_info_ptr->MODE = I2S_MODE_MASTER;
            break;

        case IO_IOCTL_I2S_SET_MODE_SLAVE:
            if(io_info_ptr->ONTHEWAY)
            {
                #ifdef I2S_DEBUG
                printf("_i2s_ioctl - IO_IOCTL_I2S_SET_MODE_SLAVE: Error - Cannot chnage I2S mode while I/O actions are in progress.\n");
                #endif
                result = I2S_ERROR_DEVICE_BUSY;
                break;
            }
            ssi_reg_ptr->SSI_CR &= ~(MCF5441_SSI_CR_SSI_EN);
            ssi_reg_ptr->SSI_CR |= (MCF5441_SSI_CR_I2S_SLAVE);
            /* Master clock disabled */
            ssi_reg_ptr->SSI_CR &= ~(MCF5441_SSI_CR_MCE);
            /* set MCLK pin to CLKIN*/
            pReg->GPIO.PAR_SSI0H |= (MCF5441_GPIO_PAR_SSI0H_MCLK & ALTERNATE_FC_1_10);
            ssi_reg_ptr->SSI_TCR &= ~(
                                      MCF5441_SSI_TCR_TXDIR | /* Bit clock generated externally */
                                      MCF5441_SSI_TCR_TFDIR   /* Frame sync generated externally */
                                     );
            ssi_reg_ptr->SSI_CR |= (MCF5441_SSI_CR_SSI_EN);
            io_info_ptr->MODE = I2S_MODE_SLAVE;
            break;

        case IO_IOCTL_I2S_SET_CLOCK_SOURCE_INT:
            if(io_info_ptr->ONTHEWAY)
            {
                #ifdef I2S_DEBUG
                printf("_i2s_ioctl - IO_IOCTL_I2S_SET_CLOCK_SOURCE_INT: Error - Cannot chnage I2S clock source while I/O actions are in progress.\n");
                #endif
                result = I2S_ERROR_DEVICE_BUSY;
                break;
            }
            /* You should not change clock source, when in slave mode */
            if (io_info_ptr->MODE == I2S_MODE_SLAVE)
            {
                #ifdef I2S_DEBUG
                printf("_i2s_ioctl - IO_IOCTL_I2S_SET_CLOCK_SOURCE_INT: Error - Cannot chnage I2S clock source while in slave mode.\n");
                #endif
                result = IO_ERROR_INVALID_IOCTL_CMD;
                break;
            }

            pReg->GPIO.PAR_SSI0H |= (MCF5441_GPIO_PAR_SSI0H_MCLK);
            /* disable SSI - see table at top of this file */
            ssi_reg_ptr->SSI_CR  &= ~(MCF5441_SSI_CR_SSI_EN);
            /* Internal master clock enable */
            ssi_reg_ptr->SSI_CR  |= (MCF5441_SSI_CR_MCE);
            ssi_reg_ptr->SSI_CR  |= (MCF5441_SSI_CR_SSI_EN);

            if (io_info_ptr->CHANNEL == 0)
            {
                /*SSI0 clock source = PLL*/
                pReg->CCM.MISCCR |= (MCF54XX_CCM_MISCCR_SSI0SRC);
            }
            else if (io_info_ptr->CHANNEL == 1)
            {
                /*SSI1 clock source = PLL*/
                pReg->CCM.MISCCR |= (MCF54XX_CCM_MISCCR_SSI1SRC);
            }
            io_info_ptr->CLOCK_SOURCE = I2S_CLK_INT;
            io_info_ptr->MCLK_FREQ    = BSP_SYSTEM_CLOCK / io_info_ptr->MCLK_DIV;
            io_info_ptr->BCLK_FREQ    = io_info_ptr->MCLK_FREQ / io_info_ptr->BCLK_DIV;
            io_info_ptr->FS_FREQ      = io_info_ptr->BCLK_FREQ / io_info_ptr->DATA_BITS;
            break;

        case IO_IOCTL_I2S_SET_CLOCK_SOURCE_EXT:
            if(io_info_ptr->ONTHEWAY)
            {
                #ifdef I2S_DEBUG
                printf("_i2s_ioctl - IO_IOCTL_I2S_SET_CLOCK_SOURCE_EXT: Error - Cannot chnage I2S clock source while I/O actions are in progress.\n");
                #endif
                result = I2S_ERROR_DEVICE_BUSY;
                break;
            }
            if (io_info_ptr->MODE == I2S_MODE_SLAVE)
            {
                #ifdef I2S_DEBUG
                printf("_i2s_ioctl - IO_IOCTL_I2S_SET_CLOCK_SOURCE_EXT: Error - Cannot change I2S clock source while in slave mode.\n");
                #endif
                result = IO_ERROR_INVALID_IOCTL_CMD;
                break;
            }
            /*enable pin to receive external CLK signal*/
            pReg->GPIO.PAR_SSI0H |= (MCF5441_GPIO_PAR_SSI0H_MCLK & ALTERNATE_FC_1_10);
            /* disable SSI - see table at top of this file */
            ssi_reg_ptr->SSI_CR  &= ~(MCF5441_SSI_CR_SSI_EN);
            /* Internal master clock disable */
            ssi_reg_ptr->SSI_CR  &= ~(MCF5441_SSI_CR_MCE);
            ssi_reg_ptr->SSI_CR  |= (MCF5441_SSI_CR_SSI_EN);

            if (io_info_ptr->CHANNEL == 0)
            {
                /*SSI0 clock source = SSI_CLKIN pin*/
                pReg->CCM.MISCCR &= ~(MCF54XX_CCM_MISCCR_SSI0SRC);
            }
            else if (io_info_ptr->CHANNEL == 1)
            {
                /*SSI1 clock source = SSI_CLKIN pin*/
                pReg->CCM.MISCCR &= ~(MCF54XX_CCM_MISCCR_SSI1SRC);
            }
            io_info_ptr->CLOCK_SOURCE = I2S_CLK_EXT;
            break;

        case IO_IOCTL_I2S_SET_DATA_BITS:
            if(io_info_ptr->ONTHEWAY)
            {
                #ifdef I2S_DEBUG
                printf("_i2s_ioctl - IO_IOCTL_I2S_SET_DATA_BITS: Error - Cannot chnage I2S data bit length while I/O actions are in progress.\n");
                #endif
                result = I2S_ERROR_DEVICE_BUSY;
                break;
            }
            if (param_ptr == NULL)
            {
                result = I2S_ERROR_INVALID_PARAMETER;
            }
            else
            {
                uint8_t data_bits =  *((uint8_t *) param_ptr);
                if ((data_bits < 8) || (data_bits > 24) || (data_bits == 14) || (data_bits % 2))
                {
                    #ifdef I2S_DEBUG
                    printf("_i2s_ioctl - IO_IOCTL_I2S_SET_DATA_BITS: Error - Unsupported data word length.\n");
                    #endif
                    result = I2S_ERROR_WORD_LENGTH_UNSUPPORTED;
                    break;
                }
                data_bits = (data_bits/2) - 1;
                ssi_reg_ptr->SSI_CR  &= ~(MCF5441_SSI_CR_SSI_EN);
                ssi_reg_ptr->SSI_CCR |= (MCF5441_SSI_CCR_WL((uint32_t) data_bits));
                ssi_reg_ptr->SSI_CR  |= (MCF5441_SSI_CR_SSI_EN);
                io_info_ptr->DATA_BITS = *((uint8_t *) param_ptr);
                io_info_ptr->FS_FREQ   = io_info_ptr->BCLK_FREQ / io_info_ptr->DATA_BITS;
            }
            break;

        case IO_IOCTL_I2S_DISABLE_DEVICE:
            _int_disable();
            ssi_reg_ptr->SSI_IER &= ~(
                                    MCF5441_SSI_IER_TIE | /* Transmit interrupts disable */
                                    MCF5441_SSI_IER_RIE   /* Receive interrupts disable */
                                    );
            /* Transmitter and receiver disable */
            ssi_reg_ptr->SSI_CR &= ~(
               MCF5441_SSI_CR_TE |
               MCF5441_SSI_CR_RE
               );
            _int_enable();
            /*SSI disable*/
            ssi_reg_ptr->SSI_CR &= ~(MCF5441_SSI_CR_SSI_EN);
            break;

        case IO_IOCTL_I2S_ENABLE_DEVICE:
            ssi_reg_ptr->SSI_CR |= (MCF5441_SSI_CR_SSI_EN); /*SSI enable*/
            ssi_reg_ptr->SSI_IER |= (
                                    MCF5441_SSI_IER_TIE | /* Transmit interrupts enable */
                                    MCF5441_SSI_IER_RIE   /* Receive interrupts enable */
                                    );
            break;

        case IO_IOCTL_I2S_SET_MCLK_FREQ:
            if(io_info_ptr->ONTHEWAY)
            {
                #ifdef I2S_DEBUG
                printf("_i2s_ioctl - IO_IOCTL_I2S_SET_MCLK_FREQ: Error - Cannot chnage I2S master clock frequency while I/O actions are in progress.\n");
                #endif
                result = I2S_ERROR_DEVICE_BUSY;
                break;
            }
            if (param_ptr == NULL)
            {
                result = I2S_ERROR_INVALID_PARAMETER;
                break;
            }
            else
            {
                uint8_t divider;
                uint32_t frequency;

                frequency = *((uint32_t *) param_ptr);

                if (io_info_ptr->CLOCK_SOURCE == I2S_CLK_INT)
                {
                    divider = BSP_SYSTEM_CLOCK / frequency;
                    if ((divider < SSIDIV_MIN) || (divider > SSIDIV_MAX))
                    {
                        #ifdef I2S_DEBUG
                        printf("_i2s_ioctl - IO_IOCTL_I2S_SET_MCLK_FREQ: Error - System clock divider value out of range.\n");
                        #endif
                        result = I2S_ERROR_DIVIDER_VALUE_INVALID;
                    }
                    else
                    {
                        /* SSI baud clock = Fsys / (SSIDIV/2)*/
                        if (io_info_ptr->CHANNEL == 0)
                        {
                            pReg->CCM.CDRH |= MCF54XX_CCM_MISCCR_CDRH_SSI0DIV((uint32_t) divider * 2);
                        }
                        else if (io_info_ptr->CHANNEL == 1)
                        {
                            pReg->CCM.CDRH |= MCF54XX_CCM_MISCCR_CDRH_SSI1DIV((uint32_t) divider * 2);
                        }
                        io_info_ptr->MCLK_FREQ = BSP_SYSTEM_CLOCK / divider;
                        io_info_ptr->MCLK_DIV = divider;
                    }
                }
                else
                {
                    io_info_ptr->MCLK_FREQ = frequency;
                }
                if (io_info_ptr->MODE == I2S_MODE_MASTER)
                {
                    io_info_ptr->FS_FREQ = io_info_ptr->BCLK_FREQ / 64;
                }
                else
                {
                    io_info_ptr->FS_FREQ = io_info_ptr->BCLK_FREQ / (2 * io_info_ptr->DATA_BITS);
                }
                io_info_ptr->BCLK_FREQ = io_info_ptr->MCLK_FREQ / io_info_ptr->BCLK_DIV;
            }
            break;

        case IO_IOCTL_I2S_SET_FS_FREQ:
            if(io_info_ptr->ONTHEWAY)
            {
                #ifdef I2S_DEBUG
                printf("_i2s_ioctl - IO_IOCTL_I2S_SET_FS_FREQ: Error - Cannot chnage I2S sampling frequency while I/O actions are in progress.\n");
                #endif
                result = I2S_ERROR_DEVICE_BUSY;
                break;
            }
            if (param_ptr == NULL)
            {
                result = I2S_ERROR_INVALID_PARAMETER;
                break;
            }
            else
            {
                uint32_t frequency, bclk_frequency;
                uint16_t divider;

                frequency = *((uint32_t *) param_ptr);

                if (io_info_ptr->MODE == I2S_MODE_SLAVE)
                {
                    bclk_frequency = frequency * io_info_ptr->DATA_BITS * 2;
                    if ((bclk_frequency > (io_info_ptr->MCLK_FREQ / 2)) || (bclk_frequency > (BSP_BUS_CLOCK / 5)))
                    {
                        #ifdef I2S_DEBUG
                        printf("_i2s_ioctl - IO_IOCTL_I2S_SET_FS_FREQ: Error - BCLK_FREQ > MCLK_FREQ/2 or BCLK_FREQ > BUS_CLOCK / 5.\n");
                        #endif
                        result = I2S_ERROR_FREQUENCY_INVALID;
                        break;
                    }
                    io_info_ptr->BCLK_FREQ = bclk_frequency;
                    io_info_ptr->FS_FREQ = frequency;
                    break;
                }

                bclk_frequency = frequency * 64;
                divider = io_info_ptr->MCLK_FREQ / (bclk_frequency * 2);
                /*
                    ** valid divider values are:
                    ** 1-256 in step of 1
                    ** 257-512 in step of 2
                    ** 513-2048 in step of 8
                    ** 2049-4096 in step of 16
                    */
                if (
                        (divider < BCLK_DIV_MIN) || (divider > BCLK_DIV_MAX) ||
                        (divider > 256) && (divider < 513) && (divider%2) ||
                        (divider > 512) && (divider < 2049) && (divider%8) ||
                        (divider > 2048) && (divider < BCLK_DIV_MAX+1) && (divider%16)
                        )
                {
                    #ifdef I2S_DEBUG
                    printf("_i2s_ioctl - IO_IOCTL_I2S_SET_FS_FREQ: Error - Invalid BCLK divider value.\n");
                    #endif
                    result = I2S_ERROR_DIVIDER_VALUE_INVALID;
                }
                else
                {
                    if (divider < 257)
                    {
                        /* According to MCF5441X reference manual values of DIV2, PSR and PM must
                          not be changed simultaneously */
                        ssi_reg_ptr->SSI_CCR |= (MCF5441_SSI_CCR_PM(divider - 1));
                        ssi_reg_ptr->SSI_CCR &= ~(MCF5441_SSI_CCR_PSR);
                        ssi_reg_ptr->SSI_CCR &= ~(MCF5441_SSI_CCR_DIV2);
                    }
                    else if ((divider > 256) && (divider < 513))
                    {
                        divider = divider/2;
                        ssi_reg_ptr->SSI_CCR |= (MCF5441_SSI_CCR_PM(divider - 1));
                        ssi_reg_ptr->SSI_CCR &= ~(MCF5441_SSI_CCR_PSR);
                        ssi_reg_ptr->SSI_CCR |= (MCF5441_SSI_CCR_DIV2);
                    }
                    else if ((divider > 512) && (divider < 2049))
                    {
                        divider = divider/8;
                        ssi_reg_ptr->SSI_CCR |= (MCF5441_SSI_CCR_PM(divider - 1));
                        ssi_reg_ptr->SSI_CCR |= (MCF5441_SSI_CCR_PSR);
                        ssi_reg_ptr->SSI_CCR &= ~(MCF5441_SSI_CCR_DIV2);
                    }
                    else if (divider > 2048)
                    {
                        divider = divider/16;
                        ssi_reg_ptr->SSI_CCR |= (MCF5441_SSI_CCR_PM(divider - 1));
                        ssi_reg_ptr->SSI_CCR |= (MCF5441_SSI_CCR_PSR);
                        ssi_reg_ptr->SSI_CCR |= (MCF5441_SSI_CCR_DIV2);
                    }
                    io_info_ptr->BCLK_DIV  = divider * 2;
                    io_info_ptr->BCLK_FREQ = io_info_ptr->MCLK_FREQ / io_info_ptr->BCLK_DIV;
                    /* The word length is fixed to 32 in I2S master mode and DC is 1
                     thus frame sync frequency is always BCLK_FREQ/64 */
                    io_info_ptr->FS_FREQ = io_info_ptr->BCLK_FREQ / 64;
                }
            }
            break;

        case IO_IOCTL_AUDIO_SET_IO_DATA_FORMAT:
            if(io_info_ptr->ONTHEWAY)
            {
                #ifdef I2S_DEBUG
                printf("_i2s_ioctl - IO_IOCTL_I2S_SET_IO_DATA_FORMAT: Error - Cannot change I/O data format while I/O actions are in progress.\n");
                #endif
                result = I2S_ERROR_DEVICE_BUSY;
                break;
            }
            if (param_ptr == NULL)
            {
                result = I2S_ERROR_INVALID_PARAMETER;
                break;
            }
            else
            {
                uint8_t      aligment, endianity, bits, channels;
                endianity   = ((AUDIO_DATA_FORMAT_PTR) param_ptr)->ENDIAN;
                bits        = ((AUDIO_DATA_FORMAT_PTR) param_ptr)->BITS;
                aligment    = ((AUDIO_DATA_FORMAT_PTR) param_ptr)->ALIGNMENT;
                channels    = ((AUDIO_DATA_FORMAT_PTR) param_ptr)->CHANNELS;

                if ((aligment > AUDIO_ALIGNMENT_LEFT) || (bits < AUDIO_BIT_SIZE_MIN) ||
                        (bits >AUDIO_BIT_SIZE_MAX) || (endianity > AUDIO_LITTLE_ENDIAN) ||
                        (channels > 2) || (channels == 0))
                {
                    #ifdef I2S_DEBUG
                    printf("_i2s_ioctl - IO_IOCTL_I2S_SET_IO_DATA_FORMAT: Error - Invalid I/O data format.\n");
                    #endif
                    result = AUDIO_ERROR_INVALID_IO_FORMAT;
                    break;
                }
                io_info_ptr->IO_FORMAT.ENDIAN       = endianity;
                io_info_ptr->IO_FORMAT.BITS         = bits;
                io_info_ptr->IO_FORMAT.ALIGNMENT    = aligment;
                io_info_ptr->IO_FORMAT.CHANNELS     = channels;
            }
            break;

        case IO_IOCTL_I2S_SET_TXFIFO_WATERMARK:
            if (param_ptr == NULL)
            {
                result = I2S_ERROR_INVALID_PARAMETER;
                break;
            }
            watermark = *((uint8_t *) param_ptr);
            if (watermark > FIFO_WATERMARK_MAX)
            {
                result = I2S_ERROR_PARAM_OUT_OF_RANGE;
                break;
            }
            ssi_reg_ptr->SSI_FCSR |= (
              MCF5441_SSI_FCSR_TFWM0(watermark) | /* Set transmit FIFO0 watermark */
              MCF5441_SSI_FCSR_TFWM1(watermark)   /* Set transmit FIFO1 watermark */
           );
          break;

       case IO_IOCTL_I2S_SET_RXFIFO_WATERMARK:
          if (param_ptr == NULL)
            {
                result = I2S_ERROR_INVALID_PARAMETER;
                break;
            }
            watermark = *((uint8_t *) param_ptr);
            if (watermark > FIFO_WATERMARK_MAX)
            {
                result = I2S_ERROR_PARAM_OUT_OF_RANGE;
                break;
            }
            ssi_reg_ptr->SSI_FCSR |= (
              MCF5441_SSI_FCSR_RFWM0(watermark) | /* Set receive FIFO0 watermark */
              MCF5441_SSI_FCSR_RFWM1(watermark)   /* Set reveive FIFO1 watermark */
           );
          break;

        case IO_IOCTL_I2S_TX_DUMMY_ON:
           if (io_info_ptr->TX_DUMMY == FALSE)
           {
            io_info_ptr->TX_DUMMY = TRUE;
            dummy->LENGTH   = _mcf54xx_i2s_sine_wave(io_info_ptr->FS_FREQ, 440, &(dummy->DATA));
            dummy->INDEX_L  = 0;
            dummy->INDEX_R  = 0;
           }
            break;

        case IO_IOCTL_I2S_TX_DUMMY_OFF:
           if (io_info_ptr->TX_DUMMY == TRUE)
           {
            io_info_ptr->TX_DUMMY = FALSE;
            result = _mem_free(dummy->DATA);
            if (result == MQX_OK)
            {
                dummy->LENGTH = 0;
                dummy->DATA = NULL;
            }
            dummy->INDEX_L = 0;
            dummy->INDEX_R = 0;
           }
            break;

        case IO_IOCTL_I2S_GET_MODE:
            if (param_ptr == NULL)
            {
                result = I2S_ERROR_INVALID_PARAMETER;
                break;
            }
            *((uint8_t *) param_ptr) = io_info_ptr->MODE;
            break;

        case IO_IOCTL_I2S_GET_CLOCK_SOURCE:
            if (param_ptr == NULL)
            {
                result = I2S_ERROR_INVALID_PARAMETER;
                break;
            }
            *((uint8_t *) param_ptr) = io_info_ptr->CLOCK_SOURCE;
            break;

        case IO_IOCTL_I2S_GET_DATA_BITS:
            if (param_ptr == NULL)
            {
                result = I2S_ERROR_INVALID_PARAMETER;
                break;
            }
            *((uint8_t *) param_ptr) = io_info_ptr->DATA_BITS;
            break;

        case IO_IOCTL_I2S_GET_MCLK_FREQ:
            if (param_ptr == NULL)
            {
                result = I2S_ERROR_INVALID_PARAMETER;
                break;
            }
            *((uint32_t *) param_ptr) = io_info_ptr->MCLK_FREQ;
            break;

        case IO_IOCTL_I2S_GET_BCLK_FREQ:
            if (param_ptr == NULL)
            {
                result = I2S_ERROR_INVALID_PARAMETER;
                break;
            }
            *((uint32_t *) param_ptr) = io_info_ptr->BCLK_FREQ;
            break;

        case IO_IOCTL_I2S_GET_TX_DUMMY:
            if (param_ptr == NULL)
            {
                result = I2S_ERROR_INVALID_PARAMETER;
                break;
            }
            *((bool*) param_ptr) = io_info_ptr->TX_DUMMY;
            break;

        case IO_IOCTL_I2S_GET_FS_FREQ:
            if (param_ptr == NULL)
            {
                result = I2S_ERROR_INVALID_PARAMETER;
                break;
            }
            *((uint32_t *) param_ptr) = io_info_ptr->FS_FREQ;
            break;

        case IO_IOCTL_I2S_GET_STATISTICS:
            if (param_ptr == NULL)
            {
                result = I2S_ERROR_INVALID_PARAMETER;
                break;
            }
            else
            {
                I2S_STATISTICS_STRUCT_PTR stats_out = (I2S_STATISTICS_STRUCT_PTR) param_ptr;
                I2S_STATISTICS_STRUCT_PRV_PTR stats_in = &(io_info_ptr->STATS);
                
                stats_out->INTERRUPTS = stats_in->INTERRUPTS;
                stats_out->FIFO_ERROR = stats_in->UNDERRUNS_L + stats_in->UNDERRUNS_R + stats_in->OVERRUNS_L +stats_in->OVERRUNS_R;
                stats_out->BUFFER_ERROR = 0;
                stats_out->RX_PACKETS = stats_in->RX_PACKETS;
                stats_out->TX_PACKETS = stats_in->TX_PACKETS;
                stats_out->PACKETS_PROCESSED = stats_in->PACKETS_PROCESSED_L + stats_in->PACKETS_PROCESSED_R;
                stats_out->PACKETS_QUEUED = stats_in->PACKETS_QUEUED_L + stats_in->PACKETS_QUEUED_R;
                stats_out->PACKETS_REQUESTED = stats_in->PACKETS_REQUESTED_L + stats_in->PACKETS_REQUESTED_R;
            }
            break;

        case IO_IOCTL_AUDIO_GET_IO_DATA_FORMAT:
            if (param_ptr == NULL)
            {
                result = I2S_ERROR_INVALID_PARAMETER;
                break;
            }
            ((AUDIO_DATA_FORMAT_PTR) param_ptr)->ENDIAN     = io_info_ptr->IO_FORMAT.ENDIAN;
            ((AUDIO_DATA_FORMAT_PTR) param_ptr)->BITS       = io_info_ptr->IO_FORMAT.BITS;
            ((AUDIO_DATA_FORMAT_PTR) param_ptr)->ALIGNMENT  = io_info_ptr->IO_FORMAT.ALIGNMENT;
            ((AUDIO_DATA_FORMAT_PTR) param_ptr)->CHANNELS   = io_info_ptr->IO_FORMAT.CHANNELS;
            break;

        case IO_IOCTL_I2S_GET_TXFIFO_WATERMARK:
            if (param_ptr == NULL)
            {
                result = I2S_ERROR_INVALID_PARAMETER;
                break;
            }
            *((uint8_t *) param_ptr) = ssi_reg_ptr->SSI_FCSR & 0x000F;
            break;

        case IO_IOCTL_I2S_GET_RXFIFO_WATERMARK:
            if (param_ptr == NULL)
            {
                result = I2S_ERROR_INVALID_PARAMETER;
                break;
            }
            *((uint8_t *) param_ptr) = (ssi_reg_ptr->SSI_FCSR & 0x00F0) >> 8;
            break;

        case IO_IOCTL_I2S_SET_CLK_ALWAYS_ENABLED_ON:
            /* Enable receiving clocks after receiver is disabled */
            ssi_reg_ptr->SSI_CR &= ~(MCF5441_SSI_CR_RCD);
            /* Enable transmitting clocks after transmitter is disabled */
            ssi_reg_ptr->SSI_CR &= ~(MCF5441_SSI_CR_TCD);
            io_info_ptr->CLK_ALWAYS_ON = TRUE;
            break;

        case IO_IOCTL_I2S_SET_CLK_ALWAYS_ENABLED_OFF:
            /* Disable receiving clocks after receiver is disabled */
            ssi_reg_ptr->SSI_CR |= (MCF5441_SSI_CR_RCD);
            /* Disable transmitting clocks after transmitter is disabled */
            ssi_reg_ptr->SSI_CR |= (MCF5441_SSI_CR_TCD);
            io_info_ptr->CLK_ALWAYS_ON = FALSE;
            break;

        case IO_IOCTL_I2S_GET_CLK_ALWAYS_ENABLED:
            if (param_ptr == NULL)
            {
                result = I2S_ERROR_INVALID_PARAMETER;
                break;
            }
            *((bool*) param_ptr) = io_info_ptr->CLK_ALWAYS_ON;
            break;

        case IO_IOCTL_I2S_CLEAR_STATISTICS:
            io_info_ptr->STATS.INTERRUPTS   = 0;
            io_info_ptr->STATS.UNDERRUNS_L  = 0;
            io_info_ptr->STATS.UNDERRUNS_R  = 0;
            io_info_ptr->STATS.OVERRUNS_L   = 0;
            io_info_ptr->STATS.OVERRUNS_R   = 0;
            io_info_ptr->STATS.RX_PACKETS   = 0;
            io_info_ptr->STATS.TX_PACKETS   = 0;
            io_info_ptr->STATS.PACKETS_PROCESSED_L  = 0;
            io_info_ptr->STATS.PACKETS_QUEUED_L     = 0;
            io_info_ptr->STATS.PACKETS_REQUESTED_L  = 0;
            io_info_ptr->STATS.PACKETS_PROCESSED_R  = 0;
            io_info_ptr->STATS.PACKETS_QUEUED_R     = 0;
            io_info_ptr->STATS.PACKETS_REQUESTED_R  = 0;
            break;

        default:
            result = IO_ERROR_INVALID_IOCTL_CMD;
            break;
    }
    return result;
} /* Endbody */

/*FUNCTION****************************************************************
*
* Function Name    : _mcf54xx_i2s_format_in_data
* Returned Value   : formatted input data
* Comments         : Function performs I2S input data formatting
*
*END*********************************************************************/
uint32_t _mcf54xx_i2s_format_in_data
(
    /* [IN] data to format */
    char                *input,

    /* [IN] Requested format */
    AUDIO_DATA_FORMAT_PTR   format
)
{/* Body */
    uint32_t retval = 0;
    uint8_t  i;
    for (i = 0; i < format->SIZE; i++)
    {
        retval |= ((((uint32_t) *(input + i)) << (format->SIZE - 1 - i) * 8) & (0xFF) << (format->SIZE - 1 - i) * 8);
    }
    if (format->ENDIAN == AUDIO_LITTLE_ENDIAN)
    {
        unsigned char def[] = {sizeof(retval), 0};
        _mem_swap_endian(def, &retval);
        retval >>= sizeof(retval) * I2S_CHAR_BIT - format->BITS;
    }
    else
    {
        if ((format->ALIGNMENT == AUDIO_ALIGNMENT_RIGHT) && (format->BITS < (format->SIZE * 8)))
        {
            retval >>= (format->SIZE * I2S_CHAR_BIT) - format->BITS;
        }
    }
    retval = ~retval;
    return(retval);
}/* Endbody */

/*FUNCTION****************************************************************
*
* Function Name    : _mcf54xx_i2s_format_out_data
* Returned Value   : none
* Comments         : Function performs I2S output data formatting
*
*END*********************************************************************/
void _mcf54xx_i2s_format_out_data
(
    /* [IN] data to format */
    uint32_t                 input,

    /* [OUT] location for output */
    char                *output,

    /* [IN] Requested format */
    AUDIO_DATA_FORMAT_PTR   format
)
{
    uint8_t i;
    input = ~input;
    for(i = 0; i < format->SIZE; i++)
    {
       *(output + i) = (char) (input >> I2S_CHAR_BIT * (format->SIZE - i - 1) & 0xFF);
    }
    if (format->ENDIAN == AUDIO_BIG_ENDIAN)
    {
      if ((format->ALIGNMENT == AUDIO_ALIGNMENT_LEFT) && (format->BITS < format->SIZE * 8))
        {
            input <<= (format->SIZE * I2S_CHAR_BIT) - format->BITS;
        }
    }
    else
    {
      unsigned char def[2];
      def[0] = (unsigned char) format->SIZE;
      def[1] = 0;
        _mem_swap_endian(def, output);
    }

    return;
}

/*FUNCTION****************************************************************
*
* Function Name    : _mcf54xx_i2s_sine_wave
* Returned Value   : samples number if successful, 0 otherwise
* Comments         :
*    This function generate a sine wave.
*
*END*********************************************************************/

uint32_t _mcf54xx_i2s_sine_wave
(
    uint32_t     fs,
    uint32_t     freq,
    int16_t **output
)
{
    uint32_t samples, i;
    bool over180 = 0;
    ieee_single period, step, angle = 0.0;
    period = 1 / (ieee_single) freq;
    samples = fs * period;
    if (samples == 0)
    {
        return(0);
    }
    *output = (int16_t *) _mem_alloc(sizeof(int16_t) * samples);
    if (*output == NULL)
    {
        return(0);
    }
    step = 360.0 / (ieee_single) samples;
    for(i = 0; i < samples; i++)
    {
        if (angle > 180)
        {
            angle = angle - 180;
            over180 = TRUE;
        }
        *((*output) + i) =  BIT16_MAX * (4*angle*(180 - angle))/(40500 - angle * (180 - angle));
        if (over180)
        {
            *((*output) + i) = -(*((*output) + i));
        }
        angle += step;
    }
    return(samples);
}
/* EOF */
