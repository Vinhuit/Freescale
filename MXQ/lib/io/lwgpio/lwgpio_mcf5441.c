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
*   This file contains the GPIO standard functions used on boards
*
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"
#include "lwgpio.h"

enum {
    PODR_A=0,PODR_B,PODR_C,
    PODR_D, PODR_E,PODR_F,
    PODR_G,PODR_H,PODR_I,
    PODR_J,PODR_K
};
enum {
    PAR_FBCTL   = 0,
    PAR_BE      = 1,
    PAR_CS      = 2,
    PAR_CANI2C  = 3,
    PAR_IRQH    = 4,
    PAR_IRQL    = 5,
    PAR_DSPIOWH = 6,
    PAR_DSPIOWL = 7,
    PAR_TIMER   = 8,
    PAR_UART2   = 9,
    PAR_UART1   = 10,
    PAR_UART0   = 11,
    PAR_SDHCH   = 12,
    PAR_SDHCL   = 13,
    PAR_SIMP0H  = 14,
    PAR_SIMP0L  = 15,
    PAR_SSI0H   = 16,
    PAR_SSI0L   = 17,
    PAR_DEBUGH1 = 18,
    PAR_DEBUGH0 = 19,
    PAR_DEBUGL  = 20,
    DUMMY0      = 21,
    PAR_FEC     = 22
};

enum {
    REG_PORT = 0,
    REG_DDR = 1,
    REG_SET = 2, /* also REG_PORTP */
    REG_CLR = 3,
    REG_PAR = 4,
    REG_PCR = 5
};

#define LWGPIO_PIN_FROM_ID(id) (((id) & LWGPIO_PIN_MASK) >> LWGPIO_PIN_SHIFT)
#define LWGPIO_PORT_FROM_ID(id) (((id) & LWGPIO_PORT_MASK) >> LWGPIO_PORT_SHIFT)

/* Interrupt mode definitions */
#define LWGPIO_INT_MODE_SHIFT (11)
#define LWGPIO_INT_MODE_MASK (0x03 << LWGPIO_INT_MODE_SHIFT)
#define LWGPIO_INT_MODE(x) (((x) << LWGPIO_INT_MODE_SHIFT) & LWGPIO_INT_MODE_MASK)
#define LWGPIO_INT_MODE_FROM_ID(id) (((id) & LWGPIO_INT_MODE_MASK) >> LWGPIO_INT_MODE_SHIFT)

/* Direction field definitions */
#define LWGPIO_DIR_SHIFT (13)
#define LWGPIO_DIR_MASK (0x01 << LWGPIO_DIR_SHIFT)
#define LWGPIO_PULL_UP_ENABLED      0x01
#define LWGPIO_PULL_DOWN_ENABLED    0x02

/* Flags for Pull-up/down */
#define LWGPIO_PULL_UP_SHIFT    (14)
#define LWGPIO_PULL_DOWN_SHIFT  (15)
#define LWGPIO_PULL_UP_MASK     (0x01 << LWGPIO_PULL_UP_SHIFT)
#define LWGPIO_PULL_DOWN_MASK   (0x01 << LWGPIO_PULL_DOWN_SHIFT)

static volatile uint8_t *gpio_ptr[REG_PCR + 1];

static VMCF5441_EPORT_STRUCT_PTR eport_ptr;

