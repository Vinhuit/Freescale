/**HEADER********************************************************************
*
* Copyright (c) 2013 Freescale Semiconductor;
* All Rights Reserved
*
***************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************
*
* $FileName: rnga.c$
* $Version : 4.1.0$
* $Date    : Dec-25-2013$
*
* Comments:
*            Kinetis RNGA driver functions
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include "rnga.h"
/*FUNCTION****************************************************************
*
* Function Name    : _rnga_init
* Returned Value   : void
* Comments         :
*    This function (re)initializes and enables RNGA module.
*
*END*********************************************************************/
void _rnga_init
(
    RNGA_STRUCT_PTR handle
)
{ /* Body */
    RNG_MemMapPtr rnga    = RNG_BASE_PTR;

    _bsp_rnga_io_init();

    handle->rnga_ptr = rnga;

    rnga->CR |= RNG_CR_INTM_MASK;
    rnga->CR |= RNG_CR_HA_MASK;
    rnga->CR |= RNG_CR_GO_MASK;

} /* Endbody */

/*FUNCTION****************************************************************
*
* Function Name    : _rnga_insert_entropy
* Returned Value   : void
* Comments         :
*    This function inserts entropy into the RNGA.
*
*END*********************************************************************/
void _rnga_insert_entropy
(
    /* [IN] given seed to be inserted into the RNGA */
    RNGA_STRUCT_PTR handle, uint32_t seed
)
{ /* Body */
    /* seed RNGA */
    handle->rnga_ptr->ER = seed;

} /* Endbody */

/*FUNCTION****************************************************************
*
* Function Name    : _rnga_get_random_data
* Returned Value   : RNGA_OK or RNGA_ERROR
* Comments         :
*    This function gets the random data from RNGA.
*
*END*********************************************************************/
int32_t _rnga_get_random_data
(
    RNGA_STRUCT_PTR handle, uint32_t *randout
)
{ /* Body */
    volatile uint8_t oreg_lvl = 0;
    uint32_t count = 0;
    RNG_MemMapPtr rnga = handle->rnga_ptr;

    _int_disable();
	
    while (oreg_lvl == 0) {
      oreg_lvl = ((rnga->SR) & RNG_SR_OREG_LVL_MASK) >> RNG_SR_OREG_LVL_SHIFT;
      count ++;
      if (count == MAX_COUNT)
        return RNGA_ERROR;
    }
    *randout = (uint32_t)rnga->OR;

    _int_enable();

    return RNGA_OK;
} /* Endbody */

/*FUNCTION****************************************************************
*
* Function Name    : _rnga_set_mode
* Returned Value   : void
* Comments         :
*    This function sets RNGA mode into normal or sleep.
*
*END*********************************************************************/
void _rnga_set_mode
(
    /* [IN] enter sleep or normal mode */
    RNGA_STRUCT_PTR handle,  uint8_t mode
)
{ /* Body */
    RNG_MemMapPtr rnga = handle->rnga_ptr;

    if (mode == SLEEP_MODE)
      rnga->CR |= RNG_CR_SLP_MASK;
    else
      rnga->CR &= ~RNG_CR_SLP_MASK;
} /* Endbody */

/*FUNCTION****************************************************************
*
* Function Name    : _rnga_get_mode
* Returned Value   : RNGA mode, 0--Normal mode, 1-- Sleep mode
* Comments         :
*    This function get RNGA mode.
*
*END*********************************************************************/
uint8_t _rnga_get_mode
(
    /* [IN] get rnga mode */
    RNGA_STRUCT_PTR handle
)
{ /* Body */
    return ((handle->rnga_ptr->SR) & RNG_SR_SLP_MASK) >> RNG_SR_SLP_SHIFT;
} /* Endbody */

