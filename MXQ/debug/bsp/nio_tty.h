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
* See license agreement file for full license terms including other
* restrictions.
*****************************************************************************
*
* Comments:
*
*
*END************************************************************************/
#ifndef __NIO_TTY_H__
#define __NIO_TTY_H__

#include <stdint.h>
#include "nio.h"


#define IOCTL_NIO_TTY_SET_FLAGS 0x20000000

#define NIO_TTY_FLAGS_EOL_N    0x00
#define NIO_TTY_FLAGS_EOL_RN   0x01
#define NIO_TTY_FLAGS_ECHO     0x02

extern const NIO_DEV_FN_STRUCT nio_tty_dev_fn;

typedef struct
{
    char        *DEV_NAME;   /* Name for device used by ntty driver */
    uint32_t     OPEN_FLAGS; /* Flag to open device used by ntty driver*/
} NIO_TTY_INIT_DATA_STRUCT;

#endif //__ntty_h__
