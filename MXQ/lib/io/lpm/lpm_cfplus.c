/*HEADER**********************************************************************
*
* Copyright 2011 Freescale Semiconductor, Inc.
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
*   This file contains Low Power Manager functionality specific to CF+.
*
*
*END************************************************************************/


#include "mqx.h"
#include "bsp.h"


#if MQX_ENABLE_LOW_POWER


#ifndef PE_LDD_VERSION


static const LPM_CPU_POWER_MODE LPM_CPU_POWER_MODES_CFPLUS[LPM_CPU_POWER_MODES] =
{
    // RUN
    {
        SMC_PMCTRL_LPWUI_MASK,                      // Mode PMCTRL register == voltage regulator ON after wakeup
        0,                                          // Mode flags == clear settings
    },
    // WAIT
    {
        SMC_PMCTRL_LPWUI_MASK,                      // Mode PMCTRL register == voltage regulator ON after wakeup
        LPM_CPU_POWER_MODE_FLAG_WAITE,              // Mode flags == execute WFI
    },
    // STOP
    {
        SMC_PMCTRL_LPWUI_MASK,                       // Mode PMCTRL register == voltage regulator ON after wakeup
        LPM_CPU_POWER_MODE_FLAG_STOPE,              // Mode flags == deepsleep, execute WFI
    },
    // VLPR
    {
        SMC_PMCTRL_RUNM(2),                         // Mode PMCTRL register == VLPR
        0,                                          // Mode flags == clear settings
    },
    // VLPW
    {
        SMC_PMCTRL_RUNM(2),                         // Mode PMCTRL register == VLPW
        LPM_CPU_POWER_MODE_FLAG_WAITE,              // Mode flags == execute WFI
    },
    // VLPS
    {
        SMC_PMCTRL_STOPM(2),                         // Mode PMCTRL register == VLPS
        LPM_CPU_POWER_MODE_FLAG_STOPE,              // Mode flags == deepsleep, execute WFI
    },
    // LLS
    {
        SMC_PMCTRL_LPWUI_MASK | SMC_PMCTRL_STOPM(4), // Mode PMCTRL register == voltage regulator ON after wakeup, LLS
        LPM_CPU_POWER_MODE_FLAG_STOPE,              // Mode flags == deepsleep, execute WFI
    }
};


#else


static const LDD_TDriverOperationMode LPM_PE_OPERATION_MODE_MAP[LPM_OPERATION_MODES] =
{
    DOM_RUN,
    DOM_WAIT,
    DOM_SLEEP,
    DOM_STOP
};


#endif


/*FUNCTION*------------------------------------------------------------------
*
* Function Name    : _lpm_set_cpu_operation_mode
* Returned Value   : MQX error code
* Comments         :
*    This function changes operation mode of the CPU core.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _lpm_set_cpu_operation_mode
    (
        /* [IN] Specification of CPU core low power operation modes available */
        const LPM_CPU_OPERATION_MODE  *operation_modes,

        /* [IN] Low power operation mode identifier */
        LPM_OPERATION_MODE                 target_mode
    )
{

#ifndef PE_LDD_VERSION

    const LPM_CPU_POWER_MODE              *mode_ptr;
    _mqx_uint                              scr, flags, mcg, index;

    /* Check parameters */
    if ((NULL == operation_modes) || (LPM_OPERATION_MODES <= (_mqx_uint)target_mode))
    {
        return MQX_INVALID_PARAMETER;
    }
    index = operation_modes[target_mode].MODE_INDEX;

    if (LPM_CPU_POWER_MODES <= index)
    {
        return MQX_INVALID_CONFIGURATION;
    }
    mode_ptr = &(LPM_CPU_POWER_MODES_CFPLUS[index]);
    flags = mode_ptr->FLAGS;

    /* Go through Run */
    SIM_SOPT4 |= SIM_SOPT4_WAITE_MASK;
    SMC_PMCTRL = LPM_CPU_POWER_MODES_CFPLUS[LPM_CPU_POWER_MODE_RUN].PMCTRL;
    while (0 == (PMC_REGSC & PMC_REGSC_REGONS_MASK))
        { };
    while (SMC_PMSTAT_PMSTAT(1) != (SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK))
        { };

    /* Go to VLPW through VLPR */
    if (LPM_CPU_POWER_MODE_VLPW == index)
    {
        SMC_PMCTRL = LPM_CPU_POWER_MODES_CFPLUS[LPM_CPU_POWER_MODE_VLPR].PMCTRL;
        while (SMC_PMSTAT_PMSTAT(4) != (SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK))
            { };
    }

    /* Setup wakeup unit for LLS mode */
    if (LPM_CPU_POWER_MODE_LLS == index)
    {
        LLWU_PE1 = operation_modes[target_mode].PE1;
        LLWU_PE2 = operation_modes[target_mode].PE2;
        LLWU_PE3 = operation_modes[target_mode].PE3;
        LLWU_PE4 = operation_modes[target_mode].PE4;
        LLWU_ME = operation_modes[target_mode].ME;
        LLWU_FILT1 = operation_modes[target_mode].FILT1;
        LLWU_FILT2 = operation_modes[target_mode].FILT2;
        LLWU_F1 = 0xFF;
        LLWU_F2 = 0xFF;
        LLWU_F3 = 0xFF;
    }

    /* Keep status of MCG before mode change */
    mcg = MCG_S & MCG_S_CLKST_MASK;

    /* Operation mode setup */
    SMC_PMCTRL = mode_ptr->PMCTRL;

    /* Wait for proper setup of VLPR */
    if (LPM_CPU_POWER_MODE_VLPR == index)
    {
        while (SMC_PMSTAT_PMSTAT(4) != (SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK))
            { };
    }

    /* Go to sleep if required */
    if (flags & (LPM_CPU_POWER_MODE_FLAG_WAITE | LPM_CPU_POWER_MODE_FLAG_STOPE))
    {
        if (flags & LPM_CPU_POWER_MODE_FLAG_STOPE)
        {
            SIM_SOPT4 &= (~ SIM_SOPT4_WAITE_MASK);
        }

        _ASM_SLEEP();
    }

    /* After stop modes, reconfigure MCG if needed */
    if ( (LPM_CPU_POWER_MODE_STOP == index)
      || (LPM_CPU_POWER_MODE_VLPS == index)
      || (LPM_CPU_POWER_MODE_LLS == index) )
    {
        if ((MCG_S_CLKST(3) == mcg) && (MCG_S_CLKST(2) == (MCG_S & MCG_S_CLKST_MASK)))
        {
            MCG_C1 &= (~ (MCG_C1_CLKS_MASK | MCG_C1_IREFS_MASK));
            while (0 == (MCG_S & MCG_S_LOCK_MASK))
                { };
        }
    }

    return MQX_OK;

#else

#ifdef Cpu_SetOperationMode_METHOD_ENABLED
    if (LPM_OPERATION_MODES <= (_mqx_uint)target_mode)
    {
        return MQX_INVALID_PARAMETER;
    }
    return Cpu_SetOperationMode (LPM_PE_OPERATION_MODE_MAP[target_mode], NULL, NULL);
#else
    #error Undefined method Cpu_SetOperationMode() in PE CPU component!
#endif

#endif

}

