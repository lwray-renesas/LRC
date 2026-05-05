/**********************************************************************************************************************
    Program Name    : Renesas Flash Sample Program (RFSP Type01 for RL78/G15,RL78/G16)
    
    File Name       : r_rfsp_common_control_api.c
    Program Version : V1.20
    Device(s)       : RL78/G15,RL78/G16 microcontroller
    Description     : Common Flash Control program
**********************************************************************************************************************/

/**********************************************************************************************************************
    DISCLAIMER
    This software is supplied by Renesas Electronics Corporation and is only intended for use with
    Renesas products. No other uses are authorized. This software is owned by Renesas Electronics
    Corporation and is protected under all applicable laws, including copyright laws.
    THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE,
    WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
    TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR
    ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR
    CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
    BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
    Renesas reserves the right, without notice, to make changes to this software and to discontinue the
    availability of this software. By using this software, you agree to the additional terms and conditions
    found by accessing the following link:
    http://www.renesas.com/disclaimer
    
    Copyright (C) 2021-2023 Renesas Electronics Corporation. All rights reserved.
**********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_rfsp_common_control_api.h"

#if (COMPILER_IAR == COMPILER)
    #include "intrinsics.h"
#endif

/**********************************************************************************************************************
 * Function name : R_RFSP_CheckCFDFSeqEndStep1
 *********************************************************************************************************************/
/*********************************************************************************************************************/
/**
 *  Check whether the operation of the activated code/data flash memory sequencer has been completed.
 *  If completed, then clear the control register for code/data flash memory sequencer.
 *  
 *  @param[in]      -
 *  @return         Execution result status
 *                  - R_RFSP_ENUM_RET_STS_OK :   The operation of the code/data flash memory sequencer is completed
 *                  - R_RFSP_ENUM_RET_STS_BUSY : The code/data flash memory sequencer is still being processed
 */
/*********************************************************************************************************************/
R_RFSP_FAR_FUNC e_rfsp_ret_t R_RFSP_CheckCFDFSeqEndStep1(void)
{
    /* Local variable definitions */
    e_rfsp_ret_t l_e_ret_value;
    uint8_t      l_u08_fsasth_value;
    
    /* Set local variables */
    l_e_ret_value      = R_RFSP_ENUM_RET_STS_OK;
    l_u08_fsasth_value = R_RFSP_REG_U08_FSASTH;
    
    /* SQEND bit is valid */
    if (0u != (l_u08_fsasth_value & R_RFSP_VALUE_U08_MASK1_FSASTH_SQEND))
    {
        /* Set return value */
        l_e_ret_value = R_RFSP_ENUM_RET_STS_OK;
        
        /* Set the value for FSSQ register */
        R_RFSP_REG_U08_FSSQ = R_RFSP_VALUE_U08_FSSQ_CLEAR;
    }
    /* SQEND bit is invalid */
    else
    {
        /* Set return value */
        l_e_ret_value = R_RFSP_ENUM_RET_STS_BUSY;
    }
    
    return (l_e_ret_value);
}

/**********************************************************************************************************************
 End of function R_RFSP_CheckCFDFSeqEndStep1
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name : R_RFSP_CheckCFDFSeqEndStep2
 *********************************************************************************************************************/
/*********************************************************************************************************************/
/**
 *  Check whether the code/data flash memory sequencer command has been completed 
 *  by clearing the control register for the code/data flash memory sequencer.
 *  
 *  @param[in]      -
 *  @return         Execution result status
 *                  - R_RFSP_ENUM_RET_STS_OK :   The command of code/data flash memory sequencer is completed
 *                  - R_RFSP_ENUM_RET_STS_BUSY : The command is still being processed
 */
/*********************************************************************************************************************/
R_RFSP_FAR_FUNC e_rfsp_ret_t R_RFSP_CheckCFDFSeqEndStep2(void)
{
    /* Local variable definitions */
    e_rfsp_ret_t l_e_ret_value;
    uint8_t      l_u08_fsasth_value;
    
    /* Set local variables */
    l_e_ret_value      = R_RFSP_ENUM_RET_STS_OK;
    l_u08_fsasth_value = R_RFSP_REG_U08_FSASTH;
    
    /* SQEND bit is invalid */
    if (0u == (l_u08_fsasth_value & R_RFSP_VALUE_U08_MASK1_FSASTH_SQEND))
    {
        /* Set return value */
        l_e_ret_value = R_RFSP_ENUM_RET_STS_OK;
    }
    /* SQEND bit is valid */
    else
    {
        /* Set return value */
        l_e_ret_value = R_RFSP_ENUM_RET_STS_BUSY;
    }
    
    return (l_e_ret_value);
}

/**********************************************************************************************************************
 End of function R_RFSP_CheckCFDFSeqEndStep2
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name : R_RFSP_GetSeqErrorStatus
 *********************************************************************************************************************/
/*********************************************************************************************************************/
/**
 *  Get the error information occurred by the code/data flash memory sequencer command.
 *  
 *  @param[out]     onp_u08_error_status : 
 *                    Pointer to the variable that stores error information
 *  @return         ---
 */
/*********************************************************************************************************************/
R_RFSP_FAR_FUNC void R_RFSP_GetSeqErrorStatus(uint8_t __near * onp_u08_error_status)
{
    /* Local variable definitions */
    uint8_t l_u08_fsastl_value;
    
    /* Set local variables */
    l_u08_fsastl_value = R_RFSP_REG_U08_FSASTL;
    
    /* Write to parameter */
    * onp_u08_error_status = l_u08_fsastl_value & R_RFSP_VALUE_U08_MASK1_FSASTL_ERROR_FLAG;
}

/**********************************************************************************************************************
 End of function R_RFSP_GetSeqErrorStatus
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name : R_RFSP_ForceReset
 *********************************************************************************************************************/

#if (COMPILER_CC == COMPILER)
    #pragma inline_asm R_RFSP_ForceReset
#endif
/*********************************************************************************************************************/
/**
 *  Reset the CPU.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_RFSP_FAR_FUNC void R_RFSP_ForceReset(void)
{
    /* Set illegal instruction to request internal reset */
    #if (COMPILER_CC == COMPILER)
    .DB 255
    #elif (COMPILER_IAR == COMPILER)
    __asm("DC8 0xFF");
    #elif (COMPILER_LLVM == COMPILER)
    __asm(".byte 0xff");
    #endif
}

/**********************************************************************************************************************
 End of function R_RFSP_ForceReset
 *********************************************************************************************************************/
