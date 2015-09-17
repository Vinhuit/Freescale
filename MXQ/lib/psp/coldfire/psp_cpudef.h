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
*
*
*END************************************************************************/

#ifndef __psp_cpudef_h__
#define __psp_cpudef_h__

#include "mqx_cpudef.h"

/* CPU groups - for new processors add the name at the end of the list*/
#define PSP_CPU_GROUP_MCF51AC       (1)
#define PSP_CPU_GROUP_MCF51CN       (2)
#define PSP_CPU_GROUP_MCF51JM       (3)
#define PSP_CPU_GROUP_MCF51EM       (4)
#define PSP_CPU_GROUP_MCF5222       (5)
#define PSP_CPU_GROUP_MCF5223       (6)
#define PSP_CPU_GROUP_MCF5225       (7)
#define PSP_CPU_GROUP_MCF5227       (8)
#define PSP_CPU_GROUP_MCF5282       (9)
#define PSP_CPU_GROUP_MCF5445       (10)
#define PSP_CPU_GROUP_MCF5441       (11)
#define PSP_CPU_GROUP_MCF51MM       (12)
#define PSP_CPU_GROUP_MCF5301       (13)
#define PSP_CPU_GROUP_MCF51JE       (14)
#define PSP_CPU_GROUP_MCF532        (15)
#define PSP_CPU_GROUP_MCF520        (16)
#define PSP_CPU_GROUP_MCF51AG       (17)
#define PSP_CPU_GROUP_MCF51FD       (18)
#define PSP_CPU_GROUP_MCF51JF       (19)
#define PSP_CPU_GROUP_MCF51QM       (20)

/* CPU names (ARCHITECTURE | FAMILY | DERIVATIVE ) */
#define PSP_CPU_MCF51AC128          (PSP_CPU_NUM(PSP_CPU_ARCH_COLDFIRE, PSP_CPU_GROUP_MCF51AC, 1))
#define PSP_CPU_MCF51AC256          (PSP_CPU_NUM(PSP_CPU_ARCH_COLDFIRE, PSP_CPU_GROUP_MCF51AC, 2))
#define PSP_CPU_MCF51CN128          (PSP_CPU_NUM(PSP_CPU_ARCH_COLDFIRE, PSP_CPU_GROUP_MCF51CN, 1))
#define PSP_CPU_MCF51JM128          (PSP_CPU_NUM(PSP_CPU_ARCH_COLDFIRE, PSP_CPU_GROUP_MCF51JM, 1))
#define PSP_CPU_MCF51EM128          (PSP_CPU_NUM(PSP_CPU_ARCH_COLDFIRE, PSP_CPU_GROUP_MCF51EM, 1))
#define PSP_CPU_MCF51EM256          (PSP_CPU_NUM(PSP_CPU_ARCH_COLDFIRE, PSP_CPU_GROUP_MCF51EM, 2))
#define PSP_CPU_MCF51MM256          (PSP_CPU_NUM(PSP_CPU_ARCH_COLDFIRE, PSP_CPU_GROUP_MCF51MM, 1))
#define PSP_CPU_MCF51JE128          (PSP_CPU_NUM(PSP_CPU_ARCH_COLDFIRE, PSP_CPU_GROUP_MCF51JE, 1))
#define PSP_CPU_MCF51JE256          (PSP_CPU_NUM(PSP_CPU_ARCH_COLDFIRE, PSP_CPU_GROUP_MCF51JE, 2))
#define PSP_CPU_MCF51AG128          (PSP_CPU_NUM(PSP_CPU_ARCH_COLDFIRE, PSP_CPU_GROUP_MCF51AG, 1))
#define PSP_CPU_MCF51FD256          (PSP_CPU_NUM(PSP_CPU_ARCH_COLDFIRE, PSP_CPU_GROUP_MCF51FD, 2))
#define PSP_CPU_MCF51JF128          (PSP_CPU_NUM(PSP_CPU_ARCH_COLDFIRE, PSP_CPU_GROUP_MCF51JF, 1))
#define PSP_CPU_MCF51QM128          (PSP_CPU_NUM(PSP_CPU_ARCH_COLDFIRE, PSP_CPU_GROUP_MCF51QM, 1))

#define PSP_CPU_MCF52221            (PSP_CPU_NUM(PSP_CPU_ARCH_COLDFIRE, PSP_CPU_GROUP_MCF5222, 1))
#define PSP_CPU_MCF52223            (PSP_CPU_NUM(PSP_CPU_ARCH_COLDFIRE, PSP_CPU_GROUP_MCF5222, 3))
#define PSP_CPU_MCF52233            (PSP_CPU_NUM(PSP_CPU_ARCH_COLDFIRE, PSP_CPU_GROUP_MCF5223, 3))
#define PSP_CPU_MCF52235            (PSP_CPU_NUM(PSP_CPU_ARCH_COLDFIRE, PSP_CPU_GROUP_MCF5223, 5))
#define PSP_CPU_MCF52259            (PSP_CPU_NUM(PSP_CPU_ARCH_COLDFIRE, PSP_CPU_GROUP_MCF5225, 9))
#define PSP_CPU_MCF52277            (PSP_CPU_NUM(PSP_CPU_ARCH_COLDFIRE, PSP_CPU_GROUP_MCF5227, 7))
#define PSP_CPU_MCF5282             (PSP_CPU_NUM(PSP_CPU_ARCH_COLDFIRE, PSP_CPU_GROUP_MCF5282, 0))
#define PSP_CPU_MCF5208             (PSP_CPU_NUM(PSP_CPU_ARCH_COLDFIRE, PSP_CPU_GROUP_MCF520,  8))

