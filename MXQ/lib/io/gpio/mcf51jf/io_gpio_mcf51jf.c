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
#include <user_config.h>
#include "mqx.h"
#include "bsp.h"
#include "../io_gpio.h"
#include "../io_gpio_prv.h"
#include <string.h>

#define MUX_EVEN         0xF0
#define MUX_ODD          0x0F
#define PORT_END         6
#define PIN_IDX_NUM      8
#define PIN_IDX_NUM_HALF PIN_IDX_NUM/2
#if BSPCFG_ENABLE_GPIODEV

/* This structure will hold 'used pins'. If somebody would like to open 'already used pin',
** the operation will fail. All open pins within all GPIO driver files will be written here.
** We can predefine this structure with 'already used' bits for those bits, which are not
** used at all - so we will prevent open operation to be successful.
*/
GPIO_PIN_MAP  gpio_global_pin_map = {
    0x00,  /* porta */
    0x00,  /* portb */
    0x00,  /* portc */
    0x00,  /* portd */
    0x00,  /* porte */
    0x00   /* portf */
};

GPIO_PIN_MAP  gpio_global_irq_map = {
    0x00,  /* porta */
    0x00,  /* portb */
    0x00,  /* portc */
    0x00,  /* portd */
    0x00,  /* porte */
    0x00   /* portf */
};

static const volatile PCTL_MemMapPtr pctl_ptr_arr[] = {
      PCTLA_BASE_PTR,
      PCTLB_BASE_PTR,
      PCTLC_BASE_PTR,
      PCTLD_BASE_PTR,
      PCTLE_BASE_PTR,
      PCTLF_BASE_PTR
};

static uint8_t volatile * const mxc_ptr_arr[] = {
      &MXC_PTAPF1,
      &MXC_PTBPF1,
      &MXC_PTCPF1,
      &MXC_PTDPF1,
      &MXC_PTEPF1,
      &MXC_PTFPF1
};

static uint8_t pctl_ic_ptmod[] = {
      PORTA_GPIO_INTERRUPT_MODE,   /* defined in BSP (twrmcf51jf.h) */
      PORTB_GPIO_INTERRUPT_MODE,
      PORTC_GPIO_INTERRUPT_MODE,
      PORTD_GPIO_INTERRUPT_MODE,
      PORTE_GPIO_INTERRUPT_MODE,
      PORTF_GPIO_INTERRUPT_MODE
};

extern GPIO_DEV_DATA_PTR first_irq; /* first file having used IRQ */

static _mqx_int install_isr( MQX_FILE_PTR );
static uint32_t get_irq_mask( uint32_t n, uint32_t p);
void gpio_cpu_configure(GPIO_DEV_DATA_PTR);
_mqx_int gpio_cpu_init();
_mqx_int gpio_cpu_open(MQX_FILE_PTR, char *, char *);
_mqx_int gpio_cpu_ioctl(MQX_FILE_PTR, _mqx_uint, void *);
static void gpio_eport_irq (void *param);

/*FUNCTION*****************************************************************
 *
 * Function Name    : get_irq_mask
 * Returned Value   : IRQ MASK
 * Comments         :
 *    return the IRQ mask
 *
 *END*********************************************************************/

static uint32_t get_irq_mask
    (
        /* IN first bitmask of decode table */
        uint32_t n,
        /* IN second bitmask of decode table */
        uint32_t p
    )
{ /* Body */
#define GPIO_RISING_MASK   (0x01)
#define GPIO_FALLING_MASK  (0x02)

#define IRQ_MASK_RISING     0x01    /* Interrupt on rising edge */
#define IRQ_MASK_FALLING    0x02    /* Interrupt on falling edge */

   _mqx_uint index = (n ? GPIO_FALLING_MASK : 0) | (p ? GPIO_RISING_MASK : 0);

   const static uint8_t gpio_irq_mask_table[] = {
         0,                /* 00, no valid value */
         IRQ_MASK_RISING,  /* 01, rising edge irq */
         IRQ_MASK_FALLING, /* 10, falling edge irq */
         0,                /* 11, no valid value */
   };

   return (uint32_t) gpio_irq_mask_table[index];
} /* Endbody */

/*FUNCTION*****************************************************************
 *
 * Function Name    : gpio_cpu_configure
 * Returned Value   : IO_OK
 * Comments         :
 *    Configures pins to be GPIO based on pin map
 *
 *END*********************************************************************/

void gpio_cpu_configure
    (
        /* [IN] pointer to file data */
        GPIO_DEV_DATA_PTR   dev_data_ptr
    )
{ /* Body */
   _mqx_int  port_pointer,
   pin_idx;

   /* set GPIO output behavior */
   if (dev_data_ptr->type == DEV_OUTPUT) {
      PTA_DD |= dev_data_ptr->pin_map.reg.portt[GPIO_PORT_A];
      PTB_DD |= dev_data_ptr->pin_map.reg.portt[GPIO_PORT_B];
      PTC_DD |= dev_data_ptr->pin_map.reg.portt[GPIO_PORT_C];
      PTD_DD |= dev_data_ptr->pin_map.reg.portt[GPIO_PORT_D];
      PTE_DD |= dev_data_ptr->pin_map.reg.portt[GPIO_PORT_E];
      PTF_DD |= dev_data_ptr->pin_map.reg.portt[GPIO_PORT_F];
   }
   else { /* DEV_INPUT type is supposed */
      PTA_DD &= ~dev_data_ptr->pin_map.reg.portt[GPIO_PORT_A];
      PTB_DD &= ~dev_data_ptr->pin_map.reg.portt[GPIO_PORT_B];
      PTC_DD &= ~dev_data_ptr->pin_map.reg.portt[GPIO_PORT_C];
      PTD_DD &= ~dev_data_ptr->pin_map.reg.portt[GPIO_PORT_D];
      PTE_DD &= ~dev_data_ptr->pin_map.reg.portt[GPIO_PORT_E];
      PTF_DD &= ~dev_data_ptr->pin_map.reg.portt[GPIO_PORT_F];
   }

   /* set pin functionality to be GPIO for the whole file */
   for( port_pointer = 0; port_pointer < PORT_END; port_pointer++)
   {
      /* This variable is for quick access to determine if we should perform any change
       ** interrupt configuration for port.
       */
      uint8_t port_imask =
         dev_data_ptr->irqn_map.memory8[ port_pointer ] | \
         dev_data_ptr->irqp_map.memory8[ port_pointer ];

      /* Choose edge/level interrupt mode for whole port */
      if(pctl_ic_ptmod[port_pointer])
      {
         pctl_ptr_arr[port_pointer]->IC |= PCTL_IC_PTMOD_MASK;
      }
      else
      {
         pctl_ptr_arr[port_pointer]->IC &= ~PCTL_IC_PTMOD_MASK;
      }

      /* Enable pin interrupt request for whole port */
      pctl_ptr_arr[port_pointer]->IC |= PCTL_IC_PTIE_MASK;

      for( pin_idx = 0; pin_idx < PIN_IDX_NUM; pin_idx ++)
      {
         if( dev_data_ptr->pin_map.memory8[ port_pointer ] & ( 1 << pin_idx ) )
         {
            /* set to default state */
            if((pin_idx % 2) == 0)
            {
               *(mxc_ptr_arr[port_pointer]+PIN_IDX_NUM_HALF-1-pin_idx/2) &= MUX_EVEN;
               *(mxc_ptr_arr[port_pointer]+PIN_IDX_NUM_HALF-1-pin_idx/2) |= 0x01; /* GPIO */
            }
            else
            {
               *(mxc_ptr_arr[port_pointer]+PIN_IDX_NUM_HALF-pin_idx/2)   &= MUX_ODD;
               *(mxc_ptr_arr[port_pointer]+PIN_IDX_NUM_HALF-pin_idx/2)   |= 0x10; /* GPIO */
            }
            switch(get_irq_mask(
                  dev_data_ptr->irqn_map.memory8[ port_pointer ] & ( 1 << pin_idx),
                  dev_data_ptr->irqp_map.memory8[ port_pointer ] & ( 1 << pin_idx)))
            {
            case 0x01:  /* IRQ_MASK_RISING */
               pctl_ptr_arr[port_pointer]->IES |= (1 << pin_idx);     /* Rising edge interrupt */
               pctl_ptr_arr[port_pointer]->IF |= ( 1 << pin_idx );    /* Clear ISF flag */
               pctl_ptr_arr[port_pointer]->IPE |= (1 << pin_idx);     /* Enable pin interrupt */
               break;
            case 0x02:  /* IRQ_MASK_FALLING */
               pctl_ptr_arr[port_pointer]->IES &= ~(1 << pin_idx);    /* Falling edge interrupt */
               pctl_ptr_arr[port_pointer]->IF |= ( 1 << pin_idx );    /* Clear ISF flag */
               pctl_ptr_arr[port_pointer]->IPE |= (1 << pin_idx);     /* Enable pin interrupt */
               break;
            default:    /* Disable pin interrupt */
               pctl_ptr_arr[port_pointer]->IPE &= ~(1 << pin_idx);
               break;
            }
            /* enable internal pull-up resistor for input pin */
            if(dev_data_ptr->type == DEV_INPUT)
            {
               pctl_ptr_arr[port_pointer]->PUE |= (1 << pin_idx);   /* Enable pulling resistor */
               pctl_ptr_arr[port_pointer]->PUS |= (1 << pin_idx);   /* pullup resistor */
            }
         }
      }
   }
} /* Endbody */

