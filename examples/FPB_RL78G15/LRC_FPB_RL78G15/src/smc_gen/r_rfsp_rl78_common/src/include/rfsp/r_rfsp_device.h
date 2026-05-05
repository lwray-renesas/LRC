/**********************************************************************************************************************
    Program Name    : Renesas Flash Sample Program (RFSP Type01 for RL78/G15,RL78/G16)
    
    File Name       : r_rfsp_device.h
    Program Version : V1.10
    Device(s)       : RL78/G15,RL78/G16 microcontroller
    Description     : Device dependent header file
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

#ifndef R_RFSP_DEVICE_H
#define R_RFSP_DEVICE_H

/* Flash memory control registers (2nd SFR) */
#define R_RFSP_REG_U08_FSSET        (*(volatile __near unsigned char *)0xBEu)
#define R_RFSP_REG_U08_FLPMC        (*(volatile __near unsigned char *)0xC0u)
#define R_RFSP_REG_U08_FLAPL        (*(volatile __near unsigned char *)0xC2u)
#define R_RFSP_REG_U08_FLAPH        (*(volatile __near unsigned char *)0xC3u)
#define R_RFSP_REG_U08_FSSQ         (*(volatile __near unsigned char *)0xC1u)
#define R_RFSP_REG_U08_FLSEDL       (*(volatile __near unsigned char *)0xC4u)
#define R_RFSP_REG_U08_FLSEDH       (*(volatile __near unsigned char *)0xC5u)
#define R_RFSP_REG_U08_FSASTL       (*(volatile __near unsigned char *)0xC6u)
#define R_RFSP_REG_U08_FSASTH       (*(volatile __near unsigned char *)0xC7u)
#define R_RFSP_REG_U08_FLWLL        (*(volatile __near unsigned char *)0xC8u)
#define R_RFSP_REG_U08_FLWLH        (*(volatile __near unsigned char *)0xC9u)
#define R_RFSP_REG_U08_FLWHL        (*(volatile __near unsigned char *)0xCAu)
#define R_RFSP_REG_U08_FLWHH        (*(volatile __near unsigned char *)0xCBu)

/* Internal definitions */
#define R_RFSP_VALUE_U08_FSSQ_WRITE                               (0x81u)
#define R_RFSP_VALUE_U08_FSSQ_ERASE                               (0x84u)
#define R_RFSP_VALUE_U08_FSSQ_CLEAR                               (0x00u)
#define R_RFSP_VALUE_U08_FLPMC_MODE_NONPROGRAMMABLE               (0x08u)
#define R_RFSP_VALUE_U08_FLPMC_MODE_CODE_FLASH_PROGRAMMING        (0x02u)
#define R_RFSP_VALUE_U08_FLPMC_MODE_DATA_FLASH_PROGRAMMING        (0x22u)

#define R_RFSP_VALUE_U08_INIT_VARIABLE                            (0x00u)
#define R_RFSP_VALUE_U08_INIT_FLAG                                (0x00u)
#define R_RFSP_VALUE_U08_SHIFT_8BIT                               (8u)
#define R_RFSP_VALUE_U08_SHIFT_16BIT                              (16u)

#define R_RFSP_VALUE_U08_FREQUENCY_LOWER_LIMIT                    (1u)
#define R_RFSP_VALUE_U08_FREQUENCY_UPPER_LIMIT                    (16u)
#define R_RFSP_VALUE_U08_FREQUENCY_ADJUST                         (1u)
#define R_RFSP_VALUE_U08_CODE_FLASH_BLOCK_ADDR_LOW                (0x00u)
#define R_RFSP_VALUE_U08_CODE_FLASH_BLOCK_ADDR_HIGH               (0x1Fu)
#define R_RFSP_VALUE_U08_CODE_FLASH_BLOCK_ADDR_END_LOW            (0xFCu)
#define R_RFSP_VALUE_U08_CODE_FLASH_BLOCK_ADDR_END_HIGH           (0x03u)
#define R_RFSP_VALUE_U08_CODE_FLASH_SHIFT_HIGH_ADDR               (2u)
#define R_RFSP_VALUE_U08_DATA_FLASH_BLOCK_ADDR_LOW                (0x00u)
#define R_RFSP_VALUE_U08_DATA_FLASH_BLOCK_ADDR_HIGH               (0x01u)
#define R_RFSP_VALUE_U08_DATA_FLASH_BLOCK_ADDR_END_LOW            (0xFCu)
#define R_RFSP_VALUE_U08_DATA_FLASH_BLOCK_ADDR_END_HIGH           (0x01u)
#define R_RFSP_VALUE_U08_DATA_FLASH_SHIFT_HIGH_ADDR               (1u)

#define R_RFSP_VALUE_U08_MASK1_8BIT                               (0xFFu)
#define R_RFSP_VALUE_U16_MASK1_16BIT                              (0xFFFFu)

#define R_RFSP_VALUE_U08_MASK1_FLAPL_ADDR                         (0xFCu)
#define R_RFSP_VALUE_U08_MASK1_FLAPH_ADDR                         (0x1Fu)

#define R_RFSP_VALUE_U08_MASK1_FSASTH_SQEND                       (0x40u)
#define R_RFSP_VALUE_U08_MASK1_FSASTL_ERROR_FLAG                  (0x13u)

#endif /* end of R_RFSP_DEVICE_H */
