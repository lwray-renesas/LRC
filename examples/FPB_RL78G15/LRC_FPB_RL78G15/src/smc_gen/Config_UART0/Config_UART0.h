/*
* Copyright (c) 2021 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_UART0.h
* Component Version: 1.12.0
* Device(s)        : R5F12068xSP
* Description      : This file implements device driver for Config_UART0.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_sau.h"

#ifndef CFG_Config_UART0_H
#define CFG_Config_UART0_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define _1800_SAU0_CH0_TRANSMIT_DIVISOR   (0x1800U)
#define _1800_SAU0_CH1_RECEIVE_DIVISOR    (0x1800U)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_Config_UART0_Create (void);
void R_Config_UART0_Start (void);
void R_Config_UART0_Stop (void);
MD_STATUS R_Config_UART0_Send (uint8_t * const tx_buf, uint16_t tx_num);
MD_STATUS R_Config_UART0_Receive (uint8_t * const rx_buf, uint16_t rx_num);
void R_Config_UART0_Create_UserInit (void);
/* Start user code for function. Do not edit comment generated here */

void R_Config_UART0_Send_Blocking (uint8_t * const tx_buf, uint16_t tx_num);

/* End user code. Do not edit comment generated here */
#endif
