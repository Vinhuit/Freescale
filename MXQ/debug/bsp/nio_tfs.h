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
#ifndef __NIO_TFS_H__
#define __NIO_TFS_H__

#include <stdint.h>
#include "nio.h"

#define IOCTL_NIO_TFS_GET_NAME              0x10000000
#define IOCTL_NIO_TFS_GET_ATTRIBUTES        0x10000001
#define IOCTL_NIO_TFS_GET_LENGTH            0x10000002
#define IOCTL_NIO_TFS_GET_LAST_ERROR        0x10000003

extern const NIO_DEV_FN_STRUCT nio_tfs_dev_fn;

/** TFS directory entry information
 */
typedef struct nio_tfs_dir_entry {
    char *NAME;
    uint32_t FLAGS;
    unsigned char *DATA;
    uint32_t SIZE;
} NIO_TFS_DIR_ENTRY;

/** TFS initialization data structure
 */
typedef struct {
    const NIO_TFS_DIR_ENTRY *ROOT;
} NIO_TFS_INIT_DATA_STRUCT;


#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif
