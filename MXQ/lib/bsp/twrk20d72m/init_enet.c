/*HEADER**********************************************************************
*
* Copyright 2014 Freescale Semiconductor, Inc.
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
*   This file contains the function that reads the timer and returns
*   the number of nanoseconds elapsed since the last interrupt.
*
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"
#include "bsp_prv.h"
#include "enet.h"
#include "enetprv.h"

#if BSP_ENET_WIFI_ENABLED
#include "includes.h"
#include "rs21_mqx.h"
#include "phy_rsi.h"
#include <gs_prv.h>
#include "phy_gs.h"
#include <atheros_wifi.h>
#include <atheros_phy.h>

#if (BSPCFG_ENABLE_GAINSPAN_WIFI && (BSPCFG_ENABLE_ATHEROS_WIFI || BSPCFG_ENABLE_REDPINE_WIFI)) || (BSPCFG_ENABLE_ATHEROS_WIFI && BSPCFG_ENABLE_REDPINE_WIFI)
    #error Only one wifi vendor could be enabled at a time
#endif

#if BSPCFG_ENABLE_GAINSPAN_WIFI
GS_PARAM_WIFI_STRUCT enet_wifi_param = {
    BSP_GS_SPI_DEVICE,
    BSP_GS_WAKEUP_GPIO_PIN,
    BSP_GS_WAKEUP_MUX_IRQ,
    BSP_GS_WAKEUP_INT_LEVEL,
    BSP_GS_WAKEUP_INT_SUBLEVEL
};
#elif BSPCFG_ENABLE_ATHEROS_WIFI
ATHEROS_PARAM_WIFI_STRUCT enet_wifi_param = {
    BSP_ATHEROS_WIFI_SPI_DEVICE,
    BSP_ATHEROS_WIFI_GPIO_INT_PIN,
    #if WLAN_CONFIG_ENABLE_CHIP_PWD_GPIO
    BSP_ATHEROS_WIFI_GPIO_PWD_PIN
    #endif
};
#elif BSPCFG_ENABLE_REDPINE_WIFI
const RSI_PARAM_STRUCT enet_wifi_param = {
    BSP_RSI_SPI_DEVICE,
    BSP_RSI_WAKEUP_GPIO_PIN,
    BSP_RSI_WAKEUP_MUX_IRQ ,
    BSP_RSI_WAKEUP_INT_LEVEL,
    BSP_RSI_WAKEUP_INT_SUBLEVEL,
    BSP_RSI_POWER_GPIO_PIN,
    BSP_RSI_POWER_MUX_GPIO,
    BSP_RSI_RESET_GPIO_PIN,
    BSP_RSI_RESET_MUX_GPIO
};
#endif /* BSPCFG_ENABLE_GAINSPAN_WIFI */

const ENET_IF_STRUCT ENET_WIFI = {
#if BSPCFG_ENABLE_GAINSPAN_WIFI
    &GAINSPAN_WIFI_IF,
    &GAINSPAN_PHY_IF,
#elif BSPCFG_ENABLE_ATHEROS_WIFI
    &ATHEROS_WIFI_IF,
    &ATHEROS_PHY_IF,
#elif BSPCFG_ENABLE_REDPINE_WIFI
    &REDPINE_WIFI_IF,
    &REDPINE_PHY_IF,
#endif
    0,
    0,
    0,
    0
};

const ENET_PARAM_STRUCT ENET_default_params[BSP_ENET_DEVICE_COUNT] = {
    {
        &ENET_WIFI,              /* Default WiFi Device parameter */
        Auto_Negotiate,
        0,
        0,    /* NOT USED */
        0,    /* NOT USED */
        0,    /* NOT USED */
        0,    /* NOT USED */
        0,    /* NOT USED */
        0,    /* NOT USED */
        BSP_ENET_WIFI_RX_PCB,         /* rx PCBs */
        0,
        0,
        (void *)&enet_wifi_param
    },
};
#endif /* BSP_ENET_WIFI_ENABLED */

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_mac_address
* Returned Value   : uint32_t
* Comments         :
*    This function returns the mac address associated with the specified device
*
* If the MAC address is stored in NV-storage, this fuction should read the
* MAC address from NV-storage and set it.
*
* If the MAC address is generated from a device serial number and an OUI, the
* serial number should be passed in, and the MAC address should be constructed
*
*END*----------------------------------------------------------------------*/
bool _bsp_get_mac_address
   (
      uint32_t        device,
      uint32_t        value,
      _enet_address  address
   )
{ /* Body */
   if (device >= BSP_ENET_DEVICE_COUNT)
      return FALSE;

   return TRUE;

} /* Endbody */
/* EOF */
