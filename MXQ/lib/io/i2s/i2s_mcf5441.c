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
*   This include file is used to provide information needed by
*   applications using the I2S I/O functions.
*
*
*END************************************************************************/


#include <mqx.h>
#include <bsp.h>
#include "i2s.h"
#include "i2s_mcf54xx.h"
#include "i2s_mcf54xx_prv.h"


/*FUNCTION****************************************************************
*
* Function Name    : _bsp_get_i2s_base_address
* Returned Value   : address if successful, NULL otherwise
* Comments         :
*    This function returns the base register address of the corresponding I2S device.
*
*END*********************************************************************/

void *_bsp_get_i2s_base_address
(
    uint8_t dev_num
)
{
    void   *addr;

    switch (dev_num)
    {
        case 0:
            addr = (void *)&(((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->SSI0);
            break;
        case 1:
            addr = (void *)&(((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->SSI1);
            break;
        default:
            addr = NULL;
            break;
    }
    return addr;
}


/*FUNCTION****************************************************************
*
* Function Name    : _bsp_get_i2s_vector
* Returned Value   : vector number if successful, 0 otherwise
* Comments         :
*    This function returns desired interrupt vector number for specified I2S device.
*
*END*********************************************************************/

uint32_t _bsp_get_i2s_vector
(
    uint8_t dev_num
)
{
    uint32_t vector;
    switch (dev_num)
    {
        case 0:
            vector = MCF5441_INT_SSI0;
            break;
        case 1:
            vector = MCF5441_INT_SSI1;
            break;
        default:
            vector = 0;
            break;
    }
    return vector;
}


/*FUNCTION****************************************************************
*
* Function Name    : _bsp_i2s_hw_init
* Returned Value   : MQX error code
* Comments         :
*    This function initializes an I2S hardware control registers.
*
*END*********************************************************************/

_mqx_int _bsp_i2s_hw_init
(
    MCF54XX_I2S_INIT_STRUCT_PTR i2s_init_ptr,
    VMCF5441_SSI_STRUCT_PTR     ssi_reg_ptr
)
{
    /* variables */
    uint8_t data_bits    = i2s_init_ptr->DATA_BITS;
    uint8_t i2s_mode     = i2s_init_ptr->MODE;
    uint8_t clk_source   = i2s_init_ptr->CLOCK_SOURCE;
    uint8_t channel      = i2s_init_ptr->CHANNEL;
    bool tx_empty    = i2s_init_ptr->TX_DUMMY;
    VMCF5441_STRUCT_PTR pReg = _PSP_GET_IPSBAR();

    /* global */
    data_bits = (data_bits/2) - 1;
    /* SSIn disable */
    ssi_reg_ptr->SSI_CR &= ~(MCF5441_SSI_CR_SSI_EN);
    ssi_reg_ptr->SSI_CR |= (
        MCF5441_SSI_CR_SYN | /* Set synchronous mode */
        MCF5441_SSI_CR_TCH   /* Turn dual channel mode ON */
        );
    ssi_reg_ptr->SSI_TCR &= ~(MCF5441_SSI_TCR_TSHFD); /* Data transmitted MSB first */
    ssi_reg_ptr->SSI_RCR &= ~(MCF5441_SSI_RCR_RSHFD); /* Data received MSB first */
    ssi_reg_ptr->SSI_TCR |= (
        MCF5441_SSI_TCR_TSCKP | /* Data clocked out on falling edge of bit clock */
        MCF5441_SSI_TCR_TFSI  | /* Transmit frame sync is active low */
        MCF5441_SSI_TCR_TEFS  | /* Transmit frame sync is initiated one bit before the data transmits */
        MCF5441_SSI_TCR_TFEN0 | /* Transmit FIFO0 enable */ 
        MCF5441_SSI_TCR_TFEN1   /* Transmit FIFO1 enable */
        );
    ssi_reg_ptr->SSI_FCSR |= (
        MCF5441_SSI_FCSR_TFWM0(0x5) | /* Set transmit FIFO0 watermark to 5 */
        MCF5441_SSI_FCSR_TFWM1(0x5) | /* Set transmit FIFO1 watermark to 5 */
        MCF5441_SSI_FCSR_RFWM0(0x8) | /* Set receive FIFO0 and 1 watermark to 8 */
        MCF5441_SSI_FCSR_RFWM1(0x8)
        );
    ssi_reg_ptr->SSI_RCR |= (
        MCF5441_SSI_RCR_RSCKP | /* Data latched on rising edge of bit clock */
        MCF5441_SSI_RCR_RFSI  | /* Receive frame sync is active low */
        MCF5441_SSI_RCR_REFS  | /* Receive frame sync is initiated one bit before the data is received */
        MCF5441_SSI_RCR_RFEN0 | /* Receive FIFO0 enable */
        MCF5441_SSI_RCR_RFEN1   /* Receive FIFO1 enable */
        );
    ssi_reg_ptr->SSI_CCR |= (MCF5441_SSI_CCR_WL((uint32_t) data_bits)); //Set word length (valid data bits)
    ssi_reg_ptr->SSI_CCR |= (MCF5441_SSI_CCR_DC(0x01));

    /* Interrupts setup */
    ssi_reg_ptr->SSI_IER |= (
        MCF5441_SSI_IER_TUE0 | /* Transmit underrun error 0*/
        MCF5441_SSI_IER_TFE0 | /* Transmit FIFO0 empty */
        MCF5441_SSI_IER_ROE0 | /* Receive overrun error 0*/
        MCF5441_SSI_IER_RFF0 | /* Receive FIFO0 full */
        MCF5441_SSI_IER_TUE1 | /* Transmit underrun error 1*/
        MCF5441_SSI_IER_TFE1 | /* Transmit FIFO1 empty */
        MCF5441_SSI_IER_ROE1 | /* Receive overrun error 1*/
        MCF5441_SSI_IER_RFF1   /* Receive FIFO1 full */
        );
    ssi_reg_ptr->SSI_IER &= ~(
        MCF5441_SSI_IER_RIE  | /* Receiver disable interrupts */
        MCF5441_SSI_IER_TIE  | /* Transmitter disable interrupts */
        MCF5441_SSI_IER_TDE0 | /* Transmitter data empty 0 */
        MCF5441_SSI_IER_TDE1 | /* Transmitter data empty 1 */
        MCF5441_SSI_IER_RDR0 | /* Receiver data ready 0 */
        MCF5441_SSI_IER_RDR1   /* Receiver data ready 1 */
       );
    /* mode specific */
    switch(i2s_mode)
    {
        case I2S_MODE_MASTER:
            ssi_reg_ptr->SSI_CR |= (MCF5441_SSI_CR_I2S_MASTER);
            if (clk_source == I2S_CLK_EXT)
            {
                /* Master clock disable */
                ssi_reg_ptr->SSI_CR &= ~(MCF5441_SSI_CR_MCE);
            }
            else
            {
                /* Master clock enable */
                ssi_reg_ptr->SSI_CR |= (MCF5441_SSI_CR_MCE);
            }
            ssi_reg_ptr->SSI_TCR |= (
                                    MCF5441_SSI_TCR_TXDIR | /* Bit clock generated internally */
                                      MCF5441_SSI_TCR_TFDIR /* Frame sync generated internally */
                                      );
            break;
        case I2S_MODE_SLAVE:
            ssi_reg_ptr->SSI_CR  |= (MCF5441_SSI_CR_I2S_SLAVE);
            /* Master clock disabled */
            ssi_reg_ptr->SSI_CR  &= ~(MCF5441_SSI_CR_MCE);
            ssi_reg_ptr->SSI_TCR &= ~(
                MCF5441_SSI_TCR_TXDIR | /* Bit clock generated externally */
                MCF5441_SSI_TCR_TFDIR   /* Frame sync generated externally */
                );
            break;
        default:
            break;
    }

    /* clock source specific */
    switch (clk_source)
    {
        case I2S_CLK_INT:
            if (channel == 0)
            {
                /*SSI0 system clock divider = 3*/
                pReg->CCM.CDRH   |= MCF54XX_CCM_MISCCR_CDRH_SSI0DIV(SSIDIV_MIN * 2);
                /*SSI0 clock source = PLL*/
                pReg->CCM.MISCCR |= (MCF54XX_CCM_MISCCR_SSI0SRC);
            }
            else if (channel == 1)
            {
                /*SSI1 system clock divider = 3 */
                pReg->CCM.CDRH   |= MCF54XX_CCM_MISCCR_CDRH_SSI1DIV(SSIDIV_MIN * 2);
                /*SSI1 clock source = PLL*/
                pReg->CCM.MISCCR |= (MCF54XX_CCM_MISCCR_SSI1SRC);
            }
            break;
        case I2S_CLK_EXT:
            if (channel == 0)
            {
                /*SSI0 clock source = SSI_CLKIN pin*/
                pReg->CCM.MISCCR &= ~(MCF54XX_CCM_MISCCR_SSI0SRC);
            }
            else if (channel == 1)
            {
                /*SSI1 clock source = SSI_CLKIN pin*/
                pReg->CCM.MISCCR &= ~(MCF54XX_CCM_MISCCR_SSI1SRC);
            }
            break;
        default:
            break;
    }
    /* Enable receiving clocks after receiver is disabled */
    ssi_reg_ptr->SSI_CR &= ~(MCF5441_SSI_CR_RCD);
    /* Enable transmitting clocks after transmitter is disabled */
    ssi_reg_ptr->SSI_CR &= ~(MCF5441_SSI_CR_TCD);
    /*SSIn enable*/
    ssi_reg_ptr->SSI_CR |= (MCF5441_SSI_CR_SSI_EN);

    return (MQX_OK);
}

/* EOF */
