/*
* Copyright (c) 2021 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_TAU0_0_user.c
* Component Version: 1.8.1
* Device(s)        : R5F12068xSP
* Description      : This file implements device driver for Config_TAU0_0.
***********************************************************************************************************************/
/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "Config_TAU0_0.h"
/* Start user code for include. Do not edit comment generated here */
#include "Config_ADC.h"
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt r_Config_TAU0_0_interrupt(vect=INTTM00)
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_TAU0_0_Create_UserInit
* Description  : This function adds user code after initializing the TAU0 channel 0.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_TAU0_0_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_TAU0_0_interrupt
* Description  : This function is INTTM00 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_Config_TAU0_0_interrupt(void)
{
    /* Start user code for r_Config_TAU0_0_interrupt. Do not edit comment generated here */
	R_Config_ADC_Start();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
