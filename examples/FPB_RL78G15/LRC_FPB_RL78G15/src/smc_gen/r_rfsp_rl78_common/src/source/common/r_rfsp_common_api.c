/**********************************************************************************************************************
    Program Name    : Renesas Flash Sample Program (RFSP Type01 for RL78/G15,RL78/G16)
    
    File Name       : r_rfsp_common_api.c
    Program Version : V1.10
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
    
    Copyright (C) 2021-2022 Renesas Electronics Corporation. All rights reserved.
**********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_rfsp_common_api.h"
#include "r_rfsp_common_userown.h"

/**********************************************************************************************************************
 * Function name : R_RFSP_Init
 *********************************************************************************************************************/
/*********************************************************************************************************************/
/**
 *  Initialize the RFSP.
 *  
 *  @param[in]      i_u08_cpu_frequency : 
 *                    CPU operating frequency
 *  @return         Execution result status
 *                  - R_RFSP_ENUM_RET_STS_OK :        Successful completion
 *                  - R_RFSP_ENUM_RET_ERR_PARAMETER : Out of parameter range
 */
/*********************************************************************************************************************/
R_RFSP_FAR_FUNC e_rfsp_ret_t R_RFSP_Init(uint8_t i_u08_cpu_frequency)
{
    /* Local variable definitions */
    e_rfsp_ret_t l_e_ret_value;
    
    /* Local variables initialization */
    l_e_ret_value = R_RFSP_ENUM_RET_STS_OK;
    
    if ((R_RFSP_VALUE_U08_FREQUENCY_LOWER_LIMIT <= i_u08_cpu_frequency) 
       && (R_RFSP_VALUE_U08_FREQUENCY_UPPER_LIMIT >= i_u08_cpu_frequency))
    {
        if (R_RFSP_ENUM_RET_STS_OK == l_e_ret_value)
        {
            /* Set frequency */
            R_RFSP_REG_U08_FSSET = ( i_u08_cpu_frequency - R_RFSP_VALUE_U08_FREQUENCY_ADJUST );
        }
        else
        {
            /* No operation */
        }
    }
    else
    {
        /* Set return value */
        l_e_ret_value = R_RFSP_ENUM_RET_ERR_PARAMETER;
    }
    
    return (l_e_ret_value);
}

/**********************************************************************************************************************
 End of function R_RFSP_Init
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name : R_RFSP_SetFlashMemoryMode
 *********************************************************************************************************************/
/*********************************************************************************************************************/
/**
 *  Set the flash memory mode to the specified mode and set the flash operating frequency.
 *  
 *  @param[in]      i_e_flash_mode : 
 *                    Flash memory mode
 *  @return         Execution result status
 *                  - R_RFSP_ENUM_RET_STS_OK :              Successful completion
 *                  - R_RFSP_ENUM_RET_ERR_MODE_MISMATCHED : Failed to change flash memory mode
 */
/*********************************************************************************************************************/
R_RFSP_FAR_FUNC e_rfsp_ret_t R_RFSP_SetFlashMemoryMode(e_rfsp_flash_memory_mode_t i_e_flash_mode)
{
    /* Local variable definitions */
    e_rfsp_ret_t l_e_ret_value;
    uint8_t      l_u08_set_flpmc_value;
    
    /* Local variables initialization */
    l_e_ret_value = R_RFSP_ENUM_RET_STS_OK;
    
    /* Set code flash programming mode */
    if (R_RFSP_ENUM_FLASH_MODE_CODE_PROGRAMMING == i_e_flash_mode)
    {
        /* Code flash programming mode */
        l_u08_set_flpmc_value = R_RFSP_VALUE_U08_FLPMC_MODE_CODE_FLASH_PROGRAMMING;
    }
    /* Set data flash programming mode */
    else if (R_RFSP_ENUM_FLASH_MODE_DATA_PROGRAMMING == i_e_flash_mode)
    {
        /* Data flash programming mode */
        l_u08_set_flpmc_value = R_RFSP_VALUE_U08_FLPMC_MODE_DATA_FLASH_PROGRAMMING;
    }
    /* Set nonprogrammable mode */
    else
    {
        /* Nonprogrammable mode */
        l_u08_set_flpmc_value = R_RFSP_VALUE_U08_FLPMC_MODE_NONPROGRAMMABLE;
    }
    
    /* Set mode */
    R_RFSP_REG_U08_FLPMC = l_u08_set_flpmc_value;
    
    /* Check mode */
    l_e_ret_value = R_RFSP_CheckFlashMemoryMode(i_e_flash_mode);
    
    return (l_e_ret_value);
}

/**********************************************************************************************************************
 End of function R_RFSP_SetFlashMemoryMode
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : R_RFSP_CheckFlashMemoryMode
 *********************************************************************************************************************/
/*********************************************************************************************************************/
/**
 *  Check whether the current flash memory mode is same as specified mode.
 *  
 *  @param[in]      i_e_flash_mode : 
 *                    Flash memory mode
 *  @return         Execution result status
 *                  - R_RFSP_ENUM_RET_STS_OK :              Successful completion
 *                  - R_RFSP_ENUM_RET_ERR_MODE_MISMATCHED : Mode mismatch error (Not same)
 */
/*********************************************************************************************************************/
R_RFSP_FAR_FUNC e_rfsp_ret_t R_RFSP_CheckFlashMemoryMode(e_rfsp_flash_memory_mode_t i_e_flash_mode)
{
    /* Local variable definitions */
    e_rfsp_ret_t l_e_ret_value;
    uint8_t      l_u08_flpmc_value;
    uint8_t      l_u08_flpmc_expected_value;
    
    /* Local variables initialization */
    l_e_ret_value = R_RFSP_ENUM_RET_STS_OK;
    l_u08_flpmc_expected_value = R_RFSP_VALUE_U08_INIT_VARIABLE;
    
    /* Acquire the value of FLPMC register */
    l_u08_flpmc_value  = R_RFSP_REG_U08_FLPMC;
    
    if (R_RFSP_ENUM_FLASH_MODE_DATA_PROGRAMMING == i_e_flash_mode)
    {
        l_u08_flpmc_expected_value = R_RFSP_VALUE_U08_FLPMC_MODE_DATA_FLASH_PROGRAMMING;
    }
    else if (R_RFSP_ENUM_FLASH_MODE_CODE_PROGRAMMING == i_e_flash_mode)
    {
        l_u08_flpmc_expected_value = R_RFSP_VALUE_U08_FLPMC_MODE_CODE_FLASH_PROGRAMMING;
    }
    else if (R_RFSP_ENUM_FLASH_MODE_NONPROGRAMMABLE  == i_e_flash_mode)
    {
        l_u08_flpmc_expected_value = R_RFSP_VALUE_U08_FLPMC_MODE_NONPROGRAMMABLE;
    }
    else
    {
        /* No operation */
    }
    
    if (l_u08_flpmc_expected_value == l_u08_flpmc_value)
    {
        /* Set return value */
        l_e_ret_value = R_RFSP_ENUM_RET_STS_OK;
    }
    else
    {
        /* Set return value */
        l_e_ret_value = R_RFSP_ENUM_RET_ERR_MODE_MISMATCHED;
    }
    
    return (l_e_ret_value);
}

/**********************************************************************************************************************
 End of function R_RFSP_CheckFlashMemoryMode
 *********************************************************************************************************************/
