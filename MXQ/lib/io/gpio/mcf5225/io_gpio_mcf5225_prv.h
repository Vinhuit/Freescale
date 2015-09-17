#ifndef __io_gpio_cpu_prv_h__
#define __io_gpio_cpu_prv_h__
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
*   The file contains definitions used in user program and/or in other
*   kernel modules to access GPIO pins
*
*
*END************************************************************************/

/*----------------------------------------------------------------------*/
/*
**                          CONSTANT DEFINITIONS
*/

#ifdef __cplusplus
extern "C" {
#endif

typedef enum devices {
     DEV_INPUT,
     DEV_OUTPUT
} DEVICE_TYPE;

typedef union pin_map_struct {
    struct {
        uint8_t  portte;
        uint8_t  porttf;
        uint8_t  porttg;
        uint8_t  portth;
        uint8_t  portti;
        uint8_t  unusedA;
        uint8_t  porttj;
        uint8_t  unusedB;
        uint8_t  portnq;
        uint8_t  unusedC;
        uint8_t  portan;
        uint8_t  portas;
        uint8_t  portqs;
        uint8_t  unusedD;
        uint8_t  portta;
        uint8_t  porttc;
        uint8_t  unusedE;
        uint8_t  portua;
        uint8_t  portub;
        uint8_t  portuc;
        uint8_t  portdd;
        uint8_t  unusedF[3];
    } reg;
    uint32_t memory32[6];
    uint8_t  memory8[6*4];
} GPIO_PIN_MAP, * GPIO_PIN_MAP_PTR;

#ifdef __cplusplus
}
#endif

#endif

/* EOF */
