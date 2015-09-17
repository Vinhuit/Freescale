#ifndef __ioctl_h__
#define __ioctl_h__
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

#define IOCTL_ABORT                     0x1000
#define IO_IOCTL_DEVICE_IDENTIFY        0x2000 //Note: preserving old IO functionality
#define IO_IOCTL_GET_NUM_SECTORS        0x2001 //Note: preserving old IO functionality
#define IO_IOCTL_GET_BLOCK_SIZE         0x2002 //Note: preserving old IO functionality
#define IO_IOCTL_SET_BLOCK_MODE         0x2003 //Note: preserving old IO functionality
#define IO_IOCTL_GET_REQ_ALIGNMENT      0x2004 //Note: preserving old IO functionality
#define IO_IOCTL_CHAR_AVAIL             0x2005 //Note: preserving old IO functionality in "socket:"
#define IO_IOCTL_FLUSH_OUTPUT           0x2006 //Note: preserving old IO functionality in "socket:"
#define IO_IOCTL_SERIAL_GET_FLAGS       0x2007 //Note: preserving old IO functionality in "telnet:"
#define IO_IOCTL_SERIAL_SET_FLAGS       0x2008 //Note: preserving old IO functionality in "telnet:"


//FIXME: duplicate of the definitions of IO for device type
#define IO_DEV_TYPE_PHYS_CAN                 (0x0001)
#define IO_DEV_TYPE_PHYS_ENET                (0x0002)
#define IO_DEV_TYPE_PHYS_FIREWIRE            (0x0003)
#define IO_DEV_TYPE_PHYS_FLASH               (0x0004)
#define IO_DEV_TYPE_PHYS_FLOPPY              (0x0005)
#define IO_DEV_TYPE_PHYS_HDLC                (0x0006)
#define IO_DEV_TYPE_PHYS_IDE                 (0x0007)
#define IO_DEV_TYPE_PHYS_MEMIO               (0x0008)
#define IO_DEV_TYPE_PHYS_PCCARD              (0x0009)
#define IO_DEV_TYPE_PHYS_PCFLASH             (0x000A)
#define IO_DEV_TYPE_PHYS_PCI                 (0x000B)
#define IO_DEV_TYPE_PHYS_SCSI                (0x000C)
#define IO_DEV_TYPE_PHYS_SERIAL_EE           (0x000D)
#define IO_DEV_TYPE_PHYS_SERIAL_INTERRUPT    (0x000E)
#define IO_DEV_TYPE_PHYS_SERIAL_POLLED       (0x000F)
#define IO_DEV_TYPE_PHYS_SPI                 (0x0010)
#define IO_DEV_TYPE_PHYS_USB                 (0x0011)
#define IO_DEV_TYPE_PHYS_ADV_PCCARD          (0x0012)
#define IO_DEV_TYPE_PHYS_FLASHX              (0x0013)
#define IO_DEV_TYPE_PHYS_USB_MFS             (0x0014)
#define IO_DEV_TYPE_PHYS_ADC                 (0x0015)
#define IO_DEV_TYPE_PHYS_I2C_INTERRUPT       (0x0016)
#define IO_DEV_TYPE_PHYS_I2C_POLLED          (0x0017)
#define IO_DEV_TYPE_PHYS_QSPI_POLLED         (0x0019)
#define IO_DEV_TYPE_PHYS_SDCARD              (0x001A)
#define IO_DEV_TYPE_PHYS_NANDFLASH           (0x001B)
#define IO_DEV_TYPE_PHYS_ESDHC               (0x001C)
#define IO_DEV_TYPE_PHYS_TCHSCR              (0x001D)
#define IO_DEV_TYPE_PHYS_USB_DCD_INTERRUPT   (0x001E)
#define IO_DEV_TYPE_PHYS_USB_DCD_POLLED      (0x001F)

#define IO_DEV_TYPE_LOGICAL_LAPB             (0x0001)
#define IO_DEV_TYPE_LOGICAL_LAPD             (0x0002)
#define IO_DEV_TYPE_LOGICAL_MFS              (0x0004)
#define IO_DEV_TYPE_LOGICAL_PPP              (0x0008)
#define IO_DEV_TYPE_LOGICAL_RTCS             (0x0010)
#define IO_DEV_TYPE_LOGICAL_SDLC             (0x0020)



//Temporary defining elements in the identify array
#define IO_IOCTL_ID_PHY_ELEMENT          (0)
#define IO_IOCTL_ID_LOG_ELEMENT          (1)
#define IO_IOCTL_ID_ATTR_ELEMENT         (2)
#define IO_DEV_ATTR_ERASE                    (0x0001)
#define IO_DEV_ATTR_INTERRUPT                (0x0002)
#define IO_DEV_ATTR_POLL                     (0x0004)
#define IO_DEV_ATTR_READ                     (0x0008)
#define IO_DEV_ATTR_REMOVE                   (0x0010)
#define IO_DEV_ATTR_SEEK                     (0x0020)
#define IO_DEV_ATTR_WRITE                    (0x0040)
#define IO_DEV_ATTR_SW_FLOW_CONTROL          (0x0080)
#define IO_DEV_ATTR_HW_FLOW_CONTROL          (0x0100)

#define IO_DEV_ATTR_BLOCK_MODE               (0x0200)

#endif // __ioctl_h__
