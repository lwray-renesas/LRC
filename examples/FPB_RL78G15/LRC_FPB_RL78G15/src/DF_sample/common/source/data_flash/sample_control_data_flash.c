/**********************************************************************************************************************
    Program Name    : Sample program for Renesas Flash Sample Program (RFSP Type01 for RL78/G15,RL78/G16) (Data Flash)
    
    File Name       : sample_control_data_flash.c
    Program Version : V1.20.00
    Device(s)       : RL78/G15,RL78/G16 microcontroller
    Description     : Sample program for data flash control
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
#include "sample_control_data_flash.h"
#include "sample_control_common.h"
#include "sample_defines.h"

#include "r_rfsp_common_api.h"
#include "r_rfsp_common_userown.h"
#include "r_rfsp_data_flash_api.h"

/**********************************************************************************************************************
 * Function name : Sample_DataFlashControl
 *********************************************************************************************************************/
/*********************************************************************************************************************/
/**
 *  Sample function for data flash control.
 *  BlankCheck -> Erase -> Write
 *  
 *  @param[in]      i_u16_start_addr : 
 *                    Command start address (The start address of the block)
 *                  i_u16_write_data_length : 
 *                    Write data length (MAX : 1 block size)
 *                  inp_u08_write_data : 
 *                    Write data pointer
 *  @return         Execution result status
 *                  - SAMPLE_ENUM_RET_STS_OK               : Successful completion
 *                  - SAMPLE_ENUM_RET_ERR_MODE_MISMATCHED  : Mode mismatched error
 *                  - SAMPLE_ENUM_RET_ERR_CHECK_WRITE_DATA : Check write data error
 *                  - SAMPLE_ENUM_RET_ERR_CMD_ERASE        : Erase command error
 *                  - SAMPLE_ENUM_RET_ERR_CMD_WRITE        : Write command error
 */
/*********************************************************************************************************************/
R_RFSP_FAR_FUNC e_sample_ret_t Sample_DataFlashControl(uint16_t i_u16_start_addr, 
                                                      uint16_t i_u16_write_data_length, 
                                                      uint8_t __near * inp_u08_write_data)
{
    /* Local variable definition */
    e_rfsp_ret_t    l_e_rfsp_ret_status;
    e_sample_ret_t  l_e_sam_ret_status;
    e_sample_ret_t  l_e_sam_ret_value;
    bool            l_e_sam_error_flag;
    uint16_t        l_u16_count;
    uint8_t         l_u08_block_number;
    uint16_t        l_u16_check_write_data_addr;

    /* Set local variables */
    l_e_sam_ret_value           = SAMPLE_ENUM_RET_STS_OK;
    l_e_sam_error_flag          = false;
    /* This expression (actual block number) never exceeds the range of casting uint8_t */
    l_u08_block_number          = (uint8_t)((i_u16_start_addr - SAMPLE_VALUE_U16_DF_BASE_ADDR) 
                                  >> SAMPLE_VALUE_U08_SHIFT_ADDR_TO_BLOCK_DF);
    l_u16_check_write_data_addr = i_u16_start_addr;
    
    /******************************************************************************************************************
     * Set the data flash programming mode
     *****************************************************************************************************************/
    R_RFSP_HOOK_EnterCriticalSection();

    l_e_rfsp_ret_status = R_RFSP_SetFlashMemoryMode(R_RFSP_ENUM_FLASH_MODE_DATA_PROGRAMMING);
    
    if (R_RFSP_ENUM_RET_STS_OK != l_e_rfsp_ret_status)
    {
        l_e_sam_error_flag = true;
        l_e_sam_ret_value  = SAMPLE_ENUM_RET_ERR_MODE_MISMATCHED;
    }
    else
    {
        /* No operation */
    }
    
    /******************************************************************************************************************
     * ERASE
     *****************************************************************************************************************/
    if (false == l_e_sam_error_flag)
    {
        /* ERASE (1 block) */
        R_RFSP_EraseDataFlashReq(l_u08_block_number);
        l_e_sam_ret_status = Sample_CheckCFDFSeqEnd();
        
        if (SAMPLE_ENUM_RET_STS_OK != l_e_sam_ret_status)
        {
            l_e_sam_error_flag = true;
            l_e_sam_ret_value  = SAMPLE_ENUM_RET_ERR_CMD_ERASE;
        }
        else
        {
            /* No operation */
        }
    }
    else /* true == l_e_sam_error_flag */
    {
        /* No operation */
    }
    
    /******************************************************************************************************************
     * WRITE
     *****************************************************************************************************************/
    if (false == l_e_sam_error_flag)
    {
        for (l_u16_count = 0u; (l_u16_count < i_u16_write_data_length) && (false == l_e_sam_error_flag); 
             l_u16_count += 4u)
        {
            R_RFSP_WriteDataFlashReq(i_u16_start_addr + l_u16_count, &inp_u08_write_data[l_u16_count]);
            l_e_sam_ret_status = Sample_CheckCFDFSeqEnd();
            
            if (SAMPLE_ENUM_RET_STS_OK != l_e_sam_ret_status)
            {
                l_e_sam_error_flag = true;
                l_e_sam_ret_value  = SAMPLE_ENUM_RET_ERR_CMD_WRITE;
            }
            else
            {
                /* No operation */
            }
        }
    }
    else /* true == l_e_sam_error_flag */
    {
        /* No operation */
    }
    
    /******************************************************************************************************************
     * Set nonprogrammable mode
     *****************************************************************************************************************/
    l_e_rfsp_ret_status = R_RFSP_SetFlashMemoryMode(R_RFSP_ENUM_FLASH_MODE_NONPROGRAMMABLE);
    
    if (R_RFSP_ENUM_RET_STS_OK != l_e_rfsp_ret_status)
    {
        l_e_sam_error_flag = true;
        l_e_sam_ret_value  = SAMPLE_ENUM_RET_ERR_MODE_MISMATCHED;
    }
    else
    {
        /* No operation */
    }
    
    R_RFSP_HOOK_ExitCriticalSection();

    /******************************************************************************************************************
     * Check write data
     *****************************************************************************************************************/
    if (false == l_e_sam_error_flag)
    {
        for (l_u16_count = 0u; (l_u16_count < i_u16_write_data_length) && (false == l_e_sam_error_flag) ; l_u16_count++)
        {
            if (inp_u08_write_data[l_u16_count] != (*(uint8_t __far *)((uint32_t)l_u16_check_write_data_addr)))
            {
                l_e_sam_error_flag = true;
                l_e_sam_ret_value  = SAMPLE_ENUM_RET_ERR_CHECK_WRITE_DATA;
            }
            else
            {
                /* No operation */
            }
            l_u16_check_write_data_addr++;
        }
    }
    else /* true == l_e_sam_error_flag */
    {
        /* No operation */
    }

    return (l_e_sam_ret_value);
}

/**********************************************************************************************************************
 End of function Sample_DataFlashControl
 *********************************************************************************************************************/