/*FUNCTION*------------------------------------------------------------------
*
* Function Name    : _lpm_wakeup_core
* Returned Value   : None
* Comments         :
*    This function does nothing on Coldfire+ platform.
*
*END*----------------------------------------------------------------------*/

void _lpm_wakeup_core
    (
        void
    )
{
}


/*FUNCTION*------------------------------------------------------------------
*
* Function Name    : _lpm_idle_sleep_check
* Returned Value   : TRUE if idle sleep possible, FALSE otherwise
* Comments         :
*    This function checks whether cpu core can sleep during execution of idle task
*    in current power mode.
*
*END*----------------------------------------------------------------------*/

bool _lpm_idle_sleep_check
    (
        void
    )
{
    _mqx_uint pmctrl, stop;

    pmctrl = SMC_PMCTRL;
    stop = SIM_SOPT4 & (SIM_SOPT4_WAITE_MASK | SIM_SOPT4_STOPE_MASK);

    /* Idle sleep is available only in normal RUN/WAIT and VLPR/VLPW with LPWUI disabled */
    if (((SIM_SOPT4_WAITE_MASK | SIM_SOPT4_STOPE_MASK) == stop) && (0 == (pmctrl & SMC_PMCTRL_STOPM_MASK)) && (! ((SMC_PMCTRL_LPWUI_MASK | SMC_PMCTRL_RUNM(2)) == (pmctrl & (SMC_PMCTRL_LPWUI_MASK | SMC_PMCTRL_RUNM_MASK)))))
    {
        return TRUE;
    }

    return FALSE;
}

/*FUNCTION*------------------------------------------------------------------
*
* Function Name    : _lpm_llwu_clear_flag
* Returned Value   : Void
* Comments         :
*    This function is used to clear LLWU flags and pass them to caller
*
*END*----------------------------------------------------------------------*/

void _lpm_llwu_clear_flag
    (
        /* [OUT] Pointer stores value of LLWU_Fx flags */
        uint32_t *llwu_fx_ptr
    )
{
    volatile uint8_t LLWU_F1_TMP;
    volatile uint8_t LLWU_F2_TMP;
    volatile uint8_t LLWU_F3_TMP;

    /* Read LLWU_Fx into temporary LLWU_Fx_TMP variables */
    LLWU_F1_TMP = LLWU_F1;
    LLWU_F2_TMP = LLWU_F2;
    LLWU_F3_TMP = LLWU_F3;

    /* clean wakeup flags */
    LLWU_F1 = LLWU_F1_TMP;
    LLWU_F2 = LLWU_F2_TMP;

    if(LLWU_FILT1 & LLWU_FILT1_FILTF_MASK) {
        LLWU_FILT1 |= LLWU_FILT1_FILTF_MASK;
    }

    if(LLWU_FILT2 & LLWU_FILT2_FILTF_MASK) {
        LLWU_FILT2 |= LLWU_FILT2_FILTF_MASK;
    }

    *llwu_fx_ptr = (uint32_t)(LLWU_F3_TMP << 16) | (LLWU_F2_TMP << 8) | (LLWU_F1_TMP);
}



#endif /* MQX_ENABLE_LOW_POWER */


/* EOF */
