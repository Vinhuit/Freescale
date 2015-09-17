/**HEADER***********************************************************************
*
* Copyright (c) 2013 Freescale Semiconductor;
* All Rights Reserved
*
********************************************************************************
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
********************************************************************************
*
* $FileName: rnga.h$
* $Version : 4.1.0$
* $Date    : Dec-25-2013$
*
* Comments:
*
*   This include file is used to provide constant and prototype definitions
*   common for RNGA module
*
*END***************************************************************************/

#ifndef __rnga_h__
#define __rnga_h__ 1

/*--------------------------------------------------------------------------*/
/*
 *                  PROTOTYPES OF RTC FUNCTIONS
 */

#define NORMAL_MODE           0
#define SLEEP_MODE            1

#define RNGA_OK               0
#define RNGA_ERROR            1

#define MAX_COUNT             100

typedef struct {
    RNG_MemMapPtr rnga_ptr;
} RNGA_STRUCT, * RNGA_STRUCT_PTR;


extern void  _rnga_init(RNGA_STRUCT_PTR handle);
extern void  _rnga_insert_entropy(RNGA_STRUCT_PTR handle, uint32_t seed);
extern int32_t  _rnga_get_random_data(RNGA_STRUCT_PTR handle, uint32_t*);
extern void  _rnga_set_mode(RNGA_STRUCT_PTR handle, uint8_t mode);
extern uint8_t  _rnga_get_mode(RNGA_STRUCT_PTR handle);

#endif /* __rnga_h__ */

/* EOF */
