/*
* Copyright (c) 2021 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_ADC_user.c
* Component Version: 1.9.0
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
	/* Grab the sample, 10b ADC reading in 16bit register is left justified on RL78/G15, so right justify it and cast to sample type (32bit)*/
	lrc_channel.inputs.i_sample = (spl_t)((uint16_t)(ADCR >> 6U));

    /* Single ended 10bit ADC, approximately remove midway bias*/
	lrc_channel.inputs.i_sample -= 512L;

    /* (TODO: implement HPF)*/

    /* Enter LRC state machine*/
	LRC_CB_ADC();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

