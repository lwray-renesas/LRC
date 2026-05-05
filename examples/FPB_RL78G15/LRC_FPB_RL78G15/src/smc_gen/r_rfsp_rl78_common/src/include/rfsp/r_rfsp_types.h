/**********************************************************************************************************************
    Program Name    : Renesas Flash Sample Program (RFSP Type01 for RL78/G15,RL78/G16)
    
    File Name       : r_rfsp_types.h
    Program Version : V1.10
    Device(s)       : RL78/G15,RL78/G16 microcontroller
    Description     : Type definitions header file
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

#ifndef R_RFSP_TYPES_H
#define R_RFSP_TYPES_H

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/

/**
 * @struct bitf08_rfsp_t
 * RFSP bit field definition
 */
typedef struct bitf08_rfsp
{
    unsigned char no0:1;
    unsigned char no1:1;
    unsigned char no2:1;
    unsigned char no3:1;
    unsigned char no4:1;
    unsigned char no5:1;
    unsigned char no6:1;
    unsigned char no7:1;
} bitf08_rfsp_t;

/**
 * @enum e_rfsp_flash_memory_mode_t
 * RFSP flash mode definition
 */
typedef enum e_rfsp_flash_memory_mode
{
    R_RFSP_ENUM_FLASH_MODE_NONPROGRAMMABLE  = 0x08u,
    R_RFSP_ENUM_FLASH_MODE_CODE_PROGRAMMING = 0x02u,
    R_RFSP_ENUM_FLASH_MODE_DATA_PROGRAMMING = 0x22u
} e_rfsp_flash_memory_mode_t;

/**
 * @enum e_rfsp_ret_t
 * RFSP operation status definition
 */
typedef enum e_rfsp_ret
{
    R_RFSP_ENUM_RET_STS_OK              = 0x00u,
    R_RFSP_ENUM_RET_STS_BUSY            = 0x01u,
    R_RFSP_ENUM_RET_ERR_PARAMETER       = 0x10u,
    R_RFSP_ENUM_RET_ERR_MODE_MISMATCHED = 0x11u
} e_rfsp_ret_t;

#endif /* end of R_RFSP_TYPES_H */
