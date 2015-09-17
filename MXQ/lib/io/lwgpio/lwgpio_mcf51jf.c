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

const static PT_MemMapPtr gpio_ptr_arr[] = {
    PTA_BASE_PTR,
    PTB_BASE_PTR,
    PTC_BASE_PTR,
    PTD_BASE_PTR,
    PTE_BASE_PTR,
    PTF_BASE_PTR,
};

const static PCTL_MemMapPtr pctl_ptr_arr[] = {
    PCTLA_BASE_PTR,
    PCTLB_BASE_PTR,
    PCTLC_BASE_PTR,
    PCTLD_BASE_PTR,
    PCTLE_BASE_PTR,
    PCTLF_BASE_PTR,
};
#define LWGPIO_PIN_FROM_ID(id)  (((id) & LWGPIO_PIN_MASK) >> LWGPIO_PIN_SHIFT)

#define LWGPIO_PORT_FROM_ID(id) (((id) & LWGPIO_PORT_MASK) >> LWGPIO_PORT_SHIFT)

/* Interrupt mode definitions */
#define LWGPIO_INT_MODE_SHIFT       (16)
#define LWGPIO_INT_MODE_MASK        (0x0F << LWGPIO_INT_MODE_SHIFT)
#define LWGPIO_INT_MODE(x)          (((x) << LWGPIO_INT_MODE_SHIFT) & LWGPIO_INT_MODE_MASK)
#define LWGPIO_INT_MODE_FROM_ID(id) (((id) & LWGPIO_INT_MODE_MASK) >> LWGPIO_INT_MODE_SHIFT)

/* Direction field definitions */
#define LWGPIO_DIR_SHIFT  (20)
#define LWGPIO_DIR_MASK   (0x01 << LWGPIO_DIR_SHIFT)

/* Flags for Pull-up/down */
#define LWGPIO_PULL_UP_SHIFT     (21)
#define LWGPIO_PULL_DOWN_SHIFT     (22)
#define LWGPIO_PULL_UP_MASK     (0x01 << LWGPIO_PULL_UP_SHIFT)
#define LWGPIO_PULL_DOWN_MASK     (0x01 << LWGPIO_PULL_DOWN_SHIFT)

