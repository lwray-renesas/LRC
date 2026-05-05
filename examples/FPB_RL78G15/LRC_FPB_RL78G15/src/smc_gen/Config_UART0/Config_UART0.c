/*
* Copyright (c) 2021 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_UART0.c
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
* Function Name: R_Config_UART0_Create
* Description  : This function initializes the UART0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_UART0_Create(void)
{
    uint8_t temp = SPS0L;

    temp &= _F0_SAU_CK00_CLEAR;
    temp |= _03_SAU_CK00_FCLK_3;
    SPS0L = temp;
    ST0L_bit.no0 = 1U;    /* disable UART0 transmit */
    ST0L_bit.no1 = 1U;    /* disable UART0 receive */
    STMK0 = 1U;    /* disable INTST0 interrupt */
    STIF0 = 0U;    /* clear INTST0 interrupt flag */
    SRMK0 = 1U;    /* disable INTSR0 interrupt */
    SRIF0 = 0U;    /* clear INTSR0 interrupt flag */
    SREMK0 = 1U;    /* disable INTSRE0 interrupt */
    SREIF0 = 0U;    /* clear INTSRE0 interrupt flag */
    /* Set INTST0 low priority */
    STPR10 = 1U;
    STPR00 = 1U;
    /* Set INTSR0 low priority */
    SRPR10 = 1U;
    SRPR00 = 1U;
    /* Set INTSRE0 low priority */
    SREPR10 = 1U;
    SREPR00 = 1U;
    SMR00 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0000_SAU_TRIGGER_SOFTWARE | 
            _0002_SAU_MODE_UART | _0001_SAU_BUFFER_EMPTY;
    SCR00 = _0004_SAU_SCRMN_INITIALVALUE | _8000_SAU_TRANSMISSION | _0000_SAU_INTSRE_MASK | _0000_SAU_PARITY_NONE | 
            _0080_SAU_LSB | _0010_SAU_STOP_1 | _0003_SAU_LENGTH_8;
    SDR00 = _CE00_SAU0_CH0_TRANSMIT_DIVISOR;
    NFEN0_bit.no0 = 1U;    /* RXD0 noise filter on */
    SIR01L = _04_SAU_SIRMN_FECTMN | _02_SAU_SIRMN_PECTMN | _01_SAU_SIRMN_OVCTMN;    /* clear error flag */
    SMR01 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0100_SAU_TRIGGER_RXD | _0000_SAU_EDGE_FALL | 
            _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;
    SCR01 = _0004_SAU_SCRMN_INITIALVALUE | _4000_SAU_RECEPTION | _0400_SAU_INTSRE_ENABLE | _0000_SAU_PARITY_NONE | 
            _0080_SAU_LSB | _0010_SAU_STOP_1 | _0003_SAU_LENGTH_8;
    SDR01 = _CE00_SAU0_CH1_RECEIVE_DIVISOR;
    SO0 |= _0001_SAU_CH0_DATA_OUTPUT_1;
    SOL0L |= _00_SAU_CHANNEL0_NORMAL;    /* output level normal */
    SOE0L_bit.no0 = 1U;    /* enable UART0 output */
    /* Set TxD0 pin */
    POM0 &= 0xEFU;
    PMC0 &= 0xEFU;
    P0 |= 0x10U;
    PM0 &= 0xEFU;
    /* Set RxD0 pin */
    PMC0 &= 0xDFU;
    PM0 |= 0x20U;

    R_Config_UART0_Create_UserInit();
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
