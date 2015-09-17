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
*   This file contains board-specific SPI initialization functions.
*
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include "spi_spi16.h"

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_spi_base_address
* Returned Value   : Address upon success, NULL upon failure
* Comments         :
*    This function returns the base register address of the corresponding SPI
*    module.
*
*END*----------------------------------------------------------------------*/
void *_bsp_get_spi_base_address
(
    uint8_t dev_num
)
{
    void   *addr;
    
    switch (dev_num) 
    {
        case 0:
            addr = (void *) SPI0_BASE_PTR;
            break;
        case 1:
            addr = (void *) SPI1_BASE_PTR;
            break;
        default:
            addr = NULL;
            break;
    }
    return addr;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_spi_vector
* Returned Value   : Vector number upon success, 0 upon failure
* Comments         :
*    This function returns desired interrupt vector table index for specified 
*    SPI module and vector identifier.
*
*END*----------------------------------------------------------------------*/
uint32_t _bsp_get_spi_vector
(
    uint8_t dev_num
)
{
    uint8_t vector;

    switch (dev_num) 
    {
        case 0:
            vector = Vspi0;
            break;
        case 1:
            vector = Vspi1;
            break;
        default:
            vector = 0;
            break;
    }

    return vector;
}

/* EOF */