/* Define range for digital filter */
#define LWGPIO_AVAL_DIGITAL_FILTER_MIN  (0x100)
#define LWGPIO_AVAL_DIGITAL_FILTER_MAX  (0x19F)

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
    uint32_t port_idx, pin_idx;

    port_idx = LWGPIO_PORT_FROM_ID(id);
    pin_idx  = LWGPIO_PIN_FROM_ID(id);

    handle->flags    = id;
    handle->mxc_reg  = (uint8_t *)(MXC_BASE_PTR);
    handle->irq_reg  = (uint8_t *)(IRQ_BASE_PTR);
    handle->gpio_ptr = gpio_ptr_arr[port_idx];
    handle->pctl_ptr = pctl_ptr_arr[port_idx];
    handle->pinmask  = 1 << pin_idx;
    handle->port_idx = LWGPIO_PORT_FROM_ID(id);
    handle->pin_idx  = LWGPIO_PIN_FROM_ID(id);

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
   
   switch(attribute_id) 
   {
        case(LWGPIO_ATTR_PULL_UP):
        {  
            if(value == LWGPIO_AVAL_ENABLE)
            {
                /* PULL enable */
                handle->pctl_ptr->PUE |= handle->pinmask;
                /* pullup resistor is selected */
                handle->pctl_ptr->PUS |= handle->pinmask;
                /* setting pull up flag */
                handle->flags |= LWGPIO_PULL_UP_MASK;
                handle->flags &= ~LWGPIO_PULL_DOWN_MASK;
            
            }
            else        /* value == LWGPIO_AVAL_DISABLE */
            {
                if(!(handle->flags & LWGPIO_PULL_DOWN_MASK))
                {
                    handle->pctl_ptr->PUE &= ~(handle->pinmask);
                    /* clearing pullup flag */
                    handle->flags &= ~LWGPIO_PULL_UP_MASK;
                }
                /* if pull_down flag is set do nothing and return is true*/    
            }
            break;
        }        
        case(LWGPIO_ATTR_PULL_DOWN):
        { 
            if(value == LWGPIO_AVAL_ENABLE)
            {
                /* PULL enable */
                handle->pctl_ptr->PUE |= handle->pinmask;
                /* pulldown resistor is selected */
                handle->pctl_ptr->PUS &= ~(handle->pinmask);
                /* setting pulldown flag */
                handle->flags |= LWGPIO_PULL_DOWN_MASK;
                handle->flags &= ~LWGPIO_PULL_UP_MASK;
            
            }
            else        /* value == LWGPIO_AVAL_DISABLE */
            {
                if(!(handle->flags & LWGPIO_PULL_UP_MASK))
                {
                    handle->pctl_ptr->PUE &= ~(handle->pinmask);
                    /* clearing pulldown flag */
                    handle->flags &= ~LWGPIO_PULL_DOWN_MASK;
                }
                /* if pull_up flag is set do nothing and return is true*/    
            }
            break;
        }
        case(LWGPIO_ATTR_SLEW_RATE):
        {
            if(value == LWGPIO_AVAL_ENABLE)
            {
                handle->pctl_ptr->SRE |= handle->pinmask;
            }
            else        /* value == LWGPIO_AVAL_DISABLE */
            {
                handle->pctl_ptr->SRE &= ~(handle->pinmask);
            }
            break;
        }
        case(LWGPIO_ATTR_PASIVE_FILTER):
        {
            if(value == LWGPIO_AVAL_ENABLE)
            {
                handle->pctl_ptr->PFE |= handle->pinmask;
            }
            else        /*value == LWGPIO_AVAL_DISABLE*/
            {
                handle->pctl_ptr->PFE &= ~(handle->pinmask);
            }
            break;
        }
        case(LWGPIO_ATTR_DRIVE_STRENGTH):
        {
            if(value == LWGPIO_AVAL_DRIVE_STRENGTH_HIGH)
            {
                handle->pctl_ptr->DS |= handle->pinmask;
            }
            else        /*value == LWGPIO_AVAL_DRIVE_STRENGTH_LOW*/
            {
                handle->pctl_ptr->DS &= ~(handle->pinmask);
            }        
            break;
        }
        case(LWGPIO_ATTR_DIGITAL_FILTER):
        {
            /* this attribute is available only for port B and port C */
            if((handle->port_idx != 1) && (handle->port_idx != 2))   
            {
                return FALSE;
            }
            switch(value)
            {
                case(LWGPIO_AVAL_ENABLE):
                {
                    /* default settings: bus clock and 1 clock cycle*/        
                    handle->pctl_ptr->DFE |= handle->pinmask; 
                    break;
                }
                case(LWGPIO_AVAL_DISABLE):
                {
                    handle->pctl_ptr->DFE &= ~(handle->pinmask); 
                    break;
                }
                default:
                {
                    /* Checking range */
                    if((value >= LWGPIO_AVAL_DIGITAL_FILTER_MIN ) && (value <= LWGPIO_AVAL_DIGITAL_FILTER_MAX))
                    {
                        handle->pctl_ptr->DFE |= handle->pinmask; 
                        handle->pctl_ptr->DFC = (value & 0xFF);
                    }
                    else /* Out of range */
                    {
                        return FALSE;
                    }
                }
            }         
            break;
        }
        default:
        {
           return FALSE;
        }
    }
   return TRUE;
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
    uint8_t           *mxc;
    uint8_t               port,pin,tmp;
    mxc  = handle->mxc_reg;
    port = handle->port_idx;
    pin  = handle->pin_idx;
    tmp  = port*4 + (3-pin/2);
    handle->mxc_reg += tmp ;
    if(pin & 0x01){
        *(mxc +tmp) &= ~LWGPIO_PORT_MUX_MASKH;
        *(mxc +tmp) |= LWGPIO_PORT_MUX_GPIOH;
    }
    else{
        if(function == 0){
            *(mxc +tmp) &= ~LWGPIO_PORT_MUX_MASKL;
            *(mxc +tmp) |= LWGPIO_PORT_MUX_GPIOL;
        }else if(function == 1)
        {
            if((handle->port_idx ==1)&&(handle->pin_idx ==0))
            {
                *(mxc +tmp) &= ~LWGPIO_PORT_MUX_MASKL;
                *(mxc +tmp) |= LWGPIO_PORT_MUX_IRQ;
            }
            else
            {
                *(mxc +tmp) &= ~LWGPIO_PORT_MUX_MASKL;
                *(mxc +tmp) |= LWGPIO_PORT_MUX_GPIOL;
            }
        }
    }
}

