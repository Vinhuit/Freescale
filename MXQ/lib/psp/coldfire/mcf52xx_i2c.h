/*HEADER**********************************************************************
*
* Copyright 2008 Freescale Semiconductor, Inc.
* Copyright 1989-2008 ARC International
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
*   This file contains the type definitions for the mcf52xx I2C module.
*
*
*END************************************************************************/

#ifndef __mcf52xx_i2c_h__
#define __mcf52xx_i2c_h__

#define __mcf52xx_i2c_h__version "$Version:3.5.3.0$"
#define __mcf52xx_i2c_h__date    "$Date:Nov-26-2009$"

#ifdef __cplusplus
extern "C" {
#endif


/*------------------------------------------------------------------------*/
/*
** I2C registers
*/

/* Bit definitions and macros for MCF52XX_I2C_I2ADR */
#define MCF52XX_I2C_I2ADR_ADR(x)                 (((x)&0x7F)<<0x1)

/* Bit definitions and macros for MCF52XX_I2C_I2FDR */
#define MCF52XX_I2C_I2FDR_IC(x)                  (((x)&0x3F)<<0)

/* Bit definitions and macros for MCF52XX_I2C_I2CR */
#define MCF52XX_I2C_I2CR_RSTA                    (0x4)
#define MCF52XX_I2C_I2CR_TXAK                    (0x8)
#define MCF52XX_I2C_I2CR_MTX                     (0x10)
#define MCF52XX_I2C_I2CR_MSTA                    (0x20)
#define MCF52XX_I2C_I2CR_IIEN                    (0x40)
#define MCF52XX_I2C_I2CR_IEN                     (0x80)

/* Bit definitions and macros for MCF52XX_I2C_I2SR */
#define MCF52XX_I2C_I2SR_RXAK                    (0x1)
#define MCF52XX_I2C_I2SR_IIF                     (0x2)
#define MCF52XX_I2C_I2SR_SRW                     (0x4)
#define MCF52XX_I2C_I2SR_IAL                     (0x10)
#define MCF52XX_I2C_I2SR_IBB                     (0x20)
#define MCF52XX_I2C_I2SR_IAAS                    (0x40)
#define MCF52XX_I2C_I2SR_ICF                     (0x80)


/*------------------------------------------------------------------------*/

/*
** MCF52XX_I2C_STRUCT
** This structure defines what one i2c register look like
*/

typedef struct mcf52xx_i2c_struct
{
   unsigned char    I2ADR;          // I2C address register
   unsigned char    filler1[3];
   unsigned char    I2FDR;          // I2C frequency divider register
   unsigned char    filler2[3];
   unsigned char    I2CR;           // I2C control register
   unsigned char    filler3[3];
   unsigned char    I2SR;           // I2C status register
   unsigned char    filler4[3];
   unsigned char    I2DR;           // I2C data I/O register
   unsigned char    filler5[47];
} MCF52XX_I2C_STRUCT, * MCF52XX_I2C_STRUCT_PTR;
typedef volatile struct mcf52xx_i2c_struct * VMCF52XX_I2C_STRUCT_PTR;

#ifdef __cplusplus
}
#endif

#endif  //__mcf52xx_i2c_h__
