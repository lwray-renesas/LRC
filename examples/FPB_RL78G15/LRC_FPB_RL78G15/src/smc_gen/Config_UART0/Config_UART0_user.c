/*
* Copyright (c) 2021 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_UART0_user.c
* Component Version: 1.10.1
* Device(s)        : R5F12068xSP
* Description      : This file implements device driver for Config_UART0.
***********************************************************************************************************************/
/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "Config_UART0.h"
/* Start user code for include. Do not edit comment generated here */
#include "stdbool.h"
#include "Menu.h"
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
#pragma interrupt r_Config_UART0_interrupt_send(vect=INTST0)
#pragma interrupt r_Config_UART0_interrupt_receive(vect=INTSR0)
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
static bool tx_send_end = true;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_UART0_Create_UserInit
* Description  : This function adds user code after initializing UART0.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_UART0_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

static void __near r_Config_UART0_interrupt_send(void)
{
	tx_send_end = true;
}

static void __near r_Config_UART0_interrupt_receive(void)
{
    volatile uint8_t rx_data;
    volatile uint8_t err_type;

    err_type = (uint8_t)(SSR01 & 0x0007U);
    SIR01L = err_type;

    Menu_get_byte(RXD0);
}

void R_UART0_Start(void)
{
    SO0 |= _0001_SAU_CH0_DATA_OUTPUT_1;    /* output level normal */
    SOE0L_bit.no0 = 1U;    /* enable UART0 output */
    SS0L_bit.no0 = 1U; /* enable UART0 transmit */
    SS0L_bit.no1 = 1U; /* enable UART0 receive */
    STIF0 = 0U;    /* clear INTST0 interrupt flag */
    SRIF0 = 0U;    /* clear INTSR0 interrupt flag */
    STMK0 = 0U;    /* enable INTST0 interrupt */
    SRMK0 = 0U;    /* enable INTSR0 interrupt */
}

void R_UART0_SendByte(uint8_t b)
{
	while(!tx_send_end)
	{
		NOP();
	}

	tx_send_end = false;

    STMK0 = 1U;    /* disable INTST0 interrupt */
    TXD0 = b;
    STMK0 = 0U;    /* enable INTST1 interrupt */

	while(!tx_send_end)
	{
		NOP();
	}
}

/* End user code. Do not edit comment generated here */
