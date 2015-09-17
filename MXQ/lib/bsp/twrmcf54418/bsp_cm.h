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

#ifndef __bsp_cm_h__
#define __bsp_cm_h__ 1

typedef enum {
    BSP_CLOCK_CONFIGURATION_0 = 0,
    BSP_CLOCK_CONFIGURATIONS,    /* Number of available clock configurations */
    BSP_CLOCK_CONFIGURATION_DEFAULT = BSP_CLOCK_CONFIGURATION_0
} BSP_CLOCK_CONFIGURATION;

typedef enum {
    CM_CLOCK_SOURCE_SYSTEM = 0,
    CM_CLOCK_SOURCE_BUS,
    /* Number of clock sources available */
    CM_CLOCK_SOURCES
} CM_CLOCK_SOURCE;

#ifdef __cplusplus
extern "C" {
#endif

BSP_CLOCK_CONFIGURATION _bsp_get_clock_configuration(void);

uint32_t _bsp_get_clock(
    /* [IN] clock configuration */
    const BSP_CLOCK_CONFIGURATION   clock_configuration,
    /* [IN] clock source index */
    const CM_CLOCK_SOURCE           clock_source
);

#ifdef __cplusplus
}
#endif

#endif /* __bsp_cm_h__ */
