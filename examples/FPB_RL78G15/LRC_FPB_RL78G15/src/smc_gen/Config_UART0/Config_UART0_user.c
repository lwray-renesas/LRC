/*
* Copyright (c) 2021 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_UART0_user.c
* Component Version: 1.12.0
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
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt r_Config_UART0_interrupt_send(vect=INTST0)
#pragma interrupt r_Config_UART0_interrupt_receive(vect=INTSR0)
#pragma interrupt r_Config_UART0_interrupt_error(vect=INTSRE0)
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t * gp_uart0_tx_address;    /* uart0 transmit buffer address */
extern volatile uint16_t g_uart0_tx_count;        /* uart0 transmit data number */
extern volatile uint8_t * gp_uart0_rx_address;    /* uart0 receive buffer address */
extern volatile uint16_t g_uart0_rx_count;        /* uart0 receive data number */
extern uint16_t g_uart0_rx_length;                /* uart0 receive data length */
/* Start user code for global. Do not edit comment generated here */
extern uint8_t g_rx_buf[2U];
extern bool transmit_meta;
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

/***********************************************************************************************************************
* Function Name: r_Config_UART0_callback_sendend
* Description  : This function is a callback function when UART0 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_Config_UART0_callback_sendend(void)
{
    /* Start user code for r_Config_UART0_callback_sendend. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_UART0_callback_receiveend
* Description  : This function is a callback function when UART0 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_Config_UART0_callback_receiveend(void)
{
    /* Start user code for r_Config_UART0_callback_receiveend. Do not edit comment generated here */
    if('M' == g_rx_buf[0U])
    {
        transmit_meta = true; /* Requested to transmit meta data*/
    }

    R_Config_UART0_Receive(g_rx_buf, 1U);
	/* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_UART0_interrupt_send
* Description  : This function is UART0 send interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_Config_UART0_interrupt_send(void)
{
    if (g_uart0_tx_count > 0U)
    {
        TXD0 = *gp_uart0_tx_address;
        gp_uart0_tx_address++;
        g_uart0_tx_count--;
    }
    else
    {
        if (0U == (SSR00 & _0040_SAU_UNDER_EXECUTE))
        {
            r_Config_UART0_callback_sendend();
        }
        else
        {
            SMR00 &= (uint16_t)~_0001_SAU_BUFFER_EMPTY;
        }
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_UART0_interrupt_receive
* Description  : This function is UART0 receive interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_Config_UART0_interrupt_receive(void)
{
    uint8_t rx_data;

    rx_data = RXD0;

    if (g_uart0_rx_length > g_uart0_rx_count)
    {
        *gp_uart0_rx_address = rx_data;
        gp_uart0_rx_address++;
        g_uart0_rx_count++;

        if (g_uart0_rx_length == g_uart0_rx_count)
        {
            r_Config_UART0_callback_receiveend();
        }
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_UART0_interrupt_error
* Description  : This function is UART0 error interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_Config_UART0_interrupt_error(void)
{
    uint8_t err_type;

    *gp_uart0_rx_address = RXD0;
    err_type = (uint8_t)(SSR01 & 0x0007U);
    SIR01L = err_type;
}

/* Start user code for adding. Do not edit comment generated here */

void R_Config_UART0_Send_Blocking (uint8_t * const tx_buf, uint16_t tx_num)
{
	const uint8_t * p_tx_buf = tx_buf;
	uint16_t l_tx_num = tx_num;

    STMK0 = 1U;    /* disable INTST0 interrupt */
    STIF0 = 0U;
	while(l_tx_num > 0)
	{
		TXD0 = *p_tx_buf;
	    ++p_tx_buf;
	    --l_tx_num;
	    while(0U == STIF0)
	    {
	    	NOP();
	    }
	    STIF0 = 0U;
	}
}

/* End user code. Do not edit comment generated here */
