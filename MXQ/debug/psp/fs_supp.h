#ifndef _FS_SUPP_H_
#define _FS_SUPP_H_
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
 * 
 *END************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* Definitions for filesystem table */
#define FS_MAX_DEVLEN         (8)
#define MAX_FS_INSTANCES      (4)


int strtolower(char *arg);
int _nio_supp_validate_device(char *arg);




/*!
 * \brief File system table entry structure.
 */ 
typedef struct 
{
   /*! \brief File system name. */
   char FS_NAME[FS_MAX_DEVLEN];
   /*! \brief Pointer MQX File. */
   int FD;
} FS_TABLE_ENTRY;

int _io_validate_device(char *arg);

int _io_get_first_valid_fs(void);
int _io_get_fs_by_name(char *fs_name);
int _io_get_fs_name(int fd, char *fs_name, int32_t fs_name_len);
int _io_is_fs_valid(int fd);

uint32_t _io_register_file_system(int fd, char *name_ptr);
uint32_t _io_unregister_file_system(int fd);

int32_t _io_get_dev_for_path(char *out_dev, bool * is_dev_in_path, int32_t dev_len, char *input_path, char *cur_dev);
int32_t _io_path_add(char *result, int32_t len, char *path);
int32_t _io_rel2abs(char *result, char *curdir, char *inputpath, int32_t len, char *cur_dev);
void _io_create_prefixed_filename( char *new_ptr, char *in_ptr,  char *dev_ptr);


#endif // _FS_SUPP_H_