/*FUNCTION*****************************************************************
 *
 * Function Name    : gpio_cpu_install
 * Returned Value   : IO_OK
 * Comments         :
 *    Performs basic initialization specific to board
 *
 *END*********************************************************************/

_mqx_int gpio_cpu_init()
{ /* Body */
   return IO_OK;
} /* Endbody */

/*FUNCTION*****************************************************************
 *
 * Function Name    : gpio_cpu_open
 * Returned Value   : IO_OK or error
 * Comments         :
 *    Implements initialization for DATA GPIO
 *
 *END*********************************************************************/

static _mqx_int install_isr
(
      MQX_FILE_PTR fd_ptr
)
{
   /* clear all flags */
   PCTLA_IF = 0xFF;
   PCTLB_IF = 0xFF;
   PCTLC_IF = 0xFF;
   PCTLD_IF = 0xFF;
   PCTLE_IF = 0xFF;
   PCTLF_IF = 0xFF;

   if (NULL == _int_install_isr(Vportab, gpio_eport_irq, NULL))
   {
      return IO_ERROR; /* could not install interrupt routine */
   }
   if (NULL == _int_install_isr(Vportdc, gpio_eport_irq, NULL))
   {
      return IO_ERROR;
   }
   if (NULL == _int_install_isr(Vportfe, gpio_eport_irq, NULL))
   {
      return IO_ERROR;
   }

   gpio_cpu_ioctl( fd_ptr, GPIO_IOCTL_ENABLE_IRQ, NULL);

   return IO_OK;
}

/*FUNCTION*****************************************************************
 *
 * Function Name    : gpio_cpu_open
 * Returned Value   : IO_OK or error
 * Comments         :
 *    Implements initialization for DATA GPIO
 *
 *END*********************************************************************/

