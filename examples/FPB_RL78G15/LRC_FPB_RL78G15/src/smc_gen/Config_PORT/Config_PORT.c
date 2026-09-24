/*
* Copyright (c) 2021 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_PORT.c
* Component Version: 1.9.1
* Device(s)        : R5F12068xSP
* Description      : This file implements device driver for Config_PORT.
***********************************************************************************************************************/
/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "Config_PORT.h"
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
* Function Name: R_Config_PORT_Create
* Description  : This function initializes the port I/O.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_PORT_Create(void)
{
    /* Set PORT2 registers */
    P2 = _00_Pn3_OUTPUT_0 | _00_Pn2_OUTPUT_0 | _00_Pn1_OUTPUT_0 | _01_Pn0_OUTPUT_1;
    POM2 = _00_POMn2_NCH_OFF;
    PMC2 = _F0_PMC2_DEFAULT | _08_PMCn3_NOT_USE | _04_PMCn2_NOT_USE | _02_PMCn1_NOT_USE | _00_PMCn0_DIGITAL_ON;
    PM2 = _F0_PM2_DEFAULT | _08_PMn3_NOT_USE | _04_PMn2_NOT_USE | _02_PMn1_NOT_USE | _00_PMn0_MODE_OUTPUT;

    R_Config_PORT_Create_UserInit();
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

