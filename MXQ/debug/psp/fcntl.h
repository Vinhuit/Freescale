#ifndef __fcntl_h__
#define __fcntl_h__
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

/* flag values for open and fcntl */
#define O_RDONLY        0x0000          /* open for reading only */
#define O_WRONLY        0x0001          /* open for writing only */
#define O_RDWR          0x0002          /* open for reading and writing */
#define O_ACCMODE       0x0003          /* mask for above modes */
#define O_NONBLOCK      0x0004          /* no delay */
#define O_APPEND        0x0008          /* set append mode */
#define O_CREAT         0x0200          /* create if nonexistant */
#define O_TRUNC         0x0400          /* truncate to zero length */
#define O_EXCL          0x0800          /* error if already exists */
#define O_SYNC
#define O_NOCTTY

#if defined ( __IAR_SYSTEMS_ICC__ )
/* MISRA C 2004 rule 20.1 suppress: Error[Pm098]: Reserved words and standard library function names shall not be redefined or undefined */
/* MISRA C 2004 rule 20.2 suppress: Error[Pm150]: The names of standard library macros, objects, and functions shall not be reused */
_Pragma ("diag_suppress= Pm098, Pm150")
#endif
/* whence values for lseek */
#ifndef SEEK_SET
#define SEEK_SET        0       /* set file offset to offset */
#define SEEK_CUR        1       /* set file offset to current plus offset */
#define SEEK_END        2       /* set file offset to EOF plus offset */
#endif // SEEK_SET
#if defined ( __IAR_SYSTEMS_ICC__ )
_Pragma ("diag_default= Pm098, Pm150")
#endif

#endif // __fcntl_h__