#define PSP_CPU_MCF53015            (PSP_CPU_NUM(PSP_CPU_ARCH_COLDFIRE, PSP_CPU_GROUP_MCF5301, 5))
#define PSP_CPU_MCF53281            (PSP_CPU_NUM(PSP_CPU_ARCH_COLDFIRE, PSP_CPU_GROUP_MCF532,  0))
#define PSP_CPU_MCF5329             (PSP_CPU_NUM(PSP_CPU_ARCH_COLDFIRE, PSP_CPU_GROUP_MCF532,  9))


#define PSP_CPU_MCF54455            (PSP_CPU_NUM(PSP_CPU_ARCH_COLDFIRE, PSP_CPU_GROUP_MCF5445, 5))
#define PSP_CPU_MCF54418            (PSP_CPU_NUM(PSP_CPU_ARCH_COLDFIRE, PSP_CPU_GROUP_MCF5441, 8))


/* Coldfire V1 macros */    
#define PSP_MQX_CPU_IS_MCF51AC      ((PSP_GET_CPU_GROUP(MQX_CPU) == PSP_CPU_GROUP_MCF51AC))
#define PSP_MQX_CPU_IS_MCF51CN      ((PSP_GET_CPU_GROUP(MQX_CPU) == PSP_CPU_GROUP_MCF51CN))
#define PSP_MQX_CPU_IS_MCF51EM      ((PSP_GET_CPU_GROUP(MQX_CPU) == PSP_CPU_GROUP_MCF51EM))
#define PSP_MQX_CPU_IS_MCF51JM      ((PSP_GET_CPU_GROUP(MQX_CPU) == PSP_CPU_GROUP_MCF51JM))
#define PSP_MQX_CPU_IS_MCF51MM      ((PSP_GET_CPU_GROUP(MQX_CPU) == PSP_CPU_GROUP_MCF51MM))
#define PSP_MQX_CPU_IS_MCF51JE      ((PSP_GET_CPU_GROUP(MQX_CPU) == PSP_CPU_GROUP_MCF51JE))
#define PSP_MQX_CPU_IS_MCF51AG      ((PSP_GET_CPU_GROUP(MQX_CPU) == PSP_CPU_GROUP_MCF51AG))
#define PSP_MQX_CPU_IS_MCF51FD      ((PSP_GET_CPU_GROUP(MQX_CPU) == PSP_CPU_GROUP_MCF51FD))
#define PSP_MQX_CPU_IS_MCF51JF      ((PSP_GET_CPU_GROUP(MQX_CPU) == PSP_CPU_GROUP_MCF51JF))
#define PSP_MQX_CPU_IS_MCF51QM      ((PSP_GET_CPU_GROUP(MQX_CPU) == PSP_CPU_GROUP_MCF51QM))

#define PSP_MQX_CPU_IS_MCF51        ( PSP_MQX_CPU_IS_MCF51AC || PSP_MQX_CPU_IS_MCF51CN || \
                                      PSP_MQX_CPU_IS_MCF51EM || PSP_MQX_CPU_IS_MCF51MM || \
                                      PSP_MQX_CPU_IS_MCF51JM || PSP_MQX_CPU_IS_MCF51JE || \
                                      PSP_MQX_CPU_IS_MCF51AG || PSP_MQX_CPU_IS_MCF51FD || \
                                      PSP_MQX_CPU_IS_MCF51JF || PSP_MQX_CPU_IS_MCF51QM )

/* Coldfire V2 macros */
#define PSP_MQX_CPU_IS_MCF5222X     ((PSP_GET_CPU_GROUP(MQX_CPU) == PSP_CPU_GROUP_MCF5222))
#define PSP_MQX_CPU_IS_MCF5223X     ((PSP_GET_CPU_GROUP(MQX_CPU) == PSP_CPU_GROUP_MCF5223))
#define PSP_MQX_CPU_IS_MCF5225X     ((PSP_GET_CPU_GROUP(MQX_CPU) == PSP_CPU_GROUP_MCF5225))
#define PSP_MQX_CPU_IS_MCF5227X     ((PSP_GET_CPU_GROUP(MQX_CPU) == PSP_CPU_GROUP_MCF5227))
#define PSP_MQX_CPU_IS_MCF520X      ((PSP_GET_CPU_GROUP(MQX_CPU) == PSP_CPU_GROUP_MCF520))

#define PSP_MQX_CPU_IS_MCF52        ( PSP_MQX_CPU_IS_MCF5222X || \
                                      PSP_MQX_CPU_IS_MCF5223X || \
                                      PSP_MQX_CPU_IS_MCF5225X || \
                                      PSP_MQX_CPU_IS_MCF5227X || \
                                      PSP_MQX_CPU_IS_MCF520X )

/* Coldfire V3 macros */                                      
#define PSP_MQX_CPU_IS_MCF5301X     ((PSP_GET_CPU_GROUP(MQX_CPU) == PSP_CPU_GROUP_MCF5301))
#define PSP_MQX_CPU_IS_MCF532X      ((PSP_GET_CPU_GROUP(MQX_CPU) == PSP_CPU_GROUP_MCF532))

#define PSP_MQX_CPU_IS_MCF53        ( PSP_MQX_CPU_IS_MCF5301X  || \
                                      PSP_MQX_CPU_IS_MCF532X )
        
/* Coldfire V4 macros */                                      
#define PSP_MQX_CPU_IS_MCF5445X     ((PSP_GET_CPU_GROUP(MQX_CPU) == PSP_CPU_GROUP_MCF5445))
#define PSP_MQX_CPU_IS_MCF5441X     ((PSP_GET_CPU_GROUP(MQX_CPU) == PSP_CPU_GROUP_MCF5441))


#define PSP_MQX_CPU_IS_MCF54        ( PSP_MQX_CPU_IS_MCF5445X || \
                                      PSP_MQX_CPU_IS_MCF5441X)

#endif
