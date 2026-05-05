/*
* Copyright (c) 2021 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : r_cg_ad_common.c
* Version          : 1.0.100
* Device(s)        : R5F12068xSP
* Description      : None
***********************************************************************************************************************/
/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "r_cg_ad_common.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_ADC_Set_PowerOn
* Description  : This function starts the clock supply for AD converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_ADC_Set_PowerOn(void)
{
    ADCEN = 1U;    /* start AD clock */
}

/***********************************************************************************************************************
* Function Name: R_ADC_Set_PowerOff
* Description  : This function stops the clock supply for AD converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_ADC_Set_PowerOff(void)
{
    ADCEN = 0U;    /* stop AD clock */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

