#ifndef __nio_h__
#define __nio_h__
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

#include <stdint.h>
#include <stdarg.h>

#include "fsl_os_abstraction.h"


/** POSIX API definition
 */
//#define open(name, flags, ...)      _nio_open(name, flags, __VA_ARGS__)
#define open(name, flags)      _nio_open(name, flags, 0)
#define read(fd, buf, n)            _nio_read(fd, buf, n)
#define write(fd, buf, n)           _nio_write(fd, buf, n)
#define close(fd)                   _nio_close(fd)
#define ioctl(fd, req, ...)         _nio_ioctl(fd, req, __VA_ARGS__)
#define lseek(fd, offset, whence)   _nio_lseek(fd, offset, whence)

/** io defines
 */
//The maximum driver name length including zero-ending character
#define NIO_DEV_NAME_LEN        20

/** open function type for low level driver
 */
typedef int (*NIO_OPEN_FPTR)(void *dev_context, const char *dev_name, int flags, void **fp_context);
typedef int (*NIO_READ_FPTR)(void *dev_context, void *fp_context, void *buf, size_t nbytes);
typedef int (*NIO_WRITE_FPTR)(void *dev_context, void *fp_context, const void *buf, size_t nbytes);
typedef off_t (*NIO_LSEEK_FPTR)(void *dev_context, void *fp_context, off_t offset, int whence);
typedef int (*NIO_IOCTL_FPTR)(void *dev_context, void *fp_context, unsigned long int request, va_list ap);
typedef int (*NIO_CLOSE_FPTR)(void *dev_context, void *fp_context);
typedef int (*NIO_INIT_FPTR)(void *init_data, void **dev_context);
typedef int (*NIO_DEINIT_FPTR)(void *dev_context);

/** io device functions callbacks
 */
typedef struct nio_dev_fn_struct {
    NIO_OPEN_FPTR OPEN;            ///< io driver open fn
    NIO_READ_FPTR READ;            ///< io driver read fn
    NIO_WRITE_FPTR WRITE;          ///< io driver write fn
    NIO_LSEEK_FPTR LSEEK;          ///< io driver lseek fn
    NIO_IOCTL_FPTR IOCTL;          ///< io driver ioctl fn
    NIO_CLOSE_FPTR CLOSE;          ///< io driver close fn
    NIO_INIT_FPTR INIT;            ///< io driver initialization fn (call from install)
    NIO_DEINIT_FPTR DEINIT;        ///< io driver deinitialization fn (call from uninstall)
} NIO_DEV_FN_STRUCT;

/** io device
 */
typedef struct nio_dev_struct {
    const NIO_DEV_FN_STRUCT *FN;     ///< pointer to io driver fn list
    void *CONTEXT;              ///< device context - deviced specific allocated data
    uint32_t USAGE;             ///< usage counter, counting actual  running io driver fn calls (read, write, ioctl)
    semaphore_t LOCK;          ///< device lock

    struct nio_file *FP;           ///< opened fp list for device
} NIO_DEV_STRUCT;

/** Initialize io subsystem.
 * \return successful completion return 0
 */
int _nio_init(int max_fd);

/** Install io device.
 * Function install io device to the system. If device is installed in system, return this installed device.
 * \param name < name of installing device
 * \param fn < device low level functions list
 * \param init_data < io device initialization data
 * \return successful completion return pointer to allocated io device structure
 */
NIO_DEV_STRUCT* _nio_dev_install(const char *name, const NIO_DEV_FN_STRUCT *fn, void *init_data);

/** Uninstall io device
 * \param name < name of uninstalling device
 * \return successful completion return 0
 */
int _nio_dev_uninstall(const char *name);

/** Force uninstall io device
 * \param name < name of uninstalling device
 * \return successful completion return 0
 */
int _nio_dev_uninstall_force(const char *name);

/** Open file or device
 * \param name <
 * \param flags <
 * \param mode <
 * \return return the new file descriptor, or -1 if an error occurred (errno)
 */
int _nio_open(const char *name, int flags, ...);

/** Read from a file descriptor
 *  Read up to count bytes from file descriptor fd into the buffer starting at buf.
 * \param fd < file descriptor
 * \param buf < buffer start address
 * \param nbytes < count bytes for read
 * \return On success, the number of bytes read is returned, on error,
 * -1 is returned, and errno is set.
 */
ssize_t _nio_read(int fd, void *buf, size_t nbytes);

/** Write to a file descriptor
 *  Writes up to count bytes from the buffer pointed buf to the file referred to by the file descriptor fd.
 * \param fd < file descriptor
 * \param buf < buffer start address
 * \param nbytes < count bytes for write
 * \return On success, the number of bytes written is returned, on error,
 * -1 is returned, and errno is set to indicate the error
 */
ssize_t _nio_write(int fd, const void *buf, size_t nbytes);

/** Move the read/write file offset
 * The function shall set the file offset for the open file description associated with the file descriptor fildes.
 * \param fd < file descriptor
 * \param offset < offset value
 * \param whence < base of offset calculation (SEEK_SET, SEEK_CUR, SEEK_END)
 * \return Upon successful completion, the resulting offset, as measured in bytes from the beginning of the
 * file, shall be returned. Otherwise, (off_t)-1 shall be returned, errno shall be set to indicate the
 * error, and the file offset shall remain unchanged.
 * -1 is returned, and errno is set to indicate the error
 */
off_t _nio_lseek(int fd, off_t offset, int whence);

/** IOCTL - control device
 * \param fd < file descriptor
 * \param request < request code
 * \return On success, return >=0, on error -1 is returned, and errno is set to indicate the error
 */
int _nio_ioctl(int fd, unsigned long int request, ...);

/** Close a file descriptor
 * \param fd < file descriptor
 * \return successful completion return 0. Otherwise -1 is returned and errno is set to indicate the error
 */
int _nio_close(int fd);

#endif // __nio_h__
