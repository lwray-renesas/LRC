/**********************************************************************************************************************
    Program Name    : Renesas Flash Sample Program (RFSP Type01 for RL78/G15,RL78/G16)
    
    File Name       : r_rfsp_data_flash_api.c
    Program Version : V1.10
    Device(s)       : RL78/G15,RL78/G16 microcontroller
    Description     : Data Flash Control program
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
#include "r_rfsp_data_flash_api.h"

/**********************************************************************************************************************
 * Function name : R_RFSP_EraseDataFlashReq
 *********************************************************************************************************************/
/*********************************************************************************************************************/
/**
 *  This command starts the code/data flash memory sequencer.
 *  Start the code/data flash memory sequencer and erase the specified block in the data flash memory.
 *  
 *  @param[in]      i_u08_block_number : 
 *                    Erase block number
 *  @return         ---
 */
/*********************************************************************************************************************/
R_RFSP_FAR_FUNC void R_RFSP_EraseDataFlashReq(uint8_t i_u08_block_number)
{
    /* Local variable definitions */
    uint8_t  l_u08_low_addr;
    uint8_t  l_u08_high_addr;
    
    /* Calculate and set local variables */
    l_u08_low_addr  = i_u08_block_number & R_RFSP_VALUE_U08_DATA_FLASH_BLOCK_ADDR_LOW;
    l_u08_high_addr = (uint8_t)((i_u08_block_number & R_RFSP_VALUE_U08_DATA_FLASH_BLOCK_ADDR_HIGH) 
                      << R_RFSP_VALUE_U08_DATA_FLASH_SHIFT_HIGH_ADDR);

    /* Set the value for FLAPH/L register */
    R_RFSP_REG_U08_FLAPL = l_u08_low_addr;
    R_RFSP_REG_U08_FLAPH = l_u08_high_addr;

    /* Set the value for FLSEDH/L register */
    R_RFSP_REG_U08_FLSEDL = l_u08_low_addr  | R_RFSP_VALUE_U08_DATA_FLASH_BLOCK_ADDR_END_LOW;
    R_RFSP_REG_U08_FLSEDH = l_u08_high_addr | R_RFSP_VALUE_U08_DATA_FLASH_BLOCK_ADDR_END_HIGH;

    /* Set the value for FSSQ register */
    R_RFSP_REG_U08_FSSQ = R_RFSP_VALUE_U08_FSSQ_ERASE;
}

/**********************************************************************************************************************
 End of function R_RFSP_EraseDataFlashReq
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : R_RFSP_WriteDataFlashReq
 *********************************************************************************************************************/
/*********************************************************************************************************************/
/**
 *  This command starts the code/data flash memory sequencer.
 *  Start the code/data flash memory sequencer and write the data (4 byte) to the specified address
 *  in the data flash memory.
 *  
 *  @param[in]      i_u16_start_addr : 
 *                    Write start address
 *  @param[in]      inp_u08_write_data : 
 *                    Write data pointer
 *  @return         ---
 */
/*********************************************************************************************************************/
R_RFSP_FAR_FUNC void R_RFSP_WriteDataFlashReq(uint16_t i_u16_start_addr, 
                                            uint8_t __near * inp_u08_write_data)
{
    /* Local variable definitions */
    
    /* Set the value for FLAPH/L register */
    R_RFSP_REG_U08_FLAPL = (uint8_t)(i_u16_start_addr & R_RFSP_VALUE_U08_MASK1_8BIT);
    R_RFSP_REG_U08_FLAPH = (uint8_t)((i_u16_start_addr >> R_RFSP_VALUE_U08_SHIFT_8BIT) & R_RFSP_VALUE_U08_MASK1_8BIT);
    
    /* Set the value for FLWHH/HL/LH/LL register */        
    R_RFSP_REG_U08_FLWLL = (uint8_t)(* inp_u08_write_data);
    R_RFSP_REG_U08_FLWLH = (uint8_t)(*(inp_u08_write_data + 0x01));
    R_RFSP_REG_U08_FLWHL = (uint8_t)(*(inp_u08_write_data + 0x02));
    R_RFSP_REG_U08_FLWHH = (uint8_t)(*(inp_u08_write_data + 0x03));

    /* Set the value for FSSQ register */
    R_RFSP_REG_U08_FSSQ = R_RFSP_VALUE_U08_FSSQ_WRITE;
}

/**********************************************************************************************************************
 End of function R_RFSP_WriteDataFlashReq
 *********************************************************************************************************************/