/*FUNCTION*****************************************************************
*
* Function Name    : lwgpio_get_functionality
* Returned Value   : void
* Comments         :
*    Gets functionality (peripheral mode) of the pin
*
*END*********************************************************************/
uint32_t lwgpio_get_functionality
(
    /* Pin handle to get function from */
    LWGPIO_STRUCT_PTR  handle
)
{
    if(handle->pin_idx & 0x01){
        return (*handle->mxc_reg & LWGPIO_PORT_MUX_MASKH) >> LWGPIO_PORT_MUX_SHIFTH;
    }
    else{
        return (*handle->mxc_reg & LWGPIO_PORT_MUX_MASKL) >> LWGPIO_PORT_MUX_SHIFTL;
    }
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
        handle->gpio_ptr->DD &= ~handle->pinmask;
        handle->flags |= LWGPIO_DIR_MASK; /* mark the pin is input */
    }
    else if (dir == LWGPIO_DIR_OUTPUT) {
        handle->gpio_ptr->DD |= handle->pinmask;
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
        handle->gpio_ptr->D &= ~handle->pinmask;
    }
    else if (out_value == LWGPIO_VALUE_HIGH) {
        handle->gpio_ptr->D |= handle->pinmask;
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
    /* if the pin is set to output */
    if (handle->gpio_ptr->DD & handle->pinmask) /* alternative: (handle->flags & LWGPIO_DIR_MASK == 0) */
        return (handle->gpio_ptr->D & handle->pinmask) ? LWGPIO_VALUE_HIGH : LWGPIO_VALUE_LOW;
    else
        return (handle->gpio_ptr->D & handle->pinmask) ? LWGPIO_VALUE_HIGH : LWGPIO_VALUE_LOW;
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
    return (handle->gpio_ptr->D & handle->pinmask) ? LWGPIO_VALUE_HIGH : LWGPIO_VALUE_LOW;
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

    if (mode > 4 && mode != LWGPIO_INT_MODE_LOW)
        return FALSE;
    switch (mode) {
        case LWGPIO_INT_MODE_HIGH:
            if((handle->port_idx ==1)&&(handle->pin_idx ==0))
            {
                *handle->irq_reg |= IRQ_SC_IRQMOD_MASK;
                *handle->irq_reg |= IRQ_SC_IRQEDG_MASK;
            }
            else
            {
                handle->pctl_ptr->IC  |= PCTL_IC_PTMOD_MASK;
                handle->pctl_ptr->IES |= handle->pinmask;
                handle->pctl_ptr->IPE |= handle->pinmask;
            }
            break;
        case LWGPIO_INT_MODE_FALLING:
            if((handle->port_idx ==1)&&(handle->pin_idx ==0))
            {
                *handle->irq_reg &= ~IRQ_SC_IRQMOD_MASK;
                *handle->irq_reg &= ~IRQ_SC_IRQEDG_MASK;
            }
            else
            {
                handle->pctl_ptr->IC  &= ~PCTL_IC_PTMOD_MASK;
                handle->pctl_ptr->IES &= ~handle->pinmask;
                handle->pctl_ptr->IPE |= handle->pinmask;
            }
            break;
        case LWGPIO_INT_MODE_LOW:
            if((handle->port_idx ==1)&&(handle->pin_idx ==0))
            {
                *handle->irq_reg &= ~IRQ_SC_IRQEDG_MASK;
                *handle->irq_reg |= IRQ_SC_IRQMOD_MASK;
            }
            else
            {
                handle->pctl_ptr->IC  |= PCTL_IC_PTMOD_MASK;
                handle->pctl_ptr->IES &= ~handle->pinmask;
                handle->pctl_ptr->IPE |= handle->pinmask;
            }
            break;
        case LWGPIO_INT_MODE_RISING:
            if((handle->port_idx ==1)&&(handle->pin_idx ==0))
            {
                *handle->irq_reg &= ~IRQ_SC_IRQMOD_MASK;
                *handle->irq_reg |= IRQ_SC_IRQEDG_MASK;
            }
            else
            {
                handle->pctl_ptr->IC  &= ~PCTL_IC_PTMOD_MASK;
                handle->pctl_ptr->IES |= handle->pinmask;
                handle->pctl_ptr->IPE |= handle->pinmask;
            }
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
        if((handle->port_idx ==1)&&(handle->pin_idx ==0))
        {
            *handle->irq_reg |= IRQ_SC_IRQPE_MASK;
            *handle->irq_reg |= IRQ_SC_IRQIE_MASK;
        }
        else
        {
            handle->pctl_ptr->IF |=handle->pinmask;
            handle->pctl_ptr->IC |= PCTL_IC_PTIE_MASK;
        }
    }
    else {
        /* disable pin interrupts */
        if((handle->port_idx ==1)&&(handle->pin_idx ==0))
        {
            *handle->irq_reg &= ~IRQ_SC_IRQIE_MASK;
        }
        else
        {
            handle->pctl_ptr->IC &= ~PCTL_IC_PTIE_MASK;
        }
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
    if((handle->port_idx ==1)&&(handle->pin_idx ==0))
    {
        return (*handle->irq_reg & IRQ_SC_IRQF_MASK) ? TRUE : FALSE;
    }
    else
    {
        return (handle->pctl_ptr->IF & handle->pinmask) ? TRUE : FALSE;
    }
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
    /* Clear interrupt flag.*/
    if((handle->port_idx ==1)&&(handle->pin_idx ==0))
    {
        *handle->irq_reg |= IRQ_SC_IRQACK_MASK;
    }
    else
    {
        handle->pctl_ptr->IF |=handle->pinmask;
    }
}

/*FUNCTION****************************************************************
*
* Function Name    : _bsp_get_gpio_base_address
* Returned Value   : pointer to base of GPIO registers
* Comments         :
*    This function returns base address of GPIO related register space.
*
*END*********************************************************************/
uint32_t lwgpio_int_get_vector
(
    /* Pin handle to get vector of */
    LWGPIO_STRUCT_PTR  handle
)
{
    switch (handle->port_idx) {
        case 0:
        case 1:
            if((handle->port_idx ==1)&&(handle->pin_idx ==0))
            {
                return Virq;
            }
            else
            {
                return Vportab ;
            }
            break;
        case 2:
        case 3:
            return Vportdc ;
            break;
        case 4:
        case 5:
            return Vportfe ;
            break;
    }
}

/* EOF */