_mqx_int gpio_cpu_open
(
      /* [IN] the file handle for the device */
      MQX_FILE_PTR fd_ptr,

      /* [IN] the file name */
      char   *file_name,

      /* [IN] pointer to parameters */
      char   *param_ptr
)
{ /* Body */
   GPIO_DEV_DATA_PTR dev_data_ptr = (GPIO_DEV_DATA_PTR) fd_ptr->DEV_DATA_PTR;

   /* if file_name is used, then the user wants to open peripheral */
   if ((file_name != NULL) && (*file_name != 0)) {
      if (!strncmp(file_name, "gpio:write", 11))  /* user wants write access to GPIO */
         dev_data_ptr->type = DEV_OUTPUT;
      else if (!strncmp(file_name, "gpio:output", 12))  /* user wants write access to GPIO */
         dev_data_ptr->type = DEV_OUTPUT;
      else if (!strncmp(file_name, "gpio:read", 10)) /* user wants read access to GPIO */
         dev_data_ptr->type = DEV_INPUT;
      else if (!strncmp(file_name, "gpio:input", 11)) /* user wants read access to GPIO */
         dev_data_ptr->type = DEV_INPUT;
      else
         /* peripherals not used yet */
         return IO_ERROR;
   }
   else
      return IO_ERROR;

   /* install interrupt service routine */
   if( IO_OK != install_isr( fd_ptr ) )
   {
      return IO_ERROR;
   }

   /* set pins status before selecting GPIO function */
   if ((param_ptr != NULL) && (dev_data_ptr->type == DEV_OUTPUT)) {
      /* note that this is similar to GPIO_IOCTL_WRITE function, but no checking
       ** is performed (was done in io_gpio_open function) */
      GPIO_PIN_STRUCT       *pin_table;
      uint32_t        addr;
      uint8_t         pin;
      GPIO_PIN_MAP   temp_pin0_map = {0};
      GPIO_PIN_MAP   temp_pin1_map = {0};

      /* prepare pin map */
      for (pin_table = (GPIO_PIN_STRUCT *) param_ptr; *pin_table != GPIO_LIST_END; pin_table++) {
         /* prepare address of port */
         addr = GPIO_GET_PORT(*pin_table);
         /* prepare bit mask */
         pin = GPIO_GET_BIT_MASK(*pin_table);
         if (*pin_table & GPIO_PIN_STATUS)
            temp_pin1_map.memory8[addr] |= pin;
         else
            temp_pin0_map.memory8[addr] |= pin;
      }
      /* ok, now we can apply new map */
      /* note: applying the map after collecting pins is due to have pins applied in one instruction */
      PTA_D |= temp_pin1_map.memory8[0];
      PTA_D &= ~temp_pin0_map.memory8[0];
      PTB_D |= temp_pin1_map.memory8[1];
      PTB_D &= ~temp_pin0_map.memory8[1];
      PTC_D |= temp_pin1_map.memory8[2];
      PTC_D &= ~temp_pin0_map.memory8[2];
      PTD_D |= temp_pin1_map.memory8[3];
      PTD_D &= ~temp_pin0_map.memory8[3];
      PTE_D |= temp_pin1_map.memory8[4];
      PTE_D &= ~temp_pin0_map.memory8[4];
      PTF_D |= temp_pin1_map.memory8[5];
      PTF_D &= ~temp_pin0_map.memory8[5];
   }

   gpio_cpu_configure(dev_data_ptr);
   return IO_OK;
} /* Endbody */

/*FUNCTION*****************************************************************
 *
 * Function Name    : gpio_cpu_ioctl
 * Returned Value   : depends on IOCTL command
 * Comments         :
 *    Implements all ioctl for GPIO
 *
 *END*********************************************************************/

