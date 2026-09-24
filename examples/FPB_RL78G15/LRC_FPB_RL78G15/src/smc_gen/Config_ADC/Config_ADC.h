/*
* Copyright (c) 2021 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_ADC.h
* Component Version: 1.10.0
* Device(s)        : R5F12068xSP
* Description      : This file implements device driver for Config_ADC.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_ad.h"

#ifndef CFG_Config_ADC_H
#define CFG_Config_ADC_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define AD_WAITTIME_B                   (1U)    /* wait for 0.125us as default,
                                                   please change the waiting time value according to the system */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef enum
{
    ADCHANNEL0, ADCHANNEL1, ADCHANNEL2, ADCHANNEL3, ADCHANNEL4, ADCHANNEL5,
    ADCHANNEL6, ADCHANNEL7, ADCHANNEL8, ADCHANNEL9, ADCHANNEL10, ADINTERREFVOLT = 13U
} e_ad_channel_t;
typedef enum
{
    ADNORMALINPUT,
    VSSINPUT = 2U
} e_test_channel_t;

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_Config_ADC_Create (void);
void R_Config_ADC_Start (void);
void R_Config_ADC_Stop (void);
void R_Config_ADC_Set_OperationOn (void);
void R_Config_ADC_Set_OperationOff (void);
void R_Config_ADC_Get_Result_10bit (uint16_t * const buffer);
MD_STATUS R_Config_ADC_Set_ADChannel (e_ad_channel_t channel);
MD_STATUS R_Config_ADC_Set_TestChannel (e_test_channel_t channel);
void R_Config_ADC_Create_UserInit (void);
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif

