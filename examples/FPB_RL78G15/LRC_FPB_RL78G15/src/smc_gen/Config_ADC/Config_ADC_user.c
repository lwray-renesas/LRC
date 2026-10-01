/*
* Copyright (c) 2021 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_ADC_user.c
* Component Version: 1.10.0
* Device(s)        : R5F12068xSP
* Description      : This file implements device driver for Config_ADC.
***********************************************************************************************************************/
/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "Config_ADC.h"
/* Start user code for include. Do not edit comment generated here */
#include "LRC_Core.h"
#include "hpf.h"


#if 8 == LRC_PORT_ADC_BITS
#define ADC_BIT_SHIFT (8U)
#elif 10 == LRC_PORT_ADC_BITS
#define ADC_BIT_SHIFT (6U)
#endif

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt r_Config_ADC_interrupt(vect=INTAD)
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
extern LRC_Channel lrc_channel;
extern bool adc_ready;
extern Hpf l_hpf;
static volatile int16_t raw_adc = 0;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_ADC_Create_UserInit
* Description  : This function adds user code after initializing the AD converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_ADC_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_ADC_interrupt
* Description  : This function is INTAD interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_Config_ADC_interrupt(void)
{
    /* Start user code for r_Config_ADC_interrupt. Do not edit comment generated here */
	P2 |= 1U;

	/* Grab the sample, result register is left justified on RL78/G15 */
	raw_adc = (uint16_t)(ADCR >> ADC_BIT_SHIFT);
	/* Single ended ADC, approximately remove midway bias before HPF*/
	raw_adc -= (1U << (LRC_PORT_ADC_BITS - 1));
	/* High pass filter*/
	lrc_channel.inputs.iac_sample = (spl_t)Hpf_run(&l_hpf, raw_adc);

    /* Enter LRC state machine*/
	LRC_CB_ADC();

	adc_ready = true;

	P2 &= ~1U;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