_mqx_int gpio_cpu_ioctl
(
      /* [IN] the file handle for the device */
      MQX_FILE_PTR fd_ptr,

      /* [IN] the ioctl command */
      _mqx_uint  cmd,

      /* [IN] the ioctl parameters */
      void      *param_ptr
)
{ /* Body */
   GPIO_DEV_DATA_PTR  dev_data_ptr = (GPIO_DEV_DATA_PTR) fd_ptr->DEV_DATA_PTR;

   switch (cmd) {
   case GPIO_IOCTL_ADD_PINS:
   {
      /* Add pins to this file. Params: table with pin addresses */
      GPIO_PIN_STRUCT       *pin_table;
      uint32_t         addr;
      uint8_t          pin;

      /* check pin_table if they are not in global pin_map */
      _int_disable();
      for (pin_table = (GPIO_PIN_STRUCT *) param_ptr; *pin_table != GPIO_LIST_END; pin_table++)
      {
         /* check pin validity bit */
         if (*pin_table & GPIO_PIN_VALID)
         {
            /* prepare address of port */
            addr = GPIO_GET_PORT(*pin_table);
            /* prepare bit mask */
            pin = GPIO_GET_BIT_MASK( *pin_table );
            /* pin address out of map scope? */
            if (addr < sizeof(GPIO_PIN_MAP))
            {
               /* pin address already used? */
               if (! (gpio_global_pin_map.memory8[addr] & pin))
               {
                  /* manage next pin */
                  continue;
               }
            }
         }
         /* some problem occured */
         _int_enable();
         return IO_ERROR;
      }
      /* check successful, now we have to copy these bits to local pin map */
      for (pin_table = (GPIO_PIN_STRUCT *) param_ptr; *pin_table != GPIO_LIST_END; pin_table++)
      {
         /* prepare address of port */
         addr = GPIO_GET_PORT(*pin_table);
         /* prepare bit mask */
         pin = GPIO_GET_BIT_MASK( *pin_table );
         /* mark pin as used by this file */
         dev_data_ptr->pin_map.memory8[addr] |= pin;
         /* mark pin as used globally */
         gpio_global_pin_map.memory8[addr] |= pin;
      }
      gpio_cpu_configure(dev_data_ptr);
      _int_enable();
   }
   break;

   case GPIO_IOCTL_WRITE_LOG1:
   {
      /* If this file is configured as output, sets every pin to 1. Params:
            list of pin addresses to be used or NULL for every pin in the file */
      if (dev_data_ptr->type != DEV_OUTPUT)
         return IO_ERROR;
      if (param_ptr == NULL)
      {
         /* apply command to whole file */
         _int_disable(); /* _lwsem_wait(&gpio_sem) replacement */
         PTA_D |= dev_data_ptr->pin_map.memory8[0];
         PTB_D |= dev_data_ptr->pin_map.memory8[1];
         PTC_D |= dev_data_ptr->pin_map.memory8[2];
         PTD_D |= dev_data_ptr->pin_map.memory8[3];
         PTE_D |= dev_data_ptr->pin_map.memory8[4];
         PTF_D |= dev_data_ptr->pin_map.memory8[5];
         _int_enable();
         break;
      }
      else
      {
         GPIO_PIN_STRUCT        *pin_table;
         uint32_t                 addr;
         uint8_t                  pin;
         GPIO_PIN_MAP            temp_pin_map = {0};

         _int_disable();
         /* check if there is not pin in the list which this file does not contain */
         for (pin_table = (GPIO_PIN_STRUCT *) param_ptr; *pin_table != GPIO_LIST_END; pin_table++)
         {
            /* check pin validity bit */
            if (*pin_table & GPIO_PIN_VALID)
            {
               /* prepare address of port */
               addr = GPIO_GET_PORT(*pin_table);
               /* prepare bit mask */
               pin = GPIO_GET_BIT_MASK( *pin_table );
               /* pin address out of map scope? */
               if (addr < sizeof(GPIO_PIN_MAP))
               {
                  /* pin address already used? */
                  if (dev_data_ptr->pin_map.memory8[addr] & pin)
                  {
                     temp_pin_map.memory8[addr] |= pin;
                     continue; /* manage next pin */
                  }
               }
            }
            /* some problem occured */
            _int_enable();
            return IO_ERROR;
         }
         /* ok, now we can apply new map */
         /* note: applying the map after collecting pins is due to have pins applied in one instruction */
         PTA_D |= temp_pin_map.memory8[0];
         PTB_D |= temp_pin_map.memory8[1];
         PTC_D |= temp_pin_map.memory8[2];
         PTD_D |= temp_pin_map.memory8[3];
         PTE_D |= temp_pin_map.memory8[4];
         PTF_D |= temp_pin_map.memory8[5];
         _int_enable();
      }
   }
   break;

   case GPIO_IOCTL_WRITE_LOG0:
   {
      /* If this file is configured as output, clears every pin to 0. Params:
            list of pin addresses to be used or NULL for every pin in the file */
      if (dev_data_ptr->type != DEV_OUTPUT)
         return IO_ERROR;
      /* apply command to whole file */
      if (param_ptr == NULL)
      {
         _int_disable(); /* _lwsem_wait(&gpio_sem) replacement */
         PTA_D &= ~dev_data_ptr->pin_map.memory8[0];
         PTB_D &= ~dev_data_ptr->pin_map.memory8[1];
         PTC_D &= ~dev_data_ptr->pin_map.memory8[2];
         PTD_D &= ~dev_data_ptr->pin_map.memory8[3];
         PTE_D &= ~dev_data_ptr->pin_map.memory8[4];
         PTF_D &= ~dev_data_ptr->pin_map.memory8[5];
         _int_enable();
         break;
      }
      else {
         GPIO_PIN_STRUCT        *pin_table;
         uint32_t                 addr;
         uint8_t                  pin;
         GPIO_PIN_MAP            temp_pin_map = {0};

         _int_disable();
         /* check if there is not pin in the list which this file does not contain */
         for (pin_table = (GPIO_PIN_STRUCT *) param_ptr; *pin_table != GPIO_LIST_END; pin_table++)
         {
            /* check pin validity bit */
            if (*pin_table & GPIO_PIN_VALID)
            {
               /* prepare address of port */
               addr = GPIO_GET_PORT(*pin_table);
               /* prepare bit mask */
               pin = GPIO_GET_BIT_MASK( *pin_table );
               /* pin address out of map scope? */
               if (addr < sizeof(GPIO_PIN_MAP))
               {
                  /* pin address already used? */
                  if (dev_data_ptr->pin_map.memory8[addr] & pin) {
                     temp_pin_map.memory8[addr] |= pin;
                     continue; /* manage next pin */
                  }
               }
            }
            /* some problem occured */
            _int_enable();
            return IO_ERROR;
         }
         /* ok, now we can apply new map */
         /* note: applying the map after collecting pins is due to have pins applied in one instruction */
         PTA_D &= ~temp_pin_map.memory8[0];
         PTB_D &= ~temp_pin_map.memory8[1];
         PTC_D &= ~temp_pin_map.memory8[2];
         PTD_D &= ~temp_pin_map.memory8[3];
         PTE_D &= ~temp_pin_map.memory8[4];
         PTF_D &= ~temp_pin_map.memory8[5];
         _int_enable();
      }
   }
   break;

   case GPIO_IOCTL_WRITE:
   {
      /* If this file is configured as output, sets every pin to the state
       ** defined in GPIO_PIN_STATUS. Params:
       ** list of pin addresses to be used or NULL for every pin in the file */
      if (dev_data_ptr->type != DEV_OUTPUT)
         return IO_ERROR;

      if (param_ptr == NULL) /* no param specified , but must be */
         return IO_ERROR;
      else {
         GPIO_PIN_STRUCT        *pin_table;
         uint32_t                 addr;
         uint8_t                  pin;
         GPIO_PIN_MAP            temp_pin0_map = {0};
         GPIO_PIN_MAP            temp_pin1_map = {0};

         _int_disable();
         /* check if there is not pin in the list which this file does not contain */
         for (pin_table = (GPIO_PIN_STRUCT *) param_ptr; *pin_table != GPIO_LIST_END; pin_table++)
         {
            /* check pin validity bit */
            if (*pin_table & GPIO_PIN_VALID)
            {
               /* prepare address of port */
               addr = GPIO_GET_PORT(*pin_table);
               /* prepare bit mask */
               pin = GPIO_GET_BIT_MASK( *pin_table );
               /* pin address out of map scope? */
               if (addr < sizeof(GPIO_PIN_MAP))
               {
                  /* pin address already used? */
                  if (dev_data_ptr->pin_map.memory8[addr] & pin)
                  {
                     if (*pin_table & GPIO_PIN_STATUS)
                        temp_pin1_map.memory8[addr] |= pin;
                     else
                        temp_pin0_map.memory8[addr] |= pin;
                     continue; /* manage next pin */
                  }
               }
            }
            /* some problem occured */
            _int_enable();
            return IO_ERROR;
         }
         /* ok, now we can apply new map */
         /* note: applying the map after collecting pins is due to have pins applied in one instruction */
         PTA_D |= temp_pin1_map.memory8[0];
         PTB_D |= temp_pin1_map.memory8[1];
         PTC_D |= temp_pin1_map.memory8[2];
         PTD_D |= temp_pin1_map.memory8[3];
         PTE_D |= temp_pin1_map.memory8[4];
         PTF_D |= temp_pin1_map.memory8[5];
         PTA_D &= ~temp_pin0_map.memory8[0];
         PTB_D &= ~temp_pin0_map.memory8[1];
         PTC_D &= ~temp_pin0_map.memory8[2];
         PTD_D &= ~temp_pin0_map.memory8[3];
         PTE_D &= ~temp_pin0_map.memory8[4];
         PTF_D &= ~temp_pin0_map.memory8[5];
         _int_enable();
      }
   }
   break;

   case GPIO_IOCTL_READ:
   {
      /* Read status of pins, if configured as input. Params: pointer to
       ** predefined table where data will be written */
      GPIO_PIN_STRUCT        *pin_table;
      uint32_t                 addr;
      volatile uint8_t       *rgpio_pdir_ptr;
      uint8_t                  pin;

      volatile uint8_t   *rgpio_ptr_arr[] = {
            &PTA_PV,
            &PTB_PV,
            &PTC_PV,
            &PTD_PV,
            &PTE_PV,
            &PTF_PV
      };

      if (dev_data_ptr->type != DEV_INPUT)
         return IO_ERROR;
      if (param_ptr == NULL) /* no list defined */
      return IO_ERROR;

      _int_disable();
      /* check if there is not pin in the list which this file does not contain */
      for (pin_table = (GPIO_PIN_STRUCT *) param_ptr; *pin_table != GPIO_LIST_END; pin_table++)
      {
         /* check pin validity bit */
         if (*pin_table & GPIO_PIN_VALID)
         {
            /* prepare address of port */
            addr = GPIO_GET_PORT(*pin_table);
            rgpio_pdir_ptr = rgpio_ptr_arr[ addr ];
            /* prepare bit mask */
            pin = GPIO_GET_BIT_MASK( *pin_table );
            /* pin address out of map scope? */
            if (addr < sizeof(GPIO_PIN_MAP))
            {
               /* pin address already used? */
               if (dev_data_ptr->pin_map.memory8[addr] & pin)
               {
                  /* GPIO port */
                  /* check pin status */
                  if ( *rgpio_pdir_ptr & pin)
                  {
                     /* set pin status to 1 in the list */
                     *pin_table |= GPIO_PIN_STATUS;
                  }
                  else
                  {
                     /* clear pin status to 0 in the list */
                     *pin_table &= ~GPIO_PIN_STATUS;
                  }
                  continue; /* manage next pin */
               }
            }
         }
         /* some problem occured */
         _int_enable();
         return IO_ERROR;
      }
      _int_enable();
   }
   break;

   case GPIO_IOCTL_ENABLE_IRQ: {

      uint32_t port_pointer, pin_idx;

      /* set "interrupt enabled" for the whole file */
      for( port_pointer = 0; port_pointer < PORT_END; port_pointer++)
      {
         /* This variable is for quick access to determine if we should perform any change
          ** to IRQC register field.
          ** We can set IRQC for every pin of port also, but it will be time consuming
          */
         uint8_t port_imask =
            dev_data_ptr->irqn_map.memory8[ port_pointer ] | \
            dev_data_ptr->irqp_map.memory8[ port_pointer ];

         /* Choose edge/level interrupt mode for whole port */
         if(pctl_ic_ptmod[port_pointer])
         {
            pctl_ptr_arr[port_pointer]->IC |= PCTL_IC_PTMOD_MASK;
         }
         else
         {
            pctl_ptr_arr[port_pointer]->IC &= ~PCTL_IC_PTMOD_MASK;
         }

         /* Enable pin interrupt request for whole port */
         pctl_ptr_arr[port_pointer]->IC |= PCTL_IC_PTIE_MASK;

         for( pin_idx = 0; pin_idx < PIN_IDX_NUM; pin_idx ++)
         {
            if (port_imask & 0x01) {
               switch(get_irq_mask(
                     dev_data_ptr->irqn_map.memory8[ port_pointer ] & ( 1 << pin_idx),
                     dev_data_ptr->irqp_map.memory8[ port_pointer ] & ( 1 << pin_idx)))
               {
               case 0x01:  /* IRQ_MASK_RISING */
                  pctl_ptr_arr[port_pointer]->IES |= (1 << pin_idx);     /* Rising edge interrupt */
                  pctl_ptr_arr[port_pointer]->IPE |= (1 << pin_idx);     /* Enable pin interrupt */
                  break;
               case 0x02:  /* IRQ_MASK_FALLING */
                  pctl_ptr_arr[port_pointer]->IES &= ~(1 << pin_idx);    /* Falling edge interrupt */
                  pctl_ptr_arr[port_pointer]->IPE |= (1 << pin_idx);     /* Enable pin interrupt */
                  break;
               default:    /* Disable pin interrupt */
                  pctl_ptr_arr[port_pointer]->IPE &= ~(1 << pin_idx);
                  break;
               }
            }
            port_imask >>= 1;
         }
      }
   }
   break;

   case GPIO_IOCTL_DISABLE_IRQ: {

      uint32_t port_pointer, pin_idx;

      /* set "interrupt disabled" for whole file */
      for( port_pointer = 0; port_pointer < PORT_END; port_pointer++)
      {
         /* This variable is for quick access to determine if we should perform any change
          ** to IRQC register field.
          ** We can set IRQC for every pin of port also, but it will be time consuming
          */
         uint8_t port_imask =
            dev_data_ptr->irqn_map.memory8[ port_pointer ] | \
            dev_data_ptr->irqp_map.memory8[ port_pointer ];

         for( pin_idx = 0; pin_idx < PIN_IDX_NUM; pin_idx ++)
         {
            if (port_imask & 0x01) {
               pctl_ptr_arr[port_pointer]->IPE &= ~(1 << pin_idx);  /* Disable pin interrupt */
            }
            port_imask >>= 1;
         }
      }
   }
   break;

   case GPIO_IOCTL_SET_IRQ_FUNCTION: {

      if (dev_data_ptr->type == DEV_OUTPUT)
         return IO_ERROR; /* cannot set IRQ for output devices */

      dev_data_ptr->irq_func = (IRQ_FUNC)param_ptr;
      _int_disable();

      if (param_ptr == NULL) {  /* remove function from list */
         if (first_irq != NULL) {
            GPIO_DEV_DATA_PTR gptr = first_irq;
            while (gptr->NEXT != NULL) {
               if (gptr == dev_data_ptr)
                  break;
               if (gptr->NEXT == dev_data_ptr)
                  break;
               gptr = gptr->NEXT;
            }
            if (gptr == dev_data_ptr) /* found as first in the list */
            first_irq = first_irq->NEXT;
            else if (gptr->NEXT == dev_data_ptr) /* found in the middle or at the end of list */
               gptr->NEXT = gptr->NEXT->NEXT;
         }
      }
      else { /* add new function to the list */
         if (first_irq == NULL) {
            first_irq = dev_data_ptr;
         }
         else {
            GPIO_DEV_DATA_PTR gptr = first_irq;
            while (gptr->NEXT != NULL) {
               if (gptr == dev_data_ptr)
                  break;
               gptr = gptr->NEXT;
            }
            if (gptr != dev_data_ptr)
               gptr->NEXT = dev_data_ptr;
         }
      }
      _int_enable();

   }
   break;

   default:
      return IO_ERROR_INVALID_IOCTL_CMD;
   }

   return IO_OK;
} /* Endbody */