/*FUNCTION*****************************************************************
*
* Function Name    : lwgpio_init
* Returned Value   : TRUE if succesfull, FALSE otherwise
* Comments         :
*    Decodes ID to HW specific struct and then performs pin initialization
*
*END*********************************************************************/
bool lwgpio_init
(
    /* Pointer to LWGPIO internal structure to be filled in */
    LWGPIO_STRUCT_PTR handle,
    /* Pin ID, bitmask integer value */
    LWGPIO_PIN_ID     id,
    /* Direction to be set within initialization */
    LWGPIO_DIR        dir,
    /* Value to be set within initialization */
    LWGPIO_VALUE      value
)
{ /* Body */
    static bool first_run = TRUE;
    uint32_t port_idx, pin_idx;

    if (first_run) {
        /* Update gpio_ptr to point to the register based on IPSBAR */
        gpio_ptr[REG_PORT] = &(((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->GPIO.PODR_A);
        gpio_ptr[REG_DDR] = &(((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->GPIO.PDDR_A);
        gpio_ptr[REG_SET] = &(((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->GPIO.PPDSDR_A);
        gpio_ptr[REG_CLR] = &(((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->GPIO.PCLRR_A);
        gpio_ptr[REG_PAR] = &(((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->GPIO.PAR_FBCTL);
        (volatile uint16_t *)gpio_ptr[REG_PCR] = &(((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->GPIO.PCR_A);
        
        
        eport_ptr = &(((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->EPORT);
        first_run = FALSE;
    }
    handle->port_idx = LWGPIO_PORT_FROM_ID(id);
    handle->pinmask = 1 << LWGPIO_PIN_FROM_ID(id);
    handle->flags = id;
    /* By default, port has pins 0..7 available. There are exceptions, handled here. */
    if(handle->port_idx > 7)
        return FALSE;
    /* Set value prior to set to output */
    if (value != LWGPIO_VALUE_NOCHANGE) {
        /* Note: there is no check for values not defined as LWGPIO_VALUE enum */
        lwgpio_set_value(handle, value);
    }
    if (dir != LWGPIO_DIR_NOCHANGE) {
        /* Note: there is no check for values not defined as LWGPIO_DIR enum */
        lwgpio_set_direction(handle, dir);
    }
    return TRUE;
}

/*FUNCTION*****************************************************************
* 
* Function Name    : lwgpio_set_attribute
* Returned Value   : TRUE if succesfull, FALSE otherwise
* Comments         :
*    Sets attributes
*
*END*********************************************************************/
bool lwgpio_set_attribute
(
    /* Pin handle to get function from */
    LWGPIO_STRUCT_PTR  handle,
    /* PORT attribute */
    uint32_t attribute_id,
    /* Attribute value */
    uint32_t value
)
{
    volatile uint16_t *pcr = (volatile uint16_t *)(gpio_ptr[REG_PCR]+(2*(LWGPIO_PORT_FROM_ID(handle->flags))));
    
    if (attribute_id == LWGPIO_ATTR_PULL_UP)
    {
        *pcr |= 1 << (2*LWGPIO_PIN_FROM_ID(handle->flags)); /* pull up direction */
        if (value == LWGPIO_AVAL_ENABLE)
        {
            *pcr |= value << (2*(LWGPIO_PIN_FROM_ID(handle->flags)+1));
            handle->flags |=  LWGPIO_PULL_UP_MASK;
            handle->flags &= ~LWGPIO_PULL_DOWN_MASK;
        }
        /*if pull-down enabled - can't disable pull-up */
        else 
        {
            if ((value == LWGPIO_AVAL_DISABLE) && !(handle->flags & LWGPIO_PULL_DOWN_MASK))
            {
                *pcr &= value << (2*(LWGPIO_PIN_FROM_ID(handle->flags)+1));
                handle->flags &= ~LWGPIO_PULL_UP_MASK;
            }
        }
        return TRUE;
    } else if (attribute_id == LWGPIO_ATTR_PULL_DOWN)
    {
        *pcr &= 0 << (2*LWGPIO_PIN_FROM_ID(handle->flags)); /* pull down direction */
        if (value == LWGPIO_AVAL_ENABLE)
        {
            *pcr |= value << (2*(LWGPIO_PIN_FROM_ID(handle->flags)+1));
            handle->flags &= ~LWGPIO_PULL_UP_MASK;
            handle->flags |=  LWGPIO_PULL_DOWN_MASK;
        }
        /*if pull-up enabled - can't disable pull-down */
        else
        {
            if ((value == LWGPIO_AVAL_DISABLE) && !(handle->flags & LWGPIO_PULL_UP_MASK))
            {
                *pcr &= value << (2*(LWGPIO_PIN_FROM_ID(handle->flags)+1));
                handle->flags &= ~LWGPIO_PULL_DOWN_MASK;
            }
        }
        return TRUE;
    }
    return FALSE;
}
/*FUNCTION*****************************************************************
* 
* Function Name    : lwgpio_set_functionality
* Returned Value   : void
* Comments         :
*    Sets functionality (peripheral mode) of the pin
*
*END*********************************************************************/
void lwgpio_set_functionality
(
    /* Pin handle to set function on */
    LWGPIO_STRUCT_PTR  handle,
    /* Function to be set (integer value) */
    uint32_t            function
)
{
    if(handle->port_idx== PODR_C){
        /* Create temp. new special pinmask for 8-bit registers */
        uint8_t pinmask = LWGPIO_PIN_FROM_ID(handle->flags);
        switch(pinmask){
            case 1 :
            case 4 :
            case 7 :
                {
                    volatile uint8_t *par = (gpio_ptr[REG_PAR]) + PAR_IRQH;
                    *par &=  ~(0x3<<(pinmask/2));
                    *par |=  function<<(pinmask/2);
                }
                break;
            case 2 :
            case 3 :
            case 6 :
                {
                    volatile uint8_t *par = (gpio_ptr[REG_PAR]) + PAR_IRQL;
                    *par &=  ~(0x3 << pinmask);
                    *par |= function << pinmask;
                }
                break;
        }
    }
    else {
        /*default as GPIO function */
    }
}

/*FUNCTION*****************************************************************
*
* Function Name    : lwgpio_get_functionality
* Returned Value   : void
* Comments         :
*    Gets function (peripheral mode) of the pin
*
*END*********************************************************************/
uint32_t lwgpio_get_functionality
(
    /* Pin handle to get function from */
    LWGPIO_STRUCT_PTR  handle
)
{
    uint32_t retval;
    /* Create temp. new special pinmask for 16-bit registers */
    uint8_t pinmask = 0x03 << (2 * LWGPIO_PIN_FROM_ID(handle->flags));
    /* Get 16-bit PAR value for specific port, mask to get 2-bits for specific pin */
    uint8_t parval = *(gpio_ptr[REG_PAR] + handle->port_idx) & handle->pinmask;
    retval = (parval >> (2 * LWGPIO_PIN_FROM_ID(handle->flags))) & 0x03;
    return retval;
}


/*FUNCTION*****************************************************************
*
* Function Name    : lwgpio_set_direction
* Returned Value   : void
* Comments         :
*    Sets direction of the pin
*
*END*********************************************************************/
void lwgpio_set_direction
(
    /* Pin handle to set direction on */
    LWGPIO_STRUCT_PTR  handle,
    /* Direction to be set */
    LWGPIO_DIR         dir
)
{
    if (dir == LWGPIO_DIR_INPUT) {
        *(gpio_ptr[REG_DDR] + handle->port_idx) &= ~handle->pinmask;
        handle->flags |= LWGPIO_DIR_MASK; /* mark the pin is input */
    }
    else if (dir == LWGPIO_DIR_OUTPUT) {
        *(gpio_ptr[REG_DDR] + handle->port_idx) |= handle->pinmask;
        handle->flags &= ~LWGPIO_DIR_MASK; /* mark the pin is output */
    }
    else { /* LWGPIO_DIR_NOCHANGE or other value */
    }
}

/*FUNCTION*****************************************************************
*
* Function Name    : lwgpio_set_value
* Returned Value   : void
* Comments         :
*    Sets value (output latch) of the pin
*
*END*********************************************************************/
void lwgpio_set_value
(
    /* Pin handle to set value on */
    LWGPIO_STRUCT_PTR  handle,
    /* Direction to be set */
    LWGPIO_VALUE       out_value
)
{
    if (out_value == LWGPIO_VALUE_LOW) {
        *(gpio_ptr[REG_CLR] + handle->port_idx) = ~handle->pinmask;
    }
    else if (out_value == LWGPIO_VALUE_HIGH) {
        *(gpio_ptr[REG_SET] + handle->port_idx) = handle->pinmask;
    }
    else { /* LWGPIO_VALUE_NOCHANGE or other value */
    }
}

/*FUNCTION*****************************************************************
*
* Function Name    : lwgpio_toggle_value
* Returned Value   : void
* Comments         :
*    Toggles value of output latch of the pin
*
*END*********************************************************************/
void lwgpio_toggle_value
(
    /* Pin handle to toggle value on */
    LWGPIO_STRUCT_PTR  handle
)
{
    lwgpio_set_value(handle, lwgpio_get_value(handle) == LWGPIO_VALUE_HIGH ? LWGPIO_VALUE_LOW : LWGPIO_VALUE_HIGH);
}

/*FUNCTION*****************************************************************
*
* Function Name    : lwgpio_get_value
* Returned Value   : LWGPIO_VALUE of pin status
* Comments         :
*    Returns value (output latch or read data) of the pin
*
*END*********************************************************************/
LWGPIO_VALUE lwgpio_get_value
(
    /* Pin handle to get value from */
    LWGPIO_STRUCT_PTR  handle
)
{
//    /* if the port is set to the interrupt functionality */
//    if ((handle->port_idx == PODR_C) && (eport_ptr->EPIER & handle->pinmask))
//        return (eport_ptr->EPPDR & handle->pinmask) ? LWGPIO_VALUE_HIGH : LWGPIO_VALUE_LOW;
    /* if the pin is set to output */
    if (*(gpio_ptr[REG_DDR] + handle->port_idx) & handle->pinmask) /* alternative: (handle->flags & LWGPIO_DIR_MASK == 0) */
        return (*(gpio_ptr[REG_PORT] + handle->port_idx) & handle->pinmask) ? LWGPIO_VALUE_HIGH : LWGPIO_VALUE_LOW;
    else
    /* Note: REG_SET is an union with REG_PORTP = getting value */
        return (*(gpio_ptr[REG_SET] + handle->port_idx) & handle->pinmask) ? LWGPIO_VALUE_HIGH : LWGPIO_VALUE_LOW;
}

/*FUNCTION*****************************************************************
*
* Function Name    : lwgpio_get_raw
* Returned Value   : LWGPIO_VALUE of pin status
* Comments         :
*    Returns read level of the pin
*
*END*********************************************************************/
LWGPIO_VALUE lwgpio_get_raw
(
    /* Pin handle to get value from */
    LWGPIO_STRUCT_PTR  handle
)
{
//    /* if the port is set to the interrupt functionality */
//    if ((handle->port_idx == PODR_C) && (eport_ptr->EPIER & handle->pinmask))
//        return (eport_ptr->EPPDR & handle->pinmask) ? LWGPIO_VALUE_HIGH : LWGPIO_VALUE_LOW;
    /* Note: REG_SET is an union with REG_PORTP = getting value */
    return (*(gpio_ptr[REG_SET] + handle->port_idx) & handle->pinmask) ? LWGPIO_VALUE_HIGH : LWGPIO_VALUE_LOW;
}

/*FUNCTION*****************************************************************
*
* Function Name    : lwgpio_int_init
* Returned Value   : TRUE if succesfull, FALSE otherwise
* Comments         :
*    Initializes pin to generate interrupt
*
*END*********************************************************************/
bool lwgpio_int_init
(
    /* Pin handle to initialize interrupt on */
    LWGPIO_STRUCT_PTR handle,
    /* Interrupt mode */
    LWGPIO_INT_MODE   mode
)
{ /* Body */
    /* Create temp. new special pinmask for 16-bit registers */
    uint16_t pinmask = 0x03 << (2 * LWGPIO_PIN_FROM_ID(handle->flags));
    VMCF5441_STRUCT_PTR  reg_ptr;
    reg_ptr = _PSP_GET_IPSBAR();
    reg_ptr->SCM_PMM.PMM.PPMCR0 = 36; /* enable EPORT clock */
    if (handle->port_idx !=  PODR_C)
        return FALSE; /* MCF54418 ONLY: interrupts are allowed only on PODR_C */
    if ((0x01 << LWGPIO_PIN_FROM_ID(handle->flags)) & 0x01)  /* MCF54418 ONLY: interrupts are allowed only on pin 1, 2,3,4, 5,6 and 7 */
        return FALSE;
    if (mode > 0x04)
        return FALSE; /* interrupts are only rising, falling, rising+falling and high */
    if (mode == LWGPIO_INT_MODE_NONE)
        return TRUE; /* don't setup interrupts */
    /* setup EPPAR */
    switch (mode) {
        case LWGPIO_INT_MODE_HIGH:
            eport_ptr->EPPAR &= ~pinmask;
            break;
        case LWGPIO_INT_MODE_FALLING | LWGPIO_INT_MODE_RISING:
            eport_ptr->EPPAR |= pinmask;
            break;
        case LWGPIO_INT_MODE_FALLING:
        case LWGPIO_INT_MODE_RISING:
            eport_ptr->EPPAR &=~pinmask;
            eport_ptr->EPPAR |= (mode & 0x03) << (2 * LWGPIO_PIN_FROM_ID(handle->flags));
            break;
    }
    return TRUE;
}

/*FUNCTION*****************************************************************
*
* Function Name    : lwgpio_int_enable
* Returned Value   : void
* Comments         :
*    Enables / disables interrupts for specified pin
*
*END*********************************************************************/
void lwgpio_int_enable
(
    /* Pin handle to enable interrupt on */
    LWGPIO_STRUCT_PTR handle,
    /* Enable or disable interrupt? TRUE = enable */
    bool           ena
)
{ /* Body */
    if (ena) {
        /* enable pin interrupts */
        eport_ptr->EPIER |= handle->pinmask;
    }
    else {
        /* disable pin interrupts */
        eport_ptr->EPIER &= ~handle->pinmask;
    }
}

/*FUNCTION*****************************************************************
*
* Function Name    : lwgpio_int_get_flag
* Returned Value   : TRUE if interrupt flag is set
* Comments         :
*    Checks if there is pending interrupt flag for specified pin
*
*END*********************************************************************/
bool lwgpio_int_get_flag
(
    /* Pin handle to get interrupt flag on */
    LWGPIO_STRUCT_PTR handle
)
{ /* Body */
    return (eport_ptr->EPFR & handle->pinmask) ? TRUE : FALSE;
}

/*FUNCTION*****************************************************************
*
* Function Name    : lwgpio_int_clear_flag
* Returned Value   : void
* Comments         :
*    Clears pending interrupt flag on peripheral
*
*END*********************************************************************/
void lwgpio_int_clear_flag
(
    /* Pin handle to clear interrupt flag on */
    LWGPIO_STRUCT_PTR handle
)
{ /* Body */
    /* Clear interrupt flag. Note that if an interrupt was acknowledged by NVIC,
    it will generate interrupt anyway. Interrupts on NVIC are grouped for each
    port */
    eport_ptr->EPFR = handle->pinmask;
}


/*FUNCTION****************************************************************
*
* Function Name    : _bsp_int_get_vector
* Returned Value   : vector index
* Comments         :
*    This function returns vector index for specified pin
*
*END*********************************************************************/
uint32_t lwgpio_int_get_vector
(
    /* Pin handle to get vector of */
    LWGPIO_STRUCT_PTR  handle
)
{
    return  MCF5441_INT_EPFR0_EPF1 - 1 + LWGPIO_PIN_FROM_ID(handle->flags);
}

/* EOF */
