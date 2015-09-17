
/*HEADER**********************************************************************
*
* Copyright 2013 Freescale Semiconductor, Inc.
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
* See license agreement file for full license terms including other restrictions.
*****************************************************************************
*
* Comments:
*
*   This file contains kinetis specific functions of the hwtimer component.
*
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include "hwtimer.h"


/*******************LPT********************/
void *lpt_get_base_address(uint8_t dev_num);
uint32_t lpt_get_vector(uint8_t dev_num);
_mqx_int lpt_io_init(uint32_t dev_num);
_mqx_int lpt_validate_module(uint32_t dev_num, uint32_t isr_prior);

/* Array of LPT base addresses*/
/*!
 * \cond DOXYGEN_PRIVATE
 * \brief Array of LPT base addresses
 */
static const void *lpt_address[] =
{
    (void *)LPTMR0_BASE_PTR,
    (void *)LPTMR1_BASE_PTR, 
};

/* Array of LPT interrupt vectors*/
/*!
 * \cond DOXYGEN_PRIVATE
 * \brief Array of LPT interrupt vectors
 */
static const uint32_t lpt_vectors[] =
{
    Vlptmr0,
    Vlptmr1, 
};

/* Array of LPT clock sources */
/*!
 * \cond DOXYGEN_PRIVATE
 * \brief Array of LPT interrupt vectors
 */
const int32_t lpt_pcsclk[] =
{
    BSP_HWTIMER_LPT0_DEFAULT_PCSCLK,
    BSP_HWTIMER_LPT1_DEFAULT_PCSCLK,
};

/*!
 * \cond DOXYGEN_PRIVATE
 *
 * \brief This function get base address.
 *
 * \param dev_num[in]  LPT module.
 *
 * \return base address of LPT module.
 *
 */
void *lpt_get_base_address(uint8_t dev_num)
{
    if (dev_num < ELEMENTS_OF(lpt_address)) 
    {
        return (void *)lpt_address[dev_num];
    }
    return NULL;
}

/*!
 * \cond DOXYGEN_PRIVATE
 *
 * \brief This function get Interrupt Number.
 *
 * \param dev_num[in]  LPT module.
 *
 * \return Interrupt Number for LPT module.
 *
 */
uint32_t lpt_get_vector(uint8_t dev_num)
{
    if (dev_num < ELEMENTS_OF(lpt_vectors)) 
    {
        return lpt_vectors[dev_num];
    }
    else
    {
        return 0;
    }
}

/*!
 * \cond DOXYGEN_PRIVATE
 *
 * \brief This function performs BSP-specific initialization related to LPT
 *
 * \param dev_num[in]   Number of LPT module.
 *
 * \return MQX_OK Success.
 */
_mqx_int lpt_io_init(uint32_t dev_num)
{
    return MQX_OK;
}

/*!
 * \cond DOXYGEN_PRIVATE
 *
 * \brief This function check validation of input LPT module
 *
 * \param dev_num[in]   Number of LPT module.
 * \param isr_prior[in] ISR priority.
 *
 * \return MQX_OK Valid.
 */
_mqx_int lpt_validate_module(uint32_t dev_num, uint32_t isr_prior)
{
    if (dev_num >= ELEMENTS_OF(lpt_address)) 
    {
        return MQX_INVALID_DEVICE;
    }
    
    return MQX_OK;
}