/*FUNCTION****************************************************************
 *
 * Function Name    : gpio_eport_irq
 * Returned Value   :
 * Comments         : GPIO IRQ handler
 *
 *   This function find through all gpio files interrupt flag, clear it
 *   and call installed irq function
 *
 *END*********************************************************************/

static void gpio_eport_irq
(
      void   *param
)
{ /* Body */

   GPIO_DEV_DATA_PTR   gptr = first_irq;
   bool             irq_flag = 0;
   _mqx_int            port_pointer,
   pin_idx;

   /* list through all files */
   while (gptr)
   {
      /* list through all ports */
      for( port_pointer = 0; port_pointer < PORT_END; port_pointer++)
      {
         /* list through all pins */
         for( pin_idx = 0; pin_idx < PIN_IDX_NUM; pin_idx ++)
         {
            /* is pin in file ? */
            if( gptr->pin_map.memory8[ port_pointer ] & ( 1 << pin_idx ) )
            {
               /* have pin set irq flag ? */
               if( pctl_ptr_arr[port_pointer]->IF & ( 1 << pin_idx ) )
               {
                  /* clear ISF flag */
                  pctl_ptr_arr[port_pointer]->IF |= ( 1 << pin_idx );
                  /* set irq call function flag */
                  irq_flag = 1;
               }
            }
         }
      }
      if ( irq_flag )
      {
         /* call irq function */
         gptr->irq_func(NULL);
         irq_flag = 0;
      }
      /* go to next file */
      gptr = gptr->NEXT;
   }
} /* Endbody */

/*FUNCTION****************************************************************
 *
 * Function Name    : _bsp_get_gpio_base_address
 * Returned Value   : pointer to base of GPIO registers
 * Comments         :
 *    This function returns base address of GPIO related register space.
 *
 *END*********************************************************************/
void *_bsp_get_gpio_base_address
(
      void
)
{
   return NULL;
}
#endif /*end if*/
/* EOF */
