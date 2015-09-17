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
#ifndef __lwgpio_mcf51jf_h__
#define __lwgpio_mcf51jf_h__ 1

#define LWGPIO_PIN_VALID (0x80000000) /* Note: this is for backwards compatibility with GPIO driver only */

/* Pin field definitions */
#define LWGPIO_PIN_SHIFT (0)
#define LWGPIO_PIN_MASK  (0x7 << LWGPIO_PIN_SHIFT)
#define LWGPIO_PIN(x)    (((x) << LWGPIO_PIN_SHIFT) & LWGPIO_PIN_MASK)

/* Port field definitions */
#define LWGPIO_PORT_MUX_MASKL  (0x0F)
#define LWGPIO_PORT_MUX_MASKH  (0xF0)
#define LWGPIO_PORT_MUX_SHIFTH (0x04)
#define LWGPIO_PORT_MUX_SHIFTL (0x00)
#define LWGPIO_PORT_MUX_GPIOL  (0x01)
#define LWGPIO_PORT_MUX_GPIOH  (0x10)
#define LWGPIO_PORT_MUX_IRQ    (0x05)
#define LWGPIO_PORT_SHIFT      (3)
#define LWGPIO_PORT_MASK       (0xFF << LWGPIO_PORT_SHIFT)
#define LWGPIO_PORT(x) (((x) << LWGPIO_PORT_SHIFT) & LWGPIO_PORT_MASK)
#define LWGPIO_PORT_A (LWGPIO_PORT(0) | LWGPIO_PIN_VALID)
#define LWGPIO_PORT_B (LWGPIO_PORT(1) | LWGPIO_PIN_VALID)
#define LWGPIO_PORT_C (LWGPIO_PORT(2) | LWGPIO_PIN_VALID)
#define LWGPIO_PORT_D (LWGPIO_PORT(3) | LWGPIO_PIN_VALID)
#define LWGPIO_PORT_E (LWGPIO_PORT(4) | LWGPIO_PIN_VALID)
#define LWGPIO_PORT_F (LWGPIO_PORT(5) | LWGPIO_PIN_VALID)

#define LWGPIO_PIN0   (0)
#define LWGPIO_PIN1   (1)
#define LWGPIO_PIN2   (2)
#define LWGPIO_PIN3   (3)
#define LWGPIO_PIN4   (4)
#define LWGPIO_PIN5   (5)
#define LWGPIO_PIN6   (6)
#define LWGPIO_PIN7   (7)

typedef struct {
    uint8_t    *mxc_reg;
    uint8_t    *irq_reg;
    PT_MemMapPtr  gpio_ptr;
    PCTL_MemMapPtr pctl_ptr;
    uint8_t        pinmask; /* since struct holds one pin, pinmask will have only one bit set */
    uint32_t       flags;
    uint8_t        port_idx;
    uint8_t        pin_idx;
} LWGPIO_STRUCT, * LWGPIO_STRUCT_PTR;

#define LWGPIO_MUX_A0_GPIO  (1)
#define LWGPIO_MUX_A1_GPIO  (1)
#define LWGPIO_MUX_A2_GPIO  (1)
#define LWGPIO_MUX_A3_GPIO  (1)
#define LWGPIO_MUX_A4_GPIO  (1)
#define LWGPIO_MUX_A5_GPIO  (1)
#define LWGPIO_MUX_A6_GPIO  (1)
#define LWGPIO_MUX_A7_GPIO  (1)

#define LWGPIO_MUX_B0_GPIO  (1)
#define LWGPIO_MUX_B1_GPIO  (1)
#define LWGPIO_MUX_B2_GPIO  (1)
#define LWGPIO_MUX_B3_GPIO  (1)
#define LWGPIO_MUX_B4_GPIO  (1)
#define LWGPIO_MUX_B5_GPIO  (1)
#define LWGPIO_MUX_B6_GPIO  (1)
#define LWGPIO_MUX_B7_GPIO  (1)

#define LWGPIO_MUX_C0_GPIO  (1)
#define LWGPIO_MUX_C1_GPIO  (1)
#define LWGPIO_MUX_C2_GPIO  (1)
#define LWGPIO_MUX_C3_GPIO  (1)
#define LWGPIO_MUX_C4_GPIO  (1)
#define LWGPIO_MUX_C5_GPIO  (1)
#define LWGPIO_MUX_C6_GPIO  (1)
#define LWGPIO_MUX_C7_GPIO  (1)

#define LWGPIO_MUX_D0_GPIO  (1)
#define LWGPIO_MUX_D1_GPIO  (1)
#define LWGPIO_MUX_D2_GPIO  (1)
#define LWGPIO_MUX_D3_GPIO  (1)
#define LWGPIO_MUX_D4_GPIO  (1)
#define LWGPIO_MUX_D5_GPIO  (1)
#define LWGPIO_MUX_D6_GPIO  (1)
#define LWGPIO_MUX_D7_GPIO  (1)

#define LWGPIO_MUX_E0_GPIO  (1)
#define LWGPIO_MUX_E1_GPIO  (1)
#define LWGPIO_MUX_E2_GPIO  (1)
#define LWGPIO_MUX_E3_GPIO  (1)
#define LWGPIO_MUX_E4_GPIO  (1)
#define LWGPIO_MUX_E5_GPIO  (1)
#define LWGPIO_MUX_E6_GPIO  (1)
#define LWGPIO_MUX_E7_GPIO  (1)

#define LWGPIO_MUX_F0_GPIO  (1)
#define LWGPIO_MUX_F1_GPIO  (1)
#define LWGPIO_MUX_F2_GPIO  (1)
#define LWGPIO_MUX_F3_GPIO  (1)
#define LWGPIO_MUX_F4_GPIO  (1)
#define LWGPIO_MUX_F5_GPIO  (1)
#define LWGPIO_MUX_F6_GPIO  (1)
#define LWGPIO_MUX_F7_GPIO  (1)

/* Pin attributes */
#define LWGPIO_ATTR_BASE            (0x100)
#define LWGPIO_ATTR_SLEW_RATE       (LWGPIO_ATTR_BASE + 1)
#define LWGPIO_ATTR_DRIVE_STRENGTH  (LWGPIO_ATTR_BASE + 2)
#define LWGPIO_ATTR_PASIVE_FILTER   (LWGPIO_ATTR_BASE + 3)
#define LWGPIO_ATTR_DIGITAL_FILTER  (LWGPIO_ATTR_BASE + 4)

/* Attributes value */
#define LWGPIO_AVAL_DRIVE_STRENGTH_HIGH (1)
#define LWGPIO_AVAL_DRIVE_STRENGTH_LOW  (0)
#define LWGPIO_AVAL_DIGITAL_FILTER_BUSCLK_CYC(x)    (((x)-1)|0x100)    
#define LWGPIO_AVAL_DIGITAL_FILTER_LPOCLK_CYC(x)    (((x)-1)|0x180)

#endif
