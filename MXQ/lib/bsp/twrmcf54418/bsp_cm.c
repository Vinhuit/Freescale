/*HEADER**********************************************************************
*
* Copyright 2012 Freescale Semiconductor, Inc.
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
*   Implementation of subset of clock manager API to provide clock frequencies
*   on platforms without full-featured clock manager
*
*       _bsp_get_clock_configuration();
*       _bsp_get_clock();
*
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <bsp_prv.h>



static const uint32_t cm_clock_freq[BSP_CLOCK_CONFIGURATIONS][CM_CLOCK_SOURCES] = {
  /* Clock configuration 0 */
  {
      BSP_SYSTEM_CLOCK,
      BSP_BUS_CLOCK
  }
};



BSP_CLOCK_CONFIGURATION _bsp_get_clock_configuration
(
    void
)
{
    return BSP_CLOCK_CONFIGURATION_0;
}


uint32_t _bsp_get_clock
(
    const BSP_CLOCK_CONFIGURATION   clock_configuration,
    const CM_CLOCK_SOURCE           clock_source
)
{
    if ((clock_source < CM_CLOCK_SOURCES) && (clock_configuration < BSP_CLOCK_CONFIGURATIONS))
    {
        return cm_clock_freq[clock_configuration][clock_source];
    }
    else
    {
        return 0;
    }
}
